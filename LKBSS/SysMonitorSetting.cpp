// SysMonitorSetting.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "SysMonitorSetting.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysMonitorSetting dialog


CSysMonitorSetting::CSysMonitorSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSysMonitorSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysMonitorSetting)
	m_boxFrequency = _T("");
	m_deviceFrequency = _T("");
	//}}AFX_DATA_INIT
	InitData();
}


void CSysMonitorSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysMonitorSetting)
    DDX_Control(pDX, IDC_grp_caption, m_GBCaption);
	DDX_Control(pDX, IDC_EDT_DeviceFrequency, m_edtDeviceFrequency);
	DDX_Control(pDX, IDC_EDT_BoxFrequency, m_edtBoxFrequency);
	DDX_Text(pDX, IDC_EDT_BoxFrequency, m_boxFrequency);
	DDX_Text(pDX, IDC_EDT_DeviceFrequency, m_deviceFrequency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysMonitorSetting, CDialog)
	//{{AFX_MSG_MAP(CSysMonitorSetting)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysMonitorSetting message handlers

BOOL CSysMonitorSetting::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_GBCaption.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GBCaption.SetBackgroundColor(RGB(2, 125, 204));
	m_GBCaption.SetCatptionTextColor(RGB(255, 255, 255));
	m_GBCaption.SetFontBold(TRUE);
	m_GBCaption.SetAlignment(SS_LEFT);  
    return TRUE;
}

BOOL CSysMonitorSetting::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CString strLogoPath = Util::GetDialogResPath() + "\\bg_noraml.bmp";
	Util::FillRectWithBmp(pDC, strLogoPath , rect);
	
	return TRUE;
}

HBRUSH CSysMonitorSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CSysMonitorSetting::InitData()
{
	m_boxFrequency = configUtil.getValue("DoseMonitorFrequency").c_str();
	m_deviceFrequency = configUtil.getValue("DeviceMonitorFrequency").c_str();
}
