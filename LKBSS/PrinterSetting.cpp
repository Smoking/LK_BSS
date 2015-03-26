// PrinterSetting.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "PrinterSetting.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrinterSetting dialog


CPrinterSetting::CPrinterSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CPrinterSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrinterSetting)
	m_satoPrtBaud = _T("");
	m_satoPrtPort = _T("");
	m_thermalPrtBaud = _T("");
	m_thermalPrtPaper = _T("");
	m_thermalPrtPort = _T("");
	//}}AFX_DATA_INIT
	InitData();
}


void CPrinterSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrinterSetting)
	DDX_Control(pDX, IDC_grp_caption2, m_GBCaption2);
	DDX_Control(pDX, IDC_EDT_ThermalPrtPort, m_edtThermalPrtPort);
	DDX_Control(pDX, IDC_EDT_ThermalPrtPaper, m_edtThermalPrtPaper);
	DDX_Control(pDX, IDC_EDT_ThermalPrtBaud, m_edtThermalPrtBaud);
	DDX_Control(pDX, IDC_EDT_SatoPrtPort, m_edtSatoPrtPort);
	DDX_Control(pDX, IDC_EDT_SatoPrtBaud, m_edtSatoPrtBaud);
	DDX_Text(pDX, IDC_EDT_SatoPrtBaud, m_satoPrtBaud);
	DDX_Text(pDX, IDC_EDT_SatoPrtPort, m_satoPrtPort);
	DDX_Text(pDX, IDC_EDT_ThermalPrtBaud, m_thermalPrtBaud);
	DDX_Text(pDX, IDC_EDT_ThermalPrtPaper, m_thermalPrtPaper);
	DDX_Text(pDX, IDC_EDT_ThermalPrtPort, m_thermalPrtPort);
    DDX_Control(pDX, IDC_grp_caption, m_GBCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrinterSetting, CDialog)
	//{{AFX_MSG_MAP(CPrinterSetting)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrinterSetting message handlers

BOOL CPrinterSetting::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_GBCaption.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GBCaption.SetBackgroundColor(RGB(2, 125, 204));
	m_GBCaption.SetCatptionTextColor(RGB(255, 255, 255));
	m_GBCaption.SetFontBold(TRUE);
	m_GBCaption.SetAlignment(SS_LEFT);  

    m_GBCaption2.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GBCaption2.SetBackgroundColor(RGB(2, 100, 173));   //(2, 125, 234));   .86 151.200
	m_GBCaption2.SetCatptionTextColor(RGB(255, 255, 255));
	m_GBCaption2.SetFontBold(TRUE);
	m_GBCaption2.SetAlignment(SS_LEFT);

    return TRUE;
}

BOOL CPrinterSetting::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CString strLogoPath = Util::GetDialogResPath() + "\\bg_noraml.bmp";
	Util::FillRectWithBmp(pDC, strLogoPath , rect);
	
	return TRUE;
}

HBRUSH CPrinterSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CPrinterSetting::InitData()
{
	m_satoPrtBaud = configUtil.getValue("SatobaudRate").c_str();
	m_satoPrtPort = configUtil.getValue("SatoPrinterPort").c_str();
	m_thermalPrtBaud = configUtil.getValue("baudRate").c_str();
	m_thermalPrtPaper = configUtil.getValue("rowWidth").c_str();
	m_thermalPrtPort = configUtil.getValue("printerPort").c_str();
}
