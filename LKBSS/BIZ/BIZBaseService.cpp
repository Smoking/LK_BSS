#include "stdafx.h"
#include "BIZBaseService.h"
#include "ConfigUtil.h"
#include "Utils.h"
#include <cstdlib>
#include <iterator>
#include "PrescriptionMedicine.h"
#include <algorithm>
#include "MedicineManage.h"
#include "../resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LoggerPtr CBIZBaseService::rootLogger = Logger::getLogger("CBIZBaseService");

unsigned  CBIZBaseService::uiThread1ID = 0;

WorkQueue CBIZBaseService::normalQueue ;
WorkQueue CBIZBaseService::abnormalQueue;
WorkQueue CBIZBaseService::lastDealQueue;
WorkQueue CBIZBaseService::printQueue;

CBIZBaseService::CBIZBaseService(){
	try{
		//初始化日志记录器
		string msg = configUtil.getValue("logPropFile");
		PropertyConfigurator::configure(msg);

		//初始化扫描频率
		string frequencyTime = configUtil.getValue("DealFrequency");

		if (frequencyTime.empty())
		{
			rootLogger->error(" DealFrequency is empty .......... ");
			return;
		}
		runMode = configUtil.getInt("runMode");
		sleepTime = atoi(frequencyTime.c_str());
		dealFlag = false;
		//主线程名称,用于识别
		threadName = "mainThread";

		fxcpu = singletonFxPLC.getInst();

		pBoxSinglechip=singletonBoxSinglechip.getInst();
		

		sendMedFlag = 0;
		
		cStatistic = singletonStatistic.getInst();
		cStatistic->initialize();
		cStatistic->addSumLogCount(1);
		cStatistic->startDeviceRunTime();

		cpm = new CPrescriptionManager();
        
		//读卡器
		PortPara para1;
		para1.port = configUtil.getInt("cardReaderPort1");;
		para1.baudRate = configUtil.getInt("cardReaderRate1");
		cPortManage1 = new CCardManage();
		long lRet = cPortManage1->open(para1);
		if(lRet>0)
		{
			AfxMessageBox("打开打卡器1失败");
		}
		
		PortPara para2;
		para2.port = configUtil.getInt("cardReaderPort2");
		para2.baudRate = configUtil.getInt("cardReaderRate2");
		cPortManage2 = new CCardManage();
		lRet = cPortManage2->open(para2);
		if(lRet>0)
		{
			AfxMessageBox("打开打卡器2失败");
		}

		medManage = new CMedicineManage();
		keepRec = configUtil.getInt("keepRec");
		m_machineId=configUtil.getInt("MachineID");
		splitMax = configUtil.getInt("SplitMax");
		pauseScanFlag = 0;
		ControlMode = configUtil.getInt("ControlMode");
		enableFinishFlag = configUtil.getInt("EnableFinishFlag");
		m_machineType = configUtil.getInt("MachineType");
		m_addMedflag = 0;
	}catch (...) {
		/// AfxMessageBox("CBIZBaseService construct error.");
		rootLogger->fatal("CBIZBaseService construct  ");
	}
}

void CBIZBaseService::setHwnd(HWND hwnd){
	this->hwnd = hwnd;
	pBoxSinglechip->hwnd = this->hwnd;
}
CBIZBaseService::~CBIZBaseService()
{
	try{
		
		if (fxcpu != NULL)
		{
			singletonFxPLC.releaseInst(fxcpu);
		}
        
		if( pBoxSinglechip != NULL)
		{
			singletonBoxSinglechip.releaseInst(pBoxSinglechip);
		}
		
		cStatistic->saveStatistic();
		if(cStatistic!= NULL)
		{
			singletonStatistic.releaseInst(cStatistic) ;
			cStatistic = NULL;
		}
		if (cpm != NULL)
		{	
			delete cpm;
			cpm = NULL ;
		}
		if (medManage != NULL)
		{
			delete medManage;
			medManage = NULL;
		}
		if (cPortManage1 != NULL)
		{
			cPortManage1->close();
			delete cPortManage1;
			cPortManage1 = NULL;
		}
		if (cPortManage2 != NULL)
		{
			cPortManage2->close();
			delete cPortManage2;
			cPortManage2 = NULL;
		}

	}catch (...) {
		rootLogger->fatal("CBIZBaseService Deconstruct  ");
	}
}

int CBIZBaseService::beginScan()
{
	try
	{
		if (uiThread1ID != 0)
		{
			rootLogger->info("Tread is already exist,begin to consume it....  uiThread1ID = "+ Utils::itos(uiThread1ID));
			this->resumeScan();
			return 0;
		}
		//初始化队列
		int errorCode;

		CDBManage* cdb = new CDBManage();
		errorCode = this->initQueues(cdb);
		delete cdb;
		if(errorCode)
		{
			rootLogger->error("failed to init work queues");
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,errorCode); //通知界面
			return errorCode;
		}
		
		//创建主线程
		hth1 = (HANDLE)_beginthreadex( NULL,         // security
									   0,            // stack size
									   CBIZBaseService::ThreadStaticEntryPoint,
									   this,           // arg list
									   CREATE_SUSPENDED,  // so we can later call ResumeThread()
									   &uiThread1ID );

		 if ( hth1 == 0 )
		 {
			rootLogger->error("Failed to create main thread!!");
			
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,FAILED_TO_CREATE_THREAD); //通知界面
			return FAILED_TO_CREATE_THREAD;
		 }
		 
		 if (!GetExitCodeThread( hth1, &dwExitCode ))
		 {
			 rootLogger->error("failed to get the main thread states");
		 }else{
	 		 rootLogger->debug( "main thread exited with code = "+ Utils::itos(dwExitCode) );
		 }
		  if(ResumeThread( hth1 ) == -1)
		  {
			rootLogger->error("failed to resume main thread !!");

			::PostMessage(hwnd,WM_MONITORMESSAGE,0,FAILED_TO_RESUME_THREAD); //通知界面
			return FAILED_TO_RESUME_THREAD;
		  }
		 return 0;
	}
	catch (...) {
		rootLogger->fatal("CBIZBaseService--beginScan error ");
		return -1;
	}
}

int CBIZBaseService::pauseScan(){
	try{
	while (1)
	{

		::PostMessage(hwnd,WM_THREADSTATE,0,0);
		if(SuspendThread(hth1) == -1)
		{
			rootLogger->error("failed to suspend main thread !!");
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,FAILED_TO_SUSPEND_THREAD); //通知界面
			::PostMessage(hwnd,WM_THREADSTATE,1,0);
			return FAILED_TO_SUSPEND_THREAD;
		}
		if (!GetExitCodeThread( hth1, &dwExitCode ))
		{
			rootLogger->error("failed to get the main thread states");
		}else{
			rootLogger->debug( "main thread exited with code = "+ Utils::itos(dwExitCode) );
		}
		break;

	}
	 return  0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--pauseScan error ");
		return -1;
	}
}

int CBIZBaseService::pauseDist(){
	try{
	this->pauseScanFlag = 1;
	::PostMessage(hwnd,WM_THREADSTATE,0,0);
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--pauseDist error ");
		return -1;
	}
}
int CBIZBaseService::resumeScan(){
	try{
	
	::PostMessage(hwnd,WM_THREADSTATE,1,0);
	this->pauseScanFlag = 0;
	 if(ResumeThread( hth1 ) == -1)
	{
		rootLogger->error("failed to resume main thread !!");
		::PostMessage(hwnd,WM_MONITORMESSAGE,0,FAILED_TO_RESUME_THREAD); //通知界面
		::PostMessage(hwnd,WM_THREADSTATE,0,0);
		return FAILED_TO_RESUME_THREAD;
	}
	 return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--resumeScan error ");
		return -1;
	}
}

int CBIZBaseService::endScan(){
	try{
	
	while (1)
	{
		::PostMessage(hwnd,WM_THREADSTATE,0,0);
		if(SuspendThread(hth1) == -1)
		{
			rootLogger->error("failed to suspend main thread !!");
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,FAILED_TO_SUSPEND_THREAD); //通知界面
			::PostMessage(hwnd,WM_THREADSTATE,1,0);
			return FAILED_TO_SUSPEND_THREAD;
		}
		if (!CloseHandle( hth1 ))
		{
			rootLogger->error("failed to close main thread handle !!");
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,FAILED_TO_CLOSE_HANDLE); //通知界面
			return FAILED_TO_CLOSE_HANDLE;
		}
		break;

	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--endScan error ");
		return -1;
	}
}

int CBIZBaseService::initQueues(CDBManage* cdb){

	try{

	this->normalQueue.clear();
	this->abnormalQueue.clear();

	string idSeq = " ";
	//获取原有队列记录
	WorkQueue exsitQueue = this->getExsitQueue(ALL_FETCH_STATE,cdb);

	WorkQueue::iterator ptr;
	for (ptr = exsitQueue.begin(); ptr != exsitQueue.end();++ptr )
	{
		CWorkRec record = *ptr;
		
		//未处理记录放入正常队列,其他放入异常队列
		if (record.getPreState() == 0)
		{
			this->normalQueue.push_back(record);
			if (record.getFetchState() == 0)
			{
				if (idSeq == " ")
				{
					idSeq = Utils::itos(record.getPreId()) + " ";
				}else{
				
					idSeq += " ,"+Utils::itos(record.getPreId());
				}
			}
		}else if (record.getPreState() > 1)
		{
			this->abnormalQueue.push_back(record);
		}
	}

	string sSQL = "update work_queue t set t.fetch_state = 1 where t.pre_id in ( "+idSeq+")";

	if (idSeq != " ")
	{
		int result = cdb->execSQL(sSQL);
		if (result != 0 )
		{
			rootLogger->error("update work_queue state error!!!!");
			return result;
		}
	}
	
	::PostMessage(hwnd,WM_REFRESH,0,99);  //发消息至界面,初始化队列
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--initQueues error ");
		return -1;
	}
}

WorkQueue CBIZBaseService::getExsitQueue(int fetchState,CDBManage* cdb){
	try{
	
	CWorkRec record;
	WorkQueue recordList;
	//读取数据库中所有的队列记录
	string sSQL = "select * from work_queue t ";

	if(fetchState != ALL_FETCH_STATE)
	{
		sSQL += "where t.fetch_state = "+ Utils::itos(fetchState);
		if(enableFinishFlag >0 )
			sSQL += " and t.finish_flag = 1 ";
	}else
	{
		if(enableFinishFlag > 0)
			sSQL += "where t.finish_flag = 1 ";
	}
	sSQL += " order by pre_id";

	//rootLogger->info("sql = "+sSQL);	
	int result = cdb->execSQL(sSQL);
	if (result != 0)
	{
		rootLogger->error("get exsist queue error!!!!!!!!!");
		return recordList;
	}
		
	while(!cdb->isEnd())
	{
		record.setPreId( atoi(cdb->getFieldValueByName("pre_id").c_str()));
		record.setSplitId( atoi(cdb->getFieldValueByName("split_id").c_str()));
		record.setPre_his_id(cdb->getFieldValueByName("pre_his_id"));
		record.setPreOrder(atoi(cdb->getFieldValueByName("pre_order").c_str()));
		record.setPreState(atoi(cdb->getFieldValueByName("pre_state").c_str()));
		record.setReason(cdb->getFieldValueByName("reason"));
		record.setLastDealTime(cdb->getFieldValueByName("last_deal_time"));
		record.setFetchState(atoi(cdb->getFieldValueByName("fetch_state").c_str()));
		record.setErrorCode(atoi(cdb->getFieldValueByName("error_code").c_str()));
		record.setCheckInTime(cdb->getFieldValueByName("checkin_time"));
		record.setMzNo(cdb->getFieldValueByName("mz_no"));
		record.setPatientName(cdb->getFieldValueByName("patient_name"));
		record.setPatientId(atoi(cdb->getFieldValueByName("patient_id").c_str()));
		record.setDepartmentName(cdb->getFieldValueByName("department_name"));
		recordList.push_back(record);
		cdb->nextRow();
	}	
	cdb->freeRecord();	
	return recordList;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--getExsitQueue error ");
		return NULL;
	}
}

int CBIZBaseService::updateWindow(CDBManage* cdb)
{
	try{
	string sSQL ="select window_no from windows where window_no > 0";
	int result = cdb->execSQL(sSQL);
	if (result != 0)
	{
		rootLogger->error("update window error!!!!!!!!");
		return result;
	}
	list<string>windows_list;
    while( !cdb->isEnd())
	{
        windows_list.push_back(cdb->getFieldValueByIndex(0));
		cdb->nextRow();
	}
	cdb->freeRecord();
	list<string>::iterator iter;
	for(iter=windows_list.begin();iter!=windows_list.end();iter++)
	{
		string sql=*iter;
		sSQL = "update windows set use_basket = (select count(*) from basket_mapping where  fetch_window = " +  sql;
		sSQL+= "  )  where  window_no = " + sql;
		rootLogger->info("sql = "+sSQL);

	    result = cdb->execSQL(sSQL);
		if (result != 0)
		{
			rootLogger->error("update window error!!!!!!!!");
			return result;
		}		
	}
	return 0;	
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--updateWindow error ");
		return -1;
	}
}



unsigned __stdcall CBIZBaseService::ThreadStaticEntryPoint(void * pThis)
{
	try{	
	CBIZBaseService * pthX = (CBIZBaseService*)pThis; 
	pthX->ThreadEntryPoint(); 
    return 1;  
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--threadStaticEntryPoint error ");
		return -1;
	}
}


int CBIZBaseService::getNewQueues(CDBManage* cdb)
{
	try{
	// 改为只查fetchState为0的记录
	WorkQueue exsitQueue = this->getExsitQueue(NONE_FETCH,cdb);
	string idSeq ="";
	string idSeq2 ="";
	WorkQueue::iterator ptr;
	CWorkRec record;
	for (ptr = exsitQueue.begin(); ptr != exsitQueue.end();++ptr )
	{
		record = *ptr;
		if(record.getFetchState() == 0)
		{
			if (record.getPreState() == 0)
			{
				if (record.getSplitId() != 0)
				{
					this->normalQueue.push_front(record);
					::PostMessage(hwnd,WM_REFRESH,record.getSplitId(),8);
				}else
				{
					this->normalQueue.push_back(record);
					::PostMessage(hwnd,WM_REFRESH,record.getPreId(),1);   //发消息至主界面更新队列
				}		
			}else{
				this->abnormalQueue.push_back(record);
				rootLogger->debug("add 1 prescription into the abnormal queue preId = "+Utils::itos(record.getPreId()));
				if (record.getSplitId() != 0)
				{
					::PostMessage(hwnd,WM_REFRESH,record.getSplitId(),9); 
				}else{
					::PostMessage(hwnd,WM_REFRESH,record.getPreId(),5);   //发消息至主界面更新队列
				}	
			}
			if (idSeq == "")
			{
				idSeq = Utils::itos(record.getPreId()) + " ";
			}else{
			
				idSeq += " ,"+Utils::itos(record.getPreId());
			}
		}
		
	}
	if (exsitQueue.size() != 0)
	{
		string sSQL = "update work_queue t set t.fetch_state = 1 where t.pre_id in ( "+idSeq+")";
		int result = cdb->execSQL(sSQL);
		if (result != 0)
		{
			rootLogger->error("update work_queue error!!!!!!!!");
			return result;
		}
	}	
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--getNewQueues error ");
		return -1;
	}
}


int CBIZBaseService::getMedicineList(int preId,int splitId,list<CPrescriptionMedicine>& preMedList,CDBManage* db)
{
	try{
		string sqlText;
		if(splitId == 0)
		{
			sqlText = "select *"  
				" from prescription_medicine_list medList left join medicine med on med.med_id = medList.med_id"
				" where medList.pre_id = '" + Utils::itos(preId) +"'";			
		}
		else
		{
			
			sqlText = "select *"  
				" from prescription_split_medicine_list medList left join medicine med on med.med_id = medList.med_id"
				" where medList.pre_id = '" + Utils::itos(preId)+"' and  split_id = '" + Utils::itos(splitId)+"'";
		}

		long lRet = db->execSQL(sqlText);
		if (lRet != 0)
		{
			rootLogger->error("failed to get prescription medicine, errorCode:" + Utils::itos(lRet));
			return ER_GET_PRESCRIPTION_MEDICINE;
		}	
		while (!db->isEnd())
		{
			CPrescriptionMedicine preMed;
			preMed.setMedicineName(db->getFieldValueByName("med_name_ch"));
            preMed.setMedicineNameEN(db->getFieldValueByName("med_name_en"));
			preMed.setMedicineTypeUnit(db->getFieldValueByName("type_unit"));
			preMed.setMedicineType(atoi(db->getFieldValueByName("med_type").c_str()));
            preMed.setMedicineUnit(db->getFieldValueByName("med_unit"));
			preMed.setMedicineFactoryName(db->getFieldValueByName("factory_name"));
			preMed.setMedicineId(atoi(db->getFieldValueByName("med_id").c_str()));
            preMed.setMedicineHisId(atoi(db->getFieldValueByName("med_his_id").c_str()));
			preMed.setMedicineHint(db->getFieldValueByName("med_hint"));
			preMed.setMedicineNum(atoi(db->getFieldValueByName("med_num").c_str()));
			preMed.setPrescriptionId(atoi(db->getFieldValueByName("pre_id").c_str()));
			preMed.setUseQty(atoi(db->getFieldValueByName("use_qty").c_str()));
			preMed.setUseDays(atoi(db->getFieldValueByName("use_days").c_str()));
			preMed.setUseTime(atoi(db->getFieldValueByName("use_time").c_str()));
			preMed.setMedicineHeteromorphism(atof(db->getFieldValueByName("med_heteromorphism").c_str()));	
			preMed.setMaxNumOnce(atoi(db->getFieldValueByName("max_num_once").c_str()));
			preMed.setWeight(atoi(db->getFieldValueByName("weight").c_str()));
			preMed.setDistState(atoi(db->getFieldValueByName("dist_state").c_str()));			
			preMedList.push_back(preMed);
			db->nextRow();
		}
		db->freeRecord();
		return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--getMedicineList error ");
		return -1;
	}
}

int CBIZBaseService::getPatient(int prescriptionId, CPatient &patient)	//取得病人信息
{
	//处方管理接口
    try{
		int result = cpm->getPatient(prescriptionId,patient);
		
		if (result != 0)
		{
			rootLogger->error("get getPatient error ! preId = "+Utils::itos(prescriptionId));
			return result;
		}
    }catch (...) {
		rootLogger->fatal("CBIZBaseService--getPatient error ");
		return -1;
	}
    return 0;
}


int CBIZBaseService::getLocationList( int medId,list<CLocation>* locationList)
{
	try{
	list<MedicinLocationState> locList;

	MedicinLocationState tmp2;

	int result = medManage->addressSearch(medId,&locList);
	rootLogger->info("call medManage addressSearch finish..");

	list<MedicinLocationState>::iterator ptr = locList.begin();

	
	while (ptr != locList.end())
	{
		CLocation tmp;
		tmp2 = *ptr;
		if (tmp2.location_state == 0)
		{
			ptr++;
			continue;
		}
		tmp.setLocationId(tmp2.location_id);
		tmp.setCurQuantity(tmp2.cur_quantity);
		tmp.setLocationState(tmp2.location_state);
		tmp.setLocationType(tmp2.location_type);
		tmp.setMachineId(tmp2.machine_id);
		tmp.setMaxCapacity(tmp2.max_capacity);
		tmp.setMedicineId(tmp2.medicine_id);
		tmp.setWarningPercent(tmp2.warning_percent);

		locationList->push_back(tmp);
		ptr++;
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--getLocation error ");
		return -1;
	}
}

//
int CBIZBaseService::checkStorage( list<CPrescriptionMedicine>* listMedicine,CDBManage* cdb,int splitId)
{
	try{	
	list<CPrescriptionMedicine>::iterator ptr;
	list<MedinceCount>* mclist = new list<MedinceCount>();
	int maxNum =0;	
	int preId;
	for (ptr = listMedicine->begin(); ptr != listMedicine->end();++ptr)
	{
		mclist->clear();
		preId = (*ptr).getPrescriptionId();
		int medId = (*ptr).getMedicineId();
		int num = (*ptr).getMedicineNum();
		int weight = (*ptr).getWeight();
		double heter = (*ptr).getMedicineHeteromorphism();
		int singleIgnoreNum = (*ptr).getMaxNumOnce();
		if (singleIgnoreNum ==0)
		{
			singleIgnoreNum = 99;
		}
		if (weight == 0)
		{
			weight = 200000;
		}
		if (this->isPubMed(medId,cdb)!=0)
		{
			//公用药直接跳过
			rootLogger->info("public med!");
			ptr->setDistState(8);
			updatePrescriptionMedicineListState(preId,medId,8,cdb,splitId);
			continue;  
		}
		int errorCode = medManage->getCurrentMedicineCount(medId,mclist);
		list<MedinceCount>::iterator p1 = mclist->begin();
		while(p1 != mclist->end())
		{
			if(p1->deviceType == m_machineType && p1->deviceID != m_machineId)
			{
				mclist->erase(p1) ;
				p1 = mclist->begin();
			}
			else
			{
				p1++;
			}
		}
		p1 = mclist->begin();
		if(p1!= mclist->end())
		{
			int medType = (*p1).deviceType;
			if(medType != m_machineType)
			{	
				if ( medType == ISHELF_TYPE)
				{
					updatePrescriptionMedicineListState(preId,medId,2,cdb,splitId);
					ptr->setDistState(2);//智能存储
					updatePrescriptionState(preId,NEED_READDING,cdb,splitId);
				}
				else if ( medType == DRUGSHELF_TYPE)
				{
					updatePrescriptionMedicineListState(preId,medId,7,cdb,splitId);
					ptr->setDistState(7);//智能药架
					updatePrescriptionState(preId,NEED_READDING,cdb,splitId);
				}
				else //if (medType == TABLET_TYPE || medType == INJECTION_TYPE || medType == OTHER_TYPE )
				{
					updatePrescriptionMedicineListState(preId,medId,3,cdb,splitId);
					ptr->setDistState(3);//非机储
					updatePrescriptionState(preId,NEED_READDING,cdb,splitId);
				}
			}
			else
			{
				int remainNum =0;
				remainNum = (*p1).medCount;
				if (num > singleIgnoreNum)
				{
					rootLogger->info("single medicine too many , ignore.");
					updatePrescriptionMedicineListState(preId,medId,6,cdb,splitId);
					ptr->setDistState(6);//超过单次发药
					updatePrescriptionState(preId,NEED_READDING,cdb,splitId);
					continue;
				}
				if (num > remainNum || heter >0)
				{
					if( num > remainNum)
					{
						updatePrescriptionMedicineListState(preId,medId,1,cdb,splitId,num-remainNum);
						maxNum +=(int)(remainNum*weight);
						ptr->setCurSendMedNum(remainNum);
					}
					else if(heter >0)
					{
						updatePrescriptionMedicineListState(preId,medId,1,cdb,splitId);
					}
					ptr->setDistState(1);//缺药
					updatePrescriptionState(preId,NEED_READDING,cdb,splitId);
				}
				else
				{
					if(ptr->getDistState()!=0)
						updatePrescriptionMedicineListState(preId,medId,0,cdb,splitId);
					ptr->setDistState(0);//正常
					maxNum +=(int)(num*weight);
				}	
			}
		}
		else
		{
			updatePrescriptionMedicineListState(preId,medId,3,cdb,splitId);
			ptr->setDistState(3);//非机储
			updatePrescriptionState(preId,NEED_READDING,cdb,splitId);
		}
	}
/*	if (maxNum > ignoreNum)
	{
		rootLogger->info("too many medicines , ignore this prescription ");
		updatePrescriptionState(preId,NEED_READDING,cdb,isSplit);
		return IGNORE_PRESCRIPTION;
	}
*/ //取消此功能,单药品的手工来代替此功能
  if (maxNum > splitMax)
	{
		rootLogger->info("need split prescription ");
		return SPLIT_PRESCRIPTION;
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--checkStorge error ");
		return -1;
	}
}


int CBIZBaseService::spliceToAbnormalQueue(CDBManage* cdb,int preIdN,int splitId, int preIdA){
	try{
	WorkQueue::iterator ptr1 ;
	if(splitId !=0)
	{
		ptr1= find_if(normalQueue.begin(),normalQueue.end(),FindBySplitId(splitId));
	}
	else
	{
        ptr1= find_if(normalQueue.begin(),normalQueue.end(),FindByPreId(preIdN));		
	}
	WorkQueue::iterator ptr2 = find_if(abnormalQueue.begin(),abnormalQueue.end(),FindByPreId(preIdA));
	
	if (ptr1 == normalQueue.end())
	{
		rootLogger->error("Can not find preId = "+ Utils::itos(preIdN)+" in normal work queue");
		return CAN_NOT_FIND_NORMAL_ID;
	}else{
		abnormalQueue.splice(ptr2,normalQueue,ptr1);
	}
	int result = this->updateWorkState(preIdN,splitId,HANG_ON,cdb);
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--spliceToAbnormalQueue error ");
		return -1;
	}
}
int CBIZBaseService::spliceToAbnormalQueueForRemote(CDBManage* cdb,int preId){
	try{	
	//this->spliceToAbnormalQueue(cdb,preId);
	::PostMessage(hwnd,WM_REFRESH,preId,2);
	::PostMessage(hwnd,WM_REFRESH,preId,5);
	return 1;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--spliceToAbnormalQueueForRemote error ");
		return -1;
	}
}
int CBIZBaseService::spliceToNormalQueue(CDBManage* cdb,int preIdA,int splitId, int preIdN){
	try{
	WorkQueue::iterator ptr1 = find_if(normalQueue.begin(),normalQueue.end(),FindByPreId(preIdN));

	WorkQueue::iterator ptr2; 
	if(splitId !=0)
	{
		ptr2 = find_if(abnormalQueue.begin(),abnormalQueue.end(),FindBySplitId(splitId));
	}
	else
	{
		ptr2 = find_if(abnormalQueue.begin(),abnormalQueue.end(),FindByPreId(preIdA));
	}
	
	if (ptr2 == abnormalQueue.end())
	{
		rootLogger->error("Can not find preId = "+ Utils::itos(preIdN)+" in abnormal work queue");
		return CAN_NOT_FIND_ABNORMAL_ID;
	}else{
		normalQueue.splice(ptr1,abnormalQueue,ptr2);
	}
	int result = this->updateWorkState(preIdA,splitId,NOARMAL,cdb);
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--spliceToNomalQueue error ");
		return -1;
	}
}
int CBIZBaseService::spliceToNormalQueueMulti(CDBManage* cdb,string sIdQueue)
{
	try{
	//string sql = "update work_queue set pre_state = 0 where pre_id in ("+sIdQueue+"); ";
	string sql = "update work_queue set pre_state = 0 where "+sIdQueue;
	rootLogger->info("sql="+sql);
	int result = cdb->execSQL(sql);
	if (result != 0)
	{
		rootLogger->error("spliceToNormalQueueMulti error!");
	}
	
	WorkQueue::iterator ptr1 = abnormalQueue.begin();
	WorkQueue::iterator ptr2 = abnormalQueue.end();
    normalQueue.splice(normalQueue.end(),abnormalQueue,ptr1,ptr2);
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--spliceToNormalQueueMulti error ");
		return -1;
	}
}
int CBIZBaseService::skipForword(int preId,int splitId){
	try{

	WorkQueue::iterator ptr;
	if(splitId !=0)
	{
		ptr= find_if(normalQueue.begin(),normalQueue.end(),FindBySplitId(splitId));
	}
	else
	{
        ptr= find_if(normalQueue.begin(),normalQueue.end(),FindByPreId(preId));
	}

	if(ptr == normalQueue.end())
	{
		rootLogger->error("Can not find preId = "+ Utils::itos(preId)+" in abnormal work queue");
		return CAN_NOT_FIND_NORMAL_ID;
	}else if (ptr == normalQueue.begin())
	{
		rootLogger->warn(" preId = "+Utils::itos(preId)+" is still at the beginning of queue");
		
	}else{
		WorkQueue::iterator ptr2 = ptr;
		WorkQueue::iterator ptr1 = --ptr;
		normalQueue.splice(ptr1,normalQueue,ptr2);
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--skipForword error ");
		return -1;
	}
}

int CBIZBaseService::skipBack(int preId,int splitId){
	try{
	
	WorkQueue::iterator ptr ;
	if(splitId !=0)
	{
		ptr= find_if(normalQueue.begin(),normalQueue.end(),FindBySplitId(splitId));
	}
	else
	{
        ptr= find_if(normalQueue.begin(),normalQueue.end(),FindByPreId(preId));
	}

	if(ptr == normalQueue.end())
	{
		rootLogger->error("Can not find preId = "+ Utils::itos(preId)+" in abnormal work queue");
		return CAN_NOT_FIND_NORMAL_ID;
	}else if (ptr == normalQueue.begin())
	{
		rootLogger->warn(" preId = "+Utils::itos(preId)+" is still at the beginning of queue");
		
	}else{
		WorkQueue::iterator ptr2 = ptr;
		WorkQueue::iterator ptr1 = ++ptr;
		normalQueue.splice(ptr2,normalQueue,ptr1); 
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--skipBack error ");
		return -1;
	}
}

int CBIZBaseService::getNormalRec(int preId, CWorkRec& rec,bool isSplit){
	try{
	WorkQueue::iterator ptr;
	if (isSplit)
	{
		ptr = find_if(normalQueue.begin(),normalQueue.end(),FindBySplitId(preId));
	}else
	{
		ptr = find_if(normalQueue.begin(),normalQueue.end(),FindByPreId(preId));
	}
	if (ptr == normalQueue.end())
	{
		rootLogger->error("Can not find preId = "+ Utils::itos(preId)+" in normal work queue");
		return CAN_NOT_FIND_NORMAL_ID;
		
	}else{
		rec = *ptr;
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--getNormalRec error ");
		return -1;
	}
}

int CBIZBaseService::getAbnormalRec(int preId, CWorkRec& rec,bool isSplit){
	try{
	WorkQueue::iterator ptr ;
	if (isSplit)
	{
		ptr = find_if(abnormalQueue.begin(),abnormalQueue.end(),FindBySplitId(preId));

	}else
	{
		ptr =  find_if(abnormalQueue.begin(),abnormalQueue.end(),FindByPreId(preId));
	}
	if (ptr == abnormalQueue.end())
	{
		rootLogger->error("Can not find preId = "+ Utils::itos(preId)+" in abnormal work queue");
		return CAN_NOT_FIND_ABNORMAL_ID;
		
	}else{
		rec = *ptr;
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--getAbnormalRec error ");
		return -1;
	}
}

int CBIZBaseService::sendToQueue(list<CPrescription>* preList,list<CPatient>* patList,CDBManage* cdb)
{
	try{
	list<CPrescription>::iterator ptr1 = preList->begin();
	list<CPatient>::iterator ptr2 = patList->begin();
	CPrescription tmp1;
	CPatient tmp2;
	
	string sql;
	while (ptr1 != preList->end())
	{
		tmp1 = *ptr1;
		tmp2 = *ptr2;

		
		CTime t=CTime::GetCurrentTime(); 
		string curTime = t.Format("%Y-%m-%d %H:%M:%S"); 
		sql = "INSERT INTO work_queue (pre_id,pre_state,pre_order,error_code,reason,checkin_time,";
		sql +="last_deal_time,fetch_state,mz_no,patient_name,patient_id,department_name) VALUES (";
		sql +=Utils::itos(tmp1.getPrescriptionId());
		sql += ", 0,NULL,0,NULL,'";
		sql += curTime;
		sql += "',NULL,0,'";
		sql += tmp2.getMzNo();
		sql += "','";
		sql += tmp2.getPatientName();
		sql += "','";
		sql += tmp2.getPatientId();
		sql += "','";
		sql += tmp1.getDepartmentName();
		sql += "');";

		rootLogger->info("sql = "+sql);

		int result = cdb->execSQL(sql);
		//INSERT INTO work_queue (pre_id,pre_state,pre_order,error_code,reason,checkin_time,last_deal_time,fetch_state,heteromorphism_box,mz_no,patient_name,patient_id,department_name) VALUES (10000001,0,NULL,0,NULL,'2009-08-11 08:00:00',NULL,1,NULL,'1001','1');

		if (result == 0)
		{
			this->updatePrescriptionState(tmp1.getPrescriptionId() ,WAIT_FOR_SEND,cdb);
		}
		ptr1++;
		ptr2++;
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--sendToQueue error ");
		return -1;
	}
}

int CBIZBaseService::sendToQueueById(list<int>*preIdList,CDBManage* cdb) 
{
	// TODO: Add your control notification handler code here
	try{
	
	long preId;
	list<CPrescription> preList;
	list<CPatient> patList;
	list<PrescriptionInfo> pinfoList;
	list<int>::iterator ptr ;
	map<string,string> qryWhere;     //查询条件
	PrescriptionInfo preInfo;

	if (preIdList == NULL)
	{
		rootLogger->error("pre list is NULL");
		return 1;
	}
	for(ptr = preIdList->begin();ptr != preIdList->end();ptr++)
	{
		preId = int(*ptr);
		

		qryWhere.insert(pair<string, string>("pre_id",Utils::itos(preId)));

		long lRet = cpm->queryPrescription(qryWhere,pinfoList, "");

		if (pinfoList.size() != 0)
		{
			preInfo = PrescriptionInfo(*(pinfoList.begin()));
			preList.push_back(preInfo.prescription);
			patList.push_back(preInfo.prePatient);
		}	
	}

	if (!preList.empty() && !patList.empty())
	{
		int result = this->sendToQueue(&preList, &patList,cdb);
		if(result != 0)
		{
			rootLogger->error("sendTOQueue error! errorCode = "+Utils::itos(result));
            return result;
        } 
        
        return 0;
    }
    
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--sendToQueueById error ");
		return -1;
	}
}

int CBIZBaseService::updateWorkState(int preId , int splitId, int states,CDBManage* cdb){
	try{
	string sSQL = "update work_queue t set t.pre_state="+Utils::itos(states);
	sSQL +=" where pre_id = "+Utils::itos(preId);
	sSQL +=" and split_id = "+Utils::itos(splitId);

	rootLogger->info("sql = "+sSQL);

	int result = cdb->execSQL(sSQL);
	if (result != 0)
	{
		rootLogger->error("update work state error!!!!!!");
	}
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--updateWorkState error ");
		return -1;
	}
}

int CBIZBaseService::updatePrescriptionState(int preId,int states,CDBManage* cdb,int splitId){
	try{
	string sSql;
	if (splitId != 0)
	{
		sSql ="update prescription_split t set t.split_state = "+Utils::itos(states);
		sSql += " where split_id = "+Utils::itos(preId);
	}else
	{
		sSql = "update prescription t set t.pre_state = "+Utils::itos(states);
		sSql += " where pre_id = "+Utils::itos(preId);
	}
	rootLogger->info("sql ="+sSql);
	int result = cdb->execSQL(sSql);
	if (result != 0)
	{
		rootLogger->error("update Prescription state error");
	}
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--updatePrescriptionState error ");
		return -1;
	}
}

int CBIZBaseService::updatePrescriptionMedicineListState(int preId,int medicineId,int states,CDBManage* cdb,int splitId,int remedy_med_num)
{
	try{	
	string sSql;
	if (splitId != 0)
	{
		if(states==4)
		{
			sSql="update prescription_split_medicine_list t set t.dist_state = 4 ";
			sSql += " , t.remedy_med_num = t.remedy_med_num + "+Utils::itos(remedy_med_num);
		}
		else if(states==5)
		{
			sSql="update prescription_split_medicine_list t set t.dist_state = 5 ";
			sSql += " , t.remedy_med_num = t.remedy_med_num -"+Utils::itos(remedy_med_num);
		}
		else
		{
			sSql="update prescription_split_medicine_list t set t.dist_state =" +Utils::itos(states);
			sSql += " , t.remedy_med_num ="+Utils::itos(remedy_med_num);
		}
		sSql += " where pre_id ="+Utils::itos(preId);
		sSql += " and split_id = "+Utils::itos(splitId);
	    sSql += " and med_id = "+Utils::itos(medicineId);
	}
	else
	{
		sSql="update prescription_medicine_list t set t.dist_state =" +Utils::itos(states);
		if(states==4)
		{
			sSql += " , t.remedy_med_num = t.remedy_med_num + "+Utils::itos(remedy_med_num);
		}
		else if(states==5)
		{
			sSql += " , t.remedy_med_num = t.remedy_med_num -"+Utils::itos(remedy_med_num);
		}
		else
			sSql += " , t.remedy_med_num ="+Utils::itos(remedy_med_num);
		sSql += " where pre_id ="+Utils::itos(preId);
	    sSql += " and med_id = "+Utils::itos(medicineId);
	}
	
	rootLogger->info("sql ="+sSql);

	int result = cdb->execSQL(sSql);
	if (result != 0)
	{
		rootLogger->error("update prescription_medicine_list error");
	}
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--updatePrescriptionMedicineListState error ");
		return -1;
	}
}

int CBIZBaseService::isDelete(int preId,CDBManage* cdb)
{
	try{
	int flag = 0;
	string sSql ="select delete_flag from prescription t where t.pre_id =" +Utils::itos(preId);
	rootLogger->info("sql ="+sSql);

	int result = cdb->execSQL(sSql);
	if (result != 0)
	{
		rootLogger->error("update prescription_medicine_list error");
	}
	if(!cdb->isEnd())
	{
		flag = atoi(cdb->getFieldValueByIndex(0).c_str());
	}
	cdb->freeRecord();
	return flag;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--isDelete error ");
		return 0;
	}
}

int CBIZBaseService::isPubMed(int medId,CDBManage* cdb)
{
	try{
    int flag = 0;
	map<int,MedicinLocationState>medlocationstate;
	map<int,MedicinLocationState>::iterator iter;
	if(medManage->getMedicinLocation(&medlocationstate)==0)
	{
		for(iter=medlocationstate.begin();iter!=medlocationstate.end();iter++)
		{
			if(iter->second.medicine_id==medId)
				flag = iter->second.med_publicflag;
		}
	}
    else
	{	
		string sSql ="select public_flag from medicine t where t.med_id =" +Utils::itos(medId);	
		int result = cdb->execSQL(sSql);
		if (result != 0)
		{
			rootLogger->error("select public_flag error!");
		}
		if(!cdb->isEnd())
		{
			flag = atoi(cdb->getFieldValueByIndex(0).c_str());
		}
		cdb->freeRecord();
	}
	return flag;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--isPubMed error ");
		return 0;
	}
}
WorkQueue CBIZBaseService::getNormalQueue(){
	return this->normalQueue;
}

WorkQueue CBIZBaseService::getAbnormalQueue(){
	return this->abnormalQueue;
}

int CBIZBaseService::clearCard()
{
	long bFlag;
	fxcpu->GetBasketReachInfo(&bFlag);
	if (bFlag == 1)
	{
        string origin ="";
		int result = cPortManage1->readData(origin);
		if (result != 0)
		{
			rootLogger->error("clearCard cPortManage1 readData error");
			return result;
		}
		Sleep(50);
		result = cPortManage1->writeCard(99,0);
		if (result != 0)
		{
			rootLogger->error("clearCard cPortManage1 writeCard error");
			return result;
		}
		origin ="";
		result= cPortManage2->readData(origin);
		if (result != 0)
		{
			rootLogger->error("clearCard cPortManage2 readData error");
			return result;
		}
	    Sleep(50);
		result = cPortManage2->writeCard(99,0);
		if (result != 0)
		{
			rootLogger->error("clearCard cPortManage2 writeCard error");
			return result;
		}
	}
	return 0 ;
}

string CBIZBaseService::getBasketId(CDBManage* cdb)
{
	int errorCode =0 ;
	try{
	if(runMode == 1 || runMode == 9)
	{
		return "999";
	}
	string origin ="";
	string result ="";
	//cPortManage1->clearData();
	//cPortManage2->clearData();
	int i = 100;
	int round = 0;
	int round1 = 0;
	while (i != 0)
	{
		if (round == 3)
		{
			rootLogger->error("can not read card! please check!");
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,READ_BASKET_ERROR) ;
			fxcpu->SetSendBasketOut(1); //送走篮子信号
			pauseScan();			
			//cPortManage1->clearData();
			//cPortManage2->clearData();
			i=100;
			round =0;
		}
		long bFlag;
		errorCode = fxcpu->GetBasketReachInfo(&bFlag);
        if(errorCode != 0)
		{
			rootLogger->error("获取篮子到达信号失败 错误码 = "+Utils::itos(errorCode));
		}
		if (bFlag == 1)
		{
			for (int j =0;j<3;j++)
			{
				origin ="";
				errorCode = cPortManage1->readData(origin);
				if(errorCode != 0)
				{
					rootLogger->error("cPortManage1 读卡失败 错误码="+Utils::itos(errorCode));
				}
				if(origin != "" && origin.size()==8)
				{
					result = getBasketIdFormCard(origin,cdb);
					if(result == "")
					{
						rootLogger->error("cPortManage1 获取篮子号失败，卡号为："+origin);
					}
				}
				else
				{
					rootLogger->error("cPortManage1 readCard error");
				}
				origin ="";
				errorCode = cPortManage2->readData(origin);
				if(errorCode != 0)
				{
					rootLogger->error("cPortManage2 读卡失败 错误码="+Utils::itos(errorCode));
				}
				if (origin !="" && origin.size()==8)
				{
					result = getBasketIdFormCard(origin,cdb);
					if(result == "")
					{
						rootLogger->error("cPortManage2 获取篮子号失败，卡号为："+origin);
					}
				}
				else
				{
					rootLogger->error("cPortManage2 readCard error");
				}
				if (result == "")
				{
					//cPortManage1->clearData();
					//cPortManage2->clearData();
					round++;
				}
				else
				{
					//cPortManage1->clearData();
					//cPortManage2->clearData();
					return result;
				}
				Sleep(1000);
			}
		}
		i--;
		if (i== 0)
		{
			rootLogger->error("get basket id over time!");
			round1++;
			if (round1 == 3)
			{
				if (pBoxMonitor->getKepFlag())
				{
					rootLogger->info("wait for adding basket~");
					round1=0;
				}
				else
				{
					::PostMessage(hwnd,WM_MONITORMESSAGE,0,READ_BASKET_OVER_TIME) ;
					pauseScan();
					round1 =0;
				}	
			}
			i = 100;
		}
		Sleep(50);
	}
	//cPortManage1->clearData();
	//cPortManage2->clearData();
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBoxService--getBasketId error 错误码="+Utils::itos(errorCode));
		return "";
	}
}

string CBIZBaseService::getBasketIdFormCard(string cardId,CDBManage* cdb)   
{
	try{
		string sBasketId = "";
		string sqlText = "select basket_id from basket_mapping where card_id1='" + cardId + "' or card_id2='" + cardId + "'";
		long lRet = cdb->execSQL(sqlText);
		if (lRet==0 && cdb->getRowNum()>0)
		{ 
			sBasketId = cdb->getFieldValueByIndex(0);
		}
		else
		{   
			rootLogger->error(sqlText);
			rootLogger->error("getBasketIdFormCard errorCode = "+Utils::itos(lRet));
		}
		return sBasketId;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--getBasketIdFormCard error ");
		return "";
	}
}


int CBIZBaseService::sendMedCount(int locationId ,int count)
{
    try{
    int max_count=splitMax;
 	list<MedicineInfo> medList;
	map<int,MedicinLocationState>medlocationstate;
	map<int,MedicinLocationState>::iterator iter;
	medManage->getMedicinLocation(&medlocationstate);
	iter=medlocationstate.find(locationId);
	if(iter!=medlocationstate.end())
	{
		int weight=0;
		medManage->getMedWeight(iter->second.medicine_id,weight);
		while(count>0)
		{
			MedicineInfo medInfo;
			int temp = count*weight;
			if( temp > max_count)
			{
				count -= max_count/weight;
				medInfo.count = max_count/weight;
			}
			else
			{	
				medInfo.count =count;
				count= 0;
			}
			medInfo.medId = iter->second.medicine_id;
			medInfo.position = locationId;
			medList.push_back(medInfo);
			
			int errorCode = 0 ;
			long bFlag = 0;
			while(bFlag == 0)
			{	
				errorCode = fxcpu->GetSendBasketFinishInfo(&bFlag);
				if(errorCode != 0)
				{
					rootLogger->error("获取PLC送走篮子完成信号失败 错误码 = "+Utils::itos(errorCode));
					Sleep(100);
					continue;
					//return errorCode;
				}
				Sleep(50);
			}
			
			errorCode = fxcpu->SetIniSendBasketFinish(0);
			if(errorCode != 0)
			{
				rootLogger->error("复位送走篮子完成信号失败 错误码 = "+Utils::itos(errorCode));
				return errorCode;
			}
			
			errorCode =pBoxSinglechip->distributeMed(medList);
			medList.clear();
			MedicineInfoList errorAddress;
            int res = pBoxSinglechip->isDstrOver(errorAddress);
			while(res == 1)
			{
				res = pBoxSinglechip->isDstrOver(errorAddress);
				Sleep(100);
			}
			string s_basketID = "";
			CDBManage* db = new CDBManage();
			while( s_basketID.empty())
			{
				s_basketID = getBasketId(db);
				Sleep(50);
			}
			delete db;
			clearCard();
			long lastLocationId = pBoxSinglechip->getLastLocationID();
			Sleep(lastLocationId*100);
			errorCode = fxcpu->SetStartInsideBelt(locationId);	
			if(errorCode != 0)
			{
				rootLogger->error("内部传送带启动信号失败 错误码 = "+Utils::itos(errorCode));
			}
			if(res != 0)
				break;
		}
	}	
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--sendMedCount error ");
		return -1;
	}	
}

int CBIZBaseService::clearLocation(int locationId){
	try{		
		sendMedCount(locationId,99);
		return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--clearLocation error ");
		return -1;
	}
}


//pandian_flag 1 药品盘点 ，2单个药品发药
int CBIZBaseService::clearMedicine(int medicineId,int pandian_flag,int medcount){
	try{
	int result = 0;

	list<CLocation> locList ;
	
	this->getLocationList(medicineId,&locList);

	list<CLocation>::iterator ptr = locList.begin();
	if(pandian_flag==2)//单个药品发药
	{
		map<int,MedicinLocationState> medLocMap;
		while (ptr != locList.end())
		{
			MedicinLocationState medLoc;
			CLocation tmp = *ptr;
			if(tmp.getLocationState()==0 || tmp.getCurQuantity()==0)
			{
				ptr++;
				continue;
			}
			if(tmp.getCurQuantity() > medcount)
			{
				//写数据库
				medLoc.location_id=tmp.getLocationId();
				medLoc.cur_quantity = medcount;
				medLocMap.insert(pair<int,MedicinLocationState>(medLoc.location_id,medLoc));
				sendMedCount(tmp.getLocationId(),medcount);	
				medcount = 0;
			}
			else
			{
				//写数据库
				medLoc.location_id=tmp.getLocationId();
				medLoc.cur_quantity = tmp.getCurQuantity();
				medLocMap.insert(pair<int,MedicinLocationState>(medLoc.location_id,medLoc));
				sendMedCount(tmp.getLocationId(),tmp.getCurQuantity());
				medcount -= tmp.getCurQuantity();			
			}
			medManage->popMedicinLocation(medLocMap);
			if(medcount==0)
				break;
		}
	}
	else	
	{
		ptr = locList.begin();
		while (ptr != locList.end())
		{
			CLocation tmp = *ptr;
			sendMedCount(tmp.getLocationId(),99);
	        ptr++;
		}	    
	}
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--clearMedicine error ");
		return -1;
	}
}


int CBIZBaseService::deleteFromDB(int preId,CDBManage* cdb,int splitId)
{
	try{
	
	if (preId == 0)
	{
		return 0;
	}
	string sSQL;
	if (keepRec == 0)
	{
		if (splitId == 0)
		{
			sSQL = "delete from work_queue where pre_id ="+Utils::itos(preId);
			sSQL += " and split_id = 0";
		}else
		{
			sSQL = "delete from work_queue where split_id ="+Utils::itos(splitId);
			sSQL += " and pre_id = "+ Utils::itos(preId);
		}
		rootLogger->info("sql = "+sSQL);
		int result = 0;
		result = cdb->execSQL(sSQL);
		if (result != 0)
		{
			rootLogger->error("delete recode error!!");
			return result;
		}
	}else
	{
		//配合客户端调整删除策略
		if (splitId == 0)
		{
			sSQL = "update work_queue set pre_state = " +Utils::itos(FINISH_DISTRIBUTE)+ " where pre_id ="+Utils::itos(preId);
			sSQL += " and split_id = 0";
		}else
		{
			sSQL = "update work_queue set pre_state = " +Utils::itos(FINISH_DISTRIBUTE)+ " where split_id ="+Utils::itos(splitId);	
			sSQL += " and pre_id = "+ Utils::itos(preId);
		}

		rootLogger->info("sql = "+sSQL);
		int result = 0;
		result = cdb->execSQL(sSQL);
		if (result != 0)
		{
			rootLogger->error("update recode error!!");
			return result;
		}
	}	
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--deleteFromDB error ");
		return -1;
	}
}

int CBIZBaseService::splitPrescription(int preId, list<CPrescriptionMedicine>* listMedicine,CDBManage* cdb)
{
	try{
	WorkQueue::iterator pptr= normalQueue.begin();
	CWorkRec rec = *pptr;
	normalQueue.pop_front();
	::SendMessage(hwnd,WM_REFRESH,rec.getPreId(),2);
	
	list<CPrescriptionMedicine>::iterator ptr;
	list<MedinceCount>* mclist = new list<MedinceCount>();	
	int remain_sum =splitMax;
	rootLogger->debug("splitMax="+Utils::itos(splitMax));
	int newSplitId;
	int result = insertSplitPrescription(preId,rec,newSplitId,cdb);
	if(result != 0)
	{
		rootLogger->error("insertSplitPrescription error");
		return result;
	}
	for (ptr = listMedicine->begin(); ptr != listMedicine->end();++ptr)
	{
		int medId = (*ptr).getMedicineId();
		int num = (*ptr).getMedicineNum();
		int weight =ptr->getWeight();
		double medHete = (*ptr).getMedicineHeteromorphism();
		int singleIgnoreNum = ptr->getMaxNumOnce();
		if (weight == 0)
		{
			weight = 10;
		}	
		if (this->isPubMed(medId,cdb)!=0)
		{
			//公用药直接全额添加
			result = insertSplitPrescriptionMedicine(preId,newSplitId,medId,num,medHete,cdb);
			if(result != 0)
			{
				rootLogger->error("insertSplitPrescriptionMedicine error,medId="+Utils::itos(medId));
				return result;
			}
			continue;  
		}
		
		if (num > singleIgnoreNum)
		{
			rootLogger->info("single medicine too many , add all.");
			result = insertSplitPrescriptionMedicine(preId,newSplitId,medId,num,medHete,cdb);
			if(result != 0)
			{
				rootLogger->error("insertSplitPrescriptionMedicine error,medId="+Utils::itos(medId));
				return result;
			}

			continue;
		}
		int medType;
		rootLogger->debug("Analyze med_id="+Utils::itos(medId)+" ,pre_id="+Utils::itos(preId));
		mclist->clear();
		int errorCode = medManage->getCurrentMedicineCount(medId,mclist);
		list<MedinceCount>::iterator p1 = mclist->begin();
		if (p1 != mclist->end())
		{
			medType = (*p1).deviceType;
			if(medType != m_machineType)
			{
				rootLogger->debug("not box_type,add all medicine.");
				result = insertSplitPrescriptionMedicine(preId,newSplitId,medId,num,medHete,cdb);
				if(result != 0)
				{
					rootLogger->error("insertSplitPrescriptionMedicine error,medId="+Utils::itos(medId));
					return result;
				}
			}
			else
			{
				while (num>0)
				{
					if (num*weight > remain_sum)
					{
						if (remain_sum >= weight)
						{
							rootLogger->debug("remain sum = "+Utils::itos(remain_sum));
							result = insertSplitPrescriptionMedicine(preId,newSplitId,medId,remain_sum/weight,0,cdb);
							if(result != 0)
							{
								rootLogger->error("insertSplitPrescriptionMedicine error,medId="+Utils::itos(medId));
								return result;
							}
							num -= remain_sum/weight;
						}
						remain_sum = splitMax;
						result = insertSplitPrescription(preId,rec,newSplitId,cdb);
						if(result != 0)
						{
							rootLogger->error("insertSplitPrescription error");
							return result;
						}
					}
					else
					{
						result = insertSplitPrescriptionMedicine(preId,newSplitId,medId,num,medHete,cdb);
						if(result != 0)
						{
							rootLogger->error("insertSplitPrescriptionMedicine error,medId="+Utils::itos(medId));
							return result;
						}
						remain_sum -= num*weight;
						break;
					}
				}				
			}
		}
		else
		{
			result = insertSplitPrescriptionMedicine(preId,newSplitId,medId,num,medHete,cdb);
			if(result != 0)
			{
				rootLogger->error("insertSplitPrescriptionMedicine error,medId="+Utils::itos(medId));
				return result;
			}
		}
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--splitPrescription error ");
		return -1;
	}
}
int CBIZBaseService::pushBackInMedList(list<MedicineInfo>& medList,MedicineInfo medInfo)
{
	try{
	list<MedicineInfo>::iterator ptr = medList.begin();
	for (;ptr != medList.end();ptr++)
	{
		if (ptr->position == medInfo.position)
		{
			ptr->count += medInfo.count;
			return 0;
		}
	}
	medList.push_back(medInfo);
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--pushBackInMedList error ");
		return -1;
	}
}
int CBIZBaseService::insertSplitPrescription(int preId,CWorkRec rec,int& splitId,CDBManage* cdb)
{
	try{
	string sSQL = "insert into prescription_split (pre_id) values("+Utils::itos(preId)+")";
	rootLogger->info("sql = "+sSQL);
	int result = 0;
	result = cdb->execSQL(sSQL);
	if (result != 0)
	{
		rootLogger->error("insert prescription_split error!!");
		return result;
	}

	sSQL = "select split_id from prescription_split where pre_id ="+Utils::itos(preId);
	sSQL +=" order by split_id desc ";
	rootLogger->info("sql = "+sSQL);
	result = cdb->execSQL(sSQL);
	if (result != 0)
	{
		rootLogger->error("get current split_id error!!");
		return result;
	}
	string ssplitId;
	if(!cdb->isEnd())
		ssplitId=cdb->getFieldValueByIndex(0);
	cdb->freeRecord();
	if(ssplitId.empty())
	{
		rootLogger->error("can not find split_id");
		return -1;
	}
	splitId = atoi(ssplitId.c_str());

	sSQL ="insert into work_queue(pre_id,split_id,pre_his_id,pre_state,checkin_time,fetch_state,mz_no,patient_name,patient_id,department_name)values('";
	sSQL +=Utils::itos(rec.getPreId())+"','";
	sSQL +=ssplitId+"','";
	sSQL +=rec.getPre_his_id()+"',0,now(),0,'";
	sSQL +=rec.getMzNo()+"','";
	sSQL +=rec.getPatientName()+"','";
	sSQL +=Utils::itos(rec.getPatientId())+"','";
	sSQL +=rec.getDepartmentName()+"')";
	rootLogger->info("sql = "+sSQL);
	result = cdb->execSQL(sSQL);
	if (result != 0)
	{
		rootLogger->error(sSQL);
		rootLogger->error("insert work_queue error!!");
		return result;
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--insertSplitPrescription error ");
		return -1;
	}
}

int CBIZBaseService::insertSplitPrescriptionMedicine(int pre_id,int splitId,int medId,int medNum,double medHete,CDBManage* cdb)
{
	try{
	string sSQL = "insert into prescription_split_medicine_list (pre_id,split_id,med_id,med_num,med_heteromorphism) values(";
	sSQL += Utils::itos(pre_id)+",";
	sSQL += Utils::itos(splitId)+",";
	sSQL += Utils::itos(medId)+",";
	sSQL += Utils::itos(medNum)+",";
	sSQL += Utils::dtos(medHete)+")";
	rootLogger->info("sql = "+sSQL);
	int result = cdb->execSQL(sSQL);
	if (result != 0)
	{
		rootLogger->error("insert prescription_split_medicine_list(split) error!!");
		//return result;
	}
	return result;
	}catch (...) {
		rootLogger->fatal("CBIZBaseService--prescription_split_medicine_list error ");
		return -1;
	}
}
