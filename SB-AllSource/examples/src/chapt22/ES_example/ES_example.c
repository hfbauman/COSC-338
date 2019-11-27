#include "akUtils.h"
#include "akAppKit.h"
#include "EGL/egl.h"
#include "GLES/gl.h"
#include <math.h>


#define WIN_W 400
#define WIN_H 400

// vertex data for triangle
static const GLbyte verts [] =
{
    0,   8, 0,
    7,  -4, 0,
   -7,  -4, 0,
};


// Structure for program context info
typedef struct ProgramInfo_Rec
{
    EGLDisplay	display;
    EGLSurface	windowSurface;
    EGLConfig	config;
    EGLContext	esContext;

} ProgramInfo;

void updateWindow(int x, int y)
{
    float aspectRatio = y ? (float)x/(float)y : 1.0f;
    float fYTop     = 0.05f;
    float fYBottom  = - fYTop;
    float fXLeft    = fYTop     * aspectRatio;
    float fXRight   = fYBottom  * aspectRatio;

    // update viewport and scissor
    glViewport(0, 0, x, y);
    glScissor(0, 0, x, y);

    // Clear the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    /// set the frustrum
    glFrustumf(fXLeft, fXRight, fYBottom, fYTop, 0.1f, 100.f);
}

void setup(ProgramInfo* programInfo, AKAppKit* appKit)
{
    EGLint majorVer = 0;
    EGLint minorVer = 0;
    EGLint matchingConfigCnt = 0;
    AKSurfaceDesc surfDesc;
    EGLConfig* returnedConfigs = (EGLConfig*)malloc(1*sizeof(EGLConfig));

    // select the surface attribs for EGL window
    EGLint surfAttribs[] =
{
        EGL_COLORSPACE,		EGL_COLORSPACE_sRGB,
        EGL_NONE
    };

    // Select config attribs for config selection
    EGLint attribs[] =
    {
        EGL_RED_SIZE,				6,
        EGL_GREEN_SIZE, 			6,
        EGL_BLUE_SIZE,				6,
        EGL_RENDERABLE_TYPE,		EGL_OPENGL_ES_BIT,
        EGL_SURFACE_TYPE,			EGL_WINDOW_BIT,
        EGL_NONE
    };

    // Create a system window through the AppKit
    AKAppKit_createWindow(appKit, WIN_W, WIN_H, 
                          "OpenGL ES example - OpenGL Superbible", 
                          AK_COLORFORMAT_NONE);
    

    // Get the native window type for creating the egl surface.
    surfDesc = AKAppKit_getSurface(appKit);
   
    // Get the dispaly for use in future EGL calls.
    programInfo->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    // must initialize the display before it can be rendered to.
    eglInitialize(programInfo->display, &majorVer, &minorVer);
     
    // We want 1 config that matches our params
    eglChooseConfig(programInfo->display, attribs, returnedConfigs, 1, &matchingConfigCnt);
    programInfo->config = returnedConfigs[0];

    // Create EGL window
    programInfo->windowSurface = eglCreateWindowSurface(programInfo->display, programInfo->config, (NativeWindowType)surfDesc.nativePtr, surfAttribs);
	
    // Create a new EGL context
    programInfo->esContext = eglCreateContext(programInfo->display, programInfo->config, NULL, NULL);

    // Make the new context the current context
    eglMakeCurrent(programInfo->display, programInfo->windowSurface, 
                   programInfo->windowSurface, programInfo->esContext);

    // Tell EGL that this context will be used for OGL rendering
    eglBindAPI(EGL_OPENGL_ES_API);

    // Setup window state in OpenGL   
    updateWindow(AKAppKit_getSurface(appKit).width, AKAppKit_getSurface(appKit).height);

    // Setup default GL state 
    glShadeModel(GL_FLAT);
    glColor4x(0x00000, 0x10000, 0x10000, 0x10000);

    // Turn on vertex arrays and set the vertex pointer 
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_BYTE, 0, verts);
}


void destroy(AKAppKit* appKit)
{
    ProgramInfo* programInfo = (ProgramInfo*)AKAppKit_getUserPtr(appKit);

    // Cleanup up EGL state before exiting. 
    if (programInfo != 0)
    {
        eglMakeCurrent(programInfo->display, NULL, NULL, NULL);
        eglDestroyContext(programInfo->display, programInfo->esContext);
        eglDestroySurface(programInfo->display, programInfo->windowSurface);
        eglTerminate(programInfo->display);
        eglReleaseThread();
    }
}


void updateScene(AKAppKit* ak, HGint32 timeDelta)
{
    static int clearScale = 0;
    float fClearColor = 0;

    // calculate a time factor for rotation
    const double time = fmod(AKAppKit_getTime(ak) / 1000.0, 40.0);

    // Get our context state 
    ProgramInfo* programInfo = (ProgramInfo*)AKAppKit_getUserPtr(ak);
    
    // update window state for window dimension changes
    updateWindow(AKAppKit_getSurface(ak).width, AKAppKit_getSurface(ak).height);
    
    // Calculate a changing background color
    if (clearScale < 2048)
        fClearColor = (float)clearScale/(float)4096;
    else
         fClearColor = (float)(4096 - clearScale) /(float)4096;
   
    clearScale += 1;
    if (clearScale > 4096) clearScale = 0;
    
    // Clear the color buffer with the new value
    glClearColor(fClearColor, fClearColor, fClearColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear modelview matrix
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -20.f);

    // rotate based on application execution time
    glRotatef((GLfloat)(time*35.0f), -1.0f, -3.0f, 0.0f);
    glRotatef((GLfloat)(time*20.0f),  0.5f, 0.0f,  7.0f);
    
    // Draw a triangle
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

    // Present the surface
    eglSwapBuffers(programInfo->display, programInfo->windowSurface);
}


HGbool appInit (AKAppKit* appKit)
{
    AKAppKitParams	setupParams;

    // Used to communicating program state to callbacks
    ProgramInfo *programInfo = (ProgramInfo*)malloc(sizeof(ProgramInfo));
    
    // Set callbacks
    AKAppKitParams_clear(&setupParams);
  	setupParams.update			= updateScene;
	setupParams.userPtr			= programInfo;
    setupParams.deinit			= destroy;

    // Tell the AppKit about the which callbacks to use
    AKAppKit_initParams(appKit, &setupParams);

    // call our setup program to initialize EGL and OpenGL
    setup(programInfo, appKit);

    return HG_TRUE;
}

