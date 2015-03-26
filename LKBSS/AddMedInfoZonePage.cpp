// AddMedInfoZonePage.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "AddMedInfoZonePage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddMedInfoZonePage dialog


CAddMedInfoZonePage::CAddMedInfoZonePage(CWnd* pParent /*=NULL*/)
	: CDialog(CAddMedInfoZonePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddMedInfoZonePage)
	//}}AFX_DATA_INIT
}


void CAddMedInfoZonePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddMedInfoZonePage)
	DDX_Control(pDX, IDC_AddMedList, m_listAddMed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddMedInfoZonePage, CDialog)
	//{{AFX_MSG_MAP(CAddMedInfoZonePage)
	ON_WM_ERASEBKGND()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddMedInfoZonePage message handlers

BOOL CAddMedInfoZonePage::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(rect);
	
	CBrush brush(INFOZONE_BK_COLOR);
	pDC->FillRect(rect, &brush);
	return TRUE;
}

BOOL CAddMedInfoZonePage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//屏蔽回车，退出按钮
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))    
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CAddMedInfoZonePage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitAddMedList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddMedInfoZonePage::InitAddMedList()
{
	m_listAddMed.Init();
	m_listAddMed.SetTextColor(RGB(125,125,125));	
	m_listAddMed.InsertColumn(0, "地址编号", LVCFMT_LEFT, 60);
	m_listAddMed.InsertColumn(1, "药品名称", LVCFMT_LEFT, 80);
	m_listAddMed.InsertColumn(2, "剩余药品", LVCFMT_LEFT, 60);
	m_listAddMed.InsertColumn(3, "最大药量", LVCFMT_LEFT, 60);
	m_listAddMed.InsertColumn(4, "状态", LVCFMT_LEFT, 60);
	
	m_listAddMed.SetRedraw(FALSE);
	m_listAddMed.SetRedraw(TRUE);
	ListView_SetExtendedListViewStyle(m_listAddMed.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
	PostMessage(WM_REFRESH);
}

