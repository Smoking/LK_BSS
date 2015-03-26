#if !defined(AFX_DATABASESETTING_H__9C265C14_3D5B_455A_9012_09683B620478__INCLUDED_)
#define AFX_DATABASESETTING_H__9C265C14_3D5B_455A_9012_09683B620478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DatabaseSetting.h : header file
//

#include "XPGroupBox.h"
#include "Encrypt.h"
#include "Util.h"
/////////////////////////////////////////////////////////////////////////////
// CDatabaseSetting dialog

class CDatabaseSetting : public CDialog
{
// Construction
public:
	CDatabaseSetting(CWnd* pParent = NULL);   // standard constructor
    ~CDatabaseSetting();

// Dialog Data
	//{{AFX_DATA(CDatabaseSetting)
	enum { IDD = IDD_Setting_Database };
	CEdit	m_edtUser;
	CEdit	m_edtServer;
	CEdit	m_edtPsw;
	CEdit	m_edtPort;
	CEdit	m_edtDatabase;
	CString	m_port;
	CString	m_psw;
	CString	m_server;
	CString	m_user;
	CString	m_database;
	CString	m_characterSet;
    CXPGroupBox	m_GBCaption;
	//}}AFX_DATA
	BOOL m_characterSetChanged;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDatabaseSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDatabaseSetting)
    virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEditchangeCombo1();
	afx_msg void OnEditchangeCMBcharacterSet();
	afx_msg void OnCloseupCMBcharacterSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void InitData();
    CShellExecuteOsk osk;
    Encrypt *enp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABASESETTING_H__9C265C14_3D5B_455A_9012_09683B620478__INCLUDED_)
