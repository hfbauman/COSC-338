LIBDIRS = -L/usr/X11R6/lib -L/usr/X11R6/lib64 -L/usr/local/lib
INCDIRS = -I/usr/include -L/usr/local/include -I/usr/include/GL

CC = gcc
CFLAGS = $(COMPILERFLAGS) -g $(INCDIRS)
LIBS = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

GLXBasics : ../../../../src/chapt21/GLXBasics/GLXBasics.o
	$(CC) $(CFLAGS) -o GLXBasics $(LIBDIRS) ../../../../src/chapt21/GLXBasics/GLXBasics.c $(LIBS)

clean:
	rm -f *.o
