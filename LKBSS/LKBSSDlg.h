// LKBSSDlg.h : header file
//
//{{AFX_INCLUDES()
#include "actfxcpu3.h"
//}}AFX_INCLUDES

#if !defined(AFX_LKBSSDLG_H__B3FB267F_26BF_4900_87D3_6AAE5E041AE4__INCLUDED_)
#define AFX_LKBSSDLG_H__B3FB267F_26BF_4900_87D3_6AAE5E041AE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RCDialog.h"
#include "RCButton.h"
#include "ImageTabWnd.h"
#include "WorkQueuePage.h"
#include "DeviceMonitor.h"
#include "ServerSocket.h"
#include "Singleton.h"
#include "MedicineManage.h"
#include "Util.h"

/////////////////////////////////////////////////////////////////////////////
// CLKBSSDlg dialog
//-------------------------------------------------

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <string>
#include "BIZBaseService.h"
#include "SysConfig.h"
#include "BIZBoxService.h"
#include "BoxMonitor.h"
//---------------------------------------------------
#include "PortManage.h"


class CLKBSSDlg : public CRCDialog
{
// Construction
public:
	CLKBSSDlg(CWnd* pParent = NULL);	// standard constructor
	~CLKBSSDlg();
	 
// Dialog Data
	//{{AFX_DATA(CLKBSSDlg)
	enum { IDD = IDD_LKBSS_DIALOG };
	CRCButton	m_btnAddMedicine;
	CRCButton	m_btnQueryMedicine;
	CRCButton	m_btnStopMedicine;
	CRCButton	m_btnStartMedicine;
	CActFXCPU3	m_fxcpu;
	//}}AFX_DATA
    
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLKBSSDlg)
	protected:
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CStatusBar m_statusBar;

	afx_msg void OnRefresh(WPARAM wParam,LPARAM lParam);  //界面刷新消息处理函数
	afx_msg void OnButtonDown(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMonitorMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnChangeThreadState(WPARAM wParam,LPARAM lParam); //发药线程
	afx_msg void OnDistError(WPARAM wParam,LPARAM lParam);//出错地址
	afx_msg void OnGetErrorMessage(WPARAM wParam,LPARAM lParam);
	// Generated message map functions
	//{{AFX_MSG(CLKBSSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartMedicine();			//开始发药
	afx_msg void OnAddMedicine();			//加药
	afx_msg void OnQueryMedicine();			//药品查询
	afx_msg void OnLogOut();
	afx_msg void OnMedManage();
	afx_msg void OnQueryLog();
	afx_msg void OnDestroy();
	afx_msg void OnReboot();
	afx_msg void OnMonitor();
	afx_msg void OnStopMedicine();
	afx_msg void OnSendMed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ChangeRunningState(bool Running = true);
    DWORD FindProcess(char *strProcessName);
	VOID KillProcess();

	void CreateInfoZone();		//创建信息区域
	void CreateMonitorZone();	//创建监控区域
	void ShowPreMed(int preId,int splitid,bool IsNormal);
private:
	static LoggerPtr rootLogger;                 //日志模块

	CImageTabWnd		m_infoZoneTabWnd;			//信息区
	CWorkQueuePage		m_workQueuePage;			//工作队列页面

	CImageTabWnd		m_monitorZoneTabWnd;		//监控区域
	CDeviceMonitor		m_deviceMonitorPage;		//设备监控

	
	CBIZBaseService* bizbs;                       //核心业务模块	
	Singleton<CBIZBoxService> singletonBIZ;

	CStatistic * statistic;
	Singleton<CStatistic> singletonStatistic;
	
	Singleton<CFxPLC> singletonFxPLC;
	CFxPLC *fxcpu;

	CServerSocket *server;
	CMedicineManage *med;
	CBoxMonitor *pBoxMonitor;	
	CPrescriptionManager* m_cpm;

	int portPLC;
	int machineID;
	CString mes[20];
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LKBSSDLG_H__B3FB267F_26BF_4900_87D3_6AAE5E041AE4__INCLUDED_)
