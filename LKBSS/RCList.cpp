// RCList.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "RCList.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCList

CRCList::CRCList()
{
	m_clrBrush = INFOZONE_BK_COLOR;
}

CRCList::~CRCList()
{
}


BEGIN_MESSAGE_MAP(CRCList, CListCtrl)
	//{{AFX_MSG_MAP(CRCList)
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCList message handlers


void CRCList::SetBgColor(COLORREF clr)
{
	m_clrBrush = clr;
}

void CRCList::OnNcPaint() 
{
	CListCtrl::OnNcPaint();

	CWindowDC dc(this);
	CRect rect;
	GetWindowRect( &rect );
	CPen pen(PS_SOLID,1,RGB(153,158,162));
	CPen *old_pen = dc.SelectObject(&pen);
	
	int w = rect.Width() - 1 ;
	int h = rect.Height() - 1;
	dc.MoveTo(0,0);
	dc.LineTo(w,0);
	dc.LineTo(w,h);
	dc.LineTo(0,h);
	dc.LineTo(0,0);
	
	dc.SelectObject(old_pen);

	// Do not call CListCtrl::OnNcPaint() for painting messages
}

void CRCList::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(GetHeaderCtrl())
		m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);
	
	CListCtrl::PreSubclassWindow();
}
