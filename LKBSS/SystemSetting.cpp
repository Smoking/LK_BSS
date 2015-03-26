// SystemSetting.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "SystemSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IMAGELIST_DEFAULT_INDEX			1024
#define	ROOTITEM_DATA					-3

#define PAGE_LEFT_OFFSET	107
#define PAGE_TOP_OFFSET		25

/////////////////////////////////////////////////////////////////////////////
// CSystemSetting dialog


CSystemSetting::CSystemSetting(CWnd* pParent /*=NULL*/)
	: CRCDialog(CSystemSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemSetting)
	//}}AFX_DATA_INIT

	m_clrFolderText = RGB(20, 43, 124);
	m_clrHightLight = RGB(178,189,255);
	m_clrTextHightLight = RGB(0, 0, 0);
	m_clrTreeBk = RGB(239, 244, 255);
	m_clrText = RGB(0, 0, 0);
	m_clrFolderOverText = RGB(99, 119, 190);
    enp = NULL;
    enp = new Encrypt();

}

CSystemSetting::~CSystemSetting()
{
    if (enp == NULL)
    {
        delete enp;
        enp = NULL;
    }
}

void CSystemSetting::DoDataExchange(CDataExchange* pDX)
{
	CRCDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemSetting)
	DDX_Control(pDX, IDC_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_OK, m_btnOK);
	DDX_Control(pDX, IDC_TREEGROUP, m_treeCategore);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemSetting, CRCDialog)
	//{{AFX_MSG_MAP(CSystemSetting)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEGROUP, OnSelchangedTree)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREEGROUP, OnItemexpandedTree)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemSetting message handlers

BOOL CSystemSetting::OnInitDialog() 
{
	CRCDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_previousPageId = 0;
	InitCategoriesTree();
	sysConfig.initConfig();
    SetWindowText("国控智能盒剂快速发药系统-----设置系统"); 
    
    string defaultPath = sysConfig.getAppPath();
    string defaultFile = defaultPath + "\\default";
    string configFile = defaultPath + "\\LKBSSConfig.xml";
	ifstream fin(defaultFile.c_str());  
	if (!fin)    
	{  
       CopyFile(configFile.c_str(), defaultFile.c_str(),TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSystemSetting::InitCategoriesTree()
{
	//创建页面
	m_pageBasicSetting.Create(IDD_Setting_BasicSetting, this);
	m_pageDBSetting.Create(IDD_Setting_Database, this);
	m_pagePLCSetting.Create(IDD_Setting_PLC, this);
	m_pagePrinterSetting.Create(IDD_Setting_Printer, this);
	m_pageSysMonitorSetting.Create(IDD_Setting_SysMonitor, this);
    
    m_pageResetDefaultSetting.Create(IDD_Setting_ResetDefault, this);
	
	int xPos, yPos;
	xPos = PAGE_LEFT_OFFSET;
	yPos = PAGE_TOP_OFFSET;
	m_pageBasicSetting.SetWindowPos(&wndTop, xPos, yPos, 0 , 0, SWP_NOSIZE);
	m_pageDBSetting.SetWindowPos(&wndTop, xPos, yPos, 0 , 0, SWP_NOSIZE);
	m_pagePLCSetting.SetWindowPos(&wndTop, xPos, yPos, 0 , 0, SWP_NOSIZE);
	m_pagePrinterSetting.SetWindowPos(&wndTop, xPos, yPos, 0 , 0, SWP_NOSIZE);
	m_pageSysMonitorSetting.SetWindowPos(&wndTop, xPos, yPos, 0 , 0, SWP_NOSIZE);
    m_pageResetDefaultSetting.SetWindowPos(&wndTop, xPos, yPos, 0 , 0, SWP_NOSIZE);

	//属性设置
	m_treeCategore.SetItemHeight(20);
	m_treeCategore.SetFolderTextColor(m_clrFolderText);
	m_treeCategore.SetSelBkColor(m_clrHightLight);
	m_treeCategore.SetSelTextColor(m_clrTextHightLight);
	m_treeCategore.SetBkColor(m_clrTreeBk);
	m_treeCategore.SetTextColor(m_clrText);
	m_treeCategore.SetFolderOverTextColor(m_clrFolderOverText);

	m_treeCategore.SetClickToExpand(FALSE);
	m_treeCategore.EnableClipCursor(TRUE);

	m_treeCategore.SetWindowPos(&wndTop, 0, yPos, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
	
	//加入根节点
	CString str;
	str.LoadString(IDS_Setting);
	m_hTreeRootItem = m_treeCategore.InsertItem(str, IMAGELIST_DEFAULT_INDEX, IMAGELIST_DEFAULT_INDEX);
	m_treeCategore.SetItemData(m_hTreeRootItem, ROOTITEM_DATA);

	//加入子节点
	str.LoadString(IDS_BasicSetting);  //基本配置
	HTREEITEM hTreeItem = m_treeCategore.InsertItem(str, IMAGELIST_DEFAULT_INDEX,IMAGELIST_DEFAULT_INDEX,m_hTreeRootItem);
	m_treeCategore.SetItemData(hTreeItem, PageId_BasicSetting);

	//默认选中页面
	m_treeCategore.SelectItem(hTreeItem);
	m_treeCategore.SetHotItem(hTreeItem);
	m_pageBasicSetting.ShowWindow(SW_SHOW);


	str.LoadString(IDS_PLC);  //PLC
	hTreeItem = m_treeCategore.InsertItem(str, IMAGELIST_DEFAULT_INDEX,IMAGELIST_DEFAULT_INDEX,m_hTreeRootItem);
	m_treeCategore.SetItemData(hTreeItem, PageId_PLC);

	str.LoadString(IDS_Database);  //数据库
	hTreeItem = m_treeCategore.InsertItem(str, IMAGELIST_DEFAULT_INDEX,IMAGELIST_DEFAULT_INDEX,m_hTreeRootItem);
	m_treeCategore.SetItemData(hTreeItem, PageId_DataBase);

	str.LoadString(IDS_Print);  //打印机
	hTreeItem = m_treeCategore.InsertItem(str, IMAGELIST_DEFAULT_INDEX,IMAGELIST_DEFAULT_INDEX,m_hTreeRootItem);
	m_treeCategore.SetItemData(hTreeItem, PageId_Printer);

	str.LoadString(IDS_SysMonitor);  //系统监控
	hTreeItem = m_treeCategore.InsertItem(str, IMAGELIST_DEFAULT_INDEX,IMAGELIST_DEFAULT_INDEX,m_hTreeRootItem);
	m_treeCategore.SetItemData(hTreeItem, PageId_SysMonitor);

    str.LoadString(IDS_resetDefault);  //恢复默认
	hTreeItem = m_treeCategore.InsertItem(str, IMAGELIST_DEFAULT_INDEX,IMAGELIST_DEFAULT_INDEX,m_hTreeRootItem);
	m_treeCategore.SetItemData(hTreeItem, PageId_ResetDefault);
    
	m_treeCategore.Expand(m_hTreeRootItem, TVE_EXPAND);
}

BOOL CSystemSetting::OnEraseBkgnd(CDC* pDC)
{
//	return CRCDialog::OnEraseBkgnd(pDC);

	CRect rc;
	m_treeCategore.GetWindowRect(rc);
	ScreenToClient(rc);
	rc.InflateRect(1,0,1,1);
	pDC->Draw3dRect(rc, RGB(105,115,226),RGB(105,115,226));

	return TRUE;
}

void CSystemSetting::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	nPageId = 0;
	HTREEITEM hCurSel = m_treeCategore.GetSelectedItem();
	if(hCurSel == NULL || hCurSel == m_hTreeRootItem )
		return;

	GetChanged(m_previousPageId);
	nPageId = m_treeCategore.GetItemData(hCurSel);
	m_previousPageId = nPageId;
	
	m_treeCategore.SetHotItem(hCurSel);
	ShowPage(nPageId);
}

void CSystemSetting::ShowPage(int nPage)
{
	m_pageBasicSetting.ShowWindow(SW_HIDE);
	m_pageDBSetting.ShowWindow(SW_HIDE);
	m_pagePLCSetting.ShowWindow(SW_HIDE);
	m_pagePrinterSetting.ShowWindow(SW_HIDE);
	m_pageSysMonitorSetting.ShowWindow(SW_HIDE);
    m_pageResetDefaultSetting.ShowWindow(SW_HIDE);

	if (PageId_BasicSetting == nPage)
	{
		m_pageBasicSetting.ShowWindow(SW_SHOW);
	}
	else if (PageId_PLC == nPage)
	{
		m_pagePLCSetting.ShowWindow(SW_SHOW);
	}
	else if (PageId_DataBase == nPage)
	{
		m_pageDBSetting.ShowWindow(SW_SHOW);
	}
	else if (PageId_Printer == nPage)
	{
		m_pagePrinterSetting.ShowWindow(SW_SHOW);
	}
	else if (PageId_SysMonitor == nPage)
	{
		m_pageSysMonitorSetting.ShowWindow(SW_SHOW);
	}
    else if (PageId_ResetDefault == nPage)
    {
        //Util::MessageBox(this->GetSafeHwnd(), "text", "cap",RCMB_OKCANCEL);
        m_pageResetDefaultSetting.ShowWindow(SW_SHOW);
    }
}

void CSystemSetting::OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( TVE_COLLAPSE == pNMTreeView->action )
	{
		*pResult = 1;
	}
}

void CSystemSetting::OnOk() 
{
	// TODO: Add your control notification handler code here
	GetChanged(nPageId);
	if (!m_settingData.empty())
	{
		long lRet = 0;
        CString strText;
		lRet = sysConfig.saveConfig(m_settingData.begin(),m_settingData.end());
        if (lRet)
        {
            
            strText = configUtil.getValue(Utils::itos(lRet)).c_str();     
            Util::MessageBox(this->GetSafeHwnd(), strText, "提示", RCMB_OK);
            return;
        }
        strText.LoadString(IDS_configEnd);
        Util::MessageBox(this->GetSafeHwnd(), strText, "提示", RCMB_OK);
	}

	//CDialog::Close();
	CDialog::OnOK();
}

void CSystemSetting::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

//取得改变的数据
void CSystemSetting::GetChanged(int nPage)
{
	switch(nPage) {
	case PageId_BasicSetting:
		m_pageBasicSetting.UpdateData();
		if(m_pageBasicSetting.m_edtLog.GetModify())
		{
			m_settingData["logPropFile"] = m_pageBasicSetting.m_log;
			m_pageBasicSetting.m_edtLog.SetModify(FALSE);		
		}
		if(m_pageBasicSetting.m_edtDealFrequency.GetModify())
		{
			m_settingData["DealFrequency"] = m_pageBasicSetting.m_dealFrequency;
			m_pageBasicSetting.m_edtDealFrequency.SetModify(FALSE);		
		}
        if (m_pageBasicSetting.m_bMedValidateCtrl)
        {
            m_settingData["ValidateControl"] = "1";
        }else
        {
            m_settingData["ValidateControl"] = "0";
        }
        
		//m_pageBasicSetting.m_conTimeOut;connectTimeOut;

		//m_settingData.insert(pair<string,string>("connectTimeOut", ))
		break;
	case PageId_PLC:
		m_pagePLCSetting.UpdateData();
		if(m_pagePLCSetting.m_edtTimeOut.GetModify())
		{
			m_settingData["connectTimeOut"] = m_pagePLCSetting.m_conTimeOut;
			m_pagePLCSetting.m_edtTimeOut.SetModify(FALSE);
		}
		if(m_pagePLCSetting.m_edtIP.GetModify())
		{
			m_settingData["plcIP"] = m_pagePLCSetting.m_IP;
			m_pagePLCSetting.m_edtIP.SetModify(FALSE);
		}
		if (m_pagePLCSetting.m_edtStationNum.GetModify())
		{
			m_settingData["stationNumber"] = m_pagePLCSetting.m_stationNum;
			m_pagePLCSetting.m_edtStationNum.SetModify(FALSE);			
		}
		break;
	case PageId_DataBase:
		m_pageDBSetting.UpdateData();
		if(m_pageDBSetting.m_edtUser.GetModify())
		{
			m_settingData["user"] = enp->decrypt(desKey, (LPCSTR)m_pageDBSetting.m_user);
			m_pageDBSetting.m_edtUser.SetModify(FALSE);
		}
		if(m_pageDBSetting.m_edtServer.GetModify())
		{
			m_settingData["host"] = enp->decrypt(desKey, (LPCSTR)m_pageDBSetting.m_server);
			m_pageDBSetting.m_edtServer.SetModify(FALSE);
		}
		if (m_pageDBSetting.m_edtPsw.GetModify())
		{
			m_settingData["password"] = enp->decrypt(desKey, (LPCSTR)m_pageDBSetting.m_psw);
			m_pageDBSetting.m_edtPsw.SetModify(FALSE);			
		}
		if (m_pageDBSetting.m_edtPort.GetModify())
		{
			m_settingData["port"] = enp->decrypt(desKey, (LPCSTR)m_pageDBSetting.m_port);
			m_pageDBSetting.m_edtPort.SetModify(FALSE);	
		}
		if (m_pageDBSetting.m_edtDatabase.GetModify())
		{
			m_settingData["db"] = enp->decrypt(desKey, (LPCSTR)m_pageDBSetting.m_database);
			m_pageDBSetting.m_edtDatabase.SetModify(FALSE);	
		}
		if (m_pageDBSetting.m_characterSetChanged)
		{
			m_settingData["characterSet"] = enp->decrypt(desKey, (LPCSTR)m_pageDBSetting.m_characterSet);
			m_pageDBSetting.m_characterSetChanged = FALSE;
		}
		break;
	case PageId_Printer:
		m_pagePrinterSetting.UpdateData();
		if (m_pagePrinterSetting.m_edtThermalPrtPort.GetModify())
		{
			m_settingData["printerPort"] = m_pagePrinterSetting.m_thermalPrtPort;
			m_pagePrinterSetting.m_edtThermalPrtPort.SetModify(FALSE);	
		}
		if (m_pagePrinterSetting.m_edtThermalPrtBaud.GetModify())
		{
			m_settingData["baudRate"] = m_pagePrinterSetting.m_thermalPrtBaud;
			m_pagePrinterSetting.m_edtThermalPrtBaud.SetModify(FALSE);	
		}
		if (m_pagePrinterSetting.m_edtThermalPrtPaper.GetModify())
		{
			m_settingData["rowWidth"] = m_pagePrinterSetting.m_thermalPrtPaper;
			m_pagePrinterSetting.m_edtThermalPrtPaper.SetModify(FALSE);	
		}
		if (m_pagePrinterSetting.m_edtSatoPrtPort.GetModify())
		{
			m_settingData["SatoPrinterPort"] = m_pagePrinterSetting.m_satoPrtPort;
			m_pagePrinterSetting.m_edtSatoPrtPort.SetModify(FALSE);	
		}
		if (m_pagePrinterSetting.m_edtSatoPrtBaud.GetModify())
		{
			m_settingData["SatobaudRate"] = m_pagePrinterSetting.m_satoPrtBaud;
			m_pagePrinterSetting.m_edtSatoPrtBaud.SetModify(FALSE);	
		}
		break;
	case PageId_SysMonitor:
		m_pageSysMonitorSetting.UpdateData();
		if (m_pageSysMonitorSetting.m_edtBoxFrequency.GetModify())
		{
			m_settingData["DoseMonitorFrequency"] = m_pageSysMonitorSetting.m_boxFrequency;
			m_pageSysMonitorSetting.m_edtBoxFrequency.SetModify(FALSE);	
		}
		if (m_pageSysMonitorSetting.m_edtDeviceFrequency.GetModify())
		{
			m_settingData["DeviceMonitorFrequency"] = m_pageSysMonitorSetting.m_deviceFrequency;
			m_pageSysMonitorSetting.m_edtDeviceFrequency.SetModify(FALSE);	
		}
		break;
	default:
		break;
	}
}

/*
void CSystemSetting::ShowMessage(UINT ids, long code)
{
	CString str;
	str.LoadString(ids);
	if (code)
	{
		CString tmp;
		tmp.Format("code: %d",code);
		str = str + tmp;
	}
	MessageBox(str);	
}*/
