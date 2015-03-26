#if !defined(AFX_PRINTERSETTING_H__26B4AC87_621B_45D3_9755_4D1F3BF7E73D__INCLUDED_)
#define AFX_PRINTERSETTING_H__26B4AC87_621B_45D3_9755_4D1F3BF7E73D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrinterSetting.h : header file
//

#include "XPGroupBox.h"
#include "Util.h"
/////////////////////////////////////////////////////////////////////////////
// CPrinterSetting dialog

class CPrinterSetting : public CDialog
{
// Construction
public:
	CPrinterSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrinterSetting)
	enum { IDD = IDD_Setting_Printer };	
	CEdit	m_edtThermalPrtPort;
	CEdit	m_edtThermalPrtPaper;
	CEdit	m_edtThermalPrtBaud;
	CEdit	m_edtSatoPrtPort;
	CEdit	m_edtSatoPrtBaud;
	CString	m_satoPrtBaud;
	CString	m_satoPrtPort;
	CString	m_thermalPrtBaud;
	CString	m_thermalPrtPaper;
	CString	m_thermalPrtPort;
    CXPGroupBox	m_GBCaption;
    CXPGroupBox	m_GBCaption2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrinterSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrinterSetting)
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

#endif // !defined(AFX_PRINTERSETTING_H__26B4AC87_621B_45D3_9755_4D1F3BF7E73D__INCLUDED_)
