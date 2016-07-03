#ifndef _EASYCKL
#define _EASYCKL
#define CKLEXPORT extern "C" 

#ifndef  __ECKL_SRC_DEV_
typedef struct tagNEW_WINDOW_INFOMATION {
	SIZE_T cbSzie;
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
	SIZE_T cbSzie;
	DWORD dwFlag;
	void* lpFrame;
	const wchar_t* szSelectionText;
	const wchar_t* szLinkUrl;
	const wchar_t* szSourceUrl;
	void* Retention;
}RBUTTON_DOWN_INFOMATION, *LPRBUTTON_DOWN_INFOMATION;
#endif // __ECKL_SRC_DEV_

typedef BOOL(WINAPI * V8Handler_CallBack)(const wchar_t* name, const void* argu, void* retval);
typedef void(WINAPI * Chrome_CallBack_V8)(void* context);
typedef void(WINAPI * Chrome_CallBack_BrowserCreated)(LONG_PTR id, void* browser);
typedef void(WINAPI * Chrome_CallBack_Error)(LONG_PTR id, const wchar_t* url, BOOL isCertError);
typedef void(WINAPI * Chrome_CallBack_ChUrl)(LONG_PTR id, const wchar_t* url);
typedef void(WINAPI * Chrome_CallBack_Download)(LONG_PTR id, const wchar_t* url);
typedef BOOL(WINAPI * Chrome_CallBack_NewWindow)(LONG_PTR id, UINT_PTR uMsg, LPNEW_WINDOW_INFOMATION info, UINT_PTR not_used);
typedef BOOL(WINAPI * Chrome_CallBack_ChState)(LONG_PTR id, BOOL isLoading, BOOL canGoBack, BOOL canGoForward);
typedef void(WINAPI * Chrome_CallBack_JSDialog)(LONG_PTR id, const wchar_t* msg);
typedef void(WINAPI * Chrome_CallBack_RButtonDown)(LONG_PTR id, UINT_PTR uMsg, LPRBUTTON_DOWN_INFOMATION info, UINT_PTR not_used);
typedef void(WINAPI * Chrome_CallBack_ChTitle)(LONG_PTR id, const wchar_t* text);
typedef bool(WINAPI * Chrome_CallBack_CanLoadUrl)(LONG_PTR id, const wchar_t* url);

#ifndef  __ECKL_SRC_DEV_
typedef struct tagBROWSER_CALLBACKS {
	SIZE_T cbSzie;
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
}BROWSER_CALLBACKS, *LPBROWSER_CALLBACKS;
#endif // __ECKL_SRC_DEV_

CKLEXPORT BOOL WINAPI Chrome_IsUIThread();
CKLEXPORT int WINAPI Chrome_Initialize(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage);
CKLEXPORT int WINAPI Chrome_InitializeEx(HINSTANCE hInstance, DWORD flag, BOOL old_ver, wchar_t* local, wchar_t* cache_path);

#define BROWSERFLAG_SYNC 0x1
#define BROWSERFLAG_HEADER_REFERER 0x2
#define BROWSERFLAG_DISABLE_JAVASCRIPT 0x4
#define BROWSERFLAG_DISABLE_LOAD_IMAGE 0x8
#define BROWSERFLAG_DISABLE_WEB_SECURITY 0x10
#define BROWSERFLAG_EXTDATA 0x20
#define BROWSERFLAG_DEF_ENCODING 0x40
#define BROWSERFLAG_BACK_COLOR 0x80

#ifndef  __ECKL_SRC_DEV_
typedef struct tagCREATE_BROWSER_EXTDATA {
	SIZE_T cbSzie;
	wchar_t* szDefaultEncoding;
	DWORD dwBackColor;
}CREATE_BROWSER_EXTDATA, *LPCREATE_BROWSER_EXTDATA;
#endif // __ECKL_SRC_DEV_

CKLEXPORT void* WINAPI Chrome_CreateChildBrowser(DWORD dwFlags, LPBROWSER_CALLBACKS lpCallbacks, DWORD id, wchar_t* szHeaderReferer, wchar_t* szUrl, HWND hParent, RECT* rcBrowserRect, LPCREATE_BROWSER_EXTDATA lpExtData);

CKLEXPORT void* WINAPI Chrome_CreateBrowserSyncWithReferer(wchar_t* referer, DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev);

CKLEXPORT void* WINAPI Chrome_CreateBrowserExWithReferer(wchar_t* referer, DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev);

CKLEXPORT void* WINAPI Chrome_CreateBrowserSync(DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev);

CKLEXPORT void WINAPI Chrome_CreateBrowserEx(DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev);

CKLEXPORT void WINAPI Chrome_CreateBrowser(DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown);

CKLEXPORT void WINAPI Chrome_MessageLoop();
CKLEXPORT void WINAPI Chrome_Shutdown();
CKLEXPORT void WINAPI Chrome_SetUserAgent(const wchar_t* ua);
CKLEXPORT void WINAPI Chrome_SetProxyServer(const wchar_t* proxy);
CKLEXPORT void WINAPI Chrome_LoadUrl(void* browser, const wchar_t* url);
CKLEXPORT HWND WINAPI Chrome_Window(void* browser);
CKLEXPORT void WINAPI Chrome_GoBack(void* browser);
CKLEXPORT void WINAPI Chrome_GoForward(void* browser);
CKLEXPORT void WINAPI Chrome_Refresh(void* browser);
CKLEXPORT void WINAPI Chrome_Stop(void* browser);
CKLEXPORT void WINAPI Chrome_ExecJS(void* browser, const wchar_t* js);
CKLEXPORT void WINAPI Chrome_EnableCookieStorageEx(const wchar_t* CookiePath);
CKLEXPORT void WINAPI Chrome_EnableCookieStorage();
CKLEXPORT void WINAPI Chrome_DisableCookieStorage();
CKLEXPORT void WINAPI Chrome_CookieManagerFlushStore();
CKLEXPORT void WINAPI Chrome_Close(void* browser);
CKLEXPORT void WINAPI Chrome_SetV8ContextCallback(Chrome_CallBack_V8 contextcreate, V8Handler_CallBack handler);
CKLEXPORT void WINAPI Chrome_SetOSModalLoop(bool osModalLoop);
CKLEXPORT DWORD WINAPI Chrome_GetUrlLength(void* browser);
CKLEXPORT void WINAPI Chrome_GetUrlString(void* browser, wchar_t* buffer, DWORD buffer_length);
CKLEXPORT void WINAPI Chrome_DoMessageLoopWork();
CKLEXPORT void WINAPI Chrome_LoadString(void* browser, const wchar_t* string, const wchar_t* url);
CKLEXPORT HWND WINAPI Chrome_GetWindowHandle(void* browser);
CKLEXPORT void WINAPI Chrome_EnableSystemFlash();
CKLEXPORT void WINAPI Chrome_LoadFlashPlugin(wchar_t* ppapi_flash_path, wchar_t* ppapi_flash_version);
CKLEXPORT void WINAPI Chrome_ShowDevTools(void* browser);
CKLEXPORT void WINAPI Chrome_ShowDevToolsChild(void* browser, HWND hParent, RECT* rect);
CKLEXPORT void WINAPI Chrome_SetUserDataLongPtr(void* browser, LONG_PTR data);
CKLEXPORT void WINAPI Chrome_PrintToPDF(void* browser, wchar_t* pdf_path);
CKLEXPORT wchar_t* WINAPI Chrome_DataURIBase64Encode(BYTE* lpData, DWORD dwSize, const wchar_t* szMimeType, const wchar_t* szCharset);
CKLEXPORT void WINAPI Chrome_ReleaseBuffer(void* lpBuffer);

enum BrowserInfomationType
{
	BrowserInfomationUserDataLoogPtr = 0,
	BrowserInfomationCanGoBack = 1,
	BrowserInfomationCanGoForward = 2,
	BrowserInfomationMainFrame = 3,
	BrowserInfomationIsLoading = 4,
	BrowserInfomationLastError = 5
};

#ifndef  __ECKL_SRC_DEV_
#define BROWSER_LASTERROR_LOADING 0x1
#define BROWSER_LASTERROR_LOADERROR 0x2
#define BROWSER_LASTERROR_LOADRESERROR 0x4
#define BROWSER_LASTERROR_CERTERROR 0x8
#endif // __ECKL_SRC_DEV_

CKLEXPORT void WINAPI Chrome_QueryBrowserInfomation(void* browser, BrowserInfomationType type, void* buffer);

CKLEXPORT int WINAPI EcKeInitialize(HINSTANCE hInstance, DWORD flag, wchar_t* local, wchar_t* cache_path, void* recvd);
CKLEXPORT void WINAPI EcKeCookieStorageControl(BOOL enable, wchar_t* CookiePath, bool persist_session_cookies);

#ifndef __EC_PACK_API_CPP_
CKLEXPORT void WINAPI EcPkHtmlRefreshContentJumpUrl(void* browser, wchar_t* url, wchar_t* referer);
CKLEXPORT void WINAPI EcPkJavaScriptSetValueByObjectId(void* browser, wchar_t* id, wchar_t* value);
CKLEXPORT void WINAPI EcPkJavaScriptSetValueByObjectName(void* browser, wchar_t* name, wchar_t* value);
CKLEXPORT void WINAPI EcPkJavaScriptSubmitByFormId(void* browser, wchar_t* id);
CKLEXPORT void WINAPI EcPkJavaScriptSubmitByFormName(void* browser, wchar_t* name);
CKLEXPORT void* WINAPI EcPkCreateJSRefererBrowserSync(DWORD id, HWND hParent, RECT* rect, wchar_t* url, wchar_t* referer, LPBROWSER_CALLBACKS callbacks);
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
CKLEXPORT void* WINAPI Chrome_GetNameFrame(void* browser, wchar_t* name);
CKLEXPORT void* WINAPI EcQBIGetMainFrame(void* browser);
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
void _ECKL_CopyWString(CefString source, wchar_t* buffer, size_t buffer_length);
#endif // __ECKL_SRC_DEV_

#endif