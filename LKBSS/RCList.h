#if !defined(AFX_RCLIST_H__B4631326_D1BF_46E5_8AB7_38BEE4CBE46E__INCLUDED_)
#define AFX_RCLIST_H__B4631326_D1BF_46E5_8AB7_38BEE4CBE46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCList.h : header file
//
#include "SkinHeaderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRCList window

class CRCList : public CListCtrl
{
// Construction
public:
	CRCList();

// Attributes
public:
	CSkinHeaderCtrl m_SkinHeaderCtrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCList)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRCList();

public:
	void SetBgColor(COLORREF clr);

	// Generated message map functions
protected:
	//{{AFX_MSG(CRCList)
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_clrBrush;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCLIST_H__B4631326_D1BF_46E5_8AB7_38BEE4CBE46E__INCLUDED_)
