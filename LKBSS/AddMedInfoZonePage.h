#if !defined(AFX_ADDMEDINFOZONEPAGE_H__1620603A_AF04_4029_800C_9BEEE3145618__INCLUDED_)
#define AFX_ADDMEDINFOZONEPAGE_H__1620603A_AF04_4029_800C_9BEEE3145618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddMedInfoZonePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddMedInfoZonePage dialog
#include "RCEdit.h"
#include "SkinListCtrl.h"


class CAddMedInfoZonePage : public CDialog
{
// Construction
public:
	CAddMedInfoZonePage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddMedInfoZonePage)
	enum { IDD = IDD_AddMed_InfoZone };
	CSkinListCtrl	m_listAddMed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddMedInfoZonePage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	
	// Generated message map functions
	//{{AFX_MSG(CAddMedInfoZonePage)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitAddMedList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDMEDINFOZONEPAGE_H__1620603A_AF04_4029_800C_9BEEE3145618__INCLUDED_)
