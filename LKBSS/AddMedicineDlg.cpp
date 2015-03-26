// AddMedicineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "AddMedicineDlg.h"
#include "Util.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TABWND_MARGIN				10		//tab离对话框边框的间距

#define INFOZONE_TABWND_WIDTH		340		//InfoZoneTabWnd的宽度
#define INFOZONE_TABWND_HEIGHT		480		//InfoZoneTabWnd的高度*/

/////////////////////////////////////////////////////////////////////////////
//CBIZFillMedicine* Singleton<CBIZFillMedicine>::ptr_ = NULL;
//size_t Singleton<CBIZFillMedicine>::cnt_ = 0;

LoggerPtr CAddMedicineDlg::rootLogger = Logger::getLogger("AddMedicineDlg");  //初始化日志
CAddMedicineDlg::CAddMedicineDlg(CWnd* pParent /*=NULL*/)
	: CRCDialog(CAddMedicineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddMedicineDlg)
	//}}AFX_DATA_INIT
	
//	this->bizfm = singlebizfm.getInst();
	this->bizfm = new CBIZFillMedicine();
	this->medManage = new CMedicineManage();
	this->maxNumberPerUnit = 32; //盒剂每层最大地址数
	max_Muallocation = configUtil.getInt("MualAddMedUnit")*320 ;
	cdb = new CDBManage();
}

CAddMedicineDlg::~CAddMedicineDlg(){
	if (bizfm != NULL)
	{
		bizfm->finishFilling(0,0);
		bizfm->finishFilling(0,1);
		bizfm->finishFilling(0,2);
		bizfm->finishFilling(0,3);
		delete bizfm;
		bizfm = NULL ;
		//singlebizfm.releaseInst(bizfm);
	}
	if (medManage != NULL)
	{
		delete medManage;
		medManage = NULL ;
		//singletonMed.releaseInst(medManage);
	}
	if (cdb != NULL)
	{
		cdb->close();
		delete cdb;
		cdb = NULL ;
	}
}
void CAddMedicineDlg::DoDataExchange(CDataExchange* pDX)
{
	CRCDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddMedicineDlg)
	DDX_Control(pDX, IDC_StartAdd, m_btnStartAdd);
	DDX_Control(pDX, IDC_ExitAdd, m_btnExitAdd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddMedicineDlg, CRCDialog)
	//{{AFX_MSG_MAP(CAddMedicineDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_StartAdd, OnStartAdd)
	ON_BN_CLICKED(IDC_ExitAdd, OnExitAdd)
	ON_MESSAGE(WM_FINISH_ADD, OnFinishFilling)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddMedicineDlg message handlers

BOOL CAddMedicineDlg::OnInitDialog() 
{
	CRCDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText("国控智能盒剂快速发药系统-----添加药品");
	InitButton();
	if(addType==1)
	{
	    InitData(locationList);
		OnStartAdd();
	}
	else if(addType==2)
	{
		InitData(medicineList);
		OnStartAdd();
	}
	else if (addType==3)
	{
		InitData(medicineList);
		OnStartAdd();
	}
	return TRUE;
}

void CAddMedicineDlg::InitButton()
{
	CString strNormalPath	= Util::GetDialogResPath() + "\\ExitAddMed_normal.bmp";
	CString strDownPath		= Util::GetDialogResPath() + "\\ExitAddMed_down.bmp";
	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    m_btnExitAdd.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
}

int CAddMedicineDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRCDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	
	// TODO: Add your specialized creation code here
	CreateInfoZone();		//创建信息区域
	CreateOpZone();			//创建操作区域		
	return 0;
}

void CAddMedicineDlg::CreateInfoZone()
{
	//创建信息区域TabWnd
	if (!m_infoZoneTabWnd.Create(NULL, NULL, WS_CHILD|WS_VISIBLE,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))	
	{
		return ;
	}
	m_infoZoneTabWnd.SetTabCount(1);
	//加药信息页面创建
	m_infoZonePage.Create(IDD_AddMed_InfoZone, &m_infoZoneTabWnd);
	m_infoZoneTabWnd.SetTabWnd(0, &m_infoZonePage);
	//设置区域大小
	CRect rcClient, rcInfoTab;
	GetClientRect(&rcClient);
	rcInfoTab.left = rcClient.left + TABWND_MARGIN;
	rcInfoTab.top = rcClient.top + TABWND_MARGIN;
	rcInfoTab.right = rcInfoTab.left + INFOZONE_TABWND_WIDTH;
	rcInfoTab.bottom = rcInfoTab.top + INFOZONE_TABWND_HEIGHT + TABWND_MARGIN;
	m_infoZoneTabWnd.MoveWindow(CRect(rcInfoTab));
}

void CAddMedicineDlg::CreateOpZone()
{
	//创建操作区域
	if (!m_OpZonePage.Create(IDD_AddMed_OpZone, this))	
	{
		return ;
	}

	//移动位置
	CRect rcClient, rcOpZone;
	GetClientRect(&rcClient);
	m_OpZonePage.GetClientRect(&rcOpZone);

	int nWidth = rcOpZone.Width();
	int nHeight = rcOpZone.Height();

	rcOpZone.left = rcClient.left + TABWND_MARGIN + INFOZONE_TABWND_WIDTH;
	rcOpZone.top = rcClient.top ;
	rcOpZone.right = rcOpZone.left + nWidth;
	rcOpZone.bottom = rcOpZone.top + nHeight;

	m_OpZonePage.setParentHwnd(this->m_hWnd);
	m_OpZonePage.MoveWindow(rcOpZone, FALSE);
	m_OpZonePage.ShowWindow(SW_SHOW);
	m_OpZonePage.maxNumberPerUnit = maxNumberPerUnit;
}


void CAddMedicineDlg::OnStartAdd() 
{
	// TODO: Add your control notification handler code here
	//CString strText;
	//strText.LoadString(IDS_StartFilling);
	//if(Util::MessageBox(this->GetSafeHwnd(), strText)==IDOK)
	{
		int errorCode = bizfm->initFilling();
		int count = bizfm->getVfs().size();
		switch(count) {
		case 4:
			this->m_OpZonePage.m_btnOK4.EnableWindow(TRUE);
			this->m_OpZonePage.m_btnFullFill4.EnableWindow(TRUE);
			bizfm->fillMedicine(3);
		case 3:
			this->m_OpZonePage.m_btnOK3.EnableWindow(TRUE);
			this->m_OpZonePage.m_btnFullFill3.EnableWindow(TRUE);
			bizfm->fillMedicine(2);
		case 2:
			this->m_OpZonePage.m_btnOK2.EnableWindow(TRUE);
			this->m_OpZonePage.m_btnFullFill2.EnableWindow(TRUE);
			bizfm->fillMedicine(1);
		case 1:
			this->m_OpZonePage.m_btnOK.EnableWindow(TRUE);
			this->m_OpZonePage.m_btnFullFill1.EnableWindow(TRUE);
			bizfm->fillMedicine(0);
		default:
			1==1;
		}
		this->m_btnStartAdd.EnableWindow(false);
		
		if (errorCode != 0)
		{
			CString strText=configUtil.getValue(Utils::itos(errorCode)).c_str();
			Util::MessageBox(this->GetSafeHwnd(), strText,"提示",RCMB_OK);
		}
	}
}

void CAddMedicineDlg::OnExitAdd() 
{
	// TODO: Add your control notification handler code here
	CRCDialog::OnOK();
}

void CAddMedicineDlg::OnFinishFilling(WPARAM wParam, LPARAM lParam){ //自定义消息处理
	if (this->m_infoZonePage.m_listAddMed.GetItemCount()>0)
	{
		this->m_infoZonePage.m_listAddMed.DeleteItem(0);
	}
	
	int fmPlace = (int)wParam;
	int isLastOne = (int)lParam;

	rootLogger->info("fmPlace = "+Utils::itos(fmPlace)+"   isLastOne = "+Utils::itos(isLastOne));
	if (this->m_infoZonePage.m_listAddMed.GetItemCount() == 0)
	{
		if (isLastOne == 0)
		{
			switch(fmPlace) {
			case 0:
				this->m_OpZonePage.m_btnOK.EnableWindow(false);
				this->m_OpZonePage.m_btnFullFill1.EnableWindow(false);
				break;
			case 1:
				this->m_OpZonePage.m_btnOK2.EnableWindow(false);
				this->m_OpZonePage.m_btnFullFill2.EnableWindow(false);
				break;
			case 2:
				this->m_OpZonePage.m_btnOK3.EnableWindow(false);
				this->m_OpZonePage.m_btnFullFill3.EnableWindow(false);
				break;
			case 3:
				this->m_OpZonePage.m_btnOK4.EnableWindow(false);
				this->m_OpZonePage.m_btnFullFill4.EnableWindow(false);
				break;
			default:
				1==1;
			}
		}
	}
}

void CAddMedicineDlg::InitData(list<CLocation> locationList)
{
	
	bizfm->sendLocationToQueue(locationList);
	
	list<CLocation>::iterator ptr = locationList.begin();

	for (int i =0 ;ptr != locationList.end();ptr++,i++)
	{		
		CLocation tmp = *ptr;
		if (tmp.getLocationState() == 0 || tmp.getLocationId() > max_Muallocation )
		{
			continue;
		}
		if (i > 3)
		{
			MedInfo med;
			int medId = tmp.getMedicineId();
			medManage->lookMedicine(medId,&med);
			m_infoZonePage.m_listAddMed.InsertItem(i-4,Utils::itos(tmp.getLocationId()).c_str());
			m_infoZonePage.m_listAddMed.SetItemText(i-4,0,Utils::itos(tmp.getLocationId()).c_str());
			m_infoZonePage.m_listAddMed.SetItemText(i-4,1,med.medicineNameCH.c_str());
			m_infoZonePage.m_listAddMed.SetItemText(i-4,2,Utils::itos(tmp.getCurQuantity()).c_str());
			m_infoZonePage.m_listAddMed.SetItemText(i-4,3,Utils::itos(tmp.getMaxCapacity()).c_str());
			int locstate = tmp.getLocationState();
			CString sState;
			if (locstate == 1)
			{
				sState.LoadString(IDS_LocationUsable);
			}else if (locstate == 0)
			{
				sState.LoadString(IDS_LocationDisable);
			}
			m_infoZonePage.m_listAddMed.SetItemText(i-4,4,sState);
		}else
		{
			m_OpZonePage.displayLocationDetail(tmp.getLocationId(),i);
		}
	}
}

void CAddMedicineDlg::InitData(list<CMedicine> medicineList)
{
	bizfm->sendMedicineToQueue(medicineList);
	MedInfo med;
	list<CMedicine>::iterator ptr = medicineList.begin();

	for (int i =0 ,j=0;ptr != medicineList.end();ptr++)
	{
		CMedicine tmp = *ptr;
		int medId = tmp.getMedicineId();
		CString medName = tmp.getMedicineNameCH().c_str();

		list<MedicinLocationState> locList;
		MedicinLocationState tmp2;

		medManage->addressSearch(medId,&locList);
		list<MedicinLocationState>::iterator ptr2 = locList.begin();
		for (;ptr2 != locList.end();ptr2++)
		{
			tmp2 = *ptr2;
			if (tmp2.location_state == 0 || tmp2.location_id > max_Muallocation )
			{
				continue;
			}
			if (j > 3)
			{	
				m_infoZonePage.m_listAddMed.InsertItem(i-4,Utils::itos(tmp2.location_id).c_str());
				m_infoZonePage.m_listAddMed.SetItemText(i-4,0,Utils::itos(tmp2.location_id).c_str());
				m_infoZonePage.m_listAddMed.SetItemText(i-4,1,medName);
				m_infoZonePage.m_listAddMed.SetItemText(i-4,2,Utils::itos(tmp2.cur_quantity).c_str());
				m_infoZonePage.m_listAddMed.SetItemText(i-4,3,Utils::itos(tmp2.max_capacity).c_str());
				int locstate = tmp2.location_state;
				CString sState;
				if (locstate == 1)
				{
					sState.LoadString(IDS_LocationUsable);
				}else if (locstate == 0)
				{
					sState.LoadString(IDS_LocationDisable);
				}
				m_infoZonePage.m_listAddMed.SetItemText(i-4,4,sState);
			}else
			{
				m_OpZonePage.displayLocationDetail(tmp2.location_id,i);
			}
			i++;
			j++;
		}
	}
}


