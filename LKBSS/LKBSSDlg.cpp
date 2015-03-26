// LKBSSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "LKBSSDlg.h"
#include "AddMedicineDlg.h"
#include "MedicineMgr.h"
#include "LogViewer.h"
#include "Utils.h"
#include "SystemSetting.h"
#include "BIZBoxService.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TABWND_MARGIN				10		//tab离对话框边框的间距

#define INFOZONE_TABWND_WIDTH		392		//InfoZoneTabWnd的宽度
#define INFOZONE_TABWND_HEIGHT		630		//InfoZoneTabWnd的高度
#define INFOZONE_TAB_WIDTH			86      //InfoZoneTab的宽度

#define MONITORZONE_TABWND_WIDTH		605		//MonitorZoneTabWnd的宽度
#define MONITORZONE_TABWND_HEIGHT		630		//MonitorZoneTabWnd的高度
#define MONITORZONE_TAB_WIDTH			90      //MonitorZoneTab的宽度

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLKBSSDlg dialog
LoggerPtr CLKBSSDlg::rootLogger = Logger::getLogger("CLKBSSDlg");  //初始化日志
CLKBSSDlg::CLKBSSDlg(CWnd* pParent /*=NULL*/)
	: CRCDialog(CLKBSSDlg::IDD, pParent,1)
{	
	//{{AFX_DATA_INIT(CLKBSSDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	CString strBgPath = Util::GetDialogResPath() + "\\bg.bmp";
	SetBgPath(strBgPath);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isRunning = false;
	bizbs = NULL;
	statistic = NULL;
	server = NULL;
	fxcpu = NULL;
	med = NULL;
	pBoxMonitor = NULL;
	m_cpm = NULL;
	string msg = configUtil.getValue("logPropFile");    //日志模块加载配置文件
	PropertyConfigurator::configure(msg);
	portPLC=configUtil.getInt("PLCPort");
	machineID=configUtil.getInt("MachineID");
}

void CLKBSSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLKBSSDlg)
	DDX_Control(pDX, IDC_AddMedicine, m_btnAddMedicine);
	DDX_Control(pDX, IDC_QueryMedicine, m_btnQueryMedicine);
	DDX_Control(pDX, IDC_StopMedicine, m_btnStopMedicine);
	DDX_Control(pDX, IDC_StartMedicine, m_btnStartMedicine);
	DDX_Control(pDX, IDC_ACTFXCPU1, m_fxcpu);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLKBSSDlg, CRCDialog)
	//{{AFX_MSG_MAP(CLKBSSDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_StartMedicine, OnStartMedicine)
	ON_BN_CLICKED(IDC_AddMedicine, OnAddMedicine)
	ON_BN_CLICKED(IDC_QueryMedicine, OnQueryMedicine)
	ON_COMMAND(IDM_LogOut, OnLogOut)
	ON_COMMAND(IDM_MedManage, OnMedManage)
	ON_COMMAND(IDM_QueryLog, OnQueryLog)
	ON_WM_DESTROY()
	ON_COMMAND(IDM_Reboot, OnReboot)
	ON_COMMAND(IDM_Monitor, OnMonitor)	
	ON_BN_CLICKED(IDC_StopMedicine, OnStopMedicine)
	ON_COMMAND(IDM_SendMed, OnSendMed)
	ON_WM_CREATE()
	ON_COMMAND(IDM_AddMedicine, OnAddMedicine)
	ON_COMMAND(IDM_StartMedicine, OnStartMedicine)
	ON_MESSAGE(WM_REFRESH,OnRefresh)
	ON_MESSAGE(WM_BUTTONDOWN,OnButtonDown)
	ON_MESSAGE(WM_THREADSTATE,OnChangeThreadState)
	ON_MESSAGE(WM_MONITORMESSAGE,OnMonitorMessage)
	ON_MESSAGE(WM_ERRORADDRESS,OnDistError)
	ON_MESSAGE(WM_GETERRORMESSAGE,OnGetErrorMessage)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLKBSSDlg message handlers

BOOL CLKBSSDlg::OnInitDialog()
{
	try{
	
	CRCDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	fxcpu=singletonFxPLC.getInst();
	fxcpu->SetFxControl(&m_fxcpu);
	fxcpu->SetActPortNumber(portPLC);
	if(0 != fxcpu->Open())
	{
		Util::ShowMessageBox(NULL, "\r\n\r\n连接PLC失败","提示",RCMB_OK);
	}

	pBoxMonitor=new CBoxMonitor();
	pBoxMonitor->hwnd=this->m_hWnd;
	pBoxMonitor->startMonitor();
	m_deviceMonitorPage.pBoxMonitor=pBoxMonitor;
    
	this->bizbs = singletonBIZ.getInst();
	this->bizbs->setHwnd(this->m_hWnd);
	bizbs->pBoxMonitor = pBoxMonitor;
	bizbs->clearCard();
	fxcpu->SetStartInsideBelt(320*6);
	
    this->statistic=singletonStatistic.getInst();
	
	
	server=new CServerSocket();
	server->start();

	med=new  CMedicineManage();

	m_cpm = new CPrescriptionManager();
	
	ShowBottomLogo(TRUE);
	CString strCaption = "国控智能盒剂快速发药系统";
    SetWindowText(strCaption);
	return TRUE;  // return TRUE  unless you set the focus to a control
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--OnInitDialog error!");
		rootLogger->error("CLKBSSDlg--OnInitDialog error!");
		return false;
	}
}

void CLKBSSDlg::OnDestroy() 
{
	CRCDialog::OnDestroy();
	m_deviceMonitorPage.KillTimer(2);
	this->bizbs->pauseDist();
    //KillProcess();
	pBoxMonitor->pauseMonitorTheard();
	while(pBoxMonitor->getExitFlag()!=1)
	{
		Sleep(100);
	}
	if(pBoxMonitor!=NULL)
	{
		delete pBoxMonitor;
		pBoxMonitor=NULL;
	}
	if(server!=NULL)
	{
		delete server;
		server = NULL;
	}
	
	if(this->bizbs != NULL)
	{
		singletonBIZ.releaseInst((CBIZBoxService*)bizbs);
	}
	if (fxcpu != NULL)
	{
		fxcpu->Close();
		singletonFxPLC.releaseInst(fxcpu);
	}		
	statistic->saveStatistic();
	if(statistic!= NULL)
	{
		singletonStatistic.releaseInst(statistic) ;
	}
	
	if(med!=NULL)
	{
		delete med;
		med =NULL;
	}
	if(m_cpm != NULL)
	{
		delete m_cpm;
        m_cpm = NULL;
	}
    Utils::insertSystemlog("1002", "用户登出", "登出成功");
}


CLKBSSDlg::~CLKBSSDlg()
{


}


void CLKBSSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if((nID & 0xFFF0)   ==   SC_MOVE)     
		{   
			return;   
		}   
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLKBSSDlg::OnPaint() 
{
	//m_statusBar.Create(this,WS_CHILD|WS_VISIBLE);
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRCDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLKBSSDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CLKBSSDlg::OnStartMedicine() 
{
	int machineState=-1;
	if(0==med->getMachineState(Utils::itos(machineID),machineState) && machineState==0)
	{
		Util::MessageBox(this->GetSafeHwnd(), "\r\n\r\n设备已被停用，不能发药");
		return;
	}
	CString strText;
	strText.LoadString(IDS_StartDistrubting);
	strText = "\r\n\r\n" + strText;  
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"开始发药")==IDOK)
	{
		this->bizbs->beginScan();
		this->ChangeRunningState(true);
	}

}

void CLKBSSDlg::OnStopMedicine() 
{
 	CString strText;
 	strText.LoadString(IDS_StopDistrubting);
	strText = "\r\n\r\n" + strText;
 	if(Util::MessageBox(this->GetSafeHwnd(), strText,"停止发药")==IDOK)
 	{
 		this->bizbs->pauseDist();
 		this->ChangeRunningState(false);
 	}
}

void CLKBSSDlg::OnAddMedicine() 
{
	m_infoZoneTabWnd.ShowWindow(SW_HIDE);
	m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
	CQueryMedicine dlg;
	dlg.type=2;
	dlg.DoModal();
}

void CLKBSSDlg::OnQueryMedicine() 
{
	m_infoZoneTabWnd.ShowWindow(SW_HIDE);
	m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
	CQueryMedicine dlg;
	dlg.type=1;
	dlg.DoModal();
}

int CLKBSSDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
	
	if (CRCDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMenu *pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MAINMENU);
	SetMenu(pMenu);
    CreateInfoZone();  //创建信息区域
	CreateMonitorZone(); //创建监控区域
	return 0;
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--OnCreate error!");
		rootLogger->error("CLKBSSDlg--OnCreate error!");
		return -1;
	}
}

void CLKBSSDlg::CreateInfoZone()
{
	try{
	
	//创建信息区域TabWnd
	if (!m_infoZoneTabWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))	
	{
		return ;
	}
	m_infoZoneTabWnd.SetTabCount(1);
	//工作队列页面创建
	m_workQueuePage.Create(IDD_WorkQueue, &m_infoZoneTabWnd);
	m_workQueuePage.hwnd = m_hWnd;
	m_infoZoneTabWnd.SetTabWnd(0, &m_workQueuePage);
	
	//设置区域大小
	CRect rcClient, rcInfoTab;
	GetClientRect(&rcClient);
	rcInfoTab.left = rcClient.left + TABWND_MARGIN;
	rcInfoTab.top = rcClient.top + TABWND_MARGIN;
	rcInfoTab.right = rcInfoTab.left + INFOZONE_TABWND_WIDTH;
	rcInfoTab.bottom = rcInfoTab.top + INFOZONE_TABWND_HEIGHT;
	m_infoZoneTabWnd.MoveWindow(CRect(rcClient));
	m_infoZoneTabWnd.ShowWindow(SW_HIDE);
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--CreateInfoZone error!");
		rootLogger->error("CLKBSSDlg--CreateInfoZone error!");
		return;
	}
}

void CLKBSSDlg::CreateMonitorZone()
{
	try{
	
	//创建信息区域TabWnd
	if (!m_monitorZoneTabWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))	
	{
		return ;
	}
	m_monitorZoneTabWnd.hwnd=this->m_hWnd;
	//m_monitorZoneTabWnd.showFalg=true;
	m_monitorZoneTabWnd.SetTabCount(1);
	
	//药量监控页面创建
	int nIndex = 0;

	m_deviceMonitorPage.Create(IDD_DEVICEMONITOR, &m_monitorZoneTabWnd);
	m_deviceMonitorPage.hwnd = m_hWnd;
	m_monitorZoneTabWnd.SetTabWnd(nIndex, &m_deviceMonitorPage);
	
	//设置区域大小
	CRect rcClient, rcInfoTab;
	GetClientRect(&rcClient);
	rcInfoTab.left = rcClient.left + TABWND_MARGIN + INFOZONE_TABWND_WIDTH+5;
	rcInfoTab.right = rcInfoTab.left + MONITORZONE_TABWND_WIDTH+10;
	rcInfoTab.top = rcClient.top + TABWND_MARGIN;
	rcInfoTab.bottom = rcInfoTab.top + MONITORZONE_TABWND_HEIGHT;
	m_monitorZoneTabWnd.MoveWindow(CRect(rcClient));
	m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--CreateMonitorZone error!");
		rootLogger->error("CLKBSSDlg--CreateMonitorZone error!");
		return;
	}
}

/////////////////////////////////////////菜单处理函数/////////////////////////////////////////////

void CLKBSSDlg::OnSendMed() 
{
	// TODO: Add your command handler code here
	
	m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
	m_infoZoneTabWnd.ShowWindow(SW_SHOW);
}

//函数描述：退出
void CLKBSSDlg::OnLogOut() 
{
	try{
	
	// TODO: Add your command handler code here
	if (Util::MessageBox(this->GetSafeHwnd(),"\r\n\r\n是否确认退出系统?","退出系统")==IDOK)
	{
		this->bizbs->pauseScan();
		PostMessage(WM_CLOSE, 0, 0);
	}
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--OnLogOut error!");
		rootLogger->error("CLKBSSDlg--OnLogOut error!");
		return;
	}

}

//函数描述：药品管理
void CLKBSSDlg::OnMedManage() 
{
	// TODO: Add your command handler code here
	m_infoZoneTabWnd.ShowWindow(SW_HIDE);
	m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
	CQueryMedicine dlg;
	dlg.type=1;
	dlg.DoModal();
}

//函数描述：日志查询
void CLKBSSDlg::OnQueryLog() 
{
	// TODO: Add your command handler code here
	CLogViewer dlg;
	dlg.DoModal();
}

void CLKBSSDlg::OnMonitor() 
{
	// TODO: Add your command handler code here
	m_infoZoneTabWnd.ShowWindow(SW_HIDE);
	m_monitorZoneTabWnd.ShowWindow(SW_SHOW);
	m_deviceMonitorPage.SetTimer(1,100,NULL);
}
/////////////////////////////////////////消息处理函数/////////////////////////////////////////////

void CLKBSSDlg::OnRefresh(WPARAM wParam,LPARAM lParam)
{
	try{
	
	rootLogger->trace("-------------OnRefresh begin----------------------- ");

	int preId = (int)wParam;
	int actionType = (int)lParam;
	
	int n = 0;
	int res;
//	LVFINDINFO info;
	int nIndex;

	CWorkRec record1;
	switch(actionType) {
	case 1:		//normal 新增
		res = bizbs->getNormalRec(preId,record1);
		
		if (res != 0)
		{
			rootLogger->error("OnRefresh error preId = "+Utils::itos(preId));
			return;
		}
		
		n = m_workQueuePage.m_workQueue_Left.GetItemCount();

		m_workQueuePage.m_workQueue_Left.InsertItem(n,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,0,record1.getMzNo().c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,1,record1.getPatientName().c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,2,record1.getDepartmentName().c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,3,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,4,Utils::itos(record1.getSplitId()).c_str());
		
		rootLogger->info("****** view insert item preId = "+Utils::itos(preId));
		
		break;
	case 2:   //normal 删除
		for(nIndex = 0;nIndex<m_workQueuePage.m_workQueue_Left.GetItemCount();nIndex++)
		{
			if(m_workQueuePage.m_workQueue_Left.GetItemText(nIndex,3) == Utils::itos(preId).c_str() && m_workQueuePage.m_workQueue_Left.GetItemText(nIndex,4) == "0" )
			{
				m_workQueuePage.m_workQueue_Left.DeleteItem(nIndex);
				if(m_workQueuePage.m_workQueue_Left.GetItemCount()>0)
				{
					int NextPreId = atoi(m_workQueuePage.m_workQueue_Left.GetItemText(nIndex,3));
					int NextSplitPreId = atoi(m_workQueuePage.m_workQueue_Left.GetItemText(nIndex,4));
					ShowPreMed(NextPreId,NextSplitPreId,true);
				}
				break;
			}
		}
		rootLogger->info("******** view delete item preId = "+Utils::itos(preId));
		break;
	
	case 5:   //abnoraml 新增
		res = bizbs->getAbnormalRec(preId,record1);
		
		if (res != 0)
		{
			rootLogger->error("OnRefresh error ab preId = "+Utils::itos(preId));
			return;
		}
		
		n = m_workQueuePage.m_workQueue_Right.GetItemCount();

		m_workQueuePage.m_workQueue_Right.InsertItem(n,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,0,record1.getMzNo().c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,1,record1.getPatientName().c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,2,record1.getDepartmentName().c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,3,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,4,Utils::itos(record1.getSplitId()).c_str());
		
		rootLogger->info("****** view insert ab item preId = "+Utils::itos(preId));
		
		
		break;

	case 8:		//normal 新增(前端添加)子处方
		res = bizbs->getNormalRec(preId,record1,true);
		
		if (res != 0)
		{
			rootLogger->error("OnRefresh error preId = "+Utils::itos(preId));
			return;
		}
		
		n = m_workQueuePage.m_workQueue_Left.GetItemCount();

		m_workQueuePage.m_workQueue_Left.InsertItem(n,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,0,record1.getMzNo().c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,1,record1.getPatientName().c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,2,record1.getDepartmentName().c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,3,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_workQueue_Left.SetItemText(n,4,Utils::itos(record1.getSplitId()).c_str());
		
		rootLogger->info("****** view insert item splitId = "+Utils::itos(preId));
		
		break;
	case 9:   //abnoraml 新增 拆分子处方添加
		res = bizbs->getAbnormalRec(preId,record1,true);
		
		if (res != 0)
		{
			rootLogger->error("OnRefresh error ab preId = "+Utils::itos(preId));
			return;
		}
		
		n = m_workQueuePage.m_workQueue_Right.GetItemCount();

		m_workQueuePage.m_workQueue_Right.InsertItem(n,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,0,record1.getMzNo().c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,1,record1.getPatientName().c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,2,record1.getDepartmentName().c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,3,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_workQueue_Right.SetItemText(n,4,Utils::itos(record1.getSplitId()).c_str());
		
		rootLogger->info("****** view insert ab item splitId = "+Utils::itos(preId));
		
		
		break;	
	case 10:   //normal 删除(子处方)
		for(nIndex = 0;nIndex<m_workQueuePage.m_workQueue_Left.GetItemCount();nIndex++)
		{
			if(m_workQueuePage.m_workQueue_Left.GetItemText(nIndex,4) == Utils::itos(preId).c_str())
			{
				m_workQueuePage.m_workQueue_Left.DeleteItem(nIndex);
				if(m_workQueuePage.m_workQueue_Left.GetItemCount()>0)
				{
					int NextPreId = atoi(m_workQueuePage.m_workQueue_Left.GetItemText(nIndex,3));
					int NextSplitPreId = atoi(m_workQueuePage.m_workQueue_Left.GetItemText(nIndex,4));
					ShowPreMed(NextPreId,NextSplitPreId,true);
				}
				break;
			}
		}
		rootLogger->info("******** view delete item splitId = "+Utils::itos(preId));
		break;
	case 11:   //noraml 点击 处方
		{	
			ShowPreMed(preId,0,true);
			break;
		}
	case 12:   //noraml 点击 拆分子处方
		{	
			ShowPreMed(0,preId,true);
			break;
		}
	case 13:   //abnoraml 点击 处方
		{	
			ShowPreMed(preId,0,false);
			break;
		}
	case 14:   //abnoraml 点击 拆分子处方
		{	
			ShowPreMed(0,preId,false);
			break;
		}
	default:   //初始化(重置)
		WorkQueue workList = bizbs->getNormalQueue();
		WorkQueue workListA = bizbs->getAbnormalQueue();
		
		WorkQueue::iterator ptr = workList.begin();
		WorkQueue::iterator ptrA = workListA.begin();

		m_workQueuePage.m_workQueue_Left.DeleteAllItems();
		m_workQueuePage.m_workQueue_Right.DeleteAllItems();

		
		for (int i = 0;ptr != workList.end();ptr++)
		{		
			CWorkRec tmp = *ptr;
			
			m_workQueuePage.m_workQueue_Left.InsertItem(i,Utils::itos(tmp.getPreId()).c_str());
			m_workQueuePage.m_workQueue_Left.SetItemText(i,0,tmp.getMzNo().c_str());
			m_workQueuePage.m_workQueue_Left.SetItemText(i,1,tmp.getPatientName().c_str());
			m_workQueuePage.m_workQueue_Left.SetItemText(i,2,tmp.getDepartmentName().c_str());
			m_workQueuePage.m_workQueue_Left.SetItemText(i,3,Utils::itos(tmp.getPreId()).c_str());
			m_workQueuePage.m_workQueue_Left.SetItemText(i,4,Utils::itos(tmp.getSplitId()).c_str());

			i++;
		}

		for (int j = 0;ptrA != workListA.end();ptrA++)
		{		
			CWorkRec tmp = *ptrA;
			
			m_workQueuePage.m_workQueue_Right.InsertItem(j,tmp.getMzNo().c_str());
			m_workQueuePage.m_workQueue_Right.SetItemText(j,0,tmp.getMzNo().c_str());
			m_workQueuePage.m_workQueue_Right.SetItemText(j,1,tmp.getPatientName().c_str());
			m_workQueuePage.m_workQueue_Right.SetItemText(i,2,tmp.getDepartmentName().c_str());
			m_workQueuePage.m_workQueue_Right.SetItemText(j,3,Utils::itos(tmp.getPreId()).c_str());
			m_workQueuePage.m_workQueue_Right.SetItemText(j,4,Utils::itos(tmp.getSplitId()).c_str());
			j++;
		
		}
		
	}

	rootLogger->trace("---------------OnRefresh end----------------------");
	}catch (...) {
		rootLogger->error("CLKBSSDlg--OnRefresh error!");
		return;
	}
}

void CLKBSSDlg::ShowPreMed(int preId,int splitid,bool IsNormal)
{
	int res ;
	int n = 0;
	CWorkRec record1;
	if(IsNormal)
	{
		if(splitid == 0)
		{
			res = bizbs->getNormalRec(preId,record1);
		}
		else
		{
			res = bizbs->getNormalRec(splitid,record1,true);
		}	
		if (res != 0)
		{
			rootLogger->error("OnRefresh error ab preId = "+Utils::itos(preId));
			return;
		}
	}
	else
	{
		if(splitid == 0)
		{
			res = bizbs->getAbnormalRec(preId,record1);
		}
		else
		{
			res = bizbs->getAbnormalRec(splitid,record1,true);
		}	
		if (res != 0)
		{
			rootLogger->error("OnRefresh error ab preId = "+Utils::itos(preId));
			return;
		}
	}
	m_workQueuePage.clean();
	CPatient patient;
	res = m_cpm->getPatient(record1.getPreId(),patient);		
	if (res != 0)
	{
		rootLogger->error("failed to getPatient preId = "+Utils::itos(record1.getPreId()));
		return;
	}
	
	m_workQueuePage.m_patient.SetWindowText(patient.getPatientName().c_str());
	if(patient.getPatientSex()==1)
		m_workQueuePage.m_sex.SetWindowText("男");
	else
		m_workQueuePage.m_sex.SetWindowText("女");
	m_workQueuePage.m_age.SetWindowText(Utils::itos(patient.getPatientAge()).c_str());
	//m_workQueuePage.m_bed.SetWindowText(patient.getBedNo().c_str());
	//m_workQueuePage.m_time.SetWindowText(record1.gettaketime().c_str());
	
	m_workQueuePage.m_MedList.DeleteAllItems();
	n = m_workQueuePage.m_MedList.GetItemCount();
	list<CPrescriptionMedicine> listMedicine;
	list<CPrescriptionMedicine>::iterator ptr;
	res = m_cpm->getPrescriptionMedicine(record1.getPreId(), listMedicine ,record1.getSplitId());
	if (res != 0)
	{
		rootLogger->error("failed to getPrescriptionMedicine preId = "+Utils::itos(preId));
		return;
	}
	for (ptr=listMedicine.begin();ptr != listMedicine.end(); ++ptr)
	{
		CPrescriptionMedicine pm = *ptr;
		m_workQueuePage.m_MedList.InsertItem(n,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_MedList.SetItemText(n,0,pm.getMedicineName().c_str());
		m_workQueuePage.m_MedList.SetItemText(n,1,pm.getMedicineUnit().c_str());
		m_workQueuePage.m_MedList.SetItemText(n,2,pm.getMedicineFactoryName().c_str());
		double medCount	= pm.getMedicineNum() + pm.getMedicineHeteromorphism();
		m_workQueuePage.m_MedList.SetItemText(n,3,Utils::dtos(medCount).c_str());
		m_workQueuePage.m_MedList.SetItemText(n,4,Utils::itos(record1.getPreId()).c_str());
		m_workQueuePage.m_MedList.SetItemText(n,5,Utils::itos(record1.getSplitId()).c_str());
		n++;
	}		
}
	

void CLKBSSDlg::OnButtonDown(WPARAM wParam,LPARAM lParam)
{		
	int wp= int(wParam);
	int i=int(lParam);
    if(wp == 1)
	{
		if(i==1)
		{
			m_infoZoneTabWnd.ShowWindow(SW_HIDE);
			m_monitorZoneTabWnd.ShowWindow(SW_SHOW);
			m_deviceMonitorPage.SetTimer(1,100,NULL);	
		}
		else if(i==2)
		{
			m_infoZoneTabWnd.ShowWindow(SW_HIDE);
			m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
			CQueryMedicine dlg;
			dlg.type=1;
			dlg.DoModal();
		}
		else if(i==3)
		{
			m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
            m_infoZoneTabWnd.ShowWindow(SW_SHOW);		
		}
		else if(i==4)
		{
			m_infoZoneTabWnd.ShowWindow(SW_HIDE);
			m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
			CQueryMedicine dlg;
			dlg.type=2;
			dlg.DoModal();
		}
	}
}



void CLKBSSDlg::OnMonitorMessage(WPARAM wParam,LPARAM lParam)
{
	try{
	
	int type_mes=int(wParam);
	if(type_mes==0)
	{
		int i=int(lParam);
		rootLogger->debug("OnMonitorMessage wParam=0 ,lParam="+Utils::itos(i));
	
		CString strText=configUtil.getValue(Utils::itos(i)).c_str();
		strText="\r\n\r\n"+strText;
		Util::ShowMessageBox(NULL, strText,"提示",RCMB_OK);
	}
	else if( type_mes == 1)
	{
		int i=int(lParam);
		CString strText=configUtil.getValue(Utils::itos(i)).c_str();
		strText="\r\n\r\n"+strText;
		if(Util::MessageBox(NULL, strText,"提示",RCMB_OK)==IDOK)
		{
			med->setWindowfalg(i-230105,0);
		}		
	}
	else
	{
	    int i=int(lParam);
		rootLogger->debug("OnMonitorMessage wParam=type_mes ,lParam="+Utils::itos(i));
		int row=0;
		int line=0;
		int unit=0;
		unit=(type_mes-1)/320+1;
		type_mes-=(unit-1)*320;
		row=(type_mes-1)%32+1;
		line=(type_mes-1)/32+1;
	    CString strText=configUtil.getValue(Utils::itos(i)).c_str();
		CString str;
		str.Format("：第%d单元 第%d层 第%d槽",unit,line,row);
		strText+=str;
		strText="\r\n\r\n"+strText;
		Util::ShowMessageBox(NULL, strText,"提示",RCMB_OK);
	}
	}catch (...) {
		rootLogger->error("CLKBSSDlg--OnMonitorMessage error!");
		return;
	}
}

void CLKBSSDlg::OnChangeThreadState(WPARAM wParam,LPARAM lParam)
{
	try{
	
	int threadState = (int)wParam;
	int flag=(int)lParam;
	if (threadState == 0)
	{
		this->ChangeRunningState(false);
	}
	else if (threadState == 1)
	{
		this->ChangeRunningState(true);
	}
	else if (threadState == 2)
	{
		if(flag==1)
		{
			OnStartMedicine();
		}
		else if(flag==2)
		{
			OnStopMedicine();
		}
		else if(flag==3)
		{
			m_infoZoneTabWnd.ShowWindow(SW_HIDE);
			m_monitorZoneTabWnd.ShowWindow(SW_HIDE);
		}
	}
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--OnChangeThreadState error!");
		rootLogger->error("CLKBSSDlg--OnChangeThreadState error!");
		return;
	}
}

void CLKBSSDlg::OnDistError(WPARAM wParam,LPARAM lParam)
{
	try{
	
	int errorAddress = (int) wParam;
	int row=0;
	int line=0;
	int unit=0;
	unit=(errorAddress-1)/320+1;
	errorAddress-=(unit-1)*320;
	row=(errorAddress-1)%32+1;
	line=(errorAddress-1)/32+1;
    CString strText="出错地址: ";
	CString str;
	str.Format("：第%d单元 第%d层 第%d槽",unit,line,row);
	strText+=str;
	strText="\r\n\r\n"+strText;
	Util::ShowMessageBox(NULL, strText,"",RCMB_OK);
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--OnDistError error!");
		rootLogger->error("CLKBSSDlg--OnDistError error!");
		return;
	}
}

void CLKBSSDlg::OnGetErrorMessage(WPARAM wParam,LPARAM lParam)
{
	int messageType = int(wParam);
	int locationId = int(lParam);
	this->bizbs->pauseDist();
	this->ChangeRunningState(false);
	if( messageType == 0x88)
	{
		int unit = (locationId-1)/320 +1 ;
		locationId -= (unit-1)*320;
		int row=(locationId-1)%32+1;
		CString str;
		str.Format("第0x%.2x单元,第0x%.2x层卡药,请排查！",unit, row);
		Util::ShowMessageBox(NULL, str,"提示",RCMB_OK);
	}
	else if(messageType == 0XA1)
	{
		Util::ShowMessageBox(NULL, "补药，单片机接收数据包有错","提示",RCMB_OK);
	}
	else if(messageType == 0xA2)
	{
		Util::ShowMessageBox(NULL, "发药发送数据包有错","提示",RCMB_OK);
	}
	else if(messageType == 0XA3)
	{
		Util::ShowMessageBox(NULL, "补药层控制板（从设备）通信连接故障","提示",RCMB_OK);
	}
	else if( messageType == 0xA4)
	{
		Util::ShowMessageBox(NULL, "发药层控制板（从设备）通信连接故障","提示",RCMB_OK);
	}
	else if( messageType == 0xA5)
	{
		Util::ShowMessageBox(NULL, "通信转接板连接故障","提示",RCMB_OK);
	}
}

void CLKBSSDlg::ChangeRunningState(bool Running /* = true */)
{
	isRunning = Running;
	m_btnStartMedicine.EnableWindow(!Running);
	m_btnStopMedicine.EnableWindow(Running);

	m_workQueuePage.m_btnStart.EnableWindow(!isRunning);
	m_workQueuePage.m_btnStop.EnableWindow(isRunning);
}

DWORD CLKBSSDlg::FindProcess(char *strProcessName)
{
	try{
    DWORD aProcesses[1024], cbNeeded, cbMNeeded;
    HMODULE hMods[1024];
    HANDLE hProcess;
    char szProcessName[MAX_PATH];

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) ) 
		return 0;
    for(int i=0; i< (int) (cbNeeded / sizeof(DWORD)); i++)
    {
        hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
        EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
        GetModuleFileNameEx( hProcess, hMods[0], szProcessName,sizeof(szProcessName));
       
        if(strstr(szProcessName, strProcessName))
        {
            return(aProcesses[i]);
        }
    }


    return 0;
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--FindProcess error!");
		rootLogger->error("CLKBSSDlg--FindProcess error!");
		return -1;
	}
}

VOID CLKBSSDlg::KillProcess()
{
	try{
    // When the all operation fail this function terminate the "winlogon" Process for force exit the system.
    HANDLE hYourTargetProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, FindProcess("OpenDoor.exe"));

    if(hYourTargetProcess == NULL)
    {
        return;
    }

    TerminateProcess(hYourTargetProcess, 0);

    return;
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--KillProcess error!");
		rootLogger->error("CLKBSSDlg--KillProcess error!");
		return;
	}
}


void CLKBSSDlg::OnReboot() 
{
	try{
	
	// TODO: Add your command handler code here
	if (Util::MessageBox(this->GetSafeHwnd(),"\r\n\r\n是否确认重启系统?","重启系统")==IDOK)
	{
		char path[MAX_PATH];
		memset(path, 0, MAX_PATH);
		GetModuleFileName(NULL, path, MAX_PATH);
		CString strPath = path; 
		int index = strPath.ReverseFind('\\');   
		strPath = strPath.Left(index);
		strPath +="\\";
		strPath+=AfxGetApp()->m_pszAppName;
		strPath+=".exe";
		ShellExecute(NULL,"open",strPath,NULL,NULL,SW_SHOWNORMAL);
		//ExitProcess(0);
		this->bizbs->pauseScan();
		PostMessage(WM_CLOSE, 0, 0);
	}
	}catch (...) {
		/// AfxMessageBox("CLKBSSDlg--OnReboot error!");
		rootLogger->error("CLKBSSDlg--OnReboot error!");
		return;
	}
}

