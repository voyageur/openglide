//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*                  TexDB Class Definition
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#ifndef __TEXDB_H__
#define __TEXDB_H__

#include <windows.h>
#include "glide.h"
#include "GL/gl.h"

class TexDB  
{
public:
    void Clear( void );

    struct Record
    {
        FxU32 startAddress;
        FxU32 endAddress;
        GrTexInfo info;
        FxU32 hash;
        GLuint texNum;
        GLuint tex2Num;
        Record *next;

        Record( bool two_tex )
        {
            glGenTextures( 1, &texNum );

            if ( two_tex )
            {
                glGenTextures( 1, &tex2Num );
            }
            else
            {
                tex2Num = 0;
            }
        };

        ~Record( void )
        {
            glDeleteTextures( 1, &texNum );

            if ( tex2Num != 0 )
            {
                glDeleteTextures( 1, &tex2Num );
            }
        };

        bool Match( FxU32 stt, GrTexInfo *inf, FxU32 h )
        {
            return (startAddress == stt
                && inf->largeLod == info.largeLod
                && inf->aspectRatio == info.aspectRatio
                && inf->format == info.format
                && (hash == h || h == 0));
        };
    };
    void Add( FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 hash, GLuint *pTexNum, GLuint *pTex2Num );
    void WipeRange( FxU32 startAddress, FxU32 endAddress, FxU32 hash );
    bool Find( FxU32 startAddress, GrTexInfo *info, FxU32 hash, 
               GLuint *pTexNum, GLuint *pTex2Num, bool *pal_change );
    TexDB( void );
    virtual ~TexDB( void );

private:
    enum { TEX_SECTIONS = 256 };
    Record *m_first[ TEX_SECTIONS ];
};

#endif
