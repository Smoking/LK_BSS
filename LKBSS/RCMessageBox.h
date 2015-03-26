#if !defined(AFX_RCMESSAGEBOX_H__1357BD0F_94FB_45C5_8F8B_FD09C0DC4012__INCLUDED_)
#define AFX_RCMESSAGEBOX_H__1357BD0F_94FB_45C5_8F8B_FD09C0DC4012__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCMessageBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRCMessageBox dialog

#include "RCButton.h"
#include "RCDialog.h"


class CRCMessageBox : public CRCDialog
{
// Construction
public:
	CRCMessageBox(CWnd* pParent = NULL);   // standard constructor

public:
	void InitParam(HWND hWnd, CString strText, CString strCaption, UINT nType);

// Dialog Data
	//{{AFX_DATA(CRCMessageBox)
	enum { IDD = IDD_MessageBox };
	CRCButton	m_btnCancle;
	CRCButton	m_btnOK;
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCMessageBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void InitButton();
	// Generated message map functions
	//{{AFX_MSG(CRCMessageBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString		m_strCaption;
	HWND		m_hWndParent;
	UINT		m_uType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCMESSAGEBOX_H__1357BD0F_94FB_45C5_8F8B_FD09C0DC4012__INCLUDED_)
