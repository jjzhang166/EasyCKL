#ifndef _EASYCKL
#define _EASYCKL
#define CKLEXPORT extern "C" 

typedef BOOL(WINAPI * V8Handler_CallBack)(const wchar_t* name, const void* argu);
typedef void(WINAPI * Chrome_CallBack_V8)(void* context);
typedef void(WINAPI * Chrome_CallBack_BrowserCreated)(DWORD id, void* browser);
typedef void(WINAPI * Chrome_CallBack_Error)(DWORD id, const char* url);
typedef void(WINAPI * Chrome_CallBack_ChUrl)(DWORD id, const char* url);
typedef void(WINAPI * Chrome_CallBack_Download)(DWORD id, const char* url);
typedef BOOL(WINAPI * Chrome_CallBack_NewWindow)(DWORD id, const char* url);
typedef BOOL(WINAPI * Chrome_CallBack_ChState)(DWORD id, BOOL isLoading, BOOL canGoBack, BOOL canGoForward);
typedef void(WINAPI * Chrome_CallBack_JSDialog)(DWORD id, const wchar_t* msg);
typedef void(WINAPI * Chrome_CallBack_RButtonDown)(DWORD id, int flag, const wchar_t*, const wchar_t* link);
typedef void(WINAPI * Chrome_CallBack_ChTitle)(DWORD id, const wchar_t* text);

CKLEXPORT BOOL WINAPI Chrome_IsUIThread();
CKLEXPORT int WINAPI Chrome_Initialize(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage);
CKLEXPORT int WINAPI Chrome_InitializeEx(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage, wchar_t* local);
CKLEXPORT void* WINAPI Chrome_CreateBrowserSync(DWORD id, char* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, void* rev1, void* rev2);
CKLEXPORT void WINAPI Chrome_CreateBrowserEx(DWORD id, char* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, void* rev1, void* rev2);
CKLEXPORT void WINAPI Chrome_CreateBrowser(DWORD id, char* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown);
CKLEXPORT void WINAPI Chrome_CreateSimple(char* url, HWND hParent, RECT* rect, Chrome_CallBack_BrowserCreated created_callback);
CKLEXPORT void WINAPI ECKL_MessageLoop();
CKLEXPORT void WINAPI Chrome_MessageLoop();
CKLEXPORT void WINAPI Chrome_Shutdown();
CKLEXPORT void WINAPI Chrome_SetUserAgent(wchar_t* ua);
CKLEXPORT void WINAPI Chrome_SetProxyServer(wchar_t* proxy);
CKLEXPORT void WINAPI Chrome_LoadUrl(void* handler, char* url);
CKLEXPORT HWND WINAPI Chrome_Window(void* handler);
CKLEXPORT void WINAPI Chrome_GoBack(void* handler);
CKLEXPORT void WINAPI Chrome_GoForward(void* handler);
CKLEXPORT void WINAPI Chrome_Refresh(void* handler);
CKLEXPORT void WINAPI Chrome_Stop(void* handler);
CKLEXPORT void WINAPI Chrome_ExecJS(void* handler, wchar_t* js);
CKLEXPORT void WINAPI Chrome_EnableCookieStorageEx(wchar_t* CookiePath);
CKLEXPORT void WINAPI Chrome_EnableCookieStorage();
CKLEXPORT void WINAPI Chrome_DisableCookieStorage();
CKLEXPORT void WINAPI Chrome_Close(void* handler);
CKLEXPORT void WINAPI Chrome_SetV8ContextCallback(Chrome_CallBack_V8 contextcreate, V8Handler_CallBack handler);
CKLEXPORT void WINAPI Chrome_AddJSFunction(void* context, wchar_t* name);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueListSize(const void* arguments);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueInt(const void* arguments, size_t pos);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueStringLength(const void* arguments, size_t pos);
CKLEXPORT void WINAPI Chrome_GetV8ValueString(const void* arguments, size_t pos, wchar_t* buffer, size_t buffer_length);

#endif