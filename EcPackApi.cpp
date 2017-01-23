#include "ec_portable.h"

#include "simple_app.h"
#include "simple_handler.h"

#define __EC_PACK_API_CPP_
#define __ECKL_SRC_DEV_
#include "CKLMain.h"
#include "SDK/C and C++/EasyCKL.h"

#undef CKLEXPORT
#ifdef _WIN32
#define CKLEXPORT extern "C" __declspec(dllexport)
#elif defined __linux__
#define CKLEXPORT extern "C"
#endif

CKLEXPORT void WINAPI EcPkHtmlRefreshContentJumpUrl(SimpleHandler* handler, wchar_t* url, wchar_t* referer) {
	if (!std::wstring(url).substr(0, 6).compare(L"chrome")) return;
	std::wstring html = L"<html><head><meta http-equiv=\"refresh\" content=\"0;url=" + std::wstring(url) + L"\"></head><body bgcolor=\"white\"></body></html>";
	Chrome_LoadString(handler, (wchar_t*)html.c_str(), referer);
}

CKLEXPORT void* WINAPI EcPkCreateJSRefererBrowserSync(DWORD id, HWND hParent, RECT* rect, wchar_t* url, wchar_t* referer, LPBROWSER_CALLBACKS callbacks) {
	void* browser = Chrome_CreateChildBrowser(BROWSERFLAG_SYNC, callbacks, id, NULL, L":", hParent, rect, 0);
	if (browser) {
		if (referer) {
			std::wstring html = L"<html><head><meta http-equiv=\"refresh\" content=\"0;url=" + std::wstring(url) + L"\"></head><body bgcolor=\"white\"></body></html>";
			Chrome_LoadString((SimpleHandler *)browser, (wchar_t*)html.c_str(), referer);
		}
		else Chrome_LoadUrl((SimpleHandler *)browser, url);
	}
	return browser;
}

CKLEXPORT void WINAPI EcPkDisableDragDrop(SimpleHandler* handler) {
#ifdef _WIN32
	HWND hBrowserWindow = Chrome_GetWindowHandle(handler);
	if (hBrowserWindow) {
		HWND hWndHostChild = GetWindow(hBrowserWindow, GW_CHILD);

		HMODULE hModule = LoadLibraryW(L"ole32.dll");
		if (hModule) {
			typedef int (WINAPI * RevokeDragDrop_t)(HWND hWnd);
			RevokeDragDrop_t RevokeDragDrop = (RevokeDragDrop_t)GetProcAddress(hModule, "RevokeDragDrop");
			if (RevokeDragDrop)
				RevokeDragDrop(hWndHostChild);
			//#include <Ole2.h>
			//#pragma comment(lib, "ole32.lib")
			//RevokeDragDrop(hWndHostChild);
		}
	}
#elif defined __linux__
	//Not support linux!
#endif
}

CKLEXPORT void WINAPI EcPkJavaScriptSetValueByObjectId(SimpleHandler* handler, wchar_t* id, wchar_t* value) {
	std::wstring js = L"document.getElementById('" + std::wstring(id) + L"').value='" + value + L"'";
	Chrome_ExecJS(handler, js.c_str());
}

CKLEXPORT void WINAPI EcPkJavaScriptSetValueByObjectName(SimpleHandler* handler, wchar_t* name, wchar_t* value) {
	std::wstring js = L"document." + std::wstring(name) + L".value='" + value + L"'";
	Chrome_ExecJS(handler, js.c_str());
}

CKLEXPORT void WINAPI EcPkJavaScriptSubmitByFormId(SimpleHandler* handler, wchar_t* id) {
	std::wstring js = L"document.getElementById('" + std::wstring(id) + L"').submit()";
	Chrome_ExecJS(handler, js.c_str());
}

CKLEXPORT void WINAPI EcPkJavaScriptSubmitByFormName(SimpleHandler* handler, wchar_t* name) {
	std::wstring js = L"document." + std::wstring(name) + L".submit()";
	Chrome_ExecJS(handler, js.c_str());
}

CKLEXPORT void WINAPI EcPkJavaScriptSetCheckedByObjectId(SimpleHandler* handler, wchar_t* id, bool checked) {
	std::wstring js = L"document.getElementById('" + std::wstring(id) + L"').checked=" + (checked ? L"true" : L"false");
	Chrome_ExecJS(handler, js.c_str());
}

CKLEXPORT void WINAPI EcPkJavaScriptSetCheckedByObjectName(SimpleHandler* handler, wchar_t* name, bool checked) {
	std::wstring js = L"document." + std::wstring(name) + L".checked=" + (checked ? L"true" : L"false");
	Chrome_ExecJS(handler, js.c_str());
}

CKLEXPORT void WINAPI EcPkJavaScriptClickButtonByObjectId(SimpleHandler* handler, wchar_t* id) {
	std::wstring js = L"document.getElementById('" + std::wstring(id) + L"').click()";
	Chrome_ExecJS(handler, js.c_str());
}

CKLEXPORT void WINAPI EcPkJavaScriptClickButtonByObjectName(SimpleHandler* handler, wchar_t* name) {
	std::wstring js = L"document." + std::wstring(name) + L".click()";
	Chrome_ExecJS(handler, js.c_str());
}