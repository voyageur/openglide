//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*   Platform specific functions for handling display window
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "sdk2_3dfx.h"

// Include proper OpenGL headers
#ifdef C_USE_SDL
#include "SDL_opengl.h"
#elif defined(__MACOSX__)
#include <OpenGL/gl.h>	/* Header File For The OpenGL Library */
#include <OpenGL/glu.h>	/* Header File For The GLU Library */
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

bool InitialiseOpenGLWindow(FxU wnd, int x, int y, int width, int height);
void FinaliseOpenGLWindow( void);

void SetGamma(float value);
void RestoreGamma();

bool SetScreenMode(int &xsize, int &ysize);
void ResetScreenMode();

void SwapBuffers();

#endif
