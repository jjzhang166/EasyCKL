### EasyCKL SubProcess 程序

这是一个 EasyCKL 的可选组件，当多进程工作时，你可以直接使用 SubProcess 程序做后台渲染进程，默认情况下是进程 Fork 自身。

使用说明，在初始化时添加 `INITFLAG_SETSUBPROCESS` 标志，设置 `INIT_EXTDATA.szSubProcess` 为 SubProcess 程序路径

### 如何编译？(Win32)

用 MSVC NMake Makefile 编译

	1. 打开 “开始菜单”->“所有程序（应用）”->“Visual Studio”->“Visual Studio 开发人员命令提示”

	3. 进入源码所在目录，编译 EasyCKL 的 subprocess 组件

		```
		> cd { EasyCKL源码目录 }\subprocess
		> nmake
		```
 
		> 第一次编译时会自动运行 `cd ..\cef & nmake & cd ..\subprocess` 来编译 libcef_dll_wrapper ，但 nmake clean 不会清理 libcef_dll_wrapper 的编译产生的目标文件，欲删除这些目标文件，使用以下命令

		>```
		>> cd ..\cef
		>> nmake clean
		>```