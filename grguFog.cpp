//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*                      Fog Functions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#include <math.h>

#include "GlOgl.h"
#include "GLRender.h"
#include "OGLTables.h"


//*************************************************
//* download a fog table
//* Fog is applied after color combining and before alpha blending.
//*************************************************
DLLEXPORT void __stdcall
grFogTable( const GrFog_t *ft )
{
#ifdef OGL_DONE
    GlideMsg( "grFogTable( --- )\n" );
#endif

    if ( InternalConfig.FogEnable )
    {
        MMXCopyMemory( Glide.FogTable, (GrFog_t *)ft, GR_FOG_TABLE_SIZE * sizeof( FxU8 ) );
//        CopyMemory( Glide.FogTable, ft, GR_FOG_TABLE_SIZE * sizeof( FxU8 ) );
        Glide.FogTable[ GR_FOG_TABLE_SIZE ] = 255;

        for ( DWORD i = 0; i < GR_FOG_TABLE_SIZE; i++ )
        {
            for ( DWORD j = intStartEnd[ i ]; j < intStartEnd[ i + 1 ]; j++ )
            {
                OpenGL.FogTable[ j ] = (BYTE)( Glide.FogTable[ i ] + 
                    ( Glide.FogTable[ i + 1 ] - Glide.FogTable[ i ] ) * ( j - intStartEnd[ i ] ) / 
                    intEndMinusStart[ i ] );
            }
        }
    }
}

//*************************************************
DLLEXPORT void __stdcall
grFogColorValue( GrColor_t fogcolor )
{
#ifdef OGL_PARTDONE
    GlideMsg( "grFogColorValue( %x )\n", fogcolor );
#endif

    RenderDrawTriangles( );

    Glide.State.FogColorValue = fogcolor;
    ConvertColorF( fogcolor, 
                   OpenGL.FogColor[ 0 ], 
                   OpenGL.FogColor[ 1 ], 
                   OpenGL.FogColor[ 2 ], 
                   OpenGL.FogColor[ 3 ] );
    glFogfv( GL_FOG_COLOR, &OpenGL.FogColor[0] );
}

//*************************************************
DLLEXPORT void __stdcall
grFogMode( GrFogMode_t mode )
{
#ifdef OGL_PARTDONE
    GlideMsg( "grFogMode( %d )\n", mode );
#endif

    static GrFogMode_t  modeSource, 
                        modeAdd;
    static GLfloat      ZeroColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };

    RenderDrawTriangles( );

    modeSource = mode & ( GR_FOG_WITH_TABLE | GR_FOG_WITH_ITERATED_ALPHA );
    modeAdd = mode & ( GR_FOG_MULT2 | GR_FOG_ADD2 );

    if ( modeSource )
    {
        OpenGL.Fog = true;
        if ( InternalConfig.EXT_fog_coord )
        {
            glEnable( GL_FOG );
        }
    }
    else
    {
        OpenGL.Fog = false;
        glDisable( GL_FOG );
    }

    switch ( modeAdd )
    {
    case GR_FOG_MULT2:
    case GR_FOG_ADD2:
        glFogfv( GL_FOG_COLOR, &ZeroColor[ 0 ] );
        break;

    default:
        glFogfv( GL_FOG_COLOR, &OpenGL.FogColor[ 0 ] );
        break;
    }
    
    Glide.State.FogMode = modeSource;
//    Glide.State.FogMode = mode;
}

//*************************************************
DLLEXPORT void __stdcall
guFogGenerateExp( GrFog_t *fogtable, float density )
{
#ifdef OGL_PARTDONE
    GlideMsg( "guFogGenerateExp( ---, %-4.2f )\n", density );
#endif
    
    float f;
    float scale;
    float dp;
    
    dp = density * guFogTableIndexToW( GR_FOG_TABLE_SIZE - 1 );
    scale = 255.0F / ( 1.0F - (float) exp( -dp ) );
    
    for ( int i = 0; i < GR_FOG_TABLE_SIZE; i++ )
    {
        dp = density * guFogTableIndexToW( i );
        f = ( 1.0F - (float) exp( -dp ) ) * scale;
        
        if ( f > 255.0F )
        {
            f = 255.0F;
        }
        else if ( f < 0.0F )
        {
            f = 0.0F;
        }
        
        fogtable[ i ] = (GrFog_t) f;
    }
}

//*************************************************
DLLEXPORT void __stdcall
guFogGenerateExp2( GrFog_t *fogtable, float density )
{
#ifdef OGL_PARTDONE
    GlideMsg( "guFogGenerateExp2( ---, %-4.2f )\n", density );
#endif

    float Temp;

    for ( int i = 0; i < GR_FOG_TABLE_SIZE; i++ )
    {
        Temp = ( 1.0f - (float) exp( ( -density)  * guFogTableIndexToW( i ) ) * 
               (float)exp( (-density)  * guFogTableIndexToW( i ) ) )  * 255.0f;
        fogtable[ i ] = (BYTE) Temp;
    }
}

//*************************************************
DLLEXPORT void __stdcall
guFogGenerateLinear( GrFog_t *fogtable,
                     float nearZ, float farZ )
{
#ifdef OGL_PARTDONE
    GlideMsg( "guFogGenerateLinear( ---, %-4.2f, %-4.2f )\n", nearZ, farZ );
#endif

    int Start, 
        End, 
        i;

    for( Start = 0; Start < GR_FOG_TABLE_SIZE; Start++ )
    {
        if ( guFogTableIndexToW( Start ) >= nearZ )
        {
            break;
        }
    }
    for( End = 0; End < GR_FOG_TABLE_SIZE; End++ )
    {
        if ( guFogTableIndexToW( End ) >= farZ )
        {
            break;
        }
    }

    ZeroMemory( fogtable, GR_FOG_TABLE_SIZE );
    for( i = Start; i <= End; i++ )
    {
        fogtable[ i ] = (BYTE)((float)( End - Start ) / 255.0f * (float)( i - Start ));
    }

    for( i = End; i < GR_FOG_TABLE_SIZE; i++ )
    {
        fogtable[ i ] = 255;
    }
}

//*************************************************
//* convert a fog table index to a floating point eye-space w value
//*************************************************
DLLEXPORT float __stdcall
guFogTableIndexToW( int i )
{
#ifdef OGL_DONE
    GlideMsg( "guFogTableIndexToW( %d )\n", i );
#endif
#ifdef OGL_DEBUG
    if ( ( i < 0 ) ||
         ( i >= GR_FOG_TABLE_SIZE ) )
    {
        Error( "Error on guFogTableIndexToW( %d )\n", i );
    }
#endif

    return tableIndexToW[ i ];
}

