#if !defined(AFX_OPZONEPAGE_H__6C76C04F_2576_425F_A306_CF3F90F2EC94__INCLUDED_)
#define AFX_OPZONEPAGE_H__6C76C04F_2576_425F_A306_CF3F90F2EC94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OPZonePage.h : header file
//
#include "RCButton.h"
#include "BitmapEdit.h"
#include "Utils.h"
#include <vector>
#include "BIZFillMedicine.h"
#include "Util.h"
//#include "AddMedicineDlg.h"
/////////////////////////////////////////////////////////////////////////////
// COPZonePage dialog

class COPZonePage : public CDialog
{
// Construction
public:
	COPZonePage(CWnd* pParent = NULL);   // standard constructor
	~COPZonePage();
// Dialog Data
	//{{AFX_DATA(COPZonePage)
	enum { IDD = IDD_AddMed_OpZone };
	CRCButton	m_btnFullFill4;
	CRCButton	m_btnFullFill3;
	CRCButton	m_btnFullFill2;
	CRCButton	m_btnFullFill1;
	CRCButton	m_btnOK4;
	CRCButton	m_btnOK3;
	CRCButton	m_btnOK2;
	CRCButton	m_btnSubtract4;
	CRCButton	m_btnSubtract3;
	CRCButton	m_btnSubtract2;
	CRCButton	m_btnPlus4;
	CRCButton	m_btnPlus3;
	CRCButton	m_btnPlus2;
	CBitmapEdit	m_editAddNum;
	CBitmapEdit	m_editAddNum2;
	CBitmapEdit	m_editAddNum3;
	CBitmapEdit	m_editAddNum4;
	CRCButton	m_btnPlus;
	CRCButton	m_btnSubtract;
	CRCButton	m_btnOK;
	int		m_nAddNumber;
	int		m_nAddNumber2;
	int		m_nAddNumber3;
	int		m_nAddNumber4; 
	//}}AFX_DATA

public:
	void SetMedicineName(CString strName,int order =0);
	void SetLocation(CString strLoc,int order =0);
	void setCurQuantity(int strCurQuantity,int order =0);
	void setMaxCapacity(int strMaxCapacity,int order =0);
	void setMedicineUnit(CString strUnit,int order =0);

	void displayLocationDetail(int locationId, int order = 0);
	void setParentHwnd(HWND hwnd);
	int maxNumberPerUnit;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPZonePage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COPZonePage)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnSubtract();
	afx_msg void OnPlus();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetfocusEditNum();
	afx_msg void OnPlus2();
	afx_msg void OnPlus3();
	afx_msg void OnPlus4();
	afx_msg void OnSubtract2();
	afx_msg void OnSubtract3();
	afx_msg void OnSubtract4();
	afx_msg void OnOk2();
	afx_msg void OnOk3();
	afx_msg void OnOk4();
	afx_msg void OnFullfill1();
	afx_msg void OnFullfill2();
	afx_msg void OnFullfill3();
	afx_msg void OnFullfill4();
	afx_msg void OnSetfocusEditNum2();
	afx_msg void OnSetfocusEditNum3();
	afx_msg void OnSetfocusEditNum4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitButton();
	CShellExecuteOsk osk;

private:
	CFont m_fontMedName;	//药品名称字体
	CFont m_fontLocation;	//位置字体
	CFont m_fontNumber;     //添加数量字体

	CString m_strMedicineName;
	CString m_strLocation;
	CString m_strMedicineUnit;
	
	int m_strCurQuantity;
	int m_strMaxCapacity;

	CString m_strMedicineName2;
	CString m_strLocation2;
	CString m_strMedicineUnit2;
	
	int m_strCurQuantity2;
	int m_strMaxCapacity2;

	CString m_strMedicineName3;
	CString m_strLocation3;
	CString m_strMedicineUnit3;
	
	int m_strCurQuantity3;
	int m_strMaxCapacity3;

	CString m_strMedicineName4;
	CString m_strLocation4;
	CString m_strMedicineUnit4;
	
	int m_strCurQuantity4;
	int m_strMaxCapacity4;
	
	bool falg1;
	
	HWND h_parentHwnd;
	
	CBIZFillMedicine* bizfm; 
	CDBManage* cdb;
//	Singleton<CBIZFillMedicine> singlebizfm;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPZONEPAGE_H__6C76C04F_2576_425F_A306_CF3F90F2EC94__INCLUDED_)
