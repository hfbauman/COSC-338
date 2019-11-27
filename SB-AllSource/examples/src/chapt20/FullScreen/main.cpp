//  FULLSCREEN example program
//  main.cpp
//  CarbonGL
//  OpenGL SuperBible
//

#include <Carbon/Carbon.h>
#include <unistd.h>
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#include <Agl/agl.h>
#include "../../shared/Stopwatch.h"


static OSStatus        AppEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon );
static OSStatus        HandleNew();
static OSStatus        WindowEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon );

static IBNibRef        sNibRef;

static AGLContext      openGLContext = NULL;        // OpenGL rendering context
static int             nScreenWidth = 800;          // Set these to what you "want"
static int             nScreenHeight = 600;         
static bool            bChangeDisplayMode = false;  // Set to false to ignore above and keep current rez.

// Framerate counter
static int  nFrames = 0;
static CStopWatch fpsTimer;
static GLint fontStart;
 

// These are all in thunderGL.cpp
void SetupRC(void);
void ShutdownRC(void);
void RenderScene(void);
void ChangeSize(int w, int h);


///////////////////////////////////////////////////////////////////////////////////
bool SetupGL(void)
    {
    static AGLPixelFormat	openGLFormat = NULL;		// OpenGL pixel format 

    static GLint glAttributes[] = {      
                    AGL_FULLSCREEN,             // Full Screen
                    AGL_NO_RECOVERY,
                    // Uncoment to get FSAA
  //                  AGL_SAMPLE_BUFFERS_ARB, 1,  // How many sample buffers (only one supported at this time)
  //                  AGL_SAMPLES_ARB,  4,        // Number of samples at a point (increased antialiasing) bigger number = better quality
                    AGL_DOUBLEBUFFER, GL_TRUE,  // Double buffered
                    AGL_RGBA,         GL_TRUE,  // Four component color
                    AGL_DEPTH_SIZE,   16,       // 16-bit (at least) depth buffer
                    AGL_NONE };                 // Terminator
    
    // Get the main display and capture it. 
    CGDirectDisplayID gCGDisplayID = CGMainDisplayID();
  //  CGDisplayCapture(gCGDisplayID);
    
    // Change display mode if necessary
    if(bChangeDisplayMode)
        {
        CFDictionaryRef refDisplayMode = 
            CGDisplayBestModeForParameters(
            gCGDisplayID, 32, nScreenWidth, nScreenHeight, NULL);
        
            // Screen will revert when this program ends
            CGDisplaySwitchToMode(gCGDisplayID, refDisplayMode);
        }
    
    // Get the screen resolution (even if we have changed it)
    nScreenWidth = CGDisplayPixelsWide(gCGDisplayID);
    nScreenHeight = CGDisplayPixelsHigh(gCGDisplayID);
    
    // Initialize OpenGL
    // Choose pixelformat based on attribute list, and the main display device
    GDHandle gdevice = GetMainDevice();    
    openGLFormat   = aglChoosePixelFormat(&gdevice, 1, glAttributes);
    if(openGLFormat == NULL)
        return false;
        
    // Create the context
    openGLContext  = aglCreateContext(openGLFormat, NULL);
    if(openGLContext == NULL)
        return false;
    
    // Don't need this anymore
    aglDestroyPixelFormat(openGLFormat);    

    // Tell AGL to go full screen
    aglEnable(openGLContext, AGL_FS_CAPTURE_SINGLE);
    aglSetFullScreen(openGLContext, 0, 0, 0, 0);    
    aglSetCurrentContext(openGLContext);
 
    // OpenGL is up... go load up goemetry and stuff
    SetupRC();
        
    // Reset projection
    ChangeSize(nScreenWidth, nScreenHeight);
    
    // Setup the font
    fontStart = glGenLists(96);
    short fontID;
    GetFNum((const unsigned char *)"/pTimes", &fontID);
    aglUseFont(openGLContext, fontID,  bold, 14, ' ', 96, fontStart);

    glListBase(fontStart - ' ');
    
    HideCursor();   // Hide the cursor (personal preference)
    
    return true;
    }

///////////////////////////////////////////////////////////////////////////////////////
// Shut everyting down and do cleanup
void ShutdownGL(void)
    {
    ShutdownRC();
    
    CGReleaseAllDisplays();    
    
    // Unbind to context
    aglSetCurrentContext (NULL);
    
    // Remove drawable
    aglSetDrawable (openGLContext, NULL);
    aglDestroyContext (openGLContext);
    
    ShowCursor();   // Restore the cursor
    }


//--------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    OSStatus                    err;
    static const EventTypeSpec    kAppEvents[] =
    {
        { kEventClassCommand, kEventCommandProcess }
    };

    // Little cheat to get working directory back to /Resources like it is in glut
    char *parentdir;
    char *c;
    
    parentdir = strdup(argv[0]);
    c = (char*) parentdir;

    while (*c != '\0')     // go to end 
       c++;
    
    while (*c != '/')      // back up to parent 
        c--;
    
    *c++ = '\0';             // cut off last part (binary name) 
 
    // Change to Resources directory. Any files need to be place there
    chdir(parentdir);
    chdir("../Resources");
    free(parentdir);


    // Create a Nib reference, passing the name of the nib file (without the .nib extension).
    // CreateNibReference only searches into the application bundle.
    err = CreateNibReference( CFSTR("main"), &sNibRef );
    require_noerr( err, CantGetNibRef );
    
    // Once the nib reference is created, set the menu bar. "MainMenu" is the name of the menu bar
    // object. This name is set in InterfaceBuilder when the nib is created.
    err = SetMenuBarFromNib( sNibRef, CFSTR("MenuBar") );
    require_noerr( err, CantSetMenuBar );
    
    // Install our handler for common commands on the application target
    InstallApplicationEventHandler( NewEventHandlerUPP( AppEventHandler ),
                                    GetEventTypeCount( kAppEvents ), kAppEvents,
                                    0, NULL );
    
    // Create a new window. A full-fledged application would do this from an AppleEvent handler
    // for kAEOpenApplication.
    HandleNew();
    
    // Run the event loop
    RunApplicationEventLoop();

CantSetMenuBar:
CantGetNibRef:
    return err;
}

//--------------------------------------------------------------------------------------------
static OSStatus
AppEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon )
    {
    OSStatus    result = eventNotHandledErr;
    
    switch ( GetEventClass( inEvent ) )
        {
        case kEventClassCommand:
            {
            HICommandExtended cmd;
            verify_noerr( GetEventParameter( inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof( cmd ), NULL, &cmd ) );
            
            switch ( GetEventKind( inEvent ) )
                {
                case kEventCommandProcess:
                    switch ( cmd.commandID )
                        {
                        case kHICommandNew:
                            result = HandleNew();
                            break;
                            
                        // Add your own command-handling cases here
                        
                        default:
                            break;
                        }
                    break;
                }
                break;
            }
        default:
            break;
        }
    
    return result;
    }




//--------------------------------------------------------------------------------------------
DEFINE_ONE_SHOT_HANDLER_GETTER( WindowEventHandler )

//--------------------------------------------------------------------------------------------
static OSStatus
HandleNew()
{
    OSStatus                    err;
    WindowRef                    window;
    static const EventTypeSpec    kWindowEvents[] =
    {
        { kEventClassCommand, kEventCommandProcess },
        { kEventClassWindow, kEventWindowDrawContent },
        { kEventClassWindow, kEventWindowClose }
    };

 
     Rect            windowRect;
     int             windowAttributes;
   
    // Set window dimensions here
    SetRect(&windowRect, 10, 60, 800, 600);
    
    // Set Window attributes
    windowAttributes = kWindowStandardHandlerAttribute | kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute |
                       kWindowResizableAttribute | kWindowStandardDocumentAttributes;
    
    // Create the Window
    CreateNewWindow(kDocumentWindowClass, windowAttributes, &windowRect, &window);
    SetWTitle(window, "\pCarbonGL with Frame Rate");
 
    // Install a command handler on the window. We don't use this handler yet, but nearly all
    // Carbon apps will need to handle commands, so this saves everyone a little typing.
    InstallWindowEventHandler( window, GetWindowEventHandlerUPP(),
                               GetEventTypeCount( kWindowEvents ), kWindowEvents,
                               NULL, NULL );

    
    // Position new windows in a staggered arrangement on the main screen
    RepositionWindow( window, NULL, kWindowCascadeOnMainScreen );
    
    SetupGL();
    
    // The window was created hidden, so show it
    ShowWindow( window );
    
CantCreateWindow:
    return err;
}

//--------------------------------------------------------------------------------------------
static OSStatus
WindowEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon )
{
    OSStatus    err = eventNotHandledErr;
    static Rect windowRect;
    static char cBuffer[32] = {"FPS: "};
    
    switch ( GetEventClass( inEvent ) )
    {
        case kEventClassCommand:
        {
            HICommandExtended cmd;
            verify_noerr( GetEventParameter( inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof( cmd ), NULL, &cmd ) );
            
            switch ( GetEventKind( inEvent ) )
            {
                case kEventCommandProcess:
                    switch ( cmd.commandID )
                    {
                        // Add your own command-handling cases here
                        
                        default:
                            break;
                    }
                    break;
            }
            break;


        case kEventClassWindow:
        {
        	WindowRef			window = NULL;
            GetEventParameter(inEvent, kEventParamDirectObject, typeWindowRef, NULL, sizeof(WindowRef), NULL, &window);

            switch(GetEventKind(inEvent))
                {                                    
                // Window is closed
                case kEventWindowClose:
                    ShutdownGL();
                    QuitApplicationEventLoop();
                    break;
                    
                // Draw/Render contents
                case kEventWindowDrawContent:
                    {
                    RenderScene();
                    
                    nFrames++;
                    if(nFrames == 100)
                        {
                        float fps = 100.0f / fpsTimer.GetElapsedSeconds();
                        sprintf(cBuffer,"FPS: %.1f", fps);
                                                
                        nFrames = 0;
                        fpsTimer.Reset();
                        }
                    
                    glColor3f(1.0f, 1.0f, 1.0f); // Just in case it get's changed
                    glWindowPos2f(10.0f, 10.0f);
                    glListBase(fontStart - ' ');
                    glCallLists(strlen(cBuffer), GL_BYTE, cBuffer);
                    aglSwapBuffers(openGLContext);
                    
                    GetWindowPortBounds(window, &windowRect);
                    InvalWindowRect(window, &windowRect);
                    }
                    break;
                }
            }
        break;
        }
            
        default:
            break;
    }
    
    return err;
}
