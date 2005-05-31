//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*  Platform specific functions for handling OpenGL extensions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#ifndef __OPENGLEXT_H__
#define __OPENGLEXT_H__

typedef void (*ExtFn)();

ExtFn OGLGetProcAddress(const char *x);

#endif
