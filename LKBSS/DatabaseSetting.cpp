// DatabaseSetting.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "DatabaseSetting.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDatabaseSetting dialog


CDatabaseSetting::CDatabaseSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDatabaseSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDatabaseSetting)
	m_port = _T("");
	m_psw = _T("");
	m_server = _T("");
	m_user = _T("");
	m_database = _T("");
	m_characterSet = _T("");
	//}}AFX_DATA_INIT
	m_characterSetChanged = FALSE;
    enp = NULL;
    enp = new Encrypt();
	InitData();
}

CDatabaseSetting::~CDatabaseSetting()
{
    if (enp != NULL)
    {
        delete enp;
        enp = NULL;
    }
}

void CDatabaseSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDatabaseSetting)
    DDX_Control(pDX, IDC_grp_caption, m_GBCaption);
	DDX_Control(pDX, IDC_EDT_user, m_edtUser);
	DDX_Control(pDX, IDC_EDT_server, m_edtServer);
	DDX_Control(pDX, IDC_EDT_psw, m_edtPsw);
	DDX_Control(pDX, IDC_EDT_port, m_edtPort);
	DDX_Control(pDX, IDC_EDT_database, m_edtDatabase);
	DDX_Text(pDX, IDC_EDT_port, m_port);
	DDX_Text(pDX, IDC_EDT_psw, m_psw);
	DDX_Text(pDX, IDC_EDT_server, m_server);
	DDX_Text(pDX, IDC_EDT_user, m_user);
	DDX_Text(pDX, IDC_EDT_database, m_database);
	DDX_CBString(pDX, IDC_CMB_characterSet, m_characterSet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDatabaseSetting, CDialog)
	//{{AFX_MSG_MAP(CDatabaseSetting)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_CBN_EDITCHANGE(IDC_COMBO1, OnEditchangeCombo1)
	ON_CBN_EDITCHANGE(IDC_CMB_characterSet, OnEditchangeCMBcharacterSet)
	ON_CBN_CLOSEUP(IDC_CMB_characterSet, OnCloseupCMBcharacterSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatabaseSetting message handlers

BOOL CDatabaseSetting::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_GBCaption.SetXPGroupStyle(CXPGroupBox::XPGB_FRAME);
	m_GBCaption.SetBackgroundColor(RGB(2, 125, 204));
	m_GBCaption.SetCatptionTextColor(RGB(255, 255, 255));
	m_GBCaption.SetFontBold(TRUE);
	m_GBCaption.SetAlignment(SS_LEFT);  
    return TRUE;
}

BOOL CDatabaseSetting::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CString strLogoPath = Util::GetDialogResPath() + "\\bg_noraml.bmp";
	Util::FillRectWithBmp(pDC, strLogoPath , rect);
	
	return TRUE;
}

HBRUSH CDatabaseSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDatabaseSetting::InitData()
{	
	m_server = configUtil.getValue("host").c_str();
	m_user = configUtil.getValue("user").c_str();
	m_psw = configUtil.getValue("password").c_str();
	m_database = configUtil.getValue("db").c_str();
	m_port = configUtil.getValue("port").c_str();
    m_characterSet = configUtil.getValue("characterSet").c_str();    

	/*m_server = enp->decrypt(desKey, configUtil.getValue("host").c_str());
	m_user = enp->decrypt(desKey, configUtil.getValue("user").c_str());
	m_psw = enp->decrypt(desKey, configUtil.getValue("password").c_str());
	m_database = enp->decrypt(desKey, configUtil.getValue("db").c_str());
	m_port = (enp->decrypt(desKey, configUtil.getValue("port").c_str()));
    m_characterSet = enp->decrypt(desKey, configUtil.getValue("characterSet").c_str()); */
}

void CDatabaseSetting::OnEditchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDatabaseSetting::OnEditchangeCMBcharacterSet() 
{
	// TODO: Add your control notification handler code here
	m_characterSetChanged = TRUE;
}

void CDatabaseSetting::OnCloseupCMBcharacterSet() 
{
	// TODO: Add your control notification handler code here
	m_characterSetChanged = TRUE;
}
