//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*               Linear Frame Buffer Functions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************

#include "GlOgl.h"
#include "Glextensions.h"
#include "GLRender.h"
#include "FormatConversion.h"


#define BLUE_SCREEN     (0x07FF)

// Will need to change this later
static FxU32 tempBuf[ 2048 * 2048 ];


//*************************************************
FX_ENTRY FxBool FX_CALL
grLfbLock( GrLock_t dwType, 
           GrBuffer_t dwBuffer, 
           GrLfbWriteMode_t dwWriteMode,
           GrOriginLocation_t dwOrigin, 
           FxBool bPixelPipeline, 
           GrLfbInfo_t *lfbInfo )
{ 
#ifdef OGL_CRITICAL
    GlideMsg( "grLfbLock( %d, %d, %d, %d, %d, --- )\n", dwType, dwBuffer, dwWriteMode, dwOrigin, bPixelPipeline ); 
#endif

    RenderDrawTriangles( );

    if ( dwType & 1 )
    {
        Glide.DstBuffer.Lock            = true;
        Glide.DstBuffer.Type            = dwType;
        Glide.DstBuffer.Buffer          = dwBuffer;
        Glide.DstBuffer.WriteMode       = dwWriteMode;
        Glide.DstBuffer.PixelPipeline   = bPixelPipeline;

        lfbInfo->lfbPtr = Glide.DstBuffer.Address;
    }
    else
    {
        FxU32 j;

        glReadBuffer( dwBuffer == GR_BUFFER_BACKBUFFER
                      ? GL_BACK : GL_FRONT );

        glReadPixels( 0, 0,
                      Glide.WindowWidth, Glide.WindowHeight,
                      GL_BGRA, GL_UNSIGNED_BYTE,
                      (void *)tempBuf );

        if ( dwOrigin == GR_ORIGIN_UPPER_LEFT )
        {
            for ( j = 0; j < Glide.WindowHeight; j++ )
            {
                Convert8888to565( tempBuf + ( ( ( Glide.WindowHeight ) - 1 - j ) * Glide.WindowWidth ),
                        Glide.SrcBuffer.Address + ( j * Glide.WindowWidth ),
                        Glide.WindowWidth );
            }
        }
        else
        {
            Convert8888to565( tempBuf, Glide.SrcBuffer.Address, Glide.WindowTotalPixels );
        }    
        Glide.SrcBuffer.Lock            = true;
        Glide.SrcBuffer.Type            = dwType;
        Glide.SrcBuffer.Buffer          = dwBuffer;
        Glide.SrcBuffer.WriteMode       = dwWriteMode;
        Glide.SrcBuffer.PixelPipeline   = bPixelPipeline;

        lfbInfo->lfbPtr = Glide.SrcBuffer.Address;
    }

    lfbInfo->writeMode = GR_LFBWRITEMODE_565;
    lfbInfo->strideInBytes = Glide.WindowWidth * 2;

    return FXTRUE;
}

//*************************************************
FX_ENTRY FxBool FX_CALL
grLfbUnlock( GrLock_t dwType, GrBuffer_t dwBuffer )
{ 
#ifdef OGL_CRITICAL
    GlideMsg("grLfbUnlock( %d, %d )\n", dwType, dwBuffer ); 
#endif
    
    if ( dwType & 1 )
    {
        if ( ! Glide.DstBuffer.Lock )
        {
            return FXFALSE;
        }

        FxU32 ii,
            x,
            y,
            maxx = 0,
            maxy = 0,
            minx = 2048, 
            miny = 2048;

        for ( ii = 0, x = 0, y = 0; ii < Glide.WindowTotalPixels; ii++ )
        {
            if ( Glide.DstBuffer.Address[ ii ] != BLUE_SCREEN )
            {
                if ( x > maxx ) maxx = x;
                if ( y > maxy ) maxy = y;
                if ( x < minx ) minx = x;
                if ( y < miny ) miny = y;

                tempBuf[ ii ] = 0x0    |    // A
                ( Glide.DstBuffer.Address[ ii ] & 0x001F ) << 19 |  // B
                ( Glide.DstBuffer.Address[ ii ] & 0x07E0 ) << 5  |  // G
                ( Glide.DstBuffer.Address[ ii ] >> 8 );             // R
                Glide.DstBuffer.Address[ ii ] = BLUE_SCREEN;
            } else
                tempBuf[ ii ] = 0xFFFFFFFF;

            x++;
            if ( x == Glide.WindowWidth )
            {
                x = 0;
                y++;
            }
        }

        if ( maxx >= minx )
        {
            maxx++; maxy++;
            FxU32 xsize = maxx - minx;
            FxU32 ysize = maxy - miny;

            // Draw a textured quad
            glPushAttrib( GL_COLOR_BUFFER_BIT|GL_TEXTURE_BIT|GL_DEPTH_BUFFER_BIT );

            glDisable( GL_BLEND );
            glEnable( GL_TEXTURE_2D );

            if ( Glide.DstBuffer.PixelPipeline )
                glEnable( GL_SCISSOR_TEST );

            glAlphaFunc( GL_EQUAL, 0.0f );
            glEnable( GL_ALPHA_TEST );

            glDepthMask( GL_FALSE );
            glDisable( GL_DEPTH_TEST );

            glBindTexture( GL_TEXTURE_2D, Glide.LFBTexture );
            glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, Glide.WindowWidth, ysize, GL_RGBA,
                GL_UNSIGNED_BYTE, tempBuf + ( miny * Glide.WindowWidth ) );

            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
            glDrawBuffer( Glide.DstBuffer.Buffer == GR_BUFFER_BACKBUFFER
                        ? GL_BACK : GL_FRONT );

            glBegin( GL_QUADS );
                glColor3f( 1.0f, 1.0f, 1.0f );

                glTexCoord2f( (float)minx/Glide.LFBTextureSize, 0.0f );
                glVertex2f( (float)minx, (float)miny );

                glTexCoord2f((float)maxx/Glide.LFBTextureSize, 0.0f );
                glVertex2f( (float)maxx, (float)miny );

                glTexCoord2f( (float)maxx/Glide.LFBTextureSize, (float)ysize/Glide.LFBTextureSize );
                glVertex2f( (float)maxx, (float)maxy );

                glTexCoord2f( (float)minx/Glide.LFBTextureSize, (float)ysize/Glide.LFBTextureSize );
                glVertex2f( (float)minx, (float)maxy );
            glEnd( );

            glPopAttrib( );
            glDrawBuffer( OpenGL.RenderBuffer );

            if ( Glide.DstBuffer.Buffer != GR_BUFFER_BACKBUFFER )
            {
                glFlush( );
            }
        }

        Glide.DstBuffer.Lock = false;

        return FXTRUE;
    }
    else
    {
        if ( Glide.SrcBuffer.Lock )
        {
            Glide.SrcBuffer.Lock = false;
            
            return FXTRUE; 
        }
        else
        {
            return FXFALSE; 
        }
    }
}

//*************************************************
FX_ENTRY FxBool FX_CALL
grLfbReadRegion( GrBuffer_t src_buffer,
                 FxU32 src_x, FxU32 src_y,
                 FxU32 src_width, FxU32 src_height,
                 FxU32 dst_stride, void *dst_data )
{
#ifdef OGL_NOTDONE
    GlideMsg("grLfbReadRegion( %d, %d, %d, %d, %d, %d, --- )\n",
        src_buffer, src_x, src_y, src_width, src_height, dst_stride );
#endif

    RenderDrawTriangles( );

    switch ( src_buffer )
    {
    case GR_BUFFER_FRONTBUFFER:     glReadBuffer( GL_FRONT );   break;
    case GR_BUFFER_BACKBUFFER:
    case GR_BUFFER_AUXBUFFER:       glReadBuffer( GL_BACK );    break;
    }

    glReadPixels( src_x, src_y, src_width, src_height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)Glide.SrcBuffer.Address );
//  glReadPixels( 320, 230, 300, 200, GL_RED, GL_UNSIGNED_BYTE, &Glide.SrcBuffer.Address );
//  glDrawPixels( 300, 200, GL_RED , GL_UNSIGNED_BYTE, Glide.SrcBuffer.Address );

#ifdef OPENGL_DEBUG
    GLErro( "grLfbReadRegion" );
#endif

    return FXTRUE; 
}

//*************************************************
FX_ENTRY FxBool FX_CALL
grLfbWriteRegion( GrBuffer_t dst_buffer,
                  FxU32 dst_x, FxU32 dst_y,
                  GrLfbSrcFmt_t src_format,
                  FxU32 src_width, FxU32 src_height,
                  FxI32 src_stride, void *src_data )
{
#ifdef OGL_NOTDONE
    GlideMsg("grLfbWriteRegion( %d, %d, %d, %d, %d, %d, %d, --- )\n",
        dst_buffer, dst_x, dst_y, src_format, src_width, src_height, src_stride );
#endif

    RenderDrawTriangles( );

    if ( src_stride != (FxI32)src_width )
    {
//      Error( "grLfbWriteRegion: different width and stride.\n" );
        return FXTRUE;
    }

    switch ( dst_buffer )
    {
    case GR_BUFFER_FRONTBUFFER:     glDrawBuffer( GL_FRONT );   break;
    case GR_BUFFER_BACKBUFFER:
    case GR_BUFFER_AUXBUFFER:       glDrawBuffer( GL_BACK );    break;
    }

    glRasterPos2i( dst_x, dst_y );

    switch ( src_format )
    {
    case GR_LFB_SRC_FMT_565:
    case GR_LFB_SRC_FMT_555:
    case GR_LFB_SRC_FMT_1555:
    case GR_LFB_SRC_FMT_888:
        break;

    case GR_LFB_SRC_FMT_8888:
        break;

    case GR_LFB_SRC_FMT_565_DEPTH:
    case GR_LFB_SRC_FMT_555_DEPTH:
    case GR_LFB_SRC_FMT_1555_DEPTH:
    case GR_LFB_SRC_FMT_ZA16:
    case GR_LFB_SRC_FMT_RLE16:
        break;
    }

//  glDrawPixels( src_width, src_height, GL_RGBA, GL_UNSIGNED_BYTE, Glide.SrcBuffer.Buffer );

    glDrawBuffer( OpenGL.RenderBuffer );

#ifdef OPENGL_DEBUG
    GLErro( "grLfbWriteRegion" );
#endif

    return FXTRUE; 
}

FX_ENTRY void FX_CALL 
grLfbConstantAlpha( GrAlpha_t alpha )
{
#ifdef OGL_CRITICAL
    GlideMsg("grLfbConstantAlpha( %lu )\n", alpha );
#endif
}

FX_ENTRY void FX_CALL 
grLfbConstantDepth( FxU16 depth )
{
#ifdef OGL_CRITICAL
    GlideMsg("grLfbConstantDepth( %u )\n", depth );
#endif
}

FX_ENTRY void FX_CALL 
grLfbWriteColorSwizzle( FxBool swizzleBytes, FxBool swapWords )
{
#ifdef OGL_CRITICAL
    GlideMsg("grLfbWriteColorSwizzle( %d, %d )\n",
        swizzleBytes, swapWords );
#endif
}

FX_ENTRY void FX_CALL
grLfbWriteColorFormat( GrColorFormat_t colorFormat )
{
#ifdef OGL_CRITICAL
    GlideMsg("grLfbWriteColorFormat( %u )\n", colorFormat );
#endif
}

