﻿##EasyCKL

###通知：
已经更新到了2015的Lib，**现在仓库里有很多暂时用不到的，打算未来还是用VS的IDE，所以保留着**，但目前依旧采用Makefile编译，编译方法如下：<br>

1.打开VS2015命令提示符：<br>
![1](http://git.oschina.net/daemon_process/EasyCKL/raw/master/pic/1.png)<br>
2.切换到源码所在目录：<br>
![2](http://git.oschina.net/daemon_process/EasyCKL/raw/master/pic/2.png)<br>
3.输入nmake命令回车：<br>
![3](http://git.oschina.net/daemon_process/EasyCKL/raw/master/pic/3.png)<br>
（如果增加了源代码文件，需要改动makefile脚本）<br>

这是一个基于CEF3（嵌入式 Chromium 框架）二次开发的库，使用BSD许可证开源。<br>
<br>
~~CEF3是基于C++的，大量使用了Class，这意味着C、易语言不能很方便地使用，此库是作为一个“中间层”将复杂的Class隐藏，从而给上层提供使用WINAPI调用约定的接口，这就意味着这些函数能够很方便地被C、易语言所调用，摆脱了class的烦恼，体验和Win32API编程一样的感觉。（当然我并不是说Class不好）~~<br>
这段话不正确，CEF3本来是用C做的接口，为了方便又用C++包了一遍，EasyCKL是又包了一遍。<br>
<br>
这是在cefsimple的基础上进行开发的，这是一个Visual Studio 2013工程（现在已经升级到了2015）。CEF的版本是 cef_binary_3.2062.1930_windows32 理论上兼容更新的版本。<br>
<br>
自带了编译此库所需的CEF3 Lib (libcef.lib)，libcef_dll_wrapper.lib现在已经改为使用源码形式给出，但是没有带CEF3工作的DLL，可以下载CEF3源码编译得到。<br>
<br>
我写的代码逻辑并不明了，注释并不丰富，性能并不好，如果不是有人总是鼓动我做我也就不打算发了，所以请各路前辈轻喷。（如果路过看到的话）<br>
<br>
另外这是我第一次用Git。。。<br>