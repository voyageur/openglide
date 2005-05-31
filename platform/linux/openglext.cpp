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

#include <GL/glx.h>

#include "platform/openglext.h"

ExtFn OGLGetProcAddress(const char *x)
{
#ifndef GL_VERSION_1_4
    return glXGetProcAddressARB((const GLubyte *) x);
#else
    return glXGetProcAddress((const GLubyte *) x);
#endif
}

