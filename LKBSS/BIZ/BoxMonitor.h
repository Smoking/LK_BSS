// BoxMonitor.h: interface for the CBoxMonitor class.
//author:liuweiping 
//synopsis:监控系统模块
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITOR_H__47C6BA90_29BE_4AC1_8C03_A7225B234671__INCLUDED_)
#define AFX_MONITOR_H__47C6BA90_29BE_4AC1_8C03_A7225B234671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Singleton.h"
#include <map>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "DBManage.h"
#include "ErrorCodeRes.h"
#include "MedicineManage.h"
#include "FxPLC.h"

using namespace std;
using namespace log4cxx;

#define STOP_FLAG 1//停机标志

class CBoxMonitor  
{
public:
	int getsharp_StopFlag();
	int analyseAlarmInfo(WORD info);
	HWND hwnd;
	int getDBState();//获取数据库连接状态
	int getPLCState();//获取PLC连接状态
	int getStopFlag();//返回停机标志位
	int getKepFlag();//返回却篮子标志位
	int monitorError();//实时监控报警信息
	int startMonitor();//开始创建设备监控线程
	int pauseMonitorTheard();
	int getExitFlag();
	CBoxMonitor();
	virtual ~CBoxMonitor();
private:

	Singleton<CFxPLC> singletonFxPLC;
	CFxPLC *fxcpu;

    CMedicineManage * med;
	CDBManage *db;

	bool closeFlag;
	HANDLE handle;
	static int sharp_StopFlag;//急停标志位
	static int stopFlag;//停机标志
	static int kepFlag;//却篮子标志
	static LoggerPtr rootLogger ;
	int  windowCount;
	bool *Windowfalg;
	static int m_monitorTheard;
	static int m_exitFlag;
	
};

#endif // !defined(AFX_MONITOR_H__47C6BA90_29BE_4AC1_8C03_A7225B234671__INCLUDED_)
