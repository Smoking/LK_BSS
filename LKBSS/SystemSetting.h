#if !defined(AFX_SYSTEMSETTING_H__E5B157DA_E5F4_4CC8_8EE8_430CBFF700E5__INCLUDED_)
#define AFX_SYSTEMSETTING_H__E5B157DA_E5F4_4CC8_8EE8_430CBFF700E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemSetting.h : header file
//

#include "RCDialog.h"
#include "RCTreeCtrl.h"
#include "BasicSetting.h"
#include "DatabaseSetting.h"
#include "PLCSetting.h"
#include "PrinterSetting.h"
#include "SysMonitorSetting.h"
#include "RCButton.h"
#include "SysConfig.h"
#include "Util.h"
#include "ResetDefaultSetting.h"
#include "Encrypt.h"

enum
{
	PageId_BasicSetting = 0,	//基本配置页面
	PageId_PLC,					//PLC页面
	PageId_DataBase,			//数据库页面
	PageId_Printer,				//打印机页面
	PageId_SysMonitor,			//系统监控页面
    PageId_ResetDefault,        //恢复默认
};

/////////////////////////////////////////////////////////////////////////////
// CSystemSetting dialog

class CSystemSetting : public CRCDialog
{
// Construction
public:
	CSystemSetting(CWnd* pParent = NULL);   // standard constructor
    ~CSystemSetting();

// Dialog Data
	//{{AFX_DATA(CSystemSetting)
	enum { IDD = IDD_SysSetting };
	CRCButton	m_btnCancel;
	CRCButton	m_btnOK;
	CRCTreeCtrl	m_treeCategore;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemSetting)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOk();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCategoriesTree();
	void ShowPage(int nPage);
	void GetChanged(int nPage);  //取得改变的数据
	//void ShowMessage(UINT ids, long code);

private:
	HTREEITEM		m_hTreeRootItem;
	COLORREF		m_clrFolderText;
	COLORREF		m_clrHightLight;
	COLORREF		m_clrTextHightLight;
	COLORREF		m_clrTreeBk;
	COLORREF		m_clrText;
	COLORREF		m_clrFolderOverText;

	//页面对象
	CBasicSetting		m_pageBasicSetting;
	CDatabaseSetting	m_pageDBSetting;
	CPLCSetting			m_pagePLCSetting;
	CPrinterSetting		m_pagePrinterSetting;
	CSysMonitorSetting	m_pageSysMonitorSetting;
    CResetDefaultSetting m_pageResetDefaultSetting;

	int m_previousPageId;
	int nPageId;  //当前页
	map<string, string> m_settingData;
	CSysConfig sysConfig;
    Encrypt *enp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMSETTING_H__E5B157DA_E5F4_4CC8_8EE8_430CBFF700E5__INCLUDED_)
