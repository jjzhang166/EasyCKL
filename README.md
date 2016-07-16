## EasyCKL

这是一个基于CEF3（嵌入式 Chromium 框架）二次开发的库，使用BSD许可证开源。<br>

### API 文档

可以在这里在线查看 EasyCKL 的 API 文档：http://easyckl.tk/docs/

如果上面的连接无法打开，请使用：http://easyckl.esy.es/docs/

### 入门

访问我们的 [Wiki页面](http://git.oschina.net/daemon_process/EasyCKL/wikis/%E3%80%90%E5%85%A5%E9%97%A8%E3%80%91%E4%BD%BF%E7%94%A8-EasyCKL-%E7%BB%99%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F%E5%B5%8C%E5%85%A5-%2Ahromium-%E5%86%85%E6%A0%B8web%E6%8E%A7%E4%BB%B6) 得到入门文档

开发包获取：

http://easyckl.tk/sdk-release

http://git.oschina.net/daemon_process/EasyCKL/attach_files

### 简介

EasyCKL 致力于封装复杂的 CEF Class，呈现给用户 Win32API 格式的C语言风格接口，便于给程序嵌入 Chromium 内核的web控件

EasyCKL 在cefsimple 的基础上进行开发，是一个 Visual Studio 2015 工程。CEF的版本采用 cef_binary_3.2623.1393_windows32 理论上兼容更新的版本。

自带了编译此库所需的CEF3 符号Lib (libcef.lib)，自带 libcef_dll_wrapper 的源代码（VS2015工程），CEF3 工作需要的DLL，可以自行前往相应发布站点获取或通过源代码编译。也可以用我们提供的“开发包获取”中的地址获取。

### 如何编译？

1. **方法1** 使用 Visual Studio 2015

	用 Visual Studio 2015 打开sln工程文件，选择版本，然后依次选择“生成”->“生成解决方案”（如果有必要，可以手动迁移到低版本VS）

2. **方法2** 用 MSVC NMake Makefile 编译

	1. 打开 “开始菜单”->“所有程序（应用）”->“Visual Studio”->“Visual Studio 开发人员命令提示”

	2. 编译 libcef_dll_wrapper

		```
		> cd ./cef
		
		> nmake
		```

		将编译后的 libcef_dll_wrapper.lib 移动到 out 文件夹

	3. 编译 EasyCKL

		```
		> cd ..
	
		> nmake
		```
	
	4. 注意，如果更换了CEF版本或改动了源码文件，需要改动 Makefile 脚本，另外，提供了 getobjs.sh 脚本帮助你直接生成需要的所有目标文件列表，用于编写 Makefile。这个脚本是一个 Shell 脚本，在 Windows 环境运行需要 MSYS 环境或 Cygwin 环境。

 
3. **方法3** 使用 MinGW 编译

	此编译方法请期待新版本

### 鸣谢

Chromium Embedded Framework

New BSD