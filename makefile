INCLUDE=.\cef\;$(INCLUDE)
out\EasyCKL.dll : default out\simple_app.obj out\simple_handler.obj out\simple_handler_win.obj out\CKLMain.obj out\EcPackApi.obj out\cefsimple.res
	link /nologo /DLL /DEF:"Export.def" out\*.obj out\cefsimple.res /out:"out\EasyCKL.dll" /LIBPATH:".\lib"

out\simple_app.obj : simple_app.cpp
	cl /nologo /EHsc /MT /c simple_app.cpp /Fo:out\simple_app.obj

out\simple_handler.obj : simple_handler.cpp
	cl /nologo /EHsc /MT /c simple_handler.cpp /Fo:out\simple_handler.obj

out\simple_handler_win.obj : simple_handler_win.cpp
	cl /nologo /EHsc /MT /c simple_handler_win.cpp /Fo:out\simple_handler_win.obj

out\CKLMain.obj : CKLMain.cpp
	cl /nologo /EHsc /MT /c CKLMain.cpp /Fo:out\CKLMain.obj

out\EcPackApi.obj : EcPackApi.cpp
	cl /nologo /EHsc /MT /c EcPackApi.cpp /Fo:out\EcPackApi.obj

out\cefsimple.res : cefsimple.rc
	rc /nologo /fo out\cefsimple.res cefsimple.rc 
clean:
	del /f /q out\*.*

default:
	if not exist "out" mkdir out