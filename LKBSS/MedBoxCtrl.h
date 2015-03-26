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
	MEDBOX_CAPSTATUS_DISABLE = 0,	//药箱容量状态--不可用
	MEDBOX_CAPSTATUS_EMPTY, 		//药箱容量状态--空仓
	MEDBOX_CAPSTATUS_DANGER,		//药箱容量状态--危险
	MEDBOX_CAPSTATUS_NORMAL,		//药箱容量状态--正常
	MEDBOX_CAPSTATUS_FULL,			//药箱容量状态--满仓
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
	int GetCapacityStatus();  //得到药箱的状态

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
	int m_nMaxCapicity;		//最大容量
	int m_nCurNumber;		//当前数量
	COLORREF m_clrStatus;	//状态颜色
	COLORREF m_clrText;		//文本颜色
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDBOXCTRL_H__BA3D3E91_3BF4_4832_ACDB_E5B854F4CF9F__INCLUDED_)
