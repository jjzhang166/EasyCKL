#include "simple_handler.h"

#include <string>
#include <windows.h>
#include<Shellapi.h>

#include "include/cef_browser.h"
#include "include/wrapper/cef_helpers.h"

#include "simple_app.h"

extern CefString localinf;

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
	const CefString& title) {
	CEF_REQUIRE_UI_THREAD();

	if (!browser->IsSame(g_browser)) return;

	if (callbacks.chtitle_callback) {
		callbacks.chtitle_callback(g_id, title.c_str());
	}
}

//void SimpleHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
//	CefRefPtr<CefFrame> frame,
//	int httpStatusCode) {
//	CEF_REQUIRE_UI_THREAD();
//}

//void SimpleHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
//	CefRefPtr<CefFrame> frame) {
//	CEF_REQUIRE_UI_THREAD();
//}

void SimpleHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
	bool isLoading,
	bool canGoBack,
	bool canGoForward) {
	CEF_REQUIRE_UI_THREAD();

	if (!browser->IsSame(g_browser)) return;

	if (isLoading) lasterror = BROWSER_LASTERROR_LOADING;
	else lasterror &= ~BROWSER_LASTERROR_LOADING;

	if (callbacks.chstate_callback) {
		callbacks.chstate_callback(g_id, isLoading, canGoBack, canGoForward);
	}
}

bool SimpleHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	cef_window_open_disposition_t target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access) {

	if (callbacks.newwindow_callback) {
		CefString CurrentWindowUrl = browser->GetMainFrame()->GetURL();

		NEW_WINDOW_INFOMATION info;
		info.cbSzie = sizeof(NEW_WINDOW_INFOMATION);
		info.lpFrame = frame;
		info.szNewWindowUrl = target_url.c_str();
		info.szCurrentWindowUrl = CurrentWindowUrl.c_str();
		info.szTargetFrameName = target_frame_name.c_str();
		info.bUserGesture = user_gesture;
		info.dwOpenDisposition = target_disposition;

		if (callbacks.newwindow_callback(g_id, 0, &info, 0)) {
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

	if (!browser->IsSame(g_browser)) return;
	if (callbacks.churl_callback) {
		callbacks.churl_callback(g_id, url.ToWString().c_str());
	}
}

void SimpleHandler::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback) {
	CEF_REQUIRE_UI_THREAD();

	if (callbacks.download_callback) {
		callbacks.download_callback(g_id, download_item->GetURL().ToWString().c_str());
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

	if (!browser->IsSame(g_browser)) return;

	auto flag = params->GetTypeFlags();
	if (callbacks.rbuttondown_callback) {
		model->Clear();
		RBUTTON_DOWN_INFOMATION info;
		info.cbSzie = sizeof(RBUTTON_DOWN_INFOMATION);
		info.dwFlag = flag;
		info.lpFrame = frame;
		info.Retention = 0;

		auto SelectionText = params->GetSelectionText();
		auto LinkUrl = params->GetLinkUrl();
		auto SourceUrl = params->GetSourceUrl();

		info.szSelectionText = SelectionText.c_str();
		info.szLinkUrl = LinkUrl.c_str();
		info.szSourceUrl = SourceUrl.c_str();

		callbacks.rbuttondown_callback(g_id, 0, &info, 0);
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

	if (callbacks.jsdialog_callback && dialog_type == JSDIALOGTYPE_ALERT) {
		callbacks.jsdialog_callback(g_id, message_text.c_str());
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

bool SimpleHandler::OnCertificateError(
	CefRefPtr<CefBrowser> browser,
	cef_errorcode_t cert_error,
	const CefString& request_url,
	CefRefPtr<CefSSLInfo> ssl_info,
	CefRefPtr<CefRequestCallback> callback) {
	CEF_REQUIRE_UI_THREAD();

	if (!browser->IsSame(g_browser)) return false;

	lasterror |= BROWSER_LASTERROR_CERTERROR;

	if (callbacks.error_callback) {
		callbacks.error_callback(g_id, request_url.ToWString().c_str(), TRUE);
	}
	return false;
}

cef_return_value_t SimpleHandler::OnBeforeResourceLoad(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefRequestCallback> callback) {

	if (!browser->IsSame(g_browser)) return RV_CONTINUE;

	std::multimap<CefString, CefString> map;
	std::pair<CefString, CefString> pair;
	request->GetHeaderMap(map);

	if (!localinf.empty()) {
		pair.first = L"Accept-Language";
		pair.second = localinf;// L"zh-CN";
		map.insert(pair);
	}

	if (request->GetReferrerURL().empty() && !referer_string.empty())
	{
		/*pair.first = L"Referer";
		pair.second = CefString(referer_string);
		map.insert(pair);*/
		request->SetReferrer(CefString(referer_string), REFERRER_POLICY_ALWAYS);
		referer_string.clear();
	}

	request->SetHeaderMap(map);
	if (callbacks.canloadurl_callback)
		if (!callbacks.canloadurl_callback(g_id, request->GetURL().ToWString().c_str()))return RV_CANCEL;
	return RV_CONTINUE;
}

void SimpleHandler::_CreateBrowser(std::wstring url, HWND hParent, RECT &rect) {
	CEF_REQUIRE_UI_THREAD();

	CefWindowInfo window_info;
	window_info.SetAsChild(hParent, rect);
	CefBrowserSettings browser_settings;
	browser_settings.javascript_close_windows = STATE_DISABLED;
	if (flags & BROWSERFLAG_DISABLE_JAVASCRIPT)
		browser_settings.javascript = STATE_DISABLED;
	if (flags & BROWSERFLAG_DISABLE_LOAD_IMAGE)
		browser_settings.image_loading = STATE_DISABLED;
	if (flags & BROWSERFLAG_DISABLE_WEB_SECURITY)
		browser_settings.web_security = STATE_DISABLED;
	CefBrowserHost::CreateBrowser(window_info, this, url, browser_settings, NULL);
}

void* SimpleHandler::_CreateBrowserSync(std::wstring url, HWND hParent, RECT &rect) {
	CEF_REQUIRE_UI_THREAD();

	CefWindowInfo window_info;
	window_info.SetAsChild(hParent, rect);
	CefBrowserSettings browser_settings;
	browser_settings.javascript_close_windows = STATE_DISABLED;
	if (flags & BROWSERFLAG_DISABLE_JAVASCRIPT)
		browser_settings.javascript = STATE_DISABLED;
	if (flags & BROWSERFLAG_DISABLE_LOAD_IMAGE)
		browser_settings.image_loading = STATE_DISABLED;
	if (flags & BROWSERFLAG_DISABLE_WEB_SECURITY)
		browser_settings.web_security = STATE_DISABLED;
	CefBrowserHost::CreateBrowserSync(window_info, this, url, browser_settings, NULL);
	return this;
}