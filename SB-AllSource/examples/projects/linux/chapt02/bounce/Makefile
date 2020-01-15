LIBDIRS = -L /usr/local/lib
IMGUIPATH =
INCDIRS = -I /usr/include -I /usr/local/include/GL -I ../../../../src/shared -I/usr/include/GL -I../../../../../../contrib/imgui/
SRCPATH = ../../../../src/chapt02/bounce/
SHAREDPATH = ../../../../src/shared/

IMGUI_SRC = ../../../../../../contrib/imgui//imgui.cpp ../../../../../../contrib/imgui//imgui_demo.cpp ../../../../../../contrib/imgui//imgui_draw.cpp ../../../../../../contrib/imgui/imgui_impl_glut.cpp ../../../../../../contrib/imgui//imgui_widgets.cpp ../../../../../../contrib/imgui//imgui_impl_opengl2.cpp

CC = gcc
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS) -DGL_GLEXT_PROTOTYPES -fno-threadsafe-statics -fno-exceptions  -lstdc++
LIBS = -lglut -lGL -lGLU -lm

prog : bounce

.cpp.o :
	$(CC) -c $(CCFLAGS) $(INCDIRS) $*.cpp

.c.o :
	$(CC) -c $(CCFLAGS) $(INCDIRS) $*.c

bounce.o : $(SRCPATH)bounce.cpp

math3d.o : $(SHAREDPATH)math3d.cpp

gltools.o : $(SHAREDPATH)gltools.cpp

bounce : bounce.o gltools.o math3d.o
	$(CC) $(CFLAGS) -o bounce $(LIBDIRS) $(IMGUI_SRC) $(SRCPATH)bounce.cpp $(SHAREDPATH)math3d.cpp $(SHAREDPATH)gltools.cpp $(SHAREDPATH)GLee.c  $(LIBS)

clean:
	rm -f *.o
