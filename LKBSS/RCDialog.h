#if !defined(AFX_RCDIALOG_H__211051F9_5E39_4CF7_B2FF_8129F1822DF1__INCLUDED_)
#define AFX_RCDIALOG_H__211051F9_5E39_4CF7_B2FF_8129F1822DF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRCDialog dialog

#include "MenuBar.h"
#define  CRECTMONITOR  CRect(117,281,455,430)
#define  CRECTMEDMANG  CRect(560,280,900,430)
#define  CRECTDISMED   CRect(117,520,455,668)
#define  CRECTADDMED   CRect(560,520,900,668)

class CRCDialog : public CDialog
{
	DECLARE_DYNAMIC(CRCDialog)
public:
	CRCDialog(UINT nIDTemplate, CWnd * pParentWnd = NULL,int flag = 0);
	virtual ~CRCDialog();

public:
	void SetWindowText(CString strText);	//���ô��ڱ�����
	void GetClientRect(CRect* pRect);		//���ؿͻ��������
	void SetMenu(CMenu* pMenu);				//���ô��ڲ˵�
	void ShowBottomLogo(BOOL bShow);		//�Ƿ���ʾ�ײ�logo
	void SetBgPath(CString strPath);		//���ñ���ͼƬ·��

protected:
	void DrawFrame(CDC *pDC);  

	void DrawCaption(CDC *pDC);		//���Ʊ�����
	void DrawIconAndText(CDC *pDC); //���Ʊ�����ͼ�������

	void DrawMenu(CDC *pDC); //���Ʋ˵�����
	void DrawMenuBG(CDC *pDC); //���Ʋ˵����򱳾�

	void DrawClientBG(CDC *pDC); //���ƿͻ�������

	void DrawBottom(CDC *pDC);      //���Ʊ߿�ײ�
	void DrawBottomLogo(CDC *pDC, CRect rect); //����LOGOͼ��

	void DrawSysButton(CDC *pDC);	//����ϵͳ��ť
	void DrawCloseBtn(CDC *pDC, int nState); //���ƹرհ�ť
	void DrawMinBtn(CDC *pDC, int nState);

	CRect GetCaptionRect();			//�õ�����������
	CRect GetBottomRect();			//�õ��ײ��������
	CRect GetMenuRect();			//�õ��˵�����

	void CreateMenu();    //�����˵�

	void showMain(CPoint point);
	void showAssist(CPoint point);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:	// msg
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nHitTest, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

protected:
	CRect			m_rcSysBtnMin;             //��С����ťλ��rect
	CRect			m_rcSysBtnClose;             //�رհ�ťλ��rect
	int				m_nCloseBtnState;
	int			    m_nMinBtnState;
	CString			m_strCaption;
	CMenu			*m_pMenu;
	CMenuBar		m_menuBar;

	BOOL			m_bShowBottomLogo;			 //�Ƿ���ʾ�ײ�logo		
	int             showflag;  
	CString			m_strBgPath;
	static HWND     hWnd;
	enum 
	{
		NORMAL_STATE = 0,	//��ͨ״̬
		HOVER_STATE,		//Mouse Over״̬
		PRESSED_STATE,		//Press down״̬
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCDIALOG_H__211051F9_5E39_4CF7_B2FF_8129F1822DF1__INCLUDED_)
