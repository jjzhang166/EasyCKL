#include "simple_app.h"
#include "CKLMain.h"

#ifdef _WIN32
extern HANDLE hEvent;
#endif
extern Chrome_CallBack_V8 v8contextcreate;
extern Chrome_CallBack_AddCmdline addCmdlineFunc;

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
		v8contextcreate(context.get());
	}
}

void SimpleApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) {
	if (bEnableFlash) {
		if (!szFlashPath.empty())
			command_line->AppendSwitchWithValue(L"ppapi-flash-path", szFlashPath);
		else command_line->AppendSwitch(L"enable-system-flash");
	}
	if (!szProxyServer.empty())
		command_line->AppendSwitchWithValue(L"proxy-server", szProxyServer);
	if(bDisableGpu)
		command_line->AppendSwitchWithValue(L"disable-gpu", szProxyServer);

	if (addCmdlineFunc)
		addCmdlineFunc(&command_line);
}
