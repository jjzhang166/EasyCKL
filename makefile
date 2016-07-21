
# Copyright daemon_process

INCLUDE = ..\cef\;$(INCLUDE)
	
OBJS = CKLMain.obj CustomizeJS.obj CustomizeScheme.obj EcPackApi.obj FrameApi.obj simple_app.obj simple_handler.obj simple_handler_win.obj

out\EasyCKL.dll : default $(OBJS) cefsimple.res
	link /nologo /DLL /DEF:"..\Export.def" *.obj cefsimple.res /out:"EasyCKL.dll" /LIBPATH:"..\lib"
	cd ..

{..\}.cpp.obj:
	@cl /nologo /EHsc /MT /c /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "UNICODE" /D "_UNICODE" $< /Fo:$@

cefsimple.res : ..\cefsimple.rc
	@rc /nologo /fo cefsimple.res ..\cefsimple.rc 

default:
	@if not exist "out" mkdir out
	@if not exist "out\libcef_dll_wrapper.lib" (cd cef & nmake & cd ..)
	cd out

clean:
	del /f /q out\*.*