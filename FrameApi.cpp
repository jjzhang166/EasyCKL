#include <Windows.h>

#include "simple_app.h"
#include "simple_handler.h"

#define __EC_FRAME_API_CPP_
#define __ECKL_SRC_DEV_
#include "SDK\C and C++\EasyCKL.h"

#undef CKLEXPORT
#define CKLEXPORT extern "C" __declspec(dllexport)

CKLEXPORT void WINAPI Chrome_FrameLoadUrl(CefFrame* frame, wchar_t* url) {
	if (frame)
		frame->LoadURL(url);
}

CKLEXPORT void WINAPI Chrome_FrameLoadString(CefFrame* frame, wchar_t* string, wchar_t* url) {
	if (frame)
		if (!url)
			frame->LoadString(string, frame->GetURL());
		else frame->LoadString(string, url);
}

CKLEXPORT bool WINAPI Chrome_FrameIsMain(CefFrame* frame) {
	if (frame)
		return frame->IsMain();
	return 0;
}

CKLEXPORT void WINAPI Chrome_ReleaseFrame(CefFrame* frame) {
	if (frame)
		frame->Release();
}

CKLEXPORT CefFrame* WINAPI Chrome_GetNameFrame(SimpleHandler* handler, wchar_t* name) {
	if (handler) {
		CefRefPtr<CefBrowser> browser = handler->g_browser;
		if (browser && browser.get()) {
			CefFrame* frame = browser->GetFrame(name).get();
			frame->AddRef();
			return frame;
		}
	}
	return 0;
}

CKLEXPORT CefFrame* WINAPI EcQBIGetMainFrame(SimpleHandler* handler) {
	CefFrame* main;
	Chrome_QueryBrowserInfomation(handler, BrowserInfomationMainFrame, &main);
	return main;
}

CKLEXPORT void WINAPI Chrome_FrameDoCopy(CefFrame* frame) {
	if (frame)
		frame->Copy();
}

CKLEXPORT void WINAPI Chrome_FrameDoCut(CefFrame* frame) {
	if (frame)
		frame->Cut();
}

CKLEXPORT void WINAPI Chrome_FrameDoDelete(CefFrame* frame) {
	if (frame)
		frame->Delete();
}

CKLEXPORT void WINAPI Chrome_FrameDoPaste(CefFrame* frame) {
	if (frame)
		frame->Paste();
}

CKLEXPORT void WINAPI Chrome_FrameDoSelectAll(CefFrame* frame) {
	if (frame)
		frame->SelectAll();
}