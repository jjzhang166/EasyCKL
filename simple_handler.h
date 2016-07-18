#ifndef __EASYCKL_SIMPLE_HANDLER_H_
#define __EASYCKL_SIMPLE_HANDLER_H_

#include "include/cef_client.h"

#include <list>

#define BROWSERFLAG_SYNC 0x1
#define BROWSERFLAG_HEADER_REFERER 0x2
#define BROWSERFLAG_DISABLE_JAVASCRIPT 0x4
#define BROWSERFLAG_DISABLE_LOAD_IMAGE 0x8
#define BROWSERFLAG_DISABLE_WEB_SECURITY 0x10
#define BROWSERFLAG_EXTDATA 0x20
#define BROWSERFLAG_DEF_ENCODING 0x40
#define BROWSERFLAG_BACK_COLOR 0x80

#define BROWSER_LASTERROR_LOADING 0x1
#define BROWSER_LASTERROR_LOADERROR 0x2
#define BROWSER_LASTERROR_LOADRESERROR 0x4
#define BROWSER_LASTERROR_CERTERROR 0x8

typedef struct tagCREATE_BROWSER_EXTDATA {
	SIZE_T cbSzie;
	wchar_t* szDefaultEncoding;
	DWORD dwBackColor;
}CREATE_BROWSER_EXTDATA, *LPCREATE_BROWSER_EXTDATA;

/* 回调函数中使用的结构 */
typedef struct tagNEW_WINDOW_INFOMATION {
	SIZE_T cbSzie;
	CefFrame* lpFrame;
	const wchar_t* szNewWindowUrl;
	const wchar_t* szCurrentWindowUrl;
	const wchar_t* szTargetFrameName;
	BOOL bUserGesture;
	DWORD dwOpenDisposition;
}NEW_WINDOW_INFOMATION, *LPNEW_WINDOW_INFOMATION;

typedef struct tagRBUTTON_DOWN_INFOMATION {
	SIZE_T cbSzie;
	DWORD dwFlag;
	CefFrame* lpFrame;
	const wchar_t* szSelectionText;
	const wchar_t* szLinkUrl;
	const wchar_t* szSourceUrl;
	void* Retention;
}RBUTTON_DOWN_INFOMATION, *LPRBUTTON_DOWN_INFOMATION;

typedef struct tagERROR_INFOMATION {
	SIZE_T cbSzie;
	CefFrame* lpFrame;
	BOOL bCertError;
	int iErrorCode;
	const wchar_t* szFailedUrl;
	void* lpSslInfo;
}ERROR_INFOMATION, *LPERROR_INFOMATION;

/* 回调函数的定义 */

typedef void(WINAPI * Chrome_CallBack_BrowserCreated)(LONG_PTR id, void* browser);
typedef void(WINAPI * Chrome_CallBack_Error)(LONG_PTR id, UINT_PTR uMsg, LPERROR_INFOMATION info, UINT_PTR not_used);
typedef void(WINAPI * Chrome_CallBack_ChUrl)(LONG_PTR id, const wchar_t* url);
typedef void(WINAPI * Chrome_CallBack_Download)(LONG_PTR id, const wchar_t* url);
typedef BOOL(WINAPI * Chrome_CallBack_NewWindow)(LONG_PTR id, UINT_PTR uMsg, LPNEW_WINDOW_INFOMATION info, UINT_PTR not_used);
typedef BOOL(WINAPI * Chrome_CallBack_ChState)(LONG_PTR id, BOOL isLoading, BOOL canGoBack, BOOL canGoForward);
typedef void(WINAPI * Chrome_CallBack_JSDialog)(LONG_PTR id, const wchar_t* msg);
typedef void(WINAPI * Chrome_CallBack_RButtonDown)(LONG_PTR id, UINT_PTR uMsg, LPRBUTTON_DOWN_INFOMATION info, UINT_PTR not_used);
typedef void(WINAPI * Chrome_CallBack_ChTitle)(LONG_PTR id, const wchar_t* text);
typedef bool(WINAPI * Chrome_CallBack_CanLoadUrl)(LONG_PTR id, const wchar_t* url);

typedef struct tagBROWSER_CALLBACKS {
	SIZE_T cbSzie;
	Chrome_CallBack_BrowserCreated created_callback;
	Chrome_CallBack_ChUrl churl_callback;
	Chrome_CallBack_NewWindow newwindow_callback;
	Chrome_CallBack_Download download_callback;
	Chrome_CallBack_ChState chstate_callback;
	Chrome_CallBack_JSDialog jsdialog_callback;
	Chrome_CallBack_Error error_callback;
	Chrome_CallBack_RButtonDown rbuttondown_callback;
	Chrome_CallBack_ChTitle chtitle_callback;
	Chrome_CallBack_CanLoadUrl canloadurl_callback;
}BROWSER_CALLBACKS, *LPBROWSER_CALLBACKS;

class SimpleHandler : public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefContextMenuHandler,
	public CefDownloadHandler,
	public CefJSDialogHandler,
	public CefRequestHandler
{
public:
	BROWSER_CALLBACKS callbacks;

	LONG_PTR g_id;
	LONG_PTR userData;
	DWORD lasterror;
	DWORD flags;
	CefBrowser* g_browser = nullptr;
	std::wstring referer_string;

	SimpleHandler(DWORD id, LPBROWSER_CALLBACKS _callbacks);
	SimpleHandler(DWORD id, Chrome_CallBack_BrowserCreated callback, Chrome_CallBack_ChUrl churl,
		Chrome_CallBack_NewWindow nwin, Chrome_CallBack_Download down, Chrome_CallBack_ChState chstate,
		Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
		Chrome_CallBack_ChTitle chtitle, Chrome_CallBack_CanLoadUrl canloadurl);
	~SimpleHandler();

	virtual void _CreateBrowser(std::wstring url, HWND hParent, RECT &rect, LPCREATE_BROWSER_EXTDATA extdata);
	virtual void* _CreateBrowserSync(std::wstring url, HWND hParent, RECT &rect, LPCREATE_BROWSER_EXTDATA extdata);

	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
		return this;
	}

	// CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;
	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		cef_window_open_disposition_t target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) OVERRIDE;

	// CefLoadHandler methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;
	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) OVERRIDE;

	//virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
	//	CefRefPtr<CefFrame> frame)OVERRIDE;
	//virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
	//	CefRefPtr<CefFrame> frame,
	//	int httpStatusCode) OVERRIDE;

	//CefDownloadHandler methods:
	virtual void OnBeforeDownload(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		const CefString& suggested_name,
		CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
	virtual void OnDownloadUpdated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

	//CefContextMenuHandler methods:
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) OVERRIDE;
	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		EventFlags event_flags) OVERRIDE {
		return false;
	}

	virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
		const CefString& origin_url,
		const CefString& accept_lang,
		JSDialogType dialog_type,
		const CefString& message_text,
		const CefString& default_prompt_text,
		CefRefPtr<CefJSDialogCallback> callback,
		bool& suppress_message) OVERRIDE;

	/*

	* 此声明为新版本 CEF 准备

	//CefJSDialogHandler methods:
	virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
		const CefString& origin_url,
		JSDialogType dialog_type,
		const CefString& message_text,
		const CefString& default_prompt_text,
		CefRefPtr<CefJSDialogCallback> callback,
		bool& suppress_message) OVERRIDE;
	*/

	//CefRequestHandler methods:
	virtual bool OnCertificateError(
		CefRefPtr<CefBrowser> browser,
		cef_errorcode_t cert_error,
		const CefString& request_url,
		CefRefPtr<CefSSLInfo> ssl_info,
		CefRefPtr<CefRequestCallback> callback) OVERRIDE;
	virtual ReturnValue OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);
	bool IsClosing() const { return is_closing_; }

private:
	// List of existing browser windows. Only accessed on the CEF UI thread.
	typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
	BrowserList browser_list_;
	bool is_closing_;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(SimpleHandler);
};

#endif //EASYCKL_SIMPLE_HANDLER_H_