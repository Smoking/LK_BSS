#if !defined(AFX_RCBUTTON_H__8D36AF7B_BB79_434C_8094_A7CA40144D1B__INCLUDED_)
#define AFX_RCBUTTON_H__8D36AF7B_BB79_434C_8094_A7CA40144D1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRCButton window

class CRCButton : public CButton
{
	// Construction
public:
	CRCButton();
	virtual ~CRCButton();
	
public:
	void DrawButton(CDC *pDC, CRect rcCtrl, HBITMAP hBitmap);
	void SetSizeMode(BOOL bAutoSize);
	void SetBMP(UINT IDNormal, UINT IDHover, UINT IDDown, UINT  IDDef);
	void SetBMP(CBitmap *pBmpNormal, CBitmap *pBmpHover, CBitmap  *pBmpDown, CBitmap *pBmpDef);
	void SetBMP(HBITMAP hBmpNormal, HBITMAP hBmpHover, HBITMAP  hBmpDown, HBITMAP hBmpDef);
	void SetBMP(HBITMAP hBmpNormal, HBITMAP hBmpHover, HBITMAP hBmpDown, HBITMAP hBmpDef,HBITMAP hBmpDisable);
	CRCButton& operator = (const CRCButton button);
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	

protected:
	//{{AFX_MSG(CRCButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

	// Attributes
private:
	BOOL m_bHover;
	BOOL m_bTracking;
	BOOL m_bSelected;
	BOOL m_bFocus;
	BOOL m_bAutoSize;

	HBITMAP		m_hBmpNormal;
	HBITMAP		m_hBmpDown;
	HBITMAP		m_hBmpHover;
	HBITMAP		m_hBmpDefault;
	HBITMAP		m_hBmpDisable;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCBUTTON_H__8D36AF7B_BB79_434C_8094_A7CA40144D1B__INCLUDED_)

