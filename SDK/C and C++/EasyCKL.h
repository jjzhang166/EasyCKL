#ifndef _EASYCKL
#define _EASYCKL

#ifdef __EC_LINUX_API
#include "ec_portable.h"
#endif

#ifdef CKLEXPORT
#undef CKLEXPORT
#endif
#define CKLEXPORT extern "C" 

#ifndef  __ECKL_SRC_DEV_
typedef struct tagNEW_WINDOW_INFOMATION {
	SIZE_T cbSize;
	void* lpFrame;
	const wchar_t* szNewWindowUrl;
	const wchar_t* szCurrentWindowUrl;
	const wchar_t* szTargetFrameName;
	BOOL bUserGesture;
	DWORD dwOpenDisposition;
}NEW_WINDOW_INFOMATION, *LPNEW_WINDOW_INFOMATION;

// No node is selected.
#define MENU_TYPEFLAG_NONE 0
// The top page is selected.
#define MENU_TYPEFLAG_PAGE 1 << 0
// A subframe page is selected.
#define MENU_TYPEFLAG_FRAME 1 << 1
// A link is selected.
#define MENU_TYPEFLAG_LINK 1 << 2
// A media node is selected.
#define MENU_TYPEFLAG_MEDIA 1 << 3
// There is a textual or mixed selection that is selected.
#define MENU_TYPEFLAG_SELECTION 1 << 4
// An editable element is selected.
#define MENU_TYPEFLAG_EDITABLE 1 << 5

typedef struct tagRBUTTON_DOWN_INFOMATION {
	SIZE_T cbSize;
	DWORD dwFlag;
	void* lpFrame;
	const wchar_t* szSelectionText;
	const wchar_t* szLinkUrl;
	const wchar_t* szSourceUrl;
	void* Retention;
}RBUTTON_DOWN_INFOMATION, *LPRBUTTON_DOWN_INFOMATION;

typedef struct tagERROR_INFOMATION {
	SIZE_T cbSize;
	void* lpFrame;
	BOOL bCertError;
	int iErrorCode;
	const wchar_t* szFailedUrl;
	void* lpSslInfo;
}ERROR_INFOMATION, *LPERROR_INFOMATION;
#endif // __ECKL_SRC_DEV_

typedef BOOL(WINAPI * V8Handler_CallBack)(const wchar_t* name, const void* argu, void* retval);
typedef void(WINAPI * Chrome_CallBack_V8)(void* context);
typedef void(WINAPI * Chrome_CallBack_BrowserCreated)(LONG_PTR id, void* lpBrowser);
typedef void(WINAPI * Chrome_CallBack_Error)(LONG_PTR id, UINT_PTR uMsg, LPERROR_INFOMATION info, UINT_PTR not_used);
typedef void(WINAPI * Chrome_CallBack_ChUrl)(LONG_PTR id, const wchar_t* url);
typedef void(WINAPI * Chrome_CallBack_Download)(LONG_PTR id, const wchar_t* url);
typedef BOOL(WINAPI * Chrome_CallBack_NewWindow)(LONG_PTR id, UINT_PTR uMsg, LPNEW_WINDOW_INFOMATION info, UINT_PTR not_used);
typedef BOOL(WINAPI * Chrome_CallBack_ChState)(LONG_PTR id, BOOL isLoading, BOOL canGoBack, BOOL canGoForward);
typedef void(WINAPI * Chrome_CallBack_JSDialog)(LONG_PTR id, const wchar_t* msg);
typedef void(WINAPI * Chrome_CallBack_RButtonDown)(LONG_PTR id, UINT_PTR uMsg, LPRBUTTON_DOWN_INFOMATION info, UINT_PTR not_used);
typedef void(WINAPI * Chrome_CallBack_ChTitle)(LONG_PTR id, const wchar_t* text);
typedef bool(WINAPI * Chrome_CallBack_CanLoadUrl)(LONG_PTR id, const wchar_t* url);
typedef bool(WINAPI * Chrome_CallBack_CanClose)(LONG_PTR id, UINT_PTR uMsg, void* not_used, UINT_PTR not_used_);

/*typedef struct tagBROWSER_CALLBACKS {
	SIZE_T cbSize;
	Chrome_CallBack_BrowserCreated created_callback;
	Chrome_CallBack_ChUrl churl_callback;
	Chrome_CallBack_NewWindow newwindow_callback;
	Chrome_CallBack_Download download_callback;
	Chrome_CallBack_ChState chstate_callback;
	Chrome_CallBack_JSDialog jsdialog_callback;
	Chrome_CallBack_Error error_callback;
	Chrome_CallBack_RButtonDown rbuttondown_callback;
	Chrome_CallBack_ChTitle chtitle_callback;
	Chrome_CallBack_CanLoadUrl canloadurl_callback;
}BROWSER_CALLBACKS, *LPBROWSER_CALLBACKS;*/

#ifndef __EC_PACK_API_CPP_
CKLEXPORT void WINAPI EcPkHtmlRefreshContentJumpUrl(void* lpBrowser, wchar_t* url, wchar_t* referer);
CKLEXPORT void* WINAPI EcPkCreateJSRefererBrowserSync(DWORD id, HWND hParent, RECT* rect, wchar_t* url, wchar_t* referer, LPBROWSER_CALLBACKS callbacks);
CKLEXPORT void WINAPI EcPkDisableDragDrop(void* lpBrowser);
CKLEXPORT void WINAPI EcPkJavaScriptSetValueByObjectId(void* lpBrowser, wchar_t* id, wchar_t* value);
CKLEXPORT void WINAPI EcPkJavaScriptSetValueByObjectName(void* lpBrowser, wchar_t* name, wchar_t* value);
CKLEXPORT void WINAPI EcPkJavaScriptSubmitByFormId(void* lpBrowser, wchar_t* id);
CKLEXPORT void WINAPI EcPkJavaScriptSubmitByFormName(void* lpBrowser, wchar_t* name);
CKLEXPORT void WINAPI EcPkJavaScriptSetCheckedByObjectId(void* lpBrowser, wchar_t* id, bool checked);
CKLEXPORT void WINAPI EcPkJavaScriptSetCheckedByObjectName(void* lpBrowser, wchar_t* name, bool checked);
CKLEXPORT void WINAPI EcPkJavaScriptClickButtonByObjectId(void* lpBrowser, wchar_t* id);
CKLEXPORT void WINAPI EcPkJavaScriptClickButtonByObjectName(void* lpBrowser, wchar_t* name);
#endif // __EC_PACK_API_CPP_

#ifndef  __EC_CUSTOMIZE_JS_CPP_
CKLEXPORT void WINAPI Chrome_AddJSFunction(void* context, wchar_t* name);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueListSize(const void* arguments);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueInt(const void* arguments, size_t pos);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueStringLength(const void* arguments, size_t pos);
CKLEXPORT void WINAPI Chrome_GetV8ValueString(const void* arguments, size_t pos, wchar_t* buffer, size_t buffer_length);
CKLEXPORT void WINAPI Chrome_SetV8ReturnValueInt(const void* lpretval, int value);
CKLEXPORT void WINAPI Chrome_SetV8ReturnValueString(const void* lpretval, wchar_t* value);
#endif // __EC_CUSTOMIZE_JS_CPP_

#ifndef  __EC_FRAME_API_CPP_
CKLEXPORT void WINAPI Chrome_FrameLoadUrl(void* frame, wchar_t* url);
CKLEXPORT void WINAPI Chrome_FrameLoadString(void* frame, wchar_t* string, wchar_t* url);
CKLEXPORT bool WINAPI Chrome_FrameIsMain(void* frame);
CKLEXPORT void WINAPI Chrome_ReleaseFrame(void* frame);
CKLEXPORT void* WINAPI Chrome_GetNameFrame(void* lpBrowser, wchar_t* name);
CKLEXPORT void* WINAPI EcQBIGetMainFrame(void* lpBrowser);
CKLEXPORT wchar_t* WINAPI Chrome_FrameGetUrl(void* lpFrame);
CKLEXPORT void WINAPI Chrome_FrameDoCopy(void* frame);
CKLEXPORT void WINAPI Chrome_FrameDoCut(void* frame);
CKLEXPORT void WINAPI Chrome_FrameDoDelete(void* frame);
CKLEXPORT void WINAPI Chrome_FrameDoPaste(void* frame);
CKLEXPORT void WINAPI Chrome_FrameDoSelectAll(void* frame);
#endif // __EC_FRAME_API_CPP_

#ifndef  __EC_CUSTOMIZE_SCHEME_CPP_
typedef bool(WINAPI * SchemeProcessRequest)(void* request, void* data, void* mime_type, UINT* status);

CKLEXPORT void WINAPI Chrome_RegisterSchemeInitialize(SchemeProcessRequest callback);
CKLEXPORT void WINAPI Chrome_RegisterScheme(const wchar_t* szSchemeName, const wchar_t* szDomainName);
CKLEXPORT SIZE_T WINAPI EcCSGetRequestUrlLength(void* lpRequest);
CKLEXPORT void WINAPI EcCSGetRequestUrl(void* lpRequest, wchar_t* lpUrlBuffer, ULONG ulLength);
CKLEXPORT SIZE_T WINAPI EcCSGetRequestHeaderStringLength(void* lpRequest, wchar_t* szHeaderName);
CKLEXPORT void WINAPI EcCSGetRequestHeaderString(void* lpRequest, wchar_t* szHeaderName, wchar_t* lpBuffer, ULONG ulLength);
CKLEXPORT SIZE_T WINAPI EcCSGetRequestPostDataSize(void* lpRequest);
CKLEXPORT void WINAPI EcCSGetRequestPostData(void* lpRequest, BYTE* lpDataBuffer, ULONG_PTR notused);
CKLEXPORT void WINAPI EcCSSetData(void* lpData, unsigned char* lpDataBuffer, SIZE_T nSize);
CKLEXPORT void WINAPI EcCSSetMimeType(void* lpMimeType, const wchar_t* szMimeType);
CKLEXPORT void WINAPI EcCSSetStatus(int* lpStatus, int iStatus);
#endif // __EC_CUSTOMIZE_SCHEME_CPP_

#ifdef  __ECKL_SRC_DEV_
void _ECKL_CopyWString(std::wstring source, wchar_t* buffer, size_t buffer_length);
#endif // __ECKL_SRC_DEV_

#ifdef __EC_LINUX_API
#undef TRUE
#undef FALSE
#endif

#endif
