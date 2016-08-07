# Copyright daemon_process

INCLUDE = ..\cef\;$(INCLUDE)
	
OBJS = subprocess.obj

CLFLAGS = /nologo /EHsc /MT /c /O2 /MP /GS /TP /analyze- /W4 /wd"4100" /wd"4127" /wd"4244" /wd"4481" /wd"4512" /wd"4701" /wd"4702" /wd"4996" /Gy /Zc:wchar_t /Gm- /O2 /Ob2 /Zc:inline /fp:precise /D "NDEBUG" /D "_NDEBUG" /D "__STDC_CONSTANT_MACROS" /D "__STDC_FORMAT_MACROS" /D "WIN32" /D "_WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "WINVER=0x0602" /D "NOMINMAX" /D "WIN32_LEAN_AND_MEAN" /D "_HAS_EXCEPTIONS=0" /D "CEF_USE_ATL" /D "_USING_V110_SDK71_" /errorReport:prompt /GF /WX /Zc:forScope /GR- /Gd /Oy- /MT

subprocess.exe : default $(OBJS)
	link /nologo *.obj /out:"subprocess.exe" /subsystem:windows,5.01 /LIBPATH:"..\lib" /LIBPATH:"..\out"

.cpp.obj:
	@cl $(CLFLAGS) $< /Fo:$@

default:
	@if not exist "..\out\libcef_dll_wrapper.lib" (cd ..\cef & nmake & cd ..\subprocess)

clean:
	del /f /q subprocess.exe
	del /f /q subprocess.obj