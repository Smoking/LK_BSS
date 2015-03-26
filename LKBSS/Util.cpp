
#include "StdAfx.h"
#include "Util.h"
#include "LKBSS.h"
#include "RCMessageBox.h"


SHELLEXECUTEINFO CShellExecuteOsk::execInfo;

CString Util::GetDialogResPath()
{
	char path[MAX_PATH];
	memset(path, 0, MAX_PATH);
	GetModuleFileName(NULL, path, MAX_PATH);
	
	CString strPath = path; 
	int index = strPath.ReverseFind('\\');   
	strPath = strPath.Left(index); 
	CString strRelativePath;
	strRelativePath.LoadString(IDS_DialogRes);
	strPath = strPath + "\\" + strRelativePath;
	
	return strPath;
}

void Util::Draw9Grid(CDC *pDC, CRect rcRgn, HBITMAP hBitmap)
{
	if (!pDC || hBitmap == NULL)
		return;
	
	//采用经典的9宫格算法
	const BlockWidth = 3;  
	CBitmap *bitmap = NULL, *pOldBitmap = NULL;
	CDC		MemDC;
	BITMAP	bmp;
	bitmap = CBitmap::FromHandle(hBitmap);
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(bitmap);
	bitmap->GetBitmap(&bmp);
	if ((rcRgn.Width() >= 3*BlockWidth) && (rcRgn.Height() >= 3*BlockWidth))
	{
		pDC->BitBlt(rcRgn.left, rcRgn.top, BlockWidth, BlockWidth, &MemDC, 0, 0, SRCCOPY);
		pDC->BitBlt(rcRgn.left, rcRgn.bottom - BlockWidth, BlockWidth, BlockWidth, &MemDC, 0, bmp.bmHeight - BlockWidth, SRCCOPY);
		pDC->BitBlt(rcRgn.right - BlockWidth, rcRgn.top, BlockWidth, BlockWidth, &MemDC, bmp.bmWidth - BlockWidth, 0, SRCCOPY);
		pDC->BitBlt(rcRgn.right - BlockWidth, rcRgn.bottom - BlockWidth, BlockWidth, BlockWidth, &MemDC, 
			bmp.bmWidth - BlockWidth, bmp.bmHeight - BlockWidth, SRCCOPY);
		pDC->StretchBlt(rcRgn.left, rcRgn.top + BlockWidth, BlockWidth, rcRgn.Height() - 2*BlockWidth, &MemDC, 0, BlockWidth,
			BlockWidth, bmp.bmHeight - 2*BlockWidth, SRCCOPY);
		pDC->StretchBlt(rcRgn.left + BlockWidth, rcRgn.bottom - BlockWidth, rcRgn.Width() - 2*BlockWidth, BlockWidth, &MemDC,
			BlockWidth, bmp.bmHeight - BlockWidth, bmp.bmWidth - 2*BlockWidth, BlockWidth, SRCCOPY);
		pDC->StretchBlt(rcRgn.right - BlockWidth, rcRgn.top + BlockWidth, BlockWidth, rcRgn.Height() - 2*BlockWidth, &MemDC,
			bmp.bmWidth - BlockWidth, BlockWidth, BlockWidth, bmp.bmHeight - 2*BlockWidth, SRCCOPY);
		pDC->StretchBlt(rcRgn.left + BlockWidth, rcRgn.top, rcRgn.Width() - 2*BlockWidth, BlockWidth, &MemDC, BlockWidth, 0,
			bmp.bmWidth - 2*BlockWidth, BlockWidth, SRCCOPY);
		pDC->StretchBlt(rcRgn.left + BlockWidth, rcRgn.top + BlockWidth, rcRgn.Width() - 2*BlockWidth, rcRgn.Height() - 2*BlockWidth, &MemDC,
			BlockWidth, BlockWidth, bmp.bmWidth - 2*BlockWidth, bmp.bmHeight - 2*BlockWidth, SRCCOPY);
	}
	else if ((rcRgn.Width() >= 3*BlockWidth) && (rcRgn.Height() < 3*BlockWidth))
	{
		pDC->BitBlt(rcRgn.left, rcRgn.top, BlockWidth,  rcRgn.Height(), &MemDC, 0, 0, SRCCOPY);
		pDC->BitBlt(rcRgn.right - BlockWidth, rcRgn.top, BlockWidth, rcRgn.Height(), &MemDC, 
			bmp.bmWidth - BlockWidth, 0, SRCCOPY);
		pDC->StretchBlt(rcRgn.left + BlockWidth, rcRgn.top, rcRgn.Width() - 2*BlockWidth, rcRgn.Height(), &MemDC,
			2*BlockWidth, 0, bmp.bmWidth - 4*BlockWidth, bmp.bmHeight, SRCCOPY);
	}
	else if ((rcRgn.Width() < 3*BlockWidth) && (rcRgn.Height() >= 3*BlockWidth))
	{
		pDC->StretchBlt(rcRgn.left, rcRgn.top, rcRgn.Width(), rcRgn.Height() , &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}
	else
	{
		pDC->BitBlt(rcRgn.left, rcRgn.top, rcRgn.Width(), rcRgn.Height(), &MemDC, 0, 0, SRCCOPY);
	}
	MemDC.SelectObject(pOldBitmap);
	bitmap->Detach();
}

void Util::xTileRgnWithBmp(CDC *pDC, CRect rcRgn, HBITMAP hBitmap)
{
	if (!pDC || hBitmap == NULL)
		return;
	
	BITMAP	bmp;
	CBitmap *bitmap = NULL, *pOldBitmap = NULL;
	bitmap = CBitmap::FromHandle(hBitmap);
	bitmap->GetBitmap(&bmp);
	
	CDC		MemDC;
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(bitmap);
	
	int x = rcRgn.left;
	while(x < rcRgn.right)
	{
		pDC->StretchBlt(x, rcRgn.top, bmp.bmWidth, rcRgn.Height(), &MemDC,
			0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		
		x += bmp.bmWidth;
	}
	MemDC.SelectObject(pOldBitmap);
	bitmap->Detach();
}

void  Util::DrawTransparentBitmap(HDC hdc,HBITMAP hBitmap,short xStart,short yStart,COLORREF cTransparentColor) 
{   
	BITMAP   bm;   
	COLORREF cColor;   
	HBITMAP  bmAndBack, bmAndObject, bmAndMem, bmSave;   
	HBITMAP  bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;   
	HDC      hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;   
	POINT    ptSize;   
    
	hdcTemp = CreateCompatibleDC(hdc);   
	SelectObject(hdcTemp, hBitmap); //Select the bitmap   
    
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);   
	ptSize.x = bm.bmWidth;				//Get width of bitmap   
	ptSize.y = bm.bmHeight;				//Get height of bitmap   
	DPtoLP(hdcTemp,   &ptSize,   1);	//Convert from device   
    
	//to logical points   
    
	//Create some DCs to hold temporary data.   
	hdcBack =  CreateCompatibleDC(hdc);   
	hdcObject = CreateCompatibleDC(hdc);   
	hdcMem = CreateCompatibleDC(hdc);   
	hdcSave = CreateCompatibleDC(hdc);    
    
	//Monochrome DC   
	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);   
    
	//Monochrome DC   
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);   
    
	bmAndMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);   
	bmSave = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);   
    
	//Each DC must select a bitmap object to store pixel data.   
	bmBackOld =  (HBITMAP)SelectObject(hdcBack, bmAndBack);   
	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);   
	bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);   
	bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);   
    
	//Set proper mapping mode.   
	SetMapMode(hdcTemp, GetMapMode(hdc));   
    
	//Save the bitmap sent here,because it will be overwritten.   
	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);   
    
	//Set the background color of the source DC to the color.   
	//contained in the parts of the bitmap that should be transparent   
	cColor = SetBkColor(hdcTemp,   cTransparentColor);   
    
	//Create the object mask for the bitmap by performing a BitBlt from the source bitmap to a monochrome bitmap.   
	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);   
    
	//Set the background color of the source DC back to the original color.   
	SetBkColor(hdcTemp, cColor);   
    
	//Create the inverse of the object mask.   
	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);   
    
	//Copy the background of the main DC to the destination.   
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart, SRCCOPY);   
    
	//Mask out the places where the bitmap will be placed.   
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);   
    
	//Mask out the transparent colored pixels on the bitmap.   
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);   
    
	//XOR the bitmap with the background on the destination DC.   
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);   
    
	//Copy the destination to the screen.   
	BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);   
    
	//Place the original bitmap back into the bitmap sent here.   
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);   
    
	//Delete   the   memory   bitmaps.   
	DeleteObject(SelectObject(hdcBack,   bmBackOld));   
	DeleteObject(SelectObject(hdcObject,   bmObjectOld));   
	DeleteObject(SelectObject(hdcMem,   bmMemOld));   
	DeleteObject(SelectObject(hdcSave,   bmSaveOld));   
    
	//Delete the memory DCs.   
	DeleteDC(hdcMem);   
	DeleteDC(hdcBack);   
	DeleteDC(hdcObject);   
	DeleteDC(hdcSave);   
	DeleteDC(hdcTemp);   
}     

void Util::FillRectWithBmp(CDC *pDC, CString strBmpPath, CRect rect, BOOL bStretch/* = TRUE*/)
{
	if (!pDC)
		return;
	
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, strBmpPath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
	
	CBitmap *pBitmap = NULL, *pOldBitmap = NULL;
	pBitmap = CBitmap::FromHandle(hBitmap);
	
	BITMAP bmp;
	pBitmap->GetBitmap(&bmp);
	
	CDC		MemDC;
	MemDC.CreateCompatibleDC(pDC);
	pOldBitmap = MemDC.SelectObject(pBitmap);
	
	if (bStretch)
	{
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}
	else
	{
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	}
	
	DeleteObject(hBitmap);
	MemDC.SelectObject(pOldBitmap);
	pBitmap->Detach();
}

int Util::MessageBox(HWND hWnd , CString strText, CString strCaption/* = _T("")*/, UINT uType/* = RCMB_OKCANCEL*/)
{
    CRCMessageBox messageBox;
	messageBox.InitParam(hWnd, strText, strCaption, uType);	
	return messageBox.DoModal();
}

int Util::ShowMessageBox(HWND hWnd , CString strText, CString strCaption/* = _T("")*/, UINT uType/* = RCMB_OKCANCEL*/)
{
	CRCMessageBox *messageBox;
	messageBox=new CRCMessageBox(NULL);
	messageBox->InitParam(hWnd, strText, strCaption, uType);
	messageBox->Create(IDD_MessageBox,NULL);	
	messageBox->ShowWindow(SW_SHOW) ;
	//messageBox->SetWindowPos(NULL,500,200,500,245,SWP_SHOWWINDOW);
	return 1 ;
}

CShellExecuteOsk::CShellExecuteOsk()
{
    memset(&execInfo,0,sizeof(execInfo));  
    execInfo.cbSize=sizeof(execInfo);  
    execInfo.hwnd=NULL;  
    execInfo.lpFile="osk.exe";  
    execInfo.nShow=SW_SHOWNORMAL;  
    execInfo.fMask=SEE_MASK_NOCLOSEPROCESS;  
    openType = configUtil.getInt("Openosk");
	m_openFlag = false ;
}

CShellExecuteOsk::~CShellExecuteOsk()
{

}

//打开软键盘
void CShellExecuteOsk::open()
{
    if (openType ==1) 
    {
		if(FindProcess("osk.exe")==0)
			ShellExecuteEx(&execInfo);
    }
	else if (openType ==2 )
	{
		if(!m_openFlag )
		{
			ckb.Create(IDD_Keyboard, m_pParentWnd);
			ckb.setCPWnd(m_pParentWnd);
			m_openFlag = true ;
		}
		ckb.ShowWindow(SW_SHOW);
	}
}

void CShellExecuteOsk::close()
{
	if (openType ==1)
	{
		if(FindProcess("osk.exe")!=0 )
			TerminateProcess(execInfo.hProcess,0);
	}
	else if (openType ==2)
	{
		if(m_openFlag)
			ckb.ShowWindow(SW_HIDE);		
	}
}

DWORD CShellExecuteOsk::FindProcess(char *strProcessName)
{
    DWORD aProcesses[1024], cbNeeded, cbMNeeded;
    HMODULE hMods[1024];
    HANDLE hProcess;
    char szProcessName[MAX_PATH];

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) ) 
		return 0;
    for(int i=0; i< (int) (cbNeeded / sizeof(DWORD)); i++)
    {
        hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
        EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
        GetModuleFileNameEx( hProcess, hMods[0], szProcessName,sizeof(szProcessName));
       
        if(strstr(szProcessName, strProcessName))
        {
            return(aProcesses[i]);
        }
    }
    return 0;
}

void CShellExecuteOsk::setIdd(int idd)
{
	ckb.setIdd(idd);
}

void CShellExecuteOsk::setNextIdd(int idd)
{
	ckb.setNextIdd(idd);
}

void CShellExecuteOsk::setCWnd(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
}