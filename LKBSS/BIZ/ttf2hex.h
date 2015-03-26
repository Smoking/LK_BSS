// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Convert wrapper class

class _Convert : public COleDispatchDriver
{
public:
	_Convert() {}		// Calls COleDispatchDriver default constructor
	_Convert(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Convert(const _Convert& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString TTF2Graphic(LPCTSTR vbFontName, short vbFontSize, LPCTSTR vbText, BOOL Download, BSTR* vbFontNo, BOOL* vbFontBold, BOOL* vbFontItalic);
	CString Pic2Graphic(LPCTSTR PicPath, BOOL Download, BSTR* vbFontNo);
};

