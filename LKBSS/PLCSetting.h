#if !defined(AFX_PLCSETTING_H__2C4CB732_6841_435E_8A69_B0189E60F2AF__INCLUDED_)
#define AFX_PLCSETTING_H__2C4CB732_6841_435E_8A69_B0189E60F2AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PLCSetting.h : header file
#include "Util.h"

#include "XPGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CPLCSetting dialog

class CPLCSetting : public CDialog
{
// Construction
public:
	CPLCSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPLCSetting)
	enum { IDD = IDD_Setting_PLC };
	CEdit	m_edtStationNum;
	CEdit	m_edtTimeOut;
	CEdit	m_edtIP;
	CString	m_conTimeOut;
	CString	m_IP;
	CString	m_stationNum;
    CXPGroupBox	m_GBCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPLCSetting)
    virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitData();  //初始化数据
    CShellExecuteOsk osk;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCSETTING_H__2C4CB732_6841_435E_8A69_B0189E60F2AF__INCLUDED_)
