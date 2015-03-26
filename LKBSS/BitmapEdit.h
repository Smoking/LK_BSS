#pragma once

class CBitmapEdit : public CEdit
{
	DECLARE_DYNAMIC(CBitmapEdit)

protected:
	CString m_strBitmap;
	CRect	m_rcEditArea;

public:
	BOOL SetBitmaps(CString strBmp);		//����edit����
	BOOL SetEditArea(CRect rcEditArea);		//����edit��Ч����
	void SetThreshold(int nMin, int nMax);  //���÷�ֵ

	CBitmapEdit();
	virtual ~CBitmapEdit();
	virtual void PreSubclassWindow( );
	virtual BOOL PreTranslateMessage( MSG* pMsg );

protected:
	void ResizeWindow();

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam); // Maps to WM_SETFONT
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	int m_nMin;
	int m_nMax;
};


