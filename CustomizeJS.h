#ifndef _CUSTOMIZE_JS_H_
#define _CUSTOMIZE_JS_H_
#include "ec_portable.h"
#include "CKLMain.h"

#include "simple_app.h"
#include "simple_handler.h"

CKLEXPORT void WINAPI Chrome_AddJSFunction(CefV8Context* context, wchar_t* name);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueListSize(const CefV8ValueList* arguments);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueInt(const CefV8ValueList* arguments, size_t pos);
CKLEXPORT DWORD WINAPI Chrome_GetV8ValueStringLength(const CefV8ValueList* arguments, size_t pos);
CKLEXPORT void WINAPI Chrome_GetV8ValueString(const CefV8ValueList* arguments, size_t pos, wchar_t* buffer, size_t buffer_length);
CKLEXPORT void WINAPI Chrome_SetV8ReturnValueInt(CefRefPtr<CefV8Value>* lpretval, int value);
CKLEXPORT void WINAPI Chrome_SetV8ReturnValueString(CefRefPtr<CefV8Value>* lpretval, wchar_t* value);

#endif // _CUSTOMIZE_JS_H_
