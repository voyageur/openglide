
#ifndef __OGL_TABLES_H__
#define __OGL_TABLES_H__

#include "glogl.h"
#include "GLRender.h"

typedef struct 
{
    float h;
    float w;
} OGLAspect;

typedef struct
{
    bool                local;
    bool                other;
    ALPHAFACTORFUNCPROC func;
} OGLAlphaTable;

typedef struct
{
    bool                local;
    bool                other;
    bool                alocal;
    bool                aother;
    COLORFACTORFUNCPROC factorfunc;
    COLORFUNCTIONPROC   func;
} OGLColorTable;

// OGLTextureTables.cpp
extern OGLAspect texAspects[ 7 ];
extern DWORD nSquareLod[ 2 ][ 7 ][ 9 ];
extern DWORD nSquareTexLod[ 2 ][ 7 ][ 9 ][ 9 ];

// OGLColorAlphaTables.cpp
extern OGLAlphaTable alphaCombineTable[ 14 ][ 17 ];
extern OGLColorTable colorCombineTable[ 14 ][ 17 ];

#endif
