// Monitor.cpp: implementation of the CBoxMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoxMonitor.h"
#include "Utils.h"
#include "ConfigUtil.h"
#include <iterator>

using namespace Utils;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CBoxMonitor::stopFlag=0;
int CBoxMonitor::kepFlag=0;
int CBoxMonitor::sharp_StopFlag=0;
int CBoxMonitor::m_monitorTheard=0;
int CBoxMonitor::m_exitFlag=0;
LoggerPtr CBoxMonitor::rootLogger = Logger::getLogger("CBoxMonitor");

//监控设备状态的线程
DWORD WINAPI MonitorThread(LPVOID lpParameter)
{
    CBoxMonitor p=*(CBoxMonitor*)lpParameter;
	p.monitorError();
	return 0;
}

CBoxMonitor::CBoxMonitor()
{
	try{
	//初始化日志记录器
	string msg = configUtil.getValue("logPropFile");
	PropertyConfigurator::configure(msg);
    med=new CMedicineManage();
	db = new CDBManage();
	m_monitorTheard = 0;
	m_exitFlag = 0;
	fxcpu = singletonFxPLC.getInst();
	int res = med->getWindowCount( windowCount );
	if(res != 0)
	{
		windowCount = 8;
	}
	Windowfalg = new bool[windowCount];
	for(int i=0;i<windowCount;i++)
	{
		Windowfalg[i]=true;
	}

	closeFlag=false;
	}catch (...) {
		rootLogger->fatal("CBoxMonitor--CBoxMonitor error ");
	}
}

CBoxMonitor::~CBoxMonitor()
{
	try{
	DWORD   dwExitCode;
	if(closeFlag && GetExitCodeThread( handle, &dwExitCode ) )
	{
		::TerminateThread(handle,dwExitCode);
		CloseHandle(handle);
	}

	if (fxcpu != NULL)
	{
		singletonFxPLC.releaseInst(fxcpu);
	}
	delete [] Windowfalg;
	delete med;
	delete db;
	}catch (...) {
		rootLogger->fatal("CBoxMonitor--~CBoxMonitor error ");
	}
}

//开始创建设备监控线程
int CBoxMonitor::startMonitor()
{
	try{
    //rootLogger->trace("function startMonitor start..............");
	DWORD ThreadId;
	handle=::CreateThread(NULL,0,MonitorThread,this,0,&ThreadId);
	if(handle==NULL)
	{
		rootLogger->error("Failed to create monitor thread!");
 		return ERROR_Monitor_CreatThead; 
	}
	closeFlag=true;
	//rootLogger->trace("function startMonitor end..............");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBoxMonitor--startMonitor error ");
		return -1;
	}
}

int CBoxMonitor::pauseMonitorTheard()
{
	m_monitorTheard = 1;
	return 0;
}

int CBoxMonitor::getExitFlag()
{
	return m_exitFlag;
}

//实时监控报警信息
int CBoxMonitor::monitorError()
{	
	//rootLogger->trace("function monitorError start..............");
	static bool Basketfalg=true;
	static bool Statefalg=true;
	static bool Alarmfalg=true;
	static int plcfalg=0,winfalg=0;
	while(1)
	{
		try{
		if(m_monitorTheard ==1)
		{
			m_exitFlag =1;
			Sleep(100);
			continue;
		}
		long Data =0 ;
		long error=fxcpu->GetLackBasketInfo(&Data);			
		if( Data == 1 )
		{
			if(Basketfalg)
			{
			   ::PostMessage(hwnd,WM_MONITORMESSAGE,0,230100) ;
			   Basketfalg=false;
			}
		}
		else 
		{
			Basketfalg=true;
		}
		
        Data =0 ;
		error=fxcpu->GetUrgentInfo(&Data);			
		if( Data == 1 )
		{
			if(Statefalg)
			{
			   ::PostMessage(hwnd,WM_MONITORMESSAGE,0,230102) ;
			   Statefalg=false;
			}
			sharp_StopFlag=STOP_FLAG;
			rootLogger->error("急停按下!");
		}
		else 
		{
			sharp_StopFlag=0;
			Statefalg=true;
		}
        Data =0 ;
		error=fxcpu->GetAlarmInfo(&Data);			
		if( Data == 1 )
		{
			plcfalg=STOP_FLAG;
			rootLogger->error("提升机异常!");
		}
		else 
		{
			plcfalg=0;								
		}
        Data =0 ;
		error=fxcpu->GetMessageAlarmInfo(&Data);			
		if( Data == 1 )
		{
			if(Alarmfalg)
			{
				::PostMessage(hwnd,WM_MONITORMESSAGE,0,230101) ;
				Alarmfalg =false;
			}
			plcfalg=STOP_FLAG;
			rootLogger->error("提升机异常，需要维修!");
		}
		else 
		{
			plcfalg=0;	
			Alarmfalg =true;
			
		}	
		list<int>::iterator iter;
        list<int>Windowfalglist;
		if(0==med->getWindowfalg(&Windowfalglist))
		{
			if(Windowfalglist.empty())
			{
				for(int i=0;i<windowCount;i++)
				{
				    Windowfalg[i]=true;
				}
				if(winfalg == STOP_FLAG)
				{
				   fxcpu->SetStartOutsideBelt(1);
				}
				winfalg=0;
			}
			for(iter=Windowfalglist.begin();iter!=Windowfalglist.end();iter++)
			{
				 winfalg=STOP_FLAG;
				 fxcpu->SetStopOutsideBelt(1);
				 int i=*iter;
				 if(Windowfalg[i])
				 {
					 Windowfalg[i]=false;
				     ::PostMessage(hwnd,WM_MONITORMESSAGE,1,230105+i)	;
				 }
				 rootLogger->error("窗口异常!");
			}
		}
		if(winfalg==STOP_FLAG || plcfalg==STOP_FLAG)
		{
			stopFlag=STOP_FLAG;
		}
		else
		{
            stopFlag=0;
		}
		Sleep(1000);
		}catch (...) {
			rootLogger->fatal("CBoxMonitor--errorWhile error ");
			Sleep(1000);
			continue;
		}
	}
	//rootLogger->trace("function monitorError end..............");
    return 0;
}

//返回却篮子标志位
int CBoxMonitor::getKepFlag()
{
    //rootLogger->trace("function getKepFlag start..............");
    long Data = 0;
	static bool Basketfalg=true;
	long error=fxcpu->GetHaveBasketInfo(&Data);			
	if( Data == 1 )
	{
		if(Basketfalg)
		{
			Basketfalg = false ;
			::PostMessage(hwnd,WM_MONITORMESSAGE,0,230103) ;
		}
		kepFlag=STOP_FLAG;
	}
	else 
	{
		kepFlag=0;	
		Basketfalg=true;
	}
	//rootLogger->trace("function getKepFlag end..............");
	return kepFlag;

}

//返回急停标志位
int CBoxMonitor::getsharp_StopFlag()
{
    //rootLogger->trace("function getsharp_StopFlag start..............");

	//rootLogger->trace("function getsharp_StopFlag end..............");
	return sharp_StopFlag;
}

//返回停机标志位
int CBoxMonitor::getStopFlag()
{
    //rootLogger->trace("function getStopFlag start..............");

	//rootLogger->trace("function getStopFlag end..............");
	return stopFlag;
}


//获取PLC连接状态
int CBoxMonitor::getPLCState()
{
	try{
    //rootLogger->trace("function getPLCState start..............");
	static bool plcfalg=true;
	int errorCode=fxcpu->GetFxPLCConnectState();
	if(errorCode!=0)
	{
		rootLogger->error("connect PLC failing");
		stopFlag=STOP_FLAG;
		if(plcfalg)
		    ::PostMessage(hwnd,WM_MONITORMESSAGE,0,ERROE_Monitor_GetPLCState) ;
		plcfalg=false;
		return ERROE_Monitor_GetPLCState;
	}
	else
		plcfalg=true;
	//rootLogger->trace("function getPLCState end..............");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBoxMonitor--getPLCState error ");
		return -1;
	}
}

//获取服务器数据库连接状态
int CBoxMonitor::getDBState()
{
	try{
    //rootLogger->trace("function getDBState start..............");
	static bool DBfalg=true;
	int errorCode= db->getServerState();
	if(errorCode!=0)
	{
		rootLogger->error("connect Database failing");
		stopFlag=STOP_FLAG;
		if(DBfalg)
		    ::PostMessage(hwnd,WM_MONITORMESSAGE,0,errorCode) ;
		DBfalg=false;
		return errorCode;
	}
	else
		DBfalg=true;
	//rootLogger->trace("function getDBState end..............");
	return 0;	
	}catch (...) {
		rootLogger->fatal("CBoxMonitor--getDBState error ");
		return -1;
	}
}


int CBoxMonitor::analyseAlarmInfo(WORD info)
{
	try{
	
	for(int i=0;i<16;i++)
	{
		int count=(info>>i) & 0x0001;
		if(count==1)
		{
			if(i==15)
			    ::PostMessage(hwnd,WM_MONITORMESSAGE,0,200+i) ;
			else
				::PostMessage(hwnd,WM_MONITORMESSAGE,2,200+i) ;
		}
	}
    return 0;
	}catch (...) {
		rootLogger->fatal("CBoxMonitor--analyseAlarmInfo error ");
		return -1;
	}
}
