#if !defined(AFX_MEDBOXCTRL_H__BA3D3E91_3BF4_4832_ACDB_E5B854F4CF9F__INCLUDED_)
#define AFX_MEDBOXCTRL_H__BA3D3E91_3BF4_4832_ACDB_E5B854F4CF9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MedBoxCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMedBoxCtrl window
enum
{
	MEDBOX_CAPSTATUS_DISABLE = 0,	//ҩ������״̬--������
	MEDBOX_CAPSTATUS_EMPTY, 		//ҩ������״̬--�ղ�
	MEDBOX_CAPSTATUS_DANGER,		//ҩ������״̬--Σ��
	MEDBOX_CAPSTATUS_NORMAL,		//ҩ������״̬--����
	MEDBOX_CAPSTATUS_FULL,			//ҩ������״̬--����
};



class CMedBoxCtrl : public CStatic
{
// Construction
public:
	CMedBoxCtrl();

// Attributes
public:

// Operations
public:
	int GetCapacityStatus();  //�õ�ҩ���״̬

	void SetMaxCapacity(int n);
	void SetCurNumber(int n);
	void SetStatusColor(COLORREF clr);
	void SetTextColor(COLORREF clr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMedBoxCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMedBoxCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMedBoxCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	CString GetMedBoxPicPath();

private:
	int m_nMaxCapicity;		//�������
	int m_nCurNumber;		//��ǰ����
	COLORREF m_clrStatus;	//״̬��ɫ
	COLORREF m_clrText;		//�ı���ɫ
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDBOXCTRL_H__BA3D3E91_3BF4_4832_ACDB_E5B854F4CF9F__INCLUDED_)
