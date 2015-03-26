#if !defined(AFX_RESETDEFAULTSETTING_H__5E709F6A_6F87_4B81_9CBE_8A254E6B8CE7__INCLUDED_)
#define AFX_RESETDEFAULTSETTING_H__5E709F6A_6F87_4B81_9CBE_8A254E6B8CE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResetDefaultSetting.h : header file
//

#include "RCButton.h"
#include "XPGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CResetDefaultSetting dialog

class CResetDefaultSetting : public CDialog 
{
// Construction
public:
	CResetDefaultSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResetDefaultSetting)
	enum { IDD = IDD_Setting_ResetDefault };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CRCButton	m_btnOK;
    CRCButton   m_btnSave;
    CXPGroupBox	m_GBCaption;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResetDefaultSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResetDefaultSetting)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void Onsave();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
    string defaultFile;
    string configFile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESETDEFAULTSETTING_H__5E709F6A_6F87_4B81_9CBE_8A254E6B8CE7__INCLUDED_)
