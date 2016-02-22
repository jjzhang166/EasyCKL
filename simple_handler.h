#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include "callbacks.h"

#include <list>

typedef void(WINAPI * Chrome_CallBack_BrowserCreated)(DWORD id, void* browser);
typedef void(WINAPI * Chrome_CallBack_Error)(DWORD id, const char* url);
typedef void(WINAPI * Chrome_CallBack_ChUrl)(DWORD id, const char* url);
typedef void(WINAPI * Chrome_CallBack_Download)(DWORD id, const char* url);
typedef BOOL(WINAPI * Chrome_CallBack_NewWindow)(DWORD id, const char* url);
typedef BOOL(WINAPI * Chrome_CallBack_ChState)(DWORD id, BOOL isLoading, BOOL canGoBack, BOOL canGoForward);
typedef void(WINAPI * Chrome_CallBack_JSDialog)(DWORD id, const wchar_t* msg);
typedef void(WINAPI * Chrome_CallBack_RButtonDown)(DWORD id, int flag, const wchar_t*, const wchar_t* link);
typedef void(WINAPI * Chrome_CallBack_ChTitle)(DWORD id, const wchar_t* text);

class SimpleHandler : public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefContextMenuHandler,
	public CefDownloadHandler,
	public CefJSDialogHandler
{
public:
	Chrome_CallBack_BrowserCreated created_callback = 0;
	Chrome_CallBack_ChUrl churl_callback = 0;
	Chrome_CallBack_NewWindow newwindow_callback = 0;
	Chrome_CallBack_Download download_callback = 0;
	Chrome_CallBack_ChState chstate_callback = 0;
	Chrome_CallBack_JSDialog JSDialog_callback = 0;
	Chrome_CallBack_Error error_callback = 0;
	Chrome_CallBack_RButtonDown rbuttondown_callback = 0;
	Chrome_CallBack_ChTitle chtitle_callback = 0;
	DWORD g_id;
	CefBrowser* g_browser = nullptr;
	SimpleHandler(DWORD id, Chrome_CallBack_BrowserCreated callback, Chrome_CallBack_ChUrl churl,
		Chrome_CallBack_NewWindow nwin, Chrome_CallBack_Download down, Chrome_CallBack_ChState chstate,
		Chrome_CallBack_JSDialog JSDialog, Chrome_CallBack_Error error, Chrome_CallBack_RButtonDown rbuttondown,
		Chrome_CallBack_ChTitle chtitle);
	~SimpleHandler();

	// Provide access to the single global instance of this object.
	//static SimpleHandler* GetInstance();

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

	virtual void _CreateBrowser(std::string url, HWND hParent, RECT &rect);
	virtual void* _CreateBrowserSync(std::string url, HWND hParent, RECT &rect);

	// CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	// CefLoadHandler methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;

	//methods:

	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward);

	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame);

	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode);


	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access);

	virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& url);


	virtual void OnBeforeDownload(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		const CefString& suggested_name,
		CefRefPtr<CefBeforeDownloadCallback> callback);

	virtual void OnDownloadUpdated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		CefRefPtr<CefDownloadItemCallback> callback);

	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model);

	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		EventFlags event_flags) {
		return false;
	}
	virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
		const CefString& origin_url,
		const CefString& accept_lang,
		JSDialogType dialog_type,
		const CefString& message_text,
		const CefString& default_prompt_text,
		CefRefPtr<CefJSDialogCallback> callback,
		bool& suppress_message);

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

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
