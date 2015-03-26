// RCMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "RCMessageBox.h"
#include "Util.h"

#define ICON_XOFFSET 20

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCMessageBox dialog


CRCMessageBox::CRCMessageBox(CWnd* pParent /*=NULL*/)
	: CRCDialog(CRCMessageBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCMessageBox)
	m_strText = _T("");
	//}}AFX_DATA_INIT
	m_strCaption = _T("");
	m_hWndParent = NULL;
	m_uType = RCMB_OKCANCEL;
}


void CRCMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CRCDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCMessageBox)
	DDX_Control(pDX, IDCANCEL, m_btnCancle);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_Text, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCMessageBox, CRCDialog)
	//{{AFX_MSG_MAP(CRCMessageBox)
	ON_WM_ERASEBKGND()
//	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCMessageBox message handlers

void CRCMessageBox::InitParam(HWND hWnd, CString strText, CString strCaption, UINT nType)
{
	//���ñ�����
	m_hWndParent = hWnd;
	strText = "\r\n" + strText;
	m_strText = strText;
	if(strCaption.IsEmpty())
		m_strCaption="��ʾ";
	else
		m_strCaption = strCaption;
	m_uType = nType;
}

//���ñ���
BOOL CRCMessageBox::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	
	CString strLogoPath = Util::GetDialogResPath() + "\\bg_warn.bmp";
	Util::FillRectWithBmp(pDC, strLogoPath, rect, FALSE);
	
	return TRUE;
}

void CRCMessageBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRCDialog::OnPaint();
	
	// TODO: Add your message handler code here
	CFont font;
	font.CreatePointFont(200, "����");

	CRect rect;
	GetClientRect(&rect);
	
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255, 255, 255));
	dc.SelectObject(&font);
	
	CSize size;
	size = dc.GetTextExtent(m_strText);
	
	int xPos = (rect.Width() - size.cx)/2 + ICON_XOFFSET;
	int yPos = (rect.Height() - size.cy)/2;
	dc.TextOut(xPos, yPos, m_strText);

	
	// Do not call CRCDialog::OnPaint() for painting messages
}

BOOL CRCMessageBox::OnInitDialog() 
{
	CRCDialog::OnInitDialog();
	InitButton();
	// TODO: Add extra initialization here
	//���ñ�����
	if (!m_strCaption.IsEmpty())
	{
		SetWindowText(m_strCaption);
	}
	
	//�������ť
	if (RCMB_OKCANCEL == m_uType)
	{
	}
	else if (RCMB_OK == m_uType)
	{
		//������ȡ����ť
		m_btnCancle.ShowWindow(SW_HIDE);
		
		//����ȷ����ťλ�ò��ƶ� 
		CRect rect;
		GetClientRect(&rect);
		
		CRect rcBtnOK;
		m_btnOK.GetWindowRect(&rcBtnOK);
		ScreenToClient(&rcBtnOK);
		
		int xPos = (rect.Width() - rcBtnOK.Width())/2;
		int yPos = rcBtnOK.top;
		
		m_btnOK.MoveWindow(xPos, yPos, rcBtnOK.Width(), rcBtnOK.Height());
	}

	UpdateData();
	CenterWindow();
	//���ø�����
//	SetParent(FromHandle(m_hWndParent));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCMessageBox::InitButton()
{
	CString strNormalPath	= Util::GetDialogResPath() + "\\ensure_normal.bmp";
 	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	CString strDownPath	= Util::GetDialogResPath() + "\\ensure_down.bmp";
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	m_btnOK.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);

	strNormalPath	= Util::GetDialogResPath() + "\\cancel_normal.bmp";
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);		
	strDownPath	= Util::GetDialogResPath() + "\\cancel_down.bmp";
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	m_btnCancle.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);	
}
