// font.c
// Use GLX/X fonts with GLUT
// OpenGL SuperBible, 4th Edition
// Nick Haemel


#include "GL/gl.h"	// System and OpenGL Stuff
#include "GL/glut.h"
#include <X11/Xlib.h>
#include <string.h>


GLuint uiDListStart;
XFontStruct *xFont;

void RenderScene(void)
{
    char *szString =  "GLX Fonts";

    glClear(GL_COLOR_BUFFER_BIT);

    // Now set the screen position
    glRasterPos3f(0.0f, 0.0f, 0.0f);
    
    // Save the display list bit
    glPushAttrib(GL_LIST_BIT);

    // Setup the display list offset
    glListBase(uiDListStart - ' ' );

    // Now call the appropriate lists for the characters
    // in the string
    glCallLists(strlen(szString), GL_UNSIGNED_BYTE, (GLubyte*)szString);

    // Restore the display list bit
    glPopAttrib();
    
    // Flush and swap
    glutSwapBuffers();
}



void SetupRC(void)
{
    Display *dpy = (Display*)glXGetCurrentDisplay();

    // Set clear color to green
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    
    // Load a courier font with size 48pix.
    XFontStruct *xFont = XLoadQueryFont(dpy, 
                "-*-courier-bold-r-normal--48-*-*-*-*-*-*-*");
   
    // Generate display list names
    uiDListStart = glGenLists(96);

    // Have GLX generate the display lists for the characters
    glXUseXFont(xFont->fid, ' ', 96, uiDListStart); 
}


void ChangeSize(int nW, int nH)
{
    GLfloat fAspectRatio;

    // Prevent a divide by zero
    if(nH == 0)
        nH = 1;
		
    // Set Viewport to window dimensions
    glViewport(0, 0, nW, nH);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    fAspectRatio = (GLfloat)nW / (GLfloat)nH;
    gluPerspective(35.0f, fAspectRatio, 1.0f, 1000.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-500.0f, -50.0f, -400.0f);
}


int main(int argc, char* argv[])
	{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500,60);
        glutCreateWindow("Fonts");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	SetupRC();
	
	glutMainLoop();
        
        return 0;
    }
