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

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "sdk2_3dfx.h"

void InitialiseOpenGLWindow(FxU32 wnd, int x, int y, int width, int height);
void FinaliseOpenGLWindow( void);

void SetGamma(float value);
void RestoreGamma();

bool SetScreenMode(int &xsize, int &ysize);
void ResetScreenMode();

void SwapBuffers();

#endif
