// EditWithButton.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapEdit.h"
#include "Util.h"

// CBitmapEdit

IMPLEMENT_DYNAMIC(CBitmapEdit, CEdit)

CBitmapEdit::CBitmapEdit()
{
	m_rcEditArea.SetRect(0,0,0,0);
	m_nMin = 0;
	m_nMax = 0;
}

CBitmapEdit::~CBitmapEdit()
{
	m_strBitmap.Empty();
}


BEGIN_MESSAGE_MAP(CBitmapEdit, CEdit)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


void CBitmapEdit::SetThreshold(int nMin, int nMax)
{
	m_nMin = nMin;
	m_nMax = nMax;
}

// CBitmapEdit message handlers

void CBitmapEdit::PreSubclassWindow( )
{	
	// We must have a multiline edit
	// to be able to set the edit rect
	ASSERT( GetStyle() & ES_MULTILINE );

	ResizeWindow();

}

BOOL CBitmapEdit::PreTranslateMessage( MSG* pMsg )
{
	return CEdit::PreTranslateMessage(pMsg);
}

BOOL CBitmapEdit::SetBitmaps(CString strBmp)
{
	//delete if already loaded.. just in case
	m_strBitmap = strBmp;
	
	return TRUE;

}

void CBitmapEdit::ResizeWindow()
{
	if (!::IsWindow(m_hWnd)) return;

	//proceed only if edit area is set
	if (m_rcEditArea == CRect(0,0,0,0)) return;

//	SetWindowPos(&wndTop,m_rcEditArea.left,m_rcEditArea.top,m_rcEditArea.Width(),m_rcEditArea.Height() - 4,SWP_NOMOVE|SWP_NOZORDER);

	SetRect(&m_rcEditArea);
}


//set edit area may be called before creating the edit control
//especially when using the CEdit::Create method
//or after creating the edit control in CEdit::DoDataExchange
//we call ResizeWindow once in SetEditArea and once in PreSubclassWindow
BOOL CBitmapEdit::SetEditArea(CRect rcEditArea)
{
	m_rcEditArea = rcEditArea;

	ResizeWindow();

	return TRUE;
}

BOOL CBitmapEdit::OnEraseBkgnd(CDC* pDC)
{
	// Get the size of the bitmap
	CRect rect;
	GetClientRect(&rect);

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, m_strBitmap, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	Util::Draw9Grid(pDC, rect, hBitmap);

	return TRUE;
}

BOOL CBitmapEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint pntCursor;
	GetCursorPos(&pntCursor);
	ScreenToClient(&pntCursor);
	//if mouse is not in the edit area then
	//show arrow cursor
	if (!m_rcEditArea.PtInRect(pntCursor))
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW)));
		return TRUE;
	}

	return CEdit::OnSetCursor(pWnd, nHitTest, message);
}

int CBitmapEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	ResizeWindow();

	return 0;
}

LRESULT CBitmapEdit::OnSetFont( WPARAM wParam, LPARAM lParam )
{
	DefWindowProc( WM_SETFONT, wParam, lParam );

	ResizeWindow();

	return 0;
}

void CBitmapEdit::OnSize( UINT nType, int cx, int cy ) 
{

	CEdit::OnSize( nType, cx, cy );

	ResizeWindow();

}

void CBitmapEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//this will draw the background again
	//so that the button will be drawn if the text exists
	
	Invalidate();
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
