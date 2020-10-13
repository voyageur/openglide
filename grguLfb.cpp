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

        // BGRA has been tested to be the fastest way to read pixels
        // Reading pixels in one of the 565 modes is way slower than reading BGRA and converting to 565 later
        // This may change with new drivers/graphics hardware...
        // if anyone can show a faster way to read pixels, suggestions welcome :-)
        glReadPixels( 0, 0,
                      OpenGL.WindowWidth, OpenGL.WindowHeight,
                      GL_BGRA, GL_UNSIGNED_BYTE,
                      (void *)OpenGL.tmpBuf );

        if ( dwOrigin == GR_ORIGIN_UPPER_LEFT )
        {
            // When the OpenGL resolution differs from the Glide resolution,
            // the content of the read buffer must be scaled
            if ( OpenGL.WindowTotalPixels != Glide.WindowTotalPixels ) {
                const FxU32* src;
                FxU16* dst = Glide.SrcBuffer.Address;
                const FxU32 xratio = (OpenGL.WindowWidth << 16) / (Glide.WindowWidth);
                const FxU32 yratio = (OpenGL.WindowHeight << 16) / (Glide.WindowHeight);
                FxU32 u, v = 0, x, y;
                for ( y = 0; y < Glide.WindowHeight; y++ )
                {
                    src = OpenGL.tmpBuf + (OpenGL.WindowHeight -1 - (v >> 16)) * OpenGL.WindowWidth;
                    u = 0;
                    for ( x = 0; x < Glide.WindowWidth; x++ )
                    {
                        // Resize and convert from 888 to 565
                        FxU32 pixel = src[u >> 16];
                        *dst++ = ( FxU16 ) (
                            ( pixel & 0x00F80000 ) >> 8 |
                            ( pixel & 0x0000FC00 ) >> 5 |
                            ( pixel & 0x000000F8 ) >> 3 );
                        u += xratio;
                    }
                    v += yratio;
                }
            } else {
                for ( j = 0; j < Glide.WindowHeight; j++ )
                {
                    Convert8888to565( OpenGL.tmpBuf + ( ( ( Glide.WindowHeight ) - 1 - j ) * Glide.WindowWidth ),
                        Glide.SrcBuffer.Address + ( j * Glide.WindowWidth ),
                        Glide.WindowWidth );
                }
            }
        }
        else
        {
            if ( OpenGL.WindowTotalPixels != Glide.WindowTotalPixels ) {
                // Copy and scale
                const FxU32* src;
                FxU16* dst = Glide.SrcBuffer.Address;
                const FxU32 xratio = (OpenGL.WindowWidth << 16) / Glide.WindowWidth;
                const FxU32 yratio = (OpenGL.WindowHeight << 16) / Glide.WindowHeight;
                FxU32 u, v = 0, x, y;
                for ( y = 0; y < Glide.WindowHeight; y++ )
                {
                    src = OpenGL.tmpBuf + (v >> 16) * OpenGL.WindowWidth;
                    u = 0;
                    for( x = 0; x < Glide.WindowWidth; x++ )
                    {
                        // Resize and convert from 888 to 565
                        FxU32 pixel = src[u >> 16];
                        *dst++ = ( FxU16 ) (
                            ( pixel & 0x00F80000 ) >> 8 |
                            ( pixel & 0x0000FC00 ) >> 5 |
                            ( pixel & 0x000000F8 ) >> 3 );
                        u += xratio;
                    }
                    v += yratio;
                }
            } else {
                Convert8888to565( OpenGL.tmpBuf, Glide.SrcBuffer.Address, Glide.WindowTotalPixels );
            }
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
            minx = Glide.WindowWidth,
            miny = Glide.WindowHeight;

        for ( ii = 0, x = 0, y = 0; ii < Glide.WindowTotalPixels; ii++ )
        {
            if ( Glide.DstBuffer.Address[ ii ] != BLUE_SCREEN )
            {
                if ( x > maxx ) maxx = x;
                if ( y > maxy ) maxy = y;
                if ( x < minx ) minx = x;
                if ( y < miny ) miny = y;

                OpenGL.tmpBuf[ ii ] = 0x0    |                      // A
                ( Glide.DstBuffer.Address[ ii ] & 0x001F ) << 19 |  // B
                ( Glide.DstBuffer.Address[ ii ] & 0x07E0 ) << 5  |  // G
                ( Glide.DstBuffer.Address[ ii ] >> 8 );             // R
                Glide.DstBuffer.Address[ ii ] = BLUE_SCREEN;
            } else
                OpenGL.tmpBuf[ ii ] = 0xFFFFFFFF;

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
                GL_UNSIGNED_BYTE, OpenGL.tmpBuf + ( miny * Glide.WindowWidth ) );

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

    // Copied from the linux sst1 driver src
    FxBool rv = FXTRUE;
    GrLfbInfo_t info;

    info.size = sizeof( info );
    if ( grLfbLock( GR_LFB_READ_ONLY,
                src_buffer,
                GR_LFBWRITEMODE_ANY,
                GR_ORIGIN_UPPER_LEFT,
                FXFALSE,
                &info ) )
    {
        FxU32 *srcData;         /* Tracking Source Pointer */
        FxU32 *dstData;         /* Tracking Destination Pointer */
        FxU32 *end;             /* Demarks End of each Scanline */
        FxU32 srcJump;          /* bytes to next scanline */
        FxU32 dstJump;          /* bytes to next scanline */
        FxU32 length;           /* bytes to copy in scanline */
        FxU32 scanline;         /* scanline number */
        int   aligned;          /* word aligned? */

        dstData = ( FxU32 * ) dst_data;
        srcData = ( FxU32 * ) ( ((char*)info.lfbPtr)+
                                (src_y*info.strideInBytes) +
                                (src_x<<1) );
        scanline = src_height;
        length   = src_width * 2;
        dstJump  = dst_stride - length;
        srcJump  = info.strideInBytes - length;
        aligned  = !((FxU)srcData&0x2);

        if ( aligned ) {
            while( scanline-- ) {
                end = (FxU32*)((char*)srcData + length - 2);

                while( srcData < end )
                    *dstData++ = *srcData++;

                if ( ((int)length) & 0x2 ) {
                    (*(FxU16*)dstData) = (*(FxU16*)srcData);
                    dstData = (FxU32*)(((FxU16*)dstData) + 1 );
                    srcData = (FxU32*)(((FxU16*)srcData) + 1 );
                }

                dstData = (FxU32*)(((char*)dstData)+dstJump);
                srcData = (FxU32*)(((char*)srcData)+srcJump);
            }
        } else {
            while( scanline-- ) {
                end = (FxU32*)((char*)srcData + length - 2);

                (*(FxU16*)dstData) = (*(FxU16*)srcData);
                dstData = (FxU32*)(((FxU16*)dstData) + 1 );
                srcData = (FxU32*)(((FxU16*)srcData) + 1 );

                while( srcData < end )
                    *dstData++ = *srcData++;

                if ( !(((int)length) & 0x2) ) {
                    (*(FxU16*)dstData) = (*(FxU16*)srcData);
                    dstData = (FxU32*)(((FxU16*)dstData) + 1 );
                    srcData = (FxU32*)(((FxU16*)srcData) + 1 );
                }

                dstData = (FxU32*)(((char*)dstData)+dstJump);
                srcData = (FxU32*)(((char*)srcData)+srcJump);
            }
        }
        grLfbUnlock( GR_LFB_READ_ONLY, src_buffer );
    } else {
        rv = FXFALSE;
    }

    return rv;
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

    // Copied from the linux sst1 driver src
    FxBool           rv = FXTRUE;
    GrLfbInfo_t      info;
    GrLfbWriteMode_t writeMode;

    if ( src_format == GR_LFB_SRC_FMT_RLE16 )
        writeMode = GR_LFBWRITEMODE_565;
    else
        writeMode = (GrLfbWriteMode_t)src_format;

    info.size = sizeof( info );

    if ( grLfbLock((GrLock_t)( GR_LFB_WRITE_ONLY | GR_LFB_NOIDLE),
                 dst_buffer,
                 writeMode,
                 GR_ORIGIN_UPPER_LEFT,
                 FXFALSE,
                 &info ) )
    {
        FxU32 *srcData;         /* Tracking Source Pointer */
        FxU32 *dstData;         /* Tracking Destination Pointer */
        FxU32 *end;             /* Demarks End of each Scanline */
        FxI32 srcJump;          /* bytes to next scanline */
        FxU32 dstJump;          /* bytes to next scanline */
        FxU32 length;           /* bytes to copy in scanline */
        FxU32 scanline;         /* scanline number */
        int   aligned;          /* word aligned? */

        srcData = ( FxU32 * ) src_data;
        dstData = ( FxU32 * ) ( ((char*)info.lfbPtr)+
                                (dst_y*info.strideInBytes) );
        scanline = src_height;

        switch( src_format ) {
            /* 16-bit aligned */
            case GR_LFB_SRC_FMT_565:
            case GR_LFB_SRC_FMT_555:
            case GR_LFB_SRC_FMT_1555:
            case GR_LFB_SRC_FMT_ZA16:
                dstData = (FxU32*)(((FxU16*)dstData) + dst_x);
                length  = src_width * 2;
                aligned = !((FxU)dstData&0x2);
                srcJump = src_stride - length;
                dstJump = info.strideInBytes - length;
                if ( aligned )
                {
                    while( scanline-- )
                    {
                        end = (FxU32*)((char*)srcData + length - 2);
                        while( srcData < end )
                        {
                            // TODO: swap on bigendian?
                            *dstData = *srcData;
                            dstData++;
                            srcData++;
                        }

                        if ( ((int)length) & 0x2 )
                        {
                            (*(FxU16*)&(dstData[0])) = (*(FxU16*)&(srcData[0]));

                            dstData = (FxU32*)(((FxU16*)dstData) + 1 );
                            srcData = (FxU32*)(((FxU16*)srcData) + 1 );
                        }

                        dstData = (FxU32*)(((char*)dstData)+dstJump);
                        srcData = (FxU32*)(((char*)srcData)+srcJump);
                    }
                }
                else
                {
                    while( scanline-- ) {
                        end = (FxU32*)((char*)srcData + length - 2);

                        // TODO: swap on bigendian?
                        (*(FxU16*)&(dstData[0])) = (*(FxU16*)&(srcData[0]));
                        dstData = (FxU32*)(((FxU16*)dstData) + 1 );
                        srcData = (FxU32*)(((FxU16*)srcData) + 1 );

                        while( srcData < end ) {
                            // TODO: swap on bigendian?
                            *dstData = *srcData;
                            dstData++;
                            srcData++;
                        }

                        if ( !(length & 0x2) )
                        {
                            // TODO: swap on bigendian?
                            (*(FxU16*)&(dstData[0])) = (*(FxU16*)&(srcData[0]));
                            dstData = (FxU32*)(((FxU16*)dstData) + 1 );
                            srcData = (FxU32*)(((FxU16*)srcData) + 1 );
                        }

                        dstData = (FxU32*)(((char*)dstData)+dstJump);
                        srcData = (FxU32*)(((char*)srcData)+srcJump);
                    }
                }
                break;
                /* 32-bit aligned */
                case GR_LFB_SRC_FMT_888:
                case GR_LFB_SRC_FMT_8888:
                case GR_LFB_SRC_FMT_565_DEPTH:
                case GR_LFB_SRC_FMT_555_DEPTH:
                case GR_LFB_SRC_FMT_1555_DEPTH:
                    dstData = ((FxU32*)dstData) + dst_x;
                    length  = src_width * 4;
                    srcJump = src_stride - length;
                    dstJump = info.strideInBytes - length;

                    while( scanline-- ) {
                        end = (FxU32*)((char*)srcData + length);
                        while( srcData < end ) {
                            // TODO: swap on bigendian?
                            *dstData = *srcData;
                            dstData++;
                            srcData++;
                        }
                        dstData = (FxU32*)(((char*)dstData)+dstJump);
                        srcData = (FxU32*)(((char*)srcData)+srcJump);
                    }
                break;
                case GR_LFB_SRC_FMT_RLE16:
	            // TODO: needs to be implemented
	            rv = FXFALSE;
	        break;
                default:
	            rv = FXFALSE;
	            break;
            }
            grLfbUnlock( GR_LFB_WRITE_ONLY, dst_buffer );
        } else {
            rv = FXFALSE;
        }

    return rv;
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

