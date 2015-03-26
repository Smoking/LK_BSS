// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3CECB1F6_98E8_417F_AB19_7470AD34D2CD__INCLUDED_)
#define AFX_STDAFX_H__3CECB1F6_98E8_417F_AB19_7470AD34D2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma   warning(disable:   4786) 
#pragma   warning(disable:   4231) 
#pragma   warning(disable:   4660)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "resource.h"
#define WM_REFRESH     WM_USER+100   //�Զ���ˢ����Ϣ
#define WM_FINISH_ADD  WM_USER+101 //��ҩ�����Ϣ
#define WM_DELETEMED   WM_USER+102 //ɾ��ҩƷ��Ϣ
#define WM_BUTTONDOWN  WM_USER+103 //��ť������Ϣ
#define WM_MONITORMESSAGE  WM_USER+104 //�����Ϣ
#define WM_THREADSTATE WM_USER+106 //��ҩ�߳�״̬��Ϣ
#define WM_ERRORADDRESS WM_USER+107 //��ҩ�����ַ��Ϣ
#define WM_GETERRORMESSAGE  WM_USER+110 //��ȡPCB�������Ϣ
extern bool isRunning;

#define INFOZONE_BK_COLOR	RGB(214, 220, 226)
#define GROUP_BK_COLOR	    RGB(7,148,201)
#define LIST_BK_COLOR       RGB(115,123,165)

#define COLOR_GREEN		RGB(0, 255, 64)
#define COLOR_RED		RGB(255, 0, 0)
#define COLOR_WHITE	    RGB(255, 255, 255)
#define COLOR_YELLOW	RGB(255, 200, 0)
#define COLOR_BLUE		RGB(0, 128, 255)
#define COLOR_GRAY		RGB(213, 216, 219)
#define COLOR_EMPORT	RGB(40, 73, 103)


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3CECB1F6_98E8_417F_AB19_7470AD34D2CD__INCLUDED_)
