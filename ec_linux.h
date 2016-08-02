#ifndef __EC_LINUX_H_
#define __EC_LINUX_H_

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
	int left;
	int top;
	int right;
	int bottom;
}RECT,*LPRECT;
#else
#define HWND CefWindowHandle
#define RECT CefRect
#endif // __EC_LINUX_API

typedef struct tagECMAINARGS {
	int iArgc;
	char** pszArgv;
}ECMAINARGS,*LPECMAINARGS;

#define HINSTANCE LPECMAINARGS

#endif // __EC_LINUX_H_
