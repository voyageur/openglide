//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*				  Linear Frame Buffer Functions
//*					   Made by Glorfindel
//**************************************************************

#include "GlOgl.h"
#include "Glextensions.h"
#include "GLRender.h"

inline void Convert565to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = ((*Buffer1) ? 0xFF000000 : 0) |	// A
			((*Buffer1)		& 0x001F) << 19 |			// B
			((*Buffer1)		& 0x07E0) << 5  |			// G
			((*Buffer1++)	& 0xF800) >> 8;				// R
		Pixels--;
	}
}

inline void Convert565to5551( WORD *Buffer1, WORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = ((*Buffer1) & 0xFFC0) |
					(((*Buffer1) & 0x001F) << 1) |
					( (*Buffer1++) ? 0x0001 : 0x0000);
		Pixels--;
	}
}

//----------------------------------------------------------------
DLLEXPORT FxBool __stdcall
grLfbLock(GrLock_t dwType, 
          GrBuffer_t dwBuffer, 
          GrLfbWriteMode_t dwWriteMode,
          GrOriginLocation_t dwOrigin, 
          FxBool bPixelPipeline, 
          GrLfbInfo_t *lfbInfo )
{ 
#ifdef CRITICAL
	GlideMsg("grLfbLock( %d, %d, %d, %d, %d, --- )\n", dwType, dwBuffer, dwWriteMode, dwOrigin, bPixelPipeline); 
#endif
    int width = Glide.WindowWidth;
    int height = Glide.WindowHeight;

	RenderDrawTriangles();

	Glide.SrcBuffer.Lock = true;
	Glide.SrcBuffer.Type = dwType;
	Glide.SrcBuffer.Buffer = dwBuffer;
	Glide.SrcBuffer.WriteMode = dwWriteMode;

    if((dwType & 1) == 0)
    {
        FxU32 *buf = new FxU32[width * height];
        int i, j;

        glReadBuffer(dwBuffer == GR_BUFFER_BACKBUFFER
            ? GL_BACK : GL_FRONT);

        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void *) buf);
        
        for(j = 0; j < height; j++)
        {
            WORD *line = Glide.SrcBuffer.Address + (height - 1 - j) * width;
            FxU32 *bufl = buf + j * width;

            for(i = 0; i < width; i++)
            {
                line[i] = (WORD)
                    (((bufl[i] & 0xf8) << 8)
                    | ((bufl[i] & 0xfc00) >> 5)
                    | ((bufl[i] & 0xf80000) >> 19)
                    );
            }
        }
        
        delete [] buf;
    }
    else
    {
        memset(Glide.SrcBuffer.Address, 0, width * height * 2);
    }

	lfbInfo->lfbPtr = Glide.SrcBuffer.Address;
	lfbInfo->strideInBytes = Glide.WindowWidth;

	lfbInfo->writeMode = GR_LFBWRITEMODE_565;

	return FXTRUE;
}
//typedef struct {
//    int                size;
//    void               *lfbPtr;
//    FxU32              strideInBytes;        
//    GrLfbWriteMode_t   writeMode;
//    GrOriginLocation_t origin;
//} GrLfbInfo_t;

//----------------------------------------------------------------
DLLEXPORT FxBool __stdcall
grLfbUnlock(GrLock_t dwType, GrBuffer_t dwBuffer )
{ 
#ifdef CRITICAL
	GlideMsg("grLfbUnlock( %d, %d )\n", dwType, dwBuffer ); 
#endif
	
	if ( !Glide.SrcBuffer.Lock )
	{
		return FXFALSE;
	}

	switch (Glide.SrcBuffer.WriteMode)
	{
	case GR_LFBWRITEMODE_565:
	case GR_LFBWRITEMODE_555:
	case GR_LFBWRITEMODE_1555:
	case GR_LFBWRITEMODE_888:
	case GR_LFBWRITEMODE_8888:
	case GR_LFBWRITEMODE_565_DEPTH:
	case GR_LFBWRITEMODE_555_DEPTH:
	case GR_LFBWRITEMODE_1555_DEPTH:
	case GR_LFBWRITEMODE_ZA16:
		break;
	}

	Glide.SrcBuffer.Lock = false;

	return FXTRUE; 
}

//----------------------------------------------------------------
DLLEXPORT FxBool __stdcall
grLfbReadRegion( GrBuffer_t src_buffer,
                 FxU32 src_x, FxU32 src_y,
                 FxU32 src_width, FxU32 src_height,
                 FxU32 dst_stride, void *dst_data )
{
#ifdef NOTDONE
	GlideMsg("grLfbReadRegion( %d, %d, %d, %d, %d, %d, --- )\n",
		src_buffer, src_x, src_y, src_width, src_height, dst_stride );
#endif

	RenderDrawTriangles();

	static WORD *Buffer1;
	static DWORD *Buffer2;
	static DWORD PixelsX, PixelsY;
	static DWORD Stride;

	switch(src_buffer)
	{
	case GR_BUFFER_FRONTBUFFER:		glReadBuffer( GL_FRONT );	break;
	case GR_BUFFER_BACKBUFFER:
	case GR_BUFFER_AUXBUFFER:		glReadBuffer( GL_BACK );	break;
	}

	glReadPixels( src_x, src_y, src_width, src_height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)Glide.SrcBuffer.Address );
//	glReadPixels( 320, 230, 300, 200, GL_RED, GL_UNSIGNED_BYTE, &Glide.SrcBuffer.Address );
//	glDrawPixels( 300, 200, GL_RED , GL_UNSIGNED_BYTE, Glide.SrcBuffer.Address );

/*
	Buffer1 = (WORD*)dst_data;
	Buffer2 = (DWORD*)Glide.SrcBuffer.Address;
	Stride = dst_stride - src_width;
	PixelsY = src_height;
	while(PixelsY)
	{
		PixelsX = src_width;
		while(PixelsX)
		{
			*Buffer1++ = 
				((*Buffer2)		& 0xF8000000) >> 16 |
				((*Buffer2)		& 0x00FC0000) >> 13 |
				((*Buffer2++)	& 0x0000F800) >> 11;
			PixelsX--;
		}
//		Buffer1 += Stride;
		PixelsY--;
	}
*/
#ifdef OPENGL_DEBUG
	GLErro( "grLfbReadRegion" );
#endif

	return FXTRUE; 
}

//----------------------------------------------------------------
DLLEXPORT FxBool __stdcall
grLfbWriteRegion( GrBuffer_t dst_buffer,
                 FxU32 dst_x, FxU32 dst_y,
				 GrLfbSrcFmt_t src_format,
                 FxU32 src_width, FxU32 src_height,
                 FxI32 src_stride, void *src_data )
{
#ifdef NOTDONE
	GlideMsg("grLfbWriteRegion( %d, %d, %d, %d, %d, %d, %d, --- )\n",
		dst_buffer, dst_x, dst_y, src_format, src_width, src_height, src_stride );
#endif

	RenderDrawTriangles();

	static WORD *Buffer1;
	static DWORD *Buffer2;
	static DWORD Pixels;

	if (src_stride != (FxI32)src_width)
	{
//		Error( "grLfbWriteRegion: different width and stride.\n" );
		return FXTRUE;
	}

	switch (dst_buffer)
	{
	case GR_BUFFER_FRONTBUFFER:		glDrawBuffer( GL_FRONT );	break;
	case GR_BUFFER_BACKBUFFER:
	case GR_BUFFER_AUXBUFFER:		glDrawBuffer( GL_BACK );	break;
	}

	glRasterPos2i( dst_x, dst_y );

	switch (src_format)
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

//	glDrawPixels( src_width, src_height, GL_RGBA, GL_UNSIGNED_BYTE, Glide.SrcBuffer.Buffer );

	glDrawBuffer( OpenGL.RenderBuffer );

#ifdef OPENGL_DEBUG
	GLErro( "grLfbWriteRegion" );
#endif
	return FXTRUE; 
}

DLLEXPORT void __stdcall 
grLfbConstantAlpha( GrAlpha_t alpha )
{
#ifdef CRITICAL
	GlideMsg("grLfbConstantAlpha( %lu )\n", alpha );
#endif
}

DLLEXPORT void __stdcall 
grLfbConstantDepth( FxU16 depth )
{
#ifdef CRITICAL
	GlideMsg("grLfbConstantDepth( %u )\n", depth );
#endif
}

DLLEXPORT void __stdcall 
grLfbWriteColorSwizzle(FxBool swizzleBytes, FxBool swapWords)
{
#ifdef CRITICAL
	GlideMsg("grLfbWriteColorSwizzle( %d, %d )\n",
		swizzleBytes, swapWords );
#endif
}

DLLEXPORT void __stdcall
grLfbWriteColorFormat(GrColorFormat_t colorFormat)
{
#ifdef CRITICAL
	GlideMsg("grLfbWriteColorFormat( %u )\n", colorFormat );
#endif
}

