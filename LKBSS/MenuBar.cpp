// MenuBarXP.cpp : implementation file
//

#include "stdafx.h"
#include "MenuBar.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMenuBar

CMenuBar::CMenuBar()
{
	m_pMenu = NULL;
	m_bTrack = FALSE;
	m_nItemCount = 0;
	m_nPressed = 0;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
}

CMenuBar::~CMenuBar()
{
	if (m_pMenu)
	{
		delete m_pMenu;
	}
}


BEGIN_MESSAGE_MAP(CMenuBar, CToolBarCtrl)
	//{{AFX_MSG_MAP(CMenuBar)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MEASUREITEM()
//	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MB_POPUPMENU, OnPopupMenu)
	ON_WM_EXITMENULOOP()
	ON_WM_ENTERMENULOOP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuBar message handlers

/////////////////////////////////////////////////////////////////////////////
//Load from a menu, can be a CMenuXP instance or just a CMenu
//the pMenu object must be constructed on the heap, it will
//automatically be destoyed when the MenuBar being destroyed
BOOL CMenuBar::LoadMenu(CMenu *pMenu)
{
	if (!m_hWnd)
		return FALSE;

	if (!pMenu)
		return FALSE;

	if (m_pMenu)
		delete m_pMenu;

	m_pMenu = pMenu;

	TBBUTTON		tbb; 
	int				i,nStr;
	BOOL			bRet = FALSE;
	char			szText[_MAX_PATH];

	memset(&tbb, 0, sizeof(TBBUTTON));

	tbb.fsState = TBSTATE_ENABLED;
	tbb.fsStyle = TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE;

	m_nItemCount = m_pMenu->GetMenuItemCount();//Count the main menu items
	SetBitmapSize(CSize(0, 0));//no icons for the main menu

	m_ilIcons.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1);

	for(i = 0; i < m_nItemCount; i++)
	{
		memset(szText, 0, _MAX_PATH);

		MENUITEMINFO	info;
		memset(&info, 0, sizeof(MENUITEMINFO));
		info.cbSize = sizeof(MENUITEMINFO);
		info.fMask = MIIM_DATA | MIIM_TYPE;
		m_pMenu->GetMenuItemInfo(i, &info, TRUE);
		
// 		CMenuItem *pData = (CMenuItem *)info.dwItemData;
// 		if (pData && (info.fType & MFT_OWNERDRAW))
// 		{
// 			strcpy(szText, pData->m_strText);
// 			if (pData->m_hIcon)
// 			{
// 				tbb.iBitmap = m_ilIcons.Add(pData->m_hIcon);
// 			}
// 			else
// 				tbb.iBitmap = -1;
// 		}
//		else
//		{
			m_pMenu->GetMenuString(i, szText, 80, MF_BYPOSITION);
//		}

		nStr = AddStrings(szText);

		tbb.dwData = NULL;
		tbb.iString = nStr;
		tbb.idCommand = i;

		bRet = AddButtons(1, &tbb);

		if(!bRet)
			return FALSE;
	}

	if (m_ilIcons.GetImageCount() > 0)
		SetImageList(&m_ilIcons);

	return bRet;
}

////////////////////////////////////////////////////////////////////////////////
CMenuBar*		g_pMenuBar	= NULL;
HHOOK				g_hMsgHook	= NULL;
////////////////////////////////////////////////////////////////////////////////
//	The hook, used to process message when menu is visible
LRESULT CMenuBar::MenuInputFilter(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSG* pMsg = (MSG*)lParam;

	if(!g_pMenuBar || nCode!=MSGF_MENU)
		return CallNextHookEx(g_hMsgHook,nCode,wParam,lParam);
	if(g_pMenuBar->OnMenuInput(pMsg))
		return TRUE;
	else
		return CallNextHookEx(g_hMsgHook,nCode,wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
// Show popupmenu
void CMenuBar::TrackPopup()
{
	CMenu	*pSubMenu = m_pMenu->GetSubMenu(m_nPressed);
	if(pSubMenu == NULL)return;

	m_bTrack = TRUE;
	PressButton(m_nPressed,TRUE);


	//Get the rect of the button
	CRect rc;
	GetItemRect(m_nPressed,&rc);
	ClientToScreen(&rc);


	//get the in-screen part of the button
	CRect rcScreen;
	GetDesktopWindow()->GetWindowRect(rcScreen);
	rc.IntersectRect(rc, rcScreen);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(tpm);
	tpm.rcExclude = rc;

	//Install the hook
	g_pMenuBar = this;
	g_hMsgHook = SetWindowsHookEx(WH_MSGFILTER,
			MenuInputFilter, NULL, GetCurrentThreadId());

	//Show menu
	TrackPopupMenuEx(pSubMenu->GetSafeHmenu(),
				TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON|TPM_VERTICAL,
				rc.left, rc.bottom, m_hWnd, &tpm);

	PressButton(m_nPressed,FALSE);
	UnhookWindowsHookEx(g_hMsgHook);//Uninstall the hook

	g_hMsgHook = NULL;
	g_pMenuBar = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
//	Actually process messages, called by the hook
BOOL CMenuBar::OnMenuInput(MSG* pMsg)
{
	BOOL bResult = FALSE;

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(pMsg->lParam);
			pt.y = HIWORD(pMsg->lParam);
			ScreenToClient(&pt);
			if(m_ptMouse.x == pt.x && m_ptMouse.y == pt.y)
				return TRUE;
		
			m_ptMouse.x = pt.x;
			m_ptMouse.y = pt.y;

			int nTest = HitTest(&pt);

			if(nTest>=0 && nTest<m_nItemCount && nTest != m_nPressed)
			{
				PressButton(m_nPressed,FALSE);
				SendMessage(WM_CANCELMODE,0,0);
				m_nPressed = nTest;
				PostMessage(MB_POPUPMENU,0,0);
				bResult = TRUE;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(pMsg->lParam);
			pt.y = HIWORD(pMsg->lParam);
			ScreenToClient(&pt);

			int nTest = HitTest(&pt);

			if(nTest<0)
				m_bTrack = FALSE;
			else if(nTest == m_nPressed)
			{
				m_bTrack = FALSE;
				PostMessage(WM_CANCELMODE,0,0);
				bResult = TRUE;
			}
		}
		break;
	case WM_KEYDOWN:
		{
			TCHAR vkey = pMsg->wParam;
			if(vkey == VK_LEFT)
			{
				PressButton(m_nPressed,FALSE);
				m_nPressed --;
				PostMessage(WM_CANCELMODE,0,0);
				PostMessage(MB_POPUPMENU,0,0);
				bResult = TRUE;
			}
			else if(vkey == VK_RIGHT)
			{
				PressButton(m_nPressed,FALSE);
				m_nPressed ++;
				PostMessage(WM_CANCELMODE,0,0);
				PostMessage(MB_POPUPMENU,0,0);
				bResult = TRUE;
			}
			else if (vkey == VK_ESCAPE)
			{
				PostMessage(WM_CANCELMODE,0,0);
				m_bTrack = FALSE;
				bResult = TRUE;
			}
		}
		break;
	case WM_MENUSELECT:
		{
			GetOwner()->SendMessage(WM_MENUSELECT, pMsg->wParam, pMsg->lParam);
			bResult = TRUE;
			break;
		}
	default:
		break;
	}

	return bResult;
}

void CMenuBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int	nTest = HitTest(&point);
	if(nTest<0 || nTest>=m_nItemCount)
		return;

	m_nPressed = nTest;
	TrackPopup();
	
}

void CMenuBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	return;
}

LRESULT CMenuBar::OnPopupMenu(WPARAM wParam, LPARAM lParam)
{
	if(m_nPressed<0)
		m_nPressed = m_nItemCount - 1;
	if(m_nPressed>=m_nItemCount)
		m_nPressed = 0;
	TrackPopup();

	return 0;
}


void CMenuBar::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDCtl == 0 && m_pMenu)
	{
		m_pMenu->MeasureItem(lpMeasureItemStruct);
	}
	
}

void CMenuBar::OnExitMenuLoop(BOOL bIsTrackPopupMenu)
{
	GetOwner()->SendMessage(WM_EXITMENULOOP, (WPARAM)bIsTrackPopupMenu);
}

void CMenuBar::OnEnterMenuLoop(BOOL bIsTrackPopupMenu)
{
	GetOwner()->SendMessage(WM_ENTERMENULOOP, (WPARAM)bIsTrackPopupMenu);
}


BOOL CMenuBar::OpenMenu(UINT nChar)
{
	int		nCount = (int)m_pMenu->GetMenuItemCount();
	char	szText[80];

	for(int i=0; i<m_nItemCount; i++)
	{
		MENUITEMINFO	info;
		memset(&info, 0, sizeof(MENUITEMINFO));
		info.cbSize = sizeof(MENUITEMINFO);
		info.fMask = MIIM_DATA | MIIM_TYPE;
		m_pMenu->GetMenuItemInfo(i, &info, TRUE);
		
// 		CMenuXPItem *pData = (CMenuXPItem *)info.dwItemData;
// 		if (pData && (info.fType & MFT_OWNERDRAW))
// 		{
// 			strcpy(szText, pData->m_strText);
// 		}
// 		else
// 		{
			m_pMenu->GetMenuString(i, szText, 80, MF_BYPOSITION);
//		}

		CString	text = szText;
		int iAmpersand = text.Find('&');
		if (iAmpersand >=0 && toupper(nChar)==toupper(text[iAmpersand+1]))
		{
			m_nPressed = i;
			PostMessage(MB_POPUPMENU,0,0);
			return TRUE;
		}
	}
	return FALSE;
}

void CMenuBar::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CToolBarCtrl::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	// TODO: Add your message handler code here
	GetOwner()->SendMessage(WM_INITMENUPOPUP, (WPARAM)pPopupMenu->GetSafeHmenu(), MAKELONG(nIndex, bSysMenu));
}

BOOL CMenuBar::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//	return CToolBarCtrl::OnEraseBkgnd(pDC);

	CRect rect;
	GetClientRect(rect);
	CString strPath = Util::GetDialogResPath() + "\\bg_menu.bmp";
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	Util::Draw9Grid(pDC, rect, hBitmap);
	DeleteObject(hBitmap);
	hBitmap = NULL;
	return TRUE;
}


