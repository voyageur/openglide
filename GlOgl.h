//**************************************************************
//*				Glide->OpenGL Wrapper
//*					Main Header
//*				Made by Glorfindel
//**************************************************************

#ifndef __GLOGL_H__
#define __GLOGL_H__

#define __MSC__
#define __WIN32__
#define FX_GLIDE_NO_FUNC_PROTO

//#define ALL
//#define PARTDONE
//#define NOTDONE
//#define DEBUG
//#define CRITICAL
#define OPENGL_DEBUG
//#define PROFILING

#ifdef ALL
 #define DONE
 #define PARTDONE
 #define NOTDONE
 #define DEBUG
 #define CRITICAL
 #define OPENGL_DEBUG
 #define PROFILING
#endif

#include <windows.h>
#include "Glide.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GLRender.h"
#include "GLTexture.h"

#define RDTSC(v)	__asm _emit 0x0f \
					__asm _emit 0x31 \
					__asm mov dword ptr v, eax \
					__asm mov dword ptr v+4, edx

#define	ERRORFILE				"OpenGLid.ERR"
#define GLIDEFILE				"OpenGLid.LOG"
#define INIFILE					"OpenGLid.INI"

#define OPENGLBUFFERMEMORY		OpenGL.WindowWidth * OpenGL.WindowHeight

#define OPENGLFOGTABLESIZE		2048

#define D1OVER255				0.003921568627451f		// 1 / 255
#define D1OVER65536				0.0000152587890625f		// 1 / 65536
#define D1OVER256				0.00390625f				// 1 / 256
#define D2OVER256				0.0078125f				// 2 / 256
#define D4OVER256				0.015625f				// 4 / 256
#define D8OVER256				0.03125f				// 8 / 256

#define WBUFFERNEAR				-1.0f
#define WBUFFERFAR				0.0f
#define ZBUFFERNEAR				0.0f
#define ZBUFFERFAR				-1.0f

// Class declarations

struct BufferStruct
{
	bool					Lock;
	GrLock_t				Type;
	GrLfbWriteMode_t		WriteMode;
	GrBuffer_t				Buffer;
	DWORD					*Address;
};

struct GlideState
{
	GrBuffer_t				RenderBuffer;
	GrDepthBufferMode_t		DepthBufferMode;
	GrCmpFnc_t				DepthFunction;
	FxBool					DepthBufferWritting;
	FxI16					DepthBiasLevel;
	GrDitherMode_t			DitherMode;
	GrColor_t				ChromakeyValue;
	GrChromakeyMode_t		ChromaKeyMode;
	GrAlpha_t				AlphaReferenceValue;
	GrCmpFnc_t				AlphaTestFunction;
	FxBool					AlphaMask;
	FxBool					ColorMask;
	GrColor_t				ConstantColorValue;
	GrColor_t				FogColorValue;
	GrFogMode_t				FogMode;
	GrCullMode_t			CullMode;
	GrTextureClampMode_t	SClampMode;
	GrTextureClampMode_t	TClampMode;
	GrTextureFilterMode_t	MinFilterMode;
	GrTextureFilterMode_t	MagFilterMode;
	GrMipMapMode_t			MipMapMode;
	FxBool					LodBlend;
	GrCombineFunction_t		ColorCombineFunction;
	GrCombineFactor_t		ColorCombineFactor;
    GrCombineLocal_t		ColorCombineLocal;
	GrCombineOther_t		ColorCombineOther;
    FxBool					ColorCombineInvert;
	GrCombineFunction_t		AlphaFunction;
	GrCombineFactor_t		AlphaFactor;
	GrCombineLocal_t		AlphaLocal;
	GrCombineOther_t		AlphaOther;
	FxBool					AlphaInvert;
	GrCombineFunction_t		TextureCombineCFunction;
	GrCombineFactor_t		TextureCombineCFactor;
	GrCombineFunction_t		TextureCombineAFunction;
	GrCombineFactor_t		TextureCombineAFactor;
	FxBool					TextureCombineRGBInvert;
	FxBool					TextureCombineAInvert;
	GrOriginLocation_t		OriginInformation;
	TexSourceStruct			TexSource;
	GrAlphaBlendFnc_t		AlphaBlendRgbSf;
	GrAlphaBlendFnc_t		AlphaBlendRgbDf;
	GrAlphaBlendFnc_t		AlphaBlendAlphaSf;
	GrAlphaBlendFnc_t		AlphaBlendAlphaDf;
	FxU32					ClipMinX;
	FxU32					ClipMaxX;
	FxU32					ClipMinY;
	FxU32					ClipMaxY;
	GrColorFormat_t			ColorFormat;
};

struct GlideStruct
{
	int						ActiveVoodoo;
	// Frame Buffer Stuff
	int						WindowWidth;
	int						WindowHeight;
	GrScreenResolution_t	Resolution;
	GrScreenRefresh_t		Refresh;
	int						NumBuffers,
							AuxBuffers;
	// States and Constants
	FxU8					FogTable[GR_FOG_TABLE_SIZE+1];
//	GuTexPalette			ColorPalette;
	FxU32					TexMemoryMaxPosition;
	bool					CLocal, COther;
	bool					ALocal, AOther;
	GlideState				State;
	BufferStruct			SrcBuffer;
//	BufferStruct			DstBuffer;
	DWORD					TextureMemory;
};

struct OpenGLStruct
{
	bool					GlideInit;
	bool					WinOpen;
	long					WindowWidth, WindowHeight;
	GLfloat					Gamma;
	GLfloat					AlphaReferenceValue;
	GLenum					AlphaTestFunction;
	GLboolean				DepthBufferWritting;
	GLfloat					DepthBiasLevel;
	GLenum					DepthFunction;
	GLenum					RenderBuffer;
	GLenum					SClampMode;
	GLenum					TClampMode;
	GLenum					MinFilterMode;
	GLenum					MagFilterMode;
	GLenum					TextureMode;
	TextureStruct			*CurrentTexture;
	GLenum					SrcBlend;
	GLenum					DstBlend;
	GLenum					SrcAlphaBlend;
	GLenum					DstAlphaBlend;
	GLuint					Refresh;
	GLboolean				ColorMask;
	GLfloat					ConstantColor[4];
	GLfloat					ZNear, ZFar;
	GLfloat					FogColor[4];
	BYTE					FogTable[OPENGLFOGTABLESIZE];
	BYTE					ChromaColor[4];
	bool					Fog;
	bool					Texture;
	bool					Blend;
	bool					AlphaBuffer;
	bool					ChromaKey;
	int						MultiTextureTMUs;
	int						DepthBufferType;
	DWORD					PaletteCalc;
	BYTE					PTable[256][4];
	DWORD					WaitSignal;
};

struct ConfigStruct
{
	int		OGLVersion;
	int		FogEnable;
	int		InitFullScreen;
	int		PrecisionFixEnable;
	int		Wrap565Enable;
	int		Priority;
	int		BuildMipMaps;

	int		MultiTextureEXTEnable;
	int		PaletteEXTEnable;
	int		PackedPixelsEXTEnable;
	int		TextureEnvEXTEnable;
	int		VertexArrayEXTEnable;
	int		SecondaryColorEXTEnable;
	int		FogCoordEXTEnable;

	int		MMXEnable;
	int		TDnowEnable;
	int		CreateWindow;

	WORD	PalettePrecision;
	DWORD	TextureMemorySize;
	DWORD	FrameBufferMemorySize;
};

// Extern variables
extern GlideStruct		Glide;					// Glide Internal
extern OpenGLStruct		OpenGL;					// OpenGL equivalents
extern ConfigStruct		UserConfig,
						InternalConfig;

//extern WTexture			*Textures;
//extern WUTexture		UTextures;
//extern Render			OGLRender;

// Genral Prototypes
void __cdecl GlideMsg(char *szString, ...);
void __cdecl Error(char *szString, ...);
void GLErro( char *Funcao );

int DetectMMX();



// Prototypes of Glide functions needed
#define DLLEXPORT		extern "C" __declspec(dllexport)

DLLEXPORT void __stdcall	grFogMode( GrFogMode_t mode );
DLLEXPORT void __stdcall	grRenderBuffer(GrBuffer_t dwBuffer);
DLLEXPORT void __stdcall	grDepthMask( FxBool enable );
DLLEXPORT void __stdcall	grDepthBufferMode( GrDepthBufferMode_t mode );
DLLEXPORT void __stdcall	grDitherMode( GrDitherMode_t mode );
DLLEXPORT void __stdcall	grChromakeyMode( GrChromakeyMode_t mode );
DLLEXPORT void __stdcall	grDepthBufferFunction( GrCmpFnc_t func );
DLLEXPORT void __stdcall	grCullMode( GrCullMode_t mode );
DLLEXPORT void __stdcall	grAlphaTestFunction( GrCmpFnc_t function );
DLLEXPORT void __stdcall	grColorMask( FxBool rgb, FxBool a );
DLLEXPORT void __stdcall	grGlideShutdown( void );
DLLEXPORT void __stdcall	grSstOrigin(GrOriginLocation_t  origin);
DLLEXPORT FxU32 __stdcall	grTexTextureMemRequired(DWORD dwEvenOdd, GrTexInfo *texInfo);
DLLEXPORT void __stdcall	grTexDownloadMipMapLevel( GrChipID_t tmu, FxU32 startAddress, GrLOD_t thisLod,
											GrLOD_t largeLod, GrAspectRatio_t aspectRatio,
											GrTextureFormat_t format, FxU32 evenOdd, void *data );
DLLEXPORT void __stdcall	grTexClampMode(	GrChipID_t tmu,
										    GrTextureClampMode_t s_clampmode,
										    GrTextureClampMode_t t_clampmode );
DLLEXPORT void __stdcall	grTexFilterMode( GrChipID_t tmu,
						                  GrTextureFilterMode_t minfilter_mode,
										  GrTextureFilterMode_t magfilter_mode );
DLLEXPORT void __stdcall	grTexMipMapMode( GrChipID_t     tmu, 
										  GrMipMapMode_t mode,
										  FxBool         lodBlend );
DLLEXPORT FxU32 __stdcall	grTexMinAddress( GrChipID_t tmu );
DLLEXPORT void __stdcall	grTexCombine(GrChipID_t tmu,
										 GrCombineFunction_t rgb_function,
										 GrCombineFactor_t rgb_factor, 
										 GrCombineFunction_t alpha_function,
										 GrCombineFactor_t alpha_factor,
										 FxBool rgb_invert,
										 FxBool alpha_invert );
DLLEXPORT void __stdcall	grSstWinClose( void );
DLLEXPORT void __stdcall	grConstantColorValue( GrColor_t value );
DLLEXPORT void __stdcall	grDepthBiasLevel( FxI16 level );
DLLEXPORT void __stdcall	grFogColorValue( GrColor_t fogcolor );
DLLEXPORT void __stdcall	grChromakeyValue( GrColor_t value );
DLLEXPORT void __stdcall	grAlphaTestReferenceValue( GrAlpha_t value );
DLLEXPORT void __stdcall	grBufferClear( GrColor_t color, GrAlpha_t alpha, FxU16 depth );
DLLEXPORT void __stdcall	grGammaCorrectionValue( float value );
DLLEXPORT void __stdcall	grTexSource( GrChipID_t tmu, FxU32 startAddress, FxU32 evenOdd,
								GrTexInfo  *info );
DLLEXPORT void __stdcall	grAlphaBlendFunction(GrAlphaBlendFnc_t rgb_sf,   GrAlphaBlendFnc_t rgb_df,
											GrAlphaBlendFnc_t alpha_sf, GrAlphaBlendFnc_t alpha_df );
DLLEXPORT void __stdcall	grAlphaCombine(GrCombineFunction_t function, GrCombineFactor_t factor,
										GrCombineLocal_t local, GrCombineOther_t other,
										FxBool invert );
DLLEXPORT void __stdcall	grAlphaControlsITRGBLighting( FxBool enable );
DLLEXPORT void __stdcall	grColorCombine(GrCombineFunction_t function, GrCombineFactor_t factor,
										GrCombineLocal_t local, GrCombineOther_t other,
										FxBool invert );
DLLEXPORT float __stdcall	guFogTableIndexToW( int i );
DLLEXPORT FxU32 __stdcall	grTexCalcMemRequired(GrLOD_t lodmin, GrLOD_t lodmax, GrAspectRatio_t aspect, GrTextureFormat_t fmt);
DLLEXPORT void __stdcall	grSplash( float x, float y, float width, float height, FxU32 frame );
DLLEXPORT void __stdcall	grClipWindow( FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy );

#endif