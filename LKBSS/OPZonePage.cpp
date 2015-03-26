// OPZonePage.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "OPZonePage.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_MEDNAME	RGB(255, 255, 0)
#define COLOR_LOCATION	RGB(255, 255, 255)
#define COLOR_NUMBER	RGB(255, 255, 255)

#define YPOS_MEDNAME	30   //药品名称的Y坐标
#define YPOS_MEDUNIT    80   //药品规格的y坐标
#define YPOS_LOCATION	130	 //位置说明的Y坐标

/*
#define YPOS_MED1		20-5
#define YPOS_MED2		135-5
#define YPOS_MED3		245-5
#define YPOS_MED4		355-5
*/

//modify by ljw 20110308   for 药品名字太长，显示不全。
#define YPOS_MED1		10
#define YPOS_MED2		125
#define YPOS_MED3		235
#define YPOS_MED4		345


/////////////////////////////////////////////////////////////////////////////
// COPZonePage dialog


COPZonePage::COPZonePage(CWnd* pParent /*=NULL*/)
	: CDialog(COPZonePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(COPZonePage)
	m_nAddNumber = 0;
	m_strCurQuantity = 0;
	m_strMaxCapacity = 0;
	m_strCurQuantity2 = 0;
	m_strMaxCapacity2 = 0;
	m_strCurQuantity3 = 0;
	m_strMaxCapacity3 = 0;
	m_strCurQuantity4 = 0;
	m_strMaxCapacity4 = 0;
	falg1 = true;
	m_nAddNumber2 = 0;
	m_nAddNumber3 = 0;
	m_nAddNumber4 = 0;
	//}}AFX_DATA_INIT
	this->bizfm = new CBIZFillMedicine();
	cdb = new CDBManage();
}

COPZonePage::~COPZonePage(){

	if (bizfm != NULL)
	{
		delete bizfm;
		//singlebizfm.releaseInst(bizfm);
	}
	if (cdb != NULL)
	{
		delete cdb;
	}
}
void COPZonePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COPZonePage)
	DDX_Control(pDX, IDC_FULLFILL4, m_btnFullFill4);
	DDX_Control(pDX, IDC_FULLFILL3, m_btnFullFill3);
	DDX_Control(pDX, IDC_FULLFILL2, m_btnFullFill2);
	DDX_Control(pDX, IDC_FULLFILL1, m_btnFullFill1);
	DDX_Control(pDX, IDOK4, m_btnOK4);
	DDX_Control(pDX, IDOK3, m_btnOK3);
	DDX_Control(pDX, IDOK2, m_btnOK2);
	DDX_Control(pDX, IDC_Subtract4, m_btnSubtract4);
	DDX_Control(pDX, IDC_Subtract3, m_btnSubtract3);
	DDX_Control(pDX, IDC_Subtract2, m_btnSubtract2);
	DDX_Control(pDX, IDC_Plus4, m_btnPlus4);
	DDX_Control(pDX, IDC_Plus3, m_btnPlus3);
	DDX_Control(pDX, IDC_Plus2, m_btnPlus2);
	DDX_Control(pDX, IDC_EDIT_NUM, m_editAddNum);
	DDX_Control(pDX, IDC_EDIT_NUM2, m_editAddNum2);
	DDX_Control(pDX, IDC_EDIT_NUM3, m_editAddNum3);
	DDX_Control(pDX, IDC_EDIT_NUM4, m_editAddNum4);
	DDX_Control(pDX, IDC_Plus, m_btnPlus);
	DDX_Control(pDX, IDC_Subtract, m_btnSubtract);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDIT_NUM, m_nAddNumber);
	DDV_MinMaxInt(pDX, m_nAddNumber, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT_NUM2, m_nAddNumber2);
	DDV_MinMaxInt(pDX, m_nAddNumber2, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT_NUM3, m_nAddNumber3);
	DDV_MinMaxInt(pDX, m_nAddNumber3, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT_NUM4, m_nAddNumber4);
	DDV_MinMaxInt(pDX, m_nAddNumber4, -10000, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COPZonePage, CDialog)
	//{{AFX_MSG_MAP(COPZonePage)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_Subtract, OnSubtract)
	ON_BN_CLICKED(IDC_Plus, OnPlus)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_NUM, OnSetfocusEditNum)
	ON_BN_CLICKED(IDC_Plus2, OnPlus2)
	ON_BN_CLICKED(IDC_Plus3, OnPlus3)
	ON_BN_CLICKED(IDC_Plus4, OnPlus4)
	ON_BN_CLICKED(IDC_Subtract2, OnSubtract2)
	ON_BN_CLICKED(IDC_Subtract3, OnSubtract3)
	ON_BN_CLICKED(IDC_Subtract4, OnSubtract4)
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_BN_CLICKED(IDOK3, OnOk3)
	ON_BN_CLICKED(IDOK4, OnOk4)
	ON_BN_CLICKED(IDC_FULLFILL1, OnFullfill1)
	ON_BN_CLICKED(IDC_FULLFILL2, OnFullfill2)
	ON_BN_CLICKED(IDC_FULLFILL3, OnFullfill3)
	ON_BN_CLICKED(IDC_FULLFILL4, OnFullfill4)
	ON_EN_SETFOCUS(IDC_EDIT_NUM2, OnSetfocusEditNum2)
	ON_EN_SETFOCUS(IDC_EDIT_NUM3, OnSetfocusEditNum3)
	ON_EN_SETFOCUS(IDC_EDIT_NUM4, OnSetfocusEditNum4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COPZonePage message handlers

BOOL COPZonePage::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	CString strPath = Util::GetDialogResPath() + "\\bg_noraml.bmp";
	Util::FillRectWithBmp(pDC, strPath , rect, TRUE);

	return TRUE;
}

BOOL COPZonePage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitButton();

	m_fontMedName.CreatePointFont(150, "宋体");
	m_fontLocation.CreatePointFont(140, "宋体");
	m_fontNumber.CreatePointFont(360, "Tahoma");

	CString strBmp = Util::GetDialogResPath() + "\\no.bmp";
	m_editAddNum.SetBitmaps(strBmp);
	m_editAddNum.SetFont(&m_fontNumber);
	
	m_editAddNum2.SetBitmaps(strBmp);
	m_editAddNum2.SetFont(&m_fontNumber);
	
	m_editAddNum3.SetBitmaps(strBmp);
	m_editAddNum3.SetFont(&m_fontNumber);
	m_editAddNum4.SetBitmaps(strBmp);
	m_editAddNum4.SetFont(&m_fontNumber);


	CRect rect;
	m_editAddNum.GetWindowRect(&rect);
	rect.SetRect(2, 2, rect.Width() - 4, rect.Height() - 4);	
	m_editAddNum2.GetWindowRect(&rect);
	rect.SetRect(2, 2, rect.Width() - 4, rect.Height() - 4);
	m_editAddNum3.GetWindowRect(&rect);
	rect.SetRect(2, 2, rect.Width() - 4, rect.Height() - 4);
	m_editAddNum4.GetWindowRect(&rect);
	rect.SetRect(2, 2, rect.Width() - 4, rect.Height() - 4);
//	CRect rcEditArea(20,4,125,17);
//	m_editAddNum.SetEditArea(rect);

	return TRUE;  
}

void COPZonePage::InitButton()
{
	//OK按钮
	CString strNormalPath	= Util::GetDialogResPath() + "\\ok_off.bmp";
	CString strDownPath		= Util::GetDialogResPath() + "\\ok_on.bmp";
	CString strDisablePath  = Util::GetDialogResPath() + "\\ok_disable.bmp";
	
	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_disable = (HBITMAP)LoadImage(NULL,strDisablePath,IMAGE_BITMAP,NULL,NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	m_btnOK.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_disable);
	m_btnOK2.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_disable);
	m_btnOK3.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_disable);
	m_btnOK4.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal,hBitmap_disable);
	m_btnOK.EnableWindow(false);
	m_btnOK2.EnableWindow(false);
	m_btnOK3.EnableWindow(false);
	m_btnOK4.EnableWindow(false);

	m_btnFullFill1.EnableWindow(false);
	m_btnFullFill2.EnableWindow(false);
	m_btnFullFill3.EnableWindow(false);
	m_btnFullFill4.EnableWindow(false);

	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_disable = NULL;
	
	//+按钮
	strNormalPath	= Util::GetDialogResPath() + "\\+.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\+_down.bmp";
	
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	m_btnPlus.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	m_btnPlus2.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	m_btnPlus3.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	m_btnPlus4.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);

	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_disable = NULL;

	//-按钮
	strNormalPath	= Util::GetDialogResPath() + "\\-.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\-_down.bmp";
	
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	m_btnSubtract.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	m_btnSubtract2.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	m_btnSubtract3.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	m_btnSubtract4.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
	hBitmap_disable = NULL;
}

void COPZonePage::SetMedicineName(CString strName,int order)
{
	switch(order) {
	case 0:
		m_strMedicineName = strName;
		break;
	case 1:
		m_strMedicineName2 = strName;
		break;
	case 2:
		m_strMedicineName3 = strName;
		break;
	case 3:
		m_strMedicineName4 = strName;
		break;
	default:
		1==1;
	}
	
	Invalidate();
}

void COPZonePage::SetLocation(CString strLoc,int order)
{
	switch(order) {
	case 0:
		m_strLocation = strLoc;
		break;
	case 1:
		m_strLocation2 = strLoc;
		break;
	case 2:
		m_strLocation3 = strLoc;
		break;
	case 3:
		m_strLocation4 = strLoc;
		break;
	default:
		1==1;
	}
	
	Invalidate();
}

void COPZonePage::setCurQuantity(int strCurQuantity,int order)
{
	
	switch(order) {
	case 0:
		m_strCurQuantity = strCurQuantity;
		break;
	case 1:
		m_strCurQuantity2 = strCurQuantity;
		break;
	case 2:
		m_strCurQuantity3 = strCurQuantity;
		break;
	case 3:
		m_strCurQuantity4 = strCurQuantity;
		break;
	default:
		1==1;
	}
	Invalidate();
}

void COPZonePage::setMaxCapacity(int strMaxCapacity,int order)
{
	
	switch(order) {
	case 0:
		m_strMaxCapacity = strMaxCapacity;
		break;
	case 1:
		m_strMaxCapacity2 = strMaxCapacity;
		break;
	case 2:
		m_strMaxCapacity3 = strMaxCapacity;
		break;
	case 3:
		m_strMaxCapacity4 = strMaxCapacity;
		break;
	default:
		1==1;
	}
	Invalidate();
}
void COPZonePage::setMedicineUnit(CString strUnit,int order)
{
	
	switch(order) {
	case 0:
		m_strMedicineUnit = strUnit;
		break;
	case 1:
		m_strMedicineUnit2 = strUnit;
		break;
	case 2:
		m_strMedicineUnit3 = strUnit;
		break;
	case 3:
		m_strMedicineUnit4 = strUnit;
		break;
	default:
		1==1;
	}

	Invalidate();
}
void COPZonePage::setParentHwnd(HWND hwnd)
{
	this->h_parentHwnd = hwnd;
}
void COPZonePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CRect rect;
	GetClientRect(rect);
	
	//绘制药品名称
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);

	CSize size;
	size = dc.GetTextExtent(m_strMedicineName);

	//int xPos = rect.Width()/2 - size.cx/2;
	int xPos = 20;
	dc.TextOut(xPos, YPOS_MED1, m_strMedicineName);

	//绘制药品规格1

	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);
	int ty=25;
	int y12=YPOS_MED1+ty;
	//xPos = rect.Width()/2 - size.cx/2 + 100;
	dc.TextOut(xPos,y12,m_strMedicineUnit);
	size = dc.GetTextExtent(m_strMedicineUnit);
	//绘制位置信息1
	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);
	
	//size = dc.GetTextExtent(m_strLocation);	
	xPos +=30+ size.cx;	
	////test	
	dc.TextOut(xPos, y12, m_strLocation);  


	//绘制药品名称2
dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);

	size = dc.GetTextExtent(m_strMedicineName2);

	//int xPos = rect.Width()/2 - size.cx/2;
	xPos = 20;
	dc.TextOut(xPos, YPOS_MED2, m_strMedicineName2);


	/*
	//绘制药品规格2

	dc.SelectObject(&m_fontMedName);
	//xPos = rect.Width()/2 - size.cx/2 + 100;
	xPos += 20+size.cx;
	dc.TextOut(xPos,YPOS_MED2,m_strMedicineUnit2);
	size = dc.GetTextExtent(m_strMedicineUnit2);
	//绘制位置信息2
	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);
	
	//size = dc.GetTextExtent(m_strLocation);
	
	xPos +=30+ size.cx;
	dc.TextOut(xPos, YPOS_MED2, m_strLocation2);
*/

	//绘制药品规格2

	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);

	y12=YPOS_MED2+ty;
	//xPos = rect.Width()/2 - size.cx/2 + 100;
	dc.TextOut(xPos,y12,m_strMedicineUnit2);
	size = dc.GetTextExtent(m_strMedicineUnit2);
	//绘制位置信息1
	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);
	
	//size = dc.GetTextExtent(m_strLocation);	
	xPos +=30+ size.cx;	
	////test	
	dc.TextOut(xPos, y12, m_strLocation2);  


	//绘制药品名称3
   dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);

	size = dc.GetTextExtent(m_strMedicineName3);

	//int xPos = rect.Width()/2 - size.cx/2;
	xPos = 20;
	dc.TextOut(xPos, YPOS_MED3, m_strMedicineName3);

	/*
	//绘制药品规格3

	dc.SelectObject(&m_fontMedName);
	

	//xPos = rect.Width()/2 - size.cx/2 + 100;
	xPos += 20+size.cx;
	dc.TextOut(xPos,YPOS_MED3,m_strMedicineUnit3);
	size = dc.GetTextExtent(m_strMedicineUnit3);
	//绘制位置信息3
	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);
	
	//size = dc.GetTextExtent(m_strLocation);
	
	xPos +=30+ size.cx;
	dc.TextOut(xPos, YPOS_MED3, m_strLocation3);
*/

		//绘制药品规格3

dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);

	y12=YPOS_MED3+ty;
	//xPos = rect.Width()/2 - size.cx/2 + 100;
	dc.TextOut(xPos,y12,m_strMedicineUnit3);
	size = dc.GetTextExtent(m_strMedicineUnit3);
	//绘制位置信息3
	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);
	
	//size = dc.GetTextExtent(m_strLocation);	
	xPos +=30+ size.cx;	
	////test	
	dc.TextOut(xPos, y12, m_strLocation3);  







	//绘制药品名称4
dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);

	size = dc.GetTextExtent(m_strMedicineName4);

	//int xPos = rect.Width()/2 - size.cx/2;
	xPos = 20;
	dc.TextOut(xPos, YPOS_MED4, m_strMedicineName4);

	/*
	//绘制药品规格4

	dc.SelectObject(&m_fontMedName);
	

	//xPos = rect.Width()/2 - size.cx/2 + 100;
	xPos += 20+size.cx;
	dc.TextOut(xPos,YPOS_MED4,m_strMedicineUnit4);
	size = dc.GetTextExtent(m_strMedicineUnit4);
	//绘制位置信息4
	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);
	
	//size = dc.GetTextExtent(m_strLocation);
	
	xPos +=30+ size.cx;
	dc.TextOut(xPos, YPOS_MED4, m_strLocation4);
	*/

	//绘制药品规格4

dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);

	y12=YPOS_MED4+ty;
	//xPos = rect.Width()/2 - size.cx/2 + 100;
	dc.TextOut(xPos,y12,m_strMedicineUnit4);
	size = dc.GetTextExtent(m_strMedicineUnit4);
	//绘制位置信息4
	dc.SetTextColor(COLOR_LOCATION);
	dc.SelectObject(&m_fontLocation);
	
	//size = dc.GetTextExtent(m_strLocation);	
	xPos +=30+ size.cx;	
	////test	
	dc.TextOut(xPos, y12, m_strLocation4);  

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL COPZonePage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//屏蔽回车，退出按钮
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))    
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

void COPZonePage::OnSubtract() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber <= (0-m_strCurQuantity))
	{
		m_nAddNumber = (0-m_strCurQuantity);
		UpdateData(FALSE);

	}else{
		m_nAddNumber--;
		UpdateData(FALSE);
	}
	
}

void COPZonePage::OnPlus() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber >= m_strMaxCapacity-m_strCurQuantity)
	{
		m_nAddNumber = m_strMaxCapacity - m_strCurQuantity;
		UpdateData(FALSE);
	}else{
		m_nAddNumber++;
		UpdateData(FALSE);
	}
	
}

HBRUSH COPZonePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH   hbr   =   CDialog::OnCtlColor(pDC,   pWnd,   nCtlColor);   
	
	if(pWnd->GetDlgCtrlID() == IDC_EDIT_NUM || pWnd->GetDlgCtrlID() == IDC_EDIT_NUM2 || pWnd->GetDlgCtrlID() == IDC_EDIT_NUM3 || pWnd->GetDlgCtrlID() == IDC_EDIT_NUM4 )   
	{   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(COLOR_NUMBER);

		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}   
	
	return   hbr;   
}

void COPZonePage::displayLocationDetail(int locationId ,int order)
{
	LocDisplay loc = this->bizfm->getLocationDetail(locationId,cdb);

	CString locInfo = "";

	int n = loc.locationId;
	if (n == 0)
	{
		locInfo = "当前加药通道关闭";
	}else{
		locInfo =locInfo+ "第"+Utils::itos(1+n/320).c_str()+"单元 ";
		locInfo =locInfo+ Utils::itos(n%320/maxNumberPerUnit+1).c_str()+"层 ";
		locInfo =locInfo+ Utils::itos((n-1)%32+1).c_str()+"号槽  ";
		locInfo =locInfo+ Utils::itos(loc.curCapacity).c_str()+" / ";
		locInfo =locInfo+ Utils::itos(loc.maxCapacity).c_str();
	}
	
	SetMedicineName(loc.medicineName.c_str(),order);
	SetLocation(locInfo,order);
	setMedicineUnit(loc.medcineUnit.c_str(),order);
	setCurQuantity(loc.curCapacity,order);
	setMaxCapacity(loc.maxCapacity,order);
	
}

void COPZonePage::OnSetfocusEditNum() 
{
	// TODO: Add your control notification handler code here
	osk.setCWnd(this);
	osk.setIdd(IDC_EDIT_NUM);
	osk.setNextIdd(IDOK);
	osk.open();
}


void COPZonePage::OnPlus2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber2 >= m_strMaxCapacity2-m_strCurQuantity2)
	{
		m_nAddNumber2 = m_strMaxCapacity2 - m_strCurQuantity2;
		UpdateData(FALSE);
	}else{
		m_nAddNumber2++;
		UpdateData(FALSE);
	}
}

void COPZonePage::OnPlus3() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber3 >= m_strMaxCapacity3-m_strCurQuantity3)
	{
		m_nAddNumber3 = m_strMaxCapacity3 - m_strCurQuantity3;
		UpdateData(FALSE);
	}else{
		m_nAddNumber3++;
		UpdateData(FALSE);
	}
}

void COPZonePage::OnPlus4() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber4 >= m_strMaxCapacity4-m_strCurQuantity4)
	{
		m_nAddNumber4 = m_strMaxCapacity4 - m_strCurQuantity4;
		UpdateData(FALSE);
	}else{
		m_nAddNumber4++;
		UpdateData(FALSE);
	}
}

void COPZonePage::OnSubtract2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber2 <= (0-m_strCurQuantity2))
	{
		m_nAddNumber2 = (0-m_strCurQuantity2);
		UpdateData(FALSE);

	}else{
		m_nAddNumber2--;
		UpdateData(FALSE);
	}
}

void COPZonePage::OnSubtract3() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber3 <= (0-m_strCurQuantity3))
	{
		m_nAddNumber3 = (0-m_strCurQuantity3);
		UpdateData(FALSE);

	}else{
		m_nAddNumber3--;
		UpdateData(FALSE);
	}
}

void COPZonePage::OnSubtract4() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber4 <= (0-m_strCurQuantity4))
	{
		m_nAddNumber4 = (0-m_strCurQuantity4);
		UpdateData(FALSE);

	}else{
		m_nAddNumber4--;
		UpdateData(FALSE);
	}
}

void COPZonePage::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if (m_nAddNumber < (0-m_strCurQuantity))
	{
		m_nAddNumber = (0-m_strCurQuantity);
		UpdateData(FALSE);
	}else if (m_nAddNumber > m_strMaxCapacity-m_strCurQuantity)
	{
		m_nAddNumber = m_strMaxCapacity - m_strCurQuantity;
		UpdateData(FALSE);
	}

	CString strText;
	int errorCode;
	
	strText.LoadString(IDS_FinishFilling);

	strText.Replace("location",m_strLocation);

	CString sAddNumber;
//	::_itoa(m_nAddNumber,sAddNumber,10);
	sAddNumber = Utils::itos(m_nAddNumber).c_str();
	strText.Replace("num",sAddNumber);

	 
//	strText.Replace("beyondFill",Utils::itos(m_strCurQuantity).c_str());
	strText.Replace("afterFill",Utils::itos(m_nAddNumber+m_strCurQuantity).c_str());
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"添加药品确认")==IDOK)
	{
		errorCode = bizfm->finishFilling(m_nAddNumber);
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			strText="\r\n\r\n"+strText;
			Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			return;
		}
		
		int nextLocationId = bizfm->getVfs()[0].locationId;
		this->displayLocationDetail(nextLocationId);

		if (nextLocationId != 0)
		{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,0,1);
			errorCode = bizfm->fillMedicine();
			if (errorCode != 0)
			{
				CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
				strText="\r\n\r\n"+strText;
				Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			}
		}else{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,0,0);
		}				
	}
//	CDialog::OnOK();
}

void COPZonePage::OnOk2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber2 < (0-m_strCurQuantity2))
	{
		m_nAddNumber2 = (0-m_strCurQuantity2);
		UpdateData(FALSE);
	}else if (m_nAddNumber2 > m_strMaxCapacity2-m_strCurQuantity2)
	{
		m_nAddNumber2 = m_strMaxCapacity2 - m_strCurQuantity2;
		UpdateData(FALSE);
	}

	CString strText;
	int errorCode;
	
	strText.LoadString(IDS_FinishFilling);

	strText.Replace("location",m_strLocation2);

	CString sAddNumber;

	sAddNumber = Utils::itos(m_nAddNumber2).c_str();
	strText.Replace("num",sAddNumber);

	strText.Replace("afterFill",Utils::itos(m_nAddNumber2+m_strCurQuantity2).c_str());
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"添加药品确认")==IDOK)
	{
		errorCode = bizfm->finishFilling(m_nAddNumber2,1);
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			strText="\r\n\r\n"+strText;
			Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			return;
		}

		int nextLocationId = bizfm->getVfs()[1].locationId;
		this->displayLocationDetail(nextLocationId,1);
		
		if (nextLocationId != 0)
		{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,1,1);
			errorCode = bizfm->fillMedicine(1);
			if (errorCode != 0)
			{
				CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
				strText="\r\n\r\n"+strText;
				Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			}
		}else{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,1,0);
		}		
	}
}

void COPZonePage::OnOk3() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber3 < (0-m_strCurQuantity3))
	{
		m_nAddNumber3 = (0-m_strCurQuantity3);
		UpdateData(FALSE);
	}else if (m_nAddNumber3 > m_strMaxCapacity3-m_strCurQuantity3)
	{
		m_nAddNumber3 = m_strMaxCapacity3 - m_strCurQuantity3;
		UpdateData(FALSE);
	}

	CString strText;
	int errorCode;
	
	strText.LoadString(IDS_FinishFilling);

	strText.Replace("location",m_strLocation3);

	CString sAddNumber;

	sAddNumber = Utils::itos(m_nAddNumber3).c_str();
	strText.Replace("num",sAddNumber);

	strText.Replace("afterFill",Utils::itos(m_nAddNumber3+m_strCurQuantity3).c_str());
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"添加药品确认")==IDOK)
	{
		errorCode = bizfm->finishFilling(m_nAddNumber3,2);
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			strText="\r\n\r\n"+strText;
			Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			return;
		}

		int nextLocationId = bizfm->getVfs()[2].locationId;
		this->displayLocationDetail(nextLocationId,2);
		
		if (nextLocationId != 0)
		{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,2,1);
			errorCode = bizfm->fillMedicine(2);
			if (errorCode != 0)
			{
				CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
				strText="\r\n\r\n"+strText;
				Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			}
		}else{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,2,0);
		}			
	}
}

void COPZonePage::OnOk4() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_nAddNumber4 < (0-m_strCurQuantity4))
	{
		m_nAddNumber4 = (0-m_strCurQuantity4);
		UpdateData(FALSE);
	}else if (m_nAddNumber4 > m_strMaxCapacity4-m_strCurQuantity4)
	{
		m_nAddNumber4 = m_strMaxCapacity4 - m_strCurQuantity4;
		UpdateData(FALSE);
	}

	CString strText;
	int errorCode;
	
	strText.LoadString(IDS_FinishFilling);

	strText.Replace("location",m_strLocation4);

	CString sAddNumber;

	sAddNumber = Utils::itos(m_nAddNumber4).c_str();
	strText.Replace("num",sAddNumber);

	strText.Replace("afterFill",Utils::itos(m_nAddNumber4+m_strCurQuantity4).c_str());
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"添加药品确认")==IDOK)
	{
		errorCode = bizfm->finishFilling(m_nAddNumber4,3);
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			strText="\r\n\r\n"+strText;
			Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			return;
		}

		int nextLocationId = bizfm->getVfs()[3].locationId;
		this->displayLocationDetail(nextLocationId,3);
		
		if (nextLocationId != 0)
		{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,3,1);
			errorCode = bizfm->fillMedicine(3);
			if (errorCode != 0)
			{
				CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
				strText="\r\n\r\n"+strText;
				Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			}
		}else{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,3,0);
		}				
	}
}

void COPZonePage::OnFullfill1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nAddNumber = m_strMaxCapacity - m_strCurQuantity;
		UpdateData(FALSE);
	CString strText;
	int errorCode;
	
	strText.LoadString(IDS_FinishFilling);

	strText.Replace("location",m_strLocation);

	CString sAddNumber;
//	::_itoa(m_nAddNumber,sAddNumber,10);
	sAddNumber = Utils::itos(m_nAddNumber).c_str();
	strText.Replace("num",sAddNumber);

	 
//	strText.Replace("beyondFill",Utils::itos(m_strCurQuantity).c_str());
	strText.Replace("afterFill",Utils::itos(m_nAddNumber+m_strCurQuantity).c_str());
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"添加药品确认")==IDOK)
	{
		errorCode = bizfm->finishFilling(m_nAddNumber);
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			strText="\r\n\r\n"+strText;
			Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			return;
		}
		
		int nextLocationId = bizfm->getVfs()[0].locationId;
		this->displayLocationDetail(nextLocationId);

		if (nextLocationId != 0)
		{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,0,1);
			errorCode = bizfm->fillMedicine();
			if (errorCode != 0)
			{
				CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
				strText="\r\n\r\n"+strText;
				Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			}
		}else{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,0,0);
		}				
	}
}

void COPZonePage::OnFullfill2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nAddNumber2 = m_strMaxCapacity2 - m_strCurQuantity2;
		UpdateData(FALSE);

	CString strText;
	int errorCode;
	
	strText.LoadString(IDS_FinishFilling);

	strText.Replace("location",m_strLocation2);

	CString sAddNumber;

	sAddNumber = Utils::itos(m_nAddNumber2).c_str();
	strText.Replace("num",sAddNumber);

	strText.Replace("afterFill",Utils::itos(m_nAddNumber2+m_strCurQuantity2).c_str());
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"添加药品确认")==IDOK)
	{
		errorCode = bizfm->finishFilling(m_nAddNumber2,1);
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			strText="\r\n\r\n"+strText;
			Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			return;
		}

		int nextLocationId = bizfm->getVfs()[1].locationId;
		this->displayLocationDetail(nextLocationId,1);
		
		if (nextLocationId != 0)
		{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,1,1);
			errorCode = bizfm->fillMedicine(1);
			if (errorCode != 0)
			{
				CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
				strText="\r\n\r\n"+strText;
				Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			}
		}else{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,1,0);
		}		
	}
}

void COPZonePage::OnFullfill3() 
{
	// TODO: Add your control notification handler code here
		UpdateData();
		m_nAddNumber3 = m_strMaxCapacity3 - m_strCurQuantity3;
		UpdateData(FALSE);
	CString strText;
	int errorCode;
	
	strText.LoadString(IDS_FinishFilling);

	strText.Replace("location",m_strLocation3);

	CString sAddNumber;

	sAddNumber = Utils::itos(m_nAddNumber3).c_str();
	strText.Replace("num",sAddNumber);

	strText.Replace("afterFill",Utils::itos(m_nAddNumber3+m_strCurQuantity3).c_str());
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"添加药品确认")==IDOK)
	{
		errorCode = bizfm->finishFilling(m_nAddNumber3,2);
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			strText="\r\n\r\n"+strText;
			Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			return;
		}

		int nextLocationId = bizfm->getVfs()[2].locationId;
		this->displayLocationDetail(nextLocationId,2);
		
		if (nextLocationId != 0)
		{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,2,1);
			errorCode = bizfm->fillMedicine(2);
			if (errorCode != 0)
			{
				CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
				strText="\r\n\r\n"+strText;
				Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			}
		}else{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,2,0);
		}			
	}
}

void COPZonePage::OnFullfill4() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UpdateData();
	m_nAddNumber4 = m_strMaxCapacity4 - m_strCurQuantity4;
	UpdateData(FALSE);
	
	CString strText;
	int errorCode;
	
	strText.LoadString(IDS_FinishFilling);

	strText.Replace("location",m_strLocation4);

	CString sAddNumber;

	sAddNumber = Utils::itos(m_nAddNumber4).c_str();
	strText.Replace("num",sAddNumber);

	strText.Replace("afterFill",Utils::itos(m_nAddNumber4+m_strCurQuantity4).c_str());
	if(Util::MessageBox(this->GetSafeHwnd(), strText,"添加药品确认")==IDOK)
	{
		errorCode = bizfm->finishFilling(m_nAddNumber4,3);
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			strText="\r\n\r\n"+strText;
			Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			return;
		}

		int nextLocationId = bizfm->getVfs()[3].locationId;
		this->displayLocationDetail(nextLocationId,3);
		
		if (nextLocationId != 0)
		{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,3,1);
			errorCode = bizfm->fillMedicine(3);
			if (errorCode != 0)
			{
				CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
				strText="\r\n\r\n"+strText;
				Util::MessageBox(this->GetSafeHwnd(), strText,"",RCMB_OK);
			}
		}else{
			::PostMessage(h_parentHwnd,WM_FINISH_ADD,3,0);
		}				
	}
}

void COPZonePage::OnSetfocusEditNum2() 
{
	// TODO: Add your control notification handler code here
	osk.setCWnd(this);
	osk.setIdd(IDC_EDIT_NUM2);
	osk.setNextIdd(IDOK2);
	osk.open();
}

void COPZonePage::OnSetfocusEditNum3() 
{
	// TODO: Add your control notification handler code here
	osk.setCWnd(this);
	osk.setIdd(IDC_EDIT_NUM3);
	osk.setNextIdd(IDOK3);
	osk.open();
}

void COPZonePage::OnSetfocusEditNum4() 
{
	// TODO: Add your control notification handler code here
	osk.setCWnd(this);
	osk.setIdd(IDC_EDIT_NUM4);
	osk.setNextIdd(IDOK4);
	osk.open();
}
