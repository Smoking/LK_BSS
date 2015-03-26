// DeviceMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "DeviceMonitor.h"
#include "ConfigUtil.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceMonitor dialog


CDeviceMonitor::CDeviceMonitor(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceMonitor)
	//}}AFX_DATA_INIT
	medMag=NULL;
    medMag=new CMedicineManage();
}

CDeviceMonitor::~CDeviceMonitor()
{
	delete medMag;
	medMag=NULL;
}

void CDeviceMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceMonitor)
	DDX_Control(pDX, IDC_CHECKEMPORT, m_checkEmport);
	DDX_Control(pDX, IDC_CHECKSTOP, m_checkStop);
	DDX_Control(pDX, IDC_LISTMED, m_listMed);
	DDX_Control(pDX, IDC_LISTDEVICE, m_listDevice);
	DDX_Control(pDX, IDC_COMBO, m_medUnit);
	DDX_Control(pDX, IDC_CHECKLACK, m_checkLack);
	DDX_Control(pDX, IDC_BUTBACK, m_btnBack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceMonitor, CDialog)
	//{{AFX_MSG_MAP(CDeviceMonitor)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECKLACK, OnChecklack)
	ON_BN_CLICKED(IDC_BUTBACK, OnButback)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	ON_BN_CLICKED(IDC_CHECKSTOP, OnCheckstop)
	ON_BN_CLICKED(IDC_CHECKEMPORT, OnCheckemport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceMonitor message handlers
BOOL CDeviceMonitor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitDeviceList();
	InitMedList();
    int i = 0;
	for(i = 1;i<7;i++)
	{
        m_medUnit.AddString(Utils::itos(i).c_str());
    }
	m_medUnit.AddString("All");
	m_medUnit.SetCurSel(i-1);
    
	//返回
	CString strNormalPath	= Util::GetDialogResPath() + "\\back_normal.bmp";
	CString strDownPath		= Util::GetDialogResPath() + "\\back_down.bmp";
	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    m_btnBack.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	
	SetTimer(2,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeviceMonitor::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}


BOOL CDeviceMonitor::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
 	GetClientRect(&rect);
 	CBrush brush(GROUP_BK_COLOR);
 	pDC->FillRect(rect, &brush);

	CRect rcRet, rcWnd;
	GetWindowRect(rcWnd);
	
	rcRet = rcWnd;
	rcRet.top = rcWnd.bottom - 73;
	rcRet.OffsetRect(-rcWnd.left, -rcWnd.top);
	
	//绘制底部区域
	CString strPath = Util::GetDialogResPath() + "\\BorderBottom.bmp";
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	Util::xTileRgnWithBmp(pDC, rcRet, hBitmap);
	DeleteObject(hBitmap);
	return TRUE;
}

void CDeviceMonitor::InitDeviceList()
{
	m_listDevice.Init();
	m_listDevice.SetTextColor(RGB(125,125,125));
	m_listDevice.InsertColumn(0, "监控", LVCFMT_LEFT, 120);
	m_listDevice.InsertColumn(1, "状态", LVCFMT_LEFT, 200);
	m_listDevice.SetRedraw(FALSE);
	m_listDevice.SetRedraw(TRUE);
	ListView_SetExtendedListViewStyle(m_listDevice.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);
}

void CDeviceMonitor::InitMedList()
{
	m_listMed.Init();
	m_listMed.SetTextColor(RGB(125,125,125));	
	m_listMed.InsertColumn(0, "地址", LVCFMT_LEFT, 70);
	m_listMed.InsertColumn(1, "药品名", LVCFMT_LEFT,120);
	m_listMed.InsertColumn(2, "规格", LVCFMT_LEFT, 80);
	m_listMed.InsertColumn(3, "厂家", LVCFMT_LEFT, 120);
	m_listMed.InsertColumn(4, "当前数量", LVCFMT_LEFT, 80);
	m_listMed.InsertColumn(5, "最大数量", LVCFMT_LEFT, 80);
	m_listMed.InsertColumn(6, "状态", LVCFMT_LEFT, 90);
	m_listMed.SetRedraw(FALSE);
	m_listMed.SetRedraw(TRUE);
	ListView_SetExtendedListViewStyle(m_listMed.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);
}


BOOL CDeviceMonitor::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//屏蔽回车，退出按钮
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))    
		return   TRUE;   
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDeviceMonitor::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==2)
	{
		KillTimer(2);
		m_listDevice.DeleteAllItems();
		map<string,int>win_state;
		map<string,int>::iterator iter;
		medMag->getWindowState(&win_state);
		int i=0;
		for(iter=win_state.begin();iter!=win_state.end();iter++)
		{
			m_listDevice.InsertItem(i, _T(iter->first.c_str()));
			if(iter->second==1)
				m_listDevice.SetItemText(i, 1, _T("开启"));
			else if(iter->second==0)
				m_listDevice.SetItemText(i, 1, _T("关闭"));
			i++;
		}
		
		int sharp_StopFlag=pBoxMonitor->getsharp_StopFlag();
		if(sharp_StopFlag==0)
		{
			m_listDevice.InsertItem(i, _T("盒剂发药急停开关"));
			m_listDevice.SetItemText(i, 1, _T("关闭"));
			i++;
		}
		else if(sharp_StopFlag==1)
		{
			m_listDevice.InsertItem(i, _T("盒剂发药急停开关"));
			m_listDevice.SetItemText(i, 1, _T("开启"));
			i++;
		}
		if(pBoxMonitor->getDBState()==0)
		{
			m_listDevice.InsertItem(i, _T("数据库连接"));
			m_listDevice.SetItemText(i, 1, _T("正常"));
			i++;
		}
		else
		{
			m_listDevice.InsertItem(i, _T("数据库连接"));
			m_listDevice.SetItemText(i, 1, _T("断开"));
			i++;
		}
		
		if(pBoxMonitor->getPLCState()==0)
		{
			m_listDevice.InsertItem(i, _T("PLC连接"));
			m_listDevice.SetItemText(i, 1, _T("正常"));
			i++;
		}
		else
		{
			m_listDevice.InsertItem(i, _T("PLC连接"));
			m_listDevice.SetItemText(i, 1, _T("断开"));
			i++;
		}
		SetTimer(2,1000,NULL);
	}
	else if(nIDEvent==1)
    {
		KillTimer(1);
		getMedMonitor();
		SetTimer(1,configUtil.getInt("DeviceMonitorFrequency"),NULL);
	}
	CDialog::OnTimer(nIDEvent);
}

void CDeviceMonitor::getMedMonitor()
{
    map<int,MedicinLocationState>medlocationstate;
	map<int,MedicinLocationState>::iterator iter;
	if(medMag->getMedicinLocation(&medlocationstate)==0)
	{
		int row=0;
		int line=0;
		int unit=0;
		int i = 0;
		m_listMed.DeleteAllItems();
		for(iter=medlocationstate.begin();iter!=medlocationstate.end();iter++)
		{
			CString strLocation;
			int locationId = iter->first;
			unit=(locationId-1)/320+1;
			locationId -= (unit-1)*320;
			row=(locationId-1)%32+1;
			line=(locationId-1)/32+1;
            CString str;
			int item = m_medUnit.GetCurSel();
			m_medUnit.GetLBText(item,str);
			if(str != "All")
			{
				int m_unit = atoi(str);
				if( unit != m_unit)
				{
					continue;
				}
			}
			strLocation.Format("%d-%d-%d",unit,line,row);

			COLORREF clrStatus;
			int leave=iter->second.max_capacity*iter->second.warning_percent/100;
			if(iter->second.medicine_id==-1 && iter->second.max_capacity==-1 && iter->second.cur_quantity==-1)
			{
                clrStatus = COLOR_EMPORT;
			}
			else if(iter->second.location_state==1)
			{
				if(iter->second.medicine_id==0 && iter->second.max_capacity==0 && iter->second.cur_quantity==0)
				{
					clrStatus = COLOR_BLUE;
				}
				else
				{
					if(iter->second.med_warning_falg==1)
					{
					    clrStatus = COLOR_RED;
					}
					else if(iter->second.cur_quantity>leave)
					{
			            clrStatus = COLOR_GREEN;
					}
			        else 
					{
			    	    clrStatus = COLOR_YELLOW;
					}
				}
			}
			else 
			{
			    clrStatus = COLOR_GRAY;
			}

			if(m_checkStop.GetCheck())
			{
				if(clrStatus == COLOR_GRAY)
				{
					m_listMed.InsertItem(i,_T(strLocation));
					m_listMed.SetItemText(i, 1, _T(iter->second.med_name.c_str()));
					m_listMed.SetItemText(i, 2, _T(iter->second.med_unit.c_str()));
					m_listMed.SetItemText(i, 3, _T(iter->second.med_factory.c_str()));
					m_listMed.SetItemText(i, 4, _T(Utils::itos(iter->second.cur_quantity).c_str()));
					m_listMed.SetItemText(i, 5, _T(Utils::itos(iter->second.max_capacity).c_str()));
					m_listMed.SetItemText(i, 6, _T("停用"));
					m_listMed.SetItemColor(i,COLOR_RED);
					i++;
				}
			}
			else if(m_checkEmport.GetCheck())
			{
				if(clrStatus == COLOR_BLUE)
				{
					m_listMed.InsertItem(i,_T(strLocation));
					m_listMed.SetItemText(i, 1, _T(iter->second.med_name.c_str()));
					m_listMed.SetItemText(i, 2, _T(iter->second.med_unit.c_str()));
					m_listMed.SetItemText(i, 3, _T(iter->second.med_factory.c_str()));
					m_listMed.SetItemText(i, 4, _T(Utils::itos(iter->second.cur_quantity).c_str()));
					m_listMed.SetItemText(i, 5, _T(Utils::itos(iter->second.max_capacity).c_str()));
					m_listMed.SetItemText(i, 6, _T("未分配"));
					m_listMed.SetItemColor(i,LIST_BK_COLOR);
					i++;
				}
			}
			else if( m_checkLack.GetCheck())
			{
				if (clrStatus == COLOR_YELLOW || clrStatus == COLOR_RED) 
				{	
					m_listMed.InsertItem(i,_T(strLocation));
					m_listMed.SetItemText(i, 1, _T(iter->second.med_name.c_str()));
					m_listMed.SetItemText(i, 2, _T(iter->second.med_unit.c_str()));
					m_listMed.SetItemText(i, 3, _T(iter->second.med_factory.c_str()));
					m_listMed.SetItemText(i, 4, _T(Utils::itos(iter->second.cur_quantity).c_str()));
					m_listMed.SetItemText(i, 5, _T(Utils::itos(iter->second.max_capacity).c_str()));
					m_listMed.SetItemText(i, 6, _T("缺药"));
					m_listMed.SetItemColor(i,COLOR_RED);
					i++;
				}
			}
			else
			{
				if(clrStatus == COLOR_GREEN || clrStatus == COLOR_YELLOW || clrStatus == COLOR_RED)
				{
					m_listMed.InsertItem(i,_T(strLocation));
					m_listMed.SetItemText(i, 1, _T(iter->second.med_name.c_str()));
					m_listMed.SetItemText(i, 2, _T(iter->second.med_unit.c_str()));
					m_listMed.SetItemText(i, 3, _T(iter->second.med_factory.c_str()));
					m_listMed.SetItemText(i, 4, _T(Utils::itos(iter->second.cur_quantity).c_str()));
					m_listMed.SetItemText(i, 5, _T(Utils::itos(iter->second.max_capacity).c_str()));
					if(clrStatus == COLOR_GREEN)
					{
						m_listMed.SetItemText(i, 6, _T("正常"));
						m_listMed.SetItemColor(i,LIST_BK_COLOR);
					}
					else if(clrStatus == COLOR_YELLOW || clrStatus == COLOR_RED)
					{
						m_listMed.SetItemText(i, 6, _T("缺药"));
						m_listMed.SetItemColor(i,COLOR_RED);
					}
					i++;
				}
				else if(clrStatus == COLOR_BLUE)
				{
					m_listMed.InsertItem(i,_T(strLocation));
					m_listMed.SetItemText(i, 1, _T(iter->second.med_name.c_str()));
					m_listMed.SetItemText(i, 2, _T(iter->second.med_unit.c_str()));
					m_listMed.SetItemText(i, 3, _T(iter->second.med_factory.c_str()));
					m_listMed.SetItemText(i, 4, _T(Utils::itos(iter->second.cur_quantity).c_str()));
					m_listMed.SetItemText(i, 5, _T(Utils::itos(iter->second.max_capacity).c_str()));
					m_listMed.SetItemText(i, 6, _T("未分配"));
					m_listMed.SetItemColor(i,LIST_BK_COLOR);
					i++;
				}
				else if(clrStatus == COLOR_GRAY)
				{
					m_listMed.InsertItem(i,_T(strLocation));
					m_listMed.SetItemText(i, 1, _T(iter->second.med_name.c_str()));
					m_listMed.SetItemText(i, 2, _T(iter->second.med_unit.c_str()));
					m_listMed.SetItemText(i, 3, _T(iter->second.med_factory.c_str()));
					m_listMed.SetItemText(i, 4, _T(Utils::itos(iter->second.cur_quantity).c_str()));
					m_listMed.SetItemText(i, 5, _T(Utils::itos(iter->second.max_capacity).c_str()));
					m_listMed.SetItemText(i, 6, _T("停用"));
					m_listMed.SetItemColor(i,COLOR_RED);
					i++;
				}
			}
		}
	}
}

void CDeviceMonitor::OnButback() 
{
	// TODO: Add your control notification handler code here
	::PostMessage(hwnd,WM_THREADSTATE,2,3);
	KillTimer(1);
}

HBRUSH CDeviceMonitor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC)  //针对所有的标签
	{
		pDC->SetBkMode( TRANSPARENT );
		pDC->SetTextColor(RGB(255, 255, 255));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

void CDeviceMonitor::OnSelchangeCombo() 
{
	// TODO: Add your control notification handler code here
	getMedMonitor();
}

void CDeviceMonitor::OnChecklack() 
{
	// TODO: Add your control notification handler code here
	if(m_checkLack.GetCheck())
	{
		m_checkStop.SetCheck(FALSE);
		m_checkEmport.SetCheck(FALSE);
	}
	getMedMonitor();
}

void CDeviceMonitor::OnCheckstop() 
{
	// TODO: Add your control notification handler code here
	if(m_checkStop.GetCheck())
	{
		m_checkLack.SetCheck(FALSE);
		m_checkEmport.SetCheck(FALSE);
	}
	getMedMonitor();
}

void CDeviceMonitor::OnCheckemport() 
{
	// TODO: Add your control notification handler code here
	if(m_checkEmport.GetCheck())
	{
		m_checkStop.SetCheck(FALSE);
		m_checkLack.SetCheck(FALSE);
	}
	getMedMonitor();
}
