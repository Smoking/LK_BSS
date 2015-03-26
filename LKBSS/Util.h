#pragma once
#include "Keyboard.h"

#include "Psapi.h"

#pragma comment(lib,"Psapi.lib")

//Ŀǰֻ֧��2������
enum
{
	RCMB_OKCANCEL	= 0, //ȷ��ȡ��
	RCMB_OK			= 1, //ȷ��
};

namespace Util
{
	//������������
	CString GetDialogResPath();  //��ȡ�Ի�����Դ�ļ�

	//��ͼ���
	void Draw9Grid(CDC *pDC, CRect rcRgn, HBITMAP hBitmap); //9������ͼ�㷨
	void xTileRgnWithBmp(CDC *pDC, CRect rcRgn, HBITMAP hBitmap); //����ƽ����ͼ�㷨
	void DrawTransparentBitmap(HDC hdc,HBITMAP hBitmap,short xStart,short yStart,COLORREF cTransparentColor); //����ɫ��ͼ�㷨

	//��ͼƬ����������
	void FillRectWithBmp(CDC *pDC, CString strBmpPath, CRect rect, BOOL bStretch = TRUE);

	//ͨ�öԻ���
	int MessageBox(HWND hWnd , CString strText, CString strCaption = _T(""), UINT uType = RCMB_OKCANCEL);
    //��ģ̬�Ի���
	int ShowMessageBox(HWND hWnd , CString strText, CString strCaption = _T(""), UINT uType = RCMB_OKCANCEL);
}

//����һ��ִ�д򿪹ر�����̵���

class CShellExecuteOsk
{
public:
    CShellExecuteOsk();
    ~CShellExecuteOsk();
	void setCWnd(CWnd* pParentWnd);
	void setIdd(int idd);
	void setNextIdd(int idd);
    void open();
    void close();
	DWORD FindProcess(char *strProcessName);
private:
    static SHELLEXECUTEINFO execInfo; 
    int openType;
	CKeyboard ckb;
	CWnd* m_pParentWnd ;
	bool m_openFlag;
};