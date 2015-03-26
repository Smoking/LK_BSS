#if !defined(AFX_MenuBarXP_H__C492FD05_285B_11D5_99BD_5254AB339987__INCLUDED_)
#define AFX_MenuBarXP_H__C492FD05_285B_11D5_99BD_5254AB339987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuBarXP.h : header file
//

#define	MB_POPUPMENU	WM_USER + 1101

/////////////////////////////////////////////////////////////////////////////
// CMenuBar window 

class CMenuBar : public CToolBarCtrl
{
// Construction
public:
	CMenuBar();

// Attributes
protected:
	CMenu*		m_pMenu;
	int			m_nItemCount;
	int			m_nPressed;
	POINT		m_ptMouse;
	BOOL		m_bTrack;

	CImageList	m_ilIcons;

//override

// Operations
public:
	BOOL		LoadMenu(CMenu *pMenu);
	void		TrackPopup();
	BOOL		OnMenuInput(MSG* pMsg);
	static LRESULT CALLBACK MenuInputFilter(int nCode, WPARAM wParam, LPARAM lParam);

	BOOL		OpenMenu(UINT nChar);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
//	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG
	afx_msg LRESULT OnPopupMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnExitMenuLoop(BOOL bTrackPopupMenu);
	afx_msg void	OnEnterMenuLoop(BOOL bTrackPopupMenu);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MenuBarXP_H__C492FD05_285B_11D5_99BD_5254AB339987__INCLUDED_)
