#if !defined(AFX_RCEDIT_H__502AF029_63CB_4B2A_AA73_733DC24D882B__INCLUDED_)
#define AFX_RCEDIT_H__502AF029_63CB_4B2A_AA73_733DC24D882B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRCEdit window

class CRCEdit : public CEdit
{
// Construction
public:
	CRCEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRCEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRCEdit)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCEDIT_H__502AF029_63CB_4B2A_AA73_733DC24D882B__INCLUDED_)
