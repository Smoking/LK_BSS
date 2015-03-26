#include "stdafx.h"
#include "BIZBoxService.h"
#include "ConfigUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LoggerPtr CBIZBoxService::rootLogger = Logger::getLogger("CBIZBoxService");

CBIZBoxService::CBIZBoxService():CBIZBaseService()
{
	try
	{	
		rootLogger->info("construct CBIBoxService");
	}
	catch (...) 
	{
		rootLogger->fatal("CBIZBoxService--construct CBIBoxService error ");
	}
}

CBIZBoxService::~CBIZBoxService()
{
	try
	{
		rootLogger->info("destroy CBIBoxService");
	}
	catch (...) 
	{
		rootLogger->fatal("CBIZBoxService--destroy CBIBoxService error ");
	}
}


int CBIZBoxService::distributing(CWorkRec workRec,CDBManage* cdb)
{
	try{	
	////rootLogger->trace("function distributing begin...................");	
	int errorCode;
	int preId = workRec.getPreId();
	int splitId = workRec.getSplitId();
	//判断是否废弃处方
	//由于一人多方的处理,废弃处方也发出方便叫号
//	if(isDelete(preId,cdb)!=0 && splitId != 0)
//	{
//		rootLogger->info("this prescription has been deleted!");
//		return PRESCRIPTION_DELETE;
//	}
	//获取处方药单
	rootLogger->info("begin distribute prescription id = "+Utils::itos(preId));
	list<CPrescriptionMedicine> listMedicine;
	errorCode = this->getMedicineList( preId ,splitId ,listMedicine,cdb);
	if(errorCode != 0)
	{
		rootLogger->error("failed to get medicine List !!");
		return errorCode;
	}
	errorCode = this->checkStorage(&listMedicine,cdb,splitId);
	if (errorCode != 0 && errorCode != SPLIT_PRESCRIPTION && errorCode != IGNORE_PRESCRIPTION)
	{
		rootLogger->error("checkStorage error. errorCode = "+Utils::itos(errorCode));
		return errorCode;
	}
	else if (errorCode == SPLIT_PRESCRIPTION)
	{
		rootLogger->info("too many medicines .try to split prescription.....");
		errorCode = splitPrescription(preId,&listMedicine,cdb);
		if (errorCode != 0)
		{
			rootLogger->error("split prescription error!");
			return errorCode;
		}
		this->allocateWindows(preId,splitId,cdb,-1);
		return SPLIT_PRESCRIPTION;
	}
	else if (errorCode == IGNORE_PRESCRIPTION)
	{
		rootLogger->info("too many medicines . ignore this prescription");
		string sql = "update prescription set ignore_flag = 1 where pre_id ="+Utils::itos(preId);
		
		errorCode = cdb->execSQL(sql);
		if (errorCode !=0)
		{
			rootLogger->error("sql = "+sql);
			return errorCode;
		}
		listMedicine.clear();
		//当做空处方发
	}
	
	list<MedicineInfo> medList;
	map<int,MedicinLocationState> medLocMap;
	list<CPrescriptionMedicine>::iterator ptr;
	MedicinSort medicineSort;
	LocationStatistic locationStatistic;
	map<int,MedicinSort> msMap;
	map<int,LocationStatistic>lsMap;
	m_addMedflag = 0;
	errorCode = this->remapLocation(listMedicine,msMap,lsMap,medList,medLocMap,cdb);
	if (errorCode != 0)
	{
		rootLogger->error("remapLocation error ! errorCode = "+Utils::itos(errorCode));
		return errorCode;
	}
	long bFlag = 0;
	int sleepCount = 1000;
	while(1)
	{	
		errorCode = fxcpu->GetSendBasketFinishInfo(&bFlag);
		if(errorCode != 0)
		{
			rootLogger->error("获取PLC送走篮子完成信号失败 错误码 = "+Utils::itos(errorCode));
			Sleep(100);
			continue;
			//return errorCode;
		}
		if(bFlag == 1)
		{
			Sleep(50);
            rootLogger->info("获取PLC送走篮子完成信号成功");
			break;
		}
		Sleep(50);
		sleepCount--;
		if(sleepCount==0)
		{
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,ER_PLC_SENDBASKETFINISH) ;
		}
	}
	long DFlag = 0;
	errorCode = fxcpu->GetFinishInfo(&DFlag);
	if(DFlag !=0)
        rootLogger->info("皮带没有停止");
	else
		rootLogger->info("皮带已经停止。。。。。。。");
	errorCode = fxcpu->SetIniSendBasketFinish(0);
	if(errorCode != 0)
	{
		rootLogger->error("复位送走篮子完成信号失败 错误码 = "+Utils::itos(errorCode));
		return errorCode;
	}
	else
		rootLogger->info("复位送走篮子完成信号成功");
	errorCode = fxcpu->GetSendBasketFinishInfo(&bFlag);
	if(bFlag != 0)
		rootLogger->info("给M11赋值为0 失败。。。。");
	string distAddress=Utils::itos(preId)+" : ";
	int log_cnt=0;
	medList.sort();
	if(medList.empty())
		sendMedFlag = -1;
	for (list<MedicineInfo>::iterator mmptr = medList.begin();mmptr != medList.end();mmptr++)
	{
		if(sendMedFlag < mmptr->position)
			sendMedFlag = mmptr->position ;
		log_cnt+=mmptr->count;        
		distAddress+= Utils::itos(mmptr->position)+"-"+Utils::itos(mmptr->count)+"; ";
	}	
	distAddress="countLocation-"+Utils::itos(log_cnt)+"."+ distAddress;	
	Utils::insertSystemlog("1809","发药地址",distAddress);	
	
	errorCode =pBoxSinglechip->distributeMed(medList);
	if(errorCode != 0)
	{
		rootLogger->error("distributeMedicine error preId = "+Utils::itos(preId));
		return errorCode;
	}	
	medManage->popMedicinLocation(medLocMap);
	rootLogger->info("distributeMedicine succeed preId = "+Utils::itos(preId));
	if(!lsMap.empty())	
		cStatistic->addLocationStatistic(lsMap);
	if(!msMap.empty())
		cStatistic->addMedicinSortStatistic(msMap);
	cStatistic->addSumPrescriptionCount(1);
	
	string basketId = this->getBasketId(cdb);
	if (basketId == "")
	{
		rootLogger->error("get basket Id error ,preId = "+Utils::itos(preId));
		return READ_BASKET_ERROR;
	}	
	//绑定药篮编号
    errorCode=this->bindBasket(basketId,preId,splitId,cdb);
	if(errorCode != 0)
	{
		rootLogger->error("bindBasket error preId = "+Utils::itos(preId));
		return errorCode;
	}

	errorCode=this->allocateWindows(preId,splitId,cdb,atoi(basketId.c_str()));
	if(errorCode != 0)
	{
		rootLogger->error("allocate Windowse error preId = "+Utils::itos(preId));
		return errorCode;
	}	
	////rootLogger->trace("function distributing end........................");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBoxService--distributing error ");
		return 1;
	}
}

void CBIZBoxService::ThreadEntryPoint()
{
	int errorCode;
	string sSQL; 
	CDBManage* cdb = new CDBManage();
	this->updateWindow(cdb);
	while(true)
	{
		try{		
		Sleep(sleepTime);
		if (pauseScanFlag == 1)
		{
			continue;
		}
		//获取未提取队列
		this->getNewQueues(cdb);
		if (pBoxMonitor->getKepFlag())
		{
			continue;
		}
		if(pBoxMonitor->getStopFlag())
		{
			rootLogger->fatal("Device Fatal error pause working ");
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,MACHINE_FETAL_ERROR);
			pauseDist();
			continue;
		}
		int machineState=-1;
		if(0==medManage->getMachineState(Utils::itos(m_machineId),machineState) && machineState==0)
		{
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,MACHINE_DISABLE);
			pauseDist();
			continue;
		}
		WorkQueue::iterator ptr= normalQueue.begin();
		MedicineInfoList errorAddress;
		CWorkRec rec;
        int res = pBoxSinglechip->isDstrOver(errorAddress);
		if( !normalQueue.empty()  && res==0  )
		{
			if (dealFlag)
			{
				if(!lastDealQueue.empty())
				{
					deleteFromDB((*(lastDealQueue.begin())).getPreId(),cdb,(*(lastDealQueue.begin())).getSplitId());
				    lastDealQueue.pop_front();
				}
			}
			else
			{
				dealFlag = true;
			}
			
			rec = *ptr;
            if( sendMedFlag != 0)
			{
				long lastLocationId = pBoxSinglechip->getLastLocationID();
				Sleep(lastLocationId*100);
				long bFlag ;
				errorCode = fxcpu->GetSendBasketFinishInfo(&bFlag);
				if(bFlag != 0)
		            rootLogger->info("PLC中途给M11至1。。。。。。。。");
				errorCode = fxcpu->SetStartInsideBelt(sendMedFlag);	
				if(errorCode != 0)
				{
					rootLogger->error("内部传送带启动信号失败 错误码 = "+Utils::itos(errorCode));
				}
				else
                    rootLogger->info("启动内部传送带启动信号............");
				sendMedFlag = 0;
			}			
			
			errorCode = this->distributing(rec,cdb);	
			if(errorCode == 0)
			{
				lastDealQueue.push_back(rec);
				normalQueue.pop_front();			
				if (rec.getSplitId() == 0)
				{
					::SendMessage(hwnd,WM_REFRESH,rec.getPreId(),2);  //发消息至界面,删除队列数据
				}
				else
				{
					::SendMessage(hwnd,WM_REFRESH,rec.getSplitId(),10);  //发消息至界面,删除队列数据
				}
			}
			else if (errorCode == SPLIT_PRESCRIPTION)
			{
				rootLogger->info("split preId = "+Utils::itos(rec.getPreId()));
				lastDealQueue.push_back(rec);	
				if (rec.getSplitId() == 0)
				{
					::SendMessage(hwnd,WM_REFRESH,rec.getPreId(),2);  //发消息至界面,删除队列数据
				}
				else
				{
					::SendMessage(hwnd,WM_REFRESH,rec.getSplitId(),10);  //发消息至界面,删除队列数据
				}
			}
			else
			{
				dealFlag = false;
				ptr->setErrorCode(errorCode);   //
				ptr->setPreState(3);        //异常
				sSQL = "update work_queue t set t.error_code = "+Utils::itos(errorCode);
				if (rec.getSplitId() != 0)
				{
					sSQL += " , t.pre_state=3 where split_id = "+Utils::itos(rec.getSplitId());
					sSQL += "  and pre_id = "+Utils::itos(rec.getPreId());
				}else
				{
					sSQL += " , t.pre_state=3 where pre_id = "+Utils::itos(rec.getPreId());
					sSQL += "  and split_id = 0";
				}			
				rootLogger->info("sql = "+sSQL);
				cdb->execSQL(sSQL);
				abnormalQueue.push_back(rec);
				if(!normalQueue.empty())
				    normalQueue.pop_front();
				if (rec.getSplitId() == 0)
				{
					::PostMessage(hwnd,WM_REFRESH,rec.getPreId(),5);  //发消息至界面,新增队列数据
					::SendMessage(hwnd,WM_REFRESH,rec.getPreId(),2);  //发消息至界面,删除队列数据
				}else
				{
					::PostMessage(hwnd,WM_REFRESH,rec.getSplitId(),9);  //发消息至界面,新增队列数据
					::SendMessage(hwnd,WM_REFRESH,rec.getSplitId(),10);  //发消息至界面,删除队列数据
				}
				::PostMessage(hwnd,WM_MONITORMESSAGE,0,errorCode);//2013-11-15
			}
		}
		else if (res >1)
		{
			int preId = rec.getPreId(); //2013-11-18
			int splitId = rec.getSplitId();//2013-11-18
			int dealId = splitId==0 ? preId : splitId;
			
			if ( splitId != 0)
			{
				updatePrescriptionState(dealId,NEED_READDING,cdb,1);
			}else
			{
				updatePrescriptionState(dealId,NEED_READDING,cdb);
			}			        
			//处理出错地址
			list<MedicineInfo>::iterator iter;
			string strErrorAddress;
			for(iter = errorAddress.begin(); iter != errorAddress.end(); iter++)
			{
				MedicineInfo temp = *iter;
				if(temp.errNo == 5)
				{
					updatePrescriptionMedicineListState(preId,temp.medId,5,cdb,splitId,temp.count);
				}
				else
				{
					updatePrescriptionMedicineListState(preId,temp.medId,4,cdb,splitId,temp.count);
					map<int,MedicinLocationState> medLocMap;
					MedicinLocationState mls ;
					mls.location_id = temp.position;
					mls.cur_quantity = 99;
					medLocMap[temp.position] = mls;
					medManage->popMedicinLocation(medLocMap);
				}
				strErrorAddress += " 出错地址:" +Utils::itos(temp.position)+"错误码:"+Utils::itos(temp.errNo)+";" ;
				::PostMessage(hwnd,WM_ERRORADDRESS,temp.position ,0);				
			}
            rootLogger->error(strErrorAddress);

			string basketId = this->getBasketId(cdb);
			if (basketId == "")
			{
				rootLogger->error("get basket Id error ,preId = "+Utils::itos(preId));
			}	
			//绑定药篮编号
			errorCode=this->bindBasket(basketId,preId,splitId,cdb);
			if(errorCode != 0)
			{
				rootLogger->error("bindBasket error preId = "+Utils::itos(preId));
			}
			m_addMedflag = 1;
			errorCode=this->allocateWindows(preId,splitId,cdb,atoi(basketId.c_str()));
			if(errorCode != 0)
			{
				rootLogger->error("allocate Windowse error preId = "+Utils::itos(preId));
			}
			
			errorCode = Utils::insertSystemlog("1801","发药出错",strErrorAddress);
			if (errorCode !=0)
			{
				rootLogger->error("insertSystemLog error!");
			}
			errorCode =pBoxSinglechip->setDistributeMedicineFlag(0);
			if (errorCode !=0)
			{
				rootLogger->error("setDistributeMedicineFlag error!");
			}
			if( sendMedFlag != 0 )
			{
				long lastLocationId = pBoxSinglechip->getLastLocationID();
				Sleep(lastLocationId*100);
				long bFlag ;
				errorCode = fxcpu->GetSendBasketFinishInfo(&bFlag);
				if(bFlag != 0)
		            rootLogger->info("PLC中途给M11至1。。。。。。。。");
				errorCode = fxcpu->SetStartInsideBelt(sendMedFlag);	
				if(errorCode != 0)
				{
					rootLogger->error("内部传送带启动信号失败 错误码 = "+Utils::itos(errorCode));
				}
				else
                    rootLogger->info("发药数量出错，启动内部传送带启动信号............");
				sendMedFlag = 0;
			}
		}
		else if (normalQueue.empty() && res==0)
		{
			rootLogger->debug("condition: empty && over");
			if (dealFlag)
			{
				if(!lastDealQueue.empty())
				{
					deleteFromDB((*(lastDealQueue.begin())).getPreId(),cdb,(*(lastDealQueue.begin())).getSplitId());
					lastDealQueue.pop_front();
				}
				dealFlag = false;
			}

			if( sendMedFlag != 0)
			{
				long lastLocationId = pBoxSinglechip->getLastLocationID();
				Sleep(lastLocationId*100);
				long bFlag ;
				errorCode = fxcpu->GetSendBasketFinishInfo(&bFlag);
				if(bFlag != 0)
		            rootLogger->info("PLC中途给M11至1。。。。。。。。");
				errorCode = fxcpu->SetStartInsideBelt(sendMedFlag);	
				if(errorCode != 0)
				{
					rootLogger->error("内部传送带启动信号失败 错误码 = "+Utils::itos(errorCode));
				}
				else
                    rootLogger->info("启动内部传送带启动信号............");
				sendMedFlag = 0;
			}	
			string sql = "UPDATE work_queue SET pre_state =0,fetch_state=0";
			cdb->execSQL(sql);
		}
		}catch (...) {
			rootLogger->fatal("CBIZBoxService--Scan error ");
			continue;
		}
	}
	cdb->close();
	delete cdb;
}


int CBIZBoxService::bindBasket(string basketId,int preId,int splitId,CDBManage* cdb)
{
	try{
	
	//rootLogger->trace("function bindBasket begin..................");

	if (basketId =="" ||basketId == " ") 
		return 1;

	string  sql ="update basket_mapping t1,prescription t2 set t1.pre_id="+Utils::itos(preId);
	sql+=",t1.split_id ="+Utils::itos(splitId);
	sql+=",t1.fetch_window =t2.fetch_window ,t1.pre_his_id =t2.pre_his_id where t1.basket_id ="+basketId+" and t2.pre_id="+Utils::itos(preId);

	int rs = cdb->execSQL(sql);
	if (rs != 0)
	{
		rootLogger->error("bindBasket error!");
		return rs;
	}
	if (splitId != 0)
	{
		sql = "update prescription_split set basket_id ='"+basketId+"' where split_id = ";
		sql+= Utils::itos(splitId);
	}else
	{
		sql = "update prescription set basket_id = '";
		sql += basketId;
		sql += "' where pre_id = ";
		sql += Utils::itos(preId);
	}
	
	rootLogger->info("sql = "+sql);
    rs = cdb->execSQL(sql);
	
	//rootLogger->trace("function bindBasket end.............................");

	return rs;
	}catch (...) {
		/// AfxMessageBox("CBIZBoxService--bindBasket error.");
		rootLogger->fatal("CBIZBoxService--bindBasket error ");
		return -1;
	}
}


int CBIZBoxService::remapLocation(list<CPrescriptionMedicine> listMedicine, 
						map<int,MedicinSort>& msMap,
						map<int,LocationStatistic>& lsMap,
						list<MedicineInfo>& medList,
						map<int,MedicinLocationState>& medLocMap,
						CDBManage* cdb)
{
	try{
	
	//rootLogger->trace("function remapLocation begin..................");
	list<CPrescriptionMedicine>::iterator ptr;
	MedicinLocationState medLoc;
	MedicinSort medicineSort;
	LocationStatistic locationStatistic;
	MedicineInfo medInfo;
	int errorCode = 0;

	for (ptr=listMedicine.begin();ptr != listMedicine.end(); ++ptr)
	{
		if(ptr->getDistState() > 0)
			m_addMedflag = 1;
		if (ptr->getDistState() > 1)
		{
			//需要去补药的跳过
			continue;
		}
		int medId = (*ptr).getMedicineId();
		int num = 0;
		if(ptr->getCurSendMedNum() > 0 &&  ptr->getDistState()==1)
		{
			num = (*ptr).getCurSendMedNum();
		}
		else
		{
			num = (*ptr).getMedicineNum();
		}
		medicineSort.count = (*ptr).getMedicineNum();
		medicineSort.medName = (*ptr).getMedicineName();
		medicineSort.medType = (*ptr).getMedicineType();
		msMap.insert(pair<int,MedicinSort>(medId,medicineSort));
	
		//获取地址列表
		list<CLocation> listLocation ;
		errorCode = this->getLocationList(medId,&listLocation);
		if(errorCode != 0)
		{
			rootLogger->error("failed to get location list !!");
			return errorCode;
		}
		vector<list<CLocation> > vList(listLocation.size());
		vector<int> vCapacity(listLocation.size());
		vector<int> expNum(listLocation.size());
		list<CLocation>::iterator ltr;
		int curLayer = 0;
		int LayerNum =0;
		int locationNum = 0;
		for (ltr=listLocation.begin();ltr != listLocation.end(); ++ltr)
		{
			CLocation locTmp = CLocation(*ltr);
			if(locTmp.getCurQuantity()==0)
				continue ;
			if (curLayer != locTmp.getLayerNum())
			{
				if(curLayer != 0)
				{
					LayerNum++;
				}
				curLayer = locTmp.getLayerNum();
			}
			vList[LayerNum].push_back(locTmp);
			vCapacity[LayerNum] += locTmp.getCurQuantity();
			locationNum++;
		}
		if(locationNum==0)
			continue;
		LayerNum++;
		//组内降序
		for(int i = 0 ; i < LayerNum ; i++)
		{
			vList[i].sort();
			vList[i].reverse();
		}
		for( i = 0 ; i < LayerNum ; i++)
		{
			for(int j = i+1 ; j < LayerNum ; j++)
			{
				if(vCapacity[i] > vCapacity[j])
				{
					int tmp = vCapacity[i];
					vCapacity[i] = vCapacity[j];
					vCapacity[j] = tmp;
					list<CLocation> tmpList = vList[i];
					vList[i]=vList[j];
					vList[j] = tmpList;
				}
			}

		}
		int tempLayerNum = LayerNum;
		while( num>0 && tempLayerNum>0)
		{
			int averageNum = num/tempLayerNum;
			if(averageNum == 0 )
			{
				averageNum = 1 ;
			}			
			for( i = 0 ; i < LayerNum ; i++)
			{
				if(num < averageNum)
					break ;
				if( vCapacity[i] >= averageNum )
				{
					expNum[i] += averageNum ;
					num -= averageNum;
					vCapacity[i] -= averageNum;
					if(vCapacity[i] == 0)
						tempLayerNum--;
				}
				else if( vCapacity[i] >0 )
				{
					expNum[i] += vCapacity[i] ;
					num -= vCapacity[i];
					vCapacity[i] = 0 ;
					tempLayerNum--;
				}
			}
		}

		for( i = 0 ; i < LayerNum ; i++)
		{	
			for (ltr=vList[i].begin();ltr != vList[i].end();ltr++)
			{
				int curQuan = (*ltr).getCurQuantity();
				if (expNum[i] == 0)
				{
					break;
				}
				if ( curQuan < expNum[i])
				{
					expNum[i] -= curQuan;
					//发药队列
					medInfo.count = curQuan;
					medInfo.position = (*ltr).getLocationId();
					medInfo.medId = medId;
					if (medInfo.count != 0)
					{
						//合并相同地址的发药
						this->pushBackInMedList(medList,medInfo);
						rootLogger->debug("location id = "+ Utils::itos((*ltr).getLocationId()) + " distribute medicines  num = "+Utils::itos(curQuan));
						//写回数据库
						medLoc.location_id=ltr->getLocationId();
						medLoc.cur_quantity = curQuan;
						medLocMap.insert(pair<int,MedicinLocationState>(medLoc.location_id,medLoc));

						locationStatistic.count = curQuan;
						locationStatistic.locationType = Statistic_Location_2;
						lsMap.insert(pair<int,LocationStatistic>(medInfo.position,locationStatistic));
					}			
				}
				else
				{	
					//发药队列
					medInfo.count = expNum[i];
					medInfo.position = (*ltr).getLocationId();
					medInfo.medId = medId;
					if (medInfo.count != 0)
					{
						//合并相同地址的发药
						this->pushBackInMedList(medList,medInfo);	
						rootLogger->debug("location id = "+Utils::itos((*ltr).getLocationId())+" distribute medicines  num = "+Utils::itos(expNum[i]));
						//写回数据库
						medLoc.location_id=ltr->getLocationId();
						medLoc.cur_quantity = expNum[i];
						medLocMap.insert(pair<int,MedicinLocationState>(medLoc.location_id,medLoc));

						locationStatistic.count = expNum[i];
						locationStatistic.locationType = Statistic_Location_2;
						lsMap.insert(pair<int,LocationStatistic>(medInfo.position,locationStatistic));
					}
					expNum[i] = 0;
					break;
				}
			}
		}
	}
	//rootLogger->trace("function remapLocation end..................");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBoxService--remapLocation error ");
		return -1;
	}
}

int CBIZBoxService::allocateWindows(int preId,int splitId,CDBManage* cdb, int basketNo)
{
	//rootLogger->trace("function allocateWindows begin..................");
	int windowNo=0;
	int result = 0;
	string hisMainId="";
	string sql="";
	if (splitId!=0)
	{
		if(sendMedFlag != 0 && basketNo != -1)
		{
			sql = "select fetch_window  from prescription_split where pre_id = "+Utils::itos(preId);
			sql = sql +" and split_id = "+ Utils::itos(splitId);
			result = cdb->execSQL(sql);
			if (result != 0)
			{
				rootLogger->info("sql = "+sql);
				rootLogger->error("allocateWindows error1");
				return result;
			}
			windowNo = atoi(cdb->getFieldValueByIndex(0).c_str());
			if(windowNo == -1)
			{
				rootLogger->error("allocateWindows error1 windowNo = -1");
				return -1;
			}
			result = cPortManage1->writeCard(windowNo,basketNo,m_addMedflag);
			if (result != 0)
			{
				rootLogger->error("cPortManage1 writeCard error");
				return result;
			}
			result = cPortManage2->writeCard(windowNo,basketNo,m_addMedflag);
			if (result != 0)
			{
				rootLogger->error("cPortManage2 writeCard error");
				return result;
			}
		}
		rootLogger->info("split prescription no need to allocate windows");
		return 0;
	}
	else
	{		
		sql = "select his_main_id,fetch_window from prescription where pre_id ="+Utils::itos(preId);	
		result = cdb->execSQL(sql);
		if (result != 0)
		{
			rootLogger->info("sql = "+sql);
			rootLogger->error("allocateWindows error1");
			return result;
		}
		if (cdb->getRowNum() != 0)
		{
			windowNo = atoi(cdb->getFieldValueByIndex(1).c_str());
			hisMainId = cdb->getFieldValueByIndex(0);
			rootLogger->info("windowNo = "+Utils::itos(windowNo));
			cdb->freeRecord();
		}
		else
		{
			rootLogger->error("allocateWindows error2");
			return NO_FIT_PRESCRIPTION;
		}	
		if (windowNo == -1)
		{
			if (hisMainId!="")
			{
				sql = "select fetch_window from prescription where his_main_id = '"+hisMainId+"' and fetch_window <>-1 ";
				
				result = cdb->execSQL(sql);
				if (result != 0)
				{
					rootLogger->info("sql = "+sql);
					rootLogger->error("allocateWindows error3");
					return result;
				}
				if (cdb->getRowNum() != 0)
				{
					windowNo = atoi(cdb->getFieldValueByIndex(0).c_str());
				}
				cdb->freeRecord();
			}
			if (windowNo == -1)
			{
				CTime t=CTime::GetCurrentTime(); 
				
				CTimeSpan t1(1,0,0,0);
				string yestodayTime = (t-t1).Format("%Y-%m-%d %H:%M:%S"); 

				
				sql = "select fetch_window from prescription where patient_id = (select patient_id from prescription where pre_id = "+Utils::itos(preId)+" ) and fetch_window <> -1 ";
				sql += " and create_time > '"+yestodayTime+"';";
				
				result = cdb->execSQL(sql);
				if (result != 0)
				{
					rootLogger->info("sql = "+sql);
					rootLogger->error("allocateWindows error3");
					return result;
				}
				if (cdb->getRowNum() != 0)
				{
					windowNo = atoi(cdb->getFieldValueByIndex(0).c_str());
				}
				cdb->freeRecord();
				
			}
			if (windowNo == -1)
			{
				result = this->getWindowNo(preId,splitId,windowNo,cdb);
				if (result != 0)
				{
					rootLogger->error("allocateWindows error4");
					return result;
				}
			}
		}
		
		if(sendMedFlag != 0 && basketNo != -1)
		{
			result = cPortManage1->writeCard(windowNo,basketNo,m_addMedflag);
			if (result != 0)
			{
				rootLogger->error("cPortManage1 writeCard error");
				return result;
			}
			result = cPortManage2->writeCard(windowNo,basketNo,m_addMedflag);
			if (result != 0)
			{
				rootLogger->error("cPortManage2 writeCard error");
				return result;
			}
		}

		result = this->updateWindowBasketCount(1,windowNo,cdb);
		if (result != 0)
		{
			rootLogger->error("allocateWindows error6");
			return result;
		}	
		//rootLogger->trace("function allocateWindows end..................");
		return 0;
	}
}


int CBIZBoxService::updateWindowBasketCount(int num ,int windowNo,CDBManage* cdb)
{
	try{
	
	//rootLogger->trace("function updateWindowBasketCount begin...................");
	string sql ="update windows set use_basket = (use_basket";
	if (num>=0)
	{
		sql+="+";
	}
	sql+=Utils::itos(num)+") where window_no = ";
	sql+=Utils::itos(windowNo);
	rootLogger->info("sql = "+sql);
	
	int result = cdb->execSQL(sql);
	if (result != 0)
	{
		rootLogger->error("update window basketCount error!");
	}
	//rootLogger->trace("function updateWindowBasketCount end.....................");
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBoxService--updateWindowBasketCount error ");
		return -1;
	}
}

int CBIZBoxService::getWindowNo(int preId,int splitId,int& windowNo,CDBManage* cdb){
	try{
	
	//rootLogger->trace("function getWindowNo begin...............");
	string sql = "select window_no from windows where window_state = 1 and window_no <> 0 order by use_basket; ";
	rootLogger->info("sql = "+sql);
	int result = cdb->execSQL(sql);
	if (result != 0)
	{
		rootLogger->error("get getWindowNo error");
		return result;
	}
	if (!cdb->isEnd())
	{
		windowNo = atoi(cdb->getFieldValueByIndex(0).c_str());
	}else{
		windowNo=1;
	}
	cdb->freeRecord();

	sql = "update prescription set fetch_window = "+Utils::itos(windowNo)+" where pre_id = "+Utils::itos(preId);	
	result = cdb->execSQL(sql);
	if (result != 0)
	{
		rootLogger->info("sql = "+sql);
		return result;
	}
	if(splitId!=0)
	{
		sql = "update prescription_split set fetch_window = "+Utils::itos(windowNo)+" where pre_id = "+Utils::itos(preId);	
		result = cdb->execSQL(sql);
		if (result != 0)
		{
			rootLogger->info("sql = "+sql);
			return result;
		}
	}
	
	//rootLogger->trace("function getWindowNo end...............");
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBoxService--getWindowNo error ");
		return -1;
	}
}

