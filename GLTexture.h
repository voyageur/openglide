//**************************************************************
//*				Glide->OpenGL Wrapper
//*				   Texture Classes
//*				 Made by Glorfindel
//**************************************************************

#ifndef __GLTEXTURE_H__
#define __GLTEXTURE_H__


#define TEXTUREMEMORY			4194304					// 4*1024*1024
#define MAXTEXTURES				5000
#define DEFPALETTE				64

#include "PGTexture.h"

struct TexSourceStruct
{
	FxU32		StartAddress;
	FxU32		EvenOdd;
	GrTexInfo	Info;
};

class CPalette
{
public:
	CPalette();
	CPalette( int NumberOfTextures );
	~CPalette( );

	FxU32 GetPalette( void );
	GLuint GetTexture( void );
	GLuint SetPalette( FxU32 gPalette );
	GLuint SearchPalette( FxU32 gPalette );
	void Clear( void );

private:

	void Init( void );
	void Next( void );

	GLuint	* TextureNumbers;
	FxU32	* PaletteNumbers;

	int		ActualNumber;
    int     CurrentNumber;
	int		PMaxTextures;
};


class TextureStruct
{
public:
	DWORD				Width, Height, Lod;			// Width, Height and OpenGL Lod
	float				WAspect, HAspect;			// Real aspect multiplier
//	FxU32				StartAddress, 
//						EndAddress;	// Start and End Address in Voodoo Memory
	GLuint				Texture;					// Number of OpenGL texture
	FxU32				Palette;					// Optimization for not updating palette textures
	CPalette			PaletteOpt;
	FxU32				Format;						// Format of texture
	DWORD				NBytes;						// Number of Bytes
	DWORD				NPixels;					// Number of Pixels
	BYTE				*Data;						// Points to the Lod Data in the Voodoo Memory
	bool				NeedUpdate;					// If texture needs an update in SetSource
	FxU32				PaletteEXT;

    TextureStruct()
    {
        Width = 0;
        Height = 0;
        WAspect = 0.0f;
        HAspect = 0.0f;
        Texture = 0;
        Palette = 0;
        Format = 0;
        NBytes = 0;
        NPixels = 0;
        Data = NULL;
        NeedUpdate = false;
        PaletteEXT = 0;
    }
};

class WTexture
{
private:
	TextureStruct		*Textures;
	short				*TexMem;
	BYTE				*Memory;
	DWORD				*TexTemp;

	DWORD			TextureMemory;


public:
	int					ActualTexture;

#ifdef DEBUG
	DWORD				t565, t4444, t1555, tp8, ta8, tai88, ti8, tai44, t332, t8332, tap88;
	DWORD				tayiq8422, tyiq422, trsvd0, trsvd1, trsvd2;
	// Number of paletted textures not updated
	DWORD				NotUpdatedPalette;
#endif
	
	WTexture(void);
	WTexture( DWORD TexMemory );
	~WTexture(void);
	void AddTexture( FxU32 Address, const GrTexInfo *Info );
	GLuint TextureNumber( const FxU32 Address );
	TextureStruct *SetSource( const FxU32 Address );
	void Clear(void);
};

struct OpenGLTexture
{
	GLuint		TextureNumber;
	GLenum		SClampMode;
	GLenum		TClampMode;
	GLenum		MinFilterMode;
	GLenum		MagFilterMode;
};

class WUTexture
{
private:
	GrMipMapInfo	*Textures;
	OpenGLTexture	*OGLInfo;
	// ID of Source MipMap
	FxU32			Source;
	// Number of textures loaded
	int				NumberOfTextures;

public:
	DWORD			TexMemoryCurrentPosition;

	WUTexture(void);
	~WUTexture(void);
	GrMipMapId_t Allocate(	GrChipID_t tmu,
							FxU8 odd_even_mask,
							int width, int height,
							GrTextureFormat_t format,
							GrMipMapMode_t mm_mode,
							GrLOD_t smallest_lod, GrLOD_t largest_lod,
							GrAspectRatio_t aspect,
							GrTextureClampMode_t s_clamp_mode,
							GrTextureClampMode_t t_clamp_mode,
							GrTextureFilterMode_t minfilter_mode,
							GrTextureFilterMode_t magfilter_mode,
							float lod_bias,
							FxBool trilinear );
	FxBool ChangeAttributes(GrMipMapId_t mmid,
							int width, int height,
							GrTextureFormat_t format,
							GrMipMapMode_t mipmap_mode,
							GrLOD_t lod_min, GrLOD_t lod_max,
							GrAspectRatio_t aspect_ratio,
							GrTextureClampMode_t s_clamp_mode,
							GrTextureClampMode_t t_clamp_mode,
							GrTextureFilterMode_t minFilterMode,
							GrTextureFilterMode_t magFilterMode );
	GrMipMapInfo *TextureInfo( GrMipMapId_t mmid );
	GrMipMapId_t CurrentMipMap( GrChipID_t tmu );
	void SetSource( GrMipMapId_t id );
	void SetOpenGLTexture( GrMipMapId_t mmid , GLuint texturenumber );
	void Clear(void);
};


//extern WTexture			*Textures;
extern PGTexture			*Textures;
extern WUTexture		UTextures;

// Conversions Routines Prototypes
inline void Convert565to5551( WORD *Buffer1, WORD *Buffer2, DWORD Pixels );
inline void Convert565to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void Convert4444to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void Convert4444to4444( WORD *Buffer1, WORD *Buffer2, DWORD Pixels );
inline void Convert1555to5551( WORD *Buffer1, WORD *Buffer2, DWORD Pixels );
inline void Convert1555to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void ConvertA8to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void ConvertA8toAP88( BYTE *Buffer1, WORD *Buffer2, DWORD Pixels );
inline void ConvertI8to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void ConvertAI88to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void Convert332to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void ConvertAI44toAP88( BYTE *Buffer1, WORD *Buffer2, DWORD Pixels );
inline void ConvertAI44to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void Convert8332to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void ConvertP8to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels );
inline void ConvertAP88to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels );

//void __stdcall MMXConvert4444to8888( void *Src, void *Dst, DWORD NumberOfPixels );
//void __stdcall MMXConvert565to8888( void *Src, void *Dst, DWORD NumberOfPixels );
//void __stdcall MMXConvert1555to8888( void *Src, void *Dst, DWORD NumberOfPixels );
inline void MMXConvert4444to8888( void *Src, void *Dst, DWORD NumberOfPixels );
inline void MMXConvert565to8888( void *Src, void *Dst, DWORD NumberOfPixels );
inline void MMXConvert1555to8888( void *Src, void *Dst, DWORD NumberOfPixels );

#endif