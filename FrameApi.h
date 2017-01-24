#ifndef _FRAME_API_H_
#define _FRAME_API_H_
#include "ec_portable.h"
#include "CKLMain.h"

#include "simple_app.h"
#include "simple_handler.h"

CKLEXPORT void WINAPI Chrome_FrameLoadUrl(CefFrame* frame, wchar_t* url);
CKLEXPORT void WINAPI Chrome_FrameLoadString(CefFrame* frame, wchar_t* string, wchar_t* url);
CKLEXPORT bool WINAPI Chrome_FrameIsMain(CefFrame* frame);
CKLEXPORT void WINAPI Chrome_ReleaseFrame(CefFrame* frame);
CKLEXPORT CefFrame* WINAPI Chrome_GetNameFrame(SimpleHandler* lpBrowser, wchar_t* name);
CKLEXPORT CefFrame* WINAPI EcQBIGetMainFrame(SimpleHandler* lpBrowser);
CKLEXPORT wchar_t* WINAPI Chrome_FrameGetUrl(CefFrame* lpFrame);
CKLEXPORT void WINAPI Chrome_FrameDoCopy(CefFrame* frame);
CKLEXPORT void WINAPI Chrome_FrameDoCut(CefFrame* frame);
CKLEXPORT void WINAPI Chrome_FrameDoDelete(CefFrame* frame);
CKLEXPORT void WINAPI Chrome_FrameDoPaste(CefFrame* frame);
CKLEXPORT void WINAPI Chrome_FrameDoSelectAll(CefFrame* frame);

#endif // _FRAME_API_H_

