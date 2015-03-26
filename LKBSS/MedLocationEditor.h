#if !defined(AFX_MEDLOCATIONEDITOR_H__60C6F773_F9B4_4618_BC01_C8D2071CAE42__INCLUDED_)
#define AFX_MEDLOCATIONEDITOR_H__60C6F773_F9B4_4618_BC01_C8D2071CAE42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MedLocationEditor.h : header file
//
#include "RCButton.h"
#include "RCDialog.h"
#include "SkinListCtrl.h"
#include "MedicineManage.h"

/////////////////////////////////////////////////////////////////////////////
// CMedLocationEditor dialog

class CMedLocationEditor : public CRCDialog
{
// Construction
public:
	CMedLocationEditor(CWnd* pParent = NULL);   // standard constructor
    int type;
	int medid;
	list<MedicinLocationState>medlocation;
    CMedicineManage * med;
	map<string,int>allotLocation;
	map<string,int>unallotLocation;
	CBIZBaseService * bizser;
	Singleton<CBIZBoxService> singletonBIZ;
// Dialog Data
	//{{AFX_DATA(CMedLocationEditor)
	enum { IDD = IDD_MedLocationEditor };
	CRCButton	m_open;
	CRCButton	m_stop;
	CComboBox	m_unallotLocation;
	CRCButton	m_add;
	CRCButton	m_ok;
	CSkinListCtrl	m_list;
	CComboBox	m_allotLocation;
	CRCButton	m_delete;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMedLocationEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMedLocationEditor)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual void OnOK();
	afx_msg void OnDropdownCombo1();
	afx_msg void OnDropdownCombo2();
	afx_msg void OnButopen();
	afx_msg void OnButstop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDLOCATIONEDITOR_H__60C6F773_F9B4_4618_BC01_C8D2071CAE42__INCLUDED_)
