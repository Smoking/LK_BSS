// LKBSS.h : main header file for the LKBSS application
//

#if !defined(AFX_LKBSS_H__614DC27D_DB8D_4108_941D_8A585F558AAC__INCLUDED_)
#define AFX_LKBSS_H__614DC27D_DB8D_4108_941D_8A585F558AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SysConfig.h"
#include "BIZBoxService.h"
#include "BIZBaseService.h"
#include "Singleton.h"


/////////////////////////////////////////////////////////////////////////////
// CLKBSSApp:
// See LKBSS.cpp for the implementation of this class
//

class CLKBSSApp : public CWinApp
{
public:
	CLKBSSApp();
	~CLKBSSApp();
protected:	

	Singleton<CSysConfig> singletonConfig;
	CSysConfig* cSysConfig;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLKBSSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

		           //œµÕ≥≈‰÷√ƒ£øÈ
	//{{AFX_MSG(CLKBSSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LKBSS_H__614DC27D_DB8D_4108_941D_8A585F558AAC__INCLUDED_)
