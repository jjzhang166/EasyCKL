// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_handler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include "callbacks.h"

typedef void(WINAPI * Chrome_CallBack_BrowserCreated)(DWORD id, void* browser);
typedef void(WINAPI * Chrome_CallBack_Error)(DWORD id, const char* url);

namespace {

	SimpleHandler* g_instance = NULL;

}  // namespace

SimpleHandler::SimpleHandler(DWORD id, void* _created_callback, void* churl, void* nwin, void* down, void* chstate, void* JSDialog, void* error, void* rbuttondown)
	: is_closing_(false) {
	created_callback = _created_callback;
	churl_callback = churl;
	newwindow_callback = nwin;
	download_callback = down;
	chstate_callback = chstate;
	JSDialog_callback = JSDialog;
	error_callback = error;
	rbuttondown_callback = rbuttondown;
	g_id = id;
	//DCHECK(!g_instance);
	g_instance = this;
}

SimpleHandler::~SimpleHandler() {
	g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
	return g_instance;
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	if (!g_browser.get()) {
		g_browser = browser;
		if (created_callback) {
			Chrome_CallBack_BrowserCreated a = (Chrome_CallBack_BrowserCreated)created_callback;
			a(g_id, this);
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

	if (error_callback) {
		Chrome_CallBack_Error a = (Chrome_CallBack_Error)error_callback;
		a(g_id, failedUrl.ToString().c_str());
		return;
	}

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><head><meta http-equiv=\"content-type\" content=\"text/html;charset=gb2312\"></head><body bgcolor=\"white\">"
		"<h2>Failed to open this page" <<
		"</h2><p>Error: " << errorCode <<
		"</p><p>Url: " << std::string(failedUrl) <<
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

	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}
