#include "simple_handler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

SimpleHandler::SimpleHandler(DWORD id, Chrome_CallBack_BrowserCreated callback, Chrome_CallBack_ChUrl churl,
	Chrome_CallBack_NewWindow nwin, Chrome_CallBack_Download down, Chrome_CallBack_ChState chstate,
	Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
	Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl)

	: is_closing_(false), g_id(id), userData(0), lasterror(0), window_rect({ 0 }),
	need_create_with_referer(FALSE), hParentWnd(0) {

	//…Ë÷√ CallBack ∫Ø ˝÷∏’Î
	created_callback = callback;
	churl_callback = churl;
	newwindow_callback = nwin;
	download_callback = down;
	chstate_callback = chstate;
	JSDialog_callback = JSDialog;
	error_callback = error;
	rbuttondown_callback = rbuttondown;
	chtitle_callback = chtitle;
	canloadurl_callback = canloadurl;
	//referer_string = std::wstring(L"");
}

SimpleHandler::~SimpleHandler() {}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	if (!g_browser && !need_create_with_referer) {
		g_browser = browser.get();
		g_browser->AddRef();
		if (created_callback) {
			created_callback(g_id, this);
		}
	}

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (browser_list_.size() == 1) {
		// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// Remove from the list of existing browsers.
	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser)) {
			browser_list_.erase(bit);
			break;
		}
	}

	if (browser_list_.empty()) {
		if (g_browser)
			g_browser->Release();
		// All browser windows have closed. Quit the application message loop.
		//CefQuitMessageLoop();
	}
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl) {
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	if (!browser->IsSame(g_browser)) return;

	lasterror = 3;

	if (error_callback) {
		error_callback(g_id, failedUrl.ToWString().c_str(), FALSE);
		return;
	}

	// Display a load error message.
	std::wstringstream ss;
	ss << "<html><head><meta http-equiv=\"content-type\" content=\"text/html;charset=gb2312\"></head><body bgcolor=\"white\">"
		"<h2>Failed to open this page" <<
		"</h2><p>Error: " << errorCode <<
		"</p><p>Url: " << std::wstring(failedUrl) <<
		"</p></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI,
			base::Bind(&SimpleHandler::CloseAllBrowsers, this, force_close));
		return;
	}

	if (browser_list_.empty())
		return;

	for (auto it = browser_list_.begin(); it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}
