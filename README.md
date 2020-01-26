COSC-338 - Spring 2020
======================

Main codebase for COSC-338.  Also see github.com/billsix/modelviewprojection

Source
------

The Source code for the OpenGL SuperBible v4 is under SB-AllSource/.
Currently, I ported it to build on modern linux, Visual Studio 2019.  Still
should port to MacOS, but have not done that yet

Other Useful Sites for Study
----------------------------

* http://opengl-tutorial.org
  * https://github.com/opengl-tutorials/ogl

* http://learnopengl.com
  * https://learnopengl.com/

* McNopper's OpenGL Demos

  * A bunch of OpenGL demos that are fun to study.

  * https://github.com/McNopper/OpenGL

  * Patch including for running on Linux 0001-build-working-on-Linux.patch,
    currently I have not tried building on Windows.

  * If you don't have GLFW installed, you may need to clone McNopper's repo,
     "git submodule init" "git submodule update".

  * Verified Builds:
    * Linux

* Fogleman's 2 Minecraft Projects: Minecraft (Python, Fixed Function OpenGL), Craft (C, Shader-based OpenGL)

  * Minecraft-like game written in modern opengl.

  * https://github.com/billsix/Minecraft  (the Python version)  (based off of https://github.com/fogleman/Minecraft , updated for Visual Studio 2019, and modern Python)

  * https://github.com/fogleman/Craft  (the C version)

  * Verified Builds:
    * Linux

* KGabis's raytraces

  * Small raytracer in C

  * https://github.com/kgabis/raytracer

  * Verified Builds:
    * Linux


* glPortal
   * A game like Valve's Portal

   * https://github.com/GlPortal/glPortal.git
   * https://glportal.de/

* SuperTuxKart
   * MarioKart-style game

   * https://supertuxkart.net/Main_Page

* VVVVVVV
   * Recently open-sourced game from a decade ago.

   * https://github.com/TerryCavanagh/VVVVVV.git
   * http://distractionware.com/blog/2020/01/vvvvvv-is-now-open-source/


* Neverball
  * A SuperMonkeyBall-like game.  Large project

  * https://github.com/Neverball/neverball

* Amsimeos rubik's cube

  * https://github.com/amsimoes/rubik-opengl


Other great resources


For further information, such as lighting, shadows, and
OpenGL in more explicit detail, consult
1) OpenGL redbook/bluebook. (OpenGL superbible v4, because it covers fixed function and shaders)
2) Mathematics for 3D Game Programming and Computer Graphics
3) Computer Graphics: Principles and Practice in C (2nd Edition)

For RayTracing
1) Physically Based Rendering
2) Ray Tracing from the Ground Up
