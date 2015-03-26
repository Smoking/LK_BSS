#if !defined(AFX_WORKQUEUEPAGE_H__BCC597C4_53C2_469E_A0DA_4EF2EC996C58__INCLUDED_)
#define AFX_WORKQUEUEPAGE_H__BCC597C4_53C2_469E_A0DA_4EF2EC996C58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkQueuePage.h : header file
//

#include "RCList.h"
#include "RCEdit.h"
#include "RCButton.h"
#include "SkinListCtrl.h"
#include "XPGroupBox.h"
#include "BIZBaseService.h"
#include "BIZBoxService.h"
#include "Singleton.h"

/////////////////////////////////////////////////////////////////////////////
// CWorkQueuePage dialog

class CWorkQueuePage : public CDialog
{
// Construction
public:
	CWorkQueuePage(CWnd* pParent = NULL);   // standard constructor
	~CWorkQueuePage();
	void clean();
// Dialog Data
	//{{AFX_DATA(CWorkQueuePage)
	enum { IDD = IDD_WorkQueue };
	CRCButton	m_btnStop;
	CRCButton	m_btnStart;
	CRCButton	m_btnBack;
	CStatic	m_patient;
	CStatic	m_sex;
	CStatic	m_age;
	CXPGroupBox	m_Group;
	CSkinListCtrl	m_MedList;
	CRCButton	m_MoveLeftAll;
	CRCButton	m_btnUp;
	CRCButton	m_btnRight;
	CRCButton	m_btnLeft;
	CRCButton	m_btnDown;
	CSkinListCtrl	m_workQueue_Right;
	CSkinListCtrl	m_workQueue_Left;
	//}}AFX_DATA

    HWND hwnd;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkQueuePage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWorkQueuePage)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveLeftAll();
	afx_msg void OnButstart();
	afx_msg void OnButstop();
	afx_msg void OnButback();
	afx_msg void OnClickWorkQueueLeftList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickWorkQueueRightList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CBIZBaseService* bizbs;
	Singleton<CBIZBoxService> singletonBIZ;
	CDBManage* cdb;
	static LoggerPtr rootLogger; 
	
protected:
	void InitMedList();
	void InitButton();
	void InitLeftWorkList();
	void InitRightWorkList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKQUEUEPAGE_H__BCC597C4_53C2_469E_A0DA_4EF2EC996C58__INCLUDED_)
