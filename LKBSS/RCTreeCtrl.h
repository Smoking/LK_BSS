#pragma once


/////////////////////////////////////////////////////////////////////////////
// CRCTreeCtrl window
typedef struct tagRCTVITEM{    
    HTREEITEM hItem;				
    LPARAM    lParam;
	DWORD dwStyleExFlag;				// �ڵ�ķ��
	COLORREF crText;					// ������ɫ
  	COLORREF clrBk;						// ������ɫ
	COLORREF clrHlText;					// ѡ�е�ʱ��������ɫ
	COLORREF clrHlBk;					// ѡ��ʱ��ı�����ɫ
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
	HTREEITEM m_hHotItem;	// ��ǰѡ����
	COLORREF m_clrSelBk;	// ѡ����Ŀ�ı���ɫ
	COLORREF m_clrSelText;	// ѡ����Ŀ������ɫ
	COLORREF m_clrFolderBk;	// Folder �ı���ɫ
	COLORREF m_clrFolderText; // Folder ��������ɫ
	COLORREF m_clrFolderOverText;
	COLORREF m_clrBorder;	// ����ؼ��б߿�Ļ����ô���ɫ�����߿�

	int m_nFolderExpended;
	int m_nFolderUnExpended;
	POINT m_ptImgOffset;	
	BOOL	m_bTracking;
	HTREEITEM	m_hPressedItem;
	int m_nTextIndent;
	CBrush m_brTitle;	
};

