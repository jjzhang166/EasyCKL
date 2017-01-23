#include "simple_app.h"
#include "simple_handler.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_app.h"

typedef void(WINAPI * Chrome_CallBack_V8)(CefV8Context *context);

#ifdef _WIN32
extern HANDLE hEvent;
#endif
extern void* v8contextcreate;

extern BOOL bSetProxy;
extern CefString szProxyServer;
extern BOOL bEnableFlash;
extern CefString szFlashPath;
extern BOOL bDisableGpu;

SimpleApp::SimpleApp() {
}

void SimpleApp::OnContextInitialized() {
	CEF_REQUIRE_UI_THREAD();
#ifdef _WIN32
	SetEvent(hEvent);
#endif
}


void SimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
	if (v8contextcreate) {
		((Chrome_CallBack_V8)v8contextcreate)(context.get());
	}
}

void SimpleApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) {
	if (bEnableFlash) {
		if (!szFlashPath.empty())
			command_line->AppendSwitchWithValue(L"ppapi-flash-path", szFlashPath);
		else command_line->AppendSwitch(L"enable-system-flash");
	}
	if (bSetProxy)
		command_line->AppendSwitchWithValue(L"proxy-server", szProxyServer);
	if(bDisableGpu)
		command_line->AppendSwitchWithValue(L"disable-gpu", szProxyServer);
}
