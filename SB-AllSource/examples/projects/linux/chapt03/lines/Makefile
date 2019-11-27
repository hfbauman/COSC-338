LIBDIRS = -L /usr/local/lib
INCDIRS = -I /usr/include -I /usr/local/include/GL -I ../../../../src/shared -I/usr/include/GL
SRCPATH = ../../../../src/chapt03/lines/
SHAREDPATH = ../../../../src/shared/

CC = gcc
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS) -DGL_GLEXT_PROTOTYPES
LIBS = -lglut -lGL -lGLU -lm

prog : lines

lines.o : $(SRCPATH)lines.cpp

math3d.o : $(SHAREDPATH)math3d.cpp

gltools.o : $(SHAREDPATH)gltools.cpp

lines : lines.o gltools.o math3d.o
	$(CC) $(CFLAGS) -o lines $(LIBDIRS) $(SRCPATH)lines.cpp $(SHAREDPATH)math3d.cpp $(SHAREDPATH)gltools.cpp $(SHAREDPATH)GLee.c $(LIBS)

clean:
	rm -f *.o
