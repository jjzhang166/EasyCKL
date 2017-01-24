## EasyCKL

[简体中文](README.md.zh_CN.UTF-8)

EasyCKL is a library based on CEF3 (embedded Chromium framework) secondary development, support for Windows and Linux platforms.

[API documentation](http://easyckl.tk/docs/) | [Build EasyCKL from source](INSTALL.md) | [Getting Started Tutorial（MSVC）](https://git.oschina.net/daemon_process/EasyCKL/wikis/%E3%80%90%E5%85%A5%E9%97%A8%E3%80%91%E4%BD%BF%E7%94%A8-EasyCKL-%E7%BB%99%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E5%B5%8C%E5%85%A5-%2Ahromium-%E5%86%85%E6%A0%B8web%E6%8E%A7%E4%BB%B6) | [Getting Started Tutorial（MinGW）](https://git.oschina.net/daemon_process/EasyCKL/wikis/%E4%BD%BF%E7%94%A8-MinGW-%E5%BC%80%E5%8F%91-EasyCKL-%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F) | [Project News](NEWS)

If the above API documentation can not be opened, use the [This Connect](http://easyckl.esy.es/docs/)

For the meaning of the cause of EasyCKL development please refer to [Wiki page](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

** In short, EasyCKL has many excellent features: **

- Completely with C-style API interface, please refer to our specific [Wiki page](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

- As far as possible follow the principle of separation of mechanisms and strategies do not impose any strategy, please refer to our specific [Wiki page](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

- Simple and easy to use, lightweight, he put the CEF complex class, cumbersome inheritance and abstraction hidden, leaving the caller is like Win32API, Linux system calls, POSIX library functions as concise and powerful interface.

- He ensures backwards compatibility as much as possible, either at the source or binary level.

** With EasyCKL, you can **

- To your application embedded in a high-performance, compatible with the latest standards of mainstream Web controls

- Make browsers that based Chromium kernel, such as [ECKBrowser] (http://easyckl.tk/eckbrowser/)

- Hosting applications primarily developed with Web technologies (WebUI)

**如何开始？**

我希望基于 EasyCKL 开发我的应用程序——请回到顶部查看入门教程和 API 文档

我希望直接在 EasyCKL 源码上开发——请转至关于编译 EasyCKL 的 [Wiki 页面](https://git.oschina.net/daemon_process/EasyCKL/wikis/%E4%BB%8E%E6%BA%90%E4%BB%A3%E7%A0%81%E6%9E%84%E5%BB%BA-EasyCKL)

**注意** 如果你在 Linux 平台开发，建议你通过源代码构建 EasyCKL，这是由于不同的编译器版本、不同的 glibc 版本和不同的发行版环境会对二进制的兼容性带来麻烦。另外，在 Linux 上直接发布源代码也是最能受用户接受的形式。

[Chromium Embedded Framework](https://bitbucket.org/chromiumembedded/cef)

BSD LICENSE

Windows (Win32, ix86): cef_binary_3.2623.1401.gb90a3be_windows32（默认） cef_binary_3.2704.1431.ge7ddb8a_windows32（可选）

Linux (i686): cef_binary_3.2623.1399.g64e2fe1_linux32

> Windows 提示：cef_binary_3.2623.1401.gb90a3be_windows32 版本是支持 Windows XP 的最后一个版本，但是我同样携带了新版（cef_binary_3.2704.1431.ge7ddb8a_windows32）的相关代码，如果你认为不需要继续支持 XP，可以自行使用新代码编译

注：本项目在 BSD LICENSE 下发布源代码、README、Wiki 和 API 文档