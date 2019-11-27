MAIN = texfloat
LIBDIRS = -L /usr/local/lib
INCDIRS = -I /usr/include -I /usr/local/include/GL -I ../../../../src/shared -I /usr/local/include/OpenEXR -I/usr/include/GL
SRCPATH = ../../../../src/chapt18/$(MAIN)/
SHAREDPATH = ../../../../src/shared/

CC = gcc
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS) -DGL_GLEXT_PROTOTYPES
LIBS = -lglut -lGL -lGLU -lIlmThread -lIlmImf

prog : $(MAIN)

$(MAIN).o : $(SRCPATH)$(MAIN).cpp

math3d.o : $(SHAREDPATH)math3d.cpp

gltools.o : $(SHAREDPATH)gltools.cpp

$(MAIN) : $(MAIN).o gltools.o math3d.o
	$(CC) $(CFLAGS) -o $(MAIN) $(LIBDIRS) $(SRCPATH)$(MAIN).cpp $(SHAREDPATH)math3d.cpp $(SHAREDPATH)gltools.cpp $(SHAREDPATH)GLee.c $(LIBS)

clean:
	rm -f *.o
