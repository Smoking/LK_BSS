// MedLocationEditor.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "MedLocationEditor.h"

#include "Util.h"
#include "Utils.h"
#include "ConfigUtil.h"
extern bool isRunning;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedLocationEditor dialog


CMedLocationEditor::CMedLocationEditor(CWnd* pParent /*=NULL*/)
	: CRCDialog(CMedLocationEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMedLocationEditor)
	//}}AFX_DATA_INIT
}


void CMedLocationEditor::DoDataExchange(CDataExchange* pDX)
{
	CRCDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedLocationEditor)
	DDX_Control(pDX, IDC_BUTOPEN, m_open);
	DDX_Control(pDX, IDC_BUTSTOP, m_stop);
	DDX_Control(pDX, IDC_COMBO2, m_unallotLocation);
	DDX_Control(pDX, IDC_BUTTON2, m_add);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_allotLocation);
	DDX_Control(pDX, IDC_BUTTON1, m_delete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMedLocationEditor, CRCDialog)
	//{{AFX_MSG_MAP(CMedLocationEditor)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_CBN_DROPDOWN(IDC_COMBO1, OnDropdownCombo1)
	ON_CBN_DROPDOWN(IDC_COMBO2, OnDropdownCombo2)
	ON_BN_CLICKED(IDC_BUTOPEN, OnButopen)
	ON_BN_CLICKED(IDC_BUTSTOP, OnButstop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedLocationEditor message handlers

BOOL CMedLocationEditor::OnInitDialog() 
{
	CRCDialog::OnInitDialog();
	if(this->type==1)
	{
	    this->SetWindowText("国控智能盒剂快速发药系统-----查看药品地址");
		m_delete.EnableWindow(FALSE);
		m_add.EnableWindow(FALSE);
		m_open.EnableWindow(FALSE);
		m_stop.EnableWindow(FALSE);
	}
	else if(this->type==2)
	    this->SetWindowText("国控智能盒剂快速发药系统-----修改药品地址");

	med=new CMedicineManage();
	// TODO: Add extra initialization here
	//确定
	CString strNormalPath	= Util::GetDialogResPath() + "\\ensure_normal.bmp";
	CString strDownPath		= Util::GetDialogResPath() + "\\ensure_down.bmp";
	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	m_ok.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	
	m_list.Init();
	m_list.SetTextColor(RGB(125,125,125));
	m_list.InsertColumn(0, "位置", LVCFMT_LEFT, 120);
	m_list.InsertColumn(1, "药品数量", LVCFMT_LEFT, 120);
	m_list.InsertColumn(2, "位置状态", LVCFMT_LEFT, 120);
	m_list.InsertColumn(3, "位置编号", LVCFMT_LEFT, 0);
	
	m_list.SetRedraw(FALSE);
	m_list.SetRedraw(TRUE);
	DWORD dwStyle = m_list.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    DWORD d=m_list.SetExtendedStyle(dwStyle); 
	list<MedicinLocationState>::iterator iter;
	m_list.DeleteAllItems();
	int i=-1;
	for(iter=medlocation.begin();iter!=medlocation.end(); iter++)
	{
		i++;
		CString str;
		int row=0;
		int line=0;
		int unit=0;
		int locationid=iter->location_id;
		unit=(locationid-1)/320+1;
		locationid-=(unit-1)*320;
		row=(locationid-1)%32+1;
		line=(locationid-1)/32+1;
		str.Format("%d-%d-%d",unit,line,row);
		m_allotLocation.AddString(str);
		allotLocation.insert(pair<string,int>((string)str,iter->location_id));
		m_list.InsertItem(i, _T(str));
		m_list.SetItemText(i, 1, _T(Utils::itos(iter->cur_quantity).c_str()));
		if(iter->location_state==1)
		{
    		m_list.SetItemText(i, 2, _T("启用"));
		}
		else
		{
			m_list.SetItemText(i,2, _T("停用"));
		}
		m_list.SetItemText(i, 3, _T(Utils::itos(iter->location_id).c_str()));		
	}
	map<int,MedicinLocationState>temp;
	map<int,MedicinLocationState>::iterator iter1;
	if(0==med->getMedicinLocation(&temp))
	{
        for(iter1=temp.begin();iter1!=temp.end(); iter1++)
		{
			CString str;
			int row=0;
			int line=0;
			int unit=0;
			int locationid=iter1->second.location_id;
			unit=(locationid-1)/320+1;
			locationid-=(unit-1)*320;
			row=(locationid-1)%32+1;
			line=(locationid-1)/32+1;
			str.Format("%d-%d-%d",unit,line,row);
            if(iter1->second.medicine_id==0 && iter1->second.max_capacity==0 && iter1->second.cur_quantity==0)
			{
				m_unallotLocation.AddString(str);
				unallotLocation.insert(pair<string,int>((string)str,iter1->second.location_id));
			}
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMedLocationEditor::OnDestroy() 
{
	CRCDialog::OnDestroy();
	delete med;
	// TODO: Add your message handler code here
	
}

//删除地址
void CMedLocationEditor::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CString strText="确定要删除该地址！";
	strText="\r\n\r\n"+strText;
	if(Util::MessageBox(this->GetSafeHwnd(), strText)==IDOK)
	{
		if(isRunning)
		{
			Util::MessageBox(this->GetSafeHwnd(), "设备正在发药不能清空该地址的药品");
			return;
		}
		CString str;
		m_allotLocation.GetWindowText(str);
		map<string ,int>::iterator iter;
		iter=allotLocation.find(string(str));
		if(iter!=allotLocation.end())
		{
			int locationId=iter->second;
			unallotLocation.insert(pair<string,int>(iter->first,iter->second));
			allotLocation.erase(iter);
			for(int i=0;i<m_list.GetItemCount();i++)
			{
				char buf[100];
				memset(buf,0,100);
				m_list.GetItemText(i,0,buf,100);
				if(buf==str)
				{
					m_list.DeleteItem(i);
					break;
				}
			}
			int item=m_allotLocation.FindString(-1,str);
			m_allotLocation.DeleteString(item);
			m_allotLocation.SetWindowText("");
			bizser=singletonBIZ.getInst();
			if(0==bizser->clearLocation(locationId))
			{
				singletonBIZ.releaseInst((CBIZBoxService*)bizser);
				med->deletelocation(locationId);
				Util::MessageBox(this->GetSafeHwnd(), "\r\n\r\n请等待篮子从最后窗口出来后再点击确定按钮","提示",RCMB_OK);
			}
		}
		else
			Util::MessageBox(this->GetSafeHwnd(), "\r\n\r\n该药品不存在这个地址，删除该地址无效");
	}
}

//新增地址
void CMedLocationEditor::OnButton2() 
{
	// TODO: Add your control notification handler code here
	CString strText="确定要新增该地址！";
	strText="\r\n\r\n"+strText;
	if(Util::MessageBox(this->GetSafeHwnd(), strText)==IDOK)
	{
		CString str;
		m_unallotLocation.GetWindowText(str);
		map<string ,int>::iterator iter;
		iter=unallotLocation.find(string(str));
		if(iter!=unallotLocation.end())
		{
			map<int,MedicinLocationState>temp;
			map<int,MedicinLocationState>::iterator iter1;
			if(0==med->getMedicinLocation(&temp))
			{
				iter1 = temp.find(iter->second);
				if(iter1!= temp.end())
				{
					int i=m_list.GetItemCount();
					m_list.InsertItem(i, _T(str));
					m_list.SetItemText(i, 1, _T("0"));
					if(iter1->second.location_state==0)
					{
						m_list.SetItemText(i, 2, _T("停用"));				
					}
					else
					{	
						m_list.SetItemText(i, 2, _T("启用"));
					}
					m_list.SetItemText(i, 3, _T(Utils::itos(iter->second).c_str()));
					allotLocation.insert(pair<string,int>(iter->first,iter->second));
					unallotLocation.erase(iter);
					int item=m_unallotLocation.FindString(-1,str);
					m_unallotLocation.DeleteString(item);
					m_unallotLocation.SetWindowText("");
				}
			}
		}
		else
			Util::MessageBox(this->GetSafeHwnd(), "\r\n\r\n该地址无效,不能给该药品新增这个地址");
	}
}

void CMedLocationEditor::OnOK() 
{
	// TODO: Add extra validation here
	medlocation.clear();
	MedicinLocationState temp;
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		char buf[100];
		memset(buf,0,100);
		m_list.GetItemText(i,3,buf,100);
		temp.location_id=atoi(buf);
		
		memset(buf,0,100);
		m_list.GetItemText(i,1,buf,100);
		temp.cur_quantity=atoi(buf);

		memset(buf,0,100);
		m_list.GetItemText(i,2,buf,100);
		if(strcmp(buf,"启用")==0)
		{
		    temp.location_state=1;
		}
		else
		{
			temp.location_state=0;
		}
		medlocation.push_back(temp);
	}
	if(medlocation.size()==0)
	{
		med->updateMedType(medid,0);
	}

	CRCDialog::OnOK();
}

void CMedLocationEditor::OnDropdownCombo1() 
{
	// TODO: Add your control notification handler code here
	map<string ,int>::iterator iter;
	int j=m_allotLocation.GetCount();
	for(int i=0;i<j;i++)
	{
		m_allotLocation.DeleteString(0);
	}
	for(iter=allotLocation.begin();iter!=allotLocation.end();iter++)
	{
		m_allotLocation.AddString((iter->first).c_str());
	}
}

void CMedLocationEditor::OnDropdownCombo2() 
{
	// TODO: Add your control notification handler code here
	return ;
	map<string ,int>::iterator iter;
	int j=m_unallotLocation.GetCount();
	for(int i=0;i<j;i++)
	{
		m_unallotLocation.DeleteString(0);
	}
	for(iter=unallotLocation.begin();iter!=unallotLocation.end();iter++)
	{
		m_unallotLocation.AddString((iter->first).c_str());
	}
}

void CMedLocationEditor::OnButopen() 
{
	// TODO: Add your control notification handler code here
	int item = this->m_list.GetNextItem(-1,LVNI_SELECTED);
	if (item != -1 )
	{
		char buf[100];
		memset(buf,0,100);
		m_list.GetItemText(item,2,buf,100);
		if(strcmp(buf,"启用")==0)
			return ;
		memset(buf,0,100);
		m_list.GetItemText(item,3,buf,100);
		int locationId = atoi(buf);
		list<int>list_location;
		list_location.push_back(locationId);
		int res = med->modifyMedicinLocation(list_location,1);
		if(res == 0)
		{
			m_list.SetItemText(item, 2, _T("启用"));
		}
	}
}

void CMedLocationEditor::OnButstop() 
{
	// TODO: Add your control notification handler code here
	int item = this->m_list.GetNextItem(-1,LVNI_SELECTED);
	if (item != -1 )
	{
		char buf[100];
		memset(buf,0,100);
		m_list.GetItemText(item,2,buf,100);
		if(strcmp(buf,"停用")==0)
			return ;
		memset(buf,0,100);
		m_list.GetItemText(item,3,buf,100);
		int locationId = atoi(buf);
		list<int>list_location;
		list_location.push_back(locationId);
		int res = med->modifyMedicinLocation(list_location,0);
		if(res == 0)
		{
			m_list.SetItemText(item, 2, _T("停用"));
		}
	}
}
