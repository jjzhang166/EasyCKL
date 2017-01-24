#ifndef __BROWSER_H_
#define __BROWSER_H_
#include "ec_portable.h"
#include "include/cef_client.h"

#define BROWSERFLAG_SYNC 0x1
#define BROWSERFLAG_HEADER_REFERER 0x2
#define BROWSERFLAG_DISABLE_JAVASCRIPT 0x4
#define BROWSERFLAG_DISABLE_LOAD_IMAGE 0x8
#define BROWSERFLAG_DISABLE_WEB_SECURITY 0x10
#define BROWSERFLAG_EXTDATA 0x20
#define BROWSERFLAG_DEF_ENCODING 0x40
#define BROWSERFLAG_BACK_COLOR 0x80
#define BROWSERFLAG_DEF_FONT 0x100
#define BROWSERFLAG_DEF_FONT_SIZE 0x200

#define BROWSER_LASTERROR_LOADING 0x1
#define BROWSER_LASTERROR_LOADERROR 0x2
#define BROWSER_LASTERROR_LOADRESERROR 0x4
#define BROWSER_LASTERROR_CERTERROR 0x8

typedef struct tagCREATE_BROWSER_EXTDATA {
	SIZE_T cbSzie;
	wchar_t* szDefaultEncoding;
	DWORD dwBackColor;
	wchar_t* szDefaultFont;
	DWORD dwDefaultFontSize;
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

// No node is selected.
#define MENU_TYPEFLAG_NONE 0
// The top page is selected.
#define MENU_TYPEFLAG_PAGE 1 << 0
// A subframe page is selected.
#define MENU_TYPEFLAG_FRAME 1 << 1
// A link is selected.
#define MENU_TYPEFLAG_LINK 1 << 2
// A media node is selected.
#define MENU_TYPEFLAG_MEDIA 1 << 3
// There is a textual or mixed selection that is selected.
#define MENU_TYPEFLAG_SELECTION 1 << 4
// An editable element is selected.
#define MENU_TYPEFLAG_EDITABLE 1 << 5

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
typedef bool(WINAPI * Chrome_CallBack_CanClose)(LONG_PTR id, UINT_PTR uMsg, void* not_used, UINT_PTR not_used_);

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
	Chrome_CallBack_CanClose canclose_callback;
}BROWSER_CALLBACKS, *LPBROWSER_CALLBACKS;

#endif // __BROWSER_H_