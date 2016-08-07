# Copyright daemon_process
	
OBJS = CKLMain.o CustomizeJS.o CustomizeScheme.o EcPackApi.o FrameApi.o simple_app.o simple_handler.o simple_handler_win.o

FLAGS = -fPIC -D __linux__ -std=c++11 -I./cef_linux

libEasyCKL.a : default $(OBJS)
	g++ -shared --pic -o libEasyCKL.so *.o -lcef_dll_wrapper -lcef -lX11 -L./lib_linux
	#ar rcs libEasyCKL.a *.o
	rm ../libEasyCKL.so
	cp libEasyCKL.so ../

%.o : %.cpp
	g++ -c $< -o $@ $(FLAGS)

default:
ifneq ("out", "$(wildcard out)")
	mkdir out
endif

clean:
	rm -rf ./out
