#if !defined(AFX_MEDICINEMGR_H__4079B05A_5417_4486_B8FB_8C2E02F5AD88__INCLUDED_)
#define AFX_MEDICINEMGR_H__4079B05A_5417_4486_B8FB_8C2E02F5AD88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MedicineMgr.h : header file
//

#include "RCButton.h"
#include "RCDialog.h"
#include "SkinListCtrl.h"
#include "MedicineManage.h"
#include "MedicineInfo.h"
#include "Util.h"
/////////////////////////////////////////////////////////////////////////////
// CMedicineMgr dialog

class CQueryMedicine : public CRCDialog
{
// Construction
public:
	CQueryMedicine(CWnd* pParent = NULL);   // standard constructor
    ~CQueryMedicine();
// Dialog Data
	//{{AFX_DATA(CQueryMedicine)
	enum { IDD = IDD_QueryMedicine };
	CRCButton	m_PanDian;
	CRCButton	m_close;
	CComboBox	m_medName;
	CRCButton	m_btnQuery;
	CSkinListCtrl	m_listMedInfo;
	CRCButton	m_btnWatchMed;
	CRCButton	m_btnEditMed;
	BOOL	m_Allcheck;
	BOOL	m_medInMachine;
	BOOL	m_medState;
	BOOL	m_medLack;
	//}}AFX_DATA
	int type;
	static bool falg1,falg2;
    CMedicineManage *med;
    CMedicineInfo *dlg,*dlg_edit;//*dlg_add,*dlg_delete;
	CShellExecuteOsk osk;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueryMedicine)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    afx_msg void OnDeleteItem(WPARAM wParam,LPARAM lParam);
	// Generated message map functions
	//{{AFX_MSG(CQueryMedicine)
	afx_msg void OnWatchMedicineInfo();
	virtual BOOL OnInitDialog();
	afx_msg void OnQueryMed();
	afx_msg void OnEditchangeCombo1();
	afx_msg void OnEditMed();
	afx_msg void OnButton3();
	afx_msg void OnSetfocusCombo1();
	afx_msg void OnKillfocusCombo1();
	afx_msg void OnDblclklistMedInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClicklistMedInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPanDian();
	afx_msg void OnCheckall();
	afx_msg void OnCheckbox();
	afx_msg void OnCheckmedstate();
	afx_msg void OnChecklackmed();
	afx_msg void OnMsg(UINT uMsg, WPARAM wPara, LPARAM lPara);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_click;    //控制药品查询次数
	UINT QueryWork();
	static DWORD WINAPI ThreadFunc(LPVOID lpVoid);
	


protected:
	void InitMedicineList();
	void RefreshState();
private:
    CBIZBaseService * bizser;
	Singleton<CBIZBoxService> singletonBIZ;
    list<MedicinList> *pmedicinList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDICINEMGR_H__4079B05A_5417_4486_B8FB_8C2E02F5AD88__INCLUDED_)
