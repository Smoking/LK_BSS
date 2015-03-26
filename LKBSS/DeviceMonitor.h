#if !defined(AFX_DEVICEMONITOR_H__BBE240EA_FDB3_4346_948F_70A79FD214D1__INCLUDED_)
#define AFX_DEVICEMONITOR_H__BBE240EA_FDB3_4346_948F_70A79FD214D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceMonitor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeviceMonitor dialog
#include "BoxMonitor.h"
#include "RCButton.h"
#include "SkinListCtrl.h"
#include "MedicineManage.h"


class CDeviceMonitor : public CDialog
{
// Construction
public:
	CDeviceMonitor(CWnd* pParent = NULL);   // standard constructor
	~CDeviceMonitor();
    CBoxMonitor *pBoxMonitor;
	HANDLE m_monitorHandle;
	HWND hwnd;
// Dialog Data
	//{{AFX_DATA(CDeviceMonitor)
	enum { IDD = IDD_DEVICEMONITOR };
	CButton	m_checkEmport;
	CButton	m_checkStop;
	CButton	m_checkLack;
	CSkinListCtrl	m_listMed;
	CSkinListCtrl	m_listDevice;
	CComboBox	m_medUnit;
	CRCButton	m_btnBack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceMonitor)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void getMedMonitor();


protected:
	void InitMedList();
	void InitDeviceList();
	void InitInstrument();	


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeviceMonitor)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChecklack();
	afx_msg void OnButback();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeCombo();
	afx_msg void OnCheckstop();
	afx_msg void OnCheckemport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	CMedicineManage * medMag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEMONITOR_H__BBE240EA_FDB3_4346_948F_70A79FD214D1__INCLUDED_)
