// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_handler.h"

#include <string>
#include <windows.h>
#include<Shellapi.h>

#include "include/cef_browser.h"
#include "include/wrapper/cef_helpers.h"

#include "simple_app.h"

#include "callbacks.h"

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
	const CefString& title) {
	CEF_REQUIRE_UI_THREAD();
}

void SimpleHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	int httpStatusCode) {
	CEF_REQUIRE_UI_THREAD();
}


void SimpleHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame) {
	CEF_REQUIRE_UI_THREAD();
}

void SimpleHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
	bool isLoading,
	bool canGoBack,
	bool canGoForward) {
	CEF_REQUIRE_UI_THREAD();

	if (chstate_callback) {
		chstate_callback(g_id, isLoading, canGoBack, canGoForward);
	}
}

bool SimpleHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access) {
	//CEF_REQUIRE_UI_THREAD();

	if (newwindow_callback) {
		if (newwindow_callback(g_id, (char*)target_url.ToString().c_str())) {
			return true;
		}
	}
	browser->GetMainFrame()->LoadURL(target_url);
	//ShellExecute(0, L"open", target_url.c_str(), L"", L"", SW_SHOWNORMAL);
	return true;
}

void SimpleHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& url) {
	CEF_REQUIRE_UI_THREAD();

	if (churl_callback) {
		churl_callback(g_id, url.ToString().c_str());
	}
}

void SimpleHandler::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback) {
	CEF_REQUIRE_UI_THREAD();

	if (download_callback) {
		download_callback(g_id, (char*)download_item->GetURL().ToString().c_str());
	}

}

void SimpleHandler::OnDownloadUpdated(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback) {
	CEF_REQUIRE_UI_THREAD();

	callback->Cancel();
}

void SimpleHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model) {
	CEF_REQUIRE_UI_THREAD();

	auto flag = params->GetTypeFlags();
	if (rbuttondown_callback) {
		model->Clear();
		auto text = params->GetSelectionText();
		rbuttondown_callback(g_id, flag, text.ToString().c_str());
		return;
	}
	if (flag & CM_TYPEFLAG_PAGE) {
		//model->SetLabel(MENU_ID_BACK, L"后退");
		//model->SetLabel(MENU_ID_FORWARD, L"前进");
		//model->SetLabel(MENU_ID_VIEW_SOURCE, L"查看源代码");
		//model->SetLabel(MENU_ID_PRINT, L"打印");
		//model->SetLabel(MENU_ID_RELOAD, L"刷新");
		model->SetLabel(MENU_ID_RELOAD_NOCACHE, L"强制刷新");
		model->SetLabel(MENU_ID_STOPLOAD, L"停止加载");
		model->SetLabel(MENU_ID_REDO, L"重复");
	}
	if ((flag & CM_TYPEFLAG_EDITABLE) || (flag & CM_TYPEFLAG_SELECTION)) {
		model->SetLabel(MENU_ID_UNDO, L"撤销");
		model->SetLabel(MENU_ID_REDO, L"重做");
		model->SetLabel(MENU_ID_CUT, L"剪切");
		model->SetLabel(MENU_ID_COPY, L"复制");
		model->SetLabel(MENU_ID_PASTE, L"粘贴");
		model->SetLabel(MENU_ID_DELETE, L"删除");
		model->SetLabel(MENU_ID_SELECT_ALL, L"全选");
	}
	model->Remove(MENU_ID_PRINT);
	model->Remove(MENU_ID_VIEW_SOURCE);
	model->Remove(MENU_ID_BACK);
	model->Remove(MENU_ID_FORWARD);
	//model->Remove(MENU_ID_REDO);
	if (MENUITEMTYPE_SEPARATOR == model->GetTypeAt(0))
		model->RemoveAt(0);
	model->AddItem(MENU_ID_RELOAD, L"刷新");
	model->SetLabel(MENU_ID_COPY, L"复制");
	model->AddItem(MENU_ID_SELECT_ALL, L"全选");

}

bool SimpleHandler::OnJSDialog(CefRefPtr<CefBrowser> browser,
	const CefString& origin_url,
	const CefString& accept_lang,
	JSDialogType dialog_type,
	const CefString& message_text,
	const CefString& default_prompt_text,
	CefRefPtr<CefJSDialogCallback> _callback,
	bool& suppress_message) {
	CEF_REQUIRE_UI_THREAD();

	if (JSDialog_callback && dialog_type == JSDIALOGTYPE_ALERT) {
		JSDialog_callback(g_id, message_text.ToString().c_str());
		_callback->Continue(1, "");
		return true;
	}
	/*
	JSDIALOGTYPE_ALERT = 0,
	JSDIALOGTYPE_CONFIRM,
	JSDIALOGTYPE_PROMPT,*/
	//MessageBox(0, message_text.c_str(), 0, 0);
	return false;
}

void SimpleHandler::_CreateBrowser(std::string url, HWND hParent, RECT &rect) {
	CEF_REQUIRE_UI_THREAD();
	// Information used when creating the native window.
	CefWindowInfo window_info;

	// On Windows we need to specify certain flags that will be passed to
	// CreateWindowEx().
	//window_info.SetAsPopup(NULL, "cefsimple");
	window_info.SetAsChild(hParent, rect);

	// SimpleHandler implements browser-level callbacks.
	//CefRefPtr<SimpleHandler> handler(new SimpleHandler(callbacks));

	// Specify CEF browser settings here.
	CefBrowserSettings browser_settings;

	// Check if a "--url=" value was provided via the command-line. If so, use
	// that instead of the default URL.

	//if (!_url.empty()){
	// CefRefPtr<CefCommandLine> command_line =
	//  CefCommandLine::GetGlobalCommandLine();
	// url = command_line->GetSwitchValue("url");
	//}
	//else{
	// url = _url;
	//}

	// Create the browser window.

	CefBrowserHost::CreateBrowser(window_info, this, url, browser_settings, NULL);

	//_rect.left += 600;
	//_rect.right += 600;
	//window_info.SetAsChild(_hParent, _rect);

	//ChromeCallBack cb = { 0 };
	//CefRefPtr<SimpleHandler> handler2(new SimpleHandler(cb));
	//CefBrowserHost::CreateBrowser(window_info, handler2.get(), url, browser_settings, NULL);
}
