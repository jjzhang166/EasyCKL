## EasyCKL

[简体中文](README.md.zh_CN.UTF-8)

EasyCKL is a library based on CEF3 (embedded Chromium framework) secondary development, support for Windows and Linux platforms.

[API documentation](http://easyckl.tk/docs/) | [Build EasyCKL from source](INSTALL.md) | [Getting Started Tutorial（MSVC）](https://git.oschina.net/daemon_process/EasyCKL/wikis/%E3%80%90%E5%85%A5%E9%97%A8%E3%80%91%E4%BD%BF%E7%94%A8-EasyCKL-%E7%BB%99%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E5%B5%8C%E5%85%A5-%2Ahromium-%E5%86%85%E6%A0%B8web%E6%8E%A7%E4%BB%B6) | [Getting Started Tutorial（MinGW）](https://git.oschina.net/daemon_process/EasyCKL/wikis/%E4%BD%BF%E7%94%A8-MinGW-%E5%BC%80%E5%8F%91-EasyCKL-%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F) | [Project News](NEWS)

If the above API documentation can not be opened, use the [This Connect](http://easyckl.esy.es/docs/)

For the meaning of the cause of EasyCKL development please refer to [Wiki page](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

**In short, EasyCKL has many excellent features**

- Completely with C-style API interface, please refer to our specific [Wiki page](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

- As far as possible follow the principle of separation of mechanisms and strategies do not impose any strategy, please refer to our specific [Wiki page](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

- Simple and easy to use, lightweight, he put the CEF complex class, cumbersome inheritance and abstraction hidden, leaving the caller is like Win32API, Linux system calls, POSIX library functions as concise and powerful interface.

- He ensures backwards compatibility as much as possible, either at the source or binary level.

**With EasyCKL, you can**

- To your application embedded in a high-performance, compatible with the latest standards of mainstream Web controls

- Make browsers that based Chromium kernel, such as [ECKBrowser] (http://easyckl.tk/eckbrowser/)

- Hosting applications primarily developed with Web technologies (WebUI)

**How to get started?**

I would like to develop my application based on EasyCKL - Go back to the top Getting Started Tutorials and API Documentation

I would like to develop directly on the EasyCKL source - go to the Wiki page about compiling EasyCKL. [Wiki page](INSTALL.md)

**Note** If you are developing on the Linux platform, it is recommended that you build EasyCKL via source code, because different compiler versions, different glibc versions, and different Linux distributions environments can cause binary compatibility problems. In addition, the direct release of the source code on Linux is also the most acceptable form of user acceptance.

[Chromium Embedded Framework](https://bitbucket.org/chromiumembedded/cef)

BSD LICENSE

Windows (Win32, ix86): cef_binary_3.2623.1401.gb90a3be_windows32（默认） cef_binary_3.2704.1431.ge7ddb8a_windows32（可选）

Linux (i686): cef_binary_3.2623.1399.g64e2fe1_linux32

>Windows Tip: cef_binary_3.2623.1401.gb90a3be_windows32 version is to support the last version of Windows XP, but I also carry a new version (cef_binary_3.2704.1431.ge7ddb8a_windows32) code, if you do not need to continue to support XP, you can use the new code to compile

Note: This project publishes the source code, README, Wiki, and API documentation under BSD LICENSE