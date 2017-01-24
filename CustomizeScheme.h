#ifndef _EC_PACK_API_H_
#define _EC_PACK_API_H_
/*
Note: the "DO NOT AUTO REMOVE" in a end of a line means that DISABLE the SDK头文件自动生成脚本 remove the line.
the "REMOVE IT" in a end of a line means that force the SDK头文件自动生成脚本 remove the line.
*/

#include "ec_portable.h"
#include "CKLMain.h"

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_request.h"

#include "simple_app.h"
#include "simple_handler.h"

typedef bool(WINAPI * SchemeProcessRequest)(void* request, void* data, void* mime_type, UINT* status);
typedef std::vector<unsigned char> BytesVector;			/* REMOVE IT */
typedef bool(WINAPI * SchemeProcessRequest)(void* request, void* data, void* mime_type, UINT* status);

CKLEXPORT void WINAPI Chrome_RegisterSchemeInitialize(SchemeProcessRequest callback);
CKLEXPORT void WINAPI Chrome_RegisterScheme(const wchar_t* szSchemeName, const wchar_t* szDomainName);
CKLEXPORT SIZE_T WINAPI EcCSGetRequestUrlLength(CefRequest* lpRequest);
CKLEXPORT void WINAPI EcCSGetRequestUrl(CefRequest* lpRequest, wchar_t* lpUrlBuffer, ULONG ulLength);
CKLEXPORT SIZE_T WINAPI EcCSGetRequestHeaderStringLength(CefRequest* lpRequest, wchar_t* szHeaderName);
CKLEXPORT void WINAPI EcCSGetRequestHeaderString(CefRequest* lpRequest, wchar_t* szHeaderName, wchar_t* lpBuffer, ULONG ulLength);
CKLEXPORT SIZE_T WINAPI EcCSGetRequestPostDataSize(CefRequest* lpRequest);
CKLEXPORT void WINAPI EcCSGetRequestPostData(CefRequest* lpRequest, BYTE* lpDataBuffer, ULONG_PTR notused);
CKLEXPORT void WINAPI EcCSSetData(BytesVector* lpData, unsigned char* lpDataBuffer, SIZE_T nSize);
CKLEXPORT void WINAPI EcCSSetMimeType(std::wstring* lpMimeType, const wchar_t* szMimeType);
CKLEXPORT void WINAPI EcCSSetStatus(int* lpStatus, int iStatus);

#endif // _EC_PACK_API_H_
