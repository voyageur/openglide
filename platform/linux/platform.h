//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*             Linux Specific includes and macros
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <string.h>
#define __STDC_LIMIT_MACROS
#include <stdint.h> // Ansi C99 header
#include <stdlib.h>
#include <unistd.h>
#define _strtime(s) {time_t t = time(0); strftime(s, 99, "%H:%M:%S", localtime (&t));}
#define _strdate(s) {time_t t = time(0); strftime(s, 99, "%d %b %Y", localtime (&t));}
#define ZeroMemory(d,l) memset(d,0,l)
#define CopyMemory(d,s,l) memcpy(d,s,l)
#define max(x,y) ((x) < (y) ? (y) : (x))
#define __int64  int64_t
#define __uint64 uint64_t
#define __fastcall __attribute__((__fastcall__))

#define VARARGDECL(t) extern "C" t
typedef void (*ExtFn)();

#endif
