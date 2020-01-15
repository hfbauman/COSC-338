LIBDIRS = -L/usr/X11R6/lib -L/usr/X11R6/lib64 -L/usr/local/lib
INCDIRS = -I/usr/include -L/usr/local/include -I/usr/include/GL

CC = gcc
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS) -DGL_GLEXT_PROTOTYPES  -lstdc++
LIBS = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

font : ../../../../src/chapt21/fonts/font.o
	$(CC) $(CFLAGS) -o font $(LIBDIRS) ../../../../src/chapt21/fonts/font.c $(LIBS)

clean:
	rm -f *.o
