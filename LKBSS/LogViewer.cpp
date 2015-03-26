// LogViewer.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "LogViewer.h"
#include "Util.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogViewer dialog


CLogViewer::CLogViewer(CWnd* pParent /*=NULL*/)
	: CRCDialog(CLogViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogViewer)
	//}}AFX_DATA_INIT
}


void CLogViewer::DoDataExchange(CDataExchange* pDX)
{
	CRCDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogViewer)
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datatime);
	DDX_Control(pDX, IDB_ViewLog, m_viewLog);
	DDX_Control(pDX, IDC_listLogRecord, m_listLogRecord);
	DDX_Control(pDX, IDOK, m_btnClose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogViewer, CRCDialog)
	//{{AFX_MSG_MAP(CLogViewer)
	ON_BN_CLICKED(IDB_ViewLog, OnViewLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogViewer message handlers

void CLogViewer::OnOK() 
{
	// TODO: Add extra validation here
	
	CRCDialog::OnOK();
}

BOOL CLogViewer::OnInitDialog() 
{
	CRCDialog::OnInitDialog();
	this->SetWindowText("�������ܺм����ٷ�ҩϵͳ-----��ѯ��־");
	// TODO: Add extra initialization here
	CString strNormalPath	= Util::GetDialogResPath() + "\\back_normal.bmp";
	CString strDownPath		= Util::GetDialogResPath() + "\\back_down.bmp";

	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	m_btnClose.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);

	m_listLogRecord.Init();
	m_listLogRecord.SetTextColor(RGB(125,125,125));
	m_listLogRecord.InsertColumn(0, "������", LVCFMT_LEFT, 65);
	m_listLogRecord.InsertColumn(1, "�û�ID", LVCFMT_LEFT, 80);
	m_listLogRecord.InsertColumn(2, "�û���", LVCFMT_LEFT, 80);
	m_listLogRecord.InsertColumn(3, "�����¼�", LVCFMT_LEFT, 120);
	m_listLogRecord.InsertColumn(4, "�������", LVCFMT_LEFT, 280);
	m_listLogRecord.InsertColumn(5, "����ʱ��", LVCFMT_LEFT, 180);
	m_listLogRecord.SetRedraw(FALSE);
	m_listLogRecord.SetRedraw(TRUE);
	DWORD dwStyle = m_listLogRecord.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    DWORD d=m_listLogRecord.SetExtendedStyle(dwStyle); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogViewer::OnViewLog() 
{
	// TODO: Add your control notification handler code here
	m_listLogRecord.DeleteAllItems();
	list<SystemlogInfo>inputLog;
	CTime curTime ;
	m_datatime.GetTime(curTime);	
	string time_log=curTime.Format("%Y-%m-%d");
	if(Utils::lookSystemlog(time_log,&inputLog)!=0 || inputLog.empty() )
		return;
	else
	{
		list<SystemlogInfo>::iterator iter;
		int i=-1;
		for(iter=inputLog.begin();iter!=inputLog.end();iter++)
		{
			SystemlogInfo temp;
			temp=*iter;
			i++;
		    m_listLogRecord.InsertItem(i, _T(temp.oper_id.c_str()));
		    m_listLogRecord.SetItemText(i, 1, _T(temp.user_id.c_str()));
		    m_listLogRecord.SetItemText(i, 2, _T(temp.user_name.c_str()));
			m_listLogRecord.SetItemText(i, 3, _T(temp.event_log.c_str()));
		    m_listLogRecord.SetItemText(i, 4, _T(temp.result_log.c_str()));
			m_listLogRecord.SetItemText(i, 5, _T(temp.time_log.c_str()));
		}
	}
}
