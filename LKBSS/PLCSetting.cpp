// PLCSetting.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "PLCSetting.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPLCSetting dialog


CPLCSetting::CPLCSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CPLCSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLCSetting)
	m_conTimeOut = _T("");
	m_IP = _T("");
	m_stationNum = _T("");
	//}}AFX_DATA_INIT
	InitData();
}


void CPLCSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLCSetting)
    DDX_Control(pDX, IDC_grp_caption, m_GBCaption);
	DDX_Control(pDX, IDC_EDT_stationNum, m_edtStationNum);
	DDX_Control(pDX, IDC_EDT_conTimeOut, m_edtTimeOut);
	DDX_Control(pDX, IDC_EDT_IP, m_edtIP);
	DDX_Text(pDX, IDC_EDT_conTimeOut, m_conTimeOut);
	DDX_Text(pDX, IDC_EDT_IP, m_IP);
	DDX_Text(pDX, IDC_EDT_stationNum, m_stationNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPLCSetting, CDialog)
	//{{AFX_MSG_MAP(CPLCSetting)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLCSetting message handlers

BOOL CPLCSetting::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_GBCaption.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GBCaption.SetBackgroundColor(RGB(2, 125, 204));
	m_GBCaption.SetCatptionTextColor(RGB(255, 255, 255));
	m_GBCaption.SetFontBold(TRUE);
	m_GBCaption.SetAlignment(SS_LEFT);  
    return TRUE;
}

BOOL CPLCSetting::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CString strLogoPath = Util::GetDialogResPath() + "\\bg_noraml.bmp";
	Util::FillRectWithBmp(pDC, strLogoPath , rect);
	
	return TRUE;
}

HBRUSH CPLCSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC)  //针对所有的标签
	{
		pDC->SetBkMode( TRANSPARENT );
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPLCSetting::InitData()
{
	m_IP = configUtil.getValue("plcIP").c_str();
	m_stationNum = configUtil.getValue("stationNumber").c_str();
	m_conTimeOut = configUtil.getValue("connectTimeOut").c_str();  //连接超时ms
}
