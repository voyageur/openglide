//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*                      Main Header
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************

#ifndef __GLOGL_H__
#define __GLOGL_H__

//#define OGL_ALL
//#define OGL_PARTDONE
//#define OGL_NOTDONE
//#define OGL_DEBUG
//#define OGL_CRITICAL
//#define OPENGL_DEBUG
//#define OGL_PROFILING
//#define OGL_UTEX
//#define OGL_COMBINE

#ifdef OGL_ALL
 #define OGL_DONE
 #define OGL_PARTDONE
 #define OGL_NOTDONE
 #define OGL_DEBUG
 #define OGL_CRITICAL
 #define OPENGL_DEBUG
 #define OGL_PROFILING
 #define OGL_UTEX
 #define OGL_COMBINE
#endif

#include "platform.h"
#include "platform/window.h"

#include <stdio.h>

#include "sdk2_glide.h"

#ifdef _MSC_VER
#define RDTSC(v)    __asm _emit 0x0f                \
                    __asm _emit 0x31                \
                    __asm mov dword ptr v, eax      \
                    __asm mov dword ptr v+4, edx
#endif

#ifdef __GNUC__
#define RDTSC(v)    asm volatile ("rdtsc;" : "=A" (v) )
#endif

#define ERRORFILE               "OpenGLid.err"
#define GLIDEFILE               "OpenGLid.log"
#define INIFILE                 "OpenGLid.ini"

#define OGL_LOG_SEPARATE        "--------------------------------------------------------\n"

#define OGL_MIN_FRAME_BUFFER    2
#define OGL_MAX_FRAME_BUFFER    16
#define OGL_MIN_TEXTURE_BUFFER  2
#define OGL_MAX_TEXTURE_BUFFER  32

#define OGL_VER_1_1             101

#define OPENGLFOGTABLESIZE      64 * 1024

#define D1OVER255               0.003921568627451f      // 1 / 255
#define D1OVER65536             0.0000152587890625f     // 1 / 65536
#define D1OVER65535             0.000015259021896696421759365224689097f     // 1 / 65535
#define D1OVER256               0.00390625f             // 1 / 256
#define D2OVER256               0.0078125f              // 2 / 256
#define D4OVER256               0.015625f               // 4 / 256
#define D8OVER256               0.03125f                // 8 / 256

#define WBUFFERNEAR             -1.0f
#define WBUFFERFAR               0.0f
#define ZBUFFERNEAR              0.0f
#define ZBUFFERFAR              -1.0f

// Class declarations

typedef void (*GLIDEERRORFUNCTION)( const char *string, FxBool fatal );

struct BufferStruct
{
    bool                    Lock;
    GrLock_t                Type;
    GrLfbWriteMode_t        WriteMode;
    GrBuffer_t              Buffer;
    FxBool                  PixelPipeline;
    FxU16                   *Address;
};

struct TexSourceStruct
{
    FxU32       StartAddress;
    FxU32       EvenOdd;
    GrTexInfo   Info;
};

/*
 * Anonymous structs are not Ansi and
 * only support by Visual C++.  They must
 * be removed.  Luckily we don't use the
 * individual bytes which would be prown
 * to endian and compiler ordering problems!
union OGLByteColor
{
    struct
    {
        BYTE    B;
        BYTE    G;
        BYTE    R;
        BYTE    A;
    };
    DWORD   C;
};
*/
typedef FxU32 OGLByteColor;

struct GlideState
{
    GrBuffer_t              RenderBuffer;
    GrDepthBufferMode_t     DepthBufferMode;
    GrCmpFnc_t              DepthFunction;
    FxBool                  DepthBufferWritting;
    FxI16                   DepthBiasLevel;
    GrDitherMode_t          DitherMode;
    GrColor_t               ChromakeyValue;
    GrChromakeyMode_t       ChromaKeyMode;
    GrAlpha_t               AlphaReferenceValue;
    GrCmpFnc_t              AlphaTestFunction;
    FxBool                  AlphaMask;
    FxBool                  ColorMask;
    GrColor_t               ConstantColorValue;
    GrColor_t               FogColorValue;
    GrFogMode_t             FogMode;
    GrCullMode_t            CullMode;
    GrTextureClampMode_t    SClampMode;
    GrTextureClampMode_t    TClampMode;
    GrTextureFilterMode_t   MinFilterMode;
    GrTextureFilterMode_t   MagFilterMode;
    GrMipMapMode_t          MipMapMode;
    FxBool                  LodBlend;
    GrCombineFunction_t     ColorCombineFunction;
    GrCombineFactor_t       ColorCombineFactor;
    GrCombineLocal_t        ColorCombineLocal;
    GrCombineOther_t        ColorCombineOther;
    FxBool                  ColorCombineInvert;
    GrCombineFunction_t     AlphaFunction;
    GrCombineFactor_t       AlphaFactor;
    GrCombineLocal_t        AlphaLocal;
    GrCombineOther_t        AlphaOther;
    FxBool                  AlphaInvert;
    GrCombineFunction_t     TextureCombineCFunction;
    GrCombineFactor_t       TextureCombineCFactor;
    GrCombineFunction_t     TextureCombineAFunction;
    GrCombineFactor_t       TextureCombineAFactor;
    FxBool                  TextureCombineRGBInvert;
    FxBool                  TextureCombineAInvert;
    GrOriginLocation_t      OriginInformation;
    TexSourceStruct         TexSource;
    GrAlphaBlendFnc_t       AlphaBlendRgbSf;
    GrAlphaBlendFnc_t       AlphaBlendRgbDf;
    GrAlphaBlendFnc_t       AlphaBlendAlphaSf;
    GrAlphaBlendFnc_t       AlphaBlendAlphaDf;
    FxU32                   ClipMinX;
    FxU32                   ClipMaxX;
    FxU32                   ClipMinY;
    FxU32                   ClipMaxY;
    GrColorFormat_t         ColorFormat;
    FxU32                   STWHint;
    FxBool                  VRetrace;
};

struct GlideStruct
{
    int                     ActiveVoodoo;
    // Frame Buffer Stuff
    FxU32                   WindowWidth;
    FxU32                   WindowHeight;
    FxU32                   WindowTotalPixels; 
    int                     NumBuffers;
    int                     AuxBuffers;
    // States and Constants
    FxU8                    FogTable[ GR_FOG_TABLE_SIZE + 1 ];
    FxU32                   TexMemoryMaxPosition;
    bool                    CLocal;
    bool                    COther;
    bool                    ALocal;
    bool                    AOther;
    GlideState              State;
    BufferStruct            SrcBuffer;
    BufferStruct            DstBuffer;
    GLuint                  LFBTexture;
    FxU32                   LFBTextureSize;
    int                     TextureMemory;
};

struct OpenGLStruct
{
    bool                    GlideInit;
    bool                    WinOpen;
    GLsizei                 WindowWidth;
    GLsizei                 WindowHeight;
    FxU32                   WindowTotalPixels;
    GLfloat                 Gamma;
    GLfloat                 AlphaReferenceValue;
    GLenum                  AlphaTestFunction;
    GLboolean               DepthBufferWritting;
    GLfloat                 DepthBiasLevel;
    GLenum                  DepthFunction;
    GLenum                  RenderBuffer;
    GLenum                  SClampMode;
    GLenum                  TClampMode;
    GLenum                  MinFilterMode;
    GLenum                  MagFilterMode;
    GLenum                  TextureMode;
    GLenum                  SrcBlend;
    GLenum                  DstBlend;
    GLenum                  SrcAlphaBlend;
    GLenum                  DstAlphaBlend;
    FxU32                   ClipMinX;
    FxU32                   ClipMaxX;
    FxU32                   ClipMinY;
    FxU32                   ClipMaxY;
    GLuint                  Refresh;
    GLboolean               ColorMask;
    GLfloat                 ConstantColor[ 4 ];
    GLfloat                 AlphaColor[ 4 ];
    GLfloat                 ZNear;
    GLfloat                 ZFar;
    GLfloat                 FogColor[ 4 ];
    FxU8                    FogTable[ OPENGLFOGTABLESIZE ];
    OGLByteColor            ChromaColor;
    bool                    Fog;
    bool                    Texture;
    bool                    ColorTexture;
    bool                    AlphaTexture;
    bool                    Blend;
    bool                    AlphaBuffer;
    bool                    ChromaKey;
    bool                    Clipping;
    int                     MultiTextureTMUs;
    int                     DepthBufferType;
    int                     WaitSignal;
    FxU32                   *tmpBuf;
};

struct ConfigStruct
{
    int     OGLVersion;
    int     Priority;
    int     TextureMemorySize;
    int     FrameBufferMemorySize;

    float   Resolution;

    bool    FogEnable;
    bool    InitFullScreen;
    bool    PrecisionFix;
    bool    EnableMipMaps;
    bool    BuildMipMaps;
    bool    IgnorePaletteChange;
    bool    Wrap565to5551;
    bool    TextureEnv;
    bool    MMXEnable;
    bool    CreateWindow;

    bool    EXT_secondary_color;
    bool    ARB_multitexture;
    bool    EXT_fog_coord;
    bool    EXT_texture_env_add;
    bool    EXT_texture_env_combine;
    bool    EXT_texture_lod_bias;
    bool    SGIS_generate_mipmap;
    bool    EXT_paletted_texture;
    bool    EXT_vertex_array;
    bool    EXT_blend_func_separate;

    bool    NoSplash;
    bool    ShamelessPlug;
};

// Extern variables
extern const char *         OpenGLideVersion;
extern double               ClockFreq;
extern GlideStruct          Glide;                  // Glide Internal
extern OpenGLStruct         OpenGL;                 // OpenGL equivalents
extern ConfigStruct         UserConfig;
extern ConfigStruct         InternalConfig;
extern GLIDEERRORFUNCTION   ExternErrorFunction;

#ifdef OGL_DEBUG
    // Profiling variables
    extern FxI64            InitialTick;
    extern FxI64            FinalTick;
    extern FxU32            Frame;
    extern double           Fps;
    extern double           FpsAux;
#endif

// Genral Prototypes
VARARGDECL(void) GlideMsg( const char *szString, ... );
VARARGDECL(void) Error( const char *szString, ... );
void GLErro( const char *Funcao );
void ConvertColor4B( GrColor_t GlideColor, FxU32 &C );
void ConvertColorB( GrColor_t GlideColor, FxU8 &R, FxU8 &G, FxU8 &B, FxU8 &A );
void ConvertColorF( GrColor_t GlideColor, float &R, float &G, float &B, float &A );
GrColor_t ConvertConstantColor( float R, float G, float B, float A );
bool GenerateErrorFile( void );
bool ClearAndGenerateLogFile( void );
void CloseLogFile( void );
bool InitWindow( FxU hWnd );
void InitOpenGL( void );
void GetOptions( void );
void InitMainVariables( void );

void MMXCopyMemory( void *Dst, void *Src, FxU32 NumberOfBytes );

int DetectMMX();

#endif
