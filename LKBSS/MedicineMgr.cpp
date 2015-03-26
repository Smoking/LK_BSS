// MedicineMgr.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "MedicineMgr.h"
#include "Utils.h"
#include "AddMedicineDlg.h"
#include "ConfigUtil.h"
extern bool isRunning;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQueryMedicine dialog
bool CQueryMedicine::falg1=true;
bool CQueryMedicine::falg2=true;

CQueryMedicine::CQueryMedicine(CWnd* pParent /*=NULL*/)
	: CRCDialog(CQueryMedicine::IDD, pParent), m_click(0)
{
	//{{AFX_DATA_INIT(CQueryMedicine)
	m_Allcheck = FALSE;
	m_medInMachine = TRUE;
	m_medState = FALSE;
	m_medLack = FALSE;
	//}}AFX_DATA_INIT
   pmedicinList = new list<MedicinList>;
}

CQueryMedicine::~CQueryMedicine()
{
    delete pmedicinList;
}

void CQueryMedicine::DoDataExchange(CDataExchange* pDX)
{
	CRCDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQueryMedicine)
	DDX_Control(pDX, IDB_PanDian, m_PanDian);
	DDX_Control(pDX, IDC_BUTTON3, m_close);
	DDX_Control(pDX, IDC_COMBO1, m_medName);
	DDX_Control(pDX, IDC_QueryMed, m_btnQuery);
	DDX_Control(pDX, IDC_listMedInfo, m_listMedInfo);
	DDX_Control(pDX, IDB_WatchMed, m_btnWatchMed);
	DDX_Control(pDX, IDB_EditMed, m_btnEditMed);
	DDX_Check(pDX, IDC_CHECKALL, m_Allcheck);
	DDX_Check(pDX, IDC_CHECKBOX, m_medInMachine);
	DDX_Check(pDX, IDC_CHECKMEDSTATE, m_medState);
	DDX_Check(pDX, IDC_CHECKLACKMED, m_medLack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQueryMedicine, CRCDialog)
	//{{AFX_MSG_MAP(CQueryMedicine)
	ON_BN_CLICKED(IDB_WatchMed, OnWatchMedicineInfo)
	ON_BN_CLICKED(IDC_QueryMed, OnQueryMed)
	ON_CBN_EDITCHANGE(IDC_COMBO1, OnEditchangeCombo1)
	ON_BN_CLICKED(IDB_EditMed, OnEditMed)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_CBN_SETFOCUS(IDC_COMBO1, OnSetfocusCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, OnKillfocusCombo1)
	ON_NOTIFY(NM_DBLCLK, IDC_listMedInfo, OnDblclklistMedInfo)
	ON_NOTIFY(NM_CLICK, IDC_listMedInfo, OnClicklistMedInfo)
	ON_BN_CLICKED(IDB_PanDian, OnPanDian)
	ON_BN_CLICKED(IDC_CHECKALL, OnCheckall)
	ON_BN_CLICKED(IDC_CHECKBOX, OnCheckbox)
	ON_BN_CLICKED(IDC_CHECKMEDSTATE, OnCheckmedstate)
	ON_MESSAGE(WM_DELETEMED,OnDeleteItem)
	ON_BN_CLICKED(IDC_CHECKLACKMED, OnChecklackmed)
	ON_MESSAGE(IDC_listMedInfo, OnMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueryMedicine message handlers


BOOL CQueryMedicine::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	//返回
	CString strNormalPath	= Util::GetDialogResPath() + "\\back_normal.bmp";
	CString strDownPath		= Util::GetDialogResPath() + "\\back_down.bmp";
	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    m_close.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);

	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;

	//查看
	strNormalPath	= Util::GetDialogResPath() + "\\look_normal.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\look_down.bmp";
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	m_btnWatchMed.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);

	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;
    
	if(type==2)
	{
		//开始加药
		strNormalPath	= Util::GetDialogResPath() + "\\StartAddMed_normal.bmp";
		strDownPath		= Util::GetDialogResPath() + "\\StartAddMed_down.bmp";
		hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
		hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
		m_btnEditMed.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	}
	else
	{
		//编辑
		strNormalPath	= Util::GetDialogResPath() + "\\edit_normal.bmp";
		strDownPath		= Util::GetDialogResPath() + "\\edit_down.bmp";
		hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
		hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
		m_btnEditMed.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
    }
	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;

	//盘点
	strNormalPath	= Util::GetDialogResPath() + "\\CountMed_normal.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\CountMed_down.bmp";
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	m_PanDian.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal); 
	
	med=new CMedicineManage();
	InitMedicineList();
	if(type==1)
	{
		this->SetWindowText("国控智能盒剂快速发药系统-----管理药品");
		list<string>output;
		med->fastSearch(FAST_TYPE_2,"",&output);
		list<string>::iterator iter;
		for(iter=output.begin();iter!=output.end();iter++)
		{
			m_medName.AddString((*iter).c_str());
		}
	}
	if(type==2)
	{
		this->SetWindowText("国控智能盒剂快速发药系统-----加药");
		list<string>output;
		med->fastSearch(FAST_TYPE_1,"",&output);
		list<string>::iterator iter;
		for(iter=output.begin();iter!=output.end();iter++)
		{
			m_medName.AddString((*iter).c_str());
		}
		(CButton *)GetDlgItem(IDC_CHECKBOX)->EnableWindow(FALSE);
		(CButton *)GetDlgItem(IDC_CHECKMEDSTATE)->EnableWindow(FALSE);
		m_medLack = TRUE;
		m_Allcheck=TRUE;
		CRect rect1;
	    GetClientRect(&rect1);
	    CRect rcBtn1;
	    m_close.GetWindowRect(&rcBtn1);
	    ScreenToClient(&rcBtn1);
	    int xPos1 = (rect1.Width() - rcBtn1.Width())/5;
	    int yPos1 = rcBtn1.top;
	    m_btnWatchMed.MoveWindow(xPos1, yPos1, rcBtn1.Width(), rcBtn1.Height());
		m_btnEditMed.MoveWindow(xPos1+230, yPos1, rcBtn1.Width(), rcBtn1.Height());
        m_close.MoveWindow(xPos1+450, yPos1, rcBtn1.Width(), rcBtn1.Height());
		m_PanDian.ShowWindow(SW_HIDE);
		UpdateData(FALSE);
	}
	dlg=NULL;
	dlg_edit=NULL;
	OnQueryMed();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CQueryMedicine::InitMedicineList()
{
	m_listMedInfo.Init();
	m_listMedInfo.SetTextColor(RGB(125,125,125));	
	m_listMedInfo.InsertColumn(0, "药品名称", LVCFMT_LEFT, 280);
	m_listMedInfo.InsertColumn(1, "药品编号", LVCFMT_LEFT, 0);
	m_listMedInfo.InsertColumn(2, "拼音编码", LVCFMT_LEFT, 0);
	m_listMedInfo.InsertColumn(3, "药品规格", LVCFMT_LEFT, 150);
	m_listMedInfo.InsertColumn(4, "生产厂家", LVCFMT_LEFT, 210);
	m_listMedInfo.InsertColumn(5, "当前剩余总量", LVCFMT_LEFT, 80);
    m_listMedInfo.InsertColumn(6, "所需加药数量", LVCFMT_LEFT, 80);
	m_listMedInfo.InsertColumn(7, "状态", LVCFMT_LEFT, 80);
	
	m_listMedInfo.SetRedraw(FALSE);
	m_listMedInfo.SetRedraw(TRUE);
	DWORD dwStyle = m_listMedInfo.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    dwStyle |= LVS_EX_CHECKBOXES;
    DWORD d=m_listMedInfo.SetExtendedStyle(dwStyle); 
//	ListView_SetExtendedListViewStyle(m_listMedInfo.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
}


void CQueryMedicine::OnQueryMed() 
{
	// TODO: Add your control notification handler code here
	//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT)); 
	UpdateData(TRUE);	
	CString str;	
	m_medName.GetWindowText(str);
	string medname;
	if(str.IsEmpty())
        medname="";
	else
	     medname=str;
    pmedicinList->clear();
	int result=med->listSearch(pmedicinList,medname,type,!m_medState);
	if(result!=0)
		return;
       
	m_listMedInfo.DeleteAllItems();
	list<MedicinList>::iterator iter;
	int i=-1;
	if(!pmedicinList->empty())
	{
		pmedicinList->sort();
	}
	for(iter=pmedicinList->begin();iter!=pmedicinList->end();iter++)
	{
		if(m_medState)
		{
			if(iter->medicineState != "停用")
				continue;
		}
		if(m_medInMachine)
		{
			if(iter->medicineState == "未上架")
				continue;
		}
		if(m_medLack)
		{
			if(iter->medicineState != "缺药")
				continue;
		}
		i++;
		m_listMedInfo.InsertItem(i, _T(iter->medicineNameCH.c_str()));
	    m_listMedInfo.SetItemText(i, 1, _T(Utils::itos(iter->medicineId).c_str()));
		m_listMedInfo.SetItemText(i, 2,_T(iter->medPyCode.c_str()));
		m_listMedInfo.SetItemText(i, 3, _T(iter->medUnit.c_str()));
		m_listMedInfo.SetItemText(i, 4, _T(iter->medFactory.c_str()));
		m_listMedInfo.SetItemText(i, 5, _T(Utils::itos(iter->leavingQuantity).c_str()));
		m_listMedInfo.SetItemText(i, 6, _T(Utils::itos(iter->addmedcount).c_str()));
		m_listMedInfo.SetItemText(i, 7, _T(iter->medicineState.c_str()));
		if(iter->medicineState == "缺药" || iter->medicineState == "停用")
		{
			m_listMedInfo.SetItemColor(i,COLOR_RED);
		}
		else
		{
			m_listMedInfo.SetItemColor(i,LIST_BK_COLOR);			
		}
		if(m_Allcheck)
            m_listMedInfo.SetCheck(i,TRUE);
	}
	if(m_listMedInfo.GetItemCount()>0)
		m_listMedInfo.SetItemState(0,LVNI_FOCUSED | LVNI_SELECTED, LVNI_FOCUSED | LVNI_SELECTED);
	//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW)); 
}

/************************************
 * 原函数名：OnQueryMed()
 * 修改时间：2014-10-23
 * 修改者  ：zxx
 ************************************/
/*void CQueryMedicine::OnQueryMed() 
{
	HANDLE hThread;
	DWORD dwThread;
	hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, &dwThread);    //开辟线程
	return;
}*/

DWORD WINAPI CQueryMedicine::ThreadFunc(LPVOID lpVoid)
{
	CQueryMedicine *pThis = (CQueryMedicine *)lpVoid;
    return pThis->QueryWork();
}

UINT CQueryMedicine::QueryWork()
{
	m_click++;
	int temp = m_click;	
	Sleep(500);  //延时
	PostMessage(IDC_listMedInfo, 0, temp);  //发送消息
	
	return 0;
}

//接收消息
void CQueryMedicine::OnMsg(UINT uMsg, WPARAM wPara, LPARAM lPara)
{
	//过滤掉非最大值的线程
	if(wPara != m_click)
		return;
	else
		m_click = 0;
	
	//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT)); 
	UpdateData(TRUE);	
	CString str;	
	m_medName.GetWindowText(str);
	string medname;
	if(str.IsEmpty())
        medname="";
	else
		medname=str;
    pmedicinList->clear();
	int result=med->listSearch(pmedicinList,medname,type,!m_medState);
	if(result!=0)
		return;
	
	m_listMedInfo.DeleteAllItems();
	list<MedicinList>::iterator iter;
	int i=-1;
	if(!pmedicinList->empty())
	{
		pmedicinList->sort();
	}
	for(iter=pmedicinList->begin();iter!=pmedicinList->end();iter++)
	{
		if(m_medState)
		{
			if(iter->medicineState != "停用")
				continue;
		}
		if(m_medInMachine)
		{
			if(iter->medicineState == "未上架")
				continue;
		}
		if(m_medLack)
		{
			if(iter->medicineState != "缺药")
				continue;
		}
		i++;
		m_listMedInfo.InsertItem(i, _T(iter->medicineNameCH.c_str()));
		m_listMedInfo.SetItemText(i, 1, _T(Utils::itos(iter->medicineId).c_str()));
		m_listMedInfo.SetItemText(i, 2,_T(iter->medPyCode.c_str()));
		m_listMedInfo.SetItemText(i, 3, _T(iter->medUnit.c_str()));
		m_listMedInfo.SetItemText(i, 4, _T(iter->medFactory.c_str()));
		m_listMedInfo.SetItemText(i, 5, _T(Utils::itos(iter->leavingQuantity).c_str()));
		m_listMedInfo.SetItemText(i, 6, _T(Utils::itos(iter->addmedcount).c_str()));
		m_listMedInfo.SetItemText(i, 7, _T(iter->medicineState.c_str()));
		if(iter->medicineState == "缺药" || iter->medicineState == "停用")
		{
			m_listMedInfo.SetItemColor(i,COLOR_RED);
		}
		else
		{
			m_listMedInfo.SetItemColor(i,LIST_BK_COLOR);			
		}
		if(m_Allcheck)
            m_listMedInfo.SetCheck(i,TRUE);
	}
	if(m_listMedInfo.GetItemCount()>0)
		m_listMedInfo.SetItemState(0,LVNI_FOCUSED | LVNI_SELECTED, LVNI_FOCUSED | LVNI_SELECTED);
	//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW)); 
}

void CQueryMedicine::RefreshState()
{
	UpdateData(TRUE);
	pmedicinList->clear();
	CString str;
	m_medName.GetWindowText(str);
	string medname;
	if(str.IsEmpty())
        medname="";
	else
	     medname=str;
	int result = med->listSearch(pmedicinList,medname,type,!m_medState);
	if(result!=0 )
		return;
	if(!pmedicinList->empty())
	{
		list<MedicinList>::iterator iter;
		char buf[100];
		for(iter=pmedicinList->begin();iter!=pmedicinList->end();iter++)
		{
			for(int i=0; i<m_listMedInfo.GetItemCount(); i++ )
			{
				if( m_listMedInfo.GetCheck(i) )
				{
					m_listMedInfo.GetItemText(i,1,buf,100);
					int medID=atoi(buf);
					if(medID == iter->medicineId)
					{
						m_listMedInfo.SetItemText(i, 5, _T(Utils::itos(iter->leavingQuantity).c_str()));
						m_listMedInfo.SetItemText(i, 6, _T(Utils::itos(iter->addmedcount).c_str()));
						m_listMedInfo.SetItemText(i, 7, _T(iter->medicineState.c_str()));	
						m_listMedInfo.SetCheck(i,FALSE);
					}
				}
			}
		}
	}
	m_Allcheck = FALSE;
	UpdateData(FALSE);
}

void CQueryMedicine::OnEditchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	OnQueryMed();
}


BOOL CQueryMedicine::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(med)
	{
        delete med;
		med=NULL;
	}
	if(dlg)
	{
		delete dlg;
		dlg=NULL;
	}
	if(dlg_edit)
	{
		delete dlg_edit;
		dlg_edit=NULL;
	}
	osk.close();
	return CRCDialog::DestroyWindow();
}

void CQueryMedicine::OnWatchMedicineInfo() 
{
	// TODO: Add your control notification handler code here
	if(CMedicineInfo::showfalg)
		return;	
	
	char buf[100];
	for(int i=0; i<m_listMedInfo.GetItemCount(); i++ )
    {
        if( m_listMedInfo.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
        {
            m_listMedInfo.GetItemText(i,1,buf,100);
            break;
        }
    }
	MedInfo medicineinfo;
	int medID=atoi(buf);
	if(medID!=0)
	{
		if(dlg==NULL)
		{
		    dlg =new CMedicineInfo(NULL);
			dlg->type=1;
	        dlg->Create(IDD_MedicineInfo,this);
		}
		else
		    CMedicineInfo::showfalg=true;
		med->lookMedicine(medID,&medicineinfo);
		dlg->m_btnModify.SetWindowText("查  看");
		dlg->m_editLocation.SetWindowText("查看地址");
		dlg->clearData();
	    dlg->setValue(medicineinfo);
		dlg->SetWindowText("国控智能盒剂快速发药系统-----查看药品信息");
		dlg->m_btnModify.ShowWindow(SW_HIDE);
		dlg->m_clearLocation.ShowWindow(SW_HIDE);
		dlg->ShowWindow(SW_SHOW);
	}
	
}

void CQueryMedicine::OnEditMed() 
{
	// TODO: Add your control notification handler code here
	if(CMedicineInfo::showfalg)
		return;
	if(type==2)
	{
		list<CMedicine> medicineList;
		medicineList.clear();
		char buf[100];
	    for(int i=0; i<m_listMedInfo.GetItemCount(); i++ )
		{
            if( m_listMedInfo.GetCheck(i) )
			{
				CMedicine medicine;
				m_listMedInfo.GetItemText(i,1,buf,100);
				int medID=atoi(buf);			
                medicine.setMedicineId(medID);
				m_listMedInfo.GetItemText(i,0,buf,100);
				medicine.setMedicineNameCH(buf);
				medicineList.push_back(medicine);    
			}
		}
		if(medicineList.empty())
			return;
		CAddMedicineDlg adddlg;
		adddlg.addType=2;
		adddlg.medicineList=medicineList;
		adddlg.DoModal();
		RefreshState();
		return;
	}
    else 
	{	
		char buf[100];
		for(int i=0; i<m_listMedInfo.GetItemCount(); i++ )
		{
			if( m_listMedInfo.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
			{
				m_listMedInfo.GetItemText(i,1,buf,100);
				break;
			}
		}
		MedInfo medicineinfo;
		int medID=atoi(buf);
		if(medID!=0)
		{
			if(dlg_edit==NULL)
			{
				dlg_edit =new CMedicineInfo(NULL);
				dlg_edit->type=2;
				dlg_edit->Create(IDD_MedicineInfo,this);
				dlg_edit->m_hwnd = m_hWnd;
			}
			else
				CMedicineInfo::showfalg=true;
			med->lookMedicine(medID,&medicineinfo);
			dlg_edit->clearData();
			dlg_edit->setValue(medicineinfo);
			dlg_edit->SetWindowText("国控智能盒剂快速发药系统-----编辑药品信息");
			dlg_edit->ShowWindow(SW_SHOW);
		}
	}
}


void CQueryMedicine::OnButton3() 
{
	// TODO: Add your control notification handler code here
	if(CMedicineInfo::showfalg)
		return;
	CDialog::OnCancel();
}

void CQueryMedicine::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
	m_listMedInfo.DeleteItem(int(lParam));
}


void CQueryMedicine::OnSetfocusCombo1() 
{
	// TODO: Add your control notification handler code here
	osk.setCWnd(this);
	osk.setIdd(IDC_COMBO1);
	osk.setNextIdd(IDC_QueryMed);
	osk.open();
}

void CQueryMedicine::OnKillfocusCombo1() 
{
	// TODO: Add your control notification handler code here
	if(configUtil.getInt("Openosk")!=1)
		return;
	falg1=true;
}


void CQueryMedicine::OnDblclklistMedInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnWatchMedicineInfo();
	*pResult = 0;
}


void CQueryMedicine::OnClicklistMedInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView   =   (NM_LISTVIEW*)pNMHDR;
	int   iItem   =   pNMListView->iItem;  
	if( m_listMedInfo.GetItemState(iItem, LVIS_SELECTED) == LVIS_SELECTED)
	{
		if(!m_listMedInfo.GetCheck(iItem))
		{
			m_listMedInfo.SetCheck(iItem,TRUE);	
		}
		else
		{
			m_listMedInfo.SetCheck(iItem,FALSE);	
		}
	}
	else
	{
		if(!m_listMedInfo.GetCheck(iItem))
		{
			m_listMedInfo.SetItemState(iItem,LVNI_FOCUSED | LVNI_SELECTED, LVNI_FOCUSED | LVNI_SELECTED);
		}
	}
	*pResult = 0;
}

void CQueryMedicine::OnPanDian() 
{
	// TODO: Add your control notification handler code here
	char buf[100];
	bool falg=false;
	bizser=singletonBIZ.getInst();
	for(int i=0; i<m_listMedInfo.GetItemCount(); i++ )
	{
		if( m_listMedInfo.GetCheck(i) )
		{
			m_listMedInfo.GetItemText(i,1,buf,100);
			int medID=atoi(buf);
			m_listMedInfo.GetItemText(i,0,buf,100);

			CString strText="确定要盘点药品";
			strText+=buf;
			if(Util::MessageBox(this->GetSafeHwnd(), strText)==IDOK)
			{
				if(isRunning)
				{
					Util::MessageBox(this->GetSafeHwnd(), "设备正在发药不能盘点");
					return;
				}
				falg=true;
				int sum_count=0;
				list<MedinceCount>medincecount;
				list<MedinceCount>::iterator iter;
				med->getCurrentMedicineCount(medID,&medincecount,1);	
				for(iter=medincecount.begin();iter!=medincecount.end();iter++)
					sum_count+=(*iter).medCount;
				int res=bizser->clearMedicine(medID,1,sum_count);//药品盘点
				med->clearAllLocation(medID);
				Utils::insertSystemlog("2106","盘点药品地址",buf);
				m_listMedInfo.SetCheck(i,FALSE);
				m_listMedInfo.SetItemText(i, 5, _T("0"));
				m_listMedInfo.SetItemText(i, 7, _T("缺药"));
			}
		}
	}
	singletonBIZ.releaseInst((CBIZBoxService*)bizser);
	if(falg)
	{
		Util::MessageBox(this->GetSafeHwnd(), "盘点完成,请等待篮子从最后窗口出来后再点击确定按钮","提示",RCMB_OK);
	}
}


void CQueryMedicine::OnCheckall() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_Allcheck)
	{
		for(int i=0;i<m_listMedInfo.GetItemCount();i++)
		{
		    m_listMedInfo.SetCheck(i,TRUE);
		}
	}
	else
	{
		for(int i=0;i<m_listMedInfo.GetItemCount();i++)
		{
	        m_listMedInfo.SetCheck(i,FALSE);
		}
	}
}

void CQueryMedicine::OnCheckbox() 
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
	if(m_medInMachine)
	{	
		m_medState = FALSE;	
	}
	else
	{
		m_medLack = FALSE;
		m_medState = FALSE;
	}
	UpdateData(FALSE);
 	OnQueryMed();
}

void CQueryMedicine::OnCheckmedstate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_medState)
	{
		m_medLack = FALSE;
	    m_medInMachine = TRUE;
		UpdateData( FALSE );
	}
	OnQueryMed();
}

void CQueryMedicine::OnChecklackmed() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_medLack)
	{
		m_medState = FALSE;
	    m_medInMachine = TRUE;
		UpdateData( FALSE );
	}
	OnQueryMed();
}
