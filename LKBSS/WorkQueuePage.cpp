// WorkQueuePage.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "WorkQueuePage.h"
#include "Util.h"
#include "ConfigUtil.h"

#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
LoggerPtr CWorkQueuePage::rootLogger = Logger::getLogger("CWorkQueuePage");  //初始化日志
/////////////////////////////////////////////////////////////////////////////
// CWorkQueuePage dialog


CWorkQueuePage::CWorkQueuePage(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkQueuePage::IDD, pParent)
{
	try{
	
	//{{AFX_DATA_INIT(CWorkQueuePage)
	//}}AFX_DATA_INIT
	
	cdb = new CDBManage();
	string msg = configUtil.getValue("logPropFile");    //日志模块加载配置文件
	PropertyConfigurator::configure(msg);

	}catch (...) {
		/// AfxMessageBox("CWorkQueuePage--CWorkQueuePage error.");
		rootLogger->fatal("CWorkQueuePage--CWorkQueuePage error.");
		return ;
	}
}

CWorkQueuePage::~CWorkQueuePage(){
	try{
	
	if(this->bizbs != NULL){
		singletonBIZ.releaseInst((CBIZBoxService*)bizbs);
	}
	if (cdb != NULL)
	{
		delete cdb;
	}
	}catch (...) {
		/// AfxMessageBox("CWorkQueuePage--~CWorkQueuePage error.");
		rootLogger->fatal("CWorkQueuePage--~CWorkQueuePage error.");
		return ;
	}
}
void CWorkQueuePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkQueuePage)
	DDX_Control(pDX, IDC_BUTSTOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTSTART, m_btnStart);
	DDX_Control(pDX, IDC_BUTBACK, m_btnBack);
	DDX_Control(pDX, IDC_PATIENT, m_patient);
	DDX_Control(pDX, IDC_SEX, m_sex);
	DDX_Control(pDX, IDC_AGE, m_age);
	DDX_Control(pDX, IDC_STATGROUP, m_Group);
	DDX_Control(pDX, IDC_LISTMED, m_MedList);
	DDX_Control(pDX, IDC_Move_Left_All, m_MoveLeftAll);
	DDX_Control(pDX, IDC_MoveUp, m_btnUp);
	DDX_Control(pDX, IDC_MoveRight, m_btnRight);
	DDX_Control(pDX, IDC_MoveLeft, m_btnLeft);
	DDX_Control(pDX, IDC_MoveDown, m_btnDown);
	DDX_Control(pDX, IDC_WorkQueue_RightList, m_workQueue_Right);
	DDX_Control(pDX, IDC_WorkQueue_LeftList, m_workQueue_Left);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorkQueuePage, CDialog)
	//{{AFX_MSG_MAP(CWorkQueuePage)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MoveRight, OnMoveRight)
	ON_BN_CLICKED(IDC_MoveLeft, OnMoveLeft)
	ON_BN_CLICKED(IDC_MoveUp, OnMoveUp)
	ON_BN_CLICKED(IDC_MoveDown, OnMoveDown)
	ON_BN_CLICKED(IDC_Move_Left_All, OnMoveLeftAll)
	ON_BN_CLICKED(IDC_BUTSTART, OnButstart)
	ON_BN_CLICKED(IDC_BUTSTOP, OnButstop)
	ON_BN_CLICKED(IDC_BUTBACK, OnButback)
	ON_NOTIFY(NM_CLICK, IDC_WorkQueue_LeftList, OnClickWorkQueueLeftList)
	ON_NOTIFY(NM_CLICK, IDC_WorkQueue_RightList, OnClickWorkQueueRightList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWorkQueuePage::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
 	GetClientRect(&rect);
	CBrush brush(GROUP_BK_COLOR);
 	pDC->FillRect(rect, &brush);

	CRect rcRet, rcWnd;
	GetWindowRect(rcWnd);
	
	rcRet = rcWnd;
	rcRet.top = rcWnd.bottom - 73;
	rcRet.OffsetRect(-rcWnd.left, -rcWnd.top);
	
	//绘制底部区域
	CString strPath = Util::GetDialogResPath() + "\\BorderBottom.bmp";
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	Util::xTileRgnWithBmp(pDC, rcRet, hBitmap);
	DeleteObject(hBitmap);
	hBitmap = NULL;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkQueuePage message handlers

HBRUSH CWorkQueuePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	

	 if(nCtlColor == CTLCOLOR_STATIC)  //针对所有的标签
	{
		pDC->SetBkMode( TRANSPARENT );
		pDC->SetTextColor(RGB(255, 255, 255));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else if (nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkColor(INFOZONE_BK_COLOR);   
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CWorkQueuePage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->bizbs = singletonBIZ.getInst();
	// TODO: Add extra initialization here
	InitButton();
    InitMedList();
	InitLeftWorkList();
	InitRightWorkList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorkQueuePage::InitLeftWorkList()
{
	try{
	
	m_workQueue_Left.Init();
	m_workQueue_Left.SetTextColor(RGB(125,125,125));	
	m_workQueue_Left.InsertColumn(0, "门诊号", LVCFMT_LEFT, 60);
	m_workQueue_Left.InsertColumn(1, "患者", LVCFMT_LEFT, 80);
	m_workQueue_Left.InsertColumn(2, "科室", LVCFMT_LEFT, 60);
	m_workQueue_Left.InsertColumn(3, "处方号", LVCFMT_LEFT, 60);
	m_workQueue_Left.InsertColumn(4, "子处方号", LVCFMT_LEFT, 0);
	
	m_workQueue_Left.SetRedraw(FALSE);	
	m_workQueue_Left.SetRedraw(TRUE);
	ListView_SetExtendedListViewStyle(m_workQueue_Left.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
	}catch (...) {
		/// AfxMessageBox("CDoseMonitorPage8--InitLeftWorkList error.");
		rootLogger->fatal("CDoseMonitorPage8--InitLeftWorkList error.");
		return ;
	}
}

void CWorkQueuePage::InitRightWorkList()
{
	try{
	
	m_workQueue_Right.Init();
	m_workQueue_Right.SetTextColor(RGB(125,125,125));
	
	m_workQueue_Right.InsertColumn(0, "门诊号", LVCFMT_LEFT, 60);
	m_workQueue_Right.InsertColumn(1, "患者", LVCFMT_LEFT, 80);
	m_workQueue_Right.InsertColumn(2, "科室", LVCFMT_LEFT, 60);
	m_workQueue_Right.InsertColumn(3, "处方号", LVCFMT_LEFT, 60);
	m_workQueue_Right.InsertColumn(4, "子处方号", LVCFMT_LEFT, 0);
	m_workQueue_Right.SetRedraw(FALSE);

	m_workQueue_Right.SetRedraw(TRUE);
	ListView_SetExtendedListViewStyle(m_workQueue_Right.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
	}catch (...) {
		/// AfxMessageBox("CDoseMonitorPage8--InitRightWorkList error.");
		rootLogger->fatal("CDoseMonitorPage8--InitRightWorkList error.");
		return ;
	}
}

void CWorkQueuePage::InitMedList()
{
	m_MedList.Init();
	m_MedList.SetTextColor(RGB(125,125,125));	
	m_MedList.InsertColumn(0, "药品名", LVCFMT_LEFT, 100);
	m_MedList.InsertColumn(1, "规格", LVCFMT_LEFT, 80);
	m_MedList.InsertColumn(2, "厂家", LVCFMT_LEFT, 100);
	m_MedList.InsertColumn(3, "数量", LVCFMT_LEFT, 60);
	m_MedList.InsertColumn(4, "处方号", LVCFMT_LEFT, 0);
	m_MedList.InsertColumn(5, "子处方号", LVCFMT_LEFT, 0);
	m_MedList.SetRedraw(FALSE);
	m_MedList.SetRedraw(TRUE);
	ListView_SetExtendedListViewStyle(m_MedList.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
}

void CWorkQueuePage::InitButton()
{
	try{

	m_Group.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_Group.SetBackgroundColor(GROUP_BK_COLOR);
	m_Group.SetCatptionTextColor(RGB(255, 255, 255));
	//m_Group.SetFontBold(TRUE);
	m_Group.SetAlignment(SS_CENTER);
	
	//Right
	CString strNormalPath	= Util::GetDialogResPath() + "\\move_right.bmp";
	CString strDownPath		= Util::GetDialogResPath() + "\\move_right_on.bmp";
	CString strDisablePath  = Util::GetDialogResPath() + "\\move_right_disable.bmp";
	
	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Disable	= (HBITMAP)LoadImage(NULL, strDisablePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	m_btnRight.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_Disable);
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_Disable = NULL;

	//Left
	strNormalPath	= Util::GetDialogResPath() + "\\move_left.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\move_left_on.bmp";
	strDisablePath  = Util::GetDialogResPath() + "\\move_left_disable.bmp";

	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Disable	= (HBITMAP)LoadImage(NULL, strDisablePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	m_btnLeft.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_Disable);
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_Disable = NULL;

	//Up
	strNormalPath	= Util::GetDialogResPath() + "\\move_up.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\move_up_on.bmp";
	strDisablePath  = Util::GetDialogResPath() + "\\move_up_disable.bmp";
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Disable	= (HBITMAP)LoadImage(NULL, strDisablePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	m_btnUp.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_Disable);
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_Disable = NULL;

	//Down
	strNormalPath	= Util::GetDialogResPath() + "\\move_down.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\move_down_on.bmp";
	strDisablePath  = Util::GetDialogResPath() + "\\move_down_disable.bmp";
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Disable	= (HBITMAP)LoadImage(NULL, strDisablePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	m_btnDown.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_Disable);
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_Disable = NULL;

	//left all
	strNormalPath	= Util::GetDialogResPath() + "\\move_left_all.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\move_left_all_on.bmp";
	strDisablePath  = Util::GetDialogResPath() + "\\move_left_all_disable.bmp";
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Disable	= (HBITMAP)LoadImage(NULL, strDisablePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	m_MoveLeftAll.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_Disable);
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_Disable = NULL;

	//开始发药
	strNormalPath	= Util::GetDialogResPath() + "\\StartMedicine_normal.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\StartMedicine_down.bmp";
	strDisablePath	= Util::GetDialogResPath() + "\\StartMedicine_disable.bmp";
	
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Disable	= (HBITMAP)LoadImage(NULL, strDisablePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	m_btnStart.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_Disable);
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_Disable = NULL;

	//停止发药
	strNormalPath	= Util::GetDialogResPath() + "\\StopMedicine_normal.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\StopMedicine_down.bmp";
	strDisablePath	= Util::GetDialogResPath() + "\\StopMedicine_disable.bmp";

	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Disable	= (HBITMAP)LoadImage(NULL, strDisablePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	m_btnStop.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_Disable);
	m_btnStop.EnableWindow(false);
    
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_Disable = NULL;

	//返回
	strNormalPath	= Util::GetDialogResPath() + "\\back_normal.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\back_down.bmp";
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	m_btnBack.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	
	}catch (...) {
		/// AfxMessageBox("CDoseMonitorPage8--InitButton error.");
		rootLogger->fatal("CDoseMonitorPage8--InitButton error.");
		return ;
	}
}

BOOL CWorkQueuePage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//屏蔽回车，退出按钮
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))    
		return TRUE;   
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CWorkQueuePage::OnMoveRight() 
{
	// TODO: Add your control notification handler code here
	try{
	

	int positionN = this->m_workQueue_Left.GetNextItem(-1,LVNI_SELECTED);
	int positionA = this->m_workQueue_Right.GetNextItem(-1,LVNI_SELECTED);
	CString moid,patientname,departmentname,preid,preid2,splitId;
	if (positionN != -1)
	{
		moid=this->m_workQueue_Left.GetItemText(positionN,0);
		patientname=this->m_workQueue_Left.GetItemText(positionN,1);
		departmentname=this->m_workQueue_Left.GetItemText(positionN,2);
		preid=this->m_workQueue_Left.GetItemText(positionN,3);
		splitId = this->m_workQueue_Left.GetItemText(positionN,4);
		
		this->m_workQueue_Left.DeleteItem(positionN);
	}else{
		return;
	}
	if (positionA == -1)
	{
		positionA = this->m_workQueue_Right.GetItemCount();
		preid2 = "-1";
		
	}else{
		preid2 = this->m_workQueue_Right.GetItemText(positionA,3);
	}
	this->m_workQueue_Right.InsertItem(positionA,preid);
	this->m_workQueue_Right.SetItemText(positionA,0,moid);
	this->m_workQueue_Right.SetItemText(positionA,1,patientname);
	this->m_workQueue_Right.SetItemText(positionA,2,departmentname);
	this->m_workQueue_Right.SetItemText(positionA,3,preid);
	this->m_workQueue_Right.SetItemText(positionA,4,splitId);
	
	this->m_workQueue_Right.SetItemState(positionA,LVNI_SELECTED,LVNI_SELECTED);
	this->m_workQueue_Right.SetItemState(positionA+1,NULL,LVNI_SELECTED);
	this->bizbs->spliceToAbnormalQueue(cdb,atoi(preid),atoi(splitId),atoi(preid2));

	}catch (...) {
		/// AfxMessageBox("CDoseMonitorPage8--OnMoveRight error.");
		rootLogger->fatal("CDoseMonitorPage8--OnMoveRight error.");
		return ;
	}
}

void CWorkQueuePage::OnMoveLeft() 
{
	try{
	
	// TODO: Add your control notification handler code here
	int positionN = this->m_workQueue_Left.GetNextItem(-1,LVNI_SELECTED);
	int positionA = this->m_workQueue_Right.GetNextItem(-1,LVNI_SELECTED);
	CString moid,patientname,departmentname,preid,preid2,splitId;
	if (positionA != -1)
	{
		moid=this->m_workQueue_Right.GetItemText(positionA,0);
		patientname=this->m_workQueue_Right.GetItemText(positionA,1);
		departmentname=this->m_workQueue_Right.GetItemText(positionA,2);
		preid=this->m_workQueue_Right.GetItemText(positionA,3);
		splitId=this->m_workQueue_Right.GetItemText(positionA,4);
		this->m_workQueue_Right.DeleteItem(positionA);
	}else{
		return;
	}
	if (positionN == -1)
	{
		positionN = this->m_workQueue_Left.GetItemCount();
		preid2 = "-1";
		
	}else{
		preid2 = this->m_workQueue_Left.GetItemText(positionN,3);
	}
	this->m_workQueue_Left.InsertItem(positionN,preid);
	this->m_workQueue_Left.SetItemText(positionN,0,moid);
	this->m_workQueue_Left.SetItemText(positionN,1,patientname);
	this->m_workQueue_Left.SetItemText(positionN,2,departmentname);
	this->m_workQueue_Left.SetItemText(positionN,3,preid);
	this->m_workQueue_Left.SetItemText(positionN,4,splitId);
	
	this->m_workQueue_Left.SetItemState(positionN,LVNI_SELECTED,LVNI_SELECTED);
	this->m_workQueue_Left.SetItemState(positionN+1,NULL,LVNI_SELECTED);

	this->bizbs->spliceToNormalQueue(cdb,atoi(preid),atoi(splitId),atoi(preid2));
	}catch (...) {
		/// AfxMessageBox("CDoseMonitorPage8--OnMoveLeft error.");
		rootLogger->fatal("CDoseMonitorPage8--OnMoveLeft error.");
		return ;
	}
}

void CWorkQueuePage::OnMoveUp() 
{
	try{
	
	// TODO: Add your control notification handler code here
	int positionN = this->m_workQueue_Left.GetNextItem(-1,LVNI_SELECTED);
	CString moid,patientname,departmentname,preid,splitId;
	if (positionN != -1 && positionN != 0 )
	{
		moid=this->m_workQueue_Left.GetItemText(positionN,0);
		patientname=this->m_workQueue_Left.GetItemText(positionN,1);
		departmentname=this->m_workQueue_Left.GetItemText(positionN,2);
		preid=this->m_workQueue_Left.GetItemText(positionN,3);
		splitId=this->m_workQueue_Left.GetItemText(positionN,4);

		this->m_workQueue_Left.DeleteItem(positionN);
	}else{
		return;
	}
	this->m_workQueue_Left.InsertItem(--positionN,preid);
	this->m_workQueue_Left.SetItemText(positionN,0,moid);
	this->m_workQueue_Left.SetItemText(positionN,1,patientname);
	this->m_workQueue_Left.SetItemText(positionN,2,departmentname);
	this->m_workQueue_Left.SetItemText(positionN,3,preid);
	this->m_workQueue_Left.SetItemText(positionN,4,splitId);

	this->m_workQueue_Left.SetItemState(positionN,LVNI_SELECTED,LVNI_SELECTED);

	this->bizbs->skipForword(atoi(preid),atoi(splitId));
	}catch (...) {
		/// AfxMessageBox("CDoseMonitorPage8--OnMoveUp error.");
		rootLogger->fatal("CDoseMonitorPage8--OnMoveUp error.");
		return ;
	}
}

void CWorkQueuePage::OnMoveDown() 
{
	try{
	
	// TODO: Add your control notification handler code here
	int positionN = this->m_workQueue_Left.GetNextItem(-1,LVNI_SELECTED);
	CString moid,patientname,departmentname,preid,splitId;
	if (positionN != -1 && positionN != this->m_workQueue_Left.GetItemCount()-1 )
	{
		moid=this->m_workQueue_Left.GetItemText(positionN,0);
		patientname=this->m_workQueue_Left.GetItemText(positionN,1);
		departmentname=this->m_workQueue_Left.GetItemText(positionN,2);
		preid=this->m_workQueue_Left.GetItemText(positionN,3);
		splitId=this->m_workQueue_Left.GetItemText(positionN,4);
		this->m_workQueue_Left.DeleteItem(positionN);
	}else{
		return;
	}
	this->m_workQueue_Left.InsertItem(++positionN,preid);
	this->m_workQueue_Left.SetItemText(positionN,0,moid);
	this->m_workQueue_Left.SetItemText(positionN,1,patientname);
	this->m_workQueue_Left.SetItemText(positionN,2,departmentname);
	this->m_workQueue_Left.SetItemText(positionN,3,preid);
	this->m_workQueue_Left.SetItemText(positionN,4,splitId);

	this->m_workQueue_Left.SetItemState(positionN,LVNI_SELECTED,LVNI_SELECTED);

	this->bizbs->skipBack(atoi(preid),atoi(splitId));
	}catch (...) {
		/// AfxMessageBox("CDoseMonitorPage8--OnMoveDown error.");
		rootLogger->fatal("CDoseMonitorPage8--OnMoveDown error.");
		return ;
	}
	
}


void CWorkQueuePage::OnMoveLeftAll() 
{
	try{
	
	// TODO: Add your control notification handler code here
	int n = m_workQueue_Right.GetItemCount();
	string cIdQueue;
	string sql;
	for (int i =0;i<n;i++)
	{		
		CString moid,patientname,departmentname,preid,splitId;
		moid=this->m_workQueue_Right.GetItemText(0,0);
		patientname=this->m_workQueue_Right.GetItemText(0,1);
		departmentname=this->m_workQueue_Right.GetItemText(0,2);
		preid=this->m_workQueue_Right.GetItemText(0,3);
		splitId=this->m_workQueue_Right.GetItemText(0,4);
		if (i==0)
		{
			cIdQueue = preid;
			sql = " pre_id =" +preid + " and split_id = " +splitId;
		}else{
			cIdQueue += ","+preid;
			sql += " or ( pre_id =" +preid + " and split_id = " +splitId +") ";
		}
		
		this->m_workQueue_Right.DeleteItem(0);

		int positionN = this->m_workQueue_Left.GetItemCount();
		
		this->m_workQueue_Left.InsertItem(positionN,preid);
		this->m_workQueue_Left.SetItemText(positionN,0,moid);
		this->m_workQueue_Left.SetItemText(positionN,1,patientname);
		this->m_workQueue_Left.SetItemText(positionN,2,departmentname);
		this->m_workQueue_Left.SetItemText(positionN,3,preid);
		this->m_workQueue_Left.SetItemText(positionN,4,splitId);
		
		this->m_workQueue_Left.SetItemState(positionN,LVNI_SELECTED,LVNI_SELECTED);
		this->m_workQueue_Left.SetItemState(positionN+1,NULL,LVNI_SELECTED);
	}	
	//string sIdQueue = cIdQueue.LockBuffer();

	this->bizbs->spliceToNormalQueueMulti(cdb,sql);
	}catch (...) {
		/// AfxMessageBox("CDoseMonitorPage8--OnMoveLeftAll error.");
		rootLogger->fatal("CDoseMonitorPage8--OnMoveLeftAll error.");
		return ;
	}
}

void CWorkQueuePage::OnButstart() 
{
	// TODO: Add your control notification handler code here
	::SendMessage(hwnd,WM_THREADSTATE,2,1);
}

void CWorkQueuePage::OnButstop() 
{
	// TODO: Add your control notification handler code here
	::SendMessage(hwnd,WM_THREADSTATE,2,2);
}

void CWorkQueuePage::OnButback() 
{
	// TODO: Add your control notification handler code here
	::SendMessage(hwnd,WM_THREADSTATE,2,3);
}

void CWorkQueuePage::OnClickWorkQueueLeftList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int positionN = this->m_workQueue_Left.GetNextItem(-1,LVNI_SELECTED);
	CString preid,splitId;
	if (positionN != -1 )
	{
		preid=this->m_workQueue_Left.GetItemText(positionN,3);
        splitId=this->m_workQueue_Left.GetItemText(positionN,4);
		if(atoi(splitId)==0)
		{
			::SendMessage(hwnd,WM_REFRESH,atoi(preid),11);
		}
		else
		{
			::SendMessage(hwnd,WM_REFRESH,atoi(splitId),12);
		}
	}
	*pResult = 0;
}

void CWorkQueuePage::OnClickWorkQueueRightList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int positionA = this->m_workQueue_Right.GetNextItem(-1,LVNI_SELECTED);
	CString preid,splitId;
	if (positionA != -1 )
	{
		preid=this->m_workQueue_Right.GetItemText(positionA,3);
        splitId=this->m_workQueue_Right.GetItemText(positionA,4);
		if(atoi(splitId)==0)
		{
			::SendMessage(hwnd,WM_REFRESH,atoi(preid),13);
		}
		else
		{
			::SendMessage(hwnd,WM_REFRESH,atoi(splitId),14);
		}
	}
	*pResult = 0;
}

void CWorkQueuePage::clean()
{
	m_patient.ShowWindow(SW_HIDE);
	m_sex.ShowWindow(SW_HIDE);
	m_age.ShowWindow(SW_HIDE);
	m_MedList.DeleteAllItems();
	m_patient.SetWindowText("");
	m_sex.SetWindowText("");
	m_age.SetWindowText("");
	m_patient.ShowWindow(SW_SHOW);
	m_sex.ShowWindow(SW_SHOW);
	m_age.ShowWindow(SW_SHOW);
}
