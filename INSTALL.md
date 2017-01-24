> Note: Starting with version 7.1, EasyCKL.h for EasyCKL applications is no longer cluttered with the EasyCKL source code, and is automatically generated with a script, called mkheader.sh, which can be called directly. In other words, this script automatically generates a "sanitized" header file for use by applications that call EasyCKL. (The EasyCKL internal header file contains the internal interfaces, macros, etc. of the EasyCKL library and is not intended for direct use by the application.) Therefore, even compilation in Windows environment must rely on MinGW or Cygwin (unless you do not use EasyCKL.h, or use our synthesis, but that means that if the change EasyCKL interface can not automatically generate the header file).

In the Windows platform: This project comes with the need to compile this library CEF3 symbol library (libcef.lib), comes with libcef_dll_wrapper source code (VS2015 project + Makefile file), CEF3 work required runtime environment, you can use our offer Of the "Development Kit Get" address to obtain, you can also go to the corresponding release site to download or compile the source code.

In the Linux platform: This project is **not** have CEF3 shared library required to compile this library (libcef.so), comes with libcef_dll_wrapper source code (**not** Makefile file, but there will be) , CEF3 work needs of the file, you can go to the corresponding release site to download or compile the source code. The runtime environment required for CEF3 work (including the CEF3 shared library (libcef.so) required to compile this library) can be obtained using the address provided in the "Development Kit Get" (see above), or by yourself The corresponding release site to download or compile through the source code.

## Compile EasyCKL Shared Library (EasyCKL.dll or libEasyCKL.so) 

### On Windows platform

1. **Method 1** By Visual Studio 2015

	Use Visual Studio 2015 to open the sln project file, select the version, then choose Build -> Build Solution (you can manually migrate to the lower version if necessary)
 
2. **Method 2** By MSVC NMake Makefile

	1. Open the "Start menu" -> "All Programs (Applications)" -> "Visual Studio" -> "Visual Studio Developer Command Prompt" (in theory, any version of MSVC that supports C ++ 11 should work correctly, but my environment is Visual Studio 2015)

	2. Enter the directory where the source code live, type `nmake` to compile EasyCKL
 
		> In the first compile time, it will automatically run `cd cef & nmake & cd ..` to compile libcef_dll_wrapper, but `nmake clean` will not clean up target files of libcef_dll_wrapper. If you want to delete these files, use the following command

		>```
		> cd cef
		> nmake clean
		>```
 
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