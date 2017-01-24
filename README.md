## EasyCKL

EasyCKL 是一个基于 CEF3（嵌入式 Chromium 框架）二次开发的库，支持 Windows 和 Linux 平台。

[API 文档](http://easyckl.tk/docs/) | [从源代码构建 EasyCKL](https://git.oschina.net/daemon_process/EasyCKL/wikis/%E4%BB%8E%E6%BA%90%E4%BB%A3%E7%A0%81%E6%9E%84%E5%BB%BA-EasyCKL) | [入门教程（MSVC）](https://git.oschina.net/daemon_process/EasyCKL/wikis/%E3%80%90%E5%85%A5%E9%97%A8%E3%80%91%E4%BD%BF%E7%94%A8-EasyCKL-%E7%BB%99%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E5%B5%8C%E5%85%A5-%2Ahromium-%E5%86%85%E6%A0%B8web%E6%8E%A7%E4%BB%B6) | [入门教程（MinGW）](https://git.oschina.net/daemon_process/EasyCKL/wikis/%E4%BD%BF%E7%94%A8-MinGW-%E5%BC%80%E5%8F%91-EasyCKL-%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F) | [项目新闻](NEWS)

如果上面的 API 文档无法打开，请使用 [这个连接](http://easyckl.esy.es/docs/)

EasyCKL 开发的起因的意义请参考 [Wiki 页面](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

**简而言之，EasyCKL 具有很多优秀的特点：**

- 完全适用 C 风格的 API 接口，具体请参考我们的 [Wiki 页面](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

- 尽可能遵循机制与策略相分离的原则，不强加任何策略，具体请参考我们的 [Wiki 页面](https://git.oschina.net/daemon_process/EasyCKL/wikis/Home)

- 简洁易用，轻量，他把 CEF 冗杂的类、繁琐的继承和抽象隐藏起来，留给调用者的是像 Win32API、Linux系统调用、POSIX库函数一样简洁有力的接口。

- 他尽可能的保证向后兼容，不论是源代码级还是二进制级。

**利用 EasyCKL，你可以**

- 给你的应用程序嵌入一个高性能、兼容主流最新标准的 Web 控件

- 开发 Chromium 内核的浏览器，例如 [ECKBrowser](http://easyckl.tk/eckbrowser/)

- 托管主要以 Web 技术开发的应用程序（WebUI）

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