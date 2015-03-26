#if !defined(AFX_SYSMONITORSETTING_H__5B142E9D_ECBF_4061_BD0F_93FE23D7833F__INCLUDED_)
#define AFX_SYSMONITORSETTING_H__5B142E9D_ECBF_4061_BD0F_93FE23D7833F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysMonitorSetting.h : header file
//
#include "Util.h"
#include "XPGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSysMonitorSetting dialog

class CSysMonitorSetting : public CDialog
{
// Construction
public:
	CSysMonitorSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysMonitorSetting)
	enum { IDD = IDD_Setting_SysMonitor };
	CEdit	m_edtDeviceFrequency;
	CEdit	m_edtBoxFrequency;
	CString	m_boxFrequency;
	CString	m_deviceFrequency;
    CXPGroupBox	m_GBCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysMonitorSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysMonitorSetting)
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

#endif // !defined(AFX_SYSMONITORSETTING_H__5B142E9D_ECBF_4061_BD0F_93FE23D7833F__INCLUDED_)
