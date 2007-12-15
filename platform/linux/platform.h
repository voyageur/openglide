//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*             Linux Specific includes and macros
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
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

#if HAVE_MMX
# if SIZEOF_INT_P == 4
#  define FASTCALL __attribute__((__fastcall__))
# else
#  define FASTCALL
# endif
#define OG_memcpy MMXCopyMemory
#else
#define FASTCALL
#define OG_memcpy memcpy
#endif

#define VARARGDECL(t) extern "C" t
typedef void (*ExtFn)();

#endif
