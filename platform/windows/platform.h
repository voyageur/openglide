//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*           Windows specific includes and macros
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <windows.h>
#include <io.h>

#define __uint64 unsigned __int64
#define __UINT64_C(c) c

#define VARARGDECL(t) t _cdecl
typedef int (__stdcall *ExtFn)();

#endif
