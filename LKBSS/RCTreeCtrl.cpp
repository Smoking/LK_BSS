
#include "stdafx.h"
#include "RCTreeCtrl.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KEYDOWN(Key) ((GetKeyState(Key)&0x8000)!=0)

/////////////////////////////////////////////////////////////////////////////
// CRCTreeCtrl

CRCTreeCtrl::CRCTreeCtrl()
{
	m_clrSelBk = ::GetSysColor(COLOR_HIGHLIGHT);	// 选中项目的背景色
	m_clrSelText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);	// 选中项目的字体色
	m_clrFolderBk = ::GetSysColor(COLOR_ACTIVECAPTION);	// Folder 的背景色
	m_clrFolderText = ::GetSysColor(COLOR_CAPTIONTEXT); // Folder 的字体颜色
	m_clrFolderOverText = ::GetSysColor(COLOR_INACTIVECAPTION);

	m_clrBorder = RGB(105, 116, 225);
	m_nFolderExpended = -1;
	m_nFolderUnExpended = -1;
	m_ptImgOffset.x = 2;
	m_ptImgOffset.y = 2;
	
	m_hHotItem = NULL;
	m_hOverItem = NULL;
	m_bTracking = FALSE;
	m_hPressedItem = NULL;
	m_nTextIndent = 20;

	m_bClickToExpand = TRUE;

	m_bEnableClipCursor = FALSE;
	m_bClipCursor = FALSE;
}

CRCTreeCtrl::~CRCTreeCtrl()
{
	if (m_bEnableClipCursor)
	{
		ClipCursor(NULL);
	}
	
}


BEGIN_MESSAGE_MAP(CRCTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CRCTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SETCURSOR()	
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCTreeCtrl message handlers
void CRCTreeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CTreeCtrl::OnSize(nType, cx, cy);
	
	ShowScrollBar(SB_HORZ,FALSE);		
}

void CRCTreeCtrl::OnNcPaint() 
{
	CTreeCtrl::OnNcPaint();
	// TODO: Add your message handler code here
	CWindowDC dc(this);

	CRect rt;
	GetWindowRect(&rt);
    ScreenToClient(&rt);
	int nLeft = rt.left;
	int nTop = rt.top;
	rt.InflateRect(nLeft,nTop,-nLeft,-nTop);
	if(nLeft && nTop)
		dc.Draw3dRect(rt, m_clrBorder, m_clrBorder);
}

void CRCTreeCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);	

	HTREEITEM hItem = (HTREEITEM)pLVCD->nmcd.dwItemSpec;

	switch(pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW ;
		}
		break;
    case CDDS_ITEMPREPAINT:
		{
			DrawItem(pLVCD);
			* pResult = CDRF_SKIPDEFAULT;
		}	
		break;	
	default:
			break;
	}
}


void CRCTreeCtrl::SetSelBkColor(COLORREF clr)
{
	m_clrSelBk = clr;
}

void CRCTreeCtrl::SetFolderBkColor(COLORREF clr)
{
	m_clrFolderBk = clr;
}

void CRCTreeCtrl::SetFolderTextColor(COLORREF clr)
{
	m_clrFolderText = clr;
}

void CRCTreeCtrl::SetExpendedImage(int nImage)
{
	m_nFolderExpended = nImage;
}

void CRCTreeCtrl::SetUnExpendedImage(int nImage)
{
	m_nFolderUnExpended = nImage;
}

void CRCTreeCtrl::SetSelTextColor(COLORREF clr)
{
	m_clrSelText = clr;
}

void CRCTreeCtrl::SetHotItem(HTREEITEM hItem)
{
	HTREEITEM hOldItem = m_hHotItem;
	m_hHotItem = hItem;
	RedrawItem(hOldItem);
}

HTREEITEM CRCTreeCtrl::GetHotItem()
{
	return m_hHotItem;
}

void CRCTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;		
		m_bTracking = _TrackMouseEvent(&tme);
	}

	if (m_bEnableClipCursor && !KEYDOWN(VK_LBUTTON) && m_bClipCursor)
	{
		ClipCursor(NULL);
		m_bClipCursor = FALSE;
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
	HTREEITEM hItem = CTreeCtrl::HitTest(point);
	HTREEITEM hPreItem = m_hOverItem;
	
	
	if(hItem == NULL)
	{		
		if(m_hOverItem != NULL)
		{
			m_hOverItem = hItem;
			RedrawItem(hPreItem);						
		}
		return;
	}

	if(hItem != m_hOverItem)
	{				
		m_hOverItem = hItem;
		RedrawItem(hPreItem);			
		RedrawItem(m_hOverItem);
	}

	if(m_hPressedItem)
	{
		HTREEITEM hItem = m_hPressedItem;
		m_hPressedItem = NULL;
		RedrawItem(hItem);
	}
}

BOOL CRCTreeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW)));
	return TRUE;
}

void CRCTreeCtrl::RedrawItem(HTREEITEM hItem)
{
	if(hItem == NULL)
		return;
	CRect rcItem;
	if(TRUE == GetItemRect(hItem, &rcItem, FALSE))
		InvalidateRect(rcItem);	
}



LRESULT CRCTreeCtrl::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	RedrawItem(m_hOverItem);
	m_hOverItem = NULL;
	return S_OK;
}

POINT CRCTreeCtrl::SetImageOffset(int cx, int cy)
{
	POINT ptRet = m_ptImgOffset;
	m_ptImgOffset.x = cx;
	m_ptImgOffset.y = cy;
	return ptRet;
}



// 根结点和叶子结点分开画
void CRCTreeCtrl::DrawItem(NMLVCUSTOMDRAW *pLVCD)
{
	CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
	HTREEITEM hItem = (HTREEITEM)pLVCD->nmcd.dwItemSpec;
	HTREEITEM hDropItem = GetDropHilightItem();
	pLVCD->clrTextBk = GetBkColor();
	pLVCD->clrText = GetTextColor();
	CRect rcText(pLVCD->nmcd.rc);	
	
	if (rcText.IsRectEmpty())    //避免全部重绘   edwinzhang 2007.12.29
	{
		return;
	}

	if(ItemHasChildren(hItem))
	{					
		pDC->FillRect(&pLVCD->nmcd.rc, &m_brTitle);					
		
		GetItemRect(hItem, &rcText, TRUE);
		rcText.right = pLVCD->nmcd.rc.right;


		CFont* pFont = pDC->GetCurrentFont();
		LOGFONT logFont;
		pFont->GetLogFont(&logFont);
		logFont.lfWeight = FW_BOLD;
		CFont cf;					
		cf.CreateFontIndirect(&logFont);

		CFont* pOldFont = pDC->SelectObject(&cf);
		int nOldMode =pDC->SetBkMode(TRANSPARENT);
		COLORREF oldClr = pDC->SetTextColor(m_clrFolderText);	
		
		if(hItem == m_hOverItem)
			pDC->SetTextColor(m_clrFolderOverText);

		if(hItem == m_hPressedItem)				
		{
			rcText.OffsetRect(1, 1);
			pDC->SetTextColor(m_clrFolderText);
		}
		
		rcText.left = rcText.left > 0 ?  rcText.left : 0;

		pDC->DrawText(GetItemText(hItem), &rcText, DT_LEFT | DT_SINGLELINE | DT_WORD_ELLIPSIS | DT_VCENTER |DT_NOPREFIX);

		pDC->SetTextColor(oldClr);
		pDC->SetBkMode(nOldMode);
		pDC->SelectObject(pOldFont);
					
		CRect rcIcon(pLVCD->nmcd.rc);
		rcIcon.right = rcText.left;
		CImageList* pImgList = GetImageList(TVSIL_NORMAL);
		if(pImgList != NULL)
		{
			TVITEM item;
			item.mask = TVIF_IMAGE | TVIF_HANDLE | TVIF_STATE;
			item.hItem = hItem; 
			if(TRUE == GetItem(&item))
			{				
				int nImage = item.iImage;
				if(hItem == m_hOverItem)
					++nImage;
				if(hItem == m_hPressedItem)
					nImage = item.iImage;
				POINT pt;
				pt.x = rcIcon.left + m_ptImgOffset.x ;
				pt.y = rcIcon.top + m_ptImgOffset.y;
				pImgList->Draw(pDC, nImage, pt, ILD_TRANSPARENT);
			}
		}
		return;
	}

	if((pLVCD->nmcd.uItemState & CDIS_SELECTED) || (m_hHotItem == hItem))
	{
		pLVCD->clrTextBk = m_clrSelBk;
		pLVCD->clrText = m_clrSelText;
	}
	else if(m_hOverItem == hItem || hDropItem == hItem )
	{
		pLVCD->clrTextBk = RGB(174, 217, 251);//g_crOverBk;
	}

	pDC->FillSolidRect(&pLVCD->nmcd.rc, pLVCD->clrTextBk);

	CRect rcIcon(pLVCD->nmcd.rc);
	rcIcon.right = rcText.left;
	CImageList* pImgList = GetImageList(TVSIL_STATE);
	if(pImgList != NULL)
	{
		TVITEM item;
		item.mask = TVIF_IMAGE | TVIF_HANDLE | TVIF_STATE;
		item.hItem = hItem; 
		if(TRUE == GetItem(&item))
		{				
			int nImage = item.iImage;
			if(hItem == m_hOverItem)
				++nImage;
			if(hItem == m_hPressedItem)
				nImage = item.iImage;
			POINT pt;
			pt.x = rcIcon.left + m_ptImgOffset.x ;
			pt.y = rcIcon.top + m_ptImgOffset.y;
			pImgList->Draw(pDC, nImage, pt, ILD_TRANSPARENT);
		}
	}
	rcText.left += m_nTextIndent;
	rcText.left = rcText.left > 0 ?  rcText.left : 0;
	
	COLORREF oldClr = pDC->SetTextColor(pLVCD->clrText);
	int nOldMode =pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(GetItemText(hItem), &rcText, DT_LEFT | DT_SINGLELINE | DT_WORD_ELLIPSIS | DT_VCENTER |DT_NOPREFIX);

	pDC->SetTextColor(oldClr);
	pDC->SetBkMode(nOldMode);	
}

int CRCTreeCtrl::SetTextIndent(int nIndent)
{
	INT nRet = m_nTextIndent;
	m_nTextIndent = nIndent;
	return nRet;
}




void CRCTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	CPoint pt;
	if(FALSE == ::GetCursorPos(&pt))
		return;


	ScreenToClient(&pt);
	HTREEITEM hItem = HitTest(pt);

	if(hItem == NULL)
		return;
	
	if(ItemHasChildren(hItem) && m_bClickToExpand )
	{
		Expand(hItem, TVE_TOGGLE);
		
		if(TVIS_EXPANDED & GetItemState(hItem, TVIS_EXPANDED))
		{		
			if(m_nFolderExpended != -1)
				SetItem(hItem, TVIF_IMAGE|TVIF_SELECTEDIMAGE, NULL, m_nFolderExpended, m_nFolderExpended, 0, 0, 0);
		}
		else
		{
			if(m_nFolderUnExpended != -1)
				SetItem(hItem, TVIF_IMAGE|TVIF_SELECTEDIMAGE, NULL, m_nFolderUnExpended, m_nFolderUnExpended, 0, 0, 0);
		}
		
		HTREEITEM hCurrentItem = hItem;
		
		while (hCurrentItem != NULL)
		{
			hCurrentItem = GetNextSiblingItem(hCurrentItem);
			if(hCurrentItem != NULL)
			{
				Expand(hCurrentItem, TVE_COLLAPSE);

				if(m_nFolderUnExpended != -1)
					SetItem(hCurrentItem, TVIF_IMAGE|TVIF_SELECTEDIMAGE, NULL, m_nFolderUnExpended, m_nFolderUnExpended, 0, 0, 0);
			}
		}
		while (hItem != NULL)
		{
			hItem = GetPrevSiblingItem(hItem);
			if(hItem != NULL)
			{
				Expand(hItem, TVE_COLLAPSE);

				if(m_nFolderUnExpended != -1)
					SetItem(hItem, TVIF_IMAGE|TVIF_SELECTEDIMAGE, NULL, m_nFolderUnExpended, m_nFolderUnExpended, 0, 0, 0);
			}
		}
		
	}
	*pResult = 0;
}

void CRCTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	HTREEITEM hItem = HitTest(point);

	if(m_bEnableClipCursor && hItem)
	{
		CRect rc;
		GetWindowRect(&rc);
		rc.DeflateRect(0, 0, 10, 0);
		ClipCursor(rc);
		m_bClipCursor = TRUE;
	}

	if(hItem && ItemHasChildren(hItem))
	{
		m_hPressedItem = hItem;	
		RedrawItem(hItem);		
	}
	CTreeCtrl::OnLButtonDown(nFlags, point);	
}


void CRCTreeCtrl::PreSubclassWindow() 
{
	CBitmap bmpTitle;
	bmpTitle.LoadBitmap(IDB_FOLDER_TITLE_BK);
	m_brTitle.CreatePatternBrush(&bmpTitle);
	bmpTitle.DeleteObject();
	CTreeCtrl::PreSubclassWindow();
}

COLORREF CRCTreeCtrl::SetFolderOverTextColor(COLORREF clr)
{
	COLORREF clrRet = m_clrFolderOverText;
	m_clrFolderOverText = clr;
	return clrRet;
}


void CRCTreeCtrl::SetBorderColor(COLORREF clr)
{
	m_clrBorder = clr;
}

//只能使能，不能禁止
void CRCTreeCtrl::EnableClipCursor(BOOL bEnableClipCursor /* = TRUE */)
{
	if (bEnableClipCursor == TRUE)
	{
		m_bEnableClipCursor = bEnableClipCursor;
	}
	else
	{
		ASSERT(FALSE);
	}

}

