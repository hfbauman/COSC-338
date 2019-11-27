//
//  main.cpp
//  CarbonGL
//  OpenGL SuperBible
//

#include <Carbon/Carbon.h>
#include <unistd.h>
#include <OpenGL/gl.h>
#include <Agl/agl.h>


static OSStatus        AppEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon );
static OSStatus        HandleNew();
static OSStatus        WindowEventHandler( EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon );

static IBNibRef        sNibRef;

static AGLContext       openGLContext = NULL;       // OpenGL rendering context

// These are all in thunderGL.cpp
void SetupRC(void);
void ShutdownRC(void);
void RenderScene(void);
void ChangeSize(int w, int h);


///////////////////////////////////////////////////////////////////////////////////
bool SetupGL(WindowRef& windowRef)
    {
    static GLint glAttributes[] = {      
                    AGL_DOUBLEBUFFER, GL_TRUE,  // Double buffered
                    AGL_RGBA,         GL_TRUE,  // Four component color
                    AGL_DEPTH_SIZE,   16,       // 16-bit (at least) depth buffer
                    AGL_NONE };                 // Terminator

    // Initialize OpenGL, set to one and only window
    // Choose pixelformat based on attribute list
    AGLPixelFormat openGLFormat = aglChoosePixelFormat(NULL, 0, glAttributes);
    if(openGLFormat == NULL)
        return false;
        
    // Create the context
    openGLContext  = aglCreateContext(openGLFormat, NULL);
    if(openGLContext == NULL)
        return false;

    // No longer needed
    aglDestroyPixelFormat(openGLFormat);    
    
    // Point to window and make current
    aglSetDrawable(openGLContext, GetWindowPort(windowRef));
    aglSetCurrentContext(openGLContext);
    
    // OpenGL is up... go load up geometry and stuff
    SetupRC();
    
    return true;
    }

///////////////////////////////////////////////////////////////////////////////////////
// Shut everyting down and do cleanup
void ShutdownGL(void)
    {
    ShutdownRC();
    
    // Unbind to context
    aglSetCurrentContext (NULL);
    
    // Remove drawable
    aglSetDrawable (openGLContext, NULL);
    aglDestroyContext (openGLContext);
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
        { kEventClassWindow, kEventWindowClose },
        { kEventClassWindow, kEventWindowBoundsChanged } 
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
    SetWTitle(window, "\pCarbonGL");
 
    // Install a command handler on the window. We don't use this handler yet, but nearly all
    // Carbon apps will need to handle commands, so this saves everyone a little typing.
    InstallWindowEventHandler( window, GetWindowEventHandlerUPP(),
                               GetEventTypeCount( kWindowEvents ), kWindowEvents,
                               NULL, NULL );

    
    // Position new windows in a staggered arrangement on the main screen
    RepositionWindow( window, NULL, kWindowCascadeOnMainScreen );
    
    SetupGL(window);
    ChangeSize(800, 600);
    
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
                    aglSwapBuffers(openGLContext);
                    
                    GetWindowPortBounds(window, &windowRect);
                    InvalWindowRect(window, &windowRect);
                    }
                    break;

                // Size of window has changed
                case kEventWindowBoundsChanged:
                    if(openGLContext)
                        {
                        aglUpdateContext(openGLContext);
                      
                        GetWindowPortBounds(window, &windowRect);
                        ChangeSize(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
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
