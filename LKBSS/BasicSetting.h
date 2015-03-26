#if !defined(AFX_BASICSETTING_H__357E4B24_3D00_4144_8483_1478AE6BC604__INCLUDED_)
#define AFX_BASICSETTING_H__357E4B24_3D00_4144_8483_1478AE6BC604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BasicSetting.h : header file
//
#include "Util.h"
#include "XPGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CBasicSetting dialog
class CBasicSetting : public CDialog
{
// Construction
public:
	CBasicSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBasicSetting)
	enum { IDD = IDD_Setting_BasicSetting };
	CEdit	m_edtLog;
	CEdit	m_edtDealFrequency;
	CString	m_log;
	CString	m_dealFrequency;
    CXPGroupBox	m_GBCaption;
	BOOL	m_bMedValidateCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBasicSetting)
    virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void InitData();
    CShellExecuteOsk osk;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICSETTING_H__357E4B24_3D00_4144_8483_1478AE6BC604__INCLUDED_)
