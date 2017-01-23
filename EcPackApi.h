#ifndef _EC_PACK_API_H_
#define _EC_PACK_API_H_
#include "ec_portable.h"
#include "CKLMain.h"

#include "simple_app.h"
#include "simple_handler.h"

CKLEXPORT void WINAPI EcPkHtmlRefreshContentJumpUrl(SimpleHandler* lpBrowser, wchar_t* url, wchar_t* referer);
CKLEXPORT void* WINAPI EcPkCreateJSRefererBrowserSync(DWORD id, HWND hParent, RECT* rect, wchar_t* url, wchar_t* referer, LPBROWSER_CALLBACKS callbacks);
CKLEXPORT void WINAPI EcPkDisableDragDrop(SimpleHandler* lpBrowser);
CKLEXPORT void WINAPI EcPkJavaScriptSetValueByObjectId(SimpleHandler* lpBrowser, wchar_t* id, wchar_t* value);
CKLEXPORT void WINAPI EcPkJavaScriptSetValueByObjectName(SimpleHandler* lpBrowser, wchar_t* name, wchar_t* value);
CKLEXPORT void WINAPI EcPkJavaScriptSubmitByFormId(SimpleHandler* lpBrowser, wchar_t* id);
CKLEXPORT void WINAPI EcPkJavaScriptSubmitByFormName(SimpleHandler* lpBrowser, wchar_t* name);
CKLEXPORT void WINAPI EcPkJavaScriptSetCheckedByObjectId(SimpleHandler* lpBrowser, wchar_t* id, bool checked);
CKLEXPORT void WINAPI EcPkJavaScriptSetCheckedByObjectName(SimpleHandler* lpBrowser, wchar_t* name, bool checked);
CKLEXPORT void WINAPI EcPkJavaScriptClickButtonByObjectId(SimpleHandler* lpBrowser, wchar_t* id);
CKLEXPORT void WINAPI EcPkJavaScriptClickButtonByObjectName(SimpleHandler* lpBrowser, wchar_t* name);

#endif // _EC_PACK_API_H_
