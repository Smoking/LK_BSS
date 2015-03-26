#if !defined(AFX_LOGVIEWER_H__DFB3241F_5869_4064_9C06_050CE86E7A28__INCLUDED_)
#define AFX_LOGVIEWER_H__DFB3241F_5869_4064_9C06_050CE86E7A28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogViewer dialog

#include "RCButton.h"
#include "RCDialog.h"
#include "RCList.h"
#include "SkinListCtrl.h"

class CLogViewer : public CRCDialog
{
// Construction
public:
	CLogViewer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogViewer)
	enum { IDD = IDD_LogViewer };
	CDateTimeCtrl	m_datatime;
	CRCButton	m_viewLog;
	CSkinListCtrl	m_listLogRecord;
	CRCButton	m_btnClose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogViewer)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnViewLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGVIEWER_H__DFB3241F_5869_4064_9C06_050CE86E7A28__INCLUDED_)
