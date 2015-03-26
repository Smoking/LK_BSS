#if !defined(AFX_IMAGETABWND_H__B82892D8_3E79_4C59_B540_699F8D6F8333__INCLUDED_)
#define AFX_IMAGETABWND_H__B82892D8_3E79_4C59_B540_699F8D6F8333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageTabWnd.h : header file
//
#include <afxtempl.h>

#define MAX_DIALOG			3
#define TAB_HEIGHT			0


/////////////////////////////////////////////////////////////////////////////
// CImageTabWnd window

class CImageTabWnd : public CWnd
{
// Construction
public:
	CImageTabWnd();

// Attributes
public:
    int		m_nCurrentIndex;
	HWND hwnd;
	bool showFalg;
// Operations
public:
	void SetTabCount(int nCount);
	int  GetTabCount() { return m_nTabCount ;}
	int	 GetCurrentTab() { return m_nCurrentIndex; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageTabWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetButtonRect(int nIndex, CRect rc);
	void setButtonCount(int count);
	void ShowDialog();
	void SetToolTip(int nIndex, CString strTips);
	void SetTabWnd(int nIndex, CDialog *pDlg);
	void SetTabRect(int nIndex, CRect rc);
	void SetTabBkImage(UINT nID);
	void SetTabBkImage(CString strName);
	void SetTabImage(int nIndex, UINT nID);
	void SetTabImage(int nIndex, CString strName);
	virtual ~CImageTabWnd();

	// Generated message map functions
protected:
	CBitmap *m_pBitmapTab;
	CRect	*m_pRectTab;
	CRect	*m_pRectButton;
	CBitmap m_BmpTabBk;
	CSize	m_szBitmapBk;

	int		m_nTabCount;
	

	CRect	m_rcDialog;
	CDialog	*m_pTabDialog[MAX_DIALOG];

	CToolTipCtrl m_ToolTip;
	//{{AFX_MSG(CImageTabWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateTabWnd();
	void SetImageSize(CBitmap *pBitmap, CSize &sz);
	void DrawPosImage(CBitmap *pBitmap, CDC *pDC, CPoint pt);
	void DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	void SetImage(CBitmap &bitmap, UINT nID);
	void SetImage(CBitmap &bitmap, CString strName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGETABWND_H__B82892D8_3E79_4C59_B540_699F8D6F8333__INCLUDED_)
