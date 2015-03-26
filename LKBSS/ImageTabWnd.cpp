// ImageTabWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ImageTabWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageTabWnd

CImageTabWnd::CImageTabWnd()
{
	m_pBitmapTab = NULL;
	m_pRectTab = NULL;
	m_pRectButton= NULL;
	m_nTabCount = 0;
	m_nCurrentIndex = 0;
    showFalg=false;
	for(int i=0; i<MAX_DIALOG; i++)
		m_pTabDialog[i] = NULL;
}

CImageTabWnd::~CImageTabWnd()
{
	if (m_pRectTab)
		delete []m_pRectTab;

	if (m_pBitmapTab)
		delete []m_pBitmapTab;
	
	if(m_pRectButton)
		delete []m_pRectButton;
}


BEGIN_MESSAGE_MAP(CImageTabWnd, CWnd)
	//{{AFX_MSG_MAP(CImageTabWnd)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImageTabWnd message handlers

void CImageTabWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 
// 	CDC m_MemDC;
// 	m_MemDC.CreateCompatibleDC(&dc);
// 
// 	CBitmap btScreen;
// 	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
// 
// 	m_MemDC.SelectObject(&btScreen);
// 	btScreen.DeleteObject();

	//»­Tab
	//DrawPosImage(&m_pBitmapTab[m_nCurrentIndex], &m_MemDC, CPoint(0, 0));

	//BITMAP bm;
	//m_pBitmapTab[m_nCurrentIndex].GetBitmap(&bm);

	//»­tab±³¾° 
	//DrawRangeImage(&m_BmpTabBk, &m_MemDC, CRect(bm.bmWidth, 0, rcClient.Width(), TAB_HEIGHT));
	
	//»­µ½ÏÔÊ¾Æ÷ÉÏ
	//dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &m_MemDC, 0, 0, SRCCOPY);

	//m_MemDC.DeleteDC();

	// Do not call CWnd::OnPaint() for painting messages
}

void CImageTabWnd::SetTabCount(int nCount)
{
	m_nTabCount = nCount;

	if (m_pBitmapTab)
		delete []m_pRectTab;
	m_pBitmapTab = new CBitmap[m_nTabCount];

	if (m_pRectTab)
		delete []m_pRectTab;
	m_pRectTab = new CRect[m_nTabCount];

}

void CImageTabWnd::SetTabImage(int nIndex, CString strName)
{
	ASSERT(nIndex < m_nTabCount);
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)::LoadImage(NULL, strName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	ASSERT(hBitmap);
	
	if (m_pBitmapTab[nIndex].m_hObject)
		m_pBitmapTab[nIndex].Detach();

	m_pBitmapTab[nIndex].Attach(hBitmap);
}

void CImageTabWnd::SetTabImage(int nIndex, UINT nID)
{
	ASSERT(nIndex < m_nTabCount);

	if (m_pBitmapTab[nIndex].m_hObject)
		m_pBitmapTab[nIndex].Detach();

	m_pBitmapTab[nIndex].LoadBitmap(nID);
}

void CImageTabWnd::SetTabRect(int nIndex, CRect rc)
{
	ASSERT(nIndex < m_nTabCount);
	m_pRectTab[nIndex] = rc;
}

void CImageTabWnd::SetImage(CBitmap &bitmap, CString strName)
{
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)::LoadImage(NULL, strName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	ASSERT(hBitmap);

	if (bitmap.m_hObject)
		bitmap.Detach();

	bitmap.Attach(hBitmap);
}

void CImageTabWnd::SetImage(CBitmap &bitmap, UINT nID)
{
	if (bitmap.m_hObject)
		bitmap.Detach();

	bitmap.LoadBitmap(nID);
}


void CImageTabWnd::SetTabBkImage(CString strName)
{
	SetImage(m_BmpTabBk, strName);
	SetImageSize(&m_BmpTabBk, m_szBitmapBk);
}

void CImageTabWnd::SetTabBkImage(UINT nID)
{
	SetImage(m_BmpTabBk, nID);
	SetImageSize(&m_BmpTabBk, m_szBitmapBk);
}

void CImageTabWnd::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
{
	if (pBitmap && pBitmap->m_hObject)
	{
		CDC MemDC;
		BITMAP bm;
		pBitmap->GetBitmap(&bm);
		
		int li_Width = bm.bmWidth;
		int li_Height = bm.bmHeight;
		
		MemDC.CreateCompatibleDC(pDC);
		CBitmap* pOldBitmap = MemDC.SelectObject(pBitmap);
		
		int x=rc.left;
		int y=rc.top;
		while (y < (rc.Height()+rc.top)) 
		{
			while(x < (rc.Width()+rc.left)) 
			{
				pDC->BitBlt(x, y, li_Width, li_Height, &MemDC, 0, 0, SRCCOPY);
				x += li_Width;
			}
			x = rc.left;
			y += li_Height;
		}
		
		MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	}
}

void CImageTabWnd::DrawPosImage(CBitmap *pBitmap, CDC *pDC, CPoint pt)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = (CBitmap*)MemDC.SelectObject(pBitmap);

	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	int li_Width = bm.bmWidth;
	int li_Height = bm.bmHeight;

	pDC->BitBlt(pt.x, pt.y, li_Width, li_Height, &MemDC, 0, 0, SRCCOPY);
	
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}


BOOL CImageTabWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	for(int i=0; i<m_nTabCount; i++)
	{
		if (m_pRectTab[i].PtInRect(pt) && m_nCurrentIndex != i)
		{
			SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(32649)));
			return TRUE;
		}
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CImageTabWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	for(int i=0; i<m_nTabCount; i++)
	{
		if (m_pRectTab[i].PtInRect(point) && m_nCurrentIndex != i)
		{
			m_nCurrentIndex = i;
			ShowDialog();
			if(showFalg)
			{ 
				if(i==0)
				    ::PostMessage(hwnd,WM_BUTTONDOWN,0,i);
			    else if(i==1)
				    ::PostMessage(hwnd,WM_BUTTONDOWN,0,i);
			}
			break;
		}
	}
// 	if(showFalg && m_nCurrentIndex==0)
// 	{
// 		for(int j=0; j<=m_nTabCount; j++)
// 		{
// 	         if(m_pRectButton[j].PtInRect(point))
// 			 {
// 			    ::PostMessage(hwnd,WM_BUTTONDOWN,0,j+2);
// 				break;
// 			 }
// 		}
// 	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CImageTabWnd::SetImageSize(CBitmap *pBitmap, CSize &sz)
{
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	sz = CSize(bm.bmWidth, bm.bmHeight);
}

void CImageTabWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rcClient;
	GetClientRect(&rcClient);
	for(int i=0; i<m_nTabCount; i++)
	{
		if (m_pTabDialog[i])
		{
			m_rcDialog = CRect(0, TAB_HEIGHT, rcClient.Width(), rcClient.Height());
			if (m_pTabDialog[i]->GetSafeHwnd())
			{
				m_pTabDialog[i]->MoveWindow(m_rcDialog, FALSE);
				UpdateTabWnd();
			}
		}
	}
	ShowDialog();
}

void CImageTabWnd::SetTabWnd(int nIndex, CDialog *pDlg)
{
	ASSERT(nIndex < m_nTabCount);

	m_pTabDialog[nIndex] = pDlg;
}

void CImageTabWnd::ShowDialog()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	for(int i=0; i<m_nTabCount; i++)
	{
		if (m_pTabDialog[i] && i!= m_nCurrentIndex)
		{
			m_pTabDialog[i]->ShowWindow(SW_HIDE);
		}
	}
	if (m_pTabDialog[m_nCurrentIndex])
	{
		m_pTabDialog[m_nCurrentIndex]->ShowWindow(SW_SHOW);
		m_pTabDialog[m_nCurrentIndex]->SetFocus();
	}
	Invalidate(TRUE);
}

BOOL CImageTabWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CImageTabWnd::UpdateTabWnd()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	//TOP
	InvalidateRect(CRect(0, 0, rcClient.Width(), TAB_HEIGHT));	
}

void CImageTabWnd::SetToolTip(int nIndex, CString strTips)
{
	ASSERT(nIndex < m_nTabCount);
	//ASSERT(m_pRectTab[nIndex].IsRectEmpty() == FALSE);
	m_ToolTip.AddTool(this, strTips, m_pRectTab[nIndex], 12345678);
	m_ToolTip.SetTipBkColor(RGB(231,236,247));
	m_ToolTip.Activate(TRUE);
}

BOOL CImageTabWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_ToolTip.GetSafeHwnd())
		m_ToolTip.RelayEvent( pMsg );
	return CWnd::PreTranslateMessage(pMsg);
}

int CImageTabWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_ToolTip.Create(this))
		return -1;

	//ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	return 0;
}


void CImageTabWnd::setButtonCount(int count)
{
    if(m_pRectButton)
		delete []m_pRectButton;
	m_pRectButton=new CRect[count];
}

void CImageTabWnd::SetButtonRect(int nIndex, CRect rc)
{
    m_pRectButton[nIndex]=rc;
}
