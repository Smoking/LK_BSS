// Keyboard.cpp : implementation file
//

#include "stdafx.h"
#include "LKBSS.h"
#include "Keyboard.h"
#include "Util.h"
#include <winable.h>
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyboard dialog


CKeyboard::CKeyboard(CWnd* pParent /*=NULL*/)
	: CRCDialog(CKeyboard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyboard)
	//}}AFX_DATA_INIT
	m_idd = 0;
	m_nextIdd = 0;
}

CKeyboard::~CKeyboard()
{
	
}

void CKeyboard::DoDataExchange(CDataExchange* pDX)
{
	CRCDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyboard)
	DDX_Control(pDX, IDC_BUTTON51, m_Point);
	DDX_Control(pDX, IDC_BUTTON50, m_0);
	DDX_Control(pDX, IDC_BUTTON49, m_Right);
	DDX_Control(pDX, IDC_BUTTON48, m_Down);
	DDX_Control(pDX, IDC_BUTTON47, m_Empty);
	DDX_Control(pDX, IDC_BUTTON46, m_3);
	DDX_Control(pDX, IDC_BUTTON45, m_2);
	DDX_Control(pDX, IDC_BUTTON44, m_1);
	DDX_Control(pDX, IDC_BUTTON43, m_6);
	DDX_Control(pDX, IDC_BUTTON42, m_5);
	DDX_Control(pDX, IDC_BUTTON41, m_4);
	DDX_Control(pDX, IDC_BUTTON40, m_9);
	DDX_Control(pDX, IDC_BUTTON39, m_8);
	DDX_Control(pDX, IDC_BUTTON38, m_7);
	DDX_Control(pDX, IDC_BUTTON37, m_Enter);
	DDX_Control(pDX, IDC_BUTTON36, m_Up);
	DDX_Control(pDX, IDC_BUTTON35, m_Left);
	DDX_Control(pDX, IDC_BUTTON34, m_M);
	DDX_Control(pDX, IDC_BUTTON33, m_N);
	DDX_Control(pDX, IDC_BUTTON32, m_B);
	DDX_Control(pDX, IDC_BUTTON31, m_V);
	DDX_Control(pDX, IDC_BUTTON30, m_C);
	DDX_Control(pDX, IDC_BUTTON29, m_X);
	DDX_Control(pDX, IDC_BUTTON28, m_Z);
	DDX_Control(pDX, IDC_BUTTON26, m_L);
	DDX_Control(pDX, IDC_BUTTON27, m_Backspace);
	DDX_Control(pDX, IDC_BUTTON25, m_K);
	DDX_Control(pDX, IDC_BUTTON24, m_J);
	DDX_Control(pDX, IDC_BUTTON23, m_H);
	DDX_Control(pDX, IDC_BUTTON22, m_G);
	DDX_Control(pDX, IDC_BUTTON21, m_F);
	DDX_Control(pDX, IDC_BUTTON20, m_D);
	DDX_Control(pDX, IDC_BUTTON6, m_S);
	DDX_Control(pDX, IDC_BUTTON5, m_A);
	DDX_Control(pDX, IDC_BUTTON16, m_P);
	DDX_Control(pDX, IDC_BUTTON15, m_O);
	DDX_Control(pDX, IDC_BUTTON14, m_I);
	DDX_Control(pDX, IDC_BUTTON13, m_U);
	DDX_Control(pDX, IDC_BUTTON12, m_Y);
	DDX_Control(pDX, IDC_BUTTON11, m_T);
	DDX_Control(pDX, IDC_BUTTON10, m_R);
	DDX_Control(pDX, IDC_BUTTON8, m_E);
	DDX_Control(pDX, IDC_BUTTON2, m_W);
	DDX_Control(pDX, IDC_BUTTON1, m_Q);
	DDX_Control(pDX, IDOK, m_close);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyboard, CRCDialog)
	//{{AFX_MSG_MAP(CKeyboard)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	ON_BN_CLICKED(IDC_BUTTON23, OnButton23)
	ON_BN_CLICKED(IDC_BUTTON24, OnButton24)
	ON_BN_CLICKED(IDC_BUTTON25, OnButton25)
	ON_BN_CLICKED(IDC_BUTTON26, OnButton26)
	ON_BN_CLICKED(IDC_BUTTON28, OnButton28)
	ON_BN_CLICKED(IDC_BUTTON29, OnButton29)
	ON_BN_CLICKED(IDC_BUTTON30, OnButton30)
	ON_BN_CLICKED(IDC_BUTTON31, OnButton31)
	ON_BN_CLICKED(IDC_BUTTON32, OnButton32)
	ON_BN_CLICKED(IDC_BUTTON33, OnButton33)
	ON_BN_CLICKED(IDC_BUTTON34, OnButton34)
	ON_BN_CLICKED(IDC_BUTTON37, OnButton37)
	ON_BN_CLICKED(IDC_BUTTON27, OnButton27)
	ON_BN_CLICKED(IDC_BUTTON38, OnButton38)
	ON_BN_CLICKED(IDC_BUTTON39, OnButton39)
	ON_BN_CLICKED(IDC_BUTTON40, OnButton40)
	ON_BN_CLICKED(IDC_BUTTON41, OnButton41)
	ON_BN_CLICKED(IDC_BUTTON42, OnButton42)
	ON_BN_CLICKED(IDC_BUTTON43, OnButton43)
	ON_BN_CLICKED(IDC_BUTTON44, OnButton44)
	ON_BN_CLICKED(IDC_BUTTON45, OnButton45)
	ON_BN_CLICKED(IDC_BUTTON46, OnButton46)
	ON_BN_CLICKED(IDC_BUTTON36, OnButton36)
	ON_BN_CLICKED(IDC_BUTTON48, OnButton48)
	ON_BN_CLICKED(IDC_BUTTON35, OnButton35)
	ON_BN_CLICKED(IDC_BUTTON49, OnButton49)
	ON_BN_CLICKED(IDC_BUTTON47, OnButton47)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON50, OnButton50)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON51, OnButton51)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyboard message handlers
void CKeyboard::setIdd(int idd)
{
	m_idd = idd;
}

void CKeyboard::setNextIdd(int idd)
{
	m_nextIdd = idd;
}

void CKeyboard::setCPWnd(CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

void CKeyboard::inputKey(BYTE key)
{
	if (m_idd < 1)
	{
		return;
	}
	CWnd * wnd =NULL;
	wnd = GetParent()->GetDlgItem(m_idd) ;
	if(wnd != NULL)
	{
		wnd->SetFocus();
	}
	else
	{
		wnd = m_pParentWnd->GetDlgItem(m_idd);
		if(wnd != NULL)
			wnd->SetFocus();
	}

	if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z') || key == VK_BACK || key == 110 || key == VK_SPACE)
	{
		keybd_event(VK_RIGHT,(BYTE)0, 0 ,0);	//ÏòÓÒ
		keybd_event(VK_RIGHT, (BYTE)0, KEYEVENTF_KEYUP,0);
	}

	keybd_event(key,(BYTE)0, 0 ,0);
    keybd_event(key, (BYTE)0, KEYEVENTF_KEYUP,0);
	
}

//Q
void CKeyboard::OnButton1() 
{
	// TODO: Add your control notification handler code here
	inputKey('Q');
}

//W
void CKeyboard::OnButton2() 
{
	// TODO: Add your control notification handler code here
	inputKey('W');
}

//E
void CKeyboard::OnButton8() 
{
	// TODO: Add your control notification handler code here
	inputKey('E');
}

//R
void CKeyboard::OnButton10() 
{
	// TODO: Add your control notification handler code here
	inputKey('R');
}

//T
void CKeyboard::OnButton11() 
{
	// TODO: Add your control notification handler code here
	inputKey('T');
}

//Y
void CKeyboard::OnButton12() 
{
	// TODO: Add your control notification handler code here
	inputKey('Y');
}

//U
void CKeyboard::OnButton13() 
{
	// TODO: Add your control notification handler code here
	inputKey('U');
}

//I
void CKeyboard::OnButton14() 
{
	// TODO: Add your control notification handler code here
	inputKey('I');
}

//O
void CKeyboard::OnButton15() 
{
	// TODO: Add your control notification handler code here
	inputKey('O');
}

//P
void CKeyboard::OnButton16() 
{
	// TODO: Add your control notification handler code here
	inputKey('P');
}

//A
void CKeyboard::OnButton5() 
{
	// TODO: Add your control notification handler code here
	//inputKey('A');
	inputKey('A');
}

void CKeyboard::OnButton6() 
{
	// TODO: Add your control notification handler code here
	inputKey('S');
}

void CKeyboard::OnButton20() 
{
	// TODO: Add your control notification handler code here
	inputKey('D');
}

void CKeyboard::OnButton21() 
{
	// TODO: Add your control notification handler code here
	inputKey('F');
}

void CKeyboard::OnButton22() 
{
	// TODO: Add your control notification handler code here
	inputKey('G');
}

void CKeyboard::OnButton23() 
{
	// TODO: Add your control notification handler code here
	inputKey('H');
}

void CKeyboard::OnButton24() 
{
	// TODO: Add your control notification handler code here
	inputKey('J');
}

void CKeyboard::OnButton25() 
{
	// TODO: Add your control notification handler code here
	inputKey('K');
}

void CKeyboard::OnButton26() 
{
	// TODO: Add your control notification handler code here
	inputKey('L');
}

void CKeyboard::OnButton28() 
{
	// TODO: Add your control notification handler code here
	inputKey('Z');
}

void CKeyboard::OnButton29() 
{
	// TODO: Add your control notification handler code here
	inputKey('X');
}

void CKeyboard::OnButton30() 
{
	// TODO: Add your control notification handler code here
	inputKey('C');
}

void CKeyboard::OnButton31() 
{
	// TODO: Add your control notification handler code here
	inputKey('V');
}

void CKeyboard::OnButton32() 
{
	// TODO: Add your control notification handler code here
	inputKey('B');
}

void CKeyboard::OnButton33() 
{
	// TODO: Add your control notification handler code here
	inputKey('N');
}

void CKeyboard::OnButton34() 
{
	// TODO: Add your control notification handler code here
	inputKey('M');
}

void CKeyboard::OnButton37() 
{
	// TODO: Add your control notification handler code here
	inputKey(VK_RETURN);
}

void CKeyboard::OnButton27() 
{
	// TODO: Add your control notification handler code here
	inputKey(VK_BACK);
}

void CKeyboard::OnButton38() 
{
	// TODO: Add your control notification handler code here
	inputKey('7');
}

void CKeyboard::OnButton39() 
{
	// TODO: Add your control notification handler code here
	inputKey('8');
}

void CKeyboard::OnButton40() 
{
	// TODO: Add your control notification handler code here
	inputKey('9');
}

void CKeyboard::OnButton41() 
{
	// TODO: Add your control notification handler code here
	inputKey('4');
}

void CKeyboard::OnButton42() 
{
	// TODO: Add your control notification handler code here
	inputKey('5');
}

void CKeyboard::OnButton43() 
{
	// TODO: Add your control notification handler code here
	inputKey('6');
}

void CKeyboard::OnButton44() 
{
	// TODO: Add your control notification handler code here
	inputKey('1');
}

void CKeyboard::OnButton45() 
{
	// TODO: Add your control notification handler code here
	inputKey('2');
}

void CKeyboard::OnButton46() 
{
	// TODO: Add your control notification handler code here
	inputKey('3');
}


void CKeyboard::OnButton36() 
{
	// TODO: Add your control notification handler code here
	inputKey(VK_UP);
}

void CKeyboard::OnButton48() 
{
	// TODO: Add your control notification handler code here
	inputKey(VK_DOWN);
}

void CKeyboard::OnButton35() 
{
	// TODO: Add your control notification handler code here
	inputKey(VK_LEFT);
}

void CKeyboard::OnButton49() 
{
	// TODO: Add your control notification handler code here
	inputKey(VK_RIGHT);
}

void CKeyboard::OnButton47() 
{
	// TODO: Add your control notification handler code here
	inputKey(VK_SPACE);
}

BOOL CKeyboard::OnInitDialog() 
{
	CRCDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeyboard::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CRCDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	CRect rcWindow, rcControl,rcMove;
	GetWindowRect(&rcWindow);
	CWnd * wnd =NULL;
	wnd = GetParent()->GetDlgItem(m_idd) ;
	if(wnd != NULL)
	{
		GetParent()->GetDlgItem(m_idd)->GetWindowRect(&rcControl);	
		if(rcControl.top > rcWindow.bottom)
		{
			rcMove.left = rcWindow.left;
			rcMove.right = rcMove.left+rcWindow.Width();
			rcMove.bottom  = rcControl.top-80;
			rcMove.top = rcMove.bottom - rcWindow.Height();		
		}
		else
		{
			rcMove.left =0;
			rcMove.right = rcMove.left+rcWindow.Width();
			rcMove.bottom = 768 ;	
			rcMove.top  = rcMove.bottom -  rcWindow.Height();
		}
	}
	else
	{
		rcMove.left =0;
		rcMove.right = rcMove.left+rcWindow.Width();
		rcMove.bottom = 768 ;	
		rcMove.top  = rcMove.bottom -  rcWindow.Height();		
	}
	MoveWindow(rcMove);	
}

void CKeyboard::OnButton50() 
{
	// TODO: Add your control notification handler code here
	inputKey('0');
}

void CKeyboard::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CWnd * wnd =NULL;
	wnd = GetParent()->GetDlgItem(m_nextIdd) ;
	if(wnd != NULL)
		wnd->SetFocus();	
	else
	{	
		wnd = m_pParentWnd->GetDlgItem(m_nextIdd);	
		if(wnd != NULL)
			wnd->SetFocus();
	}
	CRCDialog::OnClose();
}

void CKeyboard::OnOK() 
{
	// TODO: Add extra validation here
	PostMessage(WM_CLOSE);
	CRCDialog::OnOK();
}

void CKeyboard::OnButton51() 
{
	// TODO: Add your control notification handler code here
	inputKey(110);
}

