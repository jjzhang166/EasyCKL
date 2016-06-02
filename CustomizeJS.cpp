#include <Windows.h>

#include "simple_app.h"
#include "simple_handler.h"

#define __EC_CUSTOMIZE_JS_CPP_
#define __ECKL_SRC_DEV_
#include "SDK\C and C++\EasyCKL.h"

#undef CKLEXPORT
#define CKLEXPORT extern "C" __declspec(dllexport)

extern CefRefPtr<CefV8Handler> myV8handle;

CKLEXPORT void WINAPI Chrome_AddJSFunction(CefV8Context* context, wchar_t* name) {
	auto myFun = CefV8Value::CreateFunction(name, myV8handle);
	auto pObjApp = context->GetGlobal();
	pObjApp->SetValue(name, myFun, V8_PROPERTY_ATTRIBUTE_NONE);
}

CKLEXPORT DWORD WINAPI Chrome_GetV8ValueListSize(const CefV8ValueList* arguments) {
	return arguments->size();
}

//*************************

CKLEXPORT DWORD WINAPI Chrome_GetV8ValueInt(const CefV8ValueList* arguments, size_t pos) {
	auto value = arguments->at(pos);
	if (value->IsValid())
		return value->GetUIntValue();
	return 0;
}

CKLEXPORT DWORD WINAPI Chrome_GetV8ValueStringLength(const CefV8ValueList* arguments, size_t pos) {
	auto value = arguments->at(pos);
	if (value->IsValid())
		return std::wstring(value->GetStringValue()).length();
	return 0;
}

CKLEXPORT void WINAPI Chrome_GetV8ValueString(const CefV8ValueList* arguments, size_t pos, wchar_t* buffer, size_t buffer_length) {
	auto value = arguments->at(pos);
	if (value->IsValid()) {
		_ECKL_CopyWString(value->GetStringValue(), buffer, buffer_length * sizeof(wchar_t));
		//std::wstring(value->GetStringValue()).copy(buffer, buffer_length);
		//memset(buffer + buffer_length - 2, 0, 2);//最后两个字节置0
	}
}

//*************************

CKLEXPORT void WINAPI Chrome_SetV8ReturnValueInt(CefRefPtr<CefV8Value>* lpretval, int value) {
	if (lpretval) {
		*lpretval = CefV8Value::CreateInt(value);
	}
}

CKLEXPORT void WINAPI Chrome_SetV8ReturnValueString(CefRefPtr<CefV8Value>* lpretval, wchar_t* value) {
	if (lpretval) {
		*lpretval = CefV8Value::CreateString(value);
	}
}