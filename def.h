/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"
 */
#ifndef __DEF_H
#define __DEF_H

#ifdef _WIN32
#include <windows.h>
#endif

typedef char			s8;
#ifdef _WIN32
typedef __int16			s16;
typedef __int32			s32;
typedef __int64			s64;
#else
typedef short			s16;
typedef long			s32;
typedef long long		s64;
#endif
typedef unsigned char		u8;
#ifdef _WIN32
typedef unsigned __int16	u16;
typedef unsigned __int32	u32;
typedef unsigned __int64	u64;
#else
typedef unsigned short		u16;
typedef unsigned long		u32;
typedef unsigned long long	u64;
#endif
typedef float			f32;
typedef double			f64;

typedef struct {u8 r, g, b, a;} COLOR;;
#define MALLOC malloc

#define SET_COLOR(c, rIn, gIn, bIn, aIn) \
	c.r = rIn; \
	c.g = gIn; \
	c.b = bIn; \
	c.a = aIn

#endif
