// MedicineInfo.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "MedicineInfo.h"
#include "Utils.h"
#include "MedicineMgr.h"
#include "ConfigUtil.h"
extern bool isRunning;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMedicineInfo dialog
bool CMedicineInfo::showfalg=false;

CMedicineInfo::CMedicineInfo(CWnd* pParent /*=NULL*/)
	: CRCDialog(CMedicineInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMedicineInfo)
	m_medCNname = _T("");
	m_medHISID = _T("");
	m_medENname = _T("");
	m_medUnit = _T("");
	m_typeUnit = _T("");
	m_medID = _T("");
	m_factoryName = _T("");
	m_medPYcode = _T("");
	m_medCUSTcode = _T("");
	m_pos = _T("");
	m_max_once = _T("");
	m_maxCapacity = _T("");
	m_validate_time = _T("");
	m_length = _T("");
	m_width = _T("");
	m_high = _T("");
	m_medCount = _T("");
	m_medbatch = _T("");
	m_Count = _T("");
	//}}AFX_DATA_INIT
	m_filePath= _T("");
	char path[MAX_PATH];
	memset(path, 0, MAX_PATH);
	GetModuleFileName(NULL, path, MAX_PATH);
	strPath = path; 
	int index = strPath.ReverseFind('\\');   
	strPath = strPath.Left(index); 
	CString strRelativePath;
	strRelativePath.LoadString(IDS_MedPictureRes);
	strPath = strPath + "\\" + strRelativePath+"\\";
}


void CMedicineInfo::DoDataExchange(CDataExchange* pDX)
{
	CRCDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMedicineInfo)
	DDX_Control(pDX, IDC_BUTTON7, m_start);
	DDX_Control(pDX, IDC_BUTTON6, m_selectPicture);
	DDX_Control(pDX, IDC_CLEARLOCATION, m_clearLocation);
	DDX_Control(pDX, IDC_LIST2, m_locationList);
	DDX_Control(pDX, IDC_BUTTON3, m_editLocation);
	DDX_Control(pDX, IDC_Slider_Warning, m_sliderWarning);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnModify);
	DDX_Text(pDX, IDC_EDIT2, m_medCNname);
	DDX_Text(pDX, IDC_EDIT4, m_medHISID);
	DDX_Text(pDX, IDC_EDIT3, m_medENname);
	DDX_Text(pDX, IDC_EDIT8, m_medUnit);
	DDX_Text(pDX, IDC_EDIT9, m_typeUnit);
	DDX_Text(pDX, IDC_EDIT1, m_medID);
	DDX_Text(pDX, IDC_EDIT10, m_factoryName);
	DDX_Text(pDX, IDC_EDIT11, m_medPYcode);
	DDX_Text(pDX, IDC_EDIT14, m_medCUSTcode);
	DDX_Text(pDX, IDC_EDIT17, m_pos);
	DDX_Text(pDX, IDC_EDIT15, m_max_once);
	DDX_Text(pDX, IDC_EDIT7, m_maxCapacity);
	DDX_Text(pDX, IDC_EDIT5, m_validate_time);
	DDX_Text(pDX, IDC_EDIT16, m_length);
	DDX_Text(pDX, IDC_EDIT18, m_width);
	DDX_Text(pDX, IDC_EDIT19, m_high);
	DDX_Text(pDX, IDC_EDIT21, m_medCount);
	DDX_Text(pDX, IDC_EDIT20, m_medbatch);
	DDX_Text(pDX, IDC_EDIT22, m_Count);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMedicineInfo, CRCDialog)
	//{{AFX_MSG_MAP(CMedicineInfo)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_CLEARLOCATION, OnClearlocation)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_EN_SETFOCUS(IDC_EDIT7, OnSetfocusEdit7)
	ON_EN_SETFOCUS(IDC_EDIT11, OnSetfocusEdit11)
	ON_EN_SETFOCUS(IDC_EDIT14, OnSetfocusEdit14)
	ON_EN_SETFOCUS(IDC_EDIT15, OnSetfocusEdit15)
	ON_EN_SETFOCUS(IDC_EDIT16, OnSetfocusEdit16)
	ON_EN_SETFOCUS(IDC_EDIT18, OnSetfocusEdit18)
	ON_EN_SETFOCUS(IDC_EDIT19, OnSetfocusEdit19)
	ON_EN_SETFOCUS(IDC_EDIT22, OnSetfocusEdit22)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnSliderMoving)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMedicineInfo message handlers

BOOL CMedicineInfo::OnInitDialog() 
{
	CRCDialog::OnInitDialog();
	med=NULL;
	if(med==NULL)
	    med=new CMedicineManage();
	m_sliderWarning.SetBitmapChannel( IDB_SL_CHANNEL, IDB_SL_CHANNEL_ACTIVE );
	m_sliderWarning.SetBitmapThumb( IDB_SL_THUMB, IDB_SL_THUMB_ACTIVE, TRUE );
	m_sliderWarning.DrawFocusRect( FALSE );
	
	m_sliderWarning.SetRange( 0, 100 );
	m_sliderWarning.SetPos( 0 );
	m_sliderWarning.SetMargin( 2, 2, 2, 0 );
	m_nPos=0;
	showfalg=true;
    InitLocationList();

	//返回
	CString strNormalPath	= Util::GetDialogResPath() + "\\back_normal.bmp";
	CString strDownPath		= Util::GetDialogResPath() + "\\back_down.bmp";
	HBITMAP hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	HBITMAP hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    m_btnCancel.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);

	hBitmap_Normal = NULL;
	hBitmap_Down = NULL;

	//修改
	strNormalPath	= Util::GetDialogResPath() + "\\modify_normal.bmp";
	strDownPath		= Util::GetDialogResPath() + "\\modify_down.bmp";
	hBitmap_Normal	= (HBITMAP)LoadImage(NULL, strNormalPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	hBitmap_Down	= (HBITMAP)LoadImage(NULL, strDownPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	m_btnModify.SetBMP(hBitmap_Normal, hBitmap_Normal, hBitmap_Down, hBitmap_Normal);
	if(type==1)
	{   
        /*CRect rect;
	    GetClientRect(&rect);
	    CRect rcBtnCancel;
	    m_btnCancel.GetWindowRect(&rcBtnCancel);
	    ScreenToClient(&rcBtnCancel);
	    int xPos = (rect.Width() - rcBtnCancel.Width())/2;
	    int yPos = rcBtnCancel.top;
	    m_btnCancel.MoveWindow(xPos, yPos, rcBtnCancel.Width(), rcBtnCancel.Height());

	    CRect rect1;
    	GetClientRect(&rect1);
	    CRect rcBtnCancel1;
	    m_editLocation.GetWindowRect(&rcBtnCancel1);
    	ScreenToClient(&rcBtnCancel1);
    	int xPos1 = (rect1.Width() - rcBtnCancel1.Width())-100;
    	int yPos1 = rcBtnCancel1.top;
    	m_editLocation.MoveWindow(xPos1, yPos1, rcBtnCancel1.Width(), rcBtnCancel1.Height());*/
        ((CEdit *)GetDlgItem(IDC_EDIT7))->SetReadOnly(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT11))->SetReadOnly(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT14))->SetReadOnly(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT15))->SetReadOnly(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT16))->SetReadOnly(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT18))->SetReadOnly(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT19))->SetReadOnly(TRUE);
		((CEdit *)GetDlgItem(IDC_EDIT17))->SetReadOnly(TRUE);

		((CStatic *)GetDlgItem(IDC_STATICMEDCOUNT))->ShowWindow(SW_HIDE);
		((CEdit *)GetDlgItem(IDC_EDIT22))->ShowWindow(SW_HIDE);
		((CButton *)GetDlgItem(IDC_BUTTON7))->ShowWindow(SW_HIDE);
		
		m_sliderWarning.EnableWindow(FALSE);
	}
	else if(type == 2)
	{
		CRect rcClient;
		GetWindowRect(&rcClient);
		rcClient.top -=60 ;
		rcClient.bottom -= 60;
		MoveWindow(CRect(rcClient));
	}
	return TRUE; 
}

LRESULT CMedicineInfo::OnSliderMoving(WPARAM wParam, LPARAM lParam)
{
	if( IDC_Slider_Warning == wParam ) 
	{
		UpdateData(TRUE);
		m_nPos = m_sliderWarning.GetPos();
        m_pos.Format("%d%%",m_nPos);
		UpdateData( FALSE );
		//((CEdit *)GetDlgItem(IDC_EDIT17))->ShowWindow(SW_HIDE);
		//((CEdit *)GetDlgItem(IDC_EDIT17))->ShowWindow(SW_SHOW);
	}
	return 0;
}

bool isNum(string str)
{
    for(int i=0;i<str.length();i++)
	{
        if(!isdigit(str.c_str()[i]))
		{
			return false;
		}
	}
	return true;
}

void CMedicineInfo::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	CString str;
	m_btnModify.GetWindowText(str);
	MedInfo Medinfo;
	if(m_medID.IsEmpty())
	    return;
	int medID=atoi(m_medID.GetBuffer(0));
	m_medID.ReleaseBuffer();
    Medinfo.medicineHisId=m_medHISID;
	Medinfo.medicineId=medID;
	if(m_maxCapacity.IsEmpty())
		Medinfo.maxCapacity=10;
	else
	{
		Medinfo.maxCapacity=atoi(m_maxCapacity.GetBuffer(0));
		m_maxCapacity.ReleaseBuffer();
	}
	if(m_max_once.IsEmpty())
        Medinfo.maxNumOnce=10;
	else
	{
	    Medinfo.maxNumOnce=atoi(m_max_once.GetBuffer(0));
		m_max_once.ReleaseBuffer();
	}

	Medinfo.medFactory=m_factoryName;
	Medinfo.medicineNameCH=m_medCNname;
	Medinfo.medicineNameEN=m_medENname;

	if(((CButton *)GetDlgItem(IDC_RADIO6))->GetCheck())
		Medinfo.medicineState=0;
	else 
	    Medinfo.medicineState=1;
	Medinfo.medicineUnit=m_medUnit;
    Medinfo.medlength=m_length;
	Medinfo.medwidth=m_width;
	Medinfo.medhigth=m_high;
	Medinfo.pyCode=m_medPYcode;
	Medinfo.custCode=m_medCUSTcode;
	Medinfo.typeUnit=m_typeUnit;
	if(m_nPos==0)
        Medinfo.warningPercent=30;
	else
	    Medinfo.warningPercent=m_nPos;
	Medinfo.valiDateTime =m_validate_time;
	MedicinLocationState location;
	location.medicine_id=medID;
	location.max_capacity=Medinfo.maxCapacity;
	location.warning_percent=Medinfo.warningPercent;
	for(int i=0;i<m_locationList.GetItemCount();i++)
	{
		char buf[100],buf1[100],buf2[100];
		m_locationList.GetItemText(i,4,buf,100);
		location.location_id=atoi(buf);
		m_locationList.GetItemText(i,1,buf1,100);
		location.cur_quantity=atoi(buf1);
		memset(buf2,0,100);
		m_locationList.GetItemText(i,3,buf2,100);
		if(strcmp(buf2,"启用")==0)
		    location.location_state=1;
		else
            location.location_state=0;
		Medinfo.locationlist.push_back(location);
	}
	if(Medinfo.locationlist.size()>0)
		Medinfo.medicineType = configUtil.getInt("MachineType");
	else
		Medinfo.medicineType = 0;
	if(type==2)
	{
        if(0==med->modifyMedicine(medID,Medinfo))
		{
			string strText="修改药品成功！";
			Utils::insertSystemlog("2101","修改药品信息",strText);
			strText="\r\n\r\n"+strText;
	        Util::MessageBox(this->GetSafeHwnd(), strText.c_str(),"提示",RCMB_OK);
		}
		else
		{
            string strText="修改药品失败！";
			Utils::insertSystemlog("2101","修改药品信息",strText);
			strText="\r\n\r\n"+strText;
	        Util::MessageBox(this->GetSafeHwnd(), strText.c_str(),"提示",RCMB_OK);
			return;
		}
		CString newfilename;
		newfilename = strPath + m_medHISID+".bmp";
		if( m_filePath != "" && m_filePath != newfilename)
		{
			CopyFile(m_filePath,strPath,newfilename);
		}
	}	
	showfalg=false;
	CRCDialog::OnOK();
}

void CMedicineInfo::setValue(MedInfo &medicineinfo)
{
	((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);

	((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
	
	m_medID=Utils::itos(medicineinfo.medicineId).c_str();
	m_medCNname=medicineinfo.medicineNameCH.c_str();
	m_medENname=medicineinfo.medicineNameEN.c_str();
	if(medicineinfo.medicineHisId!="")
	{
		m_medHISID=medicineinfo.medicineHisId.c_str();
		CFileFind find;
		CString tempfile=strPath + m_medHISID+".bmp";
		if(find.FindFile(tempfile))
		    m_filePath = strPath + m_medHISID+".bmp";
	}

	m_medUnit=medicineinfo.medicineUnit.c_str();
	if(medicineinfo.maxCapacity!=0)
	    m_maxCapacity=Utils::itos(medicineinfo.maxCapacity).c_str();
	else
        m_maxCapacity="10";
	if(medicineinfo.maxNumOnce!=-1)
        m_max_once=Utils::itos(medicineinfo.maxNumOnce).c_str();
	else
        m_max_once="";
	m_typeUnit=medicineinfo.typeUnit.c_str();
	m_factoryName=medicineinfo.medFactory.c_str();
	m_medPYcode=medicineinfo.pyCode.c_str();
	m_medCUSTcode=medicineinfo.custCode.c_str();
	m_length=medicineinfo.medlength.c_str();
	m_width=medicineinfo.medwidth.c_str();
	m_high=medicineinfo.medhigth.c_str();
	m_medCount=Utils::itos(medicineinfo.curQuantity).c_str();
	m_nPos=medicineinfo.warningPercent;
	m_pos.Format("%d%%",m_nPos);
	m_validate_time=(medicineinfo.valiDateTime).c_str();
	
	if(medicineinfo.medicineState==1)
	{
		((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);
		m_editLocation.EnableWindow(TRUE) ;
	}
	else if(medicineinfo.medicineState==0)
	{
		((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(TRUE);
		m_editLocation.EnableWindow(FALSE) ;
	}
	m_sliderWarning.SetPos(m_nPos);
	int i=-1;
	locationlist=medicineinfo.locationlist;
	list<MedicinLocationState>::iterator iter;
	m_locationList.DeleteAllItems();
	for(iter=medicineinfo.locationlist.begin();iter!=medicineinfo.locationlist.end(); iter++)
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
		m_locationList.InsertItem(i, _T(str));
		m_locationList.SetItemText(i, 1, _T(Utils::itos(iter->cur_quantity).c_str()));
		m_locationList.SetItemText(i, 2, _T(Utils::itos(iter->max_capacity).c_str()));
		if(iter->location_state==1)
			m_locationList.SetItemText(i, 3, _T("启用"));
		else 
			m_locationList.SetItemText(i, 3, _T("停用"));
		m_locationList.SetItemText(i, 4,_T(Utils::itos(iter->location_id).c_str()));
	}
	UpdateData( FALSE );
}

void CMedicineInfo::InitLocationList()
{
	m_locationList.Init();
	m_locationList.SetTextColor(RGB(125,125,125));
	m_locationList.InsertColumn(0, "位置", LVCFMT_LEFT, 70);
	m_locationList.InsertColumn(1, "药品数量", LVCFMT_LEFT, 70);
	m_locationList.InsertColumn(2, "最大数量", LVCFMT_LEFT, 70);
	m_locationList.InsertColumn(3, "位置状态", LVCFMT_LEFT, 70);
	m_locationList.InsertColumn(4, "位置编号", LVCFMT_LEFT, 0);
	m_locationList.SetRedraw(FALSE);
	m_locationList.SetRedraw(TRUE);
	DWORD dwStyle = m_locationList.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    DWORD d=m_locationList.SetExtendedStyle(dwStyle); 
}

void CMedicineInfo::OnCancel() 
{
	// TODO: Add extra cleanup here
	showfalg=false;
	((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
	CRCDialog::OnCancel();
}


void CMedicineInfo::OnButton3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	CString str;
	int medID=atoi(m_medID.GetBuffer(0));
	m_medID.ReleaseBuffer();
	CMedLocationEditor editor;
	if(type==1)
	{
		editor.medid=medID;
		editor.type=1;
		editor.medlocation =locationlist;
		editor.DoModal();
	}
	else if(type==2)
	{
		editor.medid=medID;
		editor.type=2;
		editor.medlocation =locationlist;
		if(editor.DoModal()==IDOK)
		{
            list<MedicinLocationState>::iterator iter;
			string str1;
			int j=0;
	        m_locationList.DeleteAllItems();
			locationlist.clear();
	        for(iter=editor.medlocation.begin();iter!=editor.medlocation.end(); iter++)
			{
				str1+=Utils::itos(iter->location_id);
				str1+=" ";
				int row=0;
				int line=0;
				int unit=0;
				int locationid=iter->location_id;
				unit=(locationid-1)/320+1;
				locationid-=(unit-1)*320;
				row=(locationid-1)%32+1;
				line=(locationid-1)/32+1;
				str.Format("%d-%d-%d",unit,line,row);
		        m_locationList.InsertItem(j, _T(str));
	        	m_locationList.SetItemText(j, 1, _T(Utils::itos(iter->cur_quantity).c_str()));
				m_locationList.SetItemText(j, 2, _T(m_maxCapacity));
				if(iter->location_state == 1)
				{
					m_locationList.SetItemText(j, 3, _T("启用"));
				}
				else
				{
					m_locationList.SetItemText(j, 3, _T("停用"));
				}
				m_locationList.SetItemText(j, 4, _T(Utils::itos(iter->location_id).c_str()));
				j++;
			}
			Utils::insertSystemlog("2104","修改药品地址","修改后地址值"+str1);
		}
	}	
}

void CMedicineInfo::OnClearlocation() 
{
	// TODO: Add your control notification handler code here
	bool falg=false;
	CString strText="确定要盘点该药品！";
	strText="\r\n\r\n"+strText;
	if(Util::MessageBox(this->GetSafeHwnd(), strText)==IDOK)
	{
		if(isRunning)
		{
			Util::MessageBox(this->GetSafeHwnd(), "\r\n\r\n设备正在发药不能盘点");
			return;
		}
		falg=true;
		UpdateData(TRUE);	
		int medID=atoi(m_medID.GetBuffer(0));
		
		m_medID.ReleaseBuffer(); 
		bizser=singletonBIZ.getInst();
		string str;
		for(int i=0;i<m_locationList.GetItemCount();i++)
		{
			char buf[100];
			m_locationList.GetItemText(i,0,buf,100);
			str+=" ";
			str+=buf;
			m_locationList.SetItemText(i,1,"0");	
		}
		int sum_count=atoi(m_medCount);
		int res=bizser->clearMedicine(medID,1,sum_count);	
		med->clearAllLocation(medID);
    	singletonBIZ.releaseInst((CBIZBoxService*)bizser);
		Utils::insertSystemlog("2106","盘点药品地址","地址值"+str);
	}
	if(falg)
	{
		Util::MessageBox(this->GetSafeHwnd(), "\r\n\r\n盘点完成,请等待篮子从最后窗口出来后再点击确定按钮","提示",RCMB_OK);
	}	
}

void CMedicineInfo::clearData()
{
    m_medCNname = _T("");
	m_medHISID = _T("");
	m_medENname = _T("");
	m_medUnit = _T("");
	m_typeUnit = _T("");
	m_medID = _T("");
	m_factoryName = _T("");
	m_medPYcode = _T("");
	m_medCUSTcode = _T("");
	m_pos = _T("");
	m_medCount = _T("0");
	m_max_once = _T("");
	m_maxCapacity = _T("");
	m_validate_time=_T("");
	m_filePath=_T("");
}

void CMedicineInfo::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CRCDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	((CEdit *)GetDlgItem(IDC_EDIT2))->SetFocus();
}

void CMedicineInfo::OnButton6() 
{
	// TODO: Add your control notification handler code here
	CFileDialog file(true,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"Excel Files(*.bmp)|*.bmp|",AfxGetMainWnd());
	if(file.DoModal()==IDOK)
	{
		//得到文件的完整路径
		m_filePath=file.GetPathName();
	}
	CStatic* static2 = (CStatic*)GetDlgItem(IDC_STATIC12);	
	static2->ShowWindow(SW_HIDE);
	static2->ShowWindow(SW_SHOW);
}



void CMedicineInfo::OnPaint() 
{
	CRCDialog::OnPaint();  
	//CPaintDC dc(this);// device context for painting
	if( m_filePath == "" )
			return;
	//CDC dc;
	//dc.m_hDC=::GetDC(NULL);
	CPaintDC dc(GetDlgItem(IDC_STATIC12));

	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
        m_filePath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
    if( hbmp == NULL )
         return ;
	CBitmap *pBmpBG = CBitmap::FromHandle(hbmp);		
	CBitmap * pOldBitmap = NULL;
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	pOldBitmap = memdc.SelectObject(pBmpBG);
	CRect brect;
	this->GetDlgItem(IDC_STATIC12)->GetClientRect(&brect);
    
	BITMAP bmp;
	pBmpBG->GetBitmap(&bmp);
	SetStretchBltMode(dc,COLORONCOLOR);
	dc.StretchBlt(brect.TopLeft().x,brect.TopLeft().y, brect.Width(), brect.Height(), &memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);   
	memdc.SelectObject(pOldBitmap);
	DeleteDC(memdc);
	DeleteObject(hbmp);
}


bool CMedicineInfo::CopyFile(CString pFrom, CString pTo, CString newfilename)
{
	TCHAR   buffrom[_MAX_PATH + 1]; 
	_tcscpy(buffrom,   pFrom); 
	buffrom[_tcslen(buffrom)+1]=0; 
    TCHAR   bufto[_MAX_PATH + 1]; 
	_tcscpy(bufto,   pTo); 
	bufto[_tcslen(bufto)+1]=0; 
	SHFILEOPSTRUCT lpsh;
	ZeroMemory(&lpsh,sizeof(lpsh));
	lpsh.hwnd= HWND_DESKTOP;
	lpsh.fFlags=FOF_NOCONFIRMATION|FOF_SIMPLEPROGRESS ;
	lpsh.wFunc=FO_COPY;
	lpsh.pFrom= buffrom;
	lpsh.pTo = bufto;
	if( 0 != SHFileOperation(&lpsh))
	{
		return false;
	}
	CFileFind find;
    if(find.FindFile(newfilename))
		CFile::Remove(newfilename);
	CString strPath = pFrom; 
	int index = strPath.ReverseFind('\\');   
	strPath = strPath.Right(strPath.GetLength()-index-1); 
	pTo+=strPath;
	CFile::Rename(pTo,newfilename);
	return true;
}


void CMedicineInfo::OnDestroy() 
{
	CRCDialog::OnDestroy();
	if(med)
	{
		delete med;
		med=NULL;
	}
	osk.close();
	// TODO: Add your message handler code here	
}

void CMedicineInfo::OnButton7() 
{
	// TODO: Add your control notification handler code here
	CString strText="确定要人工发该药品！";
	strText="\r\n\r\n"+strText;
	if(Util::MessageBox(this->GetSafeHwnd(), strText)==IDOK)
	{
		if(isRunning)
		{
			Util::MessageBox(this->GetSafeHwnd(), "\r\n\r\n设备正在发药不能人工发该药品");
			return;
		}
		UpdateData(TRUE);	
		int medID=atoi(m_medID.GetBuffer(0));
		m_medID.ReleaseBuffer(); 
		bizser=singletonBIZ.getInst();
		int count=atoi(m_Count.GetBuffer(0));
		m_Count.ReleaseBuffer(); 
		string str=m_Count;
		int res=bizser->clearMedicine(medID,2,count);	
    	singletonBIZ.releaseInst((CBIZBoxService*)bizser);
		Utils::insertSystemlog("2107","人工发该药品","数量为"+str);
	}
}

void CMedicineInfo::OnSetfocusEdit7() 
{
	// TODO: Add your control notification handler code here
	if(type == 2)
	{
		osk.setCWnd(this);
		osk.setIdd(IDC_EDIT7);
		osk.setNextIdd(IDCANCEL);
		osk.open();
	}
}

void CMedicineInfo::OnSetfocusEdit11() 
{
	// TODO: Add your control notification handler code here
	if(type == 2)
	{
		osk.setCWnd(this);
		osk.setIdd(IDC_EDIT11);
		osk.setNextIdd(IDCANCEL);
		osk.open();
	}
}

void CMedicineInfo::OnSetfocusEdit14() 
{
	// TODO: Add your control notification handler code here
	if(type == 2)
	{
		osk.setCWnd(this);
		osk.setIdd(IDC_EDIT14);
		osk.setNextIdd(IDCANCEL);
		osk.open();
	}
}

void CMedicineInfo::OnSetfocusEdit15() 
{
	// TODO: Add your control notification handler code here
	if(type == 2)
	{
		osk.setCWnd(this);
		osk.setIdd(IDC_EDIT15);
		osk.setNextIdd(IDCANCEL);
		osk.open();
	}
}

void CMedicineInfo::OnSetfocusEdit16() 
{
	// TODO: Add your control notification handler code here
	if(type == 2)
	{
		osk.setCWnd(this);
		osk.setIdd(IDC_EDIT16);
		osk.setNextIdd(IDCANCEL);
		osk.open();
	}
}

void CMedicineInfo::OnSetfocusEdit18() 
{
	// TODO: Add your control notification handler code here
	if(type == 2)
	{
		osk.setCWnd(this);
		osk.setIdd(IDC_EDIT18);
		osk.setNextIdd(IDCANCEL);
		osk.open();
	}
}

void CMedicineInfo::OnSetfocusEdit19() 
{
	// TODO: Add your control notification handler code here
	if(type == 2)
	{
		osk.setCWnd(this);
		osk.setIdd(IDC_EDIT19);
		osk.setNextIdd(IDCANCEL);
		osk.open();
	}
}

void CMedicineInfo::OnSetfocusEdit22() 
{
	// TODO: Add your control notification handler code here
	if(type == 2)
	{
		osk.setCWnd(this);
		osk.setIdd(IDC_EDIT22);
		osk.setNextIdd(IDCANCEL);
		osk.open();
	}
}
