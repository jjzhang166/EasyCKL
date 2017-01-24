> 注意：从版本 7.1 开始，供调用 EasyCKL 的应用程序使用的 EasyCKL.h 不再杂乱的和 EasyCKL 源代码一起维护，改为用脚本自动生成，脚本为 mkheader.sh，直接调用即可。

> 也就是说，这个脚本自动生成一个“经过消毒的”头文件，以供调用 EasyCKL 的应用程序使用（EasyCKL 内部头文件包含了 EasyCKL 库的内部接口、宏等，不适合应用程序直接使用）。

> 因此，即使在 Windows 环境下编译也必须依赖于 MinGW 或 Cygwin（除非你不要 EasyCKL.h 了，或者使用我们合成好的，但那样就意味着如果改动 EasyCKL 的接口不能自动生成该头文件了）。

Windows: 本项目自带了编译此库所需的CEF3 符号库(libcef.lib)，自带 libcef_dll_wrapper 的源代码（VS2015工程 + Makefile文件），CEF3 工作需要的运行时环境，可以使用我们提供的“开发包获取”地址获取，也可以自行前往相应发布站点下载或通过源代码编译。

Linux: 本项目暂时**没有**自带编译此库所需的 CEF3 共享库(libcef.so)，自带 libcef_dll_wrapper 的源代码（**暂时没有** Makefile文件，但以后会有），CEF3 工作需要的文件，可以自行前往相应发布站点下载或通过源代码编译。，CEF3 工作需要的运行时环境（包含编译此库所需的 CEF3 共享库(libcef.so)），可以使用我们提供的“开发包获取”（见上文）中的地址获取，也可以自行前往相应发布站点下载或通过源代码编译。

## 编译 EasyCKL.dll/EasyCKL.so 

### 在 Windows 平台编译

1. **方法1** 使用 Visual Studio 2015

	用 Visual Studio 2015 打开sln工程文件，选择版本，然后依次选择“生成”->“生成解决方案”（如果有必要，可以手动迁移到低版本VS）
 
2. **方法2** 用 MSVC NMake Makefile 编译

	1. 打开 “开始菜单”->“所有程序（应用）”->“Visual Studio”->“Visual Studio 开发人员命令提示” (从理论上讲，支持 C++11 的任何一个 MSVC 版本都应该能正确编译，但我的环境是 Visual Studio 2015)

	3. 进入源码所在目录，键入`nmake`编译 EasyCKL
 
		> 第一次编译时会自动运行 cd cef & nmake & cd .. 来编译 libcef_dll_wrapper ，但 nmake clean 不会清理 libcef_dll_wrapper 的编译产生的目标文件，欲删除这些目标文件，使用以下命令

		```
		 cd cef
		 nmake clean
		```
 
	4. 注意，如果更换了CEF版本或增加、删除了源码文件，需要改动 Makefile 脚本。另外，我提供了 getobjs.sh 脚本帮助你直接生成需要的所有目标文件列表，用于快速编写 Makefile。这个脚本是一个 Shell 脚本，在 Windows 环境运行需要 MSYS 环境或 Cygwin 环境。

### 在 Linux 平台编译、使用 (实验性)

1. 安装依赖包 libx11-dev libgtk2.0-dev libgtk-3-dev libgtkglext1-dev

    以 Debian 家族为例
    
    >$ sudo apt-get install libx11-dev libgtk2.0-dev libgtk-3-dev libgtkglext1-dev

2. 安装 GNU 编译器套件、cmake 等必要软件

3. 下载 cef_binary_3.2623.1399.g64e2fe1_linux32 并使用 cmake 创建 Makefile

4. 编译得到 libcef_dll_wrapper.a 将之和 libcef.so 复制到 EasyCKL 的 lib_linux 目录中

5. 在 EasyCKL 目录进行 make，编译得到 libEasyCKL.so

6. 可以使用类似于以下命令编译使用 EasyCKL 的应用程序 (如 demo/1.cpp)

	```
	$ g++ -o [目标] [源文件名] -std=c++11 -lEasyCKL -lcef -lX11 `pkg-config --cflags --libs gtk+-3.0` -L. -Wl,-rpath=.
	```

	> 注意：支持 GTK+3.0 不支持 2.0 

7. （如果无法正常运行，则）在运行之前设置 LD_PRELOAD 环境变量为 libcef.so

## 生成 EasyCKL.h

```
$ cd path-to-EasyCKL-src
$ ./mkheader.sh
```

If you want to compare the EasyCKL.h which is make by this script with native one, you can use `./mkheader.sh -d` to run script in debug mode, it will create EasyCKL.h.native and EasyCKL.h.diff to help you check and get what you want. :-)