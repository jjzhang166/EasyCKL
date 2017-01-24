> 注意：从版本 7.1 开始，供调用 EasyCKL 的应用程序使用的 EasyCKL.h 不再杂乱的和 EasyCKL 源代码一起维护，改为用脚本自动生成，脚本为 mkheader.sh，直接调用即可。

> 也就是说，这个脚本自动生成一个“经过消毒的”头文件，以供调用 EasyCKL 的应用程序使用（EasyCKL 内部头文件包含了 EasyCKL 库的内部接口、宏等，不适合应用程序直接使用）。

> 因此，即使在 Windows 环境下编译也必须依赖于 MinGW 或 Cygwin（除非你不要 EasyCKL.h 了，或者使用我们合成好的，但那样就意味着如果改动 EasyCKL 的接口不能自动生成该头文件了）。

Windows: 本项目自带了编译此库所需的CEF3 符号库(libcef.lib)，自带 libcef_dll_wrapper 的源代码（VS2015工程 + Makefile文件），CEF3 工作需要的运行时环境，可以使用我们提供的“开发包获取”地址获取，也可以自行前往相应发布站点下载或通过源代码编译。

Linux: 本项目暂时**没有**自带编译此库所需的 CEF3 共享库(libcef.so)，自带 libcef_dll_wrapper 的源代码（**暂时没有** Makefile文件，但以后会有），CEF3 工作需要的文件，可以自行前往相应发布站点下载或通过源代码编译。，CEF3 工作需要的运行时环境（包含编译此库所需的 CEF3 共享库(libcef.so)），可以使用我们提供的“开发包获取”（见上文）中的地址获取，也可以自行前往相应发布站点下载或通过源代码编译。

## Compile EasyCKL.dll/EasyCKL.so 

### On Windows platform

1. **Method 1** By Visual Studio 2015

	Use Visual Studio 2015 to open the sln project file, select the version, then choose Build -> Build Solution (you can manually migrate to the lower version if necessary)
 
2. **Method 2** By MSVC NMake Makefile

	1. Open the "Start menu" -> "All Programs (Applications)" -> "Visual Studio" -> "Visual Studio Developer Command Prompt" (in theory, any version of MSVC that supports C ++ 11 should work correctly, but my environment is Visual Studio 2015)

	2. Enter the directory where the source code live, type `nmake` to compile EasyCKL
 
		> In the first compile time, it will automatically run `cd cef & nmake & cd ..` to compile libcef_dll_wrapper, but `nmake clean` will not clean up target files of libcef_dll_wrapper. To delete these files, use the following command

		```
		 cd cef
		 nmake clean
		```
 
	3. Note: If you replace the CEF version or add, delete the source file, you need to change the Makefile script. In addition, I provide a getobjs.sh script to help you generate a direct need for all the target file list for the rapid preparation of Makefile. This script is a shell script that runs in a Windows environment that requires the MSYS environment or the Cygwin environment.

### On Linux platform compile and use (Experimental)

1. Install dependencies: libx11-dev libgtk2.0-dev libgtk-3-dev libgtkglext1-dev

    Take the Debian family as an example
    
	```
    $ sudo apt-get install libx11-dev libgtk2.0-dev libgtk-3-dev libgtkglext1-dev
	```

2. Install GNU Compiler Suite, cmake and other necessary software

3. Download cef_binary_3.2623.1399.g64e2fe1_linux32 And create a Makefile using cmake

4. Compile to get libcef_dll_wrapper.a. Copy it and libcef.so to EasyCKL's lib_linux directory

5. In the EasyCKL directory, run `make`, compile to get libEasyCKL.so

	Note: You must use the Free Software Foundation's GCC Compiler Suite

6. You can compile an application that uses EasyCKL, such as demo/1.cpp, with a command similar to the following:

	```
	$ g++ -o [Target] [Source] -std=c++11 -lEasyCKL -lcef -lX11 `pkg-config --cflags --libs gtk+-3.0` -L. -Wl,-rpath=.
	```

	> Note: Support GTK + 3.0 but does not support 2.0

7. (If it does not work, then) before running set the LD_PRELOAD environment variable to libcef.so

## Generate EasyCKL.h

When you clone the project in the first time on Linux, you should add executable permission to the mkheader.sh script.

```
$ chmod u+x mkheader.sh
```
On Windows, it is not necessary. But you must have **MinGW** or **Cygwin** or aother similar environment to continue.(I use MinGW on windows, but Cygwin should be no problem.)

```
$ cd path-to-EasyCKL-src
$ ./mkheader.sh
```

If you want to compare the EasyCKL.h which is make by this script with native one, you can use `./mkheader.sh -d` to run script in debug mode, it will create EasyCKL.h.native and EasyCKL.h.diff to help you check and get what you want. :-)