#import "ThunderGLView.h"
#include <OpenGL/gl.h>

// Functions in ThunderGL.cpp
void SetupRC(void); 
void ShutdownRC(void);
void RenderScene(void);
void ChangeSize(int w, int h);

@implementation ThunderGLView

- (void)idle:(NSTimer *)pTimer
    {
    [self drawRect:[self bounds]]; 
    }
    
// Do setup
 - (void) prepareOpenGL
    {
    SetupRC();

    pTimer = [NSTimer timerWithTimeInterval:(0.0) target:self 
           selector:@selector(idle:) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop]addTimer:pTimer forMode:NSDefaultRunLoopMode];    
    }
    
// Do cleanup
- (void) clearGLContext
    {
    ShutdownRC();
    }
    
// Changed size
- (void)reshape
    {
    NSRect rect = [self bounds];
    ChangeSize(rect.size.width, rect.size.height);
    }    
    

// Paint
-(void) drawRect: (NSRect) bounds
    {
    RenderScene(); 
    
    glFlush();  // All done
    }    
@end
