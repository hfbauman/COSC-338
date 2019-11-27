Setup For ES
__________________________________________________________

This sample is different from the rest of the samples 
in this book since it requires an ES rendering environment.
This document describes how to setup the sample with the
Rasteroid ES emulator.
Currently this rendering environment is only available for 
Microsoft Windows.

__________________________________________________________


1. Download and install Rasteroid 3.1 from:

http://www.hybrid.fi/main/products/devtools.php

2. Extract and install download.
   The installed structure should look something like:
    Rasteroid -
        - bin
        - doc
        - include
        - lib
        - samples

3. Copy the example directory to the Rasteroid directory.

4. The project uses the appkit.lib that gets compiled into
   samples\build\win32\Debug\appkit.lib If the lib is not 
   present, you can build it by opening the appkit project 
   in samples\build\win32 and building it.

5. Open the ES_example.dsp project in the example directory 
   you just copied over.

6. Compile the project and run it to launch an ES application
   in a Microsoft Windows environment.
