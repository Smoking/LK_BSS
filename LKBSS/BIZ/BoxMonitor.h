// BoxMonitor.h: interface for the CBoxMonitor class.
//author:liuweiping 
//synopsis:���ϵͳģ��
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

#define STOP_FLAG 1//ͣ����־

class CBoxMonitor  
{
public:
	int getsharp_StopFlag();
	int analyseAlarmInfo(WORD info);
	HWND hwnd;
	int getDBState();//��ȡ���ݿ�����״̬
	int getPLCState();//��ȡPLC����״̬
	int getStopFlag();//����ͣ����־λ
	int getKepFlag();//����ȴ���ӱ�־λ
	int monitorError();//ʵʱ��ر�����Ϣ
	int startMonitor();//��ʼ�����豸����߳�
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
	static int sharp_StopFlag;//��ͣ��־λ
	static int stopFlag;//ͣ����־
	static int kepFlag;//ȴ���ӱ�־
	static LoggerPtr rootLogger ;
	int  windowCount;
	bool *Windowfalg;
	static int m_monitorTheard;
	static int m_exitFlag;
	
};

#endif // !defined(AFX_MONITOR_H__47C6BA90_29BE_4AC1_8C03_A7225B234671__INCLUDED_)
