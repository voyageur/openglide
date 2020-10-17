//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*           Windows specific includes and macros
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originally made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifdef WIN32

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <windows.h>
#include <io.h>

#define max(x,y) ((x) < (y) ? (y) : (x))

#define FASTCALL __fastcall

#define VARARGDECL(t) t _cdecl
typedef int (__stdcall *ExtFn)();

#ifdef _MSC_VER
typedef __int64  FxI64;
typedef unsigned __int64 FxU64;
typedef int FxI;
typedef unsigned int FxU;
#endif

#endif

#endif // WIN32
