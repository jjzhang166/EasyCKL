#define __EC_LINUX_API
#include "EasyCKL.h"

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include <stdio.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

void* lpBrowser = 0;

// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

void DeleteWindow(GtkWidget *widget, gpointer data) {
    gtk_widget_hide_on_delete(widget);
    //gtk_main_quit();
    Chrome_QuitMessageLoop();
}

void CallBack_ChTitle(LONG_PTR dwId, const wchar_t* szTitle){
    //例子：修改web控件窗口标题

    std::wstring string_to_convert(szTitle);
    Display* lpDisplay = XOpenDisplay(NULL);
    XStoreName(lpDisplay, (Window)Chrome_GetWindowHandle(lpBrowser), (wstring_to_utf8(string_to_convert)).c_str());
    XFlush (lpDisplay);
}

int main(int argc, char *argv[])
{
    ECMAINARGS MainArgs;
    MainArgs.iArgc = argc;
    MainArgs.pszArgv = argv;
    Chrome_InitializeEx(&MainArgs, INITFLAG_CACHESTORAGE, 0, L"zh-CN", 0);

    Chrome_EnableCookieStorage();
    if (!Chrome_IsUIThread())
        return 0;

    GtkWidget* lpWindow;

    gtk_init(&argc, &argv);
    
    lpWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(lpWindow), "EasyCKL For Linux Caller");
    g_signal_connect(G_OBJECT(lpWindow), "delete_event", G_CALLBACK(DeleteWindow), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(lpWindow), 0);
    gtk_widget_set_size_request(GTK_WIDGET(lpWindow), 600, 400);
    gtk_window_set_position(GTK_WINDOW(lpWindow), GTK_WIN_POS_CENTER);

    BROWSER_CALLBACKS lpCallbacks = {0};
    lpCallbacks.cbSize = sizeof(BROWSER_CALLBACKS);
    lpCallbacks.chtitle_callback = CallBack_ChTitle;
    RECT rcBrowser;
    rcBrowser.x = 0; rcBrowser.y = 0;
    rcBrowser.width = 600; rcBrowser.height = 400;
    lpBrowser = Chrome_CreateChildBrowser(BROWSERFLAG_SYNC, &lpCallbacks, 0, 0, L"https://www.google.com/ncr", 
    0, &rcBrowser, 0);

    gtk_widget_show(lpWindow);

    //gtk_main();
    Chrome_MessageLoop();
    Chrome_Shutdown();
    return 0;
}