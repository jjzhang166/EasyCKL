## EasyCKL

这是一个基于CEF3（嵌入式 Chromium 框架）二次开发的库，支持 Windows 和 Linux 平台。<br>

### API 文档

可以在这里在线查看 EasyCKL 的 API 文档：http://easyckl.tk/docs/

如果上面的连接无法打开，请使用：http://easyckl.esy.es/docs/

>要是 Git@OSC 的 Pages 支持绑定域名，我就直接用 Pages 了，唉...

### 入门

请访问我们的 [Wiki页面](http://git.oschina.net/daemon_process/EasyCKL/wikis/%E3%80%90%E5%85%A5%E9%97%A8%E3%80%91%E4%BD%BF%E7%94%A8-EasyCKL-%E7%BB%99%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E5%B5%8C%E5%85%A5-%2Ahromium-%E5%86%85%E6%A0%B8web%E6%8E%A7%E4%BB%B6)

开发包获取：

http://easyckl.tk/sdk-release

http://git.oschina.net/daemon_process/EasyCKL/attach_files

### 简介

EasyCKL 致力于封装复杂的 CEF Class，呈现出 Win32API 格式的C语言风格接口（Windows）和 标准的C风格接口（Linux），便于给应用程序嵌入 Chromium 内核的 Web 控件。在 Windows 平台，EasyCKL 使用 Visual Studio 2015 编译可以获得最佳效果，Linux 平台的要求请见下文。

> 关于 Linux 版本: Linux 版本目前还处于实验性阶段，不建议在生产环境中使用

**CEF版本**

Windows (Win32, ix86): cef_binary_3.2623.1401.gb90a3be_windows32（默认） cef_binary_3.2704.1431.ge7ddb8a_windows32（可选）

Linux (i686): cef_binary_3.2623.1399.g64e2fe1_linux32

> Windows 提示：cef_binary_3.2623.1401.gb90a3be_windows32 版本是支持 Windows XP 的最后一个版本，但是我同样携带了新版（cef_binary_3.2704.1431.ge7ddb8a_windows32）的相关代码，如果你认为不需要继续支持 XP，可以自行使用新代码编译

Windows: 本项目自带了编译此库所需的CEF3 符号库(libcef.lib)，自带 libcef_dll_wrapper 的源代码（VS2015工程 + Makefile文件），CEF3 工作需要的 DLL，可以自行前往相应发布站点下载或通过源代码编译。也可以用我们提供的“开发包获取”中的地址获取。

Linux: 本项目**没有**自带了编译此库所需的 CEF3 共享库(libcef.so)，自带 libcef_dll_wrapper 的源代码（**暂时没有** Makefile文件，但以后会有），CEF3 工作需要的文件，可以自行前往相应发布站点下载或通过源代码编译。

### 如何在 Windows 平台编译？

1. **方法1** 使用 Visual Studio 2015

	用 Visual Studio 2015 打开sln工程文件，选择版本，然后依次选择“生成”->“生成解决方案”（如果有必要，可以手动迁移到低版本VS）
 
2. **方法2** 用 MSVC NMake Makefile 编译

	1. 打开 “开始菜单”->“所有程序（应用）”->“Visual Studio”->“Visual Studio 开发人员命令提示”

	3. 进入源码所在目录，编译 EasyCKL

		```
		> cd { EasyCKL源码目录 }
		> nmake
		```
 
		> 第一次编译时会自动运行 cd cef & nmake & cd .. 来编译 libcef_dll_wrapper ，但 nmake clean 不会清理 libcef_dll_wrapper 的编译产生的目标文件，欲删除这些目标文件，使用以下命令

		>```
		>> cd cef
		>> nmake clean
		>```
 
	4. 注意，如果更换了CEF版本或增加、删除了源码文件，需要改动 Makefile 脚本。另外，我提供了 getobjs.sh 脚本帮助你直接生成需要的所有目标文件列表，用于快速编写 Makefile。这个脚本是一个 Shell 脚本，在 Windows 环境运行需要 MSYS 环境或 Cygwin 环境。

### 如何在 Linux 平台编译、使用？(**实验性**)


1. 安装依赖包 libx11-dev libgtk2.0-dev libgtk-3-dev libgtkglext1-dev

2. 安装 GNU 编译器套件、cmake 等必要软件

3. 下载 cef_binary_3.2623.1399.g64e2fe1_linux32 并使用 cmake 创建 Makefile

4. 编译得到 libcef_dll_wrapper.a 将之和 libcef.so 复制到 EasyCKL 的 lib_linux 目录中

5. 在 EasyCKL 目录进行 make，编译得到 libEasyCKL.so

6. 使用以下命令编译使用 EasyCKL 的应用程序 (如 demo/1.cpp)

	```
	$ g++ -o [目标] [源文件名] -std=c++11 -lEasyCKL -lcef -lX11 `pkg-config --cflags --libs gtk+-3.0` -L. -Wl,-rpath=.
	```

	> 注意：支持 GTK+3.0 不支持 2.0 

7. （如果无法正常运行，则）在运行之前设置 LD_PRELOAD 环境变量为 libcef.so

### 鸣谢

Chromium Embedded Framework

New BSD