#pragma once


/////////////////////////////////////////////////////////////////////////////
// CRCTreeCtrl window
typedef struct tagRCTVITEM{    
    HTREEITEM hItem;				
    LPARAM    lParam;
	DWORD dwStyleExFlag;				// 节点的风格
	COLORREF crText;					// 字体颜色
  	COLORREF clrBk;						// 背景颜色
	COLORREF clrHlText;					// 选中的时候字体颜色
	COLORREF clrHlBk;					// 选中时候的背景颜色
} RCTVITEM;


class CRCTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CRCTreeCtrl();
	virtual ~CRCTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCTreeCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void EnableClipCursor(BOOL bEnableClipCursor = TRUE);
	void SetBorderColor(COLORREF clr);
	void SetClickToExpand(BOOL bClickToExpand){ m_bClickToExpand = bClickToExpand; }
	COLORREF SetFolderOverTextColor(COLORREF clr);
	int SetTextIndent(int nIndent);
	POINT SetImageOffset(int cx, int cy);
	HTREEITEM GetHotItem();
	void SetHotItem(HTREEITEM hItem);
	void SetSelTextColor(COLORREF clr);
	void SetUnExpendedImage(int nImage);
	void SetExpendedImage(int nImage);
	void SetFolderTextColor(COLORREF clr);
	void SetFolderBkColor(COLORREF clr);
	void SetSelBkColor(COLORREF clr);

	// Generated message map functions
protected:
	void DrawItem(NMLVCUSTOMDRAW* pLVCD);
	void RedrawItem(HTREEITEM hItem);
	//{{AFX_MSG(CRCTreeCtrl)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	BOOL m_bClipCursor;
	BOOL m_bEnableClipCursor;
	BOOL		m_bClickToExpand;
	HTREEITEM m_hOverItem;
	HTREEITEM m_hHotItem;	// 当前选中项
	COLORREF m_clrSelBk;	// 选中项目的背景色
	COLORREF m_clrSelText;	// 选中项目的字体色
	COLORREF m_clrFolderBk;	// Folder 的背景色
	COLORREF m_clrFolderText; // Folder 的字体颜色
	COLORREF m_clrFolderOverText;
	COLORREF m_clrBorder;	// 如果控件有边框的话，用此颜色来画边框

	int m_nFolderExpended;
	int m_nFolderUnExpended;
	POINT m_ptImgOffset;	
	BOOL	m_bTracking;
	HTREEITEM	m_hPressedItem;
	int m_nTextIndent;
	CBrush m_brTitle;	
};

