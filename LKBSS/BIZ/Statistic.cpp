// Statistic.cpp: implementation of the CStatistic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Statistic.h"
#include "Utils.h"
#include "ConfigUtil.h"
#include <iterator>

using namespace Utils;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD CStatistic::DeviceRunTime=0;
DWORD CStatistic::SumDeviceRunTime=0;
int   CStatistic::SumLayMedicinCount=0;
int   CStatistic::SumLogCount=0;
int   CStatistic::SumPackCount=0;
int   CStatistic::SumPrescriptionCount=0;
int   CStatistic::SumPrintCount=0;
int   CStatistic::SumSatoPrintCount=0;
map<int,MedicinSort>CStatistic::medicinsort;
map<int,LocationStatistic>CStatistic::locationstatistic;
//map<string,int>CStatistic::booking;
LoggerPtr CStatistic::rootLogger = Logger::getLogger("CStatistic");

CStatistic::CStatistic()
{
	//初始化日志记录器
	string msg = configUtil.getValue("logPropFile");
	PropertyConfigurator::configure(msg);
	db=NULL;
	db=new CDBManage();
	this->cSysConfig = singleton.getInst();
	machineID=configUtil.getValue("MachineID");
}

CStatistic::~CStatistic()
{
	singleton.releaseInst(cSysConfig);
	if(db!=NULL)
	{
	    delete db;
	}
	
}

//增加总摆药次数
int CStatistic::addSumLayMedicinCount(int i)
{
	//rootLogger->trace("function addSumLayMedicinCount start..............");
    SumLayMedicinCount+=i;
	/*string sql_statistics;
	static flag=false;
	if(!flag)
	{
        flag=true;
		sql_statistics="select * from statistics where type=1";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumLayMedicinCount;
	    if(db->getRowNum()==0)
		    sql_statistics="insert into statistics(type,name,number,remark)values(1,'总摆药次数','"+Utils::itos(SumLayMedicinCount)+"','')";
	    else
	        sql_statistics="update statistics set number='"+Utils::itos(SumLayMedicinCount)+"'where type=1";
		db->freeRecord();
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumLayMedicinCount;
	}
	else
	{
		sql_statistics="update statistics set number='"+Utils::itos(SumLayMedicinCount)+"'where type=1";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumLayMedicinCount;
	}*/
	//rootLogger->trace("function addSumLayMedicinCount end..............");
	return 0;
}

//返回总摆药次数
int CStatistic::getSumLayMedicinCount()
{
	//rootLogger->trace("function getSumLayMedicinCount start..............");
	//rootLogger->trace("function getSumLayMedicinCount end..............");
	return SumLayMedicinCount;
}

//增加总处方数量
int CStatistic::addSumPrescriptionCount(int i)
{
	try{
    //rootLogger->trace("function addSumPrescriptionCount start..............");
	SumPrescriptionCount+=i;
	/*string sql_statistics;
	static flag=false;
	if(!flag)
	{
        flag=true;
		sql_statistics="select * from statistics where type=2";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPrescriptionCount;
	    if(db->getRowNum()==0)
		    sql_statistics="insert into statistics(type,name,number,remark)values(2,'总处方数量','"+Utils::itos(SumPrescriptionCount)+"','')";
	    else
	        sql_statistics="update statistics set number='"+Utils::itos(SumPrescriptionCount)+"'where type=2";
		db->freeRecord();
        if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPrescriptionCount;
	}
	else
	{
		sql_statistics="update statistics set number='"+Utils::itos(SumPrescriptionCount)+"'where type=2";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPrescriptionCount;
	}*/
	//rootLogger->trace("function addSumPrescriptionCount end..............");
	}catch (...) {
		/// AfxMessageBox("CStatistic--addSumPrescriptionCount error.");
		rootLogger->fatal("CStatistic--addSumPrescriptionCount error ");
		return -1;
	}
	return 0;
}

//返回总处方数量
int CStatistic::getSumPrescriptionCount()
{
    //rootLogger->trace("function getSumPrescriptionCount start..............");
	//rootLogger->trace("function getSumPrescriptionCount end..............");
	return SumPrescriptionCount;
}

//记录本次设备运行的开始时间
int CStatistic::startDeviceRunTime()
{
    //rootLogger->trace("function startDeviceRunTime start..............");
	DeviceRunTime=::GetTickCount();
	//rootLogger->trace("function startDeviceRunTime end..............");
	return 0;
}

//输出总设备运行时间
string CStatistic::getSumDeviceRunTime()
{
    //rootLogger->trace("function getSumDeviceRunTime start..............");
	DWORD minute=(::GetTickCount()-DeviceRunTime)/60000;
	DeviceRunTime=::GetTickCount();
	SumDeviceRunTime+=minute;
	char buf_day[20],buf_hour[20],buf_minute[20];
	int  day=SumDeviceRunTime/(60*24);
	int  hour=(SumDeviceRunTime/60)%24;
    minute=SumDeviceRunTime%60;
	::_itoa(day,buf_day,10);
	::_itoa(hour,buf_hour,10);
	::_itoa(minute,buf_minute,10);
	strcat(buf_day,"天");
	strcat(buf_hour,"小时");
    strcat(buf_day,buf_hour);
	strcat(buf_minute,"分钟");
	strcat(buf_day,buf_minute);
	//rootLogger->trace("function getSumDeviceRunTime end..............");
	return buf_day;
}

//增加总登录次数
int CStatistic::addSumLogCount(int i)
{
    //rootLogger->trace("function addSumLogCount start..............");
	SumLogCount+=i;
	/*string sql_statistics;
	static flag=false;
	if(!flag)
	{
        flag=true;
		string sql_statistics="select * from statistics where type=4";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumLogCount;
	    if(db->getRowNum()==0)
		    sql_statistics="insert into statistics(type,name,number,remark)values(4,'总登录次数','"+Utils::itos(SumLogCount)+"','')";
	    else
	        sql_statistics="update statistics set number='"+Utils::itos(SumLogCount)+"'where type=4";
		db->freeRecord();
        if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumLogCount;
	}
	else
	{
		sql_statistics="update statistics set number='"+Utils::itos(SumLogCount)+"'where type=4";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumLogCount;
	}*/
    //rootLogger->trace("function addSumLogCount end..............");
	return 0;
}

//返回总登录次数
int CStatistic::getSumLogCount()
{
    //rootLogger->trace("function getSumLogCount start..............");
	//rootLogger->trace("function getSumLogCount end..............");
	return SumLogCount;
}

//增加总热敏打印次数
int CStatistic::addSumPrintCount(int i)
{
    //rootLogger->trace("function addSumPrintCount start..............");
    SumPrintCount+=i;
	/*string sql_statistics;
	static flag=false;
	if(!flag)
	{
        flag=true;
		string sql_statistics="select * from statistics where type=5";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPrintCount;
	    if(db->getRowNum()==0)
		    sql_statistics="insert into statistics(type,name,number,remark)values(5,'总热敏打印次数','"+Utils::itos(SumPrintCount)+"','')";
	    else
	        sql_statistics="update statistics set number='"+Utils::itos(SumPrintCount)+"'where type=5";
		db->freeRecord();
        if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPrintCount;
	}
	else
	{
		sql_statistics="update statistics set number='"+Utils::itos(SumPrintCount)+"'where type=5";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPrintCount;
	}*/
    //rootLogger->trace("function addSumPrintCount end..............");
	return 0;
}

//返回总热敏打印次数
int CStatistic::getSumPrintCount()
{
    //rootLogger->trace("function getSumPrintCount start..............");
	//rootLogger->trace("function getSumPrintCount end..............");
	return SumPrintCount;
}

//增加总热转打印次数
int CStatistic::addSumSatoPrintCount(int i)
{
    //rootLogger->trace("function addSumSatoPrintCount start..............");
	SumSatoPrintCount+=i;
	/*string sql_statistics;
	static flag=false;
	if(!flag)
	{
        flag=true;
		sql_statistics="select * from statistics where type=6";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumSatoPrintCount;
	    if(db->getRowNum()==0)
		    sql_statistics="insert into statistics(type,name,number,remark)values(6,'总热转打印次数','"+Utils::itos(SumSatoPrintCount)+"','')";
	    else
	        sql_statistics="update statistics set number='"+Utils::itos(SumSatoPrintCount)+"'where type=6";
		db->freeRecord();
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumSatoPrintCount;
	}
	else
	{
		sql_statistics="update statistics set number='"+Utils::itos(SumSatoPrintCount)+"'where type=6";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumSatoPrintCount;
	}*/
	//rootLogger->trace("function addSumSatoPrintCount end..............");
	return 0;
}

//返回总热转打印次数
int CStatistic::getSumSatoPrintCount()
{
    //rootLogger->trace("function getSumSatoPrintCount start..............");
	//rootLogger->trace("function getSumSatoPrintCount end..............");
	return SumSatoPrintCount;
}

//增加总包装次数
int CStatistic::addSumPackCount(int i)
{
    //rootLogger->trace("function addSumPackCount start..............");
	SumPackCount+=i;
	/*string sql_statistics;
	static flag=false;
	if(!flag)
	{
        flag=true;
		sql_statistics="select * from statistics where type=7";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPackCount;
	    if(db->getRowNum()==0)
		    sql_statistics="insert into statistics(type,name,number,remark)values(7,'总热转打印次数','"+Utils::itos(SumPackCount)+"','')";
	    else
	        sql_statistics="update statistics set number='"+Utils::itos(SumPackCount)+"'where type=7";
		db->freeRecord();
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPackCount;
	}
	else
	{
		sql_statistics="update statistics set number='"+Utils::itos(SumPackCount)+"'where type=7";
		if(db->execSQL(sql_statistics)!=0)
			return ERROR_Statistic_SumPackCount;
	}*/
	//rootLogger->trace("function addSumPackCount end..............");
	return 0;
}

//返回总包装次数
int CStatistic::getSumPackCount()
{
    //rootLogger->trace("function getSumPackCount start..............");
	//rootLogger->trace("function getSumPackCount end..............");
	return SumPackCount;
}

/********************************************************************************************************************************/
/*增加药品分类统计

/*输入：药品分类统计medicinSort包括(关键字：药品ID；关键字值：药品类型 medType，药品摆药次数count，药品名medName，药品单位 unit)

/*********************************************************************************************************************************/
int CStatistic::addMedicinSortStatistic(map<int,MedicinSort>&medicinSort)
{
	try{
    //rootLogger->trace("function addMedicinSortStatistic start..............");
	map<int,MedicinSort>::iterator temp;
	map<int,MedicinSort>::iterator iter;
	MedicinSort ms;
	for(temp=medicinSort.begin();temp!=medicinSort.end();temp++)
	{
		iter=medicinsort.find(temp->first);
		if(iter != medicinsort.end())
		{
			ms.count=iter->second.count+temp->second.count;
			ms.medType=temp->second.medType;
			ms.medName=temp->second.medName;
			ms.unit=temp->second.unit;
		}
		else
		{
			ms.count=temp->second.count;
			ms.medType=temp->second.medType;
			ms.medName=temp->second.medName;
			ms.unit=temp->second.unit;
		}
        medicinsort[temp->first]=ms;
	}
	//rootLogger->trace("function addMedicinSortStatistic end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("getMedicinSortStatistic--addMedicinSortStatistic error.");
		rootLogger->fatal("getMedicinSortStatistic--addMedicinSortStatistic error ");
		return -1;
	}
}

/**************************************************************************************************************************/
/*输出药品分类统计

/*输出：药品分类统计地址pmedicinSort包括(关键字：药品ID；关键字值：药品类型 medType，药品摆药次数 count，药品名medName，
                                         药品单位 unit)

/***************************************************************************************************************************/
int CStatistic::getMedicinSortStatistic(map<int,MedicinSort>*pmedicinSort)
{
    //rootLogger->trace("function getMedicinSortStatistic start..............");
	string sql_statistics_med="select a.med_id,a.med_type,a.med_name_ch,a.unit,b.sumCount from "
		"(select med_id,sum(number)as sumCount from statistics_med where machineID='"+machineID+"' group by med_id)as b "
		"join (select * from statistics_med where machineID='"+machineID+"') as a on b.med_id=a.med_id";
	if(db->execSQL(sql_statistics_med)!=0)
	{
		rootLogger->info(sql_statistics_med);
		rootLogger->error("function getMedicinSortStatistic failure");
	    return ERROR_Statistic_GetMedicinSort;
	}
	while(!db->isEnd())
	{
        int id=atoi(db->getFieldValueByIndex(0).c_str());
        MedicinSort temp;
		temp.medType=atoi(db->getFieldValueByIndex(1).c_str());
		temp.count=atoi(db->getFieldValueByIndex(4).c_str());
		temp.medName=db->getFieldValueByIndex(2);
		temp.unit=db->getFieldValueByIndex(3);
		(*pmedicinSort)[id]=temp;
		db->nextRow();
	}
	db->freeRecord();
	//rootLogger->trace("function getMedicinSortStatistic end..............");
	return 0;
}

/**************************************************************************************************************************/
/*增加位置统计

/*输入：位置统计locationStatistic包括(关键字：位置ID；关键字值：位置统计类型locationType，当前统计次数count，统计名称 name，
                                       其中总统计次数0)

/***************************************************************************************************************************/
int CStatistic::addLocationStatistic(map<int,LocationStatistic>&locationStatistic)
{
	try{
    //rootLogger->trace("function addLocationStatistic start..............");
	map<int,LocationStatistic>::iterator temp;
	map<int,LocationStatistic>::iterator iter;
	LocationStatistic lc;
    for(temp=locationStatistic.begin();temp!=locationStatistic.end();temp++)
	{
		iter=locationstatistic.find(temp->first);
		if(iter != locationstatistic.end())
		{
			lc.sumCount=iter->second.sumCount+temp->second.count;
			lc.locationType=temp->second.locationType;
			if(lc.locationType==Statistic_Location_1)
			    lc.name="药盒";
			else if(lc.locationType==Statistic_Location_2)
                lc.name="药槽";
		}
		else
		{
			lc.sumCount=temp->second.count;
			lc.locationType=temp->second.locationType;
			if(lc.locationType==Statistic_Location_1)
			    lc.name="药盒";
			else if(lc.locationType==Statistic_Location_2)
                lc.name="药槽";
		}
        locationstatistic[temp->first]=lc;
	}
	//rootLogger->trace("function addLocationStatistic end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CStatistic--addLocationStatistic error.");
		rootLogger->fatal("CStatistic--addLocationStatistic error ");
		return -1;
	}
}

/**************************************************************************************************************************/
/*输出位置统计

/*输出：位置统计地址plocationStatistic(关键字：位置ID；关键字值：位置统计类型locationType，总统计次数 sumCount，统计名称 name，
                                        其中当前统计次数无效)

/***************************************************************************************************************************/
int CStatistic::getLocationStatistic(map<int,LocationStatistic>*plocationStatistic)
{
    //rootLogger->trace("function getLocationStatistic start..............");
	string sql_statistics_location="select a.location_id,a.type,a.name,b.sumCount from "
	"(select location_id,sum(number)as sumCount from statistics_location where machineID='"+machineID+"' group by location_id)as b "
	"join (select * from statistics_location  where machineID='"+machineID+"') as a on b.location_id=a.location_id";
	if(db->execSQL(sql_statistics_location)!=0)
	{
		rootLogger->info(sql_statistics_location);
		rootLogger->error("function getLocationStatistic failure");
		return ERROR_Statistic_GetLocation;
	}
	while(!db->isEnd())
	{
	    int id=atoi(db->getFieldValueByIndex(0).c_str());
        LocationStatistic temp;
		temp.locationType=atoi(db->getFieldValueByIndex(1).c_str());
		temp.name=db->getFieldValueByIndex(2);
		temp.sumCount=atoi(db->getFieldValueByIndex(3).c_str());	
		(*plocationStatistic)[id]=temp;
		db->nextRow();	
	}
	db->freeRecord();
	//rootLogger->trace("function getLocationStatistic end..............");
	return 0;
}

//保存全局统计数据
int CStatistic::saveStatistic()
{
    //rootLogger->trace("function saveStatistic start..............");
	string sql_statistics1;
	static flag1=false;
	if(!flag1)
	{
        flag1=true;
		sql_statistics1="select * from statistics where type=1 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics1)!=0)
		{
			rootLogger->info(sql_statistics1);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumLayMedicinCount;
		}
	    if(db->getRowNum()==0)
		    sql_statistics1="insert into statistics(type,name,number,remark,machineID)values(1,'总摆药次数',"
			"'"+Utils::itos(SumLayMedicinCount)+"','','"+machineID+"')";
	    else
	        sql_statistics1="update statistics set number='"+Utils::itos(SumLayMedicinCount)+"'where type=1 and  machineID='"+machineID+"'";
		db->freeRecord();
		if(db->execSQL(sql_statistics1)!=0)
		{
			rootLogger->info(sql_statistics1);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumLayMedicinCount;
		}
	}
	else
	{
		sql_statistics1="update statistics set number='"+Utils::itos(SumLayMedicinCount)+"'where type=1 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics1)!=0)
		{
			rootLogger->info(sql_statistics1);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumLayMedicinCount;
		}
	}

	string sql_statistics2;
	static flag2=false;
	if(!flag2)
	{
        flag2=true;
		sql_statistics2="select * from statistics where type=2 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics2)!=0)
		{
			rootLogger->info(sql_statistics2);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPrescriptionCount;
		}
	    if(db->getRowNum()==0)
		    sql_statistics2="insert into statistics(type,name,number,remark,machineID)values(2,'总处方数量',"
			"'"+Utils::itos(SumPrescriptionCount)+"','','"+machineID+"')";
	    else
	        sql_statistics2="update statistics set number='"+Utils::itos(SumPrescriptionCount)+"'where type=2 and  machineID='"+machineID+"'";
		db->freeRecord();
        if(db->execSQL(sql_statistics2)!=0)
		{
			rootLogger->info(sql_statistics2);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPrescriptionCount;
		}
	}
	else
	{
		sql_statistics2="update statistics set number='"+Utils::itos(SumPrescriptionCount)+"'where type=2 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics2)!=0)
		{
			rootLogger->info(sql_statistics2);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPrescriptionCount;
		}
	}

	
	DWORD minute=(::GetTickCount()-DeviceRunTime)/60000;
	DeviceRunTime=::GetTickCount();
	SumDeviceRunTime+=minute;
	string sql_statistics3;
	bool flag3=false;
	if(!flag3)
	{
		flag3=true;
		sql_statistics3="select * from statistics where type=3 and  machineID='"+machineID+"'";
	    if(db->execSQL(sql_statistics3)!=0)
		{
			rootLogger->info(sql_statistics3);
			rootLogger->error("function saveStatistic failure");
	 	    return ERROR_Statistic_SumDeviceRunTime;
		}
	    if(db->getRowNum()==0)
		    sql_statistics3="insert into statistics(type,name,number,remark,machineID)values(3,'总设备运行时间',"
			"'"+Utils::itos(SumDeviceRunTime)+"','','"+machineID+"')";
	    else
	        sql_statistics3="update statistics set number='"+Utils::itos(SumDeviceRunTime)+"'where type=3 and  machineID='"+machineID+"'";
        db->freeRecord();
	    if(db->execSQL(sql_statistics3)!=0)
		{
			rootLogger->info(sql_statistics3);
			rootLogger->error("function saveStatistic failure");
		    return ERROR_Statistic_SumDeviceRunTime;
		}
	}
	else
	{
		sql_statistics3="update statistics set number='"+Utils::itos(SumDeviceRunTime)+"'where type=3 and  machineID='"+machineID+"'";
	    if(db->execSQL(sql_statistics3)!=0)
		{
			rootLogger->info(sql_statistics3);
			rootLogger->error("function saveStatistic failure");
		    return ERROR_Statistic_SumDeviceRunTime;
		}
	}

	
	string sql_statistics4;
	static flag4=false;
	if(!flag4)
	{
        flag4=true;
		string sql_statistics4="select * from statistics where type=4 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics4)!=0)
		{
		    rootLogger->info(sql_statistics4);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumLogCount;
		}
	    if(db->getRowNum()==0)
		    sql_statistics4="insert into statistics(type,name,number,remark,machineID)values(4,'总登录次数',"
			"'"+Utils::itos(SumLogCount)+"','','"+machineID+"')";
	    else
	        sql_statistics4="update statistics set number='"+Utils::itos(SumLogCount)+"'where type=4 and  machineID='"+machineID+"'";
		db->freeRecord();
        if(db->execSQL(sql_statistics4)!=0)
		{
			rootLogger->info(sql_statistics4);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumLogCount;
		}
	}
	else
	{
		sql_statistics4="update statistics set number='"+Utils::itos(SumLogCount)+"'where type=4 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics4)!=0)
		{
			rootLogger->info(sql_statistics4);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumLogCount;
		}
	}

	
	string sql_statistics5;
	static flag5=false;
	if(!flag5)
	{
        flag5=true;
		string sql_statistics5="select * from statistics where type=5 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics5)!=0)
		{
			rootLogger->info(sql_statistics5);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPrintCount;
		}
	    if(db->getRowNum()==0)
		    sql_statistics5="insert into statistics(type,name,number,remark,machineID)values(5,'总热敏打印次数',"
			"'"+Utils::itos(SumPrintCount)+"','','"+machineID+"')";
	    else
	        sql_statistics5="update statistics set number='"+Utils::itos(SumPrintCount)+"'where type=5 and  machineID='"+machineID+"'";
		db->freeRecord();
        if(db->execSQL(sql_statistics5)!=0)
		{
			rootLogger->info(sql_statistics5);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPrintCount;
		}
	}
	else
	{
		sql_statistics5="update statistics set number='"+Utils::itos(SumPrintCount)+"'where type=5 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics5)!=0)
		{
			rootLogger->info(sql_statistics5);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPrintCount;
		}
	}

    string sql_statistics6;
	static flag6=false;
	if(!flag6)
	{
        flag6=true;
		sql_statistics6="select * from statistics where type=6 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics6)!=0)
		{
			rootLogger->info(sql_statistics6);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumSatoPrintCount;
		}
	    if(db->getRowNum()==0)
		    sql_statistics6="insert into statistics(type,name,number,remark,machineID)values(6,'总热转打印次数',"
			"'"+Utils::itos(SumSatoPrintCount)+"','','"+machineID+"')";
	    else
	        sql_statistics6="update statistics set number='"+Utils::itos(SumSatoPrintCount)+"'where type=6 and  machineID='"+machineID+"'";
		db->freeRecord();
		if(db->execSQL(sql_statistics6)!=0)
		{
			rootLogger->info(sql_statistics6);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumSatoPrintCount;
		}
	}
	else
	{
		sql_statistics6="update statistics set number='"+Utils::itos(SumSatoPrintCount)+"'where type=6 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics6)!=0)
		{
			rootLogger->info(sql_statistics6);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumSatoPrintCount;
		}
	}

    string sql_statistics7;
	static flag7=false;
	if(!flag7)
	{
        flag7=true;
		sql_statistics7="select * from statistics where type=7 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics7)!=0)
		{
			rootLogger->info(sql_statistics7);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPackCount;
		}
	    if(db->getRowNum()==0)
		    sql_statistics7="insert into statistics(type,name,number,remark,machineID)values(7,'总包装次数',"
			"'"+Utils::itos(SumPackCount)+"','','"+machineID+"')";
	    else
	        sql_statistics7="update statistics set number='"+Utils::itos(SumPackCount)+"'where type=7 and  machineID='"+machineID+"'";
		db->freeRecord();
		if(db->execSQL(sql_statistics7)!=0)
		{
			rootLogger->info(sql_statistics7);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPackCount;
		}
	}
	else
	{
		sql_statistics7="update statistics set number='"+Utils::itos(SumPackCount)+"'where type=7 and  machineID='"+machineID+"'";
		if(db->execSQL(sql_statistics7)!=0)
		{
			rootLogger->info(sql_statistics7);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_SumPackCount;
		}
	}
    
	SYSTEMTIME t1;
    GetSystemTime(&t1); 
    CTime curTime(t1); 
	int year=curTime.GetYear();
	int month=curTime.GetMonth();
	int day=curTime.GetDay();
	map<int,LocationStatistic>::iterator iter;
	for(iter=locationstatistic.begin();iter!=locationstatistic.end();iter++)
	{
		string sql_statistics_location="select * from statistics_location where  machineID='"+machineID+"' and year='"+Utils::itos(year)+"' and month='"+Utils::itos(month)+"' and day='"+Utils::itos(day)+"' and location_id='"+Utils::itos(iter->first)+"'";
		if(db->execSQL(sql_statistics_location)!=0)
		{
			rootLogger->info(sql_statistics_location);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_AddLocation;
		}
		if(db->getRowNum()==0)
			sql_statistics_location="insert into statistics_location(year,month,day,location_id,type,name,number,machineID)values('"+Utils::itos(year) +"','"+Utils::itos(month)+"','"+Utils::itos(day)+"','"+Utils::itos(iter->first)+"',"
			"'"+Utils::itos(iter->second.locationType)+"','"+iter->second.name+"','"+Utils::itos(iter->second.sumCount)+"','"+machineID+"')";
		else
		    sql_statistics_location="update statistics_location set number=number+'"+Utils::itos(iter->second.sumCount)+"' where machineID='"+machineID+"' and year='"+Utils::itos(year)+"' and month='"+Utils::itos(month)+"' and day='"+Utils::itos(day)+"' and location_id='"+Utils::itos(iter->first)+"'";
	    db->freeRecord();
        if(db->execSQL(sql_statistics_location)!=0)
		{
			rootLogger->info(sql_statistics_location);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_AddLocation;
		}
	}
    locationstatistic.clear();
	
	map<int,MedicinSort>::iterator temp;
	for(temp=medicinsort.begin();temp!=medicinsort.end();temp++)
	{
        string sql_statistics_med="select * from statistics_med where  machineID='"+machineID+"' and year='"+Utils::itos(year)+"' and month='"+Utils::itos(month)+"' and day='"+Utils::itos(day)+"' and med_id='"+Utils::itos(temp->first)+"'";
		if(db->execSQL(sql_statistics_med)!=0)
		{
			rootLogger->info(sql_statistics_med);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_AddMedicinSortd;
		}
		if(db->getRowNum()==0)
		    sql_statistics_med="insert into statistics_med(year,month,day,med_type,med_id,number,med_name_ch,unit,machineID)"
			"values('"+Utils::itos(year) +"','"+Utils::itos(month)+"','"+Utils::itos(day)+"','"+Utils::itos(temp->second.medType)+"',"
			"'"+Utils::itos(temp->first)+"','"+Utils::itos(temp->second.count)+"','"+temp->second.medName+"','"+temp->second.unit+"','"+machineID+"')";
	    else
		    sql_statistics_med="update statistics_med set number=number+'"+Utils::itos(temp->second.count)+"' where machineID='"+machineID+"' and year='"+Utils::itos(year)+"' and month='"+Utils::itos(month)+"' and day='"+Utils::itos(day)+"' and med_id='"+Utils::itos(temp->first)+"'";
	    db->freeRecord();
        if(db->execSQL(sql_statistics_med)!=0)
		{
			rootLogger->info(sql_statistics_med);
			rootLogger->error("function saveStatistic failure");
			return ERROR_Statistic_AddMedicinSortd;
		}
	}
	medicinsort.clear();
	//rootLogger->trace("function saveStatistic end..............");
	return 0;
}

//初始化全局统计数据
int CStatistic::initialize()
{
    //rootLogger->trace("function initialize start..............");
	//startDeviceRunTime();//开始计时
	string sql_statistics="select type,number from statistics where machineID='"+machineID+"'";
	if(db->execSQL(sql_statistics)!=0)
	{
		rootLogger->info(sql_statistics);
		rootLogger->error("function initialize failure");
		return ERROR_Statistic_Initialize;
	}
	while(!db->isEnd())
	{
		if(atoi(db->getFieldValueByIndex(0).c_str())==1)
		    this->SumLayMedicinCount=atoi(db->getFieldValueByIndex(1).c_str());
		if(atoi(db->getFieldValueByIndex(0).c_str())==2)
		    this->SumPrescriptionCount=atoi(db->getFieldValueByIndex(1).c_str());
		if(atoi(db->getFieldValueByIndex(0).c_str())==3)
		    this->SumDeviceRunTime=atoi(db->getFieldValueByIndex(1).c_str());
		if(atoi(db->getFieldValueByIndex(0).c_str())==4)
		    this->SumLogCount=atoi(db->getFieldValueByIndex(1).c_str());
		if(atoi(db->getFieldValueByIndex(0).c_str())==5)
		    this->SumPrintCount=atoi(db->getFieldValueByIndex(1).c_str());
		if(atoi(db->getFieldValueByIndex(0).c_str())==6)
		    this->SumSatoPrintCount=atoi(db->getFieldValueByIndex(1).c_str());
		if(atoi(db->getFieldValueByIndex(0).c_str())==7)
		    this->SumPackCount=atoi(db->getFieldValueByIndex(1).c_str());
		db->nextRow();
	}
	db->freeRecord();
    /*string sql_statistics_location="select location_id,type,name,number from statistics_location";
	if(db->execSQL(sql_statistics_location)!=0)
	{
		rootLogger->info(sql_statistics_location);
		rootLogger->error("function initialize failure");
		return ERROR_Statistic_Initialize;
	}
	while(!db->isEnd())
	{
	    int id=atoi(db->getFieldValueByIndex(0).c_str());
        LocationStatistic temp;
		temp.locationType=atoi(db->getFieldValueByIndex(1).c_str());
		temp.sumCount=atoi(db->getFieldValueByIndex(3).c_str());
		temp.name=db->getFieldValueByIndex(2);
		locationstatistic[id]=temp;
		db->nextRow();	
	}
	db->freeRecord();*/
	//rootLogger->trace("function initialize end..............");
	return 0;
}

//设置预警量
int CStatistic::setBooking(map<string,string>&inBooking)
{
    //rootLogger->trace("function setBooking start..............");
	if(cSysConfig->saveConfig(inBooking.begin(),inBooking.end())!=0)
	{
		rootLogger->error("function setBooking failure");
		return ERROR_Statistic_SetBooking;
	}
	//rootLogger->trace("function setBooking end..............");
	return 0;
}

//输出预警量
int CStatistic::getBooking(map<string,int>*outBooking)
{
    //rootLogger->trace("function getBooking start..............");
	if(!outBooking->empty())
	    outBooking->clear();
	outBooking->insert(pair<string,int>("RunTime",configUtil.getInt("RunTime")));
	outBooking->insert(pair<string,int>("PrintCount",configUtil.getInt("PrintCount")));
	outBooking->insert(pair<string,int>("SatoPrintCount",configUtil.getInt("SatoPrintCount")));
	outBooking->insert(pair<string,int>("PackCount",configUtil.getInt("PackCount")));

	outBooking->insert(pair<string,int>("maintainRunTime",configUtil.getInt("maintainRunTime")));
	outBooking->insert(pair<string,int>("maintainPrintCount",configUtil.getInt("maintainPrintCount")));
	outBooking->insert(pair<string,int>("maintainSatoPrintCount",configUtil.getInt("maintainSatoPrintCount")));
	outBooking->insert(pair<string,int>("maintainPackCount",configUtil.getInt("maintainPackCount")));
	//rootLogger->trace("function getBooking end..............");
	return 0;
}
