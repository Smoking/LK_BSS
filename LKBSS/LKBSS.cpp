// LKBSS.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LKBSS.h"
#include "LKBSSDlg.h"
#include "ConfigUtil.h"
#include "Encrypt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma comment(lib,"log4cxx.lib")
/////////////////////////////////////////////////////////////////////////////
// CLKBSSApp

CSysConfig* Singleton<CSysConfig>::ptr_ = NULL;
size_t Singleton<CSysConfig>::cnt_ = 0;

CBIZBoxService* Singleton<CBIZBoxService>::ptr_ = NULL;
size_t Singleton<CBIZBoxService>::cnt_ = 0;

CStatistic* Singleton<CStatistic>::ptr_ = NULL;
size_t Singleton<CStatistic>::cnt_ = 0;

CFxPLC* Singleton<CFxPLC>::ptr_ = NULL;
size_t Singleton<CFxPLC>::cnt_ = 0;

CBoxSinglechip *Singleton<CBoxSinglechip>::ptr_ = NULL; 
size_t Singleton<CBoxSinglechip>::cnt_ = 0;

CPrescriptionManager* Singleton<CPrescriptionManager>::ptr_ = NULL;
size_t Singleton<CPrescriptionManager>::cnt_ = 0;


BEGIN_MESSAGE_MAP(CLKBSSApp, CWinApp)
	//{{AFX_MSG_MAP(CLKBSSApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLKBSSApp construction

CLKBSSApp::CLKBSSApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLKBSSApp object

CLKBSSApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLKBSSApp initialization

BOOL CLKBSSApp::InitInstance()
{
	HANDLE hMap=CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 128, "MutexRunning"); 
    if (hMap==NULL)
	{
		///// AfxMessageBox("创建用于互斥运行的内存映射文件对象失败!", MB_OK|MB_ICONSTOP); 

        return FALSE;
	}
	else if(GetLastError()==ERROR_ALREADY_EXISTS) 
	{ 
        LPVOID lpMem=MapViewOfFile(hMap, FILE_MAP_WRITE, 0,0,0); 
        CString str=(char*)lpMem; 

        UnmapViewOfFile(lpMem); 

        CloseHandle(hMap); 
		Sleep(5000);
		hMap=CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 128, "MutexRunning");
		if (hMap==NULL)
		{
			///// AfxMessageBox("创建用于互斥运行的内存映射文件对象失败!", MB_OK|MB_ICONSTOP); 
			return FALSE;
		}
		if(GetLastError()==ERROR_ALREADY_EXISTS)
		{		
			CloseHandle(hMap);
			///// AfxMessageBox(str, MB_OK|MB_ICONSTOP);
			return FALSE; 
		}
        
	}  
    else 
	{ 
        LPVOID lpMem=MapViewOfFile(hMap, FILE_MAP_WRITE, 0,0,0); 

        strcpy((char*)lpMem, "程序正在运行,请先退出!"); 

        UnmapViewOfFile(lpMem); 
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if (!AfxOleInit())
	{
		return FALSE;
	}
	
	this->cSysConfig = singletonConfig.getInst();
	this->cSysConfig->initConfig();
	this->cSysConfig->loadConfig();
	cSysConfig->initConfig("LKBSSErrorInfo.xml");
	cSysConfig->loadConfig(true);
    
    userinfo.user_id = configUtil.getValue("MachineID");
    userinfo.user_name = userinfo.user_id;

//    Encrypt *enp;
//    enp = new Encrypt();
//
//	DBparam.host = enp->decrypt(desKey, configUtil.getValue("host").c_str());
//	DBparam.user = enp->decrypt(desKey, configUtil.getValue("user").c_str());
//	DBparam.password = enp->decrypt(desKey, configUtil.getValue("password").c_str());
//	DBparam.db = enp->decrypt(desKey, configUtil.getValue("db").c_str());
//	DBparam.port = atoi(enp->decrypt(desKey, configUtil.getValue("port").c_str()));
//    DBparam.characterSet = enp->decrypt(desKey, configUtil.getValue("characterSet").c_str());
//    delete enp;

	DBparam.host = configUtil.getValue("host");
	DBparam.user = configUtil.getValue("user");
	DBparam.password = configUtil.getValue("password");
	DBparam.db = configUtil.getValue("db");
	DBparam.port = configUtil.getInt("port");
	DBparam.characterSet = configUtil.getValue("characterSet");

	BoxDBparam.host = configUtil.getValue("Boxhost");
	BoxDBparam.user = configUtil.getValue("Boxuser");
	BoxDBparam.password = configUtil.getValue("Boxpassword");
	BoxDBparam.db = configUtil.getValue("Boxdb");
	BoxDBparam.port = configUtil.getInt("Boxport");
	BoxDBparam.characterSet = configUtil.getValue("BoxcharacterSet");

	ADODBparam.m_server = configUtil.getValue("server");
	ADODBparam.m_data = configUtil.getValue("data");
	ADODBparam.m_uid = configUtil.getValue("uid");
	ADODBparam.m_pwd = configUtil.getValue("pwd");

	int i = configUtil.getInt("runMode");


	CLKBSSDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	
	return FALSE;
}

CLKBSSApp::~CLKBSSApp()
{
	if (this->cSysConfig != NULL)
	{
		singletonConfig.releaseInst(cSysConfig);
	}

	
 }
