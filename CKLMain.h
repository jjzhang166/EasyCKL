#ifndef _CKLMAIN_H_
#define _CKLMAIN_H_

#include <Windows.h>

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_parser.h"

#include "simple_app.h"
#include "simple_handler.h"

#define CKLEXPORT extern "C" __declspec(dllexport)

typedef BOOL(WINAPI * V8Handler_CallBack)(const wchar_t* name, const void* argu, void* retval);
typedef void(WINAPI * Chrome_CallBack_V8)(void* context);

class MyV8Handler : public CefV8Handler {
public:
	MyV8Handler(V8Handler_CallBack handler) {
		handler_callback = handler;
	}
	~MyV8Handler() {}
	V8Handler_CallBack handler_callback = 0;
	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) OVERRIDE {

		if (handler_callback) {
			return handler_callback(name.c_str(), &arguments, &retval) != FALSE;
		}
		return false;
	}

	// Provide the reference counting implementation for this class.
	IMPLEMENT_REFCOUNTING(MyV8Handler);
};

enum BrowserInfomationType
{
	BrowserInfomationUserDataLoogPtr = 0,
	BrowserInfomationCanGoBack = 1,
	BrowserInfomationCanGoForward = 2,
	BrowserInfomationMainFrame = 3,
	BrowserInfomationIsLoading = 4,
	BrowserInfomationLastError = 5
};

#define INITFLAG_NOSSL 0x1
#define INITFLAG_CACHESTORAGE 0x2
#define INITFLAG_SINGLEPROCESS 0x4
#define INITFLAG_USECOMPATIBILITY 0x8
#define INITFLAG_ENABLEHIGHDPISUPPORT 0x10
#define INITFLAG_DISABLEGPU 0x20
#define INITFLAG_EXTDATA 0x40
#define INITFLAG_SETUSERAGENT 0x80

typedef struct tagINIT_EXTDATA {
	SIZE_T cbSzie;
	const wchar_t* szUserAgent;
} INIT_EXTDATA, *LPINIT_EXTDATA;

typedef struct _tagCOOKIE_DESCRIPTOR {
	SIZE_T cbSize;
	DWORD dwRetention;
	const wchar_t* szCookieName;
	const wchar_t* szCookieValue;
	const wchar_t* szCookieDomain;
	const wchar_t* szCookiePath;
	BOOL bHasExpires;
	BOOL bSecure;
	BOOL bHttponly;
	int iExpiresYear;          // Four digit year "2007"
	int iExpiresMonth;         // 1-based month (values 1 = January, etc.)
	int iExpiresDayOfWeek;     // 0-based day of week (0 = Sunday, etc.)
	int iExpiresDayOfMonth;    // 1-based day of month (1-31)
	int iExpiresHour;          // Hour within the current day (0-23)
	int iExpiresMinute;        // Minute within the current hour (0-59)
	int iExpiresSecond;        // Second within the current minute (0-59 plus leap seconds which may take it up to 60).
	int iExpiresMillisecond;   // Milliseconds within the current second (0-999)
} COOKIE_DESCRIPTOR, *LPCOOKIE_DESCRIPTOR;

typedef void(WINAPI * Ec_GetSource_CallBack)(LPVOID lpContext, const wchar_t* szSource);

#endif // _CKLMAIN_H_