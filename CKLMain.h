#ifndef _CKLMAIN_H_
#define _CKLMAIN_H_
/*
Note: the "DO NOT AUTO REMOVE" in a end of a line means that DISABLE the SDK头文件自动生成脚本 remove the line.
the "REMOVE IT" in a end of a line means that force the SDK头文件自动生成脚本 remove the line.
*/
#include "ec_portable.h"

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_parser.h"

#include "simple_app.h"
#include "simple_handler.h"

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

void _ECKL_CopyWString(std::wstring source, wchar_t* buffer, size_t buffer_length);			/* REMOVE IT */

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
CKLEXPORT void WINAPI Chrome_LoadUrl(SimpleHandler* handler, wchar_t* url);
CKLEXPORT HWND WINAPI Chrome_Window(SimpleHandler* handler);
CKLEXPORT void WINAPI Chrome_GoBack(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI Chrome_GoForward(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI Chrome_Refresh(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI Chrome_RefreshIgnoreCache(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI Chrome_Stop(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI Chrome_SetFocus(SimpleHandler* lpBrowser, bool bFocus);
CKLEXPORT void WINAPI Chrome_ExecJS(SimpleHandler* lpBrowser, const wchar_t* szJavaScript);
CKLEXPORT void WINAPI Chrome_EnableCookieStorageEx(const wchar_t* CookiePath);
CKLEXPORT void WINAPI Chrome_EnableCookieStorage();
CKLEXPORT void WINAPI Chrome_DisableCookieStorage();
CKLEXPORT void WINAPI Chrome_CookieManagerFlushStore();
CKLEXPORT BOOL WINAPI Chrome_CookieManagerSetCookie(const wchar_t* szUrl, LPCOOKIE_DESCRIPTOR lpCookieDescriptor);
CKLEXPORT BOOL WINAPI Chrome_CookieManagerDeleteCookie(const wchar_t* szUrl, const wchar_t* szCookieName);
CKLEXPORT void WINAPI Chrome_Close(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI Chrome_SetV8ContextCallback(Chrome_CallBack_V8 contextcreate, V8Handler_CallBack handler);
CKLEXPORT void WINAPI Chrome_SetOSModalLoop(bool osModalLoop);
CKLEXPORT DWORD WINAPI Chrome_GetUrlLength(SimpleHandler* handler);
CKLEXPORT void WINAPI Chrome_GetUrlString(SimpleHandler* handler, wchar_t* buffer, DWORD buffer_length);
CKLEXPORT void WINAPI Chrome_LoadString(SimpleHandler* lpBrowser, const wchar_t* string, const wchar_t* url);
CKLEXPORT HWND WINAPI Chrome_GetWindowHandle(SimpleHandler* handler);
CKLEXPORT void WINAPI Chrome_EnableSystemFlash();
CKLEXPORT void WINAPI Chrome_LoadFlashPlugin(wchar_t* ppapi_flash_path, wchar_t* ppapi_flash_version);
CKLEXPORT void WINAPI Chrome_ShowDevTools(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI Chrome_ShowDevToolsChild(SimpleHandler* handler, HWND hParent, RECT* rect, HWND* devtools_hwnd);
CKLEXPORT void WINAPI Chrome_SetUserDataLongPtr(SimpleHandler* lpBrowser, LONG_PTR data);
CKLEXPORT void WINAPI Chrome_PrintToPDF(SimpleHandler* lpBrowser, wchar_t* pdf_path);
CKLEXPORT double WINAPI Chrome_GetZoomLevel(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI Chrome_SetZoomLevel(SimpleHandler* lpBrowser, double dbZoomLevel);
CKLEXPORT wchar_t* WINAPI Chrome_DataURIBase64Encode(BYTE* lpData, DWORD dwSize, const wchar_t* szMimeType, const wchar_t* szCharset);
CKLEXPORT void WINAPI Chrome_ReleaseBuffer(void* lpBuffer);
CKLEXPORT void WINAPI Chrome_GetHtmlSource(SimpleHandler* lpBrowser, LPVOID lpContext, Ec_GetSource_CallBack lpCallbackFunction);
CKLEXPORT void WINAPI Chrome_QueryBrowserInfomation(SimpleHandler* handler, BrowserInfomationType type, void* buffer);

CKLEXPORT int WINAPI EcKeInitialize(HINSTANCE hInstance, DWORD flag, wchar_t* local, wchar_t* cache_path, LPINIT_EXTDATA extData);
CKLEXPORT void WINAPI EcKeCookieStorageControl(BOOL enable, const wchar_t* CookiePath, bool persist_session_cookies);

#endif // _CKLMAIN_H_
