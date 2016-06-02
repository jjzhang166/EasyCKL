#include <Windows.h>

#include "include/cef_sandbox_win.h"

#include "simple_app.h"
#include "simple_handler.h"

#ifdef _DEBUG
#pragma comment(lib, "libcef_d.lib")
#else
#pragma comment(lib, "libcef.lib")
#endif // _DEBUG

#pragma comment(lib, "libcef_dll_wrapper.lib")
#pragma comment(lib, "user32.lib")

//#define _EPL_COMPATIBILITY

#ifdef _EPL_COMPATIBILITY
#include<Shellapi.h>
#pragma comment(lib, "Shell32.lib")
NOTIFYICONDATA nid = { 0 };
#endif // _EPL_COMPATIBILITY

#define CKLEXPORT extern "C" __declspec(dllexport)

typedef BOOL(WINAPI * V8Handler_CallBack)(const wchar_t* name, const void* argu, void* retval);
typedef void(WINAPI * Chrome_CallBack_V8)(void* context);

class MyV8Handler : public CefV8Handler {
public:
	MyV8Handler(V8Handler_CallBack handler) {
		handler_callback = handler;
	}
	~MyV8Handler() {}
	V8Handler_CallBack handler_callback = 0;
	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) OVERRIDE {

		if (handler_callback) {
			return handler_callback(name.c_str(), &arguments, &retval) != FALSE;
		}
		return false;
	}

	// Provide the reference counting implementation for this class.
	IMPLEMENT_REFCOUNTING(MyV8Handler);
};

extern HANDLE hEvent = 0;
void* v8contextcreate = 0;

extern BOOL isSetProxy = FALSE;
extern CefString proxyserver("");

extern BOOL flash = FALSE;

extern CefString localinf("");

CefRefPtr<CefV8Handler> myV8handle;

BOOL isSetUA = FALSE;
CefString ua;

void _ECKL_CopyWString(CefString source, wchar_t* buffer, size_t buffer_length) {
	const wchar_t* a = source.c_str();
	DWORD leng = (wcslen(a) + 1) * sizeof(wchar_t);//获取字节数
	if (leng <= buffer_length)
		memcpy(buffer, a, leng);
	else {
		memcpy(buffer, a, buffer_length - 2);
		memset(((char*)buffer) + buffer_length - 2, 0, 2);//最后两个字节置0
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LONG WINAPI excpcallback(_EXCEPTION_POINTERS* excp)
{
	return EXCEPTION_EXECUTE_HANDLER;
}

CKLEXPORT BOOL WINAPI Chrome_CurrentlyOn(CefThreadId threadId) {
	return CefCurrentlyOn(threadId);
}

CKLEXPORT BOOL WINAPI Chrome_IsUIThread() {
	return Chrome_CurrentlyOn(TID_UI);
}

enum BrowserInfomationType
{
	BrowserInfomationUserDataLoogPtr = 0,
	BrowserInfomationCanGoBack = 1,
	BrowserInfomationCanGoForward = 2,
	BrowserInfomationMainFrame = 3,
	BrowserInfomationIsLoading = 4,
	BrowserInfomationLastError = 5
};

CKLEXPORT void WINAPI Chrome_QueryBrowserInfomation(SimpleHandler* handler, BrowserInfomationType type, void* buffer) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			switch (type)
			{
			case BrowserInfomationUserDataLoogPtr:
				*(LONG*)buffer = handler->userData;
				break;
			case BrowserInfomationCanGoBack:
				*(BOOL*)buffer = browser->CanGoBack();
				break;
			case BrowserInfomationCanGoForward:
				*(BOOL*)buffer = browser->CanGoForward();
				break;
			case BrowserInfomationMainFrame:
				CefFrame* main; main = browser->GetMainFrame().get();
				main->AddRef();
				*(CefFrame**)buffer = main;
				break;
			case BrowserInfomationIsLoading:
				*(BOOL*)buffer = browser->IsLoading();
				break;
			case BrowserInfomationLastError:
				*(DWORD*)buffer = handler->lasterror;
				break;
			default:
				break;
			}
		}
	}
}

#define INITFLAG_NOSSL 0x1
#define INITFLAG_CACHESTORAGE 0x2
#define INITFLAG_SINGLEPROCESS 0x4
#define INITFLAG_USECOMPATIBILITY 0x8
#define INITFLAG_ENABLEHIGHDPISUPPORT 0x10

//Un Document
CKLEXPORT int WINAPI EcKeInitialize(HINSTANCE hInstance, DWORD flag, wchar_t* local, wchar_t* cache_path, void* recvd) {
	SetUnhandledExceptionFilter(excpcallback);
	hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);


	CefMainArgs main_args(hInstance);
	CefRefPtr<SimpleApp> app(new SimpleApp);

	int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
	if (exit_code >= 0) {
		return exit_code;
	}

	CefSettings settings;
	settings.command_line_args_disabled = true;
	settings.no_sandbox = true;

	if (isSetUA)
		CefString(&settings.user_agent) = ua;

#ifdef _DEBUG
	settings.log_severity = LOGSEVERITY_INFO;
#else
	settings.log_severity = LOGSEVERITY_DISABLE;
#endif // _DEBUG

	if (flag & INITFLAG_CACHESTORAGE) {
		if (cache_path) CefString(&settings.cache_path) = CefString(&settings.user_data_path) = cache_path;
		else CefString(&settings.cache_path) = CefString(&settings.user_data_path) = L".\\cache\\";
	}

	if (flag & INITFLAG_USECOMPATIBILITY)
		CefString(&settings.browser_subprocess_path) = L"eckl_epl_compatibility.exe";

	if (flag & INITFLAG_SINGLEPROCESS)
		settings.single_process = true;

	if (flag & INITFLAG_NOSSL)
		settings.ignore_certificate_errors = true;
	else
		settings.ignore_certificate_errors = false;

	if (local)
		localinf = local;

	CefInitialize(main_args, settings, app.get(), nullptr);
	WaitForSingleObject(hEvent, INFINITE);

	if (flag & INITFLAG_ENABLEHIGHDPISUPPORT)
		CefEnableHighDPISupport();

	if (Chrome_IsUIThread()) {
		SetUnhandledExceptionFilter(0);
	}
	return -1;
}

CKLEXPORT int WINAPI Chrome_InitializeEx(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage, wchar_t* local, wchar_t* cache_path) {
	DWORD flag = 0;
	int ret = 0;

#ifdef _EPL_COMPATIBILITY
	if (!IsDebuggerPresent()) {
		ExitProcess(-1);
	}
	flag |= INITFLAG_USECOMPATIBILITY;
#endif // _EPL_COMPATIBILITY

	if (nossl) flag |= INITFLAG_NOSSL;
	if (cacheStorage) flag |= INITFLAG_CACHESTORAGE;

	ret = EcKeInitialize(hInstance, flag, local, cache_path, 0);

#ifdef _EPL_COMPATIBILITY
	if (Chrome_IsUIThread()) {
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = DefWindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"EasyCKL_Tools";

		if (!RegisterClass(&wc))
		{
			MessageBox(NULL, TEXT("Message Window Class Failed!"), TEXT("Error"), MB_ICONERROR);
			return 0;
		}

		nid.cbSize = NOTIFYICONDATA_V3_SIZE; //(DWORD)sizeof(NOTIFYICONDATA);
		nid.hWnd = CreateWindow(L"EasyCKL_Tools", L"", 0, 0, 0, 0, 0, 0, 0, hInstance, 0);
		nid.uID = 1;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
		nid.uCallbackMessage = WM_USER + 1;//自定义消息
		nid.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
		wcscpy_s(nid.szTip, TEXT("EasyCKL EPL 调试兼容模式"));
		wcscpy_s(nid.szInfo, TEXT("EasyCKL EPL开发人员工具"));
		wcscpy_s(nid.szInfoTitle, TEXT("EasyCKL 正在 EPL 调试兼容模式下工作。发布你的应用时请使用标准 EasyCKL 库"));
		nid.dwInfoFlags = NIIF_WARNING | NIIF_LARGE_ICON;
		nid.uTimeout = 10000;
		Shell_NotifyIcon(NIM_ADD, &nid);
	}
#endif // _EPL_COMPATIBILITY
	return ret;
}

CKLEXPORT void WINAPI Chrome_Initialize(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage) {
	Chrome_InitializeEx(hInstance, nossl, cacheStorage, 0, 0);
}

typedef struct tagBrowserCallBacks {
	SIZE_T size;
	Chrome_CallBack_BrowserCreated created_callback;
	Chrome_CallBack_ChUrl churl_callback;
	Chrome_CallBack_NewWindow newwindow;
	Chrome_CallBack_Download download;
	Chrome_CallBack_ChState chstate;
	Chrome_CallBack_JSDialog JSDialog;
	Chrome_CallBack_Error error;
	Chrome_CallBack_RButtonDown rbuttondown;
	Chrome_CallBack_ChTitle chtitle;
	Chrome_CallBack_CanLoadUrl canloadurl;
}BrowserCallBacks, *PBrowserCallBacks;

CKLEXPORT void* WINAPI EcKeCreateChildBrowser(DWORD flags, BrowserCallBacks* callbacks, wchar_t* referer, DWORD id, wchar_t* url, HWND hParent, RECT* rect, void* rev) {
	if (callbacks->size < sizeof(BrowserCallBacks))
		return 0;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, callbacks->created_callback, callbacks->churl_callback, callbacks->newwindow,
		callbacks->download, callbacks->chstate, callbacks->JSDialog, callbacks->error, callbacks->rbuttondown, callbacks->chtitle, callbacks->canloadurl));

	if (referer)
		handler->referer_string = std::wstring(referer);
	return handler->_CreateBrowserSync(std::wstring(url), hParent, *rect);
}

CKLEXPORT void* WINAPI Chrome_CreateBrowserSyncWithJavaScriptReferer(wchar_t* referer, DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	if (!std::wstring(url).substr(0, 6).compare(L"chrome")) return 0;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow,
		download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl));

	if (referer)
		handler->referer_string = std::wstring(referer);

	return handler->_CreateBrowserWithJSReferer(std::wstring(url), hParent, *rect);
}

CKLEXPORT void* WINAPI Chrome_CreateBrowserSyncWithReferer(wchar_t* referer, DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	if (!std::wstring(url).substr(0, 6).compare(L"chrome")) return 0;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow,
		download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl));

	if (referer)
		handler->referer_string = std::wstring(referer);
	return handler->_CreateBrowserSync(std::wstring(url), hParent, *rect);
}

CKLEXPORT void WINAPI Chrome_CreateBrowserExWithReferer(wchar_t* referer, DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	if (!std::wstring(url).substr(0, 6).compare(L"chrome")) return;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow,
		download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl));

	if (referer)
		handler->referer_string = std::wstring(referer);
	handler->_CreateBrowser(std::wstring(url), hParent, *rect);
}

CKLEXPORT void* WINAPI Chrome_CreateBrowserSync(DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	return Chrome_CreateBrowserSyncWithReferer(0, id, url, hParent, rect, created_callback, churl_callback,
		newwindow, download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl, rev);
}

CKLEXPORT void WINAPI Chrome_CreateBrowserEx(DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	Chrome_CreateBrowserExWithReferer(0, id, url, hParent, rect, created_callback, churl_callback,
		newwindow, download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl, rev);
}

CKLEXPORT void WINAPI Chrome_CreateBrowser(DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown) {
	Chrome_CreateBrowserEx(id, url, hParent, rect, created_callback, churl_callback, newwindow, download, chstate, JSDialog, error, rbuttondown, 0, 0, 0);
}

CKLEXPORT void WINAPI Chrome_CreateSimple(wchar_t* url, HWND hParent, RECT* rect, Chrome_CallBack_BrowserCreated created_callback) {
	Chrome_CreateBrowserEx(0, url, hParent, rect, created_callback, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

CKLEXPORT void WINAPI Chrome_DoMessageLoopWork() {
	CefDoMessageLoopWork();
}

CKLEXPORT void WINAPI Chrome_MessageLoop() {
	CefRunMessageLoop();
}

CKLEXPORT void WINAPI Chrome_Shutdown() {
#ifdef _EPL_COMPATIBILITY
	Shell_NotifyIcon(NIM_DELETE, &nid);
#endif
	CefShutdown();
}

CKLEXPORT void WINAPI Chrome_SetOSModalLoop(bool osModalLoop) {
	CefSetOSModalLoop(osModalLoop);
}

CKLEXPORT void WINAPI Chrome_EnableSystemFlash() {
	flash = TRUE;
}

CKLEXPORT void WINAPI Chrome_SetUserAgent(wchar_t* _ua) {
	isSetUA = TRUE;
	ua = _ua;
}

CKLEXPORT void WINAPI Chrome_SetProxyServer(wchar_t* proxy) {
	isSetProxy = TRUE;
	proxyserver = proxy;
}

CKLEXPORT void WINAPI Chrome_LoadUrl(SimpleHandler* handler, wchar_t* url) {
	if (!std::wstring(url).substr(0, 6).compare(L"chrome")) return;
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GetMainFrame()->LoadURL(url);
		}
	}
}

CKLEXPORT DWORD WINAPI Chrome_GetUrlLength(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			return browser->GetMainFrame()->GetURL().length();
		}
	}
	return 0;
}

CKLEXPORT void WINAPI Chrome_GetUrlString(SimpleHandler* handler, wchar_t* buffer, DWORD buffer_length) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			_ECKL_CopyWString(browser->GetMainFrame()->GetURL().ToWString(), buffer, buffer_length * sizeof(wchar_t));
		}
	}
}

CKLEXPORT HWND WINAPI Chrome_GetWindowHandle(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			return browser->GetHost()->GetWindowHandle();
		}
	}
	return nullptr;
}

CKLEXPORT HWND WINAPI Chrome_Window(SimpleHandler* handler) {
	return Chrome_GetWindowHandle(handler);
}

CKLEXPORT void WINAPI Chrome_GoBack(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GoBack();
		}
	}
}

CKLEXPORT void WINAPI Chrome_GoForward(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GoForward();
		}
	}
}

CKLEXPORT void WINAPI Chrome_Refresh(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->Reload();
		}
	}
}

CKLEXPORT void WINAPI Chrome_Stop(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->StopLoad();
		}
	}
}

CKLEXPORT void WINAPI Chrome_ExecJS(SimpleHandler* handler, wchar_t* js) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GetMainFrame()->ExecuteJavaScript(js, browser->GetMainFrame()->GetURL(), 0);
		}
	}
}

CKLEXPORT void WINAPI EcKeCookieStorageControl(BOOL enable, wchar_t* CookiePath, bool persist_session_cookies) {
	if (enable) {
		CefRefPtr<CefCookieManager> cookiemgr = CefCookieManager::GetGlobalManager(NULL);
		if (!CookiePath)
			cookiemgr->SetStoragePath(".\\cookies\\", persist_session_cookies, NULL);
		else cookiemgr->SetStoragePath(CookiePath, persist_session_cookies, NULL);
	}
	else {
		CefRefPtr<CefCookieManager> cookiemgr = CefCookieManager::GetGlobalManager(NULL);
		cookiemgr->SetStoragePath("", false, NULL);
	}
}

CKLEXPORT void WINAPI Chrome_EnableCookieStorageEx(wchar_t* CookiePath) {
	EcKeCookieStorageControl(TRUE, CookiePath, false);
}

CKLEXPORT void WINAPI Chrome_EnableCookieStorage() {
	EcKeCookieStorageControl(TRUE, 0, false);
}

CKLEXPORT void WINAPI Chrome_DisableCookieStorage() {
	EcKeCookieStorageControl(FALSE, 0, false);
}

CKLEXPORT void WINAPI Chrome_Close(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			ShowWindow(Chrome_Window(handler), 0);
			SetParent(Chrome_Window(handler), 0);
			browser->GetHost()->CloseBrowser(true);
		}
	}
}

CKLEXPORT void WINAPI Chrome_LoadString(SimpleHandler* handler, wchar_t* string, wchar_t* url) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			CefString str(string);
			if (!url)
				browser->GetMainFrame()->LoadString(string, browser->GetMainFrame()->GetURL());
			else browser->GetMainFrame()->LoadString(string, url);
		}
	}
}

CKLEXPORT void WINAPI Chrome_SetV8ContextCallback(Chrome_CallBack_V8 contextcreate, V8Handler_CallBack handler) {
	myV8handle = new MyV8Handler(handler);
	v8contextcreate = contextcreate;
}

CKLEXPORT void WINAPI Chrome_ShowDevTools(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			CefWindowInfo dt_wininfo;
			dt_wininfo.SetAsPopup(0, L"Developer Tools");
			CefBrowserSettings browser_settings;
			CefPoint point;
			browser->GetHost()->ShowDevTools(dt_wininfo, handler, browser_settings, point);
		}
	}
}

CKLEXPORT void WINAPI Chrome_SetUserDataLongPtr(SimpleHandler* handler, LONG_PTR data) {
	if (handler) {
		handler->userData = data;
	}
}

#define EC_STRING CefString*

CKLEXPORT void WINAPI Chrome_GetHtmlSource(SimpleHandler* handler, LONG_PTR lpBuffer) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {

			class Visitor : public CefStringVisitor {
			public:
				explicit Visitor(CefRefPtr<CefBrowser> browser, LONG_PTR Buffer) : browser_(browser), Buffer_(Buffer) {}
				virtual void Visit(const CefString& string) OVERRIDE {
					*((const EC_STRING*)Buffer_) = &string;
					//MessageBox(0, string.c_str(), 0, 0);
					PostQuitMessage(0);
				}
			private:
				CefRefPtr<CefBrowser> browser_;
				LONG_PTR Buffer_;
				IMPLEMENT_REFCOUNTING(Visitor);
			};


			CefRefPtr<Visitor> a = new Visitor(browser, lpBuffer);
			//CefString t_cefstring;
			browser->GetMainFrame()->GetSource(a);

			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				CefDoMessageLoopWork();
			}
		}
	}
}

CKLEXPORT void WINAPI EcDebugShowStringValue(EC_STRING string, BOOL ShowAsMsgBox) {
	if (!string) return;
	CefString str = *string;
	if (!ShowAsMsgBox) {
		OutputDebugStringW(str.c_str());
		OutputDebugStringW(L"\r\n");
	}
	else MessageBoxW(0, str.c_str(), L"Debug", MB_ICONINFORMATION);
}