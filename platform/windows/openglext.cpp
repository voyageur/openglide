//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*   Windows specific functions for handling OpenGL extensions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef C_USE_SDL

#include <windows.h>

#include "platform/openglext.h"

ExtFn OGLGetProcAddress(const char *x)
{
    return wglGetProcAddress(x);
}

#endif // C_USE_SDL
