out\EasyCKL.dll : out\simple_app.obj out\simple_handler.obj out\simple_handler_win.obj out\CKLMain.obj
	link /DLL /DEF:"Export.def" out\*.obj /out:"out\EasyCKL.dll" /LIBPATH:".\lib"

out\simple_app.obj : simple_app.cpp
	cl /EHsc /MT /c simple_app.cpp /Fo:out\simple_app.obj

out\simple_handler.obj : simple_handler.cpp
	cl /EHsc /MT /c simple_handler.cpp /Fo:out\simple_handler.obj

out\simple_handler_win.obj : simple_handler_win.cpp
	cl /EHsc /MT /c simple_handler_win.cpp /Fo:out\simple_handler_win.obj

out\CKLMain.obj : CKLMain.cpp
	cl /EHsc /MT /c CKLMain.cpp /Fo:out\CKLMain.obj