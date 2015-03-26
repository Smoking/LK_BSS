// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "ttf2hex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _Convert properties

/////////////////////////////////////////////////////////////////////////////
// _Convert operations

CString _Convert::TTF2Graphic(LPCTSTR vbFontName, short vbFontSize, LPCTSTR vbText, BOOL Download, BSTR* vbFontNo, BOOL* vbFontBold, BOOL* vbFontItalic)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_BSTR VTS_BOOL VTS_PBSTR VTS_PBOOL VTS_PBOOL;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		vbFontName, vbFontSize, vbText, Download, vbFontNo, vbFontBold, vbFontItalic);
	return result;
}

CString _Convert::Pic2Graphic(LPCTSTR PicPath, BOOL Download, BSTR* vbFontNo)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_PBSTR;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		PicPath, Download, vbFontNo);
	return result;
}


