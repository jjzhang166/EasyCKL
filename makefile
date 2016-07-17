
# Copyright daemon_process

INCLUDE = .\cef\;$(INCLUDE)
	
OBJS = CKLMain.obj CustomizeJS.obj CustomizeScheme.obj EcPackApi.obj FrameApi.obj simple_app.obj simple_handler.obj simple_handler_win.obj

out\EasyCKL.dll : default $(OBJS) out\cefsimple.res
	link /nologo /DLL /DEF:"Export.def" out\*.obj out\cefsimple.res /out:"out\EasyCKL.dll" /LIBPATH:".\lib" /LIBPATH:".\out"

.cpp.obj:
	@cl /nologo /EHsc /MT /c $< /Fo:out\$@

out\cefsimple.res : cefsimple.rc
	@rc /nologo /fo out\cefsimple.res cefsimple.rc 

default:
	@if not exist "out" mkdir out
	@if not exist "out\libcef_dll_wrapper.lib" (cd cef & nmake & cd ..)

clean:
	del /f /q out\*.*