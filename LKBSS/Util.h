#pragma once
#include "Keyboard.h"

#include "Psapi.h"

#pragma comment(lib,"Psapi.lib")

//目前只支持2种类型
enum
{
	RCMB_OKCANCEL	= 0, //确定取消
	RCMB_OK			= 1, //确定
};

namespace Util
{
	//公共帮助函数
	CString GetDialogResPath();  //获取对话框资源文件

	//贴图相关
	void Draw9Grid(CDC *pDC, CRect rcRgn, HBITMAP hBitmap); //9宫格贴图算法
	void xTileRgnWithBmp(CDC *pDC, CRect rcRgn, HBITMAP hBitmap); //横向平铺贴图算法
	void DrawTransparentBitmap(HDC hdc,HBITMAP hBitmap,short xStart,short yStart,COLORREF cTransparentColor); //掩码色贴图算法

	//用图片填充矩形区域
	void FillRectWithBmp(CDC *pDC, CString strBmpPath, CRect rect, BOOL bStretch = TRUE);

	//通用对话框
	int MessageBox(HWND hWnd , CString strText, CString strCaption = _T(""), UINT uType = RCMB_OKCANCEL);
    //非模态对话框
	int ShowMessageBox(HWND hWnd , CString strText, CString strCaption = _T(""), UINT uType = RCMB_OKCANCEL);
}

//这是一个执行打开关闭软键盘的类

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