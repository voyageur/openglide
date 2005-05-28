//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*                  Color and Alpha File
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#include <math.h>

#include "GlOgl.h"
#include "GLRender.h"
#include "Glextensions.h"
#include "PGTexture.h"
#include "OGLTables.h"


//*************************************************
//* Sets the Dithering Mode 24->16 bits
//*************************************************
FX_ENTRY void FX_CALL
grDitherMode( GrDitherMode_t mode )
{
#ifdef OGL_DONE
    GlideMsg( "grDitherMode( %d )\n", mode );
#endif

    RenderDrawTriangles( );

    Glide.State.DitherMode = mode;

    if ( mode != GR_DITHER_DISABLE )
    {
        // GR_DITHER_2x2 or GR_DITHER_4x4
        glEnable( GL_DITHER );
    }
    else
    {
        glDisable( GL_DITHER );
    }

#ifdef OPENGL_DEBUG
    GLErro( "grDitherMode" );
#endif
}

//*************************************************
//* Sets the Constant color
//*************************************************
FX_ENTRY void FX_CALL
grConstantColorValue( GrColor_t value )
{
#ifdef OGL_DONE
    GlideMsg( "grConstantColorValue( 0x%X )\n", value );
#endif

    Glide.State.ConstantColorValue = value;
    ConvertColorF( value, 
                   OpenGL.ConstantColor[ 0 ], 
                   OpenGL.ConstantColor[ 1 ], 
                   OpenGL.ConstantColor[ 2 ], 
                   OpenGL.ConstantColor[ 3 ] );
}


//*************************************************
//* Sets the Constant color
//*************************************************
FX_ENTRY void FX_CALL
grConstantColorValue4( float a, float r, float g, float b )
{
#ifdef OGL_DONE
    GlideMsg( "grConstantColorValue4( %f, %f, %f, %f )\n", a, r, g, b );
#endif

    Glide.State.ConstantColorValue = ConvertConstantColor( r, g, b, a );
    OpenGL.ConstantColor[ 0 ] = r * D1OVER255;
    OpenGL.ConstantColor[ 1 ] = g * D1OVER255;
    OpenGL.ConstantColor[ 2 ] = b * D1OVER255;
    OpenGL.ConstantColor[ 3 ] = a * D1OVER255;
}

//*************************************************
//* Sets the Color and Alpha mask
//*************************************************
FX_ENTRY void FX_CALL
grColorMask( FxBool rgb, FxBool a )
{
#ifdef OGL_DONE
    GlideMsg( "grColorMask( %s, %s )\n", 
        rgb ? "TRUE" : "FALSE", a  ? "TRUE" : "FALSE" );
#endif

    RenderDrawTriangles( );

    Glide.State.ColorMask = rgb;
    Glide.State.AlphaMask = a;
    OpenGL.ColorMask = rgb;

    glColorMask( rgb, rgb, rgb, a );

#ifdef OPENGL_DEBUG
    GLErro( "grColorMask" );
#endif
}

FX_ENTRY void FX_CALL
grColorCombine( GrCombineFunction_t function, GrCombineFactor_t factor,
                GrCombineLocal_t local, GrCombineOther_t other,
                FxBool invert )
{
#if defined( OGL_PARTDONE ) || defined( OGL_COMBINE )
    GlideMsg( "grColorCombine( %d, %d, %d, %d, %s )\n",
        function, factor, local, other, invert ? "TRUE" : "FALSE" );
#endif

    RenderDrawTriangles( );

    Glide.State.ColorCombineFunction    = function;
    Glide.State.ColorCombineFactor      = factor;
    Glide.State.ColorCombineLocal       = local;
    Glide.State.ColorCombineOther       = other;
    Glide.State.ColorCombineInvert      = invert;
    
    Glide.CLocal = colorCombineTable[ factor ][ function ].local;
    Glide.COther = colorCombineTable[ factor ][ function ].other;
    if ( colorCombineTable[ factor ][ function ].alocal )
    {
        Glide.ALocal = true;
    }
    if ( colorCombineTable[ factor ][ function ].aother )
    {
        Glide.AOther = true;
    }
    ColorFunctionFunc = colorCombineTable[ factor ][ function ].func;
    ColorFactor3Func = colorCombineTable[ factor ][ function ].factorfunc;

    // Need to verify the statement below because of the factor will only be used if there
    // is need, so we may need not to enable the texture
    if ( ( Glide.COther &&
           ( other == GR_COMBINE_OTHER_TEXTURE ) &&
           ( Glide.State.TextureCombineCFunction != GR_COMBINE_FUNCTION_ZERO ) ) ||
         ( ( factor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
           ( factor == GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA ) ||
           ( factor == GR_COMBINE_FACTOR_TEXTURE_RGB ) ) )
    {
        OpenGL.ColorTexture = true;
    }
    else
    {
        OpenGL.ColorTexture = false;
    }

    if ( ( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) &&
         ( Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
    {
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    }
    else
    if ( ( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB ) &&
         ( Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
    {
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
    }
    else
/*    if (( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_ONE ) &&
        ( Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) &&
        ( ( Glide.State.ColorCombineFunction == GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA ) || 
        ( Glide.State.ColorCombineFunction == GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL ) ) )
    {
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
    }
    else*/
    {
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    }

    OpenGL.Texture = ( OpenGL.ColorTexture || ( OpenGL.Blend && OpenGL.AlphaTexture ) );
}

//*************************************************
FX_ENTRY void FX_CALL
guColorCombineFunction( GrColorCombineFnc_t fnc )
{
#if defined( OGL_PARTDONE ) || defined( OGL_COMBINE )
    GlideMsg( "guColorCombineFunction( %d )\n", fnc );
#endif

    switch ( fnc )
    {
        case GR_COLORCOMBINE_ZERO:                              //0x0
            grColorCombine( GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_NONE,
                        GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_NONE, FXFALSE );
            break;

        case GR_COLORCOMBINE_CCRGB:                             //0x1
            grColorCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, 
                GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_NONE, FXFALSE );
            break;

        case GR_COLORCOMBINE_ITRGB:                             //0x2
            grColorCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
                        GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE );
            break;

        case GR_COLORCOMBINE_DECAL_TEXTURE:                     //0x4
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, 
                GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_TEXTURE_TIMES_CCRGB:               //0x5
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, 
                GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB:               //0x6
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_ADD_ALPHA:     //0x8
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA, GR_COMBINE_FACTOR_LOCAL, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_TEXTURE_TIMES_ALPHA:               //0x9
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL_ALPHA, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_TEXTURE_TIMES_ALPHA_ADD_ITRGB:     //0xa
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_LOCAL_ALPHA, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_TEXTURE_ADD_ITRGB:                 //0xb
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_ONE, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_TEXTURE_SUB_ITRGB:                 //0xc
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL, GR_COMBINE_FACTOR_ONE, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_DIFF_SPEC_A:                       //0xe
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_LOCAL_ALPHA, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_DIFF_SPEC_B:                       //0xf
            grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA, GR_COMBINE_FACTOR_LOCAL, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_COLORCOMBINE_ONE:                               //0x10
            grColorCombine( GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ONE,
                        GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXTRUE );
            break;

        case GR_COLORCOMBINE_ITRGB_DELTA0:                      //0x3
            grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
                        GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_CONSTANT, FXFALSE);
            break;

        case GR_COLORCOMBINE_TEXTURE_TIMES_ITRGB_DELTA0:        //0x7
        case GR_COLORCOMBINE_CCRGB_BLEND_ITRGB_ON_TEXALPHA:     //0xd
            break;
    }
}

//*************************************************
//* Sets the Alpha Test Reference Value
//*************************************************
FX_ENTRY void FX_CALL
grAlphaTestReferenceValue( GrAlpha_t value )
{
#ifdef OGL_DONE
    GlideMsg( "grAlphaTestReferenceValue( %d )\n", value );
#endif

    RenderDrawTriangles( );

    Glide.State.AlphaReferenceValue = value;
    OpenGL.AlphaReferenceValue = value * D1OVER255;

    glAlphaFunc( OpenGL.AlphaTestFunction, OpenGL.AlphaReferenceValue );

#ifdef OPENGL_DEBUG
    GLErro( "grAlphaTestReferenceValue" );
#endif
}

//*************************************************
//* Sets the Alpha Test Function
//*************************************************
FX_ENTRY void FX_CALL
grAlphaTestFunction( GrCmpFnc_t function )
{
#ifdef OGL_DONE
    GlideMsg( "grAlphaTestFunction( %d )\n", function );
#endif

    RenderDrawTriangles( );

    Glide.State.AlphaTestFunction = function;

    // We can do this just because we know the constant values for both OpenGL and Glide
    // To port it to anything else than OpenGL we NEED to change this code
    OpenGL.AlphaTestFunction = GL_NEVER + function;

    glEnable( GL_ALPHA_TEST );
    glAlphaFunc( OpenGL.AlphaTestFunction, OpenGL.AlphaReferenceValue );

#ifdef OPENGL_DEBUG
    GLErro( "grAlphaTestFunction" );
#endif
}

//*************************************************
FX_ENTRY void FX_CALL
grAlphaBlendFunction( GrAlphaBlendFnc_t rgb_sf,   GrAlphaBlendFnc_t rgb_df,
                      GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df )
{
#ifdef OGL_PARTDONE
    GlideMsg( "grAlphaBlendFunction( %d, %d, %d, %d )\n",
        rgb_sf, rgb_df, alpha_sf, alpha_df );
#endif

    RenderDrawTriangles( );

    Glide.State.AlphaBlendRgbSf     = rgb_sf;
    Glide.State.AlphaBlendRgbDf     = rgb_df;
    Glide.State.AlphaBlendAlphaSf   = alpha_sf;
    Glide.State.AlphaBlendAlphaDf   = alpha_df;

    switch ( rgb_sf )
    {
    case GR_BLEND_ZERO:                 OpenGL.SrcBlend = GL_ZERO;                  break;
    case GR_BLEND_ONE:                  OpenGL.SrcBlend = GL_ONE;                   break;
    case GR_BLEND_DST_COLOR:            OpenGL.SrcBlend = GL_DST_COLOR;             break;
    case GR_BLEND_ONE_MINUS_DST_COLOR:  OpenGL.SrcBlend = GL_ONE_MINUS_DST_COLOR;   break;
    case GR_BLEND_SRC_ALPHA:            OpenGL.SrcBlend = GL_SRC_ALPHA;             break;
    case GR_BLEND_ONE_MINUS_SRC_ALPHA:  OpenGL.SrcBlend = GL_ONE_MINUS_SRC_ALPHA;   break;
    case GR_BLEND_DST_ALPHA:            OpenGL.SrcBlend = GL_DST_ALPHA;             break;
    case GR_BLEND_ONE_MINUS_DST_ALPHA:  OpenGL.SrcBlend = GL_ONE_MINUS_DST_ALPHA;   break;
    case GR_BLEND_ALPHA_SATURATE:       OpenGL.SrcBlend = GL_SRC_ALPHA_SATURATE;    break;

#ifdef OGL_DEBUG
    default:
        Error( "grAlphaBlendFunction: Unknow RGB source blend factor.\n" );
        OpenGL.SrcBlend = GL_ONE;
        break;
#endif
    }

    switch ( rgb_df )
    {
    case GR_BLEND_ZERO:                 OpenGL.DstBlend = GL_ZERO;                  break;
    case GR_BLEND_ONE:                  OpenGL.DstBlend = GL_ONE;                   break;
    case GR_BLEND_SRC_COLOR:            OpenGL.DstBlend = GL_SRC_COLOR;             break;
    case GR_BLEND_ONE_MINUS_SRC_COLOR:  OpenGL.DstBlend = GL_ONE_MINUS_SRC_COLOR;   break;
    case GR_BLEND_SRC_ALPHA:            OpenGL.DstBlend = GL_SRC_ALPHA;             break;
    case GR_BLEND_ONE_MINUS_SRC_ALPHA:  OpenGL.DstBlend = GL_ONE_MINUS_SRC_ALPHA;   break;
    case GR_BLEND_DST_ALPHA:            OpenGL.DstBlend = GL_DST_ALPHA;             break;
    case GR_BLEND_ONE_MINUS_DST_ALPHA:  OpenGL.DstBlend = GL_ONE_MINUS_DST_ALPHA;   break;
    case GR_BLEND_PREFOG_COLOR:         OpenGL.DstBlend = GL_ONE;                   break;

#ifdef OGL_DEBUG
    default:
        Error( "grAlphaBlendFunction: Unknow RGB destination blend factor.\n" );
        OpenGL.DstBlend = GL_ZERO;
        break;
#endif
    }

    switch ( alpha_sf )
    {
    case GR_BLEND_ZERO:                 OpenGL.SrcAlphaBlend = GL_ZERO;                 break;
    case GR_BLEND_ONE:                  OpenGL.SrcAlphaBlend = GL_ONE;                  break;
    case GR_BLEND_DST_COLOR:            OpenGL.SrcAlphaBlend = GL_DST_COLOR;            break;
    case GR_BLEND_ONE_MINUS_DST_COLOR:  OpenGL.SrcAlphaBlend = GL_ONE_MINUS_DST_COLOR;  break;
    case GR_BLEND_SRC_ALPHA:            OpenGL.SrcAlphaBlend = GL_SRC_ALPHA;            break;
    case GR_BLEND_ONE_MINUS_SRC_ALPHA:  OpenGL.SrcAlphaBlend = GL_ONE_MINUS_SRC_ALPHA;  break;
    case GR_BLEND_DST_ALPHA:            OpenGL.SrcAlphaBlend = GL_DST_ALPHA;            break;
    case GR_BLEND_ONE_MINUS_DST_ALPHA:  OpenGL.SrcAlphaBlend = GL_ONE_MINUS_DST_ALPHA;  break;
    case GR_BLEND_ALPHA_SATURATE:       OpenGL.SrcAlphaBlend = GL_SRC_ALPHA_SATURATE;   break;
    }

    switch ( alpha_df )
    {
    case GR_BLEND_ZERO:                 OpenGL.DstAlphaBlend = GL_ZERO;                 break;
    case GR_BLEND_ONE:                  OpenGL.DstAlphaBlend = GL_ONE;                  break;
    case GR_BLEND_SRC_COLOR:            OpenGL.DstAlphaBlend = GL_SRC_COLOR;            break;
    case GR_BLEND_ONE_MINUS_SRC_COLOR:  OpenGL.DstAlphaBlend = GL_ONE_MINUS_SRC_COLOR;  break;
    case GR_BLEND_SRC_ALPHA:            OpenGL.DstAlphaBlend = GL_SRC_ALPHA;            break;
    case GR_BLEND_ONE_MINUS_SRC_ALPHA:  OpenGL.DstAlphaBlend = GL_ONE_MINUS_SRC_ALPHA;  break;
    case GR_BLEND_DST_ALPHA:            OpenGL.DstAlphaBlend = GL_DST_ALPHA;            break;
    case GR_BLEND_ONE_MINUS_DST_ALPHA:  OpenGL.DstAlphaBlend = GL_ONE_MINUS_DST_ALPHA;  break;
    case GR_BLEND_PREFOG_COLOR:         OpenGL.DstAlphaBlend = GL_ONE;                  break;
    }

//    if ( ! InternalConfig.BlendFuncSeparateEXTEnable )
//    {
        glBlendFunc( OpenGL.SrcBlend, OpenGL.DstBlend );
//    }
//    else
//    {
//        p_glBlendFuncSeparateEXT( OpenGL.SrcBlend, OpenGL.DstBlend, 
//                                  OpenGL.SrcAlphaBlend, OpenGL.DstAlphaBlend );
//    }

    OpenGL.Blend = !(( rgb_sf == GR_BLEND_ONE ) && ( rgb_df == GR_BLEND_ZERO ));

    OpenGL.Texture = ( OpenGL.ColorTexture || ( OpenGL.Blend && OpenGL.AlphaTexture ) );

#ifdef OPENGL_DEBUG
    GLErro( "grAlphaBlendFunction" );
#endif
}

//*************************************************
FX_ENTRY void FX_CALL
grAlphaCombine( GrCombineFunction_t function, GrCombineFactor_t factor,
                GrCombineLocal_t local, GrCombineOther_t other,
                FxBool invert )
{
#if defined( OGL_PARTDONE ) || defined( OGL_COMBINE )
    GlideMsg( "grAlphaCombine( %d, %d, %d, %d, %s )\n",
        function, factor, local, other, invert ? "TRUE" : "FALSE" );
#endif

    RenderDrawTriangles( );

    Glide.State.AlphaFunction = function;
    Glide.State.AlphaFactor = factor;
    Glide.State.AlphaLocal = local;
    Glide.State.AlphaOther = other;
    Glide.State.AlphaInvert = invert;

    Glide.ALocal = alphaCombineTable[ factor ][ function ].local;
    Glide.AOther = alphaCombineTable[ factor ][ function ].other;
    AlphaFactorFunc = alphaCombineTable[ factor ][ function ].func;

    if ( Glide.AOther && ( other == GR_COMBINE_OTHER_TEXTURE ) )
    {
        OpenGL.AlphaTexture = true;
    }
    else
    {
        OpenGL.AlphaTexture = false;
    }

    OpenGL.Texture = ( OpenGL.ColorTexture || ( OpenGL.Blend && OpenGL.AlphaTexture ) );
}

//*************************************************
FX_ENTRY void FX_CALL
grAlphaControlsITRGBLighting( FxBool enable )
{
#ifdef OGL_NOTDONE
    GlideMsg("grAlphaControlsTGBALighting( %s )\n", enable ? "TRUE" : "FALSE" );
#endif
}

//*************************************************
FX_ENTRY void FX_CALL
guAlphaSource( GrAlphaSource_t dwMode )
{
#if defined( OGL_PARTDONE ) || defined( OGL_COMBINE )
    GlideMsg( "guAlphaSource( %d )\n", dwMode );
#endif

    switch ( dwMode )
    {
        case GR_ALPHASOURCE_CC_ALPHA:                               //0x00
            grAlphaCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, 
                GR_COMBINE_LOCAL_CONSTANT, GR_COMBINE_OTHER_NONE, FXFALSE );
            break;

        case GR_ALPHASOURCE_ITERATED_ALPHA:                         //0x01
            grAlphaCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE );
            break;

        case GR_ALPHASOURCE_TEXTURE_ALPHA:                          //0x02
            grAlphaCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, 
                GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_TEXTURE, FXFALSE );
            break;

        case GR_ALPHASOURCE_TEXTURE_ALPHA_TIMES_ITERATED_ALPHA:     //0x03
            grAlphaCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, 
                GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE ); 
            break;
    }
}

//*************************************************
//* Sets the ChromaKey Value for comparision
//*************************************************
FX_ENTRY void FX_CALL
grChromakeyValue( GrColor_t value )
{
#ifdef OGL_PARTDONE
    GlideMsg( "grChromakeyValue( 0x%X )\n", value );
#endif

    RenderDrawTriangles( );

    ConvertColor4B( value, OpenGL.ChromaColor );

//    ConvertColorB(  value, 
//                    OpenGL.ChromaColor.R, 
//                    OpenGL.ChromaColor.G, 
//                    OpenGL.ChromaColor.B, 
//                    OpenGL.ChromaColor.A );

//    Textures->ChromakeyValue( value );
    Textures->ChromakeyValue( OpenGL.ChromaColor );

    Glide.State.ChromakeyValue = value;
}

//*************************************************
//* Sets the ChromaKey Mode
//*************************************************
FX_ENTRY void FX_CALL
grChromakeyMode( GrChromakeyMode_t mode )
{
#ifdef OGL_PARTDONE
    GlideMsg( "grChromakeyMode( %s )\n", mode ? "TRUE" : "FALSE" );
#endif

    RenderDrawTriangles( );

    Textures->ChromakeyMode( mode );

    Glide.State.ChromaKeyMode = mode;

    if ( mode == GR_CHROMAKEY_ENABLE )
    {
        OpenGL.ChromaKey = true;
    }
    else
    {
        OpenGL.ChromaKey = false;
    }
}

//*************************************************
FX_ENTRY void FX_CALL
grGammaCorrectionValue( float value )
{
#ifdef OGL_PARTDONE
    GlideMsg( "grGammaCorrectionValue( %f )\n", value );
#endif
    RenderDrawTriangles();

    OpenGL.Gamma = value;

    {
        struct
        {
            WORD red[256];
            WORD green[256];
            WORD blue[256];
        } ramp;
        int i;
        HDC pDC = GetDC( NULL );

        for ( i = 0; i < 256; i++ )
        {
            WORD v = (WORD)( 0xffff * pow( i / 255.0, 1.0 / value ) );

            ramp.red[ i ] = ramp.green[ i ] = ramp.blue[ i ] = ( v & 0xff00 );
        }

        BOOL res = SetDeviceGammaRamp( pDC, &ramp );

        ReleaseDC( NULL, pDC );
    }

#ifdef OPENGL_DEBUG
    GLErro( "grGammaCorrectionValue" );
#endif
}

