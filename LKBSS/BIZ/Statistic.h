// Statistic.h: interface for the CStatistic class.
//author:liuweiping 
//synopsis:全局统计模块
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTIC_H__3656DCDB_7406_437A_BD22_3CC561CB1870__INCLUDED_)
#define AFX_STATISTIC_H__3656DCDB_7406_437A_BD22_3CC561CB1870__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "DBManage.h"
#include "Singleton.h"
#include "SysConfig.h"
#include "ErrorCodeRes.h"

using namespace std;
using namespace log4cxx;

#define Statistic_medType_1  1001   //药品类型(盒剂)
#define Statistic_medType_2  1002   //药品类型(片剂)
#define Statistic_medType_3  1003   //药品类型(针剂)
#define Statistic_medType_9  1009   //药品类型(其它)
#define Statistic_Location_1  2001  //位置统计类型(马达)
#define Statistic_Location_2  2002  //位置统计类型(电磁铁)
#define Statistic_SumLayMedicinCount 1   //统计类型(总摆药次数)
#define Statistic_SumPrescriptionCount 2 //统计类型(总处方数量)
#define Statistic_SumDeviceRunTime 3     //统计类型(总设备运行时间)
#define Statistic_SumLogCount 4          //统计类型(总登录次数)
#define Statistic_SumPrintCount 5        //统计类型(总热敏打印次数)
#define Statistic_SumSatoPrintCount 6    //统计类型(总热转打印次数)
#define Statistic_SumPackCount 7         //统计类型(总包装次数)

//药品分类统计类型结构
typedef struct _MedicinSort{
 //   int medID;//药品ID
	int medType;//药品类型
    int count;//药品摆药次数
    string medName;//药品名
	string unit;//药品单位
} MedicinSort;

//位置统计类型结构
typedef struct _LocationStatistic{
 //   int locationID;//位置ID
	int locationType;//位置统计类型
    int count;//当前统计次数
    int sumCount;//总统计次数
    string name;//统计名称(马达、电磁铁)
} LocationStatistic;


class CStatistic  
{
public:
	int getBooking(map<string,int>*outBooking);//输出预警量
	int setBooking(map<string,string>&inBooking);//设置预警量
	int initialize ();//初始化全局统计数据
	int saveStatistic();//保存全局统计数据
	int getLocationStatistic(map<int,LocationStatistic>*plocationStatistic);//输出位置统计
	int addLocationStatistic(map<int,LocationStatistic>&locationStatistic);//增加位置统计
	int getMedicinSortStatistic(map<int,MedicinSort>*pmedicinSort);//输出药品分类统计
	int addMedicinSortStatistic(map<int,MedicinSort>&medicinSort);//增加药品分类统计
	int getSumPackCount();//返回总包装次数
	int addSumPackCount(int i);//增加总包装次数
	int getSumSatoPrintCount();//返回总热转打印次数
	int addSumSatoPrintCount(int i);//增加总热转打印次数
	int getSumPrintCount();//返回总热敏打印次数
	int addSumPrintCount(int i);//增加总热敏打印次数
	int getSumLogCount();//返回总登录次数
	int addSumLogCount(int i);//增加总登录次数
	string getSumDeviceRunTime();//输出总设备运行时间
	int startDeviceRunTime();//记录本次设备运行的开始时间
	int getSumPrescriptionCount ();//返回总处方数量
	int addSumPrescriptionCount (int i);//增加总处方数量
	int getSumLayMedicinCount();//返回总摆药次数
	int addSumLayMedicinCount (int i);//增加总摆药次数
	CStatistic();
	virtual ~CStatistic();
private:
    static int SumLayMedicinCount;  //总摆药次数
    static int SumPrescriptionCount;  //总处方数量
    static DWORD DeviceRunTime;   //本次设备运行开始时间
    static DWORD SumDeviceRunTime;   //总设备运行时间(分钟)
    static int SumLogCount;        //总登录次数
    static int SumPrintCount;       //总热敏打印次数
    static int SumSatoPrintCount;   //总热转打印次数
    static int SumPackCount;      //总包装次数
    static map<int,MedicinSort>medicinsort;    //药品分类统计 (关键字:药品ID)
    static map<int,LocationStatistic>locationstatistic;    //位置统计(关键字:位置ID)
//	static map<string,int>booking;  //预警量
	static LoggerPtr rootLogger ;
	string machineID;
	CDBManage  *db;
	CSysConfig* cSysConfig;
	Singleton<CSysConfig> singleton;
};


#endif // !defined(AFX_STATISTIC_H__3656DCDB_7406_437A_BD22_3CC561CB1870__INCLUDED_)
