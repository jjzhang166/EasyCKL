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

#define CKLEXPORT extern "C" __declspec(dllexport)

typedef BOOL(WINAPI * V8Handler_CallBack)(const char* name, const void* argu);

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
			return handler_callback(name.ToString().c_str(), &arguments) != FALSE;
		}
		return false;
	}

	// Provide the reference counting implementation for this class.
	IMPLEMENT_REFCOUNTING(MyV8Handler);
};

extern HANDLE hEvent = 0;
extern void* v8contextcreate = 0;

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

CKLEXPORT int WINAPI Chrome_InitializeEx(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage) {
	SetUnhandledExceptionFilter(excpcallback);
	hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(hInstance);

	// SimpleApp implements application-level callbacks. It will create the first
	// browser instance in OnContextInitialized() after CEF has initialized.
	//CefRefPtr<SimpleApp> app(new SimpleApp(std::string(url), hParent, rect));
	CefRefPtr<SimpleApp> app(new SimpleApp);

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.
		return exit_code;
	}

	// Specify CEF global settings here.
	CefSettings settings;
#ifdef _DEBUG
	settings.log_severity = LOGSEVERITY_INFO;
#else
	settings.log_severity = LOGSEVERITY_DISABLE;
#endif // _DEBUG
	if (cacheStorage) {
		CefString(&settings.cache_path) = ".\\cache\\";
	}

	settings.command_line_args_disabled = true;
	//settings.multi_threaded_message_loop = true;
	settings.ignore_certificate_errors = nossl;

	settings.no_sandbox = true;

	// Initialize CEF.
	CefInitialize(main_args, settings, app.get(), nullptr);

	WaitForSingleObject(hEvent, INFINITE);

	if (Chrome_IsUIThread())
		SetUnhandledExceptionFilter(0);

	return -1;
}

CKLEXPORT void WINAPI Chrome_CreateBrowser(DWORD id, char* url, HWND hParent, RECT* rect, Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback, Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate, Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown) {

	if (!std::string(url).substr(0, 6).compare("chrome")) return;
	if (!Chrome_IsUIThread()) return;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow, download, chstate, JSDialog, error, rbuttondown));
	handler->_CreateBrowser(std::string(url), hParent, *rect);
}

CKLEXPORT void WINAPI Chrome_CreateSimple(char* url, HWND hParent, RECT* rect, Chrome_CallBack_BrowserCreated created_callback) {
	Chrome_CreateBrowser(0, url, hParent, rect, created_callback, 0, 0, 0, 0, 0, 0, 0);
}

CKLEXPORT void WINAPI Chrome_MessageLoop() {
	CefRunMessageLoop();
}

CKLEXPORT void WINAPI Chrome_Shutdown() {
	CefShutdown();
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

CKLEXPORT void WINAPI Chrome_ExecJS(SimpleHandler* handler, char* js) {
	if (handler) {
		/*CefString _string(js);
		_string.ToString();*/
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GetMainFrame()->ExecuteJavaScript(js, browser->GetMainFrame()->GetURL(), 0);
		}
	}
}

CKLEXPORT void WINAPI Chrome_EnableCookieStorageEx(char* CookiePath) {
	CefRefPtr<CefCookieManager> cookiemgr = CefCookieManager::GetGlobalManager();
	if (!CookiePath)
		cookiemgr->SetStoragePath(".\\cookies\\", false);
	else cookiemgr->SetStoragePath(CookiePath, false);
}

CKLEXPORT void WINAPI Chrome_EnableCookieStorage() {
	Chrome_EnableCookieStorageEx(0);
}

CKLEXPORT void WINAPI Chrome_Close(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GetHost()->CloseBrowser(true);
			//browser->Release();
		}
	}
}

CKLEXPORT void WINAPI Chrome_LoadString(SimpleHandler* handler, char* string) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			CefString str(string);
			browser->GetMainFrame()->LoadString(string, browser->GetMainFrame()->GetURL());
		}
	}
}

CKLEXPORT void WINAPI Chrome_SetV8ContextCallback(void* contextcreate, V8Handler_CallBack handler) {
	myV8handle = new MyV8Handler(handler);
	v8contextcreate = contextcreate;
}

CKLEXPORT void WINAPI Chrome_AddJSFunction(void* _context, char* name) {
	CefRefPtr<CefV8Context> context = *(CefRefPtr<CefV8Context>*)_context;
	CefRefPtr<CefV8Value> myFun = CefV8Value::CreateFunction(name, myV8handle);
	CefRefPtr<CefV8Value> pObjApp = context->GetGlobal();
	pObjApp->SetValue(name, myFun, V8_PROPERTY_ATTRIBUTE_NONE);
}

CKLEXPORT DWORD WINAPI Chrome_GetV8ValueListSize(void* list) {
	const CefV8ValueList* arguments = (const CefV8ValueList*)list;
	return arguments->size();
}

CKLEXPORT DWORD WINAPI Chrome_GetV8ValueInt(void* list, size_t pos) {
	const CefV8ValueList* arguments = (const CefV8ValueList*)list;
	const CefRefPtr<CefV8Value> value = arguments->at(pos);
	return value->GetIntValue();
}

CKLEXPORT void WINAPI Chrome_GetV8ValueString(void* list, size_t pos, char* buffer, size_t buffer_length) {
	const CefV8ValueList* arguments = (const CefV8ValueList*)list;
	const CefRefPtr<CefV8Value> value = arguments->at(pos);
	CefString s = value->GetStringValue();
	const char* a = s.ToString().c_str();
	memcpy(buffer, a, buffer_length);
}