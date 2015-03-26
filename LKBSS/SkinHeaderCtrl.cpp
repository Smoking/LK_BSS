// SkinHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkinHeaderCtrl.h"
#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl

CSkinHeaderCtrl::CSkinHeaderCtrl()
{
}

CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSkinHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl message handlers

void CSkinHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
}

void CSkinHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect, rectItem, clientRect;
	GetClientRect(&rect);
	GetClientRect(&clientRect);

	//创建位图DC和内存兼容DC
	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);
	CMemDC memDC(&dc, &rect);
	
//	memDC.FillSolidRect(&rect, RGB(76,85,118));

	//绘制整个背景
	CBitmap bitmapSpan;
	bitmapSpan.LoadBitmap(IDB_COLUMNHEADER_SPAN);
	CBitmap* pOldBitmapSpan = bitmapDC.SelectObject(&bitmapSpan);

	BITMAP	bmp;
	bitmapSpan.GetBitmap(&bmp);

	memDC.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &bitmapDC, 0, 0, 
		bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	bitmapDC.SelectObject(pOldBitmapSpan);
	bitmapSpan.DeleteObject();
	
	//绘制每个Header
	int nItems = GetItemCount();

	CBitmap bmpStart, bmpSpan, bmpEnd;
	bmpStart.LoadBitmap(IDB_COLUMNHEADER_START);
	bmpSpan.LoadBitmap(IDB_COLUMNHEADER_SPAN);
	bmpEnd.LoadBitmap(IDB_COLUMNHEADER_END);

	for(int i = 0; i <nItems; i++)
	{
		TCHAR buf[256];
		HD_ITEM hdItem;
		
		hdItem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hdItem.pszText = buf;
		hdItem.cchTextMax = 255;
		GetItem(i, &hdItem);
		
		GetItemRect(i, &rect);
		
		CBitmap* pOldBitmap = NULL;
		
		//make sure we draw the start piece
		//on the first item so it has a left border

		//For the following items we will just use the
		//right border of the previous items as the left
		//border
		if(hdItem.iOrder==0)
		{
			pOldBitmap = bitmapDC.SelectObject(&bmpStart);

			bmpStart.GetBitmap(&bmp);
			memDC.StretchBlt(rect.left,rect.top, bmp.bmWidth, rect.Height(),
				&bitmapDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		}
		else
		{
			pOldBitmap = bitmapDC.SelectObject(&bmpSpan);

			bmpSpan.GetBitmap(&bmp);
			memDC.StretchBlt(rect.left+1, rect.top, bmp.bmWidth, rect.Height(),
				&bitmapDC,0,0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		}

		bitmapDC.SelectObject(pOldBitmap);
		
		//span the bitmap for the width of the column header item
		int nWidth = rect.Width() - 4;	
		bmpSpan.GetBitmap(&bmp);
		CBitmap* pOldBitmap2 = bitmapDC.SelectObject(&bmpSpan);

		memDC.StretchBlt(rect.left+2, rect.top, nWidth, rect.Height(), &bitmapDC, 
			0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		bitmapDC.SelectObject(pOldBitmap2);
		
		
		//draw the end piece of the column header
		bmpEnd.GetBitmap(&bmp);
		CBitmap* pOldBitmap3 = bitmapDC.SelectObject(&bmpEnd);
		memDC.StretchBlt((rect.right-bmp.bmWidth), rect.top, bmp.bmWidth, rect.Height(), 
			&bitmapDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		bitmapDC.SelectObject(pOldBitmap3);
		
		//Get all the info for the current
		//item so we can draw the text to it
		//in the desired font and style
		DRAWITEMSTRUCT	DrawItemStruct;
		GetItemRect(i, &rectItem);
		
		
		DrawItemStruct.CtlType		= 100;
		DrawItemStruct.hDC			= dc.GetSafeHdc();
		DrawItemStruct.itemAction	= ODA_DRAWENTIRE; 
		DrawItemStruct.hwndItem 	= GetSafeHwnd(); 
		DrawItemStruct.rcItem	= rectItem;
		DrawItemStruct.itemID	= i;
		DrawItem(&DrawItemStruct);
		
		UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_TOP |DT_CENTER | DT_END_ELLIPSIS ;
		
		
 		CFont font;
 		LOGFONT lf;
  		memset(&lf, 0, sizeof(LOGFONT));
 		lf.lfHeight = 12;
  		strcpy(lf.lfFaceName, "宋体");
  		font.CreateFontIndirect(&lf);
  		CFont* def_font = memDC.SelectObject(&font);
		
		memDC.SetBkMode(TRANSPARENT);
		rectItem.DeflateRect(2,2,2,2);
		
		HD_ITEM hditem;
		
		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem( DrawItemStruct.itemID, &hditem );

		memDC.DrawText(buf, &rectItem, uFormat);
 		memDC.SelectObject(def_font);
 		font.DeleteObject();
	}		
}

BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;	
}
