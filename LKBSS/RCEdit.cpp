// RCEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "RCEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCEdit

#define RADIUS 5

CRCEdit::CRCEdit()
{
}

CRCEdit::~CRCEdit()
{
}


BEGIN_MESSAGE_MAP(CRCEdit, CEdit)
	//{{AFX_MSG_MAP(CRCEdit)
//	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CRCEdit::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CRCEdit::OnPaint()
{
	CPaintDC dc(this); 
	dc.SetBkColor(INFOZONE_BK_COLOR);  

	CPen pen(PS_SOLID, 4, RGB(169, 189, 213));
	CPen *old_pen = dc.SelectObject(&pen);
	
	CRect rc;
	GetClientRect(&rc);

	rc.InflateRect(RADIUS, RADIUS, RADIUS, RADIUS);
	dc.RoundRect(rc.left,rc.top,rc.right,rc.bottom, RADIUS, RADIUS);

	CRgn rgn;
	rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom, RADIUS, RADIUS);
	dc.FillRgn(&rgn, &CBrush(INFOZONE_BK_COLOR));
	
	dc.SelectObject(old_pen);
}

