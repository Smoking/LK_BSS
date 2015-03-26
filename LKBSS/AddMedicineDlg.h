#if !defined(AFX_ADDMEDICINEDLG_H__73222816_FA65_423F_A3BA_C1444EE3A960__INCLUDED_)
#define AFX_ADDMEDICINEDLG_H__73222816_FA65_423F_A3BA_C1444EE3A960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddMedicineDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddMedicineDlg dialog

#include "RCButton.h"
#include "RCDialog.h"
#include "ImageTabWnd.h"
#include "AddMedInfoZonePage.h"
#include "OPZonePage.h"

#include "BIZFillMedicine.h"
#include "Singleton.h"
#include "MedicineManage.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <string>

class CAddMedicineDlg : public CRCDialog
{
// Construction
public:
	CAddMedicineDlg(CWnd* pParent = NULL);   // standard constructor
	~CAddMedicineDlg();
// Dialog Data
	//{{AFX_DATA(CAddMedicineDlg)
	enum { IDD = IDD_AddMedcine };
	CRCButton	m_btnStartAdd;
	CRCButton	m_btnExitAdd;
	//}}AFX_DATA

    list<CLocation> locationList;
	list<CMedicine> medicineList;
	int addType;
	int maxNumberPerUnit;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddMedicineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddMedicineDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnStartAdd();
	afx_msg void OnExitAdd();
	afx_msg void OnFinishFilling(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitButton();

	void CreateInfoZone();			//创建加药信息区域
	void CreateOpZone();			//创建操作区域

	void InitData(list<CLocation> locationList);
	void InitData(list<CMedicine> medicineList);
	void displayLocationDetail(int locationId);
private:
	CImageTabWnd			m_infoZoneTabWnd;			//信息区
	CAddMedInfoZonePage		m_infoZonePage;				//加药信息区页面
	COPZonePage				m_OpZonePage;				//操作区域页面

	CBIZFillMedicine* bizfm;                       //加药模块
	static LoggerPtr rootLogger ;                 //日志模块
	//Singleton<CBIZFillMedicine> singlebizfm;
	CMedicineManage* medManage;
	CDBManage* cdb;
	int max_Muallocation;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDMEDICINEDLG_H__73222816_FA65_423F_A3BA_C1444EE3A960__INCLUDED_)
