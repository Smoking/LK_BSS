// ResetDefaultSetting.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "ResetDefaultSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Util.h"

/////////////////////////////////////////////////////////////////////////////
// CResetDefaultSetting dialog


CResetDefaultSetting::CResetDefaultSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CResetDefaultSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResetDefaultSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CResetDefaultSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResetDefaultSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, ID_save, m_btnSave);
    DDX_Control(pDX, IDC_grp_caption, m_GBCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResetDefaultSetting, CDialog)
	//{{AFX_MSG_MAP(CResetDefaultSetting)
    ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_save, Onsave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResetDefaultSetting message handlers
BOOL CResetDefaultSetting::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CString strLogoPath = Util::GetDialogResPath() + "\\bg_noraml.bmp";
	Util::FillRectWithBmp(pDC, strLogoPath , rect);

	return TRUE;
}

HBRUSH CResetDefaultSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CResetDefaultSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    
    m_GBCaption.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GBCaption.SetBackgroundColor(RGB(2, 125, 204));
	m_GBCaption.SetCatptionTextColor(RGB(255, 255, 255));
	m_GBCaption.SetFontBold(TRUE);
	m_GBCaption.SetAlignment(SS_LEFT); 

    string defaultPath = CSysConfig::getAppPath();
    defaultFile = defaultPath + "\\default";
    configFile = defaultPath + "\\LKBSSConfig.xml";
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResetDefaultSetting::OnOK() 
{
	// TODO: Add extra validation here
	 //MessageBox("ok");
    CString str;
    str.LoadString(IDS_resetConfirm);
    if(Util::MessageBox(this->GetSafeHwnd(), str, "提示") == IDCANCEL)
    {
        return;
    }
    if(CopyFile(defaultFile.c_str(), configFile.c_str(),FALSE))
    {
        str.LoadString(IDS_configEnd);
        Util::MessageBox(this->GetSafeHwnd(), str, "提示", RCMB_OK);
    }

	//CDialog::OnOK();
}


void CResetDefaultSetting::Onsave() 
{
	// TODO: Add your control notification handler code here
    CString str;
    str.LoadString(IDS_saveConfirm);
    if(Util::MessageBox(this->GetSafeHwnd(), str, "提示") == IDCANCEL)
    {
        return;
    }
    if(CopyFile(configFile.c_str(), defaultFile.c_str(),FALSE))
    {
        str.LoadString(IDS_SUCC);
        Util::MessageBox(this->GetSafeHwnd(), str, "提示", RCMB_OK); return;
    }

   
}
