//请参考：http://git.oschina.net/daemon_process/EasyCKL/wikis/%E3%80%90%E5%85%A5%E9%97%A8%E3%80%91%E4%BD%BF%E7%94%A8-EasyCKL-%E7%BB%99%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E5%B5%8C%E5%85%A5-*hromium-%E5%86%85%E6%A0%B8web%E6%8E%A7%E4%BB%B6

#include "stdafx.h"
#include "Resource.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

static TCHAR* szWindowClass = TEXT("MyECKLApplication");
HWND hWnd;
void* browser;

void WINAPI CallBack_ChTitle(LONG_PTR dwId, const wchar_t* szTitle) {
    SetWindowTextW(hWnd, szTitle);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Chrome_InitializeEx(hInstance, INITFLAG_CACHESTORAGE | INITFLAG_ENABLEHIGHDPISUPPORT, 0, L"zh-CN", 0);
    Chrome_EnableCookieStorage();

    if (!Chrome_IsUIThread()) return 0;

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassEx(&wcex);

    hWnd = CreateWindow(szWindowClass, L"ECKL web控件测试", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 700, 480, NULL, NULL, hInstance, NULL);

    if (!hWnd)
        return FALSE;

    BROWSER_CALLBACKS BrowserCallbacks = { 0 };
    BrowserCallbacks.cbSize = sizeof(BROWSER_CALLBACKS);
    BrowserCallbacks.chtitle_callback = CallBack_ChTitle;

    RECT rcWindowClient, rcWebControl;
    GetClientRect(hWnd, &rcWindowClient); //得到窗口的客户区矩形
    rcWebControl.top = rcWebControl.left = 0;
    rcWebControl.bottom = rcWindowClient.bottom;
    rcWebControl.right = rcWindowClient.right;

    browser = Chrome_CreateChildBrowser(BROWSERFLAG_SYNC, &BrowserCallbacks, 0, 0, L"https://www.google.com.hk", hWnd, &rcWebControl, 0);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    Chrome_MessageLoop();
    Chrome_Shutdown();
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        MoveWindow(Chrome_GetWindowHandle(browser), 0, 0, LOWORD(lParam), HIWORD(lParam), FALSE);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}