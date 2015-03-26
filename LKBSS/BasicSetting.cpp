// BasicSetting.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "BasicSetting.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicSetting dialog

CBasicSetting::CBasicSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CBasicSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBasicSetting)
	m_log = _T("");
	m_dealFrequency = _T("");
	m_bMedValidateCtrl = FALSE;
	//}}AFX_DATA_INIT
	InitData();
}

void CBasicSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicSetting)
	DDX_Control(pDX, IDC_EDT_log, m_edtLog);
	DDX_Control(pDX, IDC_EDT_dealFrequency, m_edtDealFrequency);
	DDX_Text(pDX, IDC_EDT_log, m_log);
	DDX_Text(pDX, IDC_EDT_dealFrequency, m_dealFrequency);
    DDX_Control(pDX, IDC_grp_caption, m_GBCaption);
	DDX_Check(pDX, IDC_CHK_medValidate, m_bMedValidateCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBasicSetting, CDialog)
	//{{AFX_MSG_MAP(CBasicSetting)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicSetting message handlers

BOOL CBasicSetting::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_GBCaption.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GBCaption.SetBackgroundColor(RGB(2, 125, 204));
	m_GBCaption.SetCatptionTextColor(RGB(255, 255, 255));
	m_GBCaption.SetFontBold(TRUE);
	m_GBCaption.SetAlignment(SS_LEFT);  
    return TRUE;
}

BOOL CBasicSetting::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CString strLogoPath = Util::GetDialogResPath() + "\\bg_noraml.bmp";
	Util::FillRectWithBmp(pDC, strLogoPath , rect);

	return TRUE;
}

HBRUSH CBasicSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CBasicSetting::InitData()
{
	m_dealFrequency = configUtil.getValue("DealFrequency").c_str();
	m_log = configUtil.getValue("logPropFile").c_str();
    m_bMedValidateCtrl = configUtil.getInt("ValidateControl");
}
