// MedicineManage.cpp: implementation of the CMedicineManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MedicineManage.h"
#include "ConfigUtil.h"
#include "Util.h"
#include "Utils.h"
#include <iterator>

using namespace Utils;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
map<int,MedicinLocationState>CMedicineManage::medlocationstate;
LoggerPtr CMedicineManage::rootLogger = Logger::getLogger("CMedicineManage");
bool CMedicineManage::ini_falg=true;
int CMedicineManage::m_machinetype=0;
int CMedicineManage::m_machineID=0;
string CMedicineManage::locationName="";
string CMedicineManage::max_Muallocation="";
CMedicineManage::CMedicineManage()
{
	try{
	//初始化日志记录器
	string msg = configUtil.getValue("logPropFile");
	PropertyConfigurator::configure(msg);
	mute=new CMutex(FALSE,"CMedicineManage",NULL);
	db=NULL;
	db=new CDBManage();
	if(ini_falg)
	{
		m_machinetype=configUtil.getInt("MachineType");
		m_machineID=configUtil.getInt("MachineID");
		max_Muallocation = Utils::itos(configUtil.getInt("MualAddMedUnit")*320+1);
		string sql="select locationName from machine where machineID='"+Utils::itos(m_machineID)+"'";
		if(db->execSQL(sql)==0 )
		{
			locationName=db->getFieldValueByIndex(0);
		}
		else
		{
			rootLogger->info(sql);
			rootLogger->error("CMedicineManage construct function failure");
			locationName="";
		}
		db->freeRecord();
		ini_falg=false;
	    initializeMedicinLocation();
	}
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--CMedicineManage error.");
		rootLogger->fatal("CMedicineManage--CMedicineManage error ");
	}
}

CMedicineManage::~CMedicineManage()
{
	try{
	delete mute;
	if(db!=NULL)
	{
	    delete db;
	}
	}catch (...) {
		rootLogger->fatal("CMedicineManage--~CMedicineManage error ");
	}
}

//初始化药品位置状态
int CMedicineManage::initializeMedicinLocation()
{
	try{
    //rootLogger->trace("function initializeMedicinLocation start..............");
	if(locationName=="")
	{
		return ERROR_MedicineManage_Initialize;
	}
	string sql="select l.location_id,l.medicine_id,l.machine_id,l.location_type,l.location_state,l.cur_quantity,l.max_capacity,l.warning_percent, "
		" m.med_name_ch,m.med_unit,m.factory_name,m.public_flag from (select * from "+locationName+  " where location_type= '"+Utils::itos(m_machinetype)+
		"' and medicine_id!= -1) l left join medicine m  on l.medicine_id = m.med_id  order by l.location_id";
		
	if(db->execSQL(sql)!=0 )
	{
		db->freeRecord();
		rootLogger->info(sql);
		rootLogger->error("initializeMedicinLocation function failure");
		return ERROR_MedicineManage_Initialize;
	}
	if(!medlocationstate.empty())
		    medlocationstate.clear();
	if(db->getRowNum() != 0)
	{
		while(!db->isEnd())
		{
			MedicinLocationState medLocation;
			if(!db->getFieldValueByIndex(0).empty())
				medLocation.location_id=atoi(db->getFieldValueByIndex(0).c_str());
			else
				continue;
			if(!db->getFieldValueByIndex(1).empty())
				medLocation.medicine_id=atoi(db->getFieldValueByIndex(1).c_str());
			if(!db->getFieldValueByIndex(2).empty())
				medLocation.machine_id=atoi(db->getFieldValueByIndex(2).c_str());
			if(!db->getFieldValueByIndex(3).empty())
				medLocation.location_type=atoi(db->getFieldValueByIndex(3).c_str());
			if(!db->getFieldValueByIndex(4).empty())
				medLocation.location_state=atoi(db->getFieldValueByIndex(4).c_str());
			else
				medLocation.location_state=0;
			if(!db->getFieldValueByIndex(5).empty())
				medLocation.cur_quantity=atoi(db->getFieldValueByIndex(5).c_str());
			else
				medLocation.cur_quantity=0;
			if(!db->getFieldValueByIndex(6).empty())
				medLocation.max_capacity=atoi(db->getFieldValueByIndex(6).c_str());
			else
				medLocation.max_capacity=0;
			if(!db->getFieldValueByIndex(7).empty())
				medLocation.warning_percent=atoi(db->getFieldValueByIndex(7).c_str());
			else
                medLocation.warning_percent=30;
			medLocation.med_name =db->getFieldValueByIndex(8);
			medLocation.med_unit =db->getFieldValueByIndex(9);
			medLocation.med_factory =db->getFieldValueByIndex(10);
            medLocation.med_publicflag=atoi(db->getFieldValueByIndex(11).c_str());
			medlocationstate[medLocation.location_id]=medLocation;
			db->nextRow();
		}
	}
	db->freeRecord();
	/*map<int,MedicinLocationState>::iterator iter;
	for(iter=medlocationstate.begin();iter!=medlocationstate.end();iter++)
	{
		QueryMedleavingPercent(iter->second.medicine_id,iter->second.med_warning_falg);
	}*/
	//rootLogger->trace("function initializeMedicinLocation end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--initializeMedicinLocation error.");
		rootLogger->fatal("CMedicineManage--initializeMedicinLocation error ");
		return -1;
	}
}

/********************************************************************************************************************************/
/*输出药品位置状态

/*输出：药品位置状态地址pmedlocationstate包括(关键字：位置ID；关键字值：位置ID location_id,药品ID medicine_id,机器ID machine_id,

        位置类型 location_type,位置状态 location_state,当前数量 cur_quantity, 最大数量 max_capacity,警告比例 warning_percent) 
/********************************************************************************************************************************/
int CMedicineManage::getMedicinLocation(map<int,MedicinLocationState>*pmedlocationstate)
{
	try{
    //rootLogger->trace("function getMedicinLocation start..............");
	*pmedlocationstate=medlocationstate;
	//rootLogger->trace("function getMedicinLocation end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--getMedicinLocation error.");
		rootLogger->fatal("CMedicineManage--getMedicinLocation error ");
		return -1;
	}
}

//修品药品位置状态,falg=0 药品位置停用  falg=1 药品位置启用
int CMedicineManage::modifyMedicinLocation(list<int>location,int falg)
{
	try{
    //rootLogger->trace("function modifyMedicinLocation start..............");
	list<int>::iterator iter;
	map<int,MedicinLocationState>::iterator temp;
	for(iter=location.begin();iter!=location.end();iter++)
	{
		temp=medlocationstate.find(*iter);
		if(temp!=medlocationstate.end())
		{
			temp->second.location_state=falg;
			string sql="update "+locationName+" set location_state='"+Utils::itos(falg)+"' where location_id='"+Utils::itos(*iter)+"'";
			if(db->execSQL(sql)!=0)
			{
                rootLogger->info(sql);
				rootLogger->error("modifyMedicinLocation function failure");
				return ERROR_MedicineManage_modifyMedicinLocation;
			}
			//QueryMed_allLocationleavingPercent(temp->second.medicine_id);
		}
	}
	//rootLogger->trace("function modifyMedicinLocation end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--modifyMedicinLocation error.");
		rootLogger->fatal("CMedicineManage--modifyMedicinLocation error ");
		return -1;
	}
}

/********************************************************************************************************************************/
/*发药时修改药品位置状态

/*输入：药品位置状态medLocationState包括(关键字：位置ID；关键字值：位置ID location_id,药品ID medicine_id,机器ID machine_id,

        位置类型 location_type,位置状态 location_state,当前数量 cur_quantity, 最大数量 max_capacity,警告比例 warning_percent) 
/********************************************************************************************************************************/
int CMedicineManage::popMedicinLocation(map<int,MedicinLocationState>&medLocationState)
{
    //rootLogger->trace("function popMedicinLocation start..............");
	mute->Lock();
	map<int,MedicinLocationState>::iterator iter;
	map<int,MedicinLocationState>::iterator temp;
	for(iter=medLocationState.begin();iter!=medLocationState.end();iter++)
	{
	    temp=medlocationstate.find(iter->first);
		if(temp!=medlocationstate.end())
		{
			temp->second.cur_quantity-=iter->second.cur_quantity;
			if(temp->second.cur_quantity<0)
			{
				temp->second.cur_quantity=0;
			}
			string sql="update "+locationName+" set cur_quantity='"+Utils::itos(temp->second.cur_quantity)+"' where location_id='"+Utils::itos(iter->first)+"'";
			if(db->execSQL(sql)!=0)
			{
				rootLogger->info(sql);
				rootLogger->error("popMedicinLocation function failure");
				mute->Unlock();
                return ERROR_MedicineManage_PopMedicinLocation;
			}
			//QueryMed_allLocationleavingPercent(temp->second.medicine_id);
		}
		else
		{
            rootLogger->error("location is not exist,location="+ Utils::itos(iter->first));
			mute->Unlock();
			return ERROR_MedicineManage_PopMedicinLocation;
		}
	}
	mute->Unlock();
	//rootLogger->trace("function popMedicinLocation end..............");
	return 0;
}

/********************************************************************************************************************************/
/*加药时修改药品位置状态

/*输入：药品位置状态medLocationState包括(关键字：位置ID；关键字值：位置ID location_id,药品ID medicine_id,机器ID machine_id,

        位置类型 location_type,位置状态 location_state,当前数量 cur_quantity, 最大数量 max_capacity,警告比例 warning_percent) 
/********************************************************************************************************************************/
int CMedicineManage::pushMedicinLocation(map<int,MedicinLocationState>&medLocationState)
{
    //rootLogger->trace("function pushMedicinLocation start..............");
	mute->Lock();
	map<int,MedicinLocationState>::iterator iter;
	map<int,MedicinLocationState>::iterator temp;
	for(iter=medLocationState.begin();iter!=medLocationState.end();iter++)
	{
	    temp=medlocationstate.find(iter->first);
		if(temp!=medlocationstate.end())
		{
			temp->second.cur_quantity+=iter->second.cur_quantity;
			if(temp->second.cur_quantity >temp->second.max_capacity)
				temp->second.cur_quantity=temp->second.max_capacity;
			string sql="update "+locationName+" set cur_quantity='"+Utils::itos(temp->second.cur_quantity)+"' where location_id='"+Utils::itos(iter->first)+"'";
			if(db->execSQL(sql)!=0)
			{
                rootLogger->info(sql);
				rootLogger->error("pushMedicinLocation function failure");
				mute->Unlock();
				return ERROR_MedicineManage_PushMedicinLocation;
			}
			//QueryMed_allLocationleavingPercent(temp->second.medicine_id);
		}	
		else
		{
            rootLogger->error("location is not exist,location="+ Utils::itos(iter->first));
			mute->Unlock();
			return ERROR_MedicineManage_PushMedicinLocation;
		}
	}
	mute->Unlock();
	//rootLogger->trace("function pushMedicinLocation end..............");
	return 0;
}

//查找药品的地址列表
int CMedicineManage::addressSearch(int medID,list<MedicinLocationState>*listAddress)
{
	try{
    //rootLogger->trace("function addressSearch start..............");
	string sql="select location_id,medicine_id,machine_id,location_type,location_state,cur_quantity,max_capacity,warning_percent from "+locationName+" where medicine_id='"+Utils::itos(medID) +"' order by location_id";
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("addressSearch function failure");
		return ERROR_MedicineManage_AddressSearch;
	}
	while(!db->isEnd())
	{
        MedicinLocationState medLocation;
		if(!db->getFieldValueByIndex(0).empty())
			medLocation.location_id=atoi(db->getFieldValueByIndex(0).c_str());
		else
			continue;
		if(!db->getFieldValueByIndex(1).empty())
			medLocation.medicine_id=atoi(db->getFieldValueByIndex(1).c_str());
		if(!db->getFieldValueByIndex(2).empty())
			medLocation.machine_id=atoi(db->getFieldValueByIndex(2).c_str());
		if(!db->getFieldValueByIndex(3).empty())
			medLocation.location_type=atoi(db->getFieldValueByIndex(3).c_str());
		if(!db->getFieldValueByIndex(4).empty())
			medLocation.location_state=atoi(db->getFieldValueByIndex(4).c_str());
		else
			medLocation.location_state=0;
		if(!db->getFieldValueByIndex(5).empty())
			medLocation.cur_quantity=atoi(db->getFieldValueByIndex(5).c_str());
		else
			medLocation.cur_quantity=0;
		if(!db->getFieldValueByIndex(6).empty())
			medLocation.max_capacity=atoi(db->getFieldValueByIndex(6).c_str());
		else
			medLocation.max_capacity=0;
		if(!db->getFieldValueByIndex(7).empty())
			medLocation.warning_percent=atoi(db->getFieldValueByIndex(7).c_str());
		listAddress->push_back(medLocation);
		db->nextRow();
	}
	db->freeRecord();
	//rootLogger->trace("function addressSearch end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--addressSearch error.");
		rootLogger->fatal("CMedicineManage--addressSearch error ");
		return -1;
	}
}

//获取药品当前所有剩余的数量
int CMedicineManage::getCurrentMedicineCount(int medID,list<MedinceCount>* medincecount,int flag)
{
	try{
    //rootLogger->trace("function getCurrentMedicineCount start..............");

	string sql="select b.machineID ,b.machine_state,b.machineType,b.locationName from medicine a, machine b where a.med_type=b.machineType and a.med_id='"+Utils::itos(medID)+"'";
	if(flag==1)
	{
		sql=sql+" and b.machineID= '" +Utils::itos(m_machineID)+"'";
	}
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("getCurrentMedicineCount function failure");
		return ERROR_MedicineManage_GetMedicineCount;
	}
	while(!db->isEnd())
	{
		MedinceCount temp;	
		temp.deviceID=atoi(db->getFieldValueByIndex(0).c_str());
		temp.machineState=atoi(db->getFieldValueByIndex(1).c_str());
		temp.deviceType=atoi(db->getFieldValueByIndex(2).c_str());
		strcpy(temp.locationName,db->getFieldValueByIndex(3).c_str());
		medincecount->push_back(temp);
		db->nextRow();
	}
	db->freeRecord();
	list<MedinceCount>::iterator iter;
	for(iter=medincecount->begin(); iter !=medincecount->end(); iter++)
	{
        string locationName=iter->locationName;
		string sql_location="select sum(cur_quantity),sum(max_capacity),max_capacity from "+locationName+" where location_state=1 and medicine_id='"+Utils::itos(medID)+"'group by medicine_id";
		if(db->execSQL(sql_location)!=0)
		{
			rootLogger->info(sql_location);
			db->freeRecord();
			rootLogger->error("getCurrentMedicineCount function failure");
			return ERROR_MedicineManage_GetMedicineCount;
		}
		if(!db->isEnd())
		{
			iter->medCount=atoi(db->getFieldValueByIndex(0).c_str());
			iter->medMaxCount=atoi(db->getFieldValueByIndex(1).c_str());
			iter->max_capacity=atoi(db->getFieldValueByIndex(2).c_str());
		}
		else
		{
			iter->medCount=0;
			iter->medMaxCount=0;
		}
		db->freeRecord();
	}
	//rootLogger->trace("function getCurrentMedicineCount end..............");
	return 0; 
	}catch (...) {
		rootLogger->fatal("CMedicineManage--getCurrentMedicineCount error ");
		return -1;
	}
}

//编码快速查找
int CMedicineManage::fastSearch(int type,string input, list<string >* output)
{
	try{
    //rootLogger->trace("function fastSearch start..............");
	string sql;
	if(!input.empty() )
	{
		sql="select med_name_ch from medicine where med_name_ch like '%"+input+"%' "
			" or med_py_code like '%"+input+"%' or  med_cust_code like '%"+input+"%'" +" order by med_name_ch";
	}
	if(type==FAST_TYPE_1)
	{
		if(sql.empty())
		{
			sql="select a.med_name_ch from medicine a where a.med_id in (select medicine_id from "+ locationName +" where location_id < '"+max_Muallocation+"') order by med_name_ch ";
		}
	}
	else if(type==FAST_TYPE_2)
	{
		if(sql.empty())
		{
			sql="select med_name_ch from medicine where med_type in ('"+Utils::itos(m_machinetype)+"','0') order by med_name_ch";
		}
	}
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("fastSearch function failure");
		return ERROR_MedicineManage_FastSearch;
	}
	while(!db->isEnd())
	{
		string rlt = db->getFieldValueByIndex(0);
        output->push_back(rlt);
        db->nextRow();
	}
	db->freeRecord();
	//rootLogger->trace("function fastSearch end..............");
	return 0;
	}catch (...) {
		rootLogger->fatal("CMedicineManage--fastSearch error ");
		return -1;
	}
}

//查找药品列表
int CMedicineManage::listSearch(list<MedicinList>*pmedicinList,string medicineName, int type,BOOL medstate)
{
	try{
    //rootLogger->trace("function listSearch start..............");
	string str="";
	string sql;
	if(!medicineName.empty() )
	{
        str=" med_name_ch like '%"+medicineName+"%' or med_py_code like '%"+medicineName+"%'  or med_cust_code like '%"+medicineName+"%' ";
	}

    if(str.empty())
	{
		if(type==2)
		{
			sql="select a.med_id,a.med_his_id,a.med_name_ch,a.med_type,a.warning_percent,b.sum_cur_quantity,b.sum_max_capacity,a.med_unit,a.med_state,a.factory_name,a.med_py_code from "
				"(select medicine_id,sum(cur_quantity) as sum_cur_quantity,sum(max_capacity) as sum_max_capacity from "+locationName+" where location_state =1 and medicine_id <>-1 and location_id < "+max_Muallocation+" group by medicine_id) as b "
				" right outer join (select med_id,med_his_id,med_name_ch,med_type, warning_percent,med_unit,med_state,factory_name,med_py_code from medicine where med_type in ('"+Utils::itos(m_machinetype)+"') and med_state= '"+Utils::itos(medstate)+"') as a on a.med_id=b.medicine_id"
				" order by a.med_name_ch ";
		}
		else
		{
			sql="select a.med_id,a.med_his_id,a.med_name_ch,a.med_type,a.warning_percent,b.sum_cur_quantity,b.sum_max_capacity,a.med_unit,a.med_state,a.factory_name,a.med_py_code from "
				"(select medicine_id,sum(cur_quantity) as sum_cur_quantity,sum(max_capacity) as sum_max_capacity from "+locationName+" where medicine_id <>-1  group by medicine_id) as b "
				" right outer join (select med_id,med_his_id,med_name_ch,med_type, warning_percent,med_unit,med_state,factory_name,med_py_code from medicine where med_type in ('"+Utils::itos(m_machinetype)+"','0') and med_state= '"+Utils::itos(medstate)+"') as a on a.med_id=b.medicine_id"
				" order by a.med_name_ch ";
		}
	}
	else
	{
		if(type==2)
		{
			sql="select a.med_id,a.med_his_id,a.med_name_ch,a.med_type,a.warning_percent,b.sum_cur_quantity,b.sum_max_capacity,a.med_unit,a.med_state,a.factory_name,a.med_py_code from "
				"(select medicine_id,sum(cur_quantity) as sum_cur_quantity,sum(max_capacity) as sum_max_capacity from "+locationName+" where location_state =1 and medicine_id <>-1  and location_id < "+max_Muallocation+" group by medicine_id) as b "
				" right outer join (select med_id,med_his_id,med_name_ch,med_type, warning_percent,med_unit,med_state,factory_name,med_py_code from medicine where med_type in ('"+Utils::itos(m_machinetype)+"') and med_state= '"+Utils::itos(medstate)+"' and "+str+") as a on a.med_id=b.medicine_id"
				" order by a.med_name_ch ";
		}
		else
		{
			sql="select a.med_id,a.med_his_id,a.med_name_ch,a.med_type,a.warning_percent,b.sum_cur_quantity,b.sum_max_capacity,a.med_unit,a.med_state,a.factory_name,a.med_py_code from "
				"(select medicine_id,sum(cur_quantity) as sum_cur_quantity,sum(max_capacity) as sum_max_capacity from "+locationName+" where medicine_id <>-1  group by medicine_id) as b "
				" right outer join (select med_id,med_his_id,med_name_ch,med_type, warning_percent,med_unit,med_state,factory_name,med_py_code from medicine where med_type in ('"+Utils::itos(m_machinetype)+"','0') and med_state= '"+Utils::itos(medstate)+"' and "+str+") as a on a.med_id=b.medicine_id"
				" order by a.med_name_ch ";
		}
	}
  	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("listSearch function failure");
		return ERROR_MedicineManage_ListSearch;
	}
	while(!db->isEnd())
	{
		MedicinList medlist;
		int warning_percent;
		if(!db->getFieldValueByIndex(0).empty())
            medlist.medicineId=atoi(db->getFieldValueByIndex(0).c_str());
		else
			continue;
		medlist.medicineHisId=db->getFieldValueByIndex(1);
		medlist.medicineNameCH=db->getFieldValueByIndex(2);
		if(!db->getFieldValueByIndex(3).empty())
		    medlist.medicineType=atoi(db->getFieldValueByIndex(3).c_str());
		
		if(db->getFieldValueByIndex(4).empty())
            warning_percent=30;
		else
			warning_percent=atoi(db->getFieldValueByIndex(4).c_str());
		
		if(!db->getFieldValueByIndex(5).empty())
            medlist.leavingQuantity=atoi(db->getFieldValueByIndex(5).c_str());
		else
			medlist.leavingQuantity=0;
		
		if(!db->getFieldValueByIndex(6).empty())
		    medlist.medSum=atoi(db->getFieldValueByIndex(6).c_str());
		else
			medlist.medSum=0;

		if(medlist.medSum-medlist.leavingQuantity>=0)
		    medlist.addmedcount=medlist.medSum-medlist.leavingQuantity;
		else
			medlist.addmedcount=0;

		medlist.medUnit=db->getFieldValueByIndex(7).c_str();
		
		if(medlist.medSum==0)
            medlist.leavingPercent=0;
		else
		    medlist.leavingPercent=medlist.leavingQuantity*100/medlist.medSum;

		if(db->getFieldValueByIndex(8)=="0")
			medlist.medicineState="停用";
		else if(medlist.medSum==0)
			medlist.medicineState="未上架";
		else if(medlist.leavingPercent<warning_percent)
		    medlist.medicineState="缺药";
		else
            medlist.medicineState="正常";
        medlist.medFactory=db->getFieldValueByIndex(9);
		medlist.medPyCode=db->getFieldValueByIndex(10);
		pmedicinList->push_back(medlist);
		db->nextRow();
	}
    db->freeRecord();
	//rootLogger->trace("function listSearch end..............");
	return 0;
	}catch (...) {
		rootLogger->fatal("CMedicineManage--listSearch error ");
		return -1;
	}
}

//查找药品详细信息
int CMedicineManage::lookMedicine(int medID, MedInfo *pmedicineinfo)
{
	try{
    //rootLogger->trace("function lookMedicine start..............");
	string sql_location="select sum(cur_quantity),sum(max_capacity) from "+locationName+" where medicine_id='"+Utils::itos(medID)+"'group by medicine_id";
    if(db->execSQL(sql_location)!=0)
	{
		rootLogger->info(sql_location);
		rootLogger->error("lookMedicine function failure");
		return ERROR_MedicineManage_LookMedicine;
	}
	if(!db->isEnd())
	{
        if(!db->getFieldValueByIndex(0).empty())
		    pmedicineinfo->curQuantity=atoi(db->getFieldValueByIndex(0).c_str());
		else
            pmedicineinfo->curQuantity=0;
		if(!db->getFieldValueByIndex(1).empty())
	        pmedicineinfo->sumQuantity=atoi(db->getFieldValueByIndex(1).c_str());
		else
			pmedicineinfo->sumQuantity=0;
	}
	else
	{
        pmedicineinfo->curQuantity=0;
	    pmedicineinfo->sumQuantity=0;
	}
	db->freeRecord();
	list<MedicinLocationState> locationlist;
	if(this->addressSearch(medID,&locationlist)==0)
		pmedicineinfo->locationlist=locationlist;
	else
	{
		rootLogger->error("lookMedicine function failure");
		return ERROR_MedicineManage_LookMedicine;
	}
	string sql_med="select * from medicine where med_id='"+Utils::itos(medID)+"'";
	if(db->execSQL(sql_med)!=0)
	{
		rootLogger->info(sql_med);
		db->freeRecord();
		rootLogger->error("lookMedicine function failure");
		return ERROR_MedicineManage_LookMedicine;
	}
	if(!db->isEnd())
	{
		if(!db->getFieldValueByName("med_id").empty())
		    pmedicineinfo->medicineId=atoi(db->getFieldValueByName("med_id").c_str());
		else
			pmedicineinfo->medicineId=0;
		pmedicineinfo->medicineHisId=db->getFieldValueByName("med_his_id").c_str();
		pmedicineinfo->medicineNameCH=db->getFieldValueByName("med_name_ch");
		pmedicineinfo->medicineNameEN=db->getFieldValueByName("med_name_en");
		//pmedicineinfo->numCode=db->getFieldValueByName("med_num_code");
		pmedicineinfo->pyCode=db->getFieldValueByName("med_py_code");
		//pmedicineinfo->stdCode=db->getFieldValueByName("med_std_code");
		pmedicineinfo->custCode=db->getFieldValueByName("med_cust_code");
		//pmedicineinfo->medicineType=atoi(db->getFieldValueByIndex(8).c_str());
		pmedicineinfo->medicineUnit=db->getFieldValueByName("med_unit");
		if(!db->getFieldValueByName("med_state").empty())
		    pmedicineinfo->medicineState=atoi(db->getFieldValueByName("med_state").c_str());
		else
			pmedicineinfo->medicineState=1;
		//pmedicineinfo->medicineHint=db->getFieldValueByName("med_hint");
		pmedicineinfo->maxCapacity=atoi(db->getFieldValueByName("max_capacity").c_str());
		if(!db->getFieldValueByName("max_num_once").empty())
		    pmedicineinfo->maxNumOnce=atoi(db->getFieldValueByName("max_num_once").c_str());
		else
			pmedicineinfo->maxNumOnce=10;
		if(!db->getFieldValueByName("warning_percent").empty())
		    pmedicineinfo->warningPercent=atoi(db->getFieldValueByName("warning_percent").c_str());
		else
			pmedicineinfo->warningPercent=30;
		pmedicineinfo->medFactory=db->getFieldValueByName("factory_name");
		pmedicineinfo->typeUnit=db->getFieldValueByName("type_unit");
		pmedicineinfo->medlength=db->getFieldValueByName("med_length");
		pmedicineinfo->medwidth=db->getFieldValueByName("med_width");
		pmedicineinfo->medhigth=db->getFieldValueByName("med_higth");
		pmedicineinfo->valiDateTime=db->getFieldValueByName("validate_time");
		pmedicineinfo->med_batch=db->getFieldValueByName("med_batch");
	}
	db->freeRecord();
	//rootLogger->trace("function lookMedicine end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--lookMedicine error.");
		rootLogger->fatal("CMedicineManage--lookMedicine error ");
		return -1;
	}
}

//修改药品详细信息
int CMedicineManage::modifyMedicine(int medID,MedInfo &medicineinfo)
{
	try{
    //rootLogger->trace("function modifyMedicine start..............");
	string sql_med;
	if(medicineinfo.pyCode.empty())
	{
		medicineinfo.pyCode = Utils::ConvertHZToPY((char*)medicineinfo.medicineNameCH.c_str());
	}
	sql_med="update medicine set "
			   //med_name_ch='"+medicineinfo.medicineNameCH+"',"
			   //"med_name_en='"+medicineinfo.medicineNameEN+"',"
		       "med_cust_code='"+medicineinfo.custCode+"',"
			   "med_py_code='"+medicineinfo.pyCode+"'," 
			   //"med_std_code='"+medicineinfo.stdCode+"',"
			   //"med_num_code='"+medicineinfo.numCode+"'," 
			   "med_type='"+Utils::itos(medicineinfo.medicineType)+"'," 
			   //"med_unit='"+medicineinfo.medicineUnit+"',"
			   "med_state='"+Utils::itos(medicineinfo.medicineState)+"'," 
			   //"med_hint='"+medicineinfo.medicineHint+"',"
			   "max_capacity='"+Utils::itos(medicineinfo.maxCapacity)+"',"
			   "max_num_once='"+Utils::itos(medicineinfo.maxNumOnce)+"'," 
			   "warning_percent='"+Utils::itos(medicineinfo.warningPercent)+"',"
			   //" factory_name='"+medicineinfo.medFactory+"'," 
			   //"type_unit='"+medicineinfo.typeUnit+"',"
			   "med_length='"+medicineinfo.medlength +"',"
			   "med_width='"+medicineinfo.medwidth +"'," 
			   "med_higth='"+medicineinfo.medhigth+"'"
			   //validate_time='"+medicineinfo.valiDateTime+"',"
			   //"med_batch='"+medicineinfo.med_batch+"'"
			   " where  med_id='"+Utils::itos(medID)+"'";
	if(db->execSQL(sql_med)!=0)
	{
	    rootLogger->info(sql_med);
		rootLogger->error("modifyMedicine function failure");
		return ERROR_MedicineManage_ModifyMedicine;
	}
    mute->Lock();
	list<MedicinLocationState>::iterator temp;
	for(temp=medicineinfo.locationlist.begin();temp!=medicineinfo.locationlist.end();temp++)
	{
        string sql_location="update "+locationName+" set medicine_id='"+Utils::itos(medID)+"',"
			        "location_type='"+Utils::itos(m_machinetype)+"',"
			        "location_state='"+Utils::itos(temp->location_state)+"',"
			        "max_capacity='"+Utils::itos(temp->max_capacity)+"',warning_percent='"+Utils::itos(temp->warning_percent)+"'"
			        "where location_id='"+Utils::itos(temp->location_id)+"'" ;
		if(db->execSQL(sql_location)!=0)
		{
		    rootLogger->info(sql_location);
			rootLogger->error("modifyMedicine function failure");
			mute->Unlock();
			return ERROR_MedicineManage_ModifyMedicine;
		}

	}
	if(this->initializeMedicinLocation()!=0)
	{
		rootLogger->error("modifyMedicine function failure");
		mute->Unlock();
		return ERROR_MedicineManage_ModifyMedicine;
	}
	mute->Unlock();
	//rootLogger->trace("function modifyMedicine end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--modifyMedicine error.");
		rootLogger->fatal("CMedicineManage--modifyMedicine error ");
		return -1;
	}
}

//新增药品详细信息
int CMedicineManage::addMedicine(MedInfo &medicineinfo)
{
	try{
    //rootLogger->trace("function addMedicine start..............");
	string sql_med;
	if(medicineinfo.medicineHisId=="")
	{
		sql_med="insert into medicine("
		       "med_id,"
			   "med_name_ch,"
			   "med_name_en,"
			   //"med_num_code,"
			   "med_py_code,"
			   //"med_std_code,"
			   "med_cust_code,"
			   //"med_type,"
			   "med_unit,"
			   "med_state,"
			   //"med_hint,"
			   "max_capacity,"
			   "max_num_once,"
			   "warning_percent,"
			   "factory_name,"
			   "type_unit,"
			   "med_length,"
			   "med_width,"
			   "med_higth,"
			   "validate_time,"
			   "med_batch"
			   ")values(";
	           +medicineinfo.medicineId+","
			   +medicineinfo.medicineNameCH+","
			   +"'"+medicineinfo.medicineNameEN+"',"
		       //+"'"+medicineinfo.numCode+"',"
			   +"'"+medicineinfo.pyCode+"',"
			   //+"'"+medicineinfo.stdCode+"',"
			   +"'"+medicineinfo.custCode+"',"
			   //+Utils::itos(medicineinfo.medicineType)+","
			   +"'"+medicineinfo.medicineUnit+"',"
			   +Utils::itos(medicineinfo.medicineState)+","
			   //+"'"+medicineinfo.medicineHint+"',"
			   +Utils::itos(medicineinfo.maxCapacity)+","
			   +Utils::itos(medicineinfo.maxNumOnce)+","
			   +Utils::itos(medicineinfo.warningPercent)+","
			   +"'"+medicineinfo.medFactory+"',"
			   +"'"+medicineinfo.typeUnit+"',"
			   +"'"+medicineinfo.medlength+"',"
			   +"'"+medicineinfo.medwidth+"',"
			   +"'"+medicineinfo.medhigth+"',"
			   +"'"+medicineinfo.valiDateTime+"',"
			   +"'"+medicineinfo.med_batch+"'"
			   +")";
	}
	else
	{
		sql_med="insert into medicine("
		       "med_id,"
			   "med_his_id,"
			   "med_name_ch,"
			   "med_name_en,"
			   //"med_num_code,"
			   "med_py_code,"
			   //"med_std_code,"
			   "med_cust_code,"
			   //"med_type,"
			   "med_unit,"
			   "med_state,"
			   //"med_hint,"
			   "max_capacity,"
			   "max_num_once,"
			   "warning_percent,"
			   "factory_name,"
			   "type_unit,"
			   "med_length,"
			   "med_width,"
			   "med_higth,"
			   "validate_time,"
			   "med_batch"
			   ")values("
			   +Utils::itos(medicineinfo.medicineId)+","
			   +medicineinfo.medicineHisId+","
			   +"'"+medicineinfo.medicineNameCH+"',"
			   +"'"+medicineinfo.medicineNameEN+"',"
		       //+"'"+medicineinfo.numCode+"',"
			   +"'"+medicineinfo.pyCode+"',"
			   //+"'"+medicineinfo.stdCode+"',"
			   +"'"+medicineinfo.custCode+"',"
			   //+Utils::itos(medicineinfo.medicineType)+","
			   +"'"+medicineinfo.medicineUnit+"',"
			   +Utils::itos(medicineinfo.medicineState)+","
			   //+"'"+medicineinfo.medicineHint+"',"
			   +Utils::itos(medicineinfo.maxCapacity)+","
			   +Utils::itos(medicineinfo.maxNumOnce)+","
			   +Utils::itos(medicineinfo.warningPercent)+","
			   +"'"+medicineinfo.medFactory+"',"
			   +"'"+medicineinfo.typeUnit+"',"
			   +"'"+medicineinfo.medlength+"',"
			   +"'"+medicineinfo.medwidth+"',"
			   +"'"+medicineinfo.medhigth+"',"
			   +"'"+medicineinfo.valiDateTime+"',"
			   +"'"+medicineinfo.med_batch+"'"
			   +")";
	}
	
	if(db->execSQL(sql_med)!=0)
	{
		rootLogger->info(sql_med);
		rootLogger->error("addMedicine function failure");
		return ERROR_MedicineManage_AddMedicine;
	}
	list<MedicinLocationState>::iterator temp;
	for(temp=medicineinfo.locationlist.begin();temp!=medicineinfo.locationlist.end();temp++)
	{
		string sql_location="update "+locationName+" set medicine_id='"+Utils::itos(medicineinfo.medicineId)+"',location_type='"+Utils::itos(m_machinetype)+"',"
			   "location_state='"+Utils::itos(temp->location_state)+"',cur_quantity='"+Utils::itos(temp->cur_quantity)+"',"
			   "max_capacity='"+Utils::itos(temp->max_capacity)+"',warning_percent='"+Utils::itos(temp->warning_percent)+"'"
			   "where location_id='"+Utils::itos(temp->location_id)+"'" ;
		if(db->execSQL(sql_location)!=0)
		{
		    rootLogger->info(sql_location);
			rootLogger->error("addMedicine function failure");
			return ERROR_MedicineManage_AddMedicine;
		}
	}
	if(this->initializeMedicinLocation()!=0)
	{
		rootLogger->error("addMedicine function failure");
		return ERROR_MedicineManage_AddMedicine;
	}
	//rootLogger->trace("function addMedicine end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--addMedicine error.");
		rootLogger->fatal("CMedicineManage--addMedicine error ");
		return -1;
	}
}

int CMedicineManage::updateMedType(int medId,int med_type)
{
	string sql_med="update medicine set "
			  "med_type='"+Utils::itos(med_type)+"'" 
			   " where  med_id='"+Utils::itos(medId)+"'";
	if(db->execSQL(sql_med)!=0)
	{
	    rootLogger->info(sql_med);
		rootLogger->error("updateMedType function failure");
		return ERROR_MedicineManage_ModifyMedicine;
	}
	return 0;
}

//删除药品详细信息
int CMedicineManage::deleteMedicine(int med_id)
{
	try{
    //rootLogger->trace("function deleteMedicine start..............");
	/*mute->Lock();
	list<MedicinLocationState> locationlist;
	if(this->addressSearch(med_id,&locationlist)==0)
	{
		list<MedicinLocationState>::iterator iter;
	    for(iter=locationlist.begin();iter!=locationlist.end();iter++)
		{
	        string sql_location="update location set medicine_id=0,cur_quantity=0,max_capacity=0 "
			        "where location_id='"+Utils::itos(iter->location_id)+"'" ;
	        if(db->execSQL(sql_location)!=0)
			{
	            rootLogger->info(sql_location);
				rootLogger->error("deleteMedicine function failure");
		        return ERROR_MedicineManage_DeleteMedicine;
			} 
		}
	}
	else
	{
        rootLogger->error("deleteMedicine function failure");
		return ERROR_MedicineManage_DeleteMedicine;
	}
	if(this->initializeMedicinLocation()!=0)
	{
		rootLogger->error("deleteMedicine function failure");
		return ERROR_MedicineManage_DeleteMedicine;
	}
    mute->Unlock();*/
	string sql_med="delete from medicine where med_id='"+Utils::itos(med_id)+"'";
	if(db->execSQL(sql_med)!=0)
	{
	    rootLogger->info(sql_med);
		rootLogger->error("deleteMedicine function failure");
		return ERROR_MedicineManage_DeleteMedicine;
	}
	//rootLogger->trace("function deleteMedicine end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--deleteMedicine error.");
		rootLogger->fatal("CMedicineManage--deleteMedicine error ");
		return -1;
	}
}

int CMedicineManage::deletelocation(int locationId)
{
	try{
    //rootLogger->trace("function deletelocation start..............");
	mute->Lock();
	string sql_location="update "+locationName+" set medicine_id=0,cur_quantity=0,max_capacity=0 "
			        "where location_id='"+Utils::itos(locationId)+"'" ;
	if(db->execSQL(sql_location)!=0)
	{
	    rootLogger->info(sql_location);
		rootLogger->error("deletelocation function failure");
		mute->Unlock();
		return -1;
	} 
	map<int,MedicinLocationState>::iterator temp;
	temp=medlocationstate.find(locationId);
	if(temp!=medlocationstate.end())
	{
		//QueryMed_allLocationleavingPercent(temp->second.medicine_id);
		temp->second.medicine_id =0;
		temp->second.cur_quantity=0;
		temp->second.max_capacity=0;
	}
	//rootLogger->trace("function deletelocation end..............");
	mute->Unlock();
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--deletelocation error.");
		rootLogger->fatal("CMedicineManage--deletelocation error ");
		return -1;
	}
}

//获取最大的药品号
int CMedicineManage::getMaxmedID(int &medID)
{
	try{
    //rootLogger->trace("function getMaxmedID start..............");
	string sql="select max(med_id) from medicine";
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("function getMaxmedID failure");
		return ERROR_MedicineManage_getMaxmedID;
	}
	if(!db->isEnd() && !db->getFieldValueByIndex(0).empty())
		medID=atoi(db->getFieldValueByIndex(0).c_str());
	db->freeRecord();
	//rootLogger->trace("function getMaxmedID end..............");
	return 0;	
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--getMaxmedID error.");
		rootLogger->fatal("CMedicineManage--getMaxmedID error ");
		return -1;
	}
}

//获取药品的权重
int CMedicineManage::getMedWeight(int medID,int &weight)
{
	try{
    //rootLogger->trace("function getMedWeight start..............");
	string sql="select weight from medicine where med_id = " +Utils::itos(medID);
	if(db->execSQL(sql)!=0)
	{
		weight=10;
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("function getMedWeight failure");
		return ERROR_MedicineManage_getMedWeight;
	}
	if(!db->isEnd())
	{
		if( db->getFieldValueByIndex(0).empty() )
		{
			weight=10;
		}
		else
		{
		    weight=atoi(db->getFieldValueByIndex(0).c_str());
		}
	}
	db->freeRecord();
	//rootLogger->trace("function getMedWeight end..............");
	return 0;	
	}catch (...) {
		rootLogger->fatal("CMedicineManage--getMedWeight error ");
		return -1;
	}
}

//清空药品所有地址信息
int CMedicineManage::clearAllLocation(int medId)
{
	try{
    //rootLogger->trace("function clearAllLocation start..............");
	mute->Lock();
	list<MedicinLocationState> locationlist;
	if(this->addressSearch(medId,&locationlist)==0)
	{
		list<MedicinLocationState>::iterator iter;
	    for(iter=locationlist.begin();iter!=locationlist.end();iter++)
		{
			if(iter->location_state != 1)
				continue;
	        string sql_location="update "+locationName+" set cur_quantity=0 "
			        "where location_id='"+Utils::itos(iter->location_id)+"'" ;
	        if(db->execSQL(sql_location)!=0)
			{
	            rootLogger->info(sql_location);
				rootLogger->error("clearAllLocation function failure");
				mute->Unlock();
		        return ERROR_MedicineManage_ClearAllLocation;
			} 
			map<int,MedicinLocationState>::iterator temp;
			temp=medlocationstate.find(iter->location_id);
			if(temp!=medlocationstate.end())
			{
				temp->second.cur_quantity=0;
			}
		}
	}
	else
	{
        rootLogger->error("clearAllLocation function failure");
		mute->Unlock();
		return ERROR_MedicineManage_ClearAllLocation;
	}
	//QueryMed_allLocationleavingPercent(medId);
    mute->Unlock();
	//rootLogger->trace("function clearAllLocation end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--clearAllLocation error.");
		rootLogger->fatal("CMedicineManage--clearAllLocation error ");
		return -1;
	}
}

//获取药品ID号
int CMedicineManage::getmedID(int locationID, int &medID)
{
	try{
    //rootLogger->trace("function getmedID start..............");
	string sql="select medicine_id from "+locationName+" where location_id= '"+Utils::itos(locationID)+"'" ;
	if(db->execSQL(sql)!=0 )
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("getmedID function failure");
		return ERROR_MedicineManage_getmedID;
	}
	if(!db->isEnd() && !db->getFieldValueByIndex(0).empty())
		medID=atoi(db->getFieldValueByIndex(0).c_str());
	db->freeRecord();
	//rootLogger->trace("function getmedID end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--getmedID error.");
		rootLogger->fatal("CMedicineManage--getmedID error ");
		return -1;
	}
}

//手持终端查询药品信息
int CMedicineManage::QueryLocationMedInfo_Client(string medscan, ClientMedinceLocation * clientmedincelocation)
{
	try{	
	//rootLogger->trace("function QueryLocationMedInfo_Client start..............");
	string sql="select med_id,med_name_ch,med_unit,med_type from medicine where  med_state=1 and med_cust_code='"+medscan +"'";
	if(db->execSQL(sql)!=0 ||db->getRowNum()==0)
	{
		rootLogger->info(sql);
		rootLogger->error("QueryLocationMedInfo_Client function failure");
		return ERROR_MedicineManage_QueryMedInfo_Client;
	}
	string medID=db->getFieldValueByIndex(0);	
	strcpy(clientmedincelocation->medicineNameCH,db->getFieldValueByIndex(1).c_str());
	strcpy(clientmedincelocation->medicineUnit,db->getFieldValueByIndex(2).c_str());
	clientmedincelocation->medtype=atoi(db->getFieldValueByIndex(3).c_str());
	db->freeRecord();
	sql="select location_id,cur_quantity,max_capacity from "+locationName+" where location_state=1 and medicine_id='"+medID+"' and location_id < "+max_Muallocation+" order by location_id ";
	if(db->execSQL(sql)!=0 )
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("QueryLocationMedInfo_Client function failure");
		return ERROR_MedicineManage_QueryMedInfo_Client;
	}

	int sum_cur_quantity=0,sum_max_capacity=0,count=0;
	string str="";
	while(!db->isEnd())
	{
		if(!db->getFieldValueByIndex(1).empty())
		    sum_cur_quantity+=atoi(db->getFieldValueByIndex(1).c_str());
		if(!db->getFieldValueByIndex(2).empty())
		    sum_max_capacity+=atoi(db->getFieldValueByIndex(2).c_str());
		strcpy(clientmedincelocation->nPositionCapacity,db->getFieldValueByIndex(2).c_str());
		str+=db->getFieldValueByIndex(0)+","+db->getFieldValueByIndex(1)+",";
		db->nextRow();
	}
	strcpy(clientmedincelocation->locationInfo,str.c_str());
	strcpy(clientmedincelocation->nMachineAmount,Utils::itos(sum_cur_quantity).c_str());
	strcpy(clientmedincelocation->nMachineCapacity,Utils::itos(sum_max_capacity).c_str());
	db->freeRecord();
	//rootLogger->trace("function QueryLocationMedInfo_Client end..............");
	return 0;
	}catch (...) {
		rootLogger->fatal("CMedicineManage--QueryLocationMedInfo_Client error ");
		return -1;
	}

}

//手持终端查询位置信息
int CMedicineManage::QueryMedInfo_Client(int location, ClientMedinceInfo *clientmedinfo)
{
	try{
    //rootLogger->trace("function QueryMedInfo_Client start..............");
	string sql="select medicine_id,cur_quantity,max_capacity,location_state from "+locationName+" where location_id='"+Utils::itos(location) +"'";
	if(db->execSQL(sql)!=0 || db->getRowNum()==0)
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("QueryMedInfo_Client function failure");
		return ERROR_MedicineManage_QueryMedInfo_Client;
	}
	string medID=db->getFieldValueByIndex(0);
	strcpy(clientmedinfo->nPositionAmount,db->getFieldValueByIndex(1).c_str());
	strcpy(clientmedinfo->nPositionCapacity,db->getFieldValueByIndex(2).c_str());
	if(!db->getFieldValueByIndex(3).empty())
	    clientmedinfo->locationState=atoi(db->getFieldValueByIndex(3).c_str());
	else
        clientmedinfo->locationState=0;
	db->freeRecord();
	sql="select a.med_name_ch,a.med_unit,b.sum_cur_quantity,b.sum_max_capacity from "
		"(select medicine_id,sum(cur_quantity) as sum_cur_quantity,sum(max_capacity) as sum_max_capacity from "+locationName+ " where medicine_id='"+medID+"' group by medicine_id) as b "
		"join (select med_id,med_name_ch, med_unit from medicine where med_state=1 and med_id='"+medID+"') as a on a.med_id=b.medicine_id";
	if(db->execSQL(sql)!=0 || db->getRowNum()==0)
	{
		rootLogger->info(sql);
		rootLogger->error("QueryMedInfo_Client function failure");
		return ERROR_MedicineManage_QueryMedInfo_Client;
	}
    strcpy(clientmedinfo->medicineNameCH,db->getFieldValueByIndex(0).c_str());
	strcpy(clientmedinfo->medicineUnit,db->getFieldValueByIndex(1).c_str());
	strcpy(clientmedinfo->nMachineAmount,db->getFieldValueByIndex(2).c_str());
	strcpy(clientmedinfo->nMachineCapacity,db->getFieldValueByIndex(3).c_str());
	db->freeRecord();
	//rootLogger->trace("function QueryMedInfo_Client end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--QueryMedInfo_Client error.");
		rootLogger->fatal("CMedicineManage--QueryMedInfo_Client error ");
		return -1;
	}
}

int CMedicineManage::QueryMedleavingPercent(int medID,int &falg)
{
	try{
	string  sql="select warning_percent,sum(cur_quantity) as sum_cur_quantity,sum(max_capacity) as sum_max_capacity from "+locationName +" where location_state=1 and medicine_id='"+Utils::itos(medID) +"' group by medicine_id ";
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("QueryMedleavingPercent function failure");
		return ERROR_MedicineManage_QueryMedleavingPercent;
	}
	if(!db->isEnd())
	{
        int leavingPercent,curleavingPercent,x,y;
		if(!db->getFieldValueByIndex(0).empty())
			curleavingPercent=atoi(db->getFieldValueByIndex(0).c_str());
		else
			curleavingPercent=30;
		if(!db->getFieldValueByIndex(1).empty())
			x=atoi(db->getFieldValueByIndex(1).c_str());
		else
			x=0;
		if(!db->getFieldValueByIndex(2).empty())
			y=atoi(db->getFieldValueByIndex(2).c_str());
		else
			y=0;
		if(y==0)
            leavingPercent=0;
		else
		    leavingPercent=x*100/curleavingPercent;
		if(leavingPercent>=y)
		    falg=0;
		else
            falg=1;
	}
    db->freeRecord();
	return 0;
	}catch (...) {
		rootLogger->fatal("CMedicineManage--QueryMedleavingPercent error ");
		return -1;
	}
}

int CMedicineManage::QueryMed_allLocationleavingPercent(int medID)
{
    int falg;
	list<int>locationlist;
	if(0==QueryMedleavingPercent( medID,falg))
	{
		string  sql="select location_id from  "+locationName+"  where medicine_id ='"+Utils::itos(medID) +"'";
		if(db->execSQL(sql)!=0)
		{
			rootLogger->info(sql);
			db->freeRecord();
			rootLogger->error("QueryMed_allLocationleavingPercent function failure");
			return ERROR_MedicineManage_QueryMedleavingPercent;
		}
		while(!db->isEnd())
		{
             locationlist.push_back(atoi(db->getFieldValueByIndex(0).c_str()));
			 db->nextRow();
		}
		db->freeRecord();
		list<int>::iterator iter;
	    map<int,MedicinLocationState>::iterator temp;
		for(iter=locationlist.begin();iter!=locationlist.end();iter++)
		{
			temp=medlocationstate.find(*iter);
			if(temp!=medlocationstate.end())
			{
				temp->second.med_warning_falg=falg;
			}
		}
		return 0;
	}
	else 
		return ERROR_MedicineManage_QueryMedleavingPercent;
}

int CMedicineManage::clearLocationNum(int location)
{
	try{
	
	//rootLogger->trace("function clearLocationNum start..............");
	mute->Lock();
	map<int,MedicinLocationState>::iterator temp;
	temp=medlocationstate.find(location);
	if(temp!=medlocationstate.end())
		temp->second.cur_quantity=0;
	string sql_location="update "+locationName+" set cur_quantity=0"
			        " where location_id='"+Utils::itos(location)+"'" ;
	if(db->execSQL(sql_location)!=0)
	{
	    rootLogger->info(sql_location);
		rootLogger->error("clearLocationNum function failure");
		mute->Unlock();
		return ERROR_MedicineManage_ClearLocation;
	} 
	//rootLogger->trace("function clearLocationNum end..............");
	mute->Unlock();
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--clearLocationNum error.");
		rootLogger->fatal("CMedicineManage--clearLocationNum error ");
		return -1;
	}
}

int CMedicineManage::getMachineState(string machineID, int &state_falg)
{
	try{
    string sql="select *  from machine where machineID='"+ machineID+"'";
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		db->freeRecord();
		rootLogger->error("getMachineState function failure");
		return ERROR_MedicineManage_getMachineState;
	}
	if(!db->isEnd() && !db->getFieldValueByName("machine_state").empty())
		state_falg=atoi(db->getFieldValueByName("machine_state").c_str());
	else
		state_falg=0;
	db->freeRecord();
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--getMachineState error.");
		rootLogger->fatal("CMedicineManage--getMachineState error ");
		return -1;
	}
}

int CMedicineManage::getWindowCount( int & windowCount)
{
	string sql="select count(*)  from windows ";
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		rootLogger->error("getWindowCount function failure");
		db->freeRecord();
		return ERROR_MedicineManage_getWindowCount;
	}
	windowCount =  atoi(db->getFieldValueByIndex(0).c_str());
	return 0 ;
}

int CMedicineManage::getWindowfalg(list<int>* Windowfalg_list)
{
	try{
    string sql="select window_no  from windows where window_falg=1";
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		rootLogger->error("getWindowfalg function failure");
		db->freeRecord();
		return ERROR_MedicineManage_getWindowfalg;
	}
	while(!db->isEnd())
	{
		Windowfalg_list->push_back(atoi(db->getFieldValueByName("window_no").c_str()));
		db->nextRow();
	}
	db->freeRecord();
	return 0;
	}catch (...) {
		rootLogger->fatal("CMedicineManage--getWindowfalg error ");
		return -1;
	}
}

int CMedicineManage::setWindowfalg(int windowID,int falg)
{
	try{
    string sql="update windows set window_falg = '"+Utils::itos(falg)+"'  where window_no='"+Utils::itos(windowID)+"'";
	if(db->execSQL(sql)!=0)
	{
		rootLogger->info(sql);
		rootLogger->error("setWindowfalg function failure");
		return ERROR_MedicineManage_setWindowfalg;
	}
	return 0;
	}catch (...) {
		/// AfxMessageBox("CMedicineManage--setWindowfalg error.");
		rootLogger->fatal("CMedicineManage--setWindowfalg error ");
		return -1;
	}
}

//手持终端设置药槽的最大值，当前值
int CMedicineManage::Setclientlocationinfo(ClientLocationSet clientlocationset)
{
	try{
	
	if(clientlocationset.location<=0)
		return 0;
	if(clientlocationset.locationCapacity!=-1)
	{
		string sql="update "+locationName+" set  cur_quantity= '"+Utils::itos(clientlocationset.locationCapacity)+"'  where location_id= '"+Utils::itos(clientlocationset.location)+"'";
		if(db->execSQL(sql)!=0)
		{
			rootLogger->info(sql);
			rootLogger->error("Setclientlocationinfo function failure");
			return -1;
		}
	}
	if(clientlocationset.maxCapacity !=-1)
	{
		string sql="update medicine set  max_capacity= '"+Utils::itos(clientlocationset.maxCapacity)+
			"' where Medicine_Id= (select medicine_id from "+ locationName+"  where location_id= '"+Utils::itos(clientlocationset.location)+"')";
		if(db->execSQL(sql)!=0)
		{
			rootLogger->info(sql);
			rootLogger->error("Setclientlocationinfo function failure");
			return -1;
		}
		sql="select medicine_id from  "+locationName+" where location_id= '"+Utils::itos(clientlocationset.location)+"'";
		if(db->execSQL(sql)!=0)
		{
			rootLogger->info(sql);
			rootLogger->error("Setclientlocationinfo function failure");
			return -1;
		}
		if(!db->isEnd())
		{
			string medid=db->getFieldValueByIndex(0);
			db->freeRecord();
			sql="update "+locationName+" set  max_capacity= '"+Utils::itos(clientlocationset.maxCapacity)+"' where medicine_id= '"+medid+"'";
			if(db->execSQL(sql)!=0)
			{
				rootLogger->info(sql);
				rootLogger->error("Setclientlocationinfo function failure");
				return -1;
			}
		}
	}
	if(this->initializeMedicinLocation()!=0)
	{
		rootLogger->error("modifyMedicine function failure");
		mute->Unlock();
		return ERROR_MedicineManage_ModifyMedicine;
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CMedicineManage--Setclientlocationinfo error ");
		return -1;
	}
}

int CMedicineManage::getWindowState(map<string,int>* pWin_state)
{
	string sql="select *  from windows where window_no>0";
	if(db->execSQL(sql)==0)
	{
		while(!db->isEnd())
		{
			string str=db->getFieldValueByName("window_no")+"号窗口";
			(*pWin_state)[str]=atoi(db->getFieldValueByName("window_state").c_str());
			db->nextRow();
		}
		db->freeRecord();
	}
	return 0;
}