//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*					Utility File
//*				Made by Glorfindel
//**************************************************************

#include <stdio.h>
#include <io.h>
#include "GlOgl.h"
#include "GLExtensions.h"

// Configuration Variables
ConfigStruct	UserConfig,
				InternalConfig;

// Extern prototypes
extern unsigned long NumberOfErrors;
extern BOOL GenerateErrorFile();


// Functions

void __cdecl GlideMsg(char *szString, ...)
{
	va_list(Arg);
	va_start(Arg, szString);

	FILE *fHandle = fopen( GLIDEFILE, "at");
	if(!fHandle) return;
	vfprintf(fHandle, szString, Arg);
	fflush( fHandle );
	fclose(fHandle);

	va_end(Arg);
}

void __cdecl Error(char *szString, ...)
{
	va_list(Arg);
	va_start(Arg, szString);

	if ( NumberOfErrors == 0 )
	{
		GenerateErrorFile();
	}

	FILE *fHandle = fopen( ERRORFILE, "at");
	if(!fHandle) return;
	vfprintf(fHandle, szString, Arg);
	fflush( fHandle );
	fclose(fHandle);

	va_end(Arg);
	NumberOfErrors++;
}

void GLErro( char *Funcao )
{
	GLenum Erro = glGetError();

	if (Erro != GL_NO_ERROR)
	{
		Error( "%s: OpenGLError = %s\n", Funcao, gluErrorString( Erro ) );//ErrorCode );
	}
}

#if 0
static LPDIRECTDRAW              pDD = NULL;
static LPDIRECTDRAWSURFACE      pSurf = NULL;
static LPDIRECTDRAWGAMMACONTROL pControl = NULL;

static HRESULT open_direct_draw(HWND hwnd)
{
    HRESULT res;
    DDSURFACEDESC desc;

    res = DirectDrawCreate(NULL, &pDD, NULL);
    if(res != DD_OK)
        return res;

    res = pDD->SetCooperativeLevel(NULL, DDSCL_NORMAL);
    //res = pDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);
    if(res != DD_OK)
        return res;

    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DDSD_CAPS;
    desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    res = pDD->CreateSurface(&desc, &pSurf, NULL);
    if(res != DD_OK)
        return res;

    res = pSurf->QueryInterface(IID_IDirectDrawGammaControl, (void **) &pControl);

    return res;
}

static void close_direct_draw()
{
    if(pControl != NULL)
    {
        pControl->Release();
        pControl = NULL;
    }

    if(pSurf != NULL)
    {
        pSurf->Release();
        pSurf = NULL;
    }

    if(pDD != NULL)
    {
        pDD->Release();
        pDD = NULL;
    }
}

static HRESULT set_gamma_ramp()
{
    HRESULT res;
    DDGAMMARAMP ramp;
    int i;
    
    if(pControl == NULL)
        return DDERR_NOTINITIALIZED;
    
    for(i = 0; i < 256; i++)
    {
        WORD v = (WORD) (0xffff * pow(i / 255.0, 0.66));
        
        ramp.red[i] = ramp.green[i] = ramp.blue[i] = (v & 0xff00);
    }
    
    res = pControl->SetGammaRamp(0, &ramp);

    return res;
}
#endif

HDC hDC;
static HGLRC hRC;
static HWND hWND;
static struct
        {
            WORD red[256];
            WORD green[256];
            WORD blue[256];
        } old_ramp;

static BOOL ramp_stored = FALSE;
static bool mode_changed = false;

static bool SetScreenMode(HWND hWnd, int xsize, int ysize)
{
    HDC hdc;
    DWORD bits_per_pixel;
    bool found;
    DEVMODE DevMode;

    hdc = ::GetDC(hWnd);
    bits_per_pixel = GetDeviceCaps(hdc, BITSPIXEL);
    ::ReleaseDC(hWnd, hdc);
    
    found = false;
    DevMode.dmSize = sizeof(DEVMODE);
    
    for(int i = 0; !found && EnumDisplaySettings(NULL, i, &DevMode) != FALSE; i++ )
    {
        if((DevMode.dmPelsWidth == (DWORD)xsize) && (DevMode.dmPelsHeight == (DWORD)ysize) && 
            (DevMode.dmBitsPerPel == bits_per_pixel))
            found = true;
    }
    
    return (found && ChangeDisplaySettings( &DevMode, CDS_RESET|CDS_FULLSCREEN ) == DISP_CHANGE_SUCCESSFUL);
}

static void ResetScreenMode()
{
    ChangeDisplaySettings(NULL, 0);
}


void InitialiseOpenGLWindow( HWND hwnd, int x, int y, UINT width, UINT height )
{
    PIXELFORMATDESCRIPTOR	pfd;
	int						PixFormat;
	unsigned int			BitsPerPixel;

    mode_changed = (UserConfig.InitFullScreen ? SetScreenMode(hwnd, width, height)
                                                  : false);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "NULL window specified", "Error", MB_OK);
        exit(1);
    }

    hWND = hwnd;

	hDC = GetDC( hwnd );
    	BitsPerPixel = GetDeviceCaps( hDC, BITSPIXEL );

	ZeroMemory( &pfd, sizeof(pfd) );
	pfd.nSize        = sizeof(pfd);
	pfd.nVersion     = 1;
	pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType   = PFD_TYPE_RGBA;
	pfd.cColorBits   = BitsPerPixel;
	pfd.cDepthBits   = BitsPerPixel;


	if (!(PixFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
		exit(1);
	} 

	if (!SetPixelFormat(hDC, PixFormat, &pfd))
	{
		MessageBox(NULL, "SetPixelFormat() failed:  "
			   "Cannot set format specified.", "Error", MB_OK);
		exit(1);
	} 

	DescribePixelFormat( hDC, PixFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd );
	GlideMsg( "ColorBits	= %d\n", pfd.cColorBits );
	GlideMsg( "DepthBits	= %d\n", pfd.cDepthBits );

	if ( pfd.cDepthBits > 16 )
        UserConfig.PrecisionFixEnable = false;

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

    {
        HDC pDC = GetDC(NULL);

        ramp_stored = GetDeviceGammaRamp(pDC, &old_ramp);

        ReleaseDC(NULL, pDC);
    }
}    

void FinaliseOpenGLWindow(void)
{
    if(ramp_stored)
    {
        HDC pDC = GetDC(NULL);

        BOOL res = SetDeviceGammaRamp(pDC, &old_ramp);

        ReleaseDC(NULL, pDC);
    }

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWND, hDC);

    if(mode_changed)
        ResetScreenMode();
}

void ConvertColorB( GrColor_t GlideColor, BYTE &R, BYTE &G, BYTE &B, BYTE &A )
{
	switch (Glide.State.ColorFormat)
	{
	case GR_COLORFORMAT_ARGB:	//0xAARRGGBB
		A = (BYTE)((GlideColor & 0xFF000000) >> 24);
		R = (BYTE)((GlideColor & 0x00FF0000) >> 16);
		G = (BYTE)((GlideColor & 0x0000FF00) >>  8);
		B = (BYTE)((GlideColor & 0x000000FF)      );
		break;
	case GR_COLORFORMAT_ABGR:	//0xAABBGGRR
		A = (BYTE)((GlideColor & 0xFF000000) >> 24);
		B = (BYTE)((GlideColor & 0x00FF0000) >> 16);
		G = (BYTE)((GlideColor & 0x0000FF00) >>  8);
		R = (BYTE)((GlideColor & 0x000000FF)      );
		break;
	case GR_COLORFORMAT_RGBA: 	//0xRRGGBBAA
		R = (BYTE)((GlideColor & 0xFF000000) >> 24);
		G = (BYTE)((GlideColor & 0x00FF0000) >> 16);
		B = (BYTE)((GlideColor & 0x0000FF00) >>  8);
		A = (BYTE)((GlideColor & 0x000000FF)      );
		break;
	case GR_COLORFORMAT_BGRA:	//0xBBGGRRAA
		B = (BYTE)((GlideColor & 0xFF000000) >> 24);
		G = (BYTE)((GlideColor & 0x00FF0000) >> 16);
		R = (BYTE)((GlideColor & 0x0000FF00) >>  8);
		A = (BYTE)((GlideColor & 0x000000FF)      );
		break;
	}
}

GrColor_t ConvertConstantColor( float R, float G, float B, float A )
{
	GrColor_t r = (GrColor_t) R;
	GrColor_t g = (GrColor_t) G;
	GrColor_t b = (GrColor_t) B;
	GrColor_t a = (GrColor_t) A;

	switch (Glide.State.ColorFormat)
	{
	case GR_COLORFORMAT_ARGB:	//0xAARRGGBB
		return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
	case GR_COLORFORMAT_ABGR:	//0xAABBGGRR
		return ( a << 24 ) | ( b << 16 ) | ( g << 8 ) | r;
	case GR_COLORFORMAT_RGBA: 	//0xRRGGBBAA
		return ( r << 24 ) | ( g << 16 ) | ( b << 8 ) | a;
	case GR_COLORFORMAT_BGRA:	//0xBBGGRRAA
		return ( b << 24 ) | ( g << 16 ) | ( r << 8 ) | a;
	}

	return 0;
}

void ConvertColorF( GrColor_t GlideColor, float &R, float &G, float &B, float &A )
{
	switch (Glide.State.ColorFormat)
	{
	case GR_COLORFORMAT_ARGB:	//0xAARRGGBB
		A = (float)((GlideColor & 0xFF000000) >> 24) * D1OVER255;
		R = (float)((GlideColor & 0x00FF0000) >> 16) * D1OVER255;
		G = (float)((GlideColor & 0x0000FF00) >>  8) * D1OVER255;
		B = (float)((GlideColor & 0x000000FF)      ) * D1OVER255;
		break;
	case GR_COLORFORMAT_ABGR:	//0xAABBGGRR
		A = (float)((GlideColor & 0xFF000000) >> 24) * D1OVER255;
		B = (float)((GlideColor & 0x00FF0000) >> 16) * D1OVER255;
		G = (float)((GlideColor & 0x0000FF00) >>  8) * D1OVER255;
		R = (float)((GlideColor & 0x000000FF)      ) * D1OVER255;
		break;
	case GR_COLORFORMAT_RGBA: 	//0xRRGGBBAA
		R = (float)((GlideColor & 0xFF000000) >> 24) * D1OVER255;
		G = (float)((GlideColor & 0x00FF0000) >> 16) * D1OVER255;
		B = (float)((GlideColor & 0x0000FF00) >>  8) * D1OVER255;
		A = (float)((GlideColor & 0x000000FF)      ) * D1OVER255;
		break;
	case GR_COLORFORMAT_BGRA:	//0xBBGGRRAA
		B = (float)((GlideColor & 0xFF000000) >> 24) * D1OVER255;
		G = (float)((GlideColor & 0x00FF0000) >> 16) * D1OVER255;
		R = (float)((GlideColor & 0x0000FF00) >>  8) * D1OVER255;
		A = (float)((GlideColor & 0x000000FF)      ) * D1OVER255;
		break;
	}
}


//----------------------------------------------------------------
DWORD GetTexSize(const int Lod, const int aspectRatio, const int format )
{
	static DWORD nLength, nBytes;

	switch(Lod)
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

	switch(aspectRatio)
	{
	case GR_ASPECT_1x1:		nBytes = nLength * nLength;				break;
	case GR_ASPECT_1x2:
	case GR_ASPECT_2x1:		nBytes = (nLength >> 1) * nLength;		break;
	case GR_ASPECT_1x4:
	case GR_ASPECT_4x1:		nBytes = (nLength >> 2) * nLength;		break;
	case GR_ASPECT_1x8:
	case GR_ASPECT_8x1:		nBytes = (nLength >> 3) * nLength;		break;
	}

	switch(format)
	{
	case GR_TEXFMT_RGB_565:
	case GR_TEXFMT_ARGB_8332:
	case GR_TEXFMT_AYIQ_8422:
	case GR_TEXFMT_ARGB_1555:
	case GR_TEXFMT_ARGB_4444:
	case GR_TEXFMT_ALPHA_INTENSITY_88:
	case GR_TEXFMT_AP_88:
		nBytes <<= 1;
		break;
	}

	return nBytes;
}

// Calculates the frequency of the processor clock
#pragma optimize( "", off )
float ClockFrequency()
{
	__int64	i64_perf_start, i64_perf_freq, i64_perf_end;
	__int64	i64_clock_start,i64_clock_end;
	double d_loop_period, d_clock_freq;

	QueryPerformanceFrequency( (LARGE_INTEGER*)&i64_perf_freq );

	QueryPerformanceCounter( (LARGE_INTEGER*)&i64_perf_start );
	i64_perf_end = 0;

	RDTSC( i64_clock_start );
	while( i64_perf_end < i64_perf_start + 250000 )
	{
		QueryPerformanceCounter( (LARGE_INTEGER*)&i64_perf_end );
	}
	RDTSC( i64_clock_end );

	i64_clock_end -= i64_clock_start;

	d_loop_period = ((double)(i64_perf_freq)) / 250000.0;
	d_clock_freq = ((double)(i64_clock_end & 0xffffffff)) * d_loop_period;

	return (float)d_clock_freq;
}
#pragma optimize( "", on )



char *FindConfig( char *IniFile, char *IniConfig )
{
	char Buffer1[256], *EqLocation, *Find;
	FILE *file;

	Find = NULL;
	file = fopen( IniFile, "r" );

	while ( fgets( Buffer1, 255, file ) != NULL )
	{
		if ( (EqLocation = strchr( Buffer1, '=' )) != NULL )
		{		
			if ( !strncmp( Buffer1, IniConfig, EqLocation - Buffer1 ) )
			{
				Find = EqLocation + 1;
				if ( Find[ strlen( Find )-1 ] == 10 )
				{
					Find[ strlen( Find )-1 ] = '\0';
				}
				break;
			}
		}
	}

 	fclose( file );

	return Find;
}

void GetOptions()
{
	FILE *IniFile;
	char *Pointer;
	extern char *OpenGLideVersion;
	char Path[255];

	UserConfig.FogEnable				= true;
	UserConfig.InitFullScreen			= false;
	UserConfig.PrecisionFixEnable		= true;
	UserConfig.CreateWindow				= false;
	UserConfig.BuildMipMaps				= false;

	UserConfig.Wrap565Enable			= false;

	UserConfig.MultiTextureEXTEnable	= false;
	UserConfig.PaletteEXTEnable			= false;
	UserConfig.PackedPixelsEXTEnable	= false;
	UserConfig.TextureEnvEXTEnable		= false;
	UserConfig.VertexArrayEXTEnable		= false;
	UserConfig.SecondaryColorEXTEnable	= false;
	UserConfig.FogCoordEXTEnable		= true;

	UserConfig.PalettePrecision			= 32;
	UserConfig.TextureMemorySize		= 16;
	UserConfig.FrameBufferMemorySize	= 8;

	UserConfig.Priority					= 2;
	UserConfig.MMXEnable				= false;
	UserConfig.TDnowEnable				= false;

    /*
	if ( GetWindowsDirectory( Path, MAX_PATH ) == 0 )
	{
		Error( "Could not get the Windows Directory\nExiting...\n" );
		exit( -1 );
	}

	strcat( Path, "\\" );
	strcat( Path, INIFILE );
    */

    strcpy( Path, INIFILE );

	GlideMsg("Configuration file is %s\n", Path );
	

	if ( access( Path, 00) == -1 )
	{
		IniFile = fopen( Path, "w" );
		fprintf( IniFile, "Configuration File for OpenGLide\n\n" );
		fprintf( IniFile, "Version=%s\n\n", OpenGLideVersion );
		fprintf( IniFile, "[Options]\n" );
		fprintf( IniFile, "EnableMMX=%d\n", UserConfig.MMXEnable );
//		fprintf( IniFile, "Enable3DNow=%d\n", UserConfig.TDnowEnable );
		fprintf( IniFile, "CreateWindow=%d\n", UserConfig.CreateWindow );
		fprintf( IniFile, "InitFullScreen=%d\n", UserConfig.InitFullScreen );
		fprintf( IniFile, "EnableMipMaps=%d\n", UserConfig.BuildMipMaps );
		fprintf( IniFile, "EnableFog=%d\n", UserConfig.FogEnable );
		fprintf( IniFile, "EnablePrecisionFix=%d\n", UserConfig.PrecisionFixEnable );
		fprintf( IniFile, "EnableWrap565=%d\n", UserConfig.Wrap565Enable );
//		fprintf( IniFile, "EnableMultiTextureEXT=%d\n", UserConfig.MultiTextureEXTEnable );
//		fprintf( IniFile, "EnablePaletteEXT=%d\n", UserConfig.PaletteEXTEnable );
		fprintf( IniFile, "EnablePackedPixelsEXT=%d\n", UserConfig.PackedPixelsEXTEnable );
		fprintf( IniFile, "EnableVertexArrayEXT=%d\n", UserConfig.VertexArrayEXTEnable );
//		fprintf( IniFile, "EnableTextureEnvEXT=%d\n", UserConfig.TextureEnvEXTEnable );
		fprintf( IniFile, "EnableSecondaryColorEXT=%d\n", UserConfig.SecondaryColorEXTEnable );
		fprintf( IniFile, "EnableFogCoordEXT=%d\n", UserConfig.FogCoordEXTEnable );
		fprintf( IniFile, "PalettePrecision=%d\n", UserConfig.PalettePrecision );
		fprintf( IniFile, "TextureMemorySize=%d\n", UserConfig.TextureMemorySize );
		fprintf( IniFile, "WrapperPriority=%d\n", UserConfig.Priority );
		fprintf( IniFile, "FrameBufferMemorySize=%d\n", UserConfig.FrameBufferMemorySize );
		fclose( IniFile );
	}
	else
	{
		Pointer = FindConfig( Path, "Version" );
		if ( !strcmp( Pointer, OpenGLideVersion ) )
		{
			Pointer = FindConfig( Path, "EnableMMX" );
			UserConfig.MMXEnable = atoi( Pointer );
//			Pointer = FindConfig( Path, "Enable3DNow" );
//			UserConfig.TDnowEnable = atoi( Pointer );
			Pointer = FindConfig( Path, "CreateWindow" );
			UserConfig.CreateWindow = atoi( Pointer );
			Pointer = FindConfig( Path, "InitFullScreen" );
			UserConfig.InitFullScreen = atoi( Pointer );
			Pointer = FindConfig( Path, "EnableMipMaps" );
			UserConfig.BuildMipMaps = atoi( Pointer );
			Pointer = FindConfig( Path, "EnableFog" );
			UserConfig.FogEnable = atoi( Pointer );
			Pointer = FindConfig( Path, "EnablePrecisionFix" );
			UserConfig.PrecisionFixEnable = atoi( Pointer );
			Pointer = FindConfig( Path, "EnableWrap565" );
			UserConfig.Wrap565Enable = atoi( Pointer );
//			Pointer = FindConfig( Path, "EnableMultiTextureEXT" );
//			UserConfig.MultiTextureEXTEnable = atoi( Pointer );
//			Pointer = FindConfig( Path, "EnablePaletteEXT" );
//			UserConfig.PaletteEXTEnable = atoi( Pointer );
			Pointer = FindConfig( Path, "EnablePackedPixelsEXT" );
			UserConfig.PackedPixelsEXTEnable = atoi( Pointer );
			Pointer = FindConfig( Path, "EnableVertexArrayEXT" );
			UserConfig.VertexArrayEXTEnable = atoi( Pointer );
//			Pointer = FindConfig( Path, "EnableTextureEnvEXT" );
//			UserConfig.TextureEnvEXTEnable = atoi( Pointer );
			Pointer = FindConfig( Path, "EnableSecondaryColorEXT" );
			UserConfig.SecondaryColorEXTEnable = atoi( Pointer );
			Pointer = FindConfig( Path, "EnableFogCoordEXT" );
			UserConfig.FogCoordEXTEnable = atoi( Pointer );
			Pointer = FindConfig( Path, "PalettePrecision" );
			UserConfig.PalettePrecision = atoi( Pointer );
			Pointer = FindConfig( Path, "TextureMemorySize" );
			UserConfig.TextureMemorySize = atoi( Pointer );
			Pointer = FindConfig( Path, "WrapperPriority" );
			UserConfig.Priority = atoi( Pointer );
			Pointer = FindConfig( Path, "FrameBufferMemorySize" );
			UserConfig.FrameBufferMemorySize = atoi( Pointer );
		}
		else
		{
			remove( Path );
			GetOptions();
		}
	}
}

// Detect if Processor has MMX Instructions
int DetectMMX()
{
	DWORD Result;

	__asm
	{
		push EAX
		push EDX
		mov EAX, 1
		CPUID
		mov Result, EDX
		pop EDX
		pop EAX
	}

	return Result & 0x00800000;
}

// Copy Blocks of Memory Using MMX
void MMXCopyMemory( void *Dst, void *Src, DWORD NumberOfBytes )
{
	__asm
	{
		mov ECX, NumberOfBytes
		mov EAX, Src
		mov EDX, Dst
		jmp start
copying:
		MOVQ MM0, [EAX+ECX]
		MOVQ [EDX+ECX], MM0
start:	sub ECX, 8
		jae copying
		EMMS
	}
}

void MMXSetShort( void *Dst, short Value, DWORD NumberOfBytes )
{
	__asm
	{
		xor EAX, EAX
		mov ECX, NumberOfBytes
		mov AX, Value
		mov EDX, Dst
		movd MM0, EAX
		PUNPCKLWD MM0, MM0
		PUNPCKLWD MM0, MM0
		jmp start
copying:
		MOVQ [EDX+ECX], MM0
start:	sub ECX, 8
		jae copying
		EMMS
	}
}

void MMXCopyByteFlip( void *Dst, void *Src, DWORD NumberOfBytes )
{
	__asm
	{		
	  mov ESI, Src
	  mov EDI, Dst
	  mov ECX, NumberOfBytes

				// find the number of pixels that fit in blocks
	  mov EAX, ECX
	  and ECX, ~0xff
	  sub EAX, ECX
	  push EAX
	  test ECX,ECX
	  jz mcbf_breakc
				// init. for inner loop
	  lea ESI, [ESI + ECX]
	  lea EDI, [EDI + ECX - 8]
	  neg ECX
.align 16
	mcbf_loopc:
	  movq MM0, [ESI + ECX]
	  movq MM1, MM0
	  psrlw MM0, 8
	  add ECX, 8
	  psllw MM1, 8
	  por MM1, MM0
	  movq [EDI + ECX], MM1
	  jnz mcbf_loopc
	  add EDI, 8
	  emms
	mcbf_breakc:
	  pop ECX
	  test ECX, ECX
	  jz mcbf_breakd
	  and ECX, ~1
	  lea ESI, [ESI + ECX]
	  lea EDI, [EDI + ECX]
	  neg ECX
	mcbf_loopd:		// trailing data, one pixel at a time
	  mov AL, [ESI + ECX]
	  mov [EDI + ECX + 1], AL 
	  mov DL, [ESI + ECX + 1]
	  mov [EDI + ECX], DL
	  add ECX, 2
	  jnz mcbf_loopd
	mcbf_breakd:
	}
}

