//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*                PGTexture Class Definition
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#ifndef __PGTEXTURE_H__
#define __PGTEXTURE_H__

#include "Glide.h"
#include "TexDB.h"

class PGTexture  
{
    struct TexValues
    {
        GrLOD_t lod;
        FxU32 width;
        FxU32 height;
        FxU32 nPixels;
    };

private:
    bool m_palette_dirty;
    FxU32 m_palette_hash;
    void ApplyKeyToPalette( void );
    TexDB m_db;
    GrChromakeyMode_t m_chromakey_mode;
    GrColor_t m_chromakey_value;
    float m_wAspect;
    float m_hAspect;
    void GetTexValues( TexValues *tval );

    FxU32 m_tex_temp[ 256 * 256 ];
    bool m_valid;
    FxU8 *m_memory;
    FxU32 m_startAddress;
    FxU32 m_evenOdd;
    GrTexInfo m_info;
    FxU32 m_palette[ 256 ];

public:
    FxU32 m_tex_memory_size;

    static FxU32 MipMapMemRequired( GrLOD_t lod, GrAspectRatio_t aspectRatio, 
                                    GrTextureFormat_t format );
    void ChromakeyMode( GrChromakeyMode_t mode );
    void ChromakeyValue( GrColor_t value );
    void GetAspect( float *hAspect, float *wAspect );
    void Clear( void );
    static FxU32 TextureMemRequired( FxU32 evenOdd, GrTexInfo *info );
    void MakeReady( void );
    void DownloadTable( GrTexTable_t type, void *data, int first, int count );
    void Source( FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info );
    void DownloadMipMap( FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info );
    PGTexture( int mem_size );
    virtual ~PGTexture();
};

#endif
