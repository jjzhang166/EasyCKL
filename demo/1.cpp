#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#define __EC_LINUX_API
#include "EasyCKL.h"
#include <stdio.h>

void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc,char *argv[])
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
    g_signal_connect(G_OBJECT(lpWindow), "delete_event", G_CALLBACK(destroy), NULL);

    gtk_container_set_border_width(GTK_CONTAINER(lpWindow), 0);
    gtk_widget_set_size_request(GTK_WIDGET(lpWindow), 600, 400);
    gtk_window_set_position(GTK_WINDOW(lpWindow), GTK_WIN_POS_CENTER);

    BROWSER_CALLBACKS lpCallbacks = {0};
    lpCallbacks.cbSize = sizeof(BROWSER_CALLBACKS);
    RECT rcBrowser;
    rcBrowser.left = 0; rcBrowser.top = 0;
    rcBrowser.right = 100; rcBrowser.bottom = 100;
    Chrome_CreateChildBrowser(BROWSERFLAG_SYNC, &lpCallbacks, 0, 0, L"https://www.baidu.com", 
    lpWindow, &rcBrowser, 0);

printf("aaaaaa\n");

    gtk_widget_show(lpWindow);
    
    //gtk_main();
    Chrome_MessageLoop();
    Chrome_Shutdown();

    return 0;
}