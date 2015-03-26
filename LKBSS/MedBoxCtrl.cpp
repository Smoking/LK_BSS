// MedBoxCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "MedBoxCtrl.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedBoxCtrl

CMedBoxCtrl::CMedBoxCtrl()
{
	m_nMaxCapicity = 10;
	m_nCurNumber = 8;
	m_clrStatus = COLOR_RED;
	m_clrText = RGB(255, 255, 255);
}

CMedBoxCtrl::~CMedBoxCtrl()
{
}


BEGIN_MESSAGE_MAP(CMedBoxCtrl, CStatic)
	//{{AFX_MSG_MAP(CMedBoxCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedBoxCtrl message handlers

BOOL CMedBoxCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}

void CMedBoxCtrl::SetStatusColor(COLORREF clr)
{
	m_clrStatus = clr;
}

void CMedBoxCtrl::SetTextColor(COLORREF clr)
{
	m_clrText = clr;
}

void CMedBoxCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//Draw Medicine Box
	CString strPath = GetMedBoxPicPath();
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

//	Util::DrawTransparentBitmap(dc.GetSafeHdc(), hBitmap, 0, 0, 0xff00ff);


	/////////////////////////////DrawTransparentBitmap///////////////////////////
	HDC hdc = dc.GetSafeHdc();
	short xStart = 0, yStart = 0;
	COLORREF cTransparentColor = 0xff00ff;

	BITMAP   bm;   
	COLORREF cColor;   
	HBITMAP  bmAndBack, bmAndObject, bmAndMem, bmSave;   
	HBITMAP  bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;   
	HDC      hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;   
	POINT    ptSize;   
    
	hdcTemp = CreateCompatibleDC(hdc);   
	SelectObject(hdcTemp, hBitmap); //Select the bitmap   
    
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);   
	ptSize.x = bm.bmWidth;				//Get width of bitmap   
	ptSize.y = bm.bmHeight;				//Get height of bitmap   
	DPtoLP(hdcTemp,   &ptSize,   1);	//Convert from device   
    
	//to logical points   
    
	//Create some DCs to hold temporary data.   
	hdcBack =  CreateCompatibleDC(hdc);   
	hdcObject = CreateCompatibleDC(hdc);   
	hdcMem = CreateCompatibleDC(hdc);   
	hdcSave = CreateCompatibleDC(hdc);    
    
	//Monochrome DC   
	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);   
    
	//Monochrome DC   
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);   
    
	bmAndMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);   
	bmSave = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);   
    
	//Each DC must select a bitmap object to store pixel data.   
	bmBackOld =  (HBITMAP)SelectObject(hdcBack, bmAndBack);   
	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);   
	bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);   
	bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);   
    
	//Set proper mapping mode.   
	SetMapMode(hdcTemp, GetMapMode(hdc));   
    
	//Save the bitmap sent here,because it will be overwritten.   
	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);   
    
	//Set the background color of the source DC to the color.   
	//contained in the parts of the bitmap that should be transparent   
	cColor = SetBkColor(hdcTemp,   cTransparentColor);   
    
	//Create the object mask for the bitmap by performing a BitBlt from the source bitmap to a monochrome bitmap.   
	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);   
    
	//Set the background color of the source DC back to the original color.   
	SetBkColor(hdcTemp, cColor);   
    
	//Create the inverse of the object mask.   
	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);   
    
	//Copy the background of the main DC to the destination.   
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart, SRCCOPY);   
    
	//Mask out the places where the bitmap will be placed.   
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);   
    
	//Mask out the transparent colored pixels on the bitmap.   
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);   
    
	//XOR the bitmap with the background on the destination DC.   
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);  

	//Draw Status Color
	CRect rect, rcColor;
	GetClientRect(&rect);
	rcColor.left = rect.left + 2;
	rcColor.top = rect.top + 3;
	
	int nWidth = (int)(rect.Width() - 2*2) * (m_nCurNumber/(float)m_nMaxCapicity);
	int nHeight = rect.Height() - 3*2;
	rcColor.SetRect(rcColor.left, rcColor.top, rcColor.left + nWidth, rcColor.top + nHeight);

	CBrush brush(m_clrStatus);
	FillRect(hdcMem, rcColor, (HBRUSH)brush.m_hObject);
	
	//TextOut Capacity 
	CString strText;
	strText.Format("%d/%d", m_nCurNumber, m_nMaxCapicity);
	
	GetClientRect(&rect);
	CSize size = dc.GetTextExtent(strText);
	int xPos = rect.Width()/2 - size.cx/2 + 4;
	int yPos = rect.Height()/2 - size.cy/2 ;
	  
	HFONT hFont, hOldFont;
	hFont = CreateFont(14,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,   
 		CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY,DEFAULT_PITCH   |   FF_SWISS, "Arial");
	hOldFont = (HFONT)SelectObject(hdcMem, hFont); 

	
	SetBkMode(hdcMem, TRANSPARENT);
	::SetTextColor(hdcMem, m_clrText);
	TextOut(hdcMem, xPos, yPos, strText, strText.GetLength());
	
	SelectObject(hdcMem, hOldFont);
	DeleteObject(hFont);
    
	//Copy the destination to the screen.   
	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);   
    
	//Place the original bitmap back into the bitmap sent here.   
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);   
    
	//Delete   the   memory   bitmaps.   
	DeleteObject(SelectObject(hdcBack,   bmBackOld));   
	DeleteObject(SelectObject(hdcObject,   bmObjectOld));   
	DeleteObject(SelectObject(hdcMem,   bmMemOld));   
	DeleteObject(SelectObject(hdcSave,   bmSaveOld));   
    
	//Delete the memory DCs.   
	DeleteDC(hdcMem);   
	DeleteDC(hdcBack);   
	DeleteDC(hdcObject);   
	DeleteDC(hdcSave);   
	DeleteDC(hdcTemp);   
	DeleteObject(hBitmap);

	/////////////////////////////DrawTransparentBitmap End///////////////////////////


	// Do not call CStatic::OnPaint() for painting messages
}

CString CMedBoxCtrl::GetMedBoxPicPath()
{
	CString strRet;
// 	if (MEDBOX_CAPSTATUS_DISABLE == GetCapacityStatus())
// 	{
// 		strRet = Util::GetDialogResPath() + "\\medbox_dis.bmp";
// 	}
// 	else if (MEDBOX_CAPSTATUS_EMPTY == GetCapacityStatus())
// 	{
// 		strRet = Util::GetDialogResPath() + "\\medbox_empty.bmp";
// 	}
// 	else if (MEDBOX_CAPSTATUS_DANGER == GetCapacityStatus())
// 	{
// 		strRet = Util::GetDialogResPath() + "\\medbox_danger.bmp";
// 	}
// 	else if (MEDBOX_CAPSTATUS_NORMAL == GetCapacityStatus() ||
// 			 MEDBOX_CAPSTATUS_FULL == GetCapacityStatus())
// 	{
// 		strRet = Util::GetDialogResPath() + "\\medbox_full.bmp";
// 	}
// 	else
// 	{
// 		strRet = Util::GetDialogResPath() + "\\medbox_dis.bmp";
// 	}
	strRet = Util::GetDialogResPath() + "\\medbox.bmp";

	return strRet;
}

void CMedBoxCtrl::SetMaxCapacity(int n)
{
	m_nMaxCapicity = n;
}

void CMedBoxCtrl::SetCurNumber(int n)
{
	m_nCurNumber = n;
}

int CMedBoxCtrl::GetCapacityStatus()
{
	//先hardcode再这里
	if (m_nCurNumber == 0)
	{
		return MEDBOX_CAPSTATUS_EMPTY;
	}
	else if (m_nCurNumber > 0 && m_nCurNumber < 5)
	{
		return MEDBOX_CAPSTATUS_DANGER;
	}
	else if (m_nCurNumber > 4 && m_nCurNumber < 10)
	{
		return MEDBOX_CAPSTATUS_NORMAL;
	}
	else if(m_nCurNumber == 10)
	{
		return MEDBOX_CAPSTATUS_FULL;
	}
	else
	{
		return MEDBOX_CAPSTATUS_DISABLE;
	}
}

void CMedBoxCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CStatic::OnLButtonUp(nFlags, point);
}

void CMedBoxCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CStatic::OnLButtonDown(nFlags, point);
}
