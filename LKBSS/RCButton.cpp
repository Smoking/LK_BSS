// RCButton.cpp : implementation file
//

#include "stdafx.h"
#include "RCButton.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCButton

CRCButton::CRCButton()
{
	m_bFocus = FALSE;
	m_bHover = FALSE;
	m_bSelected = FALSE;
	m_bTracking = FALSE;
	m_bAutoSize = TRUE;

	m_hBmpNormal = NULL;
	m_hBmpHover = NULL;
	m_hBmpDefault = NULL;
	m_hBmpDown = NULL;
	m_hBmpDisable = NULL;


	//默认按钮样式
	CString strNormalPath	= Util::GetDialogResPath() + "\\OtherBtn.bmp";
	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	CString strDisablePath	= Util::GetDialogResPath() + "\\OtherBtn_disable.bmp";
	HBITMAP hBitmap_Disable	= (HBITMAP)LoadImage(NULL, strDisablePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	CString strDownPath	= Util::GetDialogResPath() + "\\OtherBtn_down.bmp";
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
 
//	SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Normal, hBitmap_Normal);
	m_hBmpNormal = hBitmap_Normal;
	m_hBmpHover = hBitmap_Normal;
	m_hBmpDown = hBitmap_Down;
	m_hBmpDefault = hBitmap_Normal;
	m_hBmpDisable = hBitmap_Disable;
}

CRCButton::~CRCButton()
{
	if (m_hBmpNormal)
	{
		DeleteObject(m_hBmpNormal);
		m_hBmpNormal = NULL;
	}

	if (m_hBmpHover)
	{
		DeleteObject(m_hBmpHover);
		m_hBmpHover = NULL;
	}

	if (m_hBmpDefault)
	{
		DeleteObject(m_hBmpDefault);
		m_hBmpDefault = NULL;
	}

	if (m_hBmpDown)
	{
		DeleteObject(m_hBmpDown);
		m_hBmpDown = NULL;
	}

	if (m_hBmpDisable)
	{
		DeleteObject(m_hBmpDisable);
		m_hBmpDisable = NULL;
	}
}


BEGIN_MESSAGE_MAP(CRCButton, CButton)
	//{{AFX_MSG_MAP(CRCButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCButton message handlers

void CRCButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PreSubclassWindow();
	if (m_hBmpNormal)
	{
		ModifyStyle(0, BS_OWNERDRAW);
	}
}

void CRCButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC *pDC = NULL;
	CRect rcCtrl , rcCtrlDown;
	UINT uState = 0;
	CString strText = "";
	
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	rcCtrl = lpDrawItemStruct->rcItem;
	rcCtrlDown = rcCtrl;
	rcCtrlDown.OffsetRect(1, 1);

	GetWindowText(strText);
	uState = lpDrawItemStruct->itemState;

	//背景贴图
	if (uState & ODS_DISABLED && m_hBmpDisable)
	{
		DrawButton(pDC,rcCtrl,m_hBmpDisable);
	}
	else if (uState & ODS_SELECTED && m_hBmpDown)  //按下状态
	{
		DrawButton(pDC, rcCtrl, m_hBmpDown);
	}
	else if (m_bHover && m_hBmpHover)   //Hover状态
	{
		DrawButton(pDC, rcCtrl, m_hBmpHover);
	}
	else if (m_hBmpNormal)   //正常状态
	{
		DrawButton(pDC, rcCtrl, m_hBmpNormal);

	}

	//绘制按钮文字
	pDC->SetBkMode(TRANSPARENT);
	if (uState & ODS_SELECTED)
	{		
		pDC->DrawText(strText,rcCtrlDown,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	else
	{
		pDC->DrawText(strText,rcCtrl,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
}

void CRCButton::SetBMP(CBitmap *pBmpNormal = NULL, CBitmap *pBmpHover = NULL, CBitmap *pBmpDown = NULL, CBitmap *pBmpDef = NULL)
{
	m_hBmpNormal = (HBITMAP)pBmpNormal->GetSafeHandle();
	m_hBmpHover = (HBITMAP)pBmpHover->GetSafeHandle();
	m_hBmpDown = (HBITMAP)pBmpDown->GetSafeHandle();
	m_hBmpDefault = (HBITMAP)pBmpDef->GetSafeHandle();
	PreSubclassWindow();
	Invalidate();
}

void CRCButton::SetBMP(HBITMAP hBmpNormal, HBITMAP hBmpHover, HBITMAP hBmpDown, HBITMAP hBmpDef)
{
	m_hBmpNormal = hBmpNormal;
	m_hBmpHover = hBmpHover;
	m_hBmpDown = hBmpDown;
	m_hBmpDefault = hBmpDef;
	PreSubclassWindow();
	Invalidate();
}
void CRCButton::SetBMP(HBITMAP hBmpNormal, HBITMAP hBmpHover, HBITMAP hBmpDown, HBITMAP hBmpDef,HBITMAP hBmpDisable)
{
	m_hBmpNormal = hBmpNormal;
	m_hBmpHover = hBmpHover;
	m_hBmpDown = hBmpDown;
	m_hBmpDefault = hBmpDef;
	m_hBmpDisable = hBmpDisable;
	PreSubclassWindow();
	Invalidate();
}

void CRCButton::SetBMP(UINT IDNormal = 0, UINT IDHover = 0, UINT IDDown = 0, UINT IDDef = 0)
{
	CBitmap bmp;
	if (IDNormal)
	{
		bmp.LoadBitmap(MAKEINTRESOURCE(IDNormal));
		m_hBmpNormal = (HBITMAP)bmp.GetSafeHandle();
		bmp.Detach();
	}
	if (IDHover)
	{
		bmp.LoadBitmap(MAKEINTRESOURCE(IDHover));
		m_hBmpHover = (HBITMAP)bmp.GetSafeHandle();
		bmp.Detach();
	}
	if (IDDown)
	{
		bmp.LoadBitmap(MAKEINTRESOURCE(IDDown));
		m_hBmpDown = (HBITMAP)bmp.GetSafeHandle();
		bmp.Detach();
	}
	if (IDDef)
	{
		bmp.LoadBitmap(MAKEINTRESOURCE(IDDef));
		m_hBmpDefault = (HBITMAP)bmp.GetSafeHandle();
		bmp.Detach();
	}
	PreSubclassWindow();
	Invalidate();
}

CRCButton& CRCButton::operator =(const CRCButton button)
{
	m_hBmpNormal = button.m_hBmpNormal;
	m_hBmpDown = button.m_hBmpDown;
	m_hBmpHover = button.m_hBmpHover;
	m_hBmpDefault = button.m_hBmpDefault;
	return *this;
}

void CRCButton::SetSizeMode(BOOL bAutoSize)
{
	m_bAutoSize = bAutoSize;
}

void CRCButton::DrawButton(CDC *pDC, CRect rcCtrl, HBITMAP hBitmap)
{
	BITMAP		bmp;
	CDC			MemDC;
	const		BlockWidth = 3;  
	CBitmap		*bitmap = NULL, *pOldBitmap = NULL;

	bitmap = CBitmap::FromHandle(hBitmap);
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(bitmap);

	if (m_bAutoSize)  //九宫格绘图
	{
		bitmap->GetBitmap(&bmp);
		pDC->BitBlt(0, 0, BlockWidth, BlockWidth, &MemDC, 0, 0, SRCCOPY);
		pDC->BitBlt(0, rcCtrl.Height() - BlockWidth, BlockWidth, BlockWidth, &MemDC, 0, bmp.bmHeight - BlockWidth, SRCCOPY);
		pDC->BitBlt(rcCtrl.Width() - BlockWidth, 0, BlockWidth, BlockWidth, &MemDC, bmp.bmWidth - BlockWidth, 0, SRCCOPY);
		pDC->BitBlt(rcCtrl.Width() - BlockWidth, rcCtrl.Height() - BlockWidth, BlockWidth, BlockWidth, &MemDC, 
			bmp.bmWidth - BlockWidth, bmp.bmHeight - BlockWidth, SRCCOPY);
		pDC->StretchBlt(0, BlockWidth, BlockWidth, rcCtrl.Height() - 2*BlockWidth, &MemDC, 0, BlockWidth,
			BlockWidth, bmp.bmHeight - 2*BlockWidth, SRCCOPY);
		pDC->StretchBlt(BlockWidth, rcCtrl.Height() - BlockWidth, rcCtrl.Width() - 2*BlockWidth, BlockWidth, &MemDC,
			BlockWidth, bmp.bmHeight - BlockWidth, bmp.bmWidth - 2*BlockWidth, BlockWidth, SRCCOPY);
		pDC->StretchBlt(rcCtrl.Width() - BlockWidth, BlockWidth, BlockWidth, rcCtrl.Height() - 2*BlockWidth, &MemDC,
			bmp.bmWidth - BlockWidth, BlockWidth, BlockWidth, bmp.bmHeight - 2*BlockWidth, SRCCOPY);
		pDC->StretchBlt(BlockWidth, 0, rcCtrl.Width() - 2*BlockWidth, BlockWidth, &MemDC, BlockWidth, 0,
			bmp.bmWidth - 2*BlockWidth, BlockWidth, SRCCOPY);
		pDC->StretchBlt(BlockWidth, BlockWidth, rcCtrl.Width() - 2*BlockWidth, rcCtrl.Height() - 2*BlockWidth, &MemDC,
			BlockWidth, BlockWidth, bmp.bmWidth - 2*BlockWidth, bmp.bmHeight - 2*BlockWidth, SRCCOPY);
//		pDC->StretchBlt(0, 0, rcCtrl.Width(), rcCtrl.Height(), &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}
	else  //直接贴图
	{
		pDC->BitBlt(0, 0, rcCtrl.Width(), rcCtrl.Height(), &MemDC, 0, 0, SRCCOPY);
	}

	MemDC.SelectObject(pOldBitmap);
	DeleteObject(MemDC);
	bitmap->Detach();
}

void CRCButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CRCButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bHover = TRUE;
	InvalidateRect(NULL);
	return 0;
}
LRESULT CRCButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bHover = FALSE;
	m_bTracking = FALSE;
	InvalidateRect(NULL, FALSE);
	
	return 0;
}

