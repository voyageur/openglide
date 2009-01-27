//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*   Linux specific functions for handling OpenGL extensions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#include "sdk2_3dfx.h"

#ifndef C_USE_SDL
#define GLX_GLXEXT_PROTOTYPES

#include <GL/glx.h>

#include "platform/openglext.h"

ExtFn OGLGetProcAddress(const char *x)
{
#ifdef GLX_ARB_get_proc_address
    return glXGetProcAddressARB((const GLubyte *) x);
#else
    return glXGetProcAddress((const GLubyte *) x);
#endif
}

#endif // C_USE_SDL
