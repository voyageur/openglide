//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*					Textures Classes
//*					Made by Glorfindel
//**************************************************************

#include "GlOGL.h"
#include "GLTexture.h"
#include "GLextensions.h"
#include "profile.h"

// extern variables
DWORD GetTexSize(const int Lod, const int aspectRatio, const int format );

void MMXCopyByteFlip( void *Dst, void *Src, DWORD NumberOfBytes );
void MMXCopyMemory( void *Dst, void *Src, DWORD NumberOfBytes );
void MMXSetShort( void *Dst, short Value, DWORD NumberOfBytes );

// General Variables
float CurrentTextureWidth, CurrentTextureHeight;


//Functions
inline void GetTexInfo( const GrTexInfo *Info, TextureStruct *TexPointer )
{
	static int nLength;

	switch(Info->smallLod)
	{
	case GR_LOD_256:	nLength = 256;	break;
	case GR_LOD_128:	nLength = 128;	break;
	case GR_LOD_64:		nLength = 64;	break;
	case GR_LOD_32:		nLength = 32;	break;
	case GR_LOD_16:		nLength = 16;	break;
	case GR_LOD_8:		nLength = 8;	break;
	case GR_LOD_4:		nLength = 4;	break;
	case GR_LOD_2:		nLength = 2;	break;
	case GR_LOD_1:		nLength = 1;	break;
	}

	switch(Info->aspectRatio)
	{
	case GR_ASPECT_8x1:	TexPointer->Width = nLength;		TexPointer->Height = nLength >> 3;
						TexPointer->WAspect = D1OVER256;	TexPointer->HAspect = D8OVER256;			break;
	case GR_ASPECT_4x1:	TexPointer->Width = nLength;		TexPointer->Height = nLength >> 2;
						TexPointer->WAspect = D1OVER256;	TexPointer->HAspect = D4OVER256;			break;
	case GR_ASPECT_2x1:	TexPointer->Width = nLength;		TexPointer->Height = nLength >> 1;
						TexPointer->WAspect = D1OVER256;	TexPointer->HAspect = D2OVER256;			break;
	case GR_ASPECT_1x1:	TexPointer->Width = nLength;		TexPointer->Height = nLength;
						TexPointer->WAspect = D1OVER256;	TexPointer->HAspect = D1OVER256;			break;
	case GR_ASPECT_1x2:	TexPointer->Width = nLength >> 1;	TexPointer->Height = nLength;
						TexPointer->WAspect = D2OVER256;	TexPointer->HAspect = D1OVER256;			break;
	case GR_ASPECT_1x4:	TexPointer->Width = nLength >> 2;	TexPointer->Height = nLength;
						TexPointer->WAspect = D4OVER256;	TexPointer->HAspect = D1OVER256;			break;
	case GR_ASPECT_1x8:	TexPointer->Width = nLength >> 3;	TexPointer->Height = nLength;
						TexPointer->WAspect = D8OVER256;	TexPointer->HAspect = D1OVER256;			break;
	}

	TexPointer->NPixels = TexPointer->Width * TexPointer->Height;

	switch(Info->format)
	{
	case GR_TEXFMT_RGB_332:
	case GR_TEXFMT_YIQ_422:
	case GR_TEXFMT_ALPHA_8:
	case GR_TEXFMT_INTENSITY_8:
	case GR_TEXFMT_ALPHA_INTENSITY_44:
	case GR_TEXFMT_P_8:
		TexPointer->NBytes = TexPointer->NPixels;
		break;
	case GR_TEXFMT_RGB_565:
	case GR_TEXFMT_ARGB_8332:
	case GR_TEXFMT_AYIQ_8422:
	case GR_TEXFMT_ARGB_1555:
	case GR_TEXFMT_ARGB_4444:
	case GR_TEXFMT_ALPHA_INTENSITY_88:
	case GR_TEXFMT_AP_88:
		TexPointer->NBytes = TexPointer->NPixels << 1;
		break;
	}
}

WTexture::WTexture(void)
{
#ifdef DEBUG
	t565 = t4444 = t1555 = tp8 = ta8 = tai88 = ti8 = tai44 = t332 = t8332 = tap88 = 0;
	tayiq8422 = tyiq422 = trsvd0 = trsvd1 = trsvd2 = 0;
	NotUpdatedPalette	= 0;
#endif
	Memory = new BYTE[TEXTUREMEMORY];
	TexMem = new short[TEXTUREMEMORY];
	Textures = new TextureStruct[MAXTEXTURES];
	TexTemp = new DWORD[256*256*4];

	ActualTexture = 0;

	TextureMemory = TEXTUREMEMORY;

	Clear();
}

WTexture::WTexture( DWORD gTexMemory )
{
#ifdef DEBUG
	t565 = t4444 = t1555 = tp8 = ta8 = tai88 = ti8 = tai44 = t332 = t8332 = tap88 = 0;
	tayiq8422 = tyiq422 = trsvd0 = trsvd1 = trsvd2 = 0;
	NotUpdatedPalette	= 0;
#endif
	TextureMemory = gTexMemory;

	Memory = new BYTE[TextureMemory];
	TexMem = new short[TextureMemory];
	Textures = new TextureStruct[MAXTEXTURES];
	TexTemp = new DWORD[256*256*4];

	ActualTexture = 0;

	Clear();
}

WTexture::~WTexture(void)
{
	Clear();
	delete[] Memory;
	delete[] TexMem;
	delete[] Textures;
	delete[] TexTemp;
}

DWORD JumpBytes( const GrTexInfo *info )
{
	DWORD Data;
	int i;

	for( Data = 0, i = info->largeLod; i < info->smallLod; i++ )
	{
		Data += GetTexSize( i, info->aspectRatio, info->format );
	}
	return Data;
}

void WTexture::AddTexture( FxU32 Address, const GrTexInfo *Info )
{
	static TextureStruct	*TexPointer;
	static DWORD			nBytes;

	ProfileBegin( "AddTexture" );

	if ( ( Info->smallLod - Info->largeLod ) == 0 )
	{
		if ( TexMem[ Address ] == -1 )
		{
			TexMem[ Address ] = ActualTexture;
			if ( ++ActualTexture >= MAXTEXTURES )
			{
				Error( "WTexture::AddTexture ---- Maximum number of textures reached, reseting...\n" );
				Clear();
			}
		}

		TexPointer = &Textures[ TexMem[ Address ] ];

		GetTexInfo( Info, TexPointer );

		TexPointer->Lod				= Info->smallLod - Info->largeLod;
		TexPointer->Format			= Info->format;
		TexPointer->Data			= Memory + Address;

		if ( ( InternalConfig.MMXEnable ) && ( TexPointer->NBytes >= 8 ) )
		{
			if ( Info->format == GR_TEXFMT_AP_88 )
			{
				MMXCopyByteFlip( TexPointer->Data, Info->data, TexPointer->NBytes );
			}
			else
			{
				MMXCopyMemory( TexPointer->Data, Info->data, TexPointer->NBytes );
			}
		}
		else
		{
			CopyMemory( TexPointer->Data, Info->data, TexPointer->NBytes );
		}

		if ( InternalConfig.PaletteEXTEnable )
		{
			// Copy Texture Info for Palette Textures
			TexPointer->Palette = ( Info->format == GR_TEXFMT_AP_88 );
		}
		else
		{
			// Copy Texture Info for Palette Textures
			TexPointer->Palette = ( ( Info->format == GR_TEXFMT_P_8 ) || ( Info->format == GR_TEXFMT_AP_88 ) );
		}


		TexPointer->NeedUpdate = true;

		if ( TexPointer->Palette )
		{
			TexPointer->PaletteOpt.Clear();
		}
		else
		{
			if ( !TexPointer->Texture )
			{
				glGenTextures( 1, &TexPointer->Texture );
			}
		}

#ifdef DEBUG
		switch (Info->format)
		{
		case GR_TEXFMT_P_8:					tp8++;			break;
		case GR_TEXFMT_AP_88:				tap88++;		break;
		case GR_TEXFMT_RGB_565:				t565++;			break;
		case GR_TEXFMT_ARGB_4444:			t4444++;		break;
		case GR_TEXFMT_ARGB_1555:			t1555++;		break;
		case GR_TEXFMT_ALPHA_8:				ta8++;			break;
		case GR_TEXFMT_ALPHA_INTENSITY_88:	tai88++;		break;
		case GR_TEXFMT_INTENSITY_8:			ti8++;			break;
		case GR_TEXFMT_ALPHA_INTENSITY_44:	tai44++;		break;
		//GR_TEXFMT_RGB_332 or
		case GR_TEXFMT_8BIT:				t332++;			break;
		//GR_TEXFMT_ARGB_8332:
		case GR_TEXFMT_16BIT:				t8332++;		break;
		case GR_TEXFMT_YIQ_422:				tyiq422++;		break;
		case GR_TEXFMT_AYIQ_8422:			tayiq8422++;	break;
		case GR_TEXFMT_RSVD0:				trsvd0++;		break;
		case GR_TEXFMT_RSVD1:				trsvd1++;		break;
		case GR_TEXFMT_RSVD2:				trsvd1++;		break;
		}
#endif
	}
	else
	{
		nBytes = 0;
		for( int i = Info->largeLod; i < Info->smallLod; i++ )
		{
			nBytes += GetTexSize( i, Info->aspectRatio, Info->format );
		}
		nBytes += ((nBytes % 8) > 0) ? (8 - (nBytes % 8)) : 0;

		if ( TexMem[ Address ] == -1 )
		{
			Error( "AddTexture::Lod Error\n" );
		}
		else
		{
			TexMem[ Address+nBytes ] = TexMem[ Address ];
		}
	}

	ProfileEnd( "AddTexture" );
}

GLuint WTexture::TextureNumber( const FxU32 Address )
{
	return Textures[ TexMem[ Address ] ].Texture;
}

void WTexture::Clear(void)
{
	static int i;
	for( i = 0; i < ActualTexture; i++ )
	{
		if ( Textures[ i ].Texture )
		{
            Textures[ i ].PaletteOpt.Clear();
			glDeleteTextures( 1, &Textures[ i ].Texture );
		}
	}
	ActualTexture = 0;

	FillMemory( TexMem, TextureMemory * sizeof( short ), 255 ); // TexMem[ x ] = -1
}

TextureStruct *WTexture::SetSource( const FxU32 Address )
{
	static TextureStruct	*TexPointer;

	ProfileBegin( "SetSource" );

	if ( TexMem[ Address ] != -1 )
	{
		ProfileBegin( "BindTexture" );

		TexPointer = &Textures[ TexMem[ Address ] ];

		if ( TexPointer->Palette && ( TexPointer->PaletteOpt.GetTexture() != 0 ) )
		{
			glBindTexture( GL_TEXTURE_2D, TexPointer->PaletteOpt.GetTexture() );
		}
		else
		{
			glBindTexture( GL_TEXTURE_2D, TexPointer->Texture );
		}

		ProfileEnd( "BindTexture" );

		ProfileBegin( "NeedUpdate" );
		if ( TexPointer->NeedUpdate )
		{
			switch ( TexPointer->Format )
			{
			case GR_TEXFMT_RGB_565:
				if ( InternalConfig.OGLVersion < 2 )
				{
					if ( ( InternalConfig.MMXEnable ) && ( TexPointer->NPixels > 3 ) )
					{
						MMXConvert565to8888( TexPointer->Data, TexTemp, TexPointer->NPixels );
					}
					else
					{
						Convert565to8888( (WORD*)TexPointer->Data, TexTemp, TexPointer->NPixels );
					}
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 4, TexPointer->Width, TexPointer->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, 4, TexPointer->Width, TexPointer->Height, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
					}
				}
				else
				{
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 3, TexPointer->Width, TexPointer->Height, 0, GL_BGR_EXT, GL_UNSIGNED_SHORT_5_6_5_REV, TexPointer->Data );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, 3, TexPointer->Width, TexPointer->Height, GL_BGR_EXT, GL_UNSIGNED_SHORT_5_6_5_REV, TexPointer->Data );
					}
				}
				break;

			case GR_TEXFMT_ARGB_4444:
				if ( InternalConfig.OGLVersion < 2 )
				{
					if ( ( InternalConfig.MMXEnable ) && ( TexPointer->NPixels > 3 ) )
					{
						MMXConvert4444to8888( TexPointer->Data, TexTemp, TexPointer->NPixels );
					}
					else
					{
						Convert4444to8888( (WORD*)TexPointer->Data, TexTemp, TexPointer->NPixels );
					}
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 4, TexPointer->Width, TexPointer->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, 4, TexPointer->Width, TexPointer->Height, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
					}
				}
				else
				{
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 4, TexPointer->Width, TexPointer->Height, 0, GL_BGRA_EXT, GL_UNSIGNED_SHORT_4_4_4_4_REV, TexPointer->Data );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, 4, TexPointer->Width, TexPointer->Height, GL_BGRA_EXT, GL_UNSIGNED_SHORT_4_4_4_4_REV, TexPointer->Data );
					}
				}
				break;

			case GR_TEXFMT_ARGB_1555:
				if ( InternalConfig.OGLVersion < 2 )
				{
					if ( ( InternalConfig.MMXEnable ) && ( TexPointer->NPixels > 3 ) )
					{
						MMXConvert1555to8888( TexPointer->Data, TexTemp, TexPointer->NPixels );
					}
					else
					{
						Convert1555to8888( (WORD*)TexPointer->Data, TexTemp, TexPointer->NPixels );
					}
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 4, TexPointer->Width, TexPointer->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, 4, TexPointer->Width, TexPointer->Height, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
					}
				}
				else
				{
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 4, TexPointer->Width, TexPointer->Height, 0, GL_BGRA_EXT, GL_UNSIGNED_SHORT_1_5_5_5_REV, TexPointer->Data );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, 4, TexPointer->Width, TexPointer->Height, GL_BGRA_EXT, GL_UNSIGNED_SHORT_1_5_5_5_REV, TexPointer->Data );
					}
				}
				break;

			case GR_TEXFMT_P_8:
				if ( InternalConfig.PaletteEXTEnable )
				{
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, GL_COLOR_INDEX8_EXT, TexPointer->Width, TexPointer->Height, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, TexPointer->Data );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, GL_COLOR_INDEX8_EXT, TexPointer->Width, TexPointer->Height, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, TexPointer->Data );
					}
				}
				break;

			case GR_TEXFMT_ALPHA_8:
				ConvertA8toAP88( (BYTE*)TexPointer->Data, (WORD*)TexTemp, TexPointer->NPixels );
				glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 2, TexPointer->Width, TexPointer->Height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, TexTemp );
				if ( InternalConfig.BuildMipMaps )
				{
					gluBuild2DMipmaps( GL_TEXTURE_2D, 2, TexPointer->Width, TexPointer->Height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, TexTemp );
				}
				break;

			case GR_TEXFMT_ALPHA_INTENSITY_88:
				glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 2, TexPointer->Width, TexPointer->Height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, TexPointer->Data );
				if ( InternalConfig.BuildMipMaps )
				{
					gluBuild2DMipmaps( GL_TEXTURE_2D, 2, TexPointer->Width, TexPointer->Height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, TexPointer->Data );
				}
				break;

			case GR_TEXFMT_INTENSITY_8:
				glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 1, TexPointer->Width, TexPointer->Height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, TexPointer->Data );
				if ( InternalConfig.BuildMipMaps )
				{
					gluBuild2DMipmaps( GL_TEXTURE_2D, 1, TexPointer->Width, TexPointer->Height, GL_LUMINANCE, GL_UNSIGNED_BYTE, TexPointer->Data );
				}
				break;

			case GR_TEXFMT_ALPHA_INTENSITY_44:
				ConvertAI44toAP88( (BYTE*)TexPointer->Data, (WORD*)TexTemp, TexPointer->NPixels );
				glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 2, TexPointer->Width, TexPointer->Height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, TexTemp );
				if ( InternalConfig.BuildMipMaps )
				{
					gluBuild2DMipmaps( GL_TEXTURE_2D, 2, TexPointer->Width, TexPointer->Height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, TexTemp );
				}
				break;

			case GR_TEXFMT_8BIT://GR_TEXFMT_RGB_332
				if ( InternalConfig.PackedPixelsEXTEnable )
				{
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 3, TexPointer->Width, TexPointer->Height, 0, GL_RGB, GL_UNSIGNED_BYTE_3_3_2_EXT, TexPointer->Data );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, 3, TexPointer->Width, TexPointer->Height, GL_RGB, GL_UNSIGNED_BYTE_3_3_2_EXT, TexPointer->Data );
					}
				}
				else
				{
					Convert332to8888( (BYTE*)TexPointer->Data, TexTemp, TexPointer->NPixels );
					glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 4, TexPointer->Width, TexPointer->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
					if ( InternalConfig.BuildMipMaps )
					{
						gluBuild2DMipmaps( GL_TEXTURE_2D, 4, TexPointer->Width, TexPointer->Height, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
					}
				}
				break;

			case GR_TEXFMT_16BIT://GR_TEXFMT_ARGB_8332:
				Convert8332to8888( (WORD*)TexPointer->Data, TexTemp, TexPointer->NPixels );
				glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 4, TexPointer->Width, TexPointer->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
				if ( InternalConfig.BuildMipMaps )
				{
					gluBuild2DMipmaps( GL_TEXTURE_2D, 4, TexPointer->Width, TexPointer->Height, GL_RGBA, GL_UNSIGNED_BYTE, TexTemp );
				}
				break;

			case GR_TEXFMT_YIQ_422:
			case GR_TEXFMT_AYIQ_8422:
			case GR_TEXFMT_RSVD0:
			case GR_TEXFMT_RSVD1:
			case GR_TEXFMT_RSVD2:
				Error("grTexDownloadMipMapLevel - Unsupported format(%d)\n", TexPointer->Format);
				FillMemory( TexTemp, TexPointer->NBytes, 255 );
				glTexImage2D( GL_TEXTURE_2D, TexPointer->Lod, 1, TexPointer->Width, TexPointer->Height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, TexTemp );
				gluBuild2DMipmaps( GL_TEXTURE_2D, 1, TexPointer->Width, TexPointer->Height, GL_LUMINANCE, GL_UNSIGNED_BYTE, TexTemp );
				break;
			}
			TexPointer->NeedUpdate = false;
		}
		ProfileEnd( "NeedUpdate" );

		ProfileBegin( "TexParameters" );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OpenGL.MinFilterMode );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OpenGL.MagFilterMode );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OpenGL.SClampMode );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OpenGL.TClampMode );

		CurrentTextureWidth = TexPointer->WAspect;
		CurrentTextureHeight = TexPointer->HAspect;

		ProfileEnd( "TexParameters" );

#ifdef OPENGL_DEBUG
	GLErro( "Texture::SetSource" );
#endif
		ProfileEnd( "SetSource" );

		return TexPointer;
	}
#ifdef DEBUG
	Error("Texture Not found\n");
#endif

	ProfileEnd( "SetSource" );

	return &Textures[0];
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////////	WUTexture Class
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


WUTexture::WUTexture()
{
	NumberOfTextures = 0;
	Source = GR_NULL_MIPMAP_HANDLE;
	TexMemoryCurrentPosition = 0;

	Textures = new GrMipMapInfo[MAXTEXTURES];
	OGLInfo = new OpenGLTexture[MAXTEXTURES];
}

WUTexture::~WUTexture()
{
	Clear();
	delete[] Textures;
	delete[] OGLInfo;
}

GrMipMapId_t WUTexture::Allocate( 
							GrChipID_t tmu,
							FxU8 odd_even_mask,
							int width, int height,
							GrTextureFormat_t format,
							GrMipMapMode_t mipmap_mode,
							GrLOD_t lod_min, GrLOD_t lod_max,
							GrAspectRatio_t aspect_ratio,
							GrTextureClampMode_t s_clamp_mode,
							GrTextureClampMode_t t_clamp_mode,
							GrTextureFilterMode_t minfilter_mode,
							GrTextureFilterMode_t magfilter_mode,
							float lod_bias,
							FxBool trilinear )
{
	Textures[NumberOfTextures].valid			= FXTRUE;
	Textures[NumberOfTextures].width			= width;
	Textures[NumberOfTextures].height			= height;
	Textures[NumberOfTextures].aspect_ratio		= aspect_ratio;
	Textures[NumberOfTextures].format			= format;
	Textures[NumberOfTextures].mipmap_mode		= mipmap_mode;
	Textures[NumberOfTextures].magfilter_mode	= magfilter_mode;
	Textures[NumberOfTextures].minfilter_mode	= minfilter_mode;
	Textures[NumberOfTextures].s_clamp_mode		= s_clamp_mode;
	Textures[NumberOfTextures].t_clamp_mode		= t_clamp_mode;
	Textures[NumberOfTextures].tLOD				= 0;				//?????
	Textures[NumberOfTextures].tTextureMode		= 0;				//?????
	Textures[NumberOfTextures].lod_bias			= (DWORD)lod_bias;	// I have to preshift this value to 4.2... maybe later... ;-)
	Textures[NumberOfTextures].lod_min			= lod_min;
	Textures[NumberOfTextures].lod_max			= lod_max;
	Textures[NumberOfTextures].tmu				= tmu;
	Textures[NumberOfTextures].odd_even_mask	= odd_even_mask;
	Textures[NumberOfTextures].trilinear		= trilinear;
	Textures[NumberOfTextures].sst				= -1;
	Textures[NumberOfTextures].data				= NULL;
	Textures[NumberOfTextures].tmu_base_address	= TexMemoryCurrentPosition;

	TexMemoryCurrentPosition += grTexCalcMemRequired( lod_min, lod_max, aspect_ratio, format );
	
	switch (minfilter_mode)
	{
	case GR_TEXTUREFILTER_POINT_SAMPLED:
//		if (mipmap_mode != GR_MIPMAP_DISABLE)
//			OGLInfo[NumberOfTextures].MinFilterMode = GL_NEAREST_MIPMAP_LINEAR;
//		else
			OGLInfo[NumberOfTextures].MinFilterMode = GL_NEAREST;
		break;
	case GR_TEXTUREFILTER_BILINEAR:
//		if (mipmap_mode != GR_MIPMAP_DISABLE)
//			OGLInfo[NumberOfTextures].MinFilterMode = GL_LINEAR_MIPMAP_LINEAR;
//		else
			OGLInfo[NumberOfTextures].MinFilterMode = GL_LINEAR;
		break;
	}
	switch (magfilter_mode)
	{
	case GR_TEXTUREFILTER_POINT_SAMPLED:	OGLInfo[NumberOfTextures].MagFilterMode = GL_NEAREST;	break;
	case GR_TEXTUREFILTER_BILINEAR:			OGLInfo[NumberOfTextures].MagFilterMode = GL_LINEAR;	break;
	}
	switch (s_clamp_mode)
	{
	case GR_TEXTURECLAMP_CLAMP:				OGLInfo[NumberOfTextures].SClampMode = GL_CLAMP;		break;
	case GR_TEXTURECLAMP_WRAP:				OGLInfo[NumberOfTextures].SClampMode = GL_REPEAT;		break;
	}
	switch (t_clamp_mode)
	{
	case GR_TEXTURECLAMP_CLAMP:				OGLInfo[NumberOfTextures].TClampMode = GL_CLAMP;		break;
	case GR_TEXTURECLAMP_WRAP:				OGLInfo[NumberOfTextures].TClampMode = GL_REPEAT;		break;
	}

	NumberOfTextures++;
	if (NumberOfTextures >= MAXTEXTURES)
	{
		Error( "gu texture memory exausted!!!\n" );
		return GR_NULL_MIPMAP_HANDLE;
	}
	else
		return (GrMipMapId_t) (NumberOfTextures-1);
}

FxBool WUTexture::ChangeAttributes(	
							GrMipMapId_t mmid,
							int width, int height,
							GrTextureFormat_t format,
							GrMipMapMode_t mipmap_mode,
							GrLOD_t lod_min, GrLOD_t lod_max,
							GrAspectRatio_t aspect_ratio,
							GrTextureClampMode_t s_clamp_mode,
							GrTextureClampMode_t t_clamp_mode,
							GrTextureFilterMode_t minfilter_mode,
							GrTextureFilterMode_t magfilter_mode )
{
	if ( width != -1 )					Textures[mmid].width			= width;
	if ( height != -1 )					Textures[mmid].height			= height;
	if ( aspect_ratio != -1 )			Textures[mmid].aspect_ratio		= aspect_ratio;
	if ( format != -1 )					Textures[mmid].format			= format;
	if ( mipmap_mode != -1 )			Textures[mmid].mipmap_mode		= mipmap_mode;
	if ( magfilter_mode != -1 )			Textures[mmid].magfilter_mode	= magfilter_mode;
	if ( minfilter_mode != -1 )			Textures[mmid].minfilter_mode	= minfilter_mode;
	if ( s_clamp_mode != -1 )			Textures[mmid].s_clamp_mode		= s_clamp_mode;
	if ( t_clamp_mode != -1 )			Textures[mmid].t_clamp_mode		= t_clamp_mode;
	if ( lod_min != -1 )				Textures[mmid].lod_min			= lod_min;
	if ( lod_max != -1 )				Textures[mmid].lod_max			= lod_max;

	switch (minfilter_mode)
	{
	case -1:
		break;
	case GR_TEXTUREFILTER_POINT_SAMPLED:
//		if (mipmap_mode != GR_MIPMAP_DISABLE)
//			OGLInfo[NumberOfTextures].MinFilterMode = GL_NEAREST_MIPMAP_LINEAR;
//		else
			OGLInfo[NumberOfTextures].MinFilterMode = GL_NEAREST;
		break;
	case GR_TEXTUREFILTER_BILINEAR:
//		if (mipmap_mode != GR_MIPMAP_DISABLE)
//			OGLInfo[NumberOfTextures].MinFilterMode = GL_LINEAR_MIPMAP_LINEAR;
//		else
			OGLInfo[NumberOfTextures].MinFilterMode = GL_LINEAR;
		break;
	}
	switch (magfilter_mode)
	{
	case -1:	break;
	case GR_TEXTUREFILTER_POINT_SAMPLED:	OGLInfo[NumberOfTextures].MagFilterMode = GL_NEAREST;	break;
	case GR_TEXTUREFILTER_BILINEAR:			OGLInfo[NumberOfTextures].MagFilterMode = GL_LINEAR;	break;
	}
	switch (s_clamp_mode)
	{
	case -1:	break;
	case GR_TEXTURECLAMP_CLAMP:				OGLInfo[NumberOfTextures].SClampMode = GL_CLAMP;		break;
	case GR_TEXTURECLAMP_WRAP:				OGLInfo[NumberOfTextures].SClampMode = GL_REPEAT;		break;
	}
	switch (t_clamp_mode)
	{
	case -1:	break;
	case GR_TEXTURECLAMP_CLAMP:				OGLInfo[NumberOfTextures].TClampMode = GL_CLAMP;		break;
	case GR_TEXTURECLAMP_WRAP:				OGLInfo[NumberOfTextures].TClampMode = GL_REPEAT;		break;
	}

	return FXTRUE;
}

GrMipMapId_t WUTexture::CurrentMipMap( GrChipID_t tmu )
{
	return Source;
}

GrMipMapInfo *WUTexture::TextureInfo( GrMipMapId_t mmid )
{
	return &Textures[mmid];
}

void WUTexture::SetSource( GrMipMapId_t id )
{
	Source = id;

	grTexSource( GR_TMU0, Textures[id].tmu_base_address, GR_MIPMAPLEVELMASK_BOTH, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OGLInfo[id].MinFilterMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OGLInfo[id].MagFilterMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGLInfo[id].SClampMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGLInfo[id].TClampMode );

#ifdef OPENGL_DEBUG
	GLErro( "UTexture::SetSource" );
#endif
}

void WUTexture::SetOpenGLTexture( GrMipMapId_t mmid , GLuint texturenumber )
{
	OGLInfo[mmid].TextureNumber = texturenumber;
}

void WUTexture::Clear()
{
	NumberOfTextures = 1;
	Source = GR_NULL_MIPMAP_HANDLE;
	TexMemoryCurrentPosition = 0;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Conversion Routines
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

inline void Convert565to5551( WORD *Buffer1, WORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = ((*Buffer1) & 0xFFC0) |
					(((*Buffer1++) & 0x001F) << 1) |
					0x0001;
		Pixels--;
	}
}

inline void Convert565to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = 0xFF000000 |					// A
			((*Buffer1)		& 0x001F) << 19 |		// B
			((*Buffer1)		& 0x07E0) << 5  |		// G
			((*Buffer1++)	& 0xF800) >> 8;			// R
		Pixels--;
	}
}

inline void Convert4444to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = ((*Buffer1)	& 0xF000) << 16 |	// A
					 ((*Buffer1)	& 0x000F) << 20 |	// B
					 ((*Buffer1)	& 0x00F0) <<  8 |	// G
					 ((*Buffer1++)	& 0x0F00) >>  4;	// R
		Pixels--;
	}
}

inline void Convert4444to4444( WORD *Buffer1, WORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = (((*Buffer1) << 4 ) | ((*Buffer1++) >> 12 ));
		Pixels--;
	}
}

inline void Convert1555to5551( WORD *Buffer1, WORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = (((*Buffer1) << 1 ) | ((*Buffer1++) >> 15 ));
		Pixels--;
	}
}

inline void Convert1555to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = 
			(((*Buffer1)		& 0x8000) ? 0xFF000000 : 0) |		// A
			(((*Buffer1)		& 0x001F)		<< 19)		|		// B
			(((*Buffer1)		& 0x03E0)		<<  6)		|		// G
			(((*Buffer1++)		& 0x7C00)		>>  7);				// R
		Pixels--;
	}
}

inline void ConvertA8toAP88( BYTE *Buffer1, WORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = (((*Buffer1) << 8) | (*Buffer1++));
		Pixels--;
	}
}

inline void ConvertA8to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = 
			((*Buffer1) << 24)		|		// A
			((*Buffer1) << 16)		|		// B
			((*Buffer1) <<  8)		|		// G
			(*Buffer1++);					// R
		Pixels--;
	}
}

inline void ConvertI8to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = 
			0xFF000000				|		// A
			((*Buffer1) << 16)		|		// B
			((*Buffer1) <<  8)		|		// G
			(*Buffer1++);					// R
		Pixels--;
	}
}

inline void ConvertAI88to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while(Pixels)
	{
		*Buffer2++ = 
			(((*Buffer1)		& 0xFF00)	<< 16)		|		// A
			(((*Buffer1)		& 0x00FF)	<< 16)		|		// B
			(((*Buffer1)		& 0x00FF)	<<  8)		|		// G
			((*Buffer1++)		& 0x00FF);						// R
		Pixels--;
	}
}

inline void Convert332to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	DWORD R, G, B, A = 0xFF000000;
	for( DWORD i = Pixels; i > 0; i-- )
	{
		R = (((*Buffer1) >> 5) & 0x07) << 5;
		G = (((*Buffer1) >> 2) & 0x07) << 5;
		B = ((*Buffer1) & 0x03) << 6;
		*Buffer2 = A | (B << 16) | (G << 8) | R;
		Buffer1++;
		Buffer2++;
	}
}

inline void ConvertAI44toAP88( BYTE *Buffer1, WORD *Buffer2, DWORD Pixels )
{
	for( DWORD i = Pixels; i > 0; i-- )
	{
		*Buffer2++ = ((((*Buffer1) & 0xF0) << 8) | (((*Buffer1++) & 0x0F) << 4));
	}
}

inline void ConvertAI44to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	DWORD A, I;
	for( DWORD i = Pixels; i > 0; i-- )
	{
		A = (((*Buffer1) >> 4) & 0x0F) << 4;
		I = ((*Buffer1) & 0x0F) << 4;
		*Buffer2 = (A << 24) | (I << 16) | (I << 8) | I;
		Buffer1++;
		Buffer2++;
	}
}

inline void Convert8332to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	DWORD R, G, B, A;
	for( DWORD i = Pixels; i > 0; i-- )
	{
		A = (((*Buffer1) >> 8) & 0xFF);
		R = (((*Buffer1) >> 5) & 0x07) << 5;
		G = (((*Buffer1) >> 2) & 0x07) << 5;
		B = ((*Buffer1) & 0x03) << 6;
		*Buffer2 = (A << 24) | (B << 16) | (G << 8) | R;
		Buffer1++;
		Buffer2++;
	}
}

/*
inline void ConvertP8to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while(Pixels--)
	{
		*Buffer2 = 0xFF000000 |
			((Glide.ColorPalette.data[*Buffer1] & 0x000000FF) << 16) | 
			(Glide.ColorPalette.data[*Buffer1] & 0x0000FF00) | 
			((Glide.ColorPalette.data[*Buffer1] & 0x00FF0000) >> 16);
		Buffer1++;
		Buffer2++;
	}
}

inline void ConvertAP88to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	DWORD R, G, B, A;
	for( DWORD i = Pixels; i > 0; i-- )
	{
		R = (Glide.ColorPalette.data[(*Buffer1)&0x00FF] & 0x00FF0000) >> 16;
		G = (Glide.ColorPalette.data[(*Buffer1)&0x00FF] & 0x0000FF00) >> 8;
		B = (Glide.ColorPalette.data[(*Buffer1)&0x00FF] & 0x000000FF);
		A = ((*Buffer1) & 0xFF00) >> 8;
		*Buffer2 = (A << 24)| (B << 16) | (G << 8) | R;
		Buffer1++;
		Buffer2++;
	}
}
*/

BYTE Mask44441[8] = { 0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0 };
BYTE Mask44442[8] = { 0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F };
BYTE Mask44443[8] = { 0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0xFF };

inline void MMXConvert4444to8888( void *Src, void *Dst, DWORD NumberOfPixels )
{
	// Word entered is ARGB
	// Has to be ABGR
	__asm
	{
		MOVQ MM6, [Mask44441]
		mov ECX, NumberOfPixels
		MOVQ MM7, [Mask44442]
		mov EDX, Dst
		MOVQ MM5, [Mask44443]
		shr ECX, 2
		mov EAX, Src
copying:
		MOVQ MM0, [EAX]				//u
		add EAX, 8					//v
		MOVQ MM1, MM0
		PAND MM0, MM6
		PAND MM1, MM7
		PSLLQ MM1, 4

		// Packing and Reversing RGB order
		MOVQ MM2, MM0
		PUNPCKHBW MM0, MM1
		PUNPCKLBW MM2, MM1

		// Shifting Alpha Value
		MOVQ MM3, MM0
		MOVQ MM4, MM2
		PAND MM3, MM5
		PAND MM4, MM5
		PSRLQ MM3, 24
		PSRLQ MM4, 24

		// Shifting RGB Value
		PSLLD MM0, 8
		PSLLD MM2, 8

		// Joining RGB and A
		POR MM0, MM3
		POR MM2, MM4

		// Storing Unpacked 
		MOVQ [EDX], MM2
		add EDX, 16
		MOVQ [EDX-8], MM0
		dec ECX
		jnz copying
		EMMS
	}
}

BYTE Mask565A[8] = { 0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF };
BYTE Mask565B[8] = { 0x00,0xF8,0x00,0xF8,0x00,0xF8,0x00,0xF8 };
BYTE Mask565G[8] = { 0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07 };
BYTE Mask565R[8] = { 0x1F,0x00,0x1F,0x00,0x1F,0x00,0x1F,0x00 };

inline void MMXConvert565to8888( void *Src, void *Dst, DWORD NumberOfPixels )
{
	// Word entered is ARGB
	// Has to be ABGR
	__asm
	{
		MOVQ MM7, [Mask565A]
		mov ECX, NumberOfPixels
		MOVQ MM6, [Mask565B]
		mov EAX, Src
		MOVQ MM5, [Mask565G]
		shr ECX, 2
		MOVQ MM4, [Mask565R]
		mov EDX, Dst
copying:
		MOVQ MM0, [EAX]
		add EAX, 8
		MOVQ MM2, MM0
		MOVQ MM1, MM0

		PAND MM0, MM4 // Mask R
		PAND MM2, MM6 // Mask B
		PSLLW MM0, 11 // Shift R
		PAND MM1, MM5 // Mask G

		PSRLW MM2, 8  // Shift B

		MOVQ MM3, MM1
		PSLLW MM1, 13
		POR MM0, MM2
		PSRLW MM3, 3
		POR MM1, MM3

		POR MM1, MM7

		MOVQ MM2, MM0
		PUNPCKHBW MM0, MM1
		add EDX, 16
		PUNPCKLBW MM2, MM1

		// Storing Unpacked 
		MOVQ [EDX-8], MM0
		dec ECX
		MOVQ [EDX-16], MM2
		jnz copying
		EMMS
	}
}

BYTE Mask1555A[8] = { 0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80 };
BYTE Mask1555R[8] = { 0x1F,0x00,0x1F,0x00,0x1F,0x00,0x1F,0x00 };
BYTE Mask1555G[8] = { 0xE0,0x03,0xE0,0x03,0xE0,0x03,0xE0,0x03 };
BYTE Mask1555B[8] = { 0x00,0x7C,0x00,0x7C,0x00,0x7C,0x00,0x7C };
BYTE Mask1555I[8] = { 0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80 };
/*
//void __stdcall MMXConvert1555to8888( void *Src, void *Dst, DWORD NumberOfPixels )
inline void MMXConvert1555to8888( void *Src, void *Dst, DWORD NumberOfPixels )
{
	// Word entered is ARGB
	// Has to be ABGR
	__asm
	{
		mov EAX, Src
		mov EDX, Dst
		mov ECX, NumberOfPixels
		shr ECX, 2
		MOVQ MM7, [Mask1555A]
		MOVQ MM6, [Mask1555B]
		MOVQ MM5, [Mask1555G]
copying:
		MOVQ MM0, [EAX]
		add EAX, 8
		MOVQ MM1, MM0
		MOVQ MM2, MM0
		MOVQ MM3, MM0

		PAND MM0, [Mask1555R] // Mask R
		PAND MM1, MM5 // Mask G
		PAND MM2, MM6 // Mask B
		PAND MM3, MM7 // Mask A

		PSLLW MM0, 11  // Shift R
		PSRLW MM2, 7  // Shift B

		MOVQ MM4, MM1 // Shift G
		PSLLW MM1, 14
		PSRLW MM4, 2
		POR MM1, MM4

		PCMPEQB MM3, [Mask1555I]

		POR MM0, MM2
		POR MM1, MM3

		MOVQ MM2, MM0
		PUNPCKHBW MM0, MM1
		PUNPCKLBW MM2, MM1

		// Storing Unpacked 
		MOVQ [EDX], MM2
		add EDX, 8
		MOVQ [EDX], MM0
		add EDX, 8
		dec ECX
		jnz copying
		EMMS
	}
}
*/
inline void MMXConvert1555to8888( void *Src, void *Dst, DWORD NumberOfPixels )
{
	// Word entered is ARGB
	// Has to be ABGR
	__asm
	{
		MOVQ MM7, [Mask1555A]
		mov ECX, NumberOfPixels
		MOVQ MM6, [Mask1555B]
		mov EDX, Dst
		shr ECX, 2
		mov EAX, Src
		MOVQ MM5, [Mask1555G]
copying:
		MOVQ MM0, [EAX]				//u
		add EAX, 8					//v
		MOVQ MM1, MM0				//u
		MOVQ MM2, MM0				//v
		PAND MM0, [Mask1555R]		//u
		MOVQ MM3, MM1				//v

		PAND MM1, MM5				//u
		PAND MM2, MM6				//v
		PAND MM3, MM7				//u

		PSLLW MM0, 11				//v
		MOVQ MM4, MM1				//u
		PSRLW MM2, 7				//v

		PSLLW MM1, 14				//u
		POR MM0, MM2				//v
		PSRLW MM4, 2				//u
		POR MM1, MM4				//v

		PCMPEQB MM3, [Mask1555I]	//u
		MOVQ MM2, MM0				//v

		POR MM1, MM3				//v

		PUNPCKHBW MM0, MM1			//u
		PUNPCKLBW MM2, MM1

		MOVQ [EDX], MM2
		add EDX, 16
		MOVQ [EDX-8], MM0
//		add EDX, 8
		dec ECX
		jnz copying
		EMMS
	}
}


CPalette::CPalette()
{
	PMaxTextures = 300;//DEFPALETTE;

	Init();
}

CPalette::CPalette( int NumberOfTextures )
{
	PMaxTextures = NumberOfTextures;

	Init();
}

CPalette::~CPalette( )
{
	delete[] TextureNumbers;
	delete[] PaletteNumbers;
}

void CPalette::Init( void )
{
	CurrentNumber = ActualNumber = 0;

	TextureNumbers = new GLuint[PMaxTextures];
	PaletteNumbers = new FxU32[PMaxTextures];

//	Clear();

	for( int i = 0; i < PMaxTextures; i++ )
	{
		PaletteNumbers[i] = 0;
		TextureNumbers[i] = 0;
	}
}

FxU32 CPalette::GetPalette( void )
{
	return PaletteNumbers[CurrentNumber];
}

GLuint CPalette::GetTexture( void )
{
	return TextureNumbers[CurrentNumber];
}

GLuint CPalette::SetPalette( FxU32 gPalette )
{
	Next();

	PaletteNumbers[CurrentNumber] = gPalette;
	if ( TextureNumbers[CurrentNumber] == 0 )
	{
		glGenTextures( 1, &TextureNumbers[CurrentNumber] );
	}

	return TextureNumbers[CurrentNumber];
}

GLuint CPalette::SearchPalette( FxU32 gPalette )
{
	for( int i = 0; i < ActualNumber; i++ )
	{
		if ( PaletteNumbers[i] == gPalette )
		{
			CurrentNumber = i;
			return TextureNumbers[i];
		}
	}

	return 0;
}

void CPalette::Clear( void )
{
	for( int i = 0; i < ActualNumber; i++ )
	{
        glDeleteTextures(1, &TextureNumbers[i]);
        TextureNumbers[i] = 0;
		PaletteNumbers[i] = 0;
	}

    CurrentNumber = ActualNumber = 0;
}

void CPalette::Next( void )
{
	if ( ActualNumber >= PMaxTextures )
        Clear();

	CurrentNumber = ActualNumber++;
}
