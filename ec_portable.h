#ifndef __EC_PORTABLE_H_
#define __EC_PORTABLE_H_

#ifdef CKLEXPORT
#undef CKLEXPORT
#endif

#define CKLEXPORT extern "C"
/*#ifdef _WIN32
#define CKLEXPORT extern "C" __declspec(dllexport)
#elif defined __linux__
#define CKLEXPORT extern "C"
#endif*/

#ifdef _WIN32
#include <Windows.h>
#elif defined __linux__
#include <X11/Xlib.h>
#include <stddef.h>

typedef unsigned int DWORD;
typedef bool BOOL;
typedef void* LPVOID;
typedef void* PVOID;
typedef size_t SIZE_T;
typedef long LONG;
typedef unsigned long LONG_PTR;
typedef LONG_PTR UINT_PTR;
typedef LONG_PTR ULONG_PTR;
typedef char BYTE;
typedef unsigned int UINT;
typedef unsigned long ULONG;

#define WINAPI
#define TRUE true
#define FALSE false

#ifdef __EC_LINUX_API
#define HWND void*
typedef struct tagRECT {
	int x;
	int y;
	int width;
	int height;
}RECT,*LPRECT;
#else
#define HWND CefWindowHandle
#define RECT _cef_rect_t
#endif // __EC_LINUX_API

typedef struct tagECMAINARGS {
	int iArgc;
	char** pszArgv;
}ECMAINARGS,*LPECMAINARGS;

#define HINSTANCE LPECMAINARGS

//#define wchar_t char
//#define std::wstring std::string

#endif
#endif // __EC_PORTABLE_H_