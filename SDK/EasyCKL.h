/*
	EasyCKL Interface Header

	BUILD Version: 7.1.?.?
	Copyright (c) daemon_process. All rights reserved.

	!!!WARNING!!! - 接口头文件从 7.1 版本由 EasyCKL 编译构建环境自动鞣制合成，请勿修改
	Report bug: https://git.oschina.net/daemon_process/EasyCKL/issues （如果是此头文件有问题请注明为 头文件生成Bug）
*/

#ifndef __EASY_CKL_H_
#define __EASY_CKL_H_


#define CKLEXPORT extern "C"
//#ifdef _WIN32
//#define CKLEXPORT extern "C" __declspec(dllexport)
//#elif defined __linux__
//#define CKLEXPORT extern "C"
//#endif

typedef unsigned int DWORD;
typedef bool BOOL;
typedef void* LPVOID;
typedef void* PVOID;
typedef size_t SIZE_T;
typedef long LONG;
typedef unsigned long LONG_PTR;
typedef LONG_PTR UINT_PTR;
typedef LONG_PTR ULONG_PTR;
typedef char BYTE;
typedef unsigned int UINT;
typedef unsigned long ULONG;

#define WINAPI
#define TRUE true
#define FALSE false

#define HWND void*
typedef struct tagRECT {
	int x;
	int y;
	int width;
	int height;
}RECT,*LPRECT;
#define HWND CefWindowHandle
#define RECT _cef_rect_t

typedef struct tagECMAINARGS {
	int iArgc;
	char** pszArgv;
}ECMAINARGS,*LPECMAINARGS;

#define HINSTANCE LPECMAINARGS

//#define wchar_t char
//#define std::wstring std::string

typedef BOOL(WINAPI * V8Handler_CallBack)(const wchar_t* name, const void* argu, void* retval);
typedef void(WINAPI * Chrome_CallBack_V8)(void* context);

enum BrowserInfomationType
{
	BrowserInfomationUserDataLoogPtr = 0,
	BrowserInfomationCanGoBack = 1,
	BrowserInfomationCanGoForward = 2,
	BrowserInfomationMainFrame = 3,
	BrowserInfomationIsLoading = 4,
	BrowserInfomationLastError = 5,
	BrowserInfomationBrowserId = 6
};

#define INITFLAG_NOSSL 0x1
#define INITFLAG_CACHESTORAGE 0x2
#define INITFLAG_SINGLEPROCESS 0x4
#define INITFLAG_USECOMPATIBILITY 0x8
#define INITFLAG_ENABLEHIGHDPISUPPORT 0x10
#define INITFLAG_DISABLEGPU 0x20
#define INITFLAG_EXTDATA 0x40
#define INITFLAG_SETUSERAGENT 0x80
#define INITFLAG_SETSUBPROCESS 0x100

typedef struct tagINIT_EXTDATA {
	SIZE_T cbSzie;
	const wchar_t* szUserAgent;
	const wchar_t* szSubProcess;
} INIT_EXTDATA, *LPINIT_EXTDATA;

typedef struct _tagCOOKIE_DESCRIPTOR {
	SIZE_T cbSize;
	DWORD dwRetention;
	const wchar_t* szCookieName;
	const wchar_t* szCookieValue;
	const wchar_t* szCookieDomain;
	const wchar_t* szCookiePath;
	BOOL bHasExpires;
	BOOL bSecure;
	BOOL bHttponly;
	int iExpiresYear;          // Four digit year "2007"
	int iExpiresMonth;         // 1-based month (values 1 = January, etc.)
	int iExpiresDayOfWeek;     // 0-based day of week (0 = Sunday, etc.)
	int iExpiresDayOfMonth;    // 1-based day of month (1-31)
	int iExpiresHour;          // Hour within the current day (0-23)
	int iExpiresMinute;        // Minute within the current hour (0-59)
	int iExpiresSecond;        // Second within the current minute (0-59 plus leap seconds which may take it up to 60).
	int iExpiresMillisecond;   // Milliseconds within the current second (0-999)
} COOKIE_DESCRIPTOR, *LPCOOKIE_DESCRIPTOR;

typedef void(WINAPI * Ec_GetSource_CallBack)(LPVOID lpContext, const wchar_t* szSource);

CKLEXPORT BOOL WINAPI Chrome_IsUIThread();
CKLEXPORT void WINAPI Chrome_Initialize(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage);
CKLEXPORT int WINAPI Chrome_InitializeEx(HINSTANCE hInstance, DWORD dwFlags, LPINIT_EXTDATA lpExtData, wchar_t* szLocal, wchar_t* szCachePath);

CKLEXPORT void* WINAPI Chrome_CreateChildBrowser(DWORD dwFlags, LPBROWSER_CALLBACKS lpCallbacks, DWORD id, wchar_t* szHeaderReferer, 
	wchar_t* szUrl, HWND hParent, RECT* rcBrowserRect, LPCREATE_BROWSER_EXTDATA lpExtData);

CKLEXPORT void* WINAPI Chrome_CreateBrowserSyncWithReferer(wchar_t* referer, DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev);

CKLEXPORT void WINAPI Chrome_CreateBrowserExWithReferer(wchar_t* referer, DWORD id, wchar_t* url, HWND hParent, RECT* rect,
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

CKLEXPORT void WINAPI Chrome_DoMessageLoopWork();
CKLEXPORT void WINAPI Chrome_MessageLoop();
CKLEXPORT void WINAPI Chrome_QuitMessageLoop();
CKLEXPORT void WINAPI Chrome_Shutdown();
CKLEXPORT void WINAPI Chrome_SetUserAgent(const wchar_t* ua);
CKLEXPORT void WINAPI Chrome_SetProxyServer(const wchar_t* proxy);
CKLEXPORT void WINAPI Chrome_LoadUrl(void* handler, wchar_t* url);
CKLEXPORT HWND WINAPI Chrome_Window(void* handler);
CKLEXPORT void WINAPI Chrome_GoBack(void* lpBrowser);
CKLEXPORT void WINAPI Chrome_GoForward(void* lpBrowser);
CKLEXPORT void WINAPI Chrome_Refresh(void* lpBrowser);
CKLEXPORT void WINAPI Chrome_RefreshIgnoreCache(void* lpBrowser);
CKLEXPORT void WINAPI Chrome_Stop(void* lpBrowser);
CKLEXPORT void WINAPI Chrome_SetFocus(void* lpBrowser, bool bFocus);
CKLEXPORT void WINAPI Chrome_ExecJS(void* lpBrowser, const wchar_t* szJavaScript);
CKLEXPORT void WINAPI Chrome_EnableCookieStorageEx(const wchar_t* CookiePath);
CKLEXPORT void WINAPI Chrome_EnableCookieStorage();
CKLEXPORT void WINAPI Chrome_DisableCookieStorage();
CKLEXPORT void WINAPI Chrome_CookieManagerFlushStore();
CKLEXPORT BOOL WINAPI Chrome_CookieManagerSetCookie(const wchar_t* szUrl, LPCOOKIE_DESCRIPTOR lpCookieDescriptor);
CKLEXPORT BOOL WINAPI Chrome_CookieManagerDeleteCookie(const wchar_t* szUrl, const wchar_t* szCookieName);
CKLEXPORT void WINAPI Chrome_Close(void* lpBrowser);
CKLEXPORT void WINAPI Chrome_SetV8ContextCallback(Chrome_CallBack_V8 contextcreate, V8Handler_CallBack handler);
CKLEXPORT void WINAPI Chrome_SetOSModalLoop(bool osModalLoop);
CKLEXPORT DWORD WINAPI Chrome_GetUrlLength(void* handler);
CKLEXPORT void WINAPI Chrome_GetUrlString(void* handler, wchar_t* buffer, DWORD buffer_length);
CKLEXPORT void WINAPI Chrome_LoadString(void* lpBrowser, const wchar_t* string, const wchar_t* url);
CKLEXPORT HWND WINAPI Chrome_GetWindowHandle(void* handler);
CKLEXPORT void WINAPI Chrome_EnableSystemFlash();
CKLEXPORT void WINAPI Chrome_LoadFlashPlugin(wchar_t* ppapi_flash_path, wchar_t* ppapi_flash_version);
CKLEXPORT void WINAPI Chrome_ShowDevTools(void* lpBrowser);
CKLEXPORT void WINAPI Chrome_ShowDevToolsChild(void* handler, HWND hParent, RECT* rect, HWND* devtools_hwnd);
CKLEXPORT void WINAPI Chrome_SetUserDataLongPtr(void* lpBrowser, LONG_PTR data);
CKLEXPORT void WINAPI Chrome_PrintToPDF(void* lpBrowser, wchar_t* pdf_path);
CKLEXPORT double WINAPI Chrome_GetZoomLevel(void* lpBrowser);
CKLEXPORT void WINAPI Chrome_SetZoomLevel(void* lpBrowser, double dbZoomLevel);
CKLEXPORT wchar_t* WINAPI Chrome_DataURIBase64Encode(BYTE* lpData, DWORD dwSize, const wchar_t* szMimeType, const wchar_t* szCharset);
CKLEXPORT void WINAPI Chrome_ReleaseBuffer(void* lpBuffer);
CKLEXPORT void WINAPI Chrome_GetHtmlSource(void* lpBrowser, LPVOID lpContext, Ec_GetSource_CallBack lpCallbackFunction);
CKLEXPORT void WINAPI Chrome_QueryBrowserInfomation(void* handler, BrowserInfomationType type, void* buffer);

CKLEXPORT int WINAPI EcKeInitialize(HINSTANCE hInstance, DWORD flag, wchar_t* local, wchar_t* cache_path, LPINIT_EXTDATA extData);
CKLEXPORT void WINAPI EcKeCookieStorageControl(BOOL enable, const wchar_t* CookiePath, bool persist_session_cookies);

CKLEXPORT void WINAPI Chrome_AddJSFunction(void* context, wchar_t* name);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueListSize(const void* arguments);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueInt(const void* arguments, size_t pos);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueStringLength(const void* arguments, size_t pos);
CKLEXPORT void WINAPI Chrome_GetV8ValueString(const void* arguments, size_t pos, wchar_t* buffer, size_t buffer_length);
CKLEXPORT void WINAPI Chrome_SetV8ReturnValueInt(void* lpretval, int value);
CKLEXPORT void WINAPI Chrome_SetV8ReturnValueString(void* lpretval, wchar_t* value);

typedef bool(WINAPI * SchemeProcessRequest)(void* request, void* data, void* mime_type, UINT* status);
typedef std::vector<unsigned char> void;
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
CKLEXPORT void WINAPI EcCSSetMimeType(std::wstring* lpMimeType, const wchar_t* szMimeType);
CKLEXPORT void WINAPI EcCSSetStatus(int* lpStatus, int iStatus);

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

#define BROWSERFLAG_SYNC 0x1
#define BROWSERFLAG_HEADER_REFERER 0x2
#define BROWSERFLAG_DISABLE_JAVASCRIPT 0x4
#define BROWSERFLAG_DISABLE_LOAD_IMAGE 0x8
#define BROWSERFLAG_DISABLE_WEB_SECURITY 0x10
#define BROWSERFLAG_EXTDATA 0x20
#define BROWSERFLAG_DEF_ENCODING 0x40
#define BROWSERFLAG_BACK_COLOR 0x80
#define BROWSERFLAG_DEF_FONT 0x100
#define BROWSERFLAG_DEF_FONT_SIZE 0x200

#define BROWSER_LASTERROR_LOADING 0x1
#define BROWSER_LASTERROR_LOADERROR 0x2
#define BROWSER_LASTERROR_LOADRESERROR 0x4
#define BROWSER_LASTERROR_CERTERROR 0x8

typedef struct tagCREATE_BROWSER_EXTDATA {
	SIZE_T cbSzie;
	wchar_t* szDefaultEncoding;
	DWORD dwBackColor;
	wchar_t* szDefaultFont;
	DWORD dwDefaultFontSize;
}CREATE_BROWSER_EXTDATA, *LPCREATE_BROWSER_EXTDATA;

/* 回调函数中使用的结构 */
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

typedef struct tagERROR_INFOMATION {
	SIZE_T cbSzie;
	void* lpFrame;
	BOOL bCertError;
	int iErrorCode;
	const wchar_t* szFailedUrl;
	void* lpSslInfo;
}ERROR_INFOMATION, *LPERROR_INFOMATION;

/* 回调函数的定义 */
typedef void(WINAPI * Chrome_CallBack_BrowserCreated)(LONG_PTR id, void* browser);
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
	Chrome_CallBack_CanClose canclose_callback;
}BROWSER_CALLBACKS, *LPBROWSER_CALLBACKS;

#endif // __EASY_CKL_H_