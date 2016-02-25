#include <Windows.h>

#include "simple_app.h"
#include "include/cef_sandbox_win.h"

#include "callbacks.h"
#include "simple_handler.h"

#ifdef _DEBUG
#pragma comment(lib, "libcef_d.lib")
#else
#pragma comment(lib, "libcef.lib")
#endif // _DEBUG

#pragma comment(lib, "libcef_dll_wrapper.lib")

#pragma comment(lib, "user32.lib")

#define CKLEXPORT extern "C" __declspec(dllexport)

typedef BOOL(WINAPI * V8Handler_CallBack)(const wchar_t* name, const void* argu);
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
			return handler_callback(name.c_str(), &arguments) != FALSE;
		}
		return false;
	}

	// Provide the reference counting implementation for this class.
	IMPLEMENT_REFCOUNTING(MyV8Handler);
};

extern HANDLE hEvent = 0;
void* v8contextcreate = 0;

extern BOOL isSetUA = FALSE;
extern CefString useragen("");

extern BOOL isSetProxy = FALSE;
extern CefString proxyserver("");

extern CefString localinf("");

CefRefPtr<CefV8Handler> myV8handle;

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

CKLEXPORT int WINAPI Chrome_InitializeEx(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage, wchar_t* local) {
	SetUnhandledExceptionFilter(excpcallback);
	hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	CefMainArgs main_args(hInstance);
	CefRefPtr<SimpleApp> app(new SimpleApp);

	int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
	if (exit_code >= 0) {
		return exit_code;
	}

	CefSettings settings;
#ifdef _DEBUG
	settings.log_severity = LOGSEVERITY_INFO;
#else
	settings.log_severity = LOGSEVERITY_DISABLE;
#endif // _DEBUG
	if (cacheStorage) {
		CefString(&settings.cache_path) = L".\\cache\\";
	}

	//settings.single_process = true;
	settings.command_line_args_disabled = true;
	//settings.multi_threaded_message_loop = true;
	settings.ignore_certificate_errors = nossl;
	if (local) {
		CefString(&settings.locale) = local;
		localinf = local;
	}

	settings.no_sandbox = true;

	CefInitialize(main_args, settings, app.get(), nullptr);

	WaitForSingleObject(hEvent, INFINITE);

	if (Chrome_IsUIThread()) 
		SetUnhandledExceptionFilter(0);
		
	return -1;
}

CKLEXPORT void WINAPI Chrome_Initialize(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage) {
	Chrome_InitializeEx(hInstance, nossl, cacheStorage, 0);
}

CKLEXPORT void* WINAPI Chrome_CreateBrowserSync(DWORD id, char* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {
	if (!std::string(url).substr(0, 6).compare("chrome")) return 0;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow,
		download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl));
	return handler->_CreateBrowserSync(std::string(url), hParent, *rect);
}

CKLEXPORT void WINAPI Chrome_CreateBrowserEx(DWORD id, char* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	if (!std::string(url).substr(0, 6).compare("chrome")) return;
	//if (!Chrome_IsUIThread()) return;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow,
		download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl));
	handler->_CreateBrowser(std::string(url), hParent, *rect);
}

CKLEXPORT void WINAPI Chrome_CreateBrowser(DWORD id, char* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown) {
	Chrome_CreateBrowserEx(0, url, hParent, rect, created_callback, churl_callback, newwindow, download, chstate, JSDialog, error, rbuttondown, 0, 0, 0);
}

CKLEXPORT void WINAPI Chrome_CreateSimple(char* url, HWND hParent, RECT* rect, Chrome_CallBack_BrowserCreated created_callback) {
	Chrome_CreateBrowserEx(0, url, hParent, rect, created_callback, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

CKLEXPORT void* WINAPI Chrome_CreateBrowserSyncWithReferer(wchar_t* referer, DWORD id, char* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	if (!std::string(url).substr(0, 6).compare("chrome")) return 0;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow,
		download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl));
	handler->referer_string = std::wstring(referer);
	return handler->_CreateBrowserSync(std::string(url), hParent, *rect);
}

CKLEXPORT void WINAPI Chrome_CreateBrowserExWithReferer(wchar_t* referer, DWORD id, char* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	if (!std::string(url).substr(0, 6).compare("chrome")) return;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow,
		download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl));
	handler->referer_string = std::wstring(referer);
	handler->_CreateBrowser(std::string(url), hParent, *rect);
}

//CKLEXPORT void WINAPI ECKL_MessageLoop() {
//	MSG msg;
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//		CefDoMessageLoopWork();
//	}
//}

CKLEXPORT void WINAPI Chrome_MessageLoop() {
	CefRunMessageLoop();
}

CKLEXPORT void WINAPI Chrome_Shutdown() {
	CefShutdown();
}

CKLEXPORT void WINAPI Chrome_SetUserAgent(wchar_t* ua) {
	isSetUA = TRUE;
	useragen = ua;
}

CKLEXPORT void WINAPI Chrome_SetProxyServer(wchar_t* proxy) {
	isSetProxy = TRUE;
	proxyserver = proxy;
}

CKLEXPORT void WINAPI Chrome_LoadUrl(SimpleHandler* handler, char* url) {
	if (!std::string(url).substr(0, 6).compare("chrome")) return;
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GetMainFrame()->LoadURL(url);
		}
	}
}

CKLEXPORT DWORD WINAPI Chrome_GetUrlLength_UTF16(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			return browser->GetMainFrame()->GetURL().length();
		}
	}
	return 0;
}

CKLEXPORT void WINAPI Chrome_GetUrlString_UTF16(SimpleHandler* handler, wchar_t* buffer, DWORD buffer_length) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GetMainFrame()->GetURL().ToWString().copy(buffer, buffer_length);
			memset(buffer + buffer_length - 2, 0, 2);//最后两个字节置0
		}
	}
}

CKLEXPORT HWND WINAPI Chrome_Window(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			return browser->GetHost()->GetWindowHandle();
		}
	}
	return nullptr;
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

CKLEXPORT void WINAPI Chrome_EnableCookieStorageEx(wchar_t* CookiePath) {
	CefRefPtr<CefCookieManager> cookiemgr = CefCookieManager::GetGlobalManager();
	if (!CookiePath)
		cookiemgr->SetStoragePath(".\\cookies\\", false);
	else cookiemgr->SetStoragePath(CookiePath, false);
}

CKLEXPORT void WINAPI Chrome_EnableCookieStorage() {
	Chrome_EnableCookieStorageEx(0);
}

CKLEXPORT void WINAPI Chrome_DisableCookieStorage() {
	CefRefPtr<CefCookieManager> cookiemgr = CefCookieManager::GetGlobalManager();
	cookiemgr->SetStoragePath("", false);
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

/*CKLEXPORT void WINAPI Chrome_LoadString(SimpleHandler* handler, char* string) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			CefString str(string);
			browser->GetMainFrame()->LoadString(string, browser->GetMainFrame()->GetURL());
		}
	}
}*/

CKLEXPORT void WINAPI Chrome_SetV8ContextCallback(Chrome_CallBack_V8 contextcreate, V8Handler_CallBack handler) {
	myV8handle = new MyV8Handler(handler);
	v8contextcreate = contextcreate;
}

CKLEXPORT void WINAPI Chrome_AddJSFunction(CefV8Context* context, wchar_t* name) {
	auto myFun = CefV8Value::CreateFunction(name, myV8handle);
	auto pObjApp = context->GetGlobal();
	pObjApp->SetValue(name, myFun, V8_PROPERTY_ATTRIBUTE_NONE);
}

CKLEXPORT DWORD WINAPI Chrome_GetV8ValueListSize(const CefV8ValueList* arguments) {
	return arguments->size();
}

CKLEXPORT DWORD WINAPI Chrome_GetV8ValueInt(const CefV8ValueList* arguments, size_t pos) {
	auto value = arguments->at(pos);
	if (value->IsValid())
		return value->GetUIntValue();
	return 0;
}

CKLEXPORT DWORD WINAPI Chrome_GetV8ValueStringLength(const CefV8ValueList* arguments, size_t pos) {
	auto value = arguments->at(pos);
	if (value->IsValid())
		return std::wstring(value->GetStringValue()).length();
	return 0;
}

CKLEXPORT void WINAPI Chrome_GetV8ValueString(const CefV8ValueList* arguments, size_t pos, wchar_t* buffer, size_t buffer_length) {
	auto value = arguments->at(pos);
	if (value->IsValid()) {
		std::wstring(value->GetStringValue()).copy(buffer, buffer_length);
		memset(buffer + buffer_length - 2, 0, 2);//最后两个字节置0
	}
	/*const wchar_t* a = s.c_str();
	DWORD leng = (wcslen(a) + 1)*sizeof(wchar_t);//获取字节数
	if(leng <= buffer_length)
		memcpy(buffer, a, leng);
	else {
		memcpy(buffer, a, buffer_length - 2);
		memset(buffer + buffer_length - 2, 0, 2);//最后两个字节置0
	}*/
}