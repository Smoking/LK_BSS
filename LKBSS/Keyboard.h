#if !defined(AFX_KEYBOARD_H__52660F06_4BF5_4E6C_B6F0_1915AEEE7197__INCLUDED_)
#define AFX_KEYBOARD_H__52660F06_4BF5_4E6C_B6F0_1915AEEE7197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Keyboard.h : header file
//

#include "RCDialog.h"
#include "RCButton.h"

/////////////////////////////////////////////////////////////////////////////
// CKeyboard dialog

class CKeyboard : public CRCDialog
{
// Construction
public:
	CKeyboard(CWnd* pParent = NULL);   // standard constructor
	~CKeyboard();
	void setIdd(int idd);
	void setNextIdd(int idd);
	void setCPWnd(CWnd* pParentWnd);
// Dialog Data
	//{{AFX_DATA(CKeyboard)
	enum { IDD = IDD_Keyboard };
	CRCButton	m_Point;
	CRCButton	m_0;
	CRCButton	m_Right;
	CRCButton	m_Down;
	CRCButton	m_Empty;
	CRCButton	m_3;
	CRCButton	m_2;
	CRCButton	m_1;
	CRCButton	m_6;
	CRCButton	m_5;
	CRCButton	m_4;
	CRCButton	m_9;
	CRCButton	m_8;
	CRCButton	m_7;
	CRCButton	m_Enter;
	CRCButton	m_Up;
	CRCButton	m_Left;
	CRCButton	m_M;
	CRCButton	m_N;
	CRCButton	m_B;
	CRCButton	m_V;
	CRCButton	m_C;
	CRCButton	m_X;
	CRCButton	m_Z;
	CRCButton	m_L;
	CRCButton	m_Backspace;
	CRCButton	m_K;
	CRCButton	m_J;
	CRCButton	m_H;
	CRCButton	m_G;
	CRCButton	m_F;
	CRCButton	m_D;
	CRCButton	m_S;
	CRCButton	m_A;
	CRCButton	m_P;
	CRCButton	m_O;
	CRCButton	m_I;
	CRCButton	m_U;
	CRCButton	m_Y;
	CRCButton	m_T;
	CRCButton	m_R;
	CRCButton	m_E;
	CRCButton	m_W;
	CRCButton	m_Q;
	CRCButton	m_close;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyboard)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyboard)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton8();
	afx_msg void OnButton10();
	afx_msg void OnButton12();
	afx_msg void OnButton11();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton20();
	afx_msg void OnButton21();
	afx_msg void OnButton22();
	afx_msg void OnButton23();
	afx_msg void OnButton24();
	afx_msg void OnButton25();
	afx_msg void OnButton26();
	afx_msg void OnButton28();
	afx_msg void OnButton29();
	afx_msg void OnButton30();
	afx_msg void OnButton31();
	afx_msg void OnButton32();
	afx_msg void OnButton33();
	afx_msg void OnButton34();
	afx_msg void OnButton37();
	afx_msg void OnButton27();
	afx_msg void OnButton38();
	afx_msg void OnButton39();
	afx_msg void OnButton40();
	afx_msg void OnButton41();
	afx_msg void OnButton42();
	afx_msg void OnButton43();
	afx_msg void OnButton44();
	afx_msg void OnButton45();
	afx_msg void OnButton46();
	afx_msg void OnButton36();
	afx_msg void OnButton48();
	afx_msg void OnButton35();
	afx_msg void OnButton49();
	afx_msg void OnButton47();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButton50();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnButton51();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void inputKey(BYTE key);

private:
	int m_idd;	//接收输入的控件ID
	int m_nextIdd;	//关闭后聚焦的控件
	CWnd* m_pParentWnd ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARD_H__52660F06_4BF5_4E6C_B6F0_1915AEEE7197__INCLUDED_)
