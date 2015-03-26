#if !defined(AFX_RCDIALOG_H__211051F9_5E39_4CF7_B2FF_8129F1822DF1__INCLUDED_)
#define AFX_RCDIALOG_H__211051F9_5E39_4CF7_B2FF_8129F1822DF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRCDialog dialog

#include "MenuBar.h"
#define  CRECTMONITOR  CRect(117,281,455,430)
#define  CRECTMEDMANG  CRect(560,280,900,430)
#define  CRECTDISMED   CRect(117,520,455,668)
#define  CRECTADDMED   CRect(560,520,900,668)

class CRCDialog : public CDialog
{
	DECLARE_DYNAMIC(CRCDialog)
public:
	CRCDialog(UINT nIDTemplate, CWnd * pParentWnd = NULL,int flag = 0);
	virtual ~CRCDialog();

public:
	void SetWindowText(CString strText);	//设置窗口标题名
	void GetClientRect(CRect* pRect);		//重载客户区域矩形
	void SetMenu(CMenu* pMenu);				//设置窗口菜单
	void ShowBottomLogo(BOOL bShow);		//是否显示底部logo
	void SetBgPath(CString strPath);		//设置背景图片路径

protected:
	void DrawFrame(CDC *pDC);  

	void DrawCaption(CDC *pDC);		//绘制标题栏
	void DrawIconAndText(CDC *pDC); //绘制标题栏图标和文字

	void DrawMenu(CDC *pDC); //绘制菜单区域
	void DrawMenuBG(CDC *pDC); //绘制菜单区域背景

	void DrawClientBG(CDC *pDC); //绘制客户区背景

	void DrawBottom(CDC *pDC);      //绘制边框底部
	void DrawBottomLogo(CDC *pDC, CRect rect); //绘制LOGO图标

	void DrawSysButton(CDC *pDC);	//绘制系统按钮
	void DrawCloseBtn(CDC *pDC, int nState); //绘制关闭按钮
	void DrawMinBtn(CDC *pDC, int nState);

	CRect GetCaptionRect();			//得到标题栏矩形
	CRect GetBottomRect();			//得到底部区域矩形
	CRect GetMenuRect();			//得到菜单区域

	void CreateMenu();    //创建菜单

	void showMain(CPoint point);
	void showAssist(CPoint point);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:	// msg
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nHitTest, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

protected:
	CRect			m_rcSysBtnMin;             //最小化按钮位置rect
	CRect			m_rcSysBtnClose;             //关闭按钮位置rect
	int				m_nCloseBtnState;
	int			    m_nMinBtnState;
	CString			m_strCaption;
	CMenu			*m_pMenu;
	CMenuBar		m_menuBar;

	BOOL			m_bShowBottomLogo;			 //是否显示底部logo		
	int             showflag;  
	CString			m_strBgPath;
	static HWND     hWnd;
	enum 
	{
		NORMAL_STATE = 0,	//普通状态
		HOVER_STATE,		//Mouse Over状态
		PRESSED_STATE,		//Press down状态
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCDIALOG_H__211051F9_5E39_4CF7_B2FF_8129F1822DF1__INCLUDED_)
