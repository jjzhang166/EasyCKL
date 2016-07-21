#include "CKLMain.h"

#ifdef _DEBUG
#pragma comment(lib, "libcef_d.lib")
#else
#pragma comment(lib, "libcef.lib")
#endif // _DEBUG

#pragma comment(lib, "libcef_dll_wrapper.lib")
#pragma comment(lib, "user32.lib")

#define _EPL_COMPATIBILITY

#ifdef _EPL_COMPATIBILITY
#include<Shellapi.h>
#pragma comment(lib, "Shell32.lib")
NOTIFYICONDATA nid = { 0 };
#endif // _EPL_COMPATIBILITY

extern HANDLE hEvent = 0;
void* v8contextcreate = 0;

BOOL isSetUA = FALSE;
CefString ua;
extern CefString szLocalInf("");

extern BOOL bSetProxy = FALSE;
extern CefString szProxyServer("");
extern BOOL bEnableFlash = FALSE;
extern CefString szFlashPath("");
extern BOOL bDisableGpu = FALSE;

CefRefPtr<CefV8Handler> myV8handle;

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
	LPVOID lpReserved)
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

CKLEXPORT int WINAPI EcKeInitialize(HINSTANCE hInstance, DWORD flag, wchar_t* local, wchar_t* cache_path, LPINIT_EXTDATA extData) {
	SetUnhandledExceptionFilter(excpcallback);
	hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	if (flag & INITFLAG_ENABLEHIGHDPISUPPORT)
		CefEnableHighDPISupport();

	if (flag & INITFLAG_DISABLEGPU)
		bDisableGpu = TRUE;

	CefMainArgs main_args(hInstance);
	CefRefPtr<SimpleApp> app(new SimpleApp);

	int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
	if (exit_code >= 0) {
		return exit_code;
	}

	CefSettings settings;
	settings.command_line_args_disabled = true;
	settings.no_sandbox = true;

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

	if (flag & INITFLAG_EXTDATA) {
		if (flag & INITFLAG_SETUSERAGENT) {
			CefString(&settings.user_agent) = extData->szUserAgent;
		}
	}
	else if (isSetUA)
		CefString(&settings.user_agent) = ua;

	if (local) {
		CefString(&settings.locale) = local;
		szLocalInf = local;
	}

	CefInitialize(main_args, settings, app.get(), nullptr);
	WaitForSingleObject(hEvent, INFINITE);

	if (Chrome_IsUIThread()) {
		SetUnhandledExceptionFilter(0);
	}
	return -1;
}

CKLEXPORT int WINAPI Chrome_InitializeEx(HINSTANCE hInstance, DWORD dwFlags, LPINIT_EXTDATA lpExtData, wchar_t* szLocal, wchar_t* szCachePath) {
	DWORD _flag = 0;
	int ret = 0;

	/* 兼容旧版本的 Chrome_InitializeEx */
	if (!(dwFlags & INITFLAG_EXTDATA) && lpExtData) {
		_flag |= INITFLAG_CACHESTORAGE;
		if (dwFlags) _flag |= INITFLAG_NOSSL;
	}
	else _flag = dwFlags &~INITFLAG_USECOMPATIBILITY;

#ifdef _EPL_COMPATIBILITY
	if (!IsDebuggerPresent()) {
		ExitProcess(-1);
	}
	_flag |= INITFLAG_USECOMPATIBILITY;
#endif // _EPL_COMPATIBILITY

	ret = EcKeInitialize(hInstance, _flag, szLocal, szCachePath, lpExtData);

#ifdef _EPL_COMPATIBILITY
	if (Chrome_IsUIThread() && GetFileAttributes(L".\\disable-epl-debug-compatibility-warning") == INVALID_FILE_ATTRIBUTES) {
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
	DWORD flag = 0;
	if (nossl) flag |= INITFLAG_NOSSL;
	if (cacheStorage) flag |= INITFLAG_CACHESTORAGE;
	Chrome_InitializeEx(hInstance, flag, 0, 0, 0);
}

CKLEXPORT void* WINAPI Chrome_CreateChildBrowser(DWORD dwFlags, LPBROWSER_CALLBACKS lpCallbacks, DWORD id, wchar_t* szHeaderReferer, wchar_t* szUrl, HWND hParent, RECT* rcBrowserRect, LPCREATE_BROWSER_EXTDATA lpExtData) {

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, lpCallbacks));
	handler->flags = dwFlags;

	if ((dwFlags & BROWSERFLAG_HEADER_REFERER) && szHeaderReferer)
		handler->referer_string = std::wstring(szHeaderReferer);
	if (dwFlags & BROWSERFLAG_SYNC)
		return handler->_CreateBrowserSync(std::wstring(szUrl), hParent, *rcBrowserRect, lpExtData);
	else handler->_CreateBrowser(std::wstring(szUrl), hParent, *rcBrowserRect, lpExtData);
	return 0;
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
	return handler->_CreateBrowserSync(std::wstring(url), hParent, *rect, 0);
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
	handler->_CreateBrowser(std::wstring(url), hParent, *rect, 0);
}

CKLEXPORT void* WINAPI Chrome_CreateBrowserSync(DWORD id, wchar_t* url, HWND hParent, RECT* rect,
	Chrome_CallBack_BrowserCreated created_callback, Chrome_CallBack_ChUrl churl_callback,
	Chrome_CallBack_NewWindow newwindow, Chrome_CallBack_Download download, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl, void* rev) {

	if (!std::wstring(url).substr(0, 6).compare(L"chrome")) return 0;

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(id, created_callback, churl_callback, newwindow,
		download, chstate, JSDialog, error, rbuttondown, chtitle, canloadurl));

	return handler->_CreateBrowserSync(std::wstring(url), hParent, *rect, 0);
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

//CKLEXPORT void WINAPI Chrome_CreateSimple(wchar_t* url, HWND hParent, RECT* rect, Chrome_CallBack_BrowserCreated created_callback) {
//	Chrome_CreateBrowserEx(0, url, hParent, rect, created_callback, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
//}

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
	CefCookieManager::GetGlobalManager(NULL)->FlushStore(NULL);

	CefShutdown();
}

CKLEXPORT void WINAPI Chrome_SetOSModalLoop(bool osModalLoop) {
	CefSetOSModalLoop(osModalLoop);
}

CKLEXPORT void WINAPI Chrome_EnableSystemFlash() {
	bEnableFlash = TRUE;
}

CKLEXPORT void WINAPI Chrome_LoadFlashPlugin(wchar_t* ppapi_flash_path, wchar_t* ppapi_flash_version) {
	bEnableFlash = TRUE;
	szFlashPath = ppapi_flash_path;
}

CKLEXPORT void WINAPI Chrome_SetUserAgent(wchar_t* _ua) {
	isSetUA = TRUE;
	ua = _ua;
}

CKLEXPORT void WINAPI Chrome_SetProxyServer(wchar_t* proxy) {
	bSetProxy = TRUE;
	szProxyServer = proxy;
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
		cookiemgr->FlushStore(NULL);
		cookiemgr->SetStoragePath("", false, NULL);
	}
}

CKLEXPORT void WINAPI Chrome_CookieManagerFlushStore() {
	CefCookieManager::GetGlobalManager(NULL)->FlushStore(NULL);
}

CKLEXPORT BOOL WINAPI Chrome_CookieManagerSetCookie(const wchar_t* szUrl, LPCOOKIE_DESCRIPTOR lpCookieDescriptor) {
	CefRefPtr<CefCookieManager> lpCookieManager = CefCookieManager::GetGlobalManager(NULL);
	CefCookie Cookie;
	CefString(&Cookie.name).FromWString(lpCookieDescriptor->szCookieName);
	CefString(&Cookie.value).FromWString(lpCookieDescriptor->szCookieValue);
	CefString(&Cookie.domain).FromWString(lpCookieDescriptor->szCookieDomain);//"xxx.xxx.com"
	CefString(&Cookie.path).FromWString(lpCookieDescriptor->szCookiePath); //"/"
	Cookie.has_expires = lpCookieDescriptor->bHasExpires;
	Cookie.secure = lpCookieDescriptor->bSecure;
	Cookie.httponly = lpCookieDescriptor->bHttponly;
	Cookie.expires.year = lpCookieDescriptor->iExpiresYear;
	Cookie.expires.month = lpCookieDescriptor->iExpiresMonth;
	Cookie.expires.day_of_week = lpCookieDescriptor->iExpiresDayOfWeek;
	Cookie.expires.day_of_month = lpCookieDescriptor->iExpiresDayOfMonth;
	Cookie.expires.hour = lpCookieDescriptor->iExpiresHour;
	Cookie.expires.minute = lpCookieDescriptor->iExpiresMinute;
	Cookie.expires.second = lpCookieDescriptor->iExpiresSecond;
	Cookie.expires.millisecond = lpCookieDescriptor->iExpiresMillisecond;
	//CefPostTask(TID_IO, base::Bind(lpCookieManager.get(), &CefCookieManager::SetCookie, CefString(szUrl), Cookie));
	return lpCookieManager->SetCookie(szUrl, Cookie, 0);
}

CKLEXPORT BOOL WINAPI Chrome_CookieManagerDeleteCookie(const wchar_t* szUrl, const wchar_t* szCookieName) {
	CefRefPtr<CefCookieManager> lpCookieManager = CefCookieManager::GetGlobalManager(NULL);
	return lpCookieManager->DeleteCookies(szUrl, szCookieName, 0);
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
	CefCookieManager::GetGlobalManager(NULL)->FlushStore(NULL);
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

CKLEXPORT void WINAPI Chrome_ShowDevToolsChild(SimpleHandler* handler, HWND hParent, RECT* rect, HWND* devtools_hwnd) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			CefWindowInfo dt_wininfo;
			dt_wininfo.SetAsChild(hParent, *rect);
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

CKLEXPORT void WINAPI Chrome_PrintToPDF(SimpleHandler* handler, wchar_t* pdf_path) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			CefPdfPrintSettings settings;
			settings.backgrounds_enabled = TRUE;
			browser->GetHost()->PrintToPDF(pdf_path, settings, 0);
		}
	}
}

CKLEXPORT double WINAPI Chrome_GetZoomLevel(SimpleHandler* handler) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			return browser->GetHost()->GetZoomLevel();
		}
	}
	return 0;
}


CKLEXPORT void WINAPI Chrome_SetZoomLevel(SimpleHandler* handler, double dbZoomLevel) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			browser->GetHost()->SetZoomLevel(dbZoomLevel);
		}
	}
}

CKLEXPORT wchar_t* WINAPI Chrome_DataURIBase64Encode(BYTE* lpData, DWORD dwSize, const wchar_t* szMimeType, const wchar_t* szCharset) {
	std::wstring encode_string;
	if (szCharset)
		encode_string = L"data:" + std::wstring(szMimeType) + L";charset=" + szCharset + L";base64," +
		CefURIEncode(CefBase64Encode(lpData, dwSize), false).ToWString();
	else encode_string = L"data:" + std::wstring(szMimeType) + L";base64," +
		CefURIEncode(CefBase64Encode(lpData, dwSize), false).ToWString();

	size_t stringlength = encode_string.length();

	wchar_t* buffer = (wchar_t*)malloc((stringlength + 1) * sizeof(wchar_t));
	encode_string.copy(buffer, stringlength);
	buffer[stringlength] = 0;

	return buffer;
}

CKLEXPORT void WINAPI Chrome_ReleaseBuffer(void* lpBuffer) {
	free(lpBuffer);
}