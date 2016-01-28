#include<Windows.h>

#include "cefsimple/simple_app.h"
#include "include/cef_sandbox_win.h"

#include"callbacks.h"
#include "cefsimple/simple_handler.h"
// Set to 0 to disable sandbox support.
#define CEF_ENABLE_SANDBOX 0

class MyV8Handler : public CefV8Handler {
	public:
		typedef bool(WINAPI * V8Handler_CallBack)(const char* name);
		MyV8Handler(void* handler) {
			handler_callback = handler;
		}
		~MyV8Handler(){}
		void* handler_callback=0;
		virtual bool Execute(const CefString& name,
			CefRefPtr<CefV8Value> object,
			const CefV8ValueList& arguments,
			CefRefPtr<CefV8Value>& retval,
			CefString& exception) OVERRIDE{

			if (handler_callback){
				V8Handler_CallBack a = (V8Handler_CallBack)handler_callback;
				return a(name.ToString().c_str());
			}
			return false;
		}
	
			// Provide the reference counting implementation for this class.
		IMPLEMENT_REFCOUNTING(MyV8Handler);
	};

extern HANDLE hEvent=0;
extern void* v8contextcreate = 0;

CefRefPtr<CefV8Handler> myV8handle;

#if CEF_ENABLE_SANDBOX
// The cef_sandbox.lib static library is currently built with VS2010. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

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

long WINAPI excpcallback(_EXCEPTION_POINTERS* excp)
{
	return EXCEPTION_EXECUTE_HANDLER;
}

extern "C" bool WINAPI Chrome_IsUIThread(){
	return CefCurrentlyOn(TID_UI);
}

extern "C" int WINAPI Chrome_InitializeEx(HINSTANCE hInstance, BOOL nossl, BOOL cacheStorage){
	SetUnhandledExceptionFilter(excpcallback);
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	void* sandbox_info = NULL;

#if CEF_ENABLE_SANDBOX
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(hInstance);

	// SimpleApp implements application-level callbacks. It will create the first
	// browser instance in OnContextInitialized() after CEF has initialized.
	//CefRefPtr<SimpleApp> app(new SimpleApp(std::string(url), hParent, rect));
	CefRefPtr<SimpleApp> app(new SimpleApp);

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, app.get(), sandbox_info);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.
		return exit_code;
	}

	// Specify CEF global settings here.
	CefSettings settings;
	settings.log_severity = LOGSEVERITY_DISABLE;
	if (cacheStorage){
		cef_string_t cache = { 0 };
		cache.dtor = 0;
		cache.str = L".\\cache\\";
		cache.length = lstrlenW(L".\\cache\\");
		settings.cache_path = cache;
	}

	settings.command_line_args_disabled = true;
	//settings.multi_threaded_message_loop = true;
	if (nossl){
		settings.ignore_certificate_errors = true;
	}

#if !CEF_ENABLE_SANDBOX
	settings.no_sandbox = true;
#endif

	// Initialize CEF.
	CefInitialize(main_args, settings, app.get(), sandbox_info);

	WaitForSingleObject(hEvent, INFINITE);
	//WaitForSingleObject(hEvent2, INFINITE);

	if (Chrome_IsUIThread())
		SetUnhandledExceptionFilter(0);

	return -1;
}

extern "C" void WINAPI Chrome_CreateBrowser(DWORD id, char* url, HWND hParent, RECT* rect, void* created_callback, void* churl_callback, void* newwindow, void* download, void* chstate, void* JSDialog, void* error, void* rbuttondown){

	if (!std::string(url).substr(0, 6).compare("chrome"))return;
	if (!Chrome_IsUIThread())return;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id,created_callback, churl_callback, newwindow, download, chstate, JSDialog, error, rbuttondown));
	handler->_CreateBrowser(std::string(url), hParent, rect);
}

extern "C" void WINAPI Chrome_CreateEx(char* url, HWND hParent, RECT* rect, void* created_callback, void* churl_callback, void* newwindow, void* download, void* chstate, void* JSDialog){
	Chrome_CreateBrowser(0,url, hParent, rect, created_callback, churl_callback, newwindow, download, chstate, JSDialog, 0, 0);
}

extern "C" void WINAPI Chrome_MessageLoop(){
	CefRunMessageLoop();
}

extern "C" void WINAPI Chrome_Shutdown(){
	CefShutdown();
}

extern "C" void WINAPI Chrome_LoadUrl(void* point, char* url){
	if (!std::string(url).substr(0, 6).compare("chrome"))return;
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			browser->GetMainFrame()->LoadURL(url);
		}
	}
}
extern "C" HWND WINAPI Chrome_Window(void* point){
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			return browser->GetHost()->GetWindowHandle();
		}
	}
	return NULL;
}

extern "C" void WINAPI Chrome_GoBack(void* point){
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			browser->GoBack();
		}
	}
}

extern "C" void WINAPI Chrome_GoForward(void* point){
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			browser->GoForward();
		}
	}
}

extern "C" void WINAPI Chrome_Refresh(void* point){
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			browser->Reload();
		}
	}
}

extern "C" void WINAPI Chrome_Stop(void* point){
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			browser->StopLoad();
		}
	}
}

extern "C" void WINAPI Chrome_ExecJS(void* point, char* js){
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		/*CefString _string(js);
		_string.ToString();*/
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			browser->GetMainFrame()->ExecuteJavaScript(js, browser->GetMainFrame()->GetURL(), 0);
		}
	}
}

extern "C" void WINAPI Chrome_EnableCookieStorage(){
	CefRefPtr<CefCookieManager> cookiemgr = CefCookieManager::GetGlobalManager();
	//cookiemgr = CefCookieManager::CreateManager(path, force_persist_session_cookies);
	cookiemgr->SetStoragePath(L".\\cookies\\", false);

}


extern "C" void WINAPI Chrome_Close(void* point){
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			browser->GetHost()->CloseBrowser(true);
			//delete a;
		}
	}
}

extern "C" void WINAPI Chrome_LoadString(void* point, char* string){
	SimpleHandler* a = (SimpleHandler*)point;
	if (a != NULL){
		CefRefPtr<CefBrowser> browser = a->g_browser;
		if (browser.get()){
			CefString str(string);
			browser->GetMainFrame()->LoadString(string, browser->GetMainFrame()->GetURL());
		}
	}
}

extern "C" void WINAPI Chrome_SetV8ContextCallback(void* contextcreate,void* handler){
	myV8handle = new MyV8Handler(handler);
	v8contextcreate = contextcreate;
}

extern "C" void WINAPI Chrome_AddJSFunction(void* _context,char* name){
	CefRefPtr<CefV8Context> context = *(CefRefPtr<CefV8Context>*)_context;
	CefRefPtr<CefV8Value> myFun = CefV8Value::CreateFunction(name, myV8handle);
	CefRefPtr<CefV8Value> pObjApp = context->GetGlobal();
	pObjApp->SetValue(name, myFun, V8_PROPERTY_ATTRIBUTE_NONE);
}