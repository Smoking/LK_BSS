#if !defined(AFX_MEDICINEINFO_H__892E2249_C056_46A8_BB83_47F01FBCA654__INCLUDED_)
#define AFX_MEDICINEINFO_H__892E2249_C056_46A8_BB83_47F01FBCA654__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MedicineInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMedicineInfo dialog

#include "RCButton.h"
#include "RCDialog.h"
#include "SkinListCtrl.h"
#include "BitmapSlider.h"
#include "MedicineManage.h"
#include "MedLocationEditor.h"
#include "Util.h"
class CMedicineInfo : public CRCDialog
{
// Construction
public:
	bool CopyFile(CString pFrom, CString pTo, CString newfilename);
	void clearData();
	void InitLocationList();
	void setValue(MedInfo &medicineinfo);
	CMedicineInfo(CWnd* pParent = NULL);   // standard constructor
    HWND m_hwnd;
	int line;
	int type;
	CString m_filePath;
	CString strPath;
// Dialog Data
	//{{AFX_DATA(CMedicineInfo)
	enum { IDD = IDD_MedicineInfo };
	CRCButton	m_start;
	CRCButton	m_selectPicture;
	CRCButton	m_clearLocation;
	CSkinListCtrl	m_locationList;
	CRCButton	m_editLocation;
	CBitmapSlider	m_sliderWarning;
	CRCButton	m_btnCancel;
	CRCButton	m_btnModify;
	CString	m_medCNname;
	CString	m_medHISID;
	CString	m_medENname;
	CString	m_medUnit;
	CString	m_typeUnit;
	CString	m_medID;
	CString	m_factoryName;
	CString	m_medPYcode;
	CString	m_medCUSTcode;
	CString	m_pos;
	CString	m_max_once;
	CString	m_maxCapacity;
	CString	m_validate_time;
	CString	m_length;
	CString	m_width;
	CString	m_high;
	CString	m_medCount;
	CString	m_medbatch;
	CString	m_Count;
	//}}AFX_DATA
    static bool showfalg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMedicineInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    int m_nPos;
	CMedicineManage *med;
	CBIZBaseService * bizser;
	Singleton<CBIZBoxService> singletonBIZ;
	list<MedicinLocationState>locationlist;
	CShellExecuteOsk osk;
	// Generated message map functions
	//{{AFX_MSG(CMedicineInfo)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButton3();
	afx_msg void OnClearlocation();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButton6();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnButton7();
	afx_msg void OnSetfocusEdit7();
	afx_msg void OnSetfocusEdit11();
	afx_msg void OnSetfocusEdit14();
	afx_msg void OnSetfocusEdit15();
	afx_msg void OnSetfocusEdit16();
	afx_msg void OnSetfocusEdit18();
	afx_msg void OnSetfocusEdit19();
	afx_msg void OnSetfocusEdit22();
	//}}AFX_MSG
	afx_msg LRESULT OnSliderMoving(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDICINEINFO_H__892E2249_C056_46A8_BB83_47F01FBCA654__INCLUDED_)
