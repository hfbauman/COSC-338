MAIN = thundergl
LIBDIRS = -L /usr/local/lib
INCDIRS = -I /usr/include -I /usr/local/include/GL -I ../../../../src/shared -I/usr/include/GL
SRCPATH = ../../../../src/chapt11/vbo/
SHAREDPATH = ../../../../src/shared/

CC = gcc
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS) -DGL_GLEXT_PROTOTYPES
LIBS = -lglut -lGL -lGLU -lm

prog : $(MAIN)

$(MAIN).o : $(SRCPATH)$(MAIN).cpp

body.o : $(SRCPATH)body.cpp

glass.o : $(SRCPATH)glass.cpp

math3d.o : $(SHAREDPATH)math3d.cpp

gltools.o : $(SHAREDPATH)gltools.cpp

VBOMesh.o : $(SHAREDPATH)VBOMesh.cpp

$(MAIN) : $(MAIN).o gltools.o math3d.o body.o glass.o VBOMesh.o
	$(CC) $(CFLAGS) -o $(MAIN) $(LIBDIRS) $(SRCPATH)$(MAIN).cpp  $(SRCPATH)body.cpp  $(SRCPATH)glass.cpp $(SHAREDPATH)math3d.cpp $(SHAREDPATH)gltools.cpp $(SHAREDPATH)VBOMesh.cpp $(SHAREDPATH)GLee.c $(LIBS)

clean:
	rm -f *.o
