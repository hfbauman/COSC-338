LIBDIRS = -L /usr/local/lib
INCDIRS = -I /usr/include -I /usr/local/include/GL -I ../../../../src/shared -I/usr/include/GL
SRCPATH = ../../../../src/chapt03/points/
SHAREDPATH = ../../../../src/shared/

CC = gcc
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS)
LIBS = -lglut -lGL -lGLU -lm

prog : points

points.o : $(SRCPATH)points.cpp

math3d.o : $(SHAREDPATH)math3d.cpp

gltools.o : $(SHAREDPATH)gltools.cpp

points : points.o gltools.o math3d.o
	$(CC) $(CFLAGS) -o points $(LIBDIRS) $(SRCPATH)points.cpp $(SHAREDPATH)math3d.cpp $(SHAREDPATH)gltools.cpp $(SHAREDPATH)GLee.c $(LIBS)

clean:
	rm -f *.o


