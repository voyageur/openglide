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
#ifdef WIN32

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <windows.h>
#include <io.h>

#define FASTCALL __fastcall
#define OG_memcpy MMXCopyMemory

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
