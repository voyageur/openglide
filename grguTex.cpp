//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*					 Glide Texture Functions
//*					   Made by Glorfindel
//**************************************************************

#include "GlOgl.h"
#include "GLRender.h"
#include "GLextensions.h"



// extern functions
DWORD GetTexSize(const int Lod, const int aspectRatio, const int format );



// Functions

//*************************************************
//* Return the lowest start address for texture downloads
//*************************************************
DLLEXPORT FxU32 __stdcall
grTexMinAddress( GrChipID_t tmu )
{
#ifdef DONE
	GlideMsg( "grTexMinAddress( %d )\n", tmu );
#endif

	return (FxU32) 0;
}

//*************************************************
//* Return the highest start address for texture downloads
//*************************************************
DLLEXPORT FxU32 __stdcall
grTexMaxAddress( GrChipID_t tmu )
{
#ifdef DONE
	GlideMsg( "grTexMaxAddress( %d ) = %lu\n", tmu, Glide.TexMemoryMaxPosition );
#endif

	return (FxU32)(Glide.TexMemoryMaxPosition);
}

//*************************************************
//* Specify the current texture source for rendering
//*************************************************
DLLEXPORT void __stdcall
grTexSource( GrChipID_t tmu,
             FxU32      startAddress,
             FxU32      evenOdd,
             GrTexInfo  *info )
{
#ifdef DONE
	GlideMsg( "grTexSource( %d, %d, %d, --- )\n", tmu, startAddress, evenOdd );
#endif
	if (tmu != GR_TMU0)
		return;

	RenderDrawTriangles();

	Glide.State.TexSource.StartAddress = startAddress;
	Glide.State.TexSource.EvenOdd = evenOdd;

    Textures->Source( startAddress, evenOdd, info );	
}

//*************************************************
//* Return the texture memory consumed by a texture
//*************************************************
DLLEXPORT FxU32 __stdcall
grTexTextureMemRequired(DWORD dwEvenOdd, GrTexInfo *texInfo)
{
#ifdef DONE
	GlideMsg("grTexTextureMemRequired( %u, --- )\n", dwEvenOdd );
#endif

	register int i;
	static DWORD nTotalBytes;
	nTotalBytes = 0;
	for( i = texInfo->largeLod; i <= texInfo->smallLod; i++ )
	{
		nTotalBytes += GetTexSize( i, texInfo->aspectRatio, texInfo->format );
	}
	nTotalBytes += ((nTotalBytes % 8) > 0) ? (8 - (nTotalBytes % 8)) : 0;

	return nTotalBytes;
}

//*************************************************
//* Return the texture memory consumed by a texture
//*************************************************
DLLEXPORT void __stdcall
grTexDownloadMipMap( GrChipID_t tmu,
                     FxU32      startAddress,
                     FxU32      evenOdd,
                     GrTexInfo  *info )
{
#ifdef PARTDONE
	GlideMsg( "grTexDownloadMipMap( %d, %u, %u, --- )\n", tmu, 
		startAddress, evenOdd );
#endif
	if (tmu != GR_TMU0)
		return;

	info->smallLod = info->largeLod;
	Textures->DownloadMipMap( startAddress, evenOdd, info );
//	register int i;
//	BYTE *Data = (BYTE*)info->data;
//	for( i = info->largeLod; i <= info->smallLod; i++ )
//	{
//		grTexDownloadMipMapLevel( tmu, startAddress, i, info->largeLod, info->aspectRatio,
//				info->format, evenOdd, Data );
//		Data += GetTexSize( i, info->aspectRatio, info->format );
//	}
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
grTexDownloadMipMapLevel( GrChipID_t        tmu,
                          FxU32             startAddress,
                          GrLOD_t           thisLod,
                          GrLOD_t           largeLod,
                          GrAspectRatio_t   aspectRatio,
                          GrTextureFormat_t format,
                          FxU32             evenOdd,
                          void              *data )
{
#ifdef PARTDONE
	GlideMsg( "grTexDownloadMipMapLevel( %d, %lu, %d, %d, %d, %d, %d, %lu )\n",
		tmu, startAddress, thisLod, largeLod, aspectRatio, format, evenOdd, data );
#endif
	if (tmu != GR_TMU0)
		return;

	static GrTexInfo info;

	info.smallLod		= thisLod;
	info.largeLod		= largeLod;
	info.aspectRatio	= aspectRatio;
	info.format			= format;
	info.data			= data;

	Textures->DownloadMipMap( startAddress, evenOdd, &info );
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
grTexDownloadMipMapLevelPartial( GrChipID_t        tmu,
                                 FxU32             startAddress,
                                 GrLOD_t           thisLod,
                                 GrLOD_t           largeLod,
                                 GrAspectRatio_t   aspectRatio,
                                 GrTextureFormat_t format,
                                 FxU32             evenOdd,
                                 void              *data,
                                 int               start,
                                 int               end )
{
#ifdef PARTDONE
	GlideMsg( "grTexDownloadMipMapLevelPartial( %d, %lu, %d, %d, %d, %d, %d, ---, %d, %d )\n",
		tmu, startAddress, thisLod, largeLod, aspectRatio, format, evenOdd, start, end );
#endif
	if (tmu != GR_TMU0)
		return;

	grTexDownloadMipMapLevel( tmu, startAddress, thisLod, largeLod, aspectRatio, format,
		evenOdd, data );
}

//*************************************************
//* Set the texture map clamping/wrapping mode
//*************************************************
DLLEXPORT void __stdcall
grTexClampMode(GrChipID_t tmu,
               GrTextureClampMode_t s_clampmode,
               GrTextureClampMode_t t_clampmode )
{
#ifdef DONE
	GlideMsg( "grTexClampMode( %d, %d, %d )\n",
		tmu, s_clampmode, t_clampmode );
#endif
	if (tmu != GR_TMU0)
		return;

	RenderDrawTriangles();

	Glide.State.SClampMode = s_clampmode;
	Glide.State.TClampMode = t_clampmode;

	switch ( s_clampmode )
	{
	case GR_TEXTURECLAMP_CLAMP:		OpenGL.SClampMode = GL_CLAMP;	break;
	case GR_TEXTURECLAMP_WRAP:		OpenGL.SClampMode = GL_REPEAT;	break;
	}
	switch ( t_clampmode )
	{
	case GR_TEXTURECLAMP_CLAMP:		OpenGL.TClampMode = GL_CLAMP;	break;
	case GR_TEXTURECLAMP_WRAP:		OpenGL.TClampMode = GL_REPEAT;	break;
	}

   /*
    * The parameters are set in PGTexture
    */

#ifdef OPENGL_DEBUG
	GLErro( "grTexClampMode" );
#endif
}

//*************************************************
//* Set the texture Min/Mag Filter
//*************************************************
DLLEXPORT void __stdcall
grTexFilterMode(GrChipID_t tmu,
                GrTextureFilterMode_t minfilter_mode,
                GrTextureFilterMode_t magfilter_mode )
{
#ifdef PARTDONE
	GlideMsg( "grTexFilterMode( %d, %d, %d )\n",
		tmu, minfilter_mode, magfilter_mode );
#endif
	if (tmu != GR_TMU0)
		return;

	RenderDrawTriangles();

	Glide.State.MinFilterMode = minfilter_mode;
	Glide.State.MagFilterMode = magfilter_mode;

	switch ( minfilter_mode )
	{
	case GR_TEXTUREFILTER_POINT_SAMPLED:
		if (( Glide.State.MipMapMode != GR_MIPMAP_DISABLE ) && ( InternalConfig.BuildMipMaps ) )
		{
			if (Glide.State.LodBlend)
			{
				OpenGL.MinFilterMode = GL_NEAREST_MIPMAP_LINEAR;
			}
			else
			{
				OpenGL.MinFilterMode = GL_NEAREST_MIPMAP_NEAREST;
			}
		}
		else
			OpenGL.MinFilterMode = GL_NEAREST;
		break;
	case GR_TEXTUREFILTER_BILINEAR:
		if (( Glide.State.MipMapMode != GR_MIPMAP_DISABLE ) && ( InternalConfig.BuildMipMaps ) )
		{
			if (Glide.State.LodBlend)
			{
				OpenGL.MinFilterMode = GL_LINEAR_MIPMAP_LINEAR;
			}
			else
			{
				OpenGL.MinFilterMode = GL_LINEAR_MIPMAP_NEAREST;
			}
		}
		else
			OpenGL.MinFilterMode = GL_LINEAR;
		break;
	}
	switch ( magfilter_mode )
	{
	case GR_TEXTUREFILTER_POINT_SAMPLED:	OpenGL.MagFilterMode = GL_NEAREST;		break;
	case GR_TEXTUREFILTER_BILINEAR:			OpenGL.MagFilterMode = GL_LINEAR;		break;
	}

   /*
    * The parameters are set in PGTexture
    */

#ifdef OPENGL_DEBUG
	GLErro( "grTexFilterMode" );
#endif
}

//*************************************************
//* Set the texture MipMap Mode
//*************************************************
DLLEXPORT void __stdcall
grTexMipMapMode( GrChipID_t     tmu, 
                 GrMipMapMode_t mode,
                 FxBool         lodBlend )
{
#ifdef PARTDONE
	GlideMsg( "grTexMipMapMode( %d, %d, %d )\n",
		tmu, mode, lodBlend );
#endif
	if (tmu != GR_TMU0)
		return;

	Glide.State.MipMapMode = mode;
	Glide.State.LodBlend = lodBlend;

	grTexFilterMode( GR_TMU0,
                Glide.State.MinFilterMode,
                Glide.State.MagFilterMode );
/*
	switch (Glide.State.MipMapMode)
	{
	case GR_MIPMAP_DISABLE:
		if (Glide.State.MinFilterMode == GR_TEXTUREFILTER_POINT_SAMPLED)
			OpenGL.MinFilterMode = GL_NEAREST;
		else
			OpenGL.MinFilterMode = GL_LINEAR;
		break;
	case GR_MIPMAP_NEAREST:
	case GR_MIPMAP_NEAREST_DITHER:
		if (Glide.State.MinFilterMode == GR_TEXTUREFILTER_POINT_SAMPLED)
		{
			if (Glide.State.LodBlend)
			{
//				OpenGL.MinFilterMode = GL_NEAREST_MIPMAP_LINEAR;
				OpenGL.MinFilterMode = GL_NEAREST;
			}
			else
			{
//				OpenGL.MinFilterMode = GL_NEAREST_MIPMAP_NEAREST;
				OpenGL.MinFilterMode = GL_NEAREST;
			}
		}
		else
		{
			if (Glide.State.LodBlend)
			{
//				OpenGL.MinFilterMode = GL_LINEAR_MIPMAP_LINEAR;
				OpenGL.MinFilterMode = GL_LINEAR;
			}
			else
			{
//				OpenGL.MinFilterMode = GL_LINEAR_MIPMAP_NEAREST;
				OpenGL.MinFilterMode = GL_LINEAR;
			}
		}
		break;
	}

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OpenGL.MinFilterMode );
*/
#ifdef OPENGL_DEBUG
	GLErro( "grTexMipMapMode" );
#endif
}

//*************************************************
//* Returns the memory occupied by a texture
//*************************************************
DLLEXPORT FxU32 __stdcall
grTexCalcMemRequired(GrLOD_t lodmin, GrLOD_t lodmax,
                     GrAspectRatio_t aspect, GrTextureFormat_t fmt)
{
#ifdef DONE
	GlideMsg( "grTexCalcMemRequired( %d, %d, %d, %d )\n",
		lodmin, lodmax, aspect, fmt );
#endif

	register int i;
	static DWORD nTotalBytes;
	nTotalBytes = 0;
	for( i = lodmax; i <= lodmin; i++ )
	{
		nTotalBytes += GetTexSize( i, aspect, fmt );
	}
	nTotalBytes += ((nTotalBytes % 8) > 0) ? (8 - (nTotalBytes % 8)) : 0;

	return nTotalBytes;
}
/*
inline void ConvertTable()
{
	static int i;
	for( i = 0; i < 256; i++ )
	{
		OpenGL.RTable[i] = (Glide.ColorPalette.data[i] & 0x00FF0000) >> 16;
		OpenGL.GTable[i] = (Glide.ColorPalette.data[i] & 0x0000FF00) >> 8;
		OpenGL.BTable[i] = (Glide.ColorPalette.data[i] & 0x000000FF);
	}
}
*/
inline FxU32 CalcPalette()
{
	static DWORD Result;
	register DWORD *Point = (DWORD*) OpenGL.PTable;
	register int i;
	Result = 0;
	for( i = 0; i < 256; i += InternalConfig.PalettePrecision )
	{
//		Result += Glide.ColorPalette.data[i];
		Result += Point[i];
	}
	return Result;
}

inline DWORD MMXCalcPalette()
{
	static DWORD Result;
	__asm
	{
		xor EAX, EAX
		PXOR MM0, MM0
		mov EBX, offset OpenGL.PTable
		mov AX, InternalConfig.PalettePrecision
		mov CX, 256
		mov DX, AX
		shl EAX, 2
looping:
		PADDD MM0, [EBX]
		add EBX, EAX
		sub CX, DX
		ja looping
		MOVD Result, MM0
		EMMS
	}
	return Result;
}


BYTE MaskPalette3[8] = { 0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0xFF };
BYTE MaskPalette2[8] = { 0x00,0x00,0xFF,0x00,0x00,0x00,0xFF,0x00 };
BYTE MaskPalette1[8] = { 0x00,0xFF,0x00,0x00,0x00,0xFF,0x00,0x00 };
BYTE MaskPalette0[8] = { 0xFF,0x00,0x00,0x00,0xFF,0x00,0x00,0x00 };

inline void MMXCopyTable( void *Dst, void *Src, DWORD NumberOfBytes )
{
	__asm
	{
		mov EAX, Src
		mov EDX, Dst
		mov ECX, NumberOfBytes
		MOVQ MM4, [MaskPalette3]
		MOVQ MM5, [MaskPalette2]
		MOVQ MM6, [MaskPalette1]
		MOVQ MM7, [MaskPalette0]
		shr ECX, 3
copying:
		MOVQ MM1, [EAX]
		add EAX, 8
		MOVQ MM2, MM1
		MOVQ MM3, MM1

		PAND MM1, MM5
		PAND MM2, MM6
		PAND MM3, MM7

		PSRLD MM1, 16
		PSLLD MM3, 16

		POR MM3, MM4
		POR MM1, MM2
		POR MM1, MM3
		MOVQ [EDX], MM1
		add EDX, 8
		dec ECX
		jnz copying
		EMMS
	}
}

bool TablePronta = false;

//*************************************************
//* Download a subset of an NCC table or color palette
//*************************************************
DLLEXPORT void __stdcall
grTexDownloadTablePartial( GrChipID_t   tmu,
                           GrTexTable_t type, 
                           void        *data,
                           int          start,
                           int          end )
{
#ifdef PARTDONE
	GlideMsg("grTexDownloadTablePartial( %d, %d, ---, %d, %d )\n",
		tmu, type, start, end );
#endif
	if (tmu != GR_TMU0)
		return;

	RenderDrawTriangles();

	switch ( type )
	{
	case GR_TEXTABLE_PALETTE:
		// If the entire palette was not initialized, initialize it now
		if ( !TablePronta )
		{
			unsigned int datatemp[ 256 ];
			TablePronta = true;
			memset( datatemp, 0, 256 * sizeof( unsigned int ) );
			glColorTableEXT( GL_SHARED_TEXTURE_PALETTE_EXT, GL_RGBA, 256, GL_RGBA, GL_UNSIGNED_BYTE, &datatemp );
		}

		// Copying the Palette Table changing byte positions
		if ( InternalConfig.MMXEnable )
		{
			MMXCopyTable( OpenGL.PTable[start], data, (end-start+1)*4 );
			OpenGL.PaletteCalc = CalcPalette();
		}
		else
		{
			CopyMemory( &OpenGL.PTable[start][0], data, (end-start+1)*4 );
			OpenGL.PaletteCalc = CalcPalette();
		}

		// Shared Palette Extension
		if ( InternalConfig.PaletteEXTEnable )
		{
			glColorSubTableEXT( GL_SHARED_TEXTURE_PALETTE_EXT, start, ( end - start ) + 1, GL_RGBA, GL_UNSIGNED_BYTE, OpenGL.PTable[start] );
		}
		break;
	case GR_TEXTABLE_NCC0:
	case GR_TEXTABLE_NCC1:
		Error( "grTexDownloadTablePartial:::: Does not work with NCC tables.\n" );
		return;
	}
}

//*************************************************
//* download an NCC table or color palette
//*************************************************
DLLEXPORT void __stdcall
grTexDownloadTable( GrChipID_t   tmu,
                    GrTexTable_t type, 
                    void         *data )
{
#ifdef PARTDONE
	GlideMsg( "grTexDownloadTable( %d, %d, --- )\n", tmu, type );
#endif
	if (tmu != GR_TMU0)
		return;

	RenderDrawTriangles();

    Textures->DownloadTable(type, data);

	if ( type == GR_TEXTABLE_PALETTE )
	{
		TablePronta = true;

		// Copying the Palette Table changing byte positions
		if ( InternalConfig.MMXEnable )
		{
			MMXCopyTable( OpenGL.PTable, data, 1024 );
			OpenGL.PaletteCalc = MMXCalcPalette();
		}
		else
		{
			CopyMemory( OpenGL.PTable, data, 1024 );
			OpenGL.PaletteCalc = CalcPalette();
		}

		// Shared Palette Extension
		if ( InternalConfig.PaletteEXTEnable )
		{
			glColorTableEXT( GL_SHARED_TEXTURE_PALETTE_EXT, GL_RGBA, 256, GL_RGBA, GL_UNSIGNED_BYTE, OpenGL.PTable );
		}
	}
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
grTexLodBiasValue( GrChipID_t tmu, float bias )
{
#ifdef NOTDONE
	GlideMsg( "grTexLodBiasValue( %d, %d )\n",
		tmu, bias );
#endif
	if (tmu != GR_TMU0)
		return;
}

//----------------------------------------------------------------
//grTexCombine( 0, 7, 8, 7, 8, 0, 0 )
//grTexCombine( 1, 1, 0, 1, 0, 0, 0 )
DLLEXPORT void __stdcall
grTexCombine(GrChipID_t tmu,
             GrCombineFunction_t rgb_function,
             GrCombineFactor_t rgb_factor, 
             GrCombineFunction_t alpha_function,
             GrCombineFactor_t alpha_factor,
             FxBool rgb_invert,
             FxBool alpha_invert )
{
#ifdef PARTDONE
	GlideMsg( "grTexCombine( %d, %d, %d, %d, %d, %d, %d )\n",
		tmu, rgb_function, rgb_factor, alpha_function, alpha_factor, 
		rgb_invert, alpha_invert );
#endif

	if (tmu != GR_TMU0)
		return;

	RenderDrawTriangles();

	Glide.State.TextureCombineCFunction = rgb_function;
	Glide.State.TextureCombineCFactor	= rgb_factor;
	Glide.State.TextureCombineAFunction = alpha_function;
	Glide.State.TextureCombineAFactor	= alpha_factor;
	Glide.State.TextureCombineRGBInvert = rgb_invert;
	Glide.State.TextureCombineAInvert	= alpha_invert;

	switch ( rgb_function )
	{
	case GR_COMBINE_FUNCTION_ZERO:
		OpenGL.Texture = false;
//		GlideMsg( "GR_COMBINE_FUNCTION_ZERO," );
		break;
	case GR_COMBINE_FUNCTION_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FUNCTION_LOCAL_ALPHA," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA," );
		OpenGL.Texture = true;
		break;
	}

	switch ( rgb_factor )
	{
	case GR_COMBINE_FACTOR_ZERO:
//		GlideMsg( "GR_COMBINE_FACTOR_ZERO," );
		break;
	case GR_COMBINE_FACTOR_LOCAL:
//		GlideMsg( "GR_COMBINE_FACTOR_LOCAL," );
		break;
	case GR_COMBINE_FACTOR_OTHER_ALPHA:
//		GlideMsg( "GR_COMBINE_FACTOR_OTHER_ALPHA," );
		break;
	case GR_COMBINE_FACTOR_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FACTOR_LOCAL_ALPHA," );
		break;
	case GR_COMBINE_FACTOR_DETAIL_FACTOR:
//		GlideMsg( "GR_COMBINE_FACTOR_DETAIL_FACTOR," );
		break;
	case GR_COMBINE_FACTOR_LOD_FRACTION:
//		GlideMsg( "GR_COMBINE_FACTOR_LOD_FRACTION," );
		break;
	case GR_COMBINE_FACTOR_ONE:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE," );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_LOCAL," );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA," );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA," );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_DETAIL_FACTOR:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_DETAIL_FACTOR," );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION," );
		break;
	}

	switch ( alpha_function )
	{
	case GR_COMBINE_FUNCTION_ZERO:
//		OpenGL.Texture = false;
//		GlideMsg( "GR_COMBINE_FUNCTION_ZERO," );
		break;
	case GR_COMBINE_FUNCTION_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FUNCTION_LOCAL_ALPHA," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL," );
		OpenGL.Texture = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA," );
		OpenGL.Texture = true;
		break;
	}

	switch ( alpha_factor )
	{
	case GR_COMBINE_FACTOR_ZERO:
//		GlideMsg( "GR_COMBINE_FACTOR_ZERO\n" );
		break;
	case GR_COMBINE_FACTOR_LOCAL:
//		GlideMsg( "GR_COMBINE_FACTOR_LOCAL\n" );
		break;
	case GR_COMBINE_FACTOR_OTHER_ALPHA:
//		GlideMsg( "GR_COMBINE_FACTOR_OTHER_ALPHA\n" );
		break;
	case GR_COMBINE_FACTOR_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FACTOR_LOCAL_ALPHA\n" );
		break;
	case GR_COMBINE_FACTOR_DETAIL_FACTOR:
//		GlideMsg( "GR_COMBINE_FACTOR_DETAIL_FACTOR\n" );
		break;
	case GR_COMBINE_FACTOR_LOD_FRACTION:
//		GlideMsg( "GR_COMBINE_FACTOR_LOD_FRACTION\n" );
		break;
	case GR_COMBINE_FACTOR_ONE:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE\n" );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_LOCAL\n" );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA\n" );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA\n" );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_DETAIL_FACTOR:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_DETAIL_FACTOR\n" );
		break;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION:
//		GlideMsg( "GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION\n" );
		break;
	}
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
grTexNCCTable( GrChipID_t tmu, GrNCCTable_t NCCTable )
{
#ifdef NOTDONE
	GlideMsg("grTexNCCTable( %d, %u )\n", tmu, NCCTable );
#endif
	if (tmu != GR_TMU0)
		return;
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall 
grTexDetailControl( GrChipID_t tmu,
                    int lod_bias,
                    FxU8 detail_scale,
                    float detail_max )
{
#ifdef NOTDONE
	GlideMsg("grTexDetailControl( %d, %d, %d, %-4.2f )\n",
		tmu, lod_bias, detail_scale, detail_max );
#endif
	if (tmu != GR_TMU0)
		return;
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall 
grTexMultibase( GrChipID_t tmu,
                FxBool     enable )
{
#ifdef NOTDONE
	GlideMsg("grTexMultibase( %d, %d )\n", tmu, enable );
#endif
	if (tmu != GR_TMU0)
		return;
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
grTexMultibaseAddress( GrChipID_t       tmu,
                       GrTexBaseRange_t range,
                       FxU32            startAddress,
                       FxU32            evenOdd,
                       GrTexInfo        *info )
{
#ifdef NOTDONE
	GlideMsg("grTexMultibaseAddress( %d, %d, %lu, %lu, --- )\n",
		tmu, range, startAddress, evenOdd );
#endif
	if (tmu != GR_TMU0)
		return;
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
grTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func )
{
#ifdef PARTDONE
	GlideMsg("grTexCombineFunction( %d, %d )\n", tmu, func );
#endif
	if (tmu != GR_TMU0)
		return;

	switch ( func )
	{
	case GR_TEXTURECOMBINE_ZERO:			// 0x00 per component
		grTexCombine( tmu, GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ZERO,
			GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ZERO, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_DECAL:			// Clocal decal texture
		grTexCombine( tmu, GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
			GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_OTHER:			// Cother pass through
		grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE,
			GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_ADD:				// Cother + Clocal additive texture
		grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_ONE,
			GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_MULTIPLY:		// Cother * Clocal modulated texture
		grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL,
			GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_SUBTRACT:		// Cother – Clocal subtractive texture
		grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL, GR_COMBINE_FACTOR_ONE,
			GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_ONE:				// 255 0xFF per component
		grTexCombine( tmu, GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ZERO,
			GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ZERO, FXTRUE, FXTRUE );
		break;
//	case GR_TEXTURECOMBINE_DETAIL:			// blend (Cother, Clocal) detail textures with detail on selected TMU
//	case GR_TEXTURECOMBINE_DETAIL_OTHER:	// blend (Cother, Clocal) detail textures with detail on neighboring TMU
//	case GR_TEXTURECOMBINE_TRILINEAR_ODD:	// blend (Cother, Clocal) LOD blended textures with odd levels on selected TMU
//	case GR_TEXTURECOMBINE_TRILINEAR_EVEN:	// blend (Cother, Clocal) LOD blended textures with even levels on selected TMU
//		break;
	}
}

//*************************************************
//* Return the amount of unallocated texture memory on a Texture Mapping Unit
//*************************************************
DLLEXPORT FxU32 __stdcall 
guTexMemQueryAvail( GrChipID_t tmu )
{
#ifdef PARTDONE
	GlideMsg("guTexMemQueryAvail( %d )\n", tmu );
#endif
	if (tmu != GR_TMU0)
		return 0;

	return UTextures.MemQueryAvail(tmu);
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
guTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func )
{
#ifdef PARTDONE
	GlideMsg("guTexCombineFunction( %d, %d )\n", tmu, func );
#endif
	if (tmu != GR_TMU0)
		return;

	switch ( func )
	{
	case GR_TEXTURECOMBINE_ZERO:			// 0x00 per component
		grTexCombine( tmu, GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ZERO,
			GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ZERO, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_DECAL:			// Clocal decal texture
		grTexCombine( tmu, GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
			GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_OTHER:			// Cother pass through
		grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE,
			GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_ADD:				// Cother + Clocal additive texture
		grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_ONE,
			GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_MULTIPLY:		// Cother * Clocal modulated texture
		grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL,
			GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_SUBTRACT:		// Cother – Clocal subtractive texture
		grTexCombine( tmu, GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL, GR_COMBINE_FACTOR_ONE,
			GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL, GR_COMBINE_FACTOR_ONE, FXFALSE, FXFALSE );
		break;
	case GR_TEXTURECOMBINE_ONE:				// 255 0xFF per component
		grTexCombine( tmu, GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ZERO,
			GR_COMBINE_FUNCTION_ZERO, GR_COMBINE_FACTOR_ZERO, FXTRUE, FXTRUE );
		break;
//	case GR_TEXTURECOMBINE_DETAIL:			// blend (Cother, Clocal) detail textures with detail on selected TMU
//	case GR_TEXTURECOMBINE_DETAIL_OTHER:	// blend (Cother, Clocal) detail textures with detail on neighboring TMU
//	case GR_TEXTURECOMBINE_TRILINEAR_ODD:	// blend (Cother, Clocal) LOD blended textures with odd levels on selected TMU
//	case GR_TEXTURECOMBINE_TRILINEAR_EVEN:	// blend (Cother, Clocal) LOD blended textures with even levels on selected TMU
//		break;
	}
}

//----------------------------------------------------------------
DLLEXPORT GrMipMapId_t __stdcall 
guTexGetCurrentMipMap( GrChipID_t tmu )
{
#ifdef DONE
	GlideMsg("guTexGetCurrentMipMap( %d )\n", tmu );
#endif
	if (tmu != GR_TMU0)
		return GR_NULL_MIPMAP_HANDLE;

	return UTextures.GetCurrentMipMap( tmu );
}

//----------------------------------------------------------------
DLLEXPORT FxBool __stdcall 
guTexChangeAttributes( GrMipMapId_t mmid,
                       int width, int height,
                       GrTextureFormat_t fmt,
                       GrMipMapMode_t mm_mode,
                       GrLOD_t smallest_lod, GrLOD_t largest_lod,
                       GrAspectRatio_t aspect,
                       GrTextureClampMode_t s_clamp_mode,
                       GrTextureClampMode_t t_clamp_mode,
                       GrTextureFilterMode_t minFilterMode,
                       GrTextureFilterMode_t magFilterMode )
{
#ifdef DONE
	GlideMsg("guTexChangeAttributes( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d )\n",
		mmid, width, height, fmt, mm_mode, smallest_lod, largest_lod, aspect,
		s_clamp_mode, t_clamp_mode, minFilterMode, magFilterMode );
#endif

	return UTextures.ChangeAttributes( mmid, width, height, fmt, mm_mode,
		smallest_lod, largest_lod, aspect, s_clamp_mode, t_clamp_mode, 
		minFilterMode, magFilterMode );
}

//----------------------------------------------------------------
DLLEXPORT GrMipMapInfo * __stdcall 
guTexGetMipMapInfo( GrMipMapId_t mmid )
{
#ifdef DONE
	GlideMsg("guTexGetMipMapInfo\n");
#endif

	return UTextures.GetMipMapInfo( mmid );
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall 
guTexMemReset()
{
#ifdef PARTDONE
	GlideMsg("guTexMemReset\n");
#endif

	UTextures.MemReset();
	Textures->Clear();
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall 
guTexDownloadMipMapLevel( GrMipMapId_t mmid, GrLOD_t lod, const void **src )
{
#ifdef DONE
	GlideMsg("guTexDownloadMipMapLevel( %d, %d, --- )\n", mmid, lod );
#endif

	UTextures.DownloadMipMapLevel(mmid, lod, src);
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall 
guTexDownloadMipMap( GrMipMapId_t mmid, const void *src, const GuNccTable *table )
{
#ifdef DONE
	GlideMsg("guTexDownloadMipMap\n");
#endif

	UTextures.DownloadMipMap(mmid, src, table);
}

//----------------------------------------------------------------
DLLEXPORT GrMipMapId_t __stdcall 
guTexAllocateMemory( GrChipID_t tmu,
                     FxU8 odd_even_mask,
                     int width, int height,
                     GrTextureFormat_t fmt,
                     GrMipMapMode_t mm_mode,
                     GrLOD_t smallest_lod, GrLOD_t largest_lod,
                     GrAspectRatio_t aspect,
                     GrTextureClampMode_t s_clamp_mode,
                     GrTextureClampMode_t t_clamp_mode,
                     GrTextureFilterMode_t minfilter_mode,
                     GrTextureFilterMode_t magfilter_mode,
                     float lod_bias,
                     FxBool trilinear )
{
#ifdef DONE
	GlideMsg("guTexAllocateMemory( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d )\n",
		tmu, odd_even_mask, width, height, fmt, mm_mode, smallest_lod, largest_lod, aspect,
		s_clamp_mode, t_clamp_mode, minfilter_mode, magfilter_mode, lod_bias, trilinear );
#endif
	if (tmu != GR_TMU0)
		return GR_NULL_MIPMAP_HANDLE;

	return UTextures.AllocateMemory( tmu, odd_even_mask, width, height, fmt, mm_mode,
		smallest_lod, largest_lod, aspect, s_clamp_mode, t_clamp_mode,
		minfilter_mode, magfilter_mode, lod_bias, trilinear);
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall 
guTexSource( GrMipMapId_t id )
{
#ifdef DONE
	GlideMsg("guTexSource( %d )\n", id );
#endif

	RenderDrawTriangles();

    UTextures.Source(id);
}

//----------------------------------------------------------------
DLLEXPORT FxU16 * __stdcall
guTexCreateColorMipMap( void )
{
#ifdef NOTDONE
	GlideMsg("guTexCreateColorMipMap()\n"); 
#endif

	return 0;
}

