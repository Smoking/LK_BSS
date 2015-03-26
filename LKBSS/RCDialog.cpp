// RCDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RCDialog.h"
#include "Util.h"
#include "Cximage/ximage.h" 

#define CAPTION_HEIGHT		30   //标题栏高度
#define BOTTOM_HEIGHT		73   //底部区域高度
#define LOGO_WIDTH			120  //LOGO的宽度
#define CAPTION_ICON_SIZE	28   //标题栏ICON的尺寸
#define MENU_HEIGHT			20   //菜单背景高度

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 
#pragma comment(lib, "imagelib/png.lib")  
#pragma comment(lib, "imagelib/jpeg.lib")  
#pragma comment(lib, "imagelib/cximage.lib")  

HWND  CRCDialog::hWnd=NULL;
IMPLEMENT_DYNAMIC(CRCDialog, CDialog)

CRCDialog::CRCDialog(UINT nIDTemplate, CWnd * pParentWnd,int flag )
	: CDialog(nIDTemplate, pParentWnd)
{
	showflag = flag;
	m_nCloseBtnState = NORMAL_STATE;
	m_nMinBtnState   = NORMAL_STATE;
	m_pMenu = NULL;
	m_strCaption.LoadString(IDS_Caption);
	m_bShowBottomLogo = FALSE;
	m_strBgPath = Util::GetDialogResPath() + "\\bg_noraml.bmp";
}

CRCDialog::~CRCDialog()
{
}

BEGIN_MESSAGE_MAP(CRCDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CRCDialog::SetBgPath(CString strPath)
{
	m_strBgPath = strPath;
}

void CRCDialog::SetWindowText(CString strText)
{
	m_strCaption = strText;
	
	Invalidate(FALSE);
}

BOOL CRCDialog::OnInitDialog()
{
	return CDialog::OnInitDialog();
}

BOOL CRCDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//屏蔽回车，退出按钮
	/*if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))    
		return TRUE;   */
    if(pMsg->message == WM_KEYDOWN ) 
    {
        if (pMsg->wParam == VK_ESCAPE)
        {
            return TRUE;
        }
        if (pMsg->wParam == VK_RETURN)
        {
            pMsg->wParam = VK_TAB; 
        }
    }
   
	return CDialog::PreTranslateMessage(pMsg);
}

void CRCDialog::CreateMenu()
{
	//Init MenuBar
	if (m_pMenu)
	{
		CRect rect = GetMenuRect();
		m_menuBar.Create(WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | CCS_NODIVIDER | CCS_NORESIZE, 
			rect, this , 20);
		m_menuBar.LoadMenu(m_pMenu);
	}
}

void CRCDialog::SetMenu(CMenu*  pMenu)
{
	if (m_pMenu)
	{
		delete m_pMenu;
		m_pMenu = pMenu;
	}
	else
	{
		m_pMenu = pMenu;
		CreateMenu();
	}
}

void CRCDialog::ShowBottomLogo(BOOL bShow)
{
	m_bShowBottomLogo = bShow;
}

void CRCDialog::OnPaint()
{
	CPaintDC dc(this); 
	DrawFrame(&dc);
}

void CRCDialog::DrawFrame(CDC *pDC)
{
	if (GetSafeHwnd() != NULL)
	{
		DrawCaption(pDC);		//绘制标题栏

		if (m_pMenu)
		{
			DrawMenu(pDC);			//绘制菜单区域
		}
		
		DrawSysButton(pDC);		//绘制系统按钮

		DrawBottom(pDC);		//绘制底部区域
	}
}

CRect CRCDialog::GetCaptionRect()
{
	CRect rcRet, rcWnd;
	GetWindowRect(rcWnd);
	
	rcRet = rcWnd;
	rcRet.bottom = rcWnd.top + CAPTION_HEIGHT;
	rcRet.OffsetRect(-rcWnd.left, -rcWnd.top);

	return rcRet;
}

CRect CRCDialog::GetBottomRect()
{
	CRect rcRet, rcWnd;
	GetWindowRect(rcWnd);
	
	rcRet = rcWnd;
	rcRet.top = rcWnd.bottom - BOTTOM_HEIGHT;
	rcRet.OffsetRect(-rcWnd.left, -rcWnd.top);
	
	return rcRet;
}

CRect CRCDialog::GetMenuRect()
{
	CRect rcRet, rcCaption;
	rcCaption = GetCaptionRect();

	rcRet = rcCaption;
	rcRet.top = rcCaption.bottom;

	if (m_pMenu)
	{
		rcRet.bottom = rcRet.top + MENU_HEIGHT;
	}
	else
	{
		rcRet.bottom = rcRet.top;
	}
	
	return rcRet;
}

void CRCDialog::GetClientRect(CRect* pRect)
{
	CRect rcRet, rcMenu, rcBottom;
	rcMenu = GetMenuRect();
	rcBottom = GetBottomRect();

	rcRet = rcMenu;
	rcRet.top = rcMenu.bottom;
	rcRet.bottom = rcBottom.top;

	*pRect = rcRet;
}

void CRCDialog::DrawCaption(CDC *pDC)
{
	if (!pDC)
		return;

	CRect rcTop = GetCaptionRect();

	//绘制标题栏（顶部）区域
	CString strPath = Util::GetDialogResPath() + "\\BorderTop.bmp";
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	Util::Draw9Grid(pDC, rcTop, hBitmap);
	DeleteObject(hBitmap);
	hBitmap = NULL;

	//绘制图标和文字
	DrawIconAndText(pDC);
}

void CRCDialog::DrawIconAndText(CDC *pDC)
{
	if (!pDC)
		return;
	
	CRect rcTop = GetCaptionRect();

	//绘制图标
	CRect rcIcon;
	rcIcon.left = rcTop.left + 0;
	rcIcon.top = rcTop.top + rcTop.Height()/2 - CAPTION_ICON_SIZE/2;
	rcIcon.right = rcIcon.left + CAPTION_ICON_SIZE;
	rcIcon.bottom = rcIcon.top + CAPTION_ICON_SIZE;

	CString strIcon = Util::GetDialogResPath() + "\\logo_men.png";

	CxImage img;
	img.Load(strIcon,CXIMAGE_FORMAT_PNG);
	img.Draw(pDC->GetSafeHdc(),rcIcon,NULL,TRUE);


	/*HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strIcon, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	CBitmap *bitmap = NULL, *pOldBitmap = NULL;
	bitmap = CBitmap::FromHandle(hBitmap);
	
	CDC		MemDC;
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(bitmap);
	
	pDC->BitBlt(rcIcon.left, rcIcon.top, rcIcon.Width(), rcIcon.Height(), &MemDC, 0, 0, SRCCOPY);
	
	DeleteObject(hBitmap);
	MemDC.SelectObject(pOldBitmap);
	bitmap->Detach();*/

	//绘制文字
	if (!m_strCaption.IsEmpty())
	{
		CFont font, *pOldFont;
		font.CreatePointFont(100, "Fixedsys", pDC);
		pOldFont = pDC->SelectObject(&font);
		TEXTMETRIC textInfo;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->GetTextMetrics(&textInfo);
		pDC->TextOut(rcIcon.right + 4 , rcTop.top + rcTop.Height()/2 - textInfo.tmHeight/2, m_strCaption);
		DeleteObject(&font);
		pDC->SelectObject(pOldFont);
	}
}

void CRCDialog::DrawMenu(CDC *pDC)
{
	if (!pDC)
		return;

	//先绘制背景
	DrawMenuBG(pDC);

}

void CRCDialog::DrawMenuBG(CDC *pDC)
{
	if (!pDC)
		return;
	CRect rcMenu = GetMenuRect();

	CString strPath = Util::GetDialogResPath() + "\\bg_menu.bmp";
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	Util::xTileRgnWithBmp(pDC, rcMenu, hBitmap);
	DeleteObject(hBitmap);
	hBitmap = NULL;
}

void CRCDialog::DrawClientBG(CDC *pDC)
{
	if (!pDC)
		return;

	CRect rect;
	GetClientRect(&rect);

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, m_strBgPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	CBitmap *pBitmap = NULL, *pOldBitmap = NULL;
	pBitmap = CBitmap::FromHandle(hBitmap);
	BITMAP bmp;
	pBitmap->GetBitmap(&bmp);
	
	CDC		MemDC;
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(pBitmap);

	if (bmp.bmWidth > rect.Width() && bmp.bmHeight > rect.Height())
	{
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	}
	else
	{
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}
	
	DeleteObject(hBitmap);
	MemDC.SelectObject(pOldBitmap);
	pBitmap->Detach();
}

void CRCDialog::DrawBottom(CDC *pDC)
{
	if (m_bShowBottomLogo || !pDC)
		return;
	
	CRect rect = GetBottomRect();
	
	//绘制底部区域
	CString strPath = Util::GetDialogResPath() + "\\BorderBottom.bmp";
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	Util::xTileRgnWithBmp(pDC, rect, hBitmap);
	DeleteObject(hBitmap);
	hBitmap = NULL;

	//绘制Logo
    if (m_bShowBottomLogo)
	{
		CRect rcLogo = rect;
		rcLogo.right = rcLogo.left + LOGO_WIDTH;
		DrawBottomLogo(pDC, rcLogo);
	}
}

void CRCDialog::DrawBottomLogo(CDC *pDC, CRect rect)
{
	if (!pDC)
		return;

	CString strLogoPath = Util::GetDialogResPath() + "\\Logo.bmp";
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strLogoPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

	CBitmap *bitmap = NULL, *pOldBitmap = NULL;
	bitmap = CBitmap::FromHandle(hBitmap);

	CDC		MemDC;
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(bitmap);

	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);

	DeleteObject(hBitmap);
	MemDC.SelectObject(pOldBitmap);
	bitmap->Detach();
}

void CRCDialog::DrawSysButton(CDC *pDC)
{
	if (!pDC)
		return;
	if(showflag==1)
	{
	    DrawMinBtn(pDC,m_nMinBtnState);
	}
	DrawCloseBtn(pDC, m_nCloseBtnState);
}

void CRCDialog::DrawMinBtn(CDC *pDC, int nState)
{
	if (!pDC)
		return;

	CDC MemDC;
	BITMAP bmp;
	CString strPath = Util::GetDialogResPath();
	CBitmap *pBitmap = NULL, *pOldBitmap = NULL;

	//获取关闭按钮的图片资源
	if (HOVER_STATE == nState)  //MouseOver状态
	{
		strPath = strPath + "\\" + "MinButton_Hover.bmp";
	}
	else if (PRESSED_STATE == nState)  //Press Down状态
	{
		strPath = strPath + "\\" + "MinButton_Down.bmp";
	}
	else  //其他状态
	{
		strPath = strPath + "\\" + "MinButton_Normal.bmp";
	}
	
	pBitmap = CBitmap::FromHandle( (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE |
		LR_DEFAULTCOLOR | LR_DEFAULTSIZE) );
	if (!pBitmap)
	{
		return ;
	}
	pBitmap->GetBitmap(&bmp);
	
	//计算关闭按钮的位置
	CRect rcCaption = GetCaptionRect();  
	m_rcSysBtnMin.top = rcCaption.top + 4;
	m_rcSysBtnMin.right = rcCaption.right - 50;
	m_rcSysBtnMin.bottom = m_rcSysBtnMin.top + bmp.bmHeight;
	m_rcSysBtnMin.left = m_rcSysBtnMin.right - bmp.bmWidth;
	
	//贴图
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(pBitmap);
	pDC->BitBlt(m_rcSysBtnMin.left, m_rcSysBtnMin.top, m_rcSysBtnMin.Width(), m_rcSysBtnMin.Height(),
		&MemDC, 0, 0, SRCCOPY);
	pDC->SelectObject(pOldBitmap);
	DeleteObject(pBitmap->m_hObject);
	pBitmap->Detach();
}


void CRCDialog::DrawCloseBtn(CDC *pDC, int nState)
{
	if (!pDC)
		return;

	CDC MemDC;
	BITMAP bmp;
	CString strPath = Util::GetDialogResPath();
	CBitmap *pBitmap = NULL, *pOldBitmap = NULL;
	
	//获取关闭按钮的图片资源
	if (HOVER_STATE == nState)  //MouseOver状态
	{
		strPath = strPath + "\\" + "CloseButton_Hover.bmp";
	}
	else if (PRESSED_STATE == nState)  //Press Down状态
	{
		strPath = strPath + "\\" + "CloseButton_Down.bmp";
	}
	else  //其他状态
	{
		strPath = strPath + "\\" + "CloseButton_Normal.bmp";
	}
	
	pBitmap = CBitmap::FromHandle( (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE |
		LR_DEFAULTCOLOR | LR_DEFAULTSIZE) );
	if (!pBitmap)
	{
		return ;
	}
	pBitmap->GetBitmap(&bmp);
	
	//计算关闭按钮的位置
	CRect rcCaption = GetCaptionRect();  
	m_rcSysBtnClose.top = rcCaption.top + 4;
	m_rcSysBtnClose.right = rcCaption.right - 7;
	m_rcSysBtnClose.bottom = m_rcSysBtnClose.top + bmp.bmHeight;
	m_rcSysBtnClose.left = m_rcSysBtnClose.right - bmp.bmWidth;
	
	//贴图
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(pBitmap);
	pDC->BitBlt(m_rcSysBtnClose.left, m_rcSysBtnClose.top, m_rcSysBtnClose.Width(), m_rcSysBtnClose.Height(),
		&MemDC, 0, 0, SRCCOPY);
	pDC->SelectObject(pOldBitmap);
	DeleteObject(pBitmap->m_hObject);
	pBitmap->Detach();
}

void CRCDialog::OnMouseMove(UINT nHitTest, CPoint point)
{
	CRect rcCaption = GetCaptionRect();  //不在标题栏区域，直接返回
	if (!rcCaption.PtInRect(point))
	{
		if (m_nCloseBtnState != NORMAL_STATE)
		{
			m_nCloseBtnState = NORMAL_STATE;
			InvalidateRect(m_rcSysBtnClose, FALSE);
		}
		if (m_nMinBtnState != NORMAL_STATE)
		{
			m_nMinBtnState = NORMAL_STATE;
			InvalidateRect(m_rcSysBtnMin, FALSE);
		}
		CDialog::OnMouseMove(nHitTest, point);
		return;
	}
	
	if (m_rcSysBtnClose.PtInRect(point))  //是否在关闭按钮区域
	{
		if (m_nCloseBtnState != HOVER_STATE)
		{
			m_nCloseBtnState = HOVER_STATE;
			InvalidateRect(m_rcSysBtnClose, FALSE);
		}
		
	}
	else
	{
		if (m_nCloseBtnState != NORMAL_STATE)
		{
			m_nCloseBtnState = NORMAL_STATE;
			InvalidateRect(m_rcSysBtnClose, FALSE);
		}
	}

	if (m_rcSysBtnMin.PtInRect(point))  //是否在最小化按钮区域
	{
		if (m_nMinBtnState != HOVER_STATE)
		{
			m_nMinBtnState = HOVER_STATE;
			InvalidateRect(m_rcSysBtnMin, FALSE);
		}	
	}
	else
	{
		if (m_nMinBtnState != NORMAL_STATE)
		{
			m_nMinBtnState = NORMAL_STATE;
			InvalidateRect(m_rcSysBtnMin, FALSE);
		}
	}

	CDialog::OnMouseMove(nHitTest, point);
}

void CRCDialog::OnLButtonDown(UINT nHitTest, CPoint point)
{
	CRect rcCaption = GetCaptionRect();  //不在标题栏区域，直接返回
	if (!rcCaption.PtInRect(point))
	{
		CDialog::OnMouseMove(nHitTest, point);
		return;
	}
	
	if (m_rcSysBtnClose.PtInRect(point))
	{
		if (m_nCloseBtnState != PRESSED_STATE)
		{
			m_nCloseBtnState = PRESSED_STATE;
			InvalidateRect(m_rcSysBtnClose, FALSE);
		}
	}
	else if (m_rcSysBtnMin.PtInRect(point))
	{
		if (m_nMinBtnState != PRESSED_STATE)
		{
			m_nMinBtnState = PRESSED_STATE;
			InvalidateRect(m_rcSysBtnMin, FALSE);
		}
	}
	else
	{
		if (m_nCloseBtnState != NORMAL_STATE)
		{
			m_nCloseBtnState = NORMAL_STATE;
			InvalidateRect(m_rcSysBtnClose, FALSE);
		}
		if (m_nMinBtnState != NORMAL_STATE)
		{
			m_nMinBtnState = NORMAL_STATE;
			InvalidateRect(m_rcSysBtnMin, FALSE);
		}
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y)); 
		CDialog::OnLButtonDown(nHitTest, point);

	}
}

void CRCDialog::OnLButtonUp(UINT nHitTest, CPoint point)
{
	if(showflag == 0)
	{	
		CRect rcCaption = GetCaptionRect();  //不在标题栏区域，直接返回
		if (!rcCaption.PtInRect(point))
		{
			CDialog::OnMouseMove(nHitTest, point);
			return;
		}
	}
	else if(showflag == 1)
	{
		showMain(point);
	}
	else if(showflag == 2)
	{
		showAssist(point);
	}
	
	if (m_rcSysBtnClose.PtInRect(point))
	{
		PostMessage(WM_CLOSE);
		 
	}
	else if(m_rcSysBtnMin.PtInRect(point))
	{
		::SendMessage(this->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else
	{
		CDialog::OnLButtonUp(nHitTest, point);
	}
}

void CRCDialog::showMain(CPoint point)
{
	if(CRECTMONITOR.PtInRect(point))
	{
		::PostMessage(m_hWnd,WM_BUTTONDOWN,1,1);
	}
	else if(CRECTMEDMANG.PtInRect(point))
	{
		::PostMessage(m_hWnd,WM_BUTTONDOWN,1,2);
	}
	else if(CRECTDISMED.PtInRect(point))
	{
		::PostMessage(m_hWnd,WM_BUTTONDOWN,1,3);
	}
	else if(CRECTADDMED.PtInRect(point))
	{
		::PostMessage(m_hWnd,WM_BUTTONDOWN,1,4);
	}
}


void CRCDialog::showAssist(CPoint point)
{
	if(CRECTMONITOR.PtInRect(point))
	{
		::PostMessage(hWnd,WM_BUTTONDOWN,2,1);
	}
	else if(CRECTMEDMANG.PtInRect(point))
	{
		::PostMessage(hWnd,WM_BUTTONDOWN,2,2);
	}
	else if(CRECTDISMED.PtInRect(point))
	{
		::PostMessage(hWnd,WM_BUTTONDOWN,2,3);
	}
	else if(CRECTADDMED.PtInRect(point))
	{
		::PostMessage(hWnd,WM_BUTTONDOWN,2,4);
	}
	/*else if(CRECTBACK.PtInRect(point))
	{
		PostMessage(WM_CLOSE);
	}*/
}

BOOL CRCDialog::OnEraseBkgnd(CDC* pDC)
{
	DrawClientBG(pDC);		//绘制客户区背景
	return TRUE;
}

HBRUSH CRCDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC)  //针对所有的标签
	{
		pDC->SetBkMode( TRANSPARENT );
		pDC->SetTextColor(RGB(255, 255, 255));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
