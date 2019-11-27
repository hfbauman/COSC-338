LIBDIRS = -L /usr/local/lib
INCDIRS = -I /usr/include -I /usr/local/include/GL -I ../../../../src/shared -I/usr/include/GL
SRCPATH = ../../../../src/chapt03/lstrips/
SHAREDPATH = ../../../../src/shared/

CC = gcc
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS)
LIBS = -lglut -lGL -lGLU -lm

prog : lstrips

lstrips.o : $(SRCPATH)lstrips.cpp

math3d.o : $(SHAREDPATH)math3d.cpp

gltools.o : $(SHAREDPATH)gltools.cpp

lstrips : lstrips.o gltools.o math3d.o
	$(CC) $(CFLAGS) -o lstrips $(LIBDIRS) $(SRCPATH)lstrips.cpp $(SHAREDPATH)math3d.cpp $(SHAREDPATH)gltools.cpp $(SHAREDPATH)GLee.c $(LIBS)

clean:
	rm -f *.o


