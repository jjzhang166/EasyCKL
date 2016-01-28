// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include "callbacks.h"

#include <list>

class SimpleHandler : public CefClient,
                      public CefDisplayHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler,
					  public CefContextMenuHandler,
					  public CefDownloadHandler,
					  public CefJSDialogHandler
{
 public:
	 void* created_callback=0;
	 void* churl_callback=0;
	 void* newwindow_callback=0;
	 void* download_callback = 0;
	 void* chstate_callback = 0;
	 void* JSDialog_callback = 0;
	 void* error_callback = 0;
	 void* rbuttondown_callback = 0;
	 DWORD g_id;
	 CefRefPtr<CefBrowser> g_browser;
	 SimpleHandler(DWORD id,void* callback, void* churl, void* nwin, void* down, void* chstate, void* JSDialog, void* error, void* rbuttondown);
  ~SimpleHandler();

  // Provide access to the single global instance of this object.
  static SimpleHandler* GetInstance();

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
	  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE{
	  return this;
  }
	  virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE{
	  return this;
  }
	  virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE{
	  return this;
  }
  virtual void _CreateBrowser(std::string url, HWND hParent, RECT* rect);

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


  ///
  // Called when the loading state has changed. This callback will be executed
  // twice -- once when loading is initiated either programmatically or by user
  // action, and once when loading is terminated due to completion, cancellation
  // of failure.
  ///
  /*--cef()--*/
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
	  bool isLoading,
	  bool canGoBack,
	  bool canGoForward);

  ///
  // Called when the browser begins loading a frame. The |frame| value will
  // never be empty -- call the IsMain() method to check if this frame is the
  // main frame. Multiple frames may be loading at the same time. Sub-frames may
  // start or continue loading after the main frame load has ended. This method
  // may not be called for a particular frame if the load request for that frame
  // fails. For notification of overall browser load status use
  // OnLoadingStateChange instead.
  ///
  /*--cef()--*/
  virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
	  CefRefPtr<CefFrame> frame);

  ///
  // Called when the browser is done loading a frame. The |frame| value will
  // never be empty -- call the IsMain() method to check if this frame is the
  // main frame. Multiple frames may be loading at the same time. Sub-frames may
  // start or continue loading after the main frame load has ended. This method
  // will always be called for all frames irrespective of whether the request
  // completes successfully.
  ///
  /*--cef()--*/
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


  // Called before a download begins. |suggested_name| is the suggested name for
  // the download file. By default the download will be canceled. Execute
  // |callback| either asynchronously or in this method to continue the download
  // if desired. Do not keep a reference to |download_item| outside of this
  // method.
  ///
  /*--cef()--*/
  virtual void OnBeforeDownload(
	  CefRefPtr<CefBrowser> browser,
	  CefRefPtr<CefDownloadItem> download_item,
	  const CefString& suggested_name,
	  CefRefPtr<CefBeforeDownloadCallback> callback);

  ///
  // Called when a download's status or progress information has been updated.
  // This may be called multiple times before and after OnBeforeDownload().
  // Execute |callback| either asynchronously or in this method to cancel the
  // download if desired. Do not keep a reference to |download_item| outside of
  // this method.
  ///
  /*--cef()--*/
  virtual void OnDownloadUpdated(
	  CefRefPtr<CefBrowser> browser,
	  CefRefPtr<CefDownloadItem> download_item,
	  CefRefPtr<CefDownloadItemCallback> callback);

  virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
	  CefRefPtr<CefFrame> frame,
	  CefRefPtr<CefContextMenuParams> params,
	  CefRefPtr<CefMenuModel> model);

  ///
  // Called to execute a command selected from the context menu. Return true if
  // the command was handled or false for the default implementation. See
  // cef_menu_id_t for the command ids that have default implementations. All
  // user-defined command ids should be between MENU_ID_USER_FIRST and
  // MENU_ID_USER_LAST. |params| will have the same values as what was passed to
  // OnBeforeContextMenu(). Do not keep a reference to |params| outside of this
  // callback.
  ///
  /*--cef()--*/
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
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
