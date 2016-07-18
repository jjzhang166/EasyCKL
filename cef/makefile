
# Copyright daemon_process

INCLUDE = .\;$(INCLUDE)

FLAGS = /c /MP /GS /TP /analyze- /W4 /wd"4100" /wd"4127" /wd"4244" /wd"4481" /wd"4512" /wd"4701" /wd"4702" /wd"4996" /Gy /Zc:wchar_t /Zi /Gm- /O2 /Ob2 /Zc:inline /fp:precise /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "__STDC_CONSTANT_MACROS" /D "__STDC_FORMAT_MACROS" /D "_WIN32" /D "UNICODE" /D "_UNICODE" /D "WINVER=0x0602" /D "_WIN32_WINNT=0x602" /D "NOMINMAX" /D "WIN32_LEAN_AND_MEAN" /D "_HAS_EXCEPTIONS=0" /D "PSAPI_VERSION=1" /D "CEF_USE_SANDBOX" /D "CEF_USE_ATL" /D "_NDEBUG" /D "USING_CEF_SHARED" /errorReport:prompt /GF /WX /Zc:forScope /GR- /Gd /Oy- /MT /EHsc /nologo /Fd:".\out\temp.pdb"
	
OBJS = 

libcef_dll_wrapper.lib : default $(OBJS)
	lib /nologo out\*.obj /out:"..\out\libcef_dll_wrapper.lib" /export:"..\out\libcef_dll_wrapper.lib.pdb"

{.\libcef_dll\base\}.cc.obj:
	@cl $(FLAGS) $< /Fo:out\$@

{.\libcef_dll\cpptoc\}.cc.obj:
	@cl $(FLAGS) $< /Fo:out\$@

{.\libcef_dll\ctocpp\}.cc.obj:
	@cl $(FLAGS) $< /Fo:out\$@

{.\libcef_dll\wrapper\}.cc.obj:
	@cl $(FLAGS) $< /Fo:out\$@

{.\libcef_dll\}.cc.obj:
	@cl $(FLAGS) $< /Fo:out\$@

default:
	@if not exist "out" mkdir out

clean:
	del /f /q out\*.*