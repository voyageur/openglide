
#ifndef __OGL_TABLES_H__
#define __OGL_TABLES_H__

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

extern OGLAlphaTable alphaCombineTable[ 14 ][ 17 ];
extern OGLColorTable colorCombineTable[ 14 ][ 17 ];

#endif
