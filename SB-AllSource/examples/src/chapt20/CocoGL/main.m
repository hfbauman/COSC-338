//
//  main.m
//  CocoaGL
//
//  Created by Richard Wright on 3/3/07.
//  Copyright __MyCompanyName__ 2007. All rights reserved.
//

#import <Cocoa/Cocoa.h>
    



int main(int argc, char *argv[])
{
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

    return NSApplicationMain(argc,  (const char **) argv);
}
