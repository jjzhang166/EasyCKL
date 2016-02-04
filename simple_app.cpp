// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_app.h"

#include <string>

#include "simple_handler.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_app.h"

#include "callbacks.h"

typedef void(WINAPI * Chrome_CallBack_V8)(CefV8Context *context);

extern HANDLE hEvent;
extern void* v8contextcreate;

/*class MyV8Handler : public CefV8Handler {
public:
	MyV8Handler() {}
	~MyV8Handler() {}
	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) OVERRIDE {

		if (name == L"MyFunction") {
			MessageBoxA(0, "frh", 0, 0);
		}
		MessageBox(0, name.c_str(), L"gfgdfh", 0);
		// Function does not exist.
		return false;
	}

	// Provide the reference counting implementation for this class.
	IMPLEMENT_REFCOUNTING(MyV8Handler);
};*/

SimpleApp::SimpleApp() {
}

void SimpleApp::OnContextInitialized() {
	CEF_REQUIRE_UI_THREAD();
	SetEvent(hEvent);
}

/*void SimpleApp::OnWebKitInitialized() {
	CefRefPtr<MyV8Handler> v8handler = new MyV8Handler();
	CefString code = L"var example"
		L" if (!example)"
		L"example = {};"
		L"(function() {"
		L"example.myfunction = function() {"
		L"	native function MyFunction();"
		L"	return MyFunction();"
		L"};"
		L"})(); ";
	if (!CefRegisterExtension(L"v8/example", code, new MyV8Handler())) {
		MessageBoxA(0, "Ê§°Ü", 0, 0);
	}
	else {
		MessageBoxA(0, "fhdhfgthf", 0, 0);
	}

	SetEvent(hEvent2);
	MessageBoxA(0, "dddd", 0, 0);
}*/

void SimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
	if (v8contextcreate) {
		((Chrome_CallBack_V8)v8contextcreate)(context.get());
	}

	/*CefRefPtr<CefV8Handler> myV8handle = new MyV8Handler();
	CefRefPtr<CefV8Value> myFun = CefV8Value::CreateFunction(L"MyFunction", myV8handle);
	CefRefPtr<CefV8Value> pObjApp = context->GetGlobal();
	pObjApp->SetValue(L"MyFunction", myFun, V8_PROPERTY_ATTRIBUTE_NONE);*/
}
