/****************************************************************************
*  Glide2x.ovl - DOS Glide library
*
* $Id$
*
*/

#define FX_GLIDE_NO_FUNC_PROTO

// Debugging
//#define _DEBUG
//#define _HEAVYDEBUG

#ifdef __WATCOMC__

/* This should be the original glide SDK include */
#include <glide.h>

#ifdef  FX_ENTRY
#undef  FX_ENTRY
#endif
#define FX_ENTRY __export

#define WRITESTRING  printf("glidedll:"); printf

#define PRINTERROR(x);  { if(x) { printf("glidedll:"); printf(x); } printf("\n"); }
#define PRINTINFO(x);  { if(x) { printf("glidedll:"); printf(x); } printf("\n"); }
#define tmpstr 0

#else

/* Safely use C standard lib instead of asm hacks */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define CPPDLL

/* Use OpenGLIDE sdk2_* files */
#include "sdk2_glide.h"

#ifdef  FX_ENTRY
#undef  FX_ENTRY
#endif
#define FX_ENTRY __declspec(dllexport)

#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

// Logging
char * tmpstr = NULL;

#ifdef _DEBUG
FILE * logfile = NULL;
#endif

void WRITESTRING(char const* format,...) {
	va_list msg;
	va_start(msg,format);
	vsnprintf(tmpstr,255,format,msg);
	va_end(msg);
}

#define PRINTERROR(x);  { MessageBox(NULL, x, "Glide2x.dll", MB_ICONERROR); }
#define PRINTINFO(x);  { if(x) { fprintf(logfile, "glidedll:"); fprintf(logfile, x); } fprintf(logfile, "\n"); }

#endif

#include "glidedll.h"

#ifdef _HEAVYDEBUG
int print_func[150];

  #define CALLNUM 100
  #ifdef __WATCOMC__
    #define OUTPUTDEBUG(x, y); { if(print_func[y-_grAADrawLine8] < CALLNUM) { printf("glidedll:"x"\n", y); print_func[y-_grAADrawLine8]++; } }
  #else
    #define OUTPUTDEBUG(x, y); { if(print_func[y-_grAADrawLine8] < CALLNUM) { fprintf(logfile, "glidedll:"x"\n", y); print_func[y-_grAADrawLine8]++; } }
  #endif
#else
  #define OUTPUTDEBUG(x, y);
#endif

#ifdef _DEBUG
  #ifdef __WATCOMC__
    #define CHECKRETURN(x); { printf("glidedll:returning value %d (0x%x)\n", x, x); }
  #else
    #define CHECKRETURN(x); { fprintf(logfile, "glidedll:returning value %d (0x%x)\n", x, x); }
  #endif
#else
  #define CHECKRETURN(x);
#endif

// Macros for function parameters
// Integer parameter
#define IPARAM(a)	{ *p_temp = a; p_temp++; }
// Float parameter
// All floats are passed as ints to avoid compiler initializing 8087
// and failing because libs are not linked in :)
#define FPARAM IPARAM
// Pointer parameter
#define PPARAM(a)	{ pp_temp=(char**)p_temp; *pp_temp=(char*)a; p_temp++; }
// Pointer (translate) parameter
#define TPARAM(a)	PPARAM(a)

#define ALIGN(x, a) (((x)+(a)-1)&~((a)-1))

// Global variables
FxBool grGlidePresent = FXFALSE;

// Shared memory for host communication
void  *memory;		// Pointer to shared memory
int   *p_temp;		// Temp
char  **pp_temp;

// I/O port for host communication
short glideport;

// Screen info
int sc_width, sc_height;
int bpp;

// Pointer to LFB
#define LFB_MAGIC 0xFFFFF1FB
void *phylfb = NULL, *memlfb = NULL;
int pages = 0;

// Initialize glide parameters
int Init(void)
{
	unsigned short seg;
	const char *grport;

#ifdef _HEAVYDEBUG
	int i;
	for(i = 0; i < 150;i ++)
		print_func[i] = 0;
#endif

	// Find glide port from env variable
	/*if(!(grport=getenv("GLIDE"))) {
		PRINTERROR("Cannot determine glide port, are you sure GLIDE is supported?");
		return 0;
	}

	glideport = atoh(grport);
	*/
	/* Fix port at 0x600...avoids setting autoexec.bat in booted Windows and non-working getenv in DOS */
	glideport = 0x600;

#ifdef _DEBUG
#ifndef __WATCOMC__
	logfile = fopen("glide2x.log", "a");
#endif
	WRITESTRING("GLIDE is set to 0x%x", glideport);
	PRINTINFO(tmpstr);
#endif

	// Initialize variables...
	sc_width = sc_height = bpp = 0;

	// Write something to the port to allocate memory
	outp(glideport, '\xff');
	seg = inp(glideport)&0xFF;
	seg |= inp(glideport)<<8;
	memory = (void*)(seg<<4);

	if(memory == NULL) {
		PRINTERROR("Not enough memory!");
		return 0;
	}

#ifdef _DEBUG
	WRITESTRING("Allocated memory at 0x%x (segment: %u)", memory, seg);
	PRINTINFO(tmpstr);
#endif

	grGlidePresent = FXTRUE;

	return 1;	// success
}

#ifdef __WATCOMC__
// dll initializer function (this doesn't seem to get called by dos4/gw)
int __DLLstart_(void* inst, unsigned reason)
{
	return Init();
}
#endif

void FX_ENTRY FX_CALL grGlideInit( void )
{
	if((!grGlidePresent) && (!Init()))
		return;

	p_temp = memory; PPARAM(0)

	OUTPUTDEBUG("grGlideInit Called (value: %d)", _grGlideInit0);

	memlfb = NULL;
	phylfb = (void*)LFB_MAGIC;
	PPARAM(&phylfb);
	PPARAM(&pages);

	outp(glideport, _grGlideInit0);
}

FxBool FX_ENTRY FX_CALL grSstQueryHardware( GrHwConfiguration *hwconfig )
{
	FxBool ret;

	if((!grGlidePresent) && (!Init()))
		return grGlidePresent;

	p_temp = memory; PPARAM(&ret)

	OUTPUTDEBUG("grSstQueryHardware Called (value: %d)", _grSstQueryHardware4);

	TPARAM(hwconfig);

	outp(glideport, _grSstQueryHardware4);

	if(!inp(glideport))	{
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grGlideShutdown( void )
{
	p_temp = memory; PPARAM(0)

	OUTPUTDEBUG("grGlideShutdown Called (value: %d)", _grGlideShutdown0);

	outp(glideport, _grGlideShutdown0);

	// Clean up
	grGlidePresent = FXFALSE;

#ifdef _DEBUG
	PRINTINFO("GLIDE shutdown");
#endif
}

void FX_ENTRY FX_CALL grSstSelect( int which_sst )
{
	p_temp = memory; PPARAM(0)

	OUTPUTDEBUG("grSstSelect Called (value: %d)", _grSstSelect4);

	IPARAM(which_sst);

	outp(glideport, _grSstSelect4);
}

FxBool FX_ENTRY FX_CALL grSstWinOpen(
		FxU32                hWnd,
		GrScreenResolution_t screen_resolution,
		GrScreenRefresh_t    refresh_rate,
		GrColorFormat_t      color_format,
		GrOriginLocation_t   origin_location,
		int                  nColBuffers,
		int                  nAuxBuffers)
{
	FxBool ret;

	OUTPUTDEBUG("grSstWinOpen Called (value: %d)", _grSstWinOpen28);

	switch(screen_resolution)
	{
	case GR_RESOLUTION_320x200:
		sc_width = 320; sc_height = 200;
		break;
	case GR_RESOLUTION_320x240:
		sc_width = 320; sc_height = 240;
		break;
	case GR_RESOLUTION_400x256:
		sc_width = 400; sc_height = 256;
		break;
	case GR_RESOLUTION_512x384:
		sc_width = 512; sc_height = 384;
		break;
	case GR_RESOLUTION_640x200:
		sc_width = 640; sc_height = 200;
		break;
	case GR_RESOLUTION_640x350:
		sc_width = 640; sc_height = 350;
		break;
	case GR_RESOLUTION_640x400:
		sc_width = 640; sc_height = 400;
		break;
	case GR_RESOLUTION_640x480:
		sc_width = 640; sc_height = 480;
		break;
	case GR_RESOLUTION_800x600:
		sc_width = 800; sc_height = 600;
		break;
	case GR_RESOLUTION_960x720:
		sc_width = 960; sc_height = 720;
		break;
	case GR_RESOLUTION_856x480:
		sc_width = 856; sc_height = 480;
		break;
	case GR_RESOLUTION_512x256:
		sc_width = 512; sc_height = 256;
		break;
	case GR_RESOLUTION_1024x768:
		sc_width = 1024; sc_height = 768;
		break;
	case GR_RESOLUTION_1280x1024:
		sc_width = 1280; sc_height = 1024;
		break;
	case GR_RESOLUTION_1600x1200:
		sc_width = 1600; sc_height = 1200;
		break;
	case GR_RESOLUTION_400x300:
		sc_width = 400; sc_height = 300;
		break;
	case GR_RESOLUTION_NONE:
		/* GR_RESOLUTION_NONE is permitted for the SST-96 */
		sc_width = sc_height = 0;
		break;
	default:
		PRINTERROR("Invalid video mode!");
		sc_width = sc_height = 0;
		break;
	}

#ifdef _DEBUG
	WRITESTRING("Open %dx%d mode", sc_width, sc_height);
	PRINTINFO(tmpstr);
#endif

	// Set LFB pointer
	if(((unsigned int)phylfb == LFB_MAGIC) || (pages == 0)) {
		// No LFB address was returned ??
		phylfb = (void*)GLIDE_LFB; pages = 1600*1200*4*3;
		WRITESTRING("Warning, using default LFB address: 0x%x!", phylfb);
		PRINTERROR(tmpstr);
	}

#ifdef _DEBUG
	WRITESTRING("LFB is at phys addr: 0x%x", phylfb);
	PRINTINFO(tmpstr);
#endif

	// Map physical address
	if(memlfb == NULL) {
		memlfb = MapPhysToLin(phylfb, (pages<<12));
	}

#ifdef _DEBUG
	WRITESTRING("LFB is at lin addr: 0x%x", memlfb);
	PRINTINFO(tmpstr);
#endif

	if(memlfb == NULL) {
		WRITESTRING("Memory map failed: 0x%x!", memlfb);
		PRINTERROR(tmpstr);
		/* Let dosbox handle it
		grGlideShutdown();
		return FXFALSE; */
	}

	p_temp = memory; PPARAM(&ret)

	IPARAM(hWnd);
	IPARAM(screen_resolution);
	IPARAM(refresh_rate);
	IPARAM(color_format);
	IPARAM(origin_location);
	IPARAM(nColBuffers);
	IPARAM(nAuxBuffers);
	IPARAM(sc_width);
	IPARAM(sc_height);
	PPARAM(&memlfb);

#ifdef _DEBUG
	WRITESTRING("Return adress pointer: 0x%x", &ret);
	PRINTINFO(tmpstr);
#endif

	outp(glideport, _grSstWinOpen28);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL guColorCombineFunction( GrColorCombineFnc_t fnc )
{
	p_temp = memory; PPARAM(0)

	OUTPUTDEBUG("guColorCombineFunction Called (value: %d)", _guColorCombineFunction4);

	IPARAM(fnc);

	outp(glideport, _guColorCombineFunction4);
}

void FX_ENTRY FX_CALL grBufferClear( GrColor_t color, GrAlpha_t alpha, FxU16 depth )
{
	p_temp = memory; PPARAM(0)

	OUTPUTDEBUG("grBufferClear Called (value: %d)", _grBufferClear12);

	IPARAM(color);
	IPARAM(alpha);
	IPARAM(depth);

	outp(glideport, _grBufferClear12);
}

void FX_ENTRY FX_CALL grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )
{
	p_temp = memory; PPARAM(0)

	OUTPUTDEBUG("grDrawTriangle Called (value: %d)", _grDrawTriangle12);

	TPARAM(a);
	TPARAM(b);
	TPARAM(c);

	outp(glideport, _grDrawTriangle12);
}

void FX_ENTRY FX_CALL grBufferSwap( int swap_interval )
{
	p_temp = memory; PPARAM(0)

	OUTPUTDEBUG("grBufferSwap Called (value: %d)", _grBufferSwap4);

	IPARAM(swap_interval);

	outp(glideport, _grBufferSwap4);
}

void FX_ENTRY FX_CALL grAADrawLine(GrVertex *va, GrVertex *vb)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAADrawLine Called (value: %d)", _grAADrawLine8);

	TPARAM(va);
	TPARAM(vb);

	outp(glideport, _grAADrawLine8);
}

void FX_ENTRY FX_CALL grAADrawPoint(GrVertex *p)
{
	p_temp = memory; PPARAM(0)

	OUTPUTDEBUG("grAADrawPoint Called (value: %d)", _grAADrawPoint4);

	TPARAM(p);

	outp(glideport, _grAADrawPoint4);
}

void FX_ENTRY FX_CALL grAADrawPolygon(int nVerts, const int ilist[], const GrVertex vlist[])
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAADrawPolygon Called (value: %d)", _grAADrawPolygon12);

	IPARAM(nVerts);
	TPARAM(ilist);
	TPARAM(vlist);

	outp(glideport, _grAADrawPolygon12);
}

void FX_ENTRY FX_CALL grAADrawPolygonVertexList(int nVerts, const GrVertex vlist[])
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAADrawPolygonVertexList Called (value: %d)", _grAADrawPolygonVertexList8);

	IPARAM(nVerts);
	TPARAM(vlist);

	outp(glideport, _grAADrawPolygonVertexList8);
}

void FX_ENTRY FX_CALL grAADrawTriangle(GrVertex *a, GrVertex *b, GrVertex *c, FxBool antialiasAB, FxBool antialiasBC,
											FxBool antialiasCA)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAADrawTriangle Called (value: %d)", _grAADrawTriangle24);

	TPARAM(a);
	TPARAM(b);
	TPARAM(c);
	IPARAM(antialiasAB);
	IPARAM(antialiasBC);
	IPARAM(antialiasCA);

	outp(glideport, _grAADrawTriangle24);
}

void FX_ENTRY FX_CALL grAlphaBlendFunction(GrAlphaBlendFnc_t rgb_sf, GrAlphaBlendFnc_t rgb_df, GrAlphaBlendFnc_t alpha_sf,
											GrAlphaBlendFnc_t alpha_df)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAlphaBlendFunction Called (value: %d)", _grAlphaBlendFunction16);

	IPARAM(rgb_sf);
	IPARAM(rgb_df);
	IPARAM(alpha_sf);
	IPARAM(alpha_df);

	outp(glideport, _grAlphaBlendFunction16);
}

void FX_ENTRY FX_CALL grAlphaCombine(	GrCombineFunction_t func, GrCombineFactor_t factor, GrCombineLocal_t local,
											GrCombineOther_t other, FxBool invert)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAlphaCombine Called (value: %d)", _grAlphaCombine20);

	IPARAM(func);
	IPARAM(factor);
	IPARAM(local);
	IPARAM(other);
	IPARAM(invert);

	outp(glideport, _grAlphaCombine20);
}

void FX_ENTRY FX_CALL grAlphaControlsITRGBLighting( FxBool enable )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAlphaControlsITRGBLighting Called (value: %d)", _grAlphaControlsITRGBLighting4);

	IPARAM(enable);

	outp(glideport, _grAlphaControlsITRGBLighting4);
}

void FX_ENTRY FX_CALL grAlphaTestFunction( GrCmpFnc_t function )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAlphaTestFunction Called (value: %d)", _grAlphaTestFunction4);

	IPARAM(function);

	outp(glideport, _grAlphaTestFunction4);
}

void FX_ENTRY FX_CALL grAlphaTestReferenceValue( GrAlpha_t value )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grAlphaTestReferenceValue Called (value: %d)", _grAlphaTestReferenceValue4);

	IPARAM(value);

	outp(glideport, _grAlphaTestReferenceValue4);
}

int FX_ENTRY FX_CALL grBufferNumPending( void )
{
	int ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grBufferNumPending Called (value: %d)", _grBufferNumPending0);

	outp(glideport, _grBufferNumPending0);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grCheckForRoom(FxI32 n)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grCheckForRoom Called (value: %d)", _grCheckForRoom4);

	IPARAM(n);

	outp(glideport, _grCheckForRoom4);
}

void FX_ENTRY FX_CALL grChromakeyMode( GrChromakeyMode_t mode )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grChromakeyMode Called (value: %d)", _grChromakeyMode4);

	IPARAM(mode);

	outp(glideport, _grChromakeyMode4);
}

void FX_ENTRY FX_CALL grChromakeyValue( GrColor_t value )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grChromakeyValue Called (value: %d)", _grChromakeyValue4);

	IPARAM(value);

	outp(glideport, _grChromakeyValue4);
}

void FX_ENTRY FX_CALL grClipWindow(FxU32 minx, FxU32 miny, FxU32 maxx, FxU32 maxy )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grClipWindow Called (value: %d)", _grClipWindow16);

	IPARAM(minx);
	IPARAM(miny);
	IPARAM(maxx);
	IPARAM(maxy);

	outp(glideport, _grClipWindow16);
}

void FX_ENTRY FX_CALL grColorCombine(	GrCombineFunction_t func, GrCombineFactor_t factor, GrCombineLocal_t local,
												GrCombineOther_t other, FxBool invert)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grColorCombine Called (value: %d)", _grColorCombine20);

	IPARAM(func);
	IPARAM(factor);
	IPARAM(local);
	IPARAM(other);
	IPARAM(invert);

	outp(glideport, _grColorCombine20);
}

void FX_ENTRY FX_CALL grColorMask( FxBool rgb, FxBool alpha )	
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grColorMask Called (value: %d)", _grColorMask8);

	IPARAM(rgb);
	IPARAM(alpha);

	outp(glideport, _grColorMask8);
}

void FX_ENTRY FX_CALL grConstantColorValue4( int a, int r, int g, int b )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grConstantColorValue4 Called (value: %d)", _grConstantColorValue416);

	FPARAM(a);
	FPARAM(r);
	FPARAM(g);
	FPARAM(b);

	outp(glideport, _grConstantColorValue416);
}

void FX_ENTRY FX_CALL grConstantColorValue( GrColor_t color )	
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grConstantColorValue Called (value: %d)", _grConstantColorValue4);

	IPARAM(color);

	outp(glideport, _grConstantColorValue4);
}

void FX_ENTRY FX_CALL grCullMode( GrCullMode_t mode )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grCullMode Called (value: %d)", _grCullMode4);

	IPARAM(mode);

	outp(glideport, _grCullMode4);
}

void FX_ENTRY FX_CALL grDepthBiasLevel( FxI16 level )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDepthBiasLevel Called (value: %d)", _grDepthBiasLevel4);

	IPARAM(level);

	outp(glideport, _grDepthBiasLevel4);
}

void FX_ENTRY FX_CALL grDepthBufferFunction( GrCmpFnc_t func )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDepthBufferFunction Called (value: %d)", _grDepthBufferFunction4);

	IPARAM(func);

	outp(glideport, _grDepthBufferFunction4);
}

void FX_ENTRY FX_CALL grDepthBufferMode( GrDepthBufferMode_t mode )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDepthBufferMode Called (value: %d)", _grDepthBufferMode4);

	IPARAM(mode);

	outp(glideport, _grDepthBufferMode4);
}

void FX_ENTRY FX_CALL grDepthMask( FxBool enable )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDepthMask Called (value: %d)", _grDepthMask4);

	IPARAM(enable);

	outp(glideport, _grDepthMask4);
}

void FX_ENTRY FX_CALL grDisableAllEffects( void )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDisableAllEffects Called (value: %d)", _grDisableAllEffects0);

	outp(glideport, _grDisableAllEffects0);
}

void FX_ENTRY FX_CALL grDitherMode( GrDitherMode_t mode )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDitherMode Called (value: %d)", _grDitherMode4);

	IPARAM(mode);

	outp(glideport, _grDitherMode4);
}

void FX_ENTRY FX_CALL grDrawLine( const GrVertex *a, const GrVertex *b )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDrawLine Called (value: %d)", _grDrawLine8);

	TPARAM(a);
	TPARAM(b);

	outp(glideport, _grDrawLine8);
}

void FX_ENTRY FX_CALL grDrawPlanarPolygon( int nVerts, const int ilist[], const GrVertex vlist[] )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDrawPlanarPolygon Called (value: %d)", _grDrawPlanarPolygon12);

	IPARAM(nVerts);
	TPARAM(ilist);
	TPARAM(vlist);

	outp(glideport, _grDrawPlanarPolygon12);
}

void FX_ENTRY FX_CALL grDrawPlanarPolygonVertexList( int nVerts, const GrVertex vlist[] )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDrawPlanarPolygonVertexList Called (value: %d)", _grDrawPlanarPolygonVertexList8);

	IPARAM(nVerts);
	TPARAM(vlist);

	outp(glideport, _grDrawPlanarPolygonVertexList8);
}

void FX_ENTRY FX_CALL grDrawPoint( const GrVertex *a )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDrawPoint Called (value: %d)", _grDrawPoint4);

	TPARAM(a);

	outp(glideport, _grDrawPoint4);
}

void FX_ENTRY FX_CALL grDrawPolygon( int nVerts, const int ilist[], const GrVertex vlist[] )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDrawPolygon Called (value: %d)", _grDrawPolygon12);

	IPARAM(nVerts);
	TPARAM(ilist);
	TPARAM(vlist);

	outp(glideport, _grDrawPolygon12);
}

void FX_ENTRY FX_CALL grDrawPolygonVertexList( int nVerts, const GrVertex vlist[] )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grDrawPolygonVertexList Called (value: %d)", _grDrawPolygonVertexList8);

	IPARAM(nVerts);
	TPARAM(vlist);

	outp(glideport, _grDrawPolygonVertexList8);
}

void FX_ENTRY FX_CALL grErrorSetCallback( void (*function)(const char *string, FxBool fatal) )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grErrorSetCallback Called (value: %d)", _grErrorSetCallback4);

	TPARAM(function);

	// Do nothing...
	//outp(glideport, _grErrorSetCallback4);
}


void FX_ENTRY FX_CALL grFogColorValue( GrColor_t value )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grFogColorValue Called (value: %d)", _grFogColorValue4);

	IPARAM(value);

	outp(glideport, _grFogColorValue4);
}

void FX_ENTRY FX_CALL grFogMode( GrFogMode_t mode )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grFogMode Called (value: %d)", _grFogMode4);

	IPARAM(mode);

	outp(glideport, _grFogMode4);
}

void FX_ENTRY FX_CALL grFogTable( const GrFog_t table[GR_FOG_TABLE_SIZE] )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grFogTable Called (value: %d)", _grFogTable4);

	TPARAM(table);

	outp(glideport, _grFogTable4);
}

void FX_ENTRY FX_CALL grGammaCorrectionValue( int value )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grGammaCorrectionValue Called (value: %d)", _grGammaCorrectionValue4);

	FPARAM(value);

	outp(glideport, _grGammaCorrectionValue4);
}

void FX_ENTRY FX_CALL grGlideGetState( GrState *state )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grGlideGetState Called (value: %d)", _grGlideGetState4);

	TPARAM(state);

	outp(glideport, _grGlideGetState4);
}

void FX_ENTRY FX_CALL grGlideGetVersion( char version[80] )
{
	if((!grGlidePresent) && (!Init()))
		return;

	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grGlideGetVersion Called (value: %d)", _grGlideGetVersion4);

	TPARAM(version);

	outp(glideport, _grGlideGetVersion4);
}

void FX_ENTRY FX_CALL grGlideSetState( const GrState *state )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grGlideSetState Called (value: %d)", _grGlideSetState4);

	TPARAM(state);

	outp(glideport, _grGlideSetState4);
}

void FX_ENTRY FX_CALL grGlideShamelessPlug( const FxBool on )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grGlideShamelessPlug Called (value: %d)", _grGlideShamelessPlug4);

	IPARAM(on);

	outp(glideport, _grGlideShamelessPlug4);
}

void FX_ENTRY FX_CALL grHints( GrHint_t type, FxU32 hintMask )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grHints Called (value: %d)", _grHints8);
	
	IPARAM(type);
	IPARAM(hintMask);

	outp(glideport, _grHints8);
}


void FX_ENTRY FX_CALL grLfbConstantAlpha( GrAlpha_t alpha )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grLfbConstantAlpha Called (value: %d)", _grLfbConstantAlpha4);

	IPARAM(alpha);

	outp(glideport, _grLfbConstantAlpha4);
}

void FX_ENTRY FX_CALL grLfbConstantDepth( FxU16 depth )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grLfbConstantDepth Called (value: %d)", _grLfbConstantDepth4);

	IPARAM(depth);

	outp(glideport, _grLfbConstantDepth4);
}

FxBool FX_ENTRY FX_CALL grLfbLock( GrLock_t type, GrBuffer_t buffer, GrLfbWriteMode_t writeMode, GrOriginLocation_t origin,
												FxBool pixelPipeline, GrLfbInfo_t *info )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)

	/* This was needed for fake locks (?)
	info->strideInBytes = sc_width * 2;
	info->writeMode = GR_LFBWRITEMODE_565;
	*/

	OUTPUTDEBUG("grLfbLock Called (value: %d)", _grLfbLock24);

	IPARAM(type);
	IPARAM(buffer);
	IPARAM(writeMode);
	IPARAM(origin);
	IPARAM(pixelPipeline);
	TPARAM(info);

	outp(glideport, _grLfbLock24);

	if(!inp(glideport))	{
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

#ifdef _DEBUG
	WRITESTRING("info->lfbPtr is set to: 0x%x", info->lfbPtr);
	PRINTINFO(tmpstr);
#endif

	return ret;
}

FxBool FX_ENTRY FX_CALL grLfbReadRegion( GrBuffer_t src_buffer, FxU32 src_x, FxU32 src_y, FxU32 src_width, FxU32 src_height,
												FxU32 dst_stride, void *dst_data )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grLfbReadRegion Called (value: %d)", _grLfbReadRegion28);

	IPARAM(src_buffer);
	IPARAM(src_x);
	IPARAM(src_y);
	IPARAM(src_width);
	IPARAM(src_height);
	IPARAM(dst_stride);
	TPARAM(dst_data);
	
	outp(glideport, _grLfbReadRegion28);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxBool FX_ENTRY FX_CALL grLfbUnlock( GrLock_t type, GrBuffer_t buffer )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grLfbUnlock Called (value: %d)", _grLfbUnlock8);

	IPARAM(type);
	IPARAM(buffer);

	outp(glideport, _grLfbUnlock8);

	if(!inp(glideport))	{
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grLfbWriteColorFormat( GrColorFormat_t colorFormat )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grLfbWriteColorFormat Called (value: %d)", _grLfbWriteColorFormat4);

	IPARAM(colorFormat);

	outp(glideport, _grLfbWriteColorFormat4);
}

void FX_ENTRY FX_CALL grLfbWriteColorSwizzle( FxBool swizzleBytes, FxBool swapWords )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grLfbWriteColorSwizzle Called (value: %d)", _grLfbWriteColorSwizzle8);

	IPARAM(swizzleBytes);
	IPARAM(swapWords);

	outp(glideport, _grLfbWriteColorSwizzle8);
}

FxBool FX_ENTRY FX_CALL grLfbWriteRegion( GrBuffer_t dst_buffer,FxU32 dst_x, FxU32 dst_y, GrLfbSrcFmt_t src_format,FxU32 src_width,
												FxU32 src_height,FxU32 src_stride, void *src_data )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grLfbWriteRegion Called (value: %d)", _grLfbWriteRegion32);

	IPARAM(dst_buffer);
	IPARAM(dst_x);
	IPARAM(dst_y);
	IPARAM(src_format);
	IPARAM(src_width);
	IPARAM(src_height);
	IPARAM(src_stride);
	TPARAM(src_data);

	outp(glideport, _grLfbWriteRegion32);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grRenderBuffer( GrBuffer_t buffer )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grRenderBuffer Called (value: %d)", _grRenderBuffer4);

	IPARAM(buffer);

	outp(glideport, _grRenderBuffer4);
}

void FX_ENTRY FX_CALL grResetTriStats(void)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grResetTriStats Called (value: %d)", _grResetTriStats0);

	outp(glideport, _grResetTriStats0);
}

void FX_ENTRY FX_CALL grSplash(int x, int y, int width, int height, FxU32 frame)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grSplash Called (value: %d)", _grSplash20);

	FPARAM(x);
	FPARAM(y);
	FPARAM(width);
	FPARAM(height);
	IPARAM(frame);

	outp(glideport, _grSplash20);
}

FxBool FX_ENTRY FX_CALL grSstControl( FxU32 code )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grSstControl Called (value: %d)", _grSstControl4);

	IPARAM(code);

	outp(glideport, _grSstControl4);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grSstIdle( void )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grSstIdle Called (value: %d)", _grSstIdle0);

	outp(glideport, _grSstIdle0);
}

FxBool FX_ENTRY FX_CALL grSstIsBusy( void )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grSstIsBusy Called (value: %d)", _grSstIsBusy0);

	outp(glideport, _grSstIsBusy0);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grSstOrigin( GrOriginLocation_t origin )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grSstOrigin Called (value: %d)", _grSstOrigin4);

	IPARAM(origin);

	outp(glideport, _grSstOrigin4);
}

void FX_ENTRY FX_CALL grSstPerfStats( GrSstPerfStats_t *pStats )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grSstPerfStats Called (value: %d)", _grSstPerfStats4);

	TPARAM(pStats);

	outp(glideport, _grSstPerfStats4);
}

FxBool FX_ENTRY FX_CALL grSstQueryBoards( GrHwConfiguration *hwConfig )
{
	FxBool ret;

	if((!grGlidePresent) && (!Init()))
		return grGlidePresent;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grSstQueryBoards Called (value: %d)", _grSstQueryBoards4);

	TPARAM(hwConfig);

	outp(glideport, _grSstQueryBoards4);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grSstResetPerfStats( void )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grSstResetPerfStats Called (value: %d)", _grSstResetPerfStats0);

	outp(glideport, _grSstResetPerfStats0);
}

FxU32 FX_ENTRY FX_CALL grSstScreenHeight( void )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grSstScreenHeight Called (value: %d)", _grSstScreenHeight0);

	outp(glideport, _grSstScreenHeight0);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxU32 FX_ENTRY FX_CALL grSstScreenWidth( void )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grSstScreenWidth Called (value: %d)", _grSstScreenWidth0);
	
	outp(glideport, _grSstScreenWidth0);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxU32 FX_ENTRY FX_CALL grSstStatus( void )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grSstStatus Called (value: %d)", _grSstStatus0);
	
	outp(glideport, _grSstStatus0);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxBool FX_ENTRY FX_CALL grSstVRetraceOn( void )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grSstVRetraceOn Called (value: %d)", _grSstVRetraceOn0);

	outp(glideport, _grSstVRetraceOn0);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxU32 FX_ENTRY FX_CALL grSstVideoLine( void )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grSstVideoLine Called (value: %d)", _grSstVideoLine0);
	
	outp(glideport, _grSstVideoLine0);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grSstWinClose( void )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grSstWinClose Called (value: %d)", _grSstWinClose0);

	outp(glideport, _grSstWinClose0);

	// Invalidate display variables
	sc_width = sc_height = bpp = 0;	
}

FxU32 FX_ENTRY FX_CALL grTexCalcMemRequired( GrLOD_t smallLod, GrLOD_t largeLod,GrAspectRatio_t aspect,
															GrTextureFormat_t format )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grTexCalcMemRequired Called (value: %d)", _grTexCalcMemRequired16);

	IPARAM(smallLod);
	IPARAM(largeLod);
	IPARAM(aspect);
	IPARAM(format);

	outp(glideport, _grTexCalcMemRequired16);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grTexClampMode( GrChipID_t tmu,GrTextureClampMode_t sClampMode,GrTextureClampMode_t tClampMode )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexClampMode Called (value: %d)", _grTexClampMode12);

	IPARAM(tmu);
	IPARAM(sClampMode);
	IPARAM(tClampMode);

	outp(glideport, _grTexClampMode12);
}

void FX_ENTRY FX_CALL grTexCombine( GrChipID_t tmu,GrCombineFunction_t rgb_function, GrCombineFactor_t rgb_factor,
														GrCombineFunction_t alpha_function,GrCombineFactor_t alpha_factor,
														FxBool rgb_invert,FxBool alpha_invert)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexCombine Called (value: %d)", _grTexCombine28);

	IPARAM(tmu);
	IPARAM(rgb_function);
	IPARAM(rgb_factor);
	IPARAM(alpha_function);
	IPARAM(alpha_factor);
	IPARAM(rgb_invert);
	IPARAM(alpha_invert);

	outp(glideport, _grTexCombine28);
}

void FX_ENTRY FX_CALL grTexCombineFunction(GrChipID_t tmu, GrTextureCombineFnc_t fnc)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexCombineFunction Called (value: %d)", _grTexCombineFunction8);

	IPARAM(tmu);
	IPARAM(fnc);

	outp(glideport, _grTexCombineFunction8);
}

void FX_ENTRY FX_CALL grTexDetailControl( GrChipID_t tmu, int lodBias, FxU8 detailScale, int detailMax )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexDetailControl Called (value: %d)", _grTexDetailControl16);

	IPARAM(tmu);
	IPARAM(lodBias);
	IPARAM(detailScale);
	FPARAM(detailMax);

	outp(glideport, _grTexDetailControl16);
}

void FX_ENTRY FX_CALL grTexDownloadMipMap( GrChipID_t tmu, FxU32 startAddress,FxU32 evenOdd, GrTexInfo *info )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexDownloadMipMap Called (value: %d)", _grTexDownloadMipMap16);

	IPARAM(tmu);
	IPARAM(startAddress);
	IPARAM(evenOdd);
	TPARAM(info);

#ifdef _HEAVYDEBUG
	if(print_func[_grTexDownloadMipMap16-_grAADrawLine8] == 1) {
		WRITESTRING("This function containes pointer to a pointer. Value: 0x%x", info->data);
		PRINTINFO(tmpstr);
		print_func[_grTexDownloadMipMap16-_grAADrawLine8] = 2;
	}
#endif

	outp(glideport, _grTexDownloadMipMap16);
}

void FX_ENTRY FX_CALL grTexDownloadMipMapLevel( GrChipID_t tmu, FxU32 startAddress, GrLOD_t thisLod, GrLOD_t largeLod,
																GrAspectRatio_t aspectRatio, GrTextureFormat_t format,
																FxU32 evenOdd, void *data )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexDownloadMipMapLevel Called (value: %d)", _grTexDownloadMipMapLevel32);

	IPARAM(tmu);
	IPARAM(startAddress);
	IPARAM(thisLod);
	IPARAM(largeLod);
	IPARAM(aspectRatio);
	IPARAM(format);
	IPARAM(evenOdd);
	TPARAM(data);

	outp(glideport, _grTexDownloadMipMapLevel32);
}

void FX_ENTRY FX_CALL grTexDownloadMipMapLevelPartial( GrChipID_t tmu, FxU32 startAddress,GrLOD_t thisLod, GrLOD_t largeLod,
																GrAspectRatio_t aspectRatio, GrTextureFormat_t format,
																FxU32 evenOdd, void *data, int start, int end )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexDownloadMipMapLevelPartial Called (value: %d)", _grTexDownloadMipMapLevelPartial40);

	IPARAM(tmu);
	IPARAM(startAddress);
	IPARAM(thisLod);
	IPARAM(largeLod);
	IPARAM(aspectRatio);
	IPARAM(format);
	IPARAM(evenOdd);
	TPARAM(data);
	IPARAM(start);
	IPARAM(end);

	outp(glideport, _grTexDownloadMipMapLevelPartial40);
}

void FX_ENTRY FX_CALL grTexDownloadTable( GrChipID_t tmu, GrTexTable_t type, void *data )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexDownloadTable Called (value: %d)", _grTexDownloadTable12);

	IPARAM(tmu);
	IPARAM(type);
	TPARAM(data);

	outp(glideport, _grTexDownloadTable12);
}

void FX_ENTRY FX_CALL grTexDownloadTablePartial( GrChipID_t tmu, GrTexTable_t type, void *data, int start, int end )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexDownloadTablePartial Called (value: %d)", _grTexDownloadTablePartial20);

	IPARAM(tmu);
	IPARAM(type);
	TPARAM(data);
	IPARAM(start);
	IPARAM(end);

	outp(glideport, _grTexDownloadTablePartial20);
}

void FX_ENTRY FX_CALL grTexFilterMode( GrChipID_t tmu, GrTextureFilterMode_t minFilterMode, GrTextureFilterMode_t magFilterMode )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexFilterMode Called (value: %d)", _grTexFilterMode12);

	IPARAM(tmu);
	IPARAM(minFilterMode);
	IPARAM(magFilterMode);

	outp(glideport, _grTexFilterMode12);
}

void FX_ENTRY FX_CALL grTexLodBiasValue( GrChipID_t tmu, int bias )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexLodBiasValue Called (value: %d)", _grTexLodBiasValue8);

	IPARAM(tmu);
	FPARAM(bias);

	outp(glideport, _grTexLodBiasValue8);
}

FxU32 FX_ENTRY FX_CALL grTexMaxAddress( GrChipID_t tmu )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grTexMaxAddress Called (value: %d)", _grTexMaxAddress4);

	IPARAM(tmu);

	outp(glideport, _grTexMaxAddress4);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxU32 FX_ENTRY FX_CALL grTexMinAddress( GrChipID_t tmu )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grTexMinAddress Called (value: %d)", _grTexMinAddress4);

	IPARAM(tmu);

	outp(glideport, _grTexMinAddress4);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grTexMipMapMode( GrChipID_t tmu, GrMipMapMode_t mode, FxBool lodBlend )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexMipMapMode Called (value: %d)", _grTexMipMapMode12);

	IPARAM(tmu);
	IPARAM(mode);
	IPARAM(lodBlend);

	outp(glideport, _grTexMipMapMode12);
}

void FX_ENTRY FX_CALL grTexMultibase( GrChipID_t tmu, FxBool enable )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexMultibase Called (value: %d)", _grTexMultibase8);

	IPARAM(tmu);
	IPARAM(enable);

	outp(glideport, _grTexMultibase8);
}

void FX_ENTRY FX_CALL grTexMultibaseAddress( GrChipID_t tmu, GrTexBaseRange_t range, FxU32 startAddress, FxU32 evenOdd,
																		GrTexInfo *info )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexMultibaseAddress Called (value: %d)", _grTexMultibaseAddress20);

	IPARAM(tmu);
	IPARAM(range);
	IPARAM(startAddress);
	IPARAM(evenOdd);
	TPARAM(info);

#ifdef _HEAVYDEBUG
	if(print_func[_grTexMultibaseAddress20-_grAADrawLine8] == 1) {
		WRITESTRING("This function containes pointer to a pointer. Value: 0x%x", info->data);
		PRINTINFO(tmpstr);
		print_func[_grTexMultibaseAddress20-_grAADrawLine8] = 2;
	}
#endif

	outp(glideport, _grTexMultibaseAddress20);
}

void FX_ENTRY FX_CALL grTexNCCTable( GrChipID_t tmu, GrNCCTable_t table )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexNCCTable Called (value: %d)", _grTexNCCTable8);

	IPARAM(tmu);
	IPARAM(table);

	outp(glideport, _grTexNCCTable8);
}

void FX_ENTRY FX_CALL grTexSource( GrChipID_t tmu, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTexSource Called (value: %d)", _grTexSource16);

	IPARAM(tmu);
	IPARAM(startAddress);
	IPARAM(evenOdd);
	TPARAM(info);

#ifdef _HEAVYDEBUG
	if(print_func[_grTexSource16-_grAADrawLine8] == 1) {
		WRITESTRING("This function containes pointer to a pointer. Value: 0x%x", info->data);
		PRINTINFO(tmpstr);
		print_func[_grTexSource16-_grAADrawLine8] = 2;
	}
#endif

	outp(glideport, _grTexSource16);
}

FxU32 FX_ENTRY FX_CALL grTexTextureMemRequired( FxU32 evenOdd, GrTexInfo *info )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("grTexTextureMemRequired Called (value: %d)", _grTexTextureMemRequired8);

	IPARAM(evenOdd);
	TPARAM(info);

#ifdef _HEAVYDEBUG
	if(print_func[_grTexTextureMemRequired8-_grAADrawLine8] == 1) {
		WRITESTRING("This function containes pointer to a pointer. Value: 0x%x", info->data);
		PRINTINFO(tmpstr);
		print_func[_grTexTextureMemRequired8-_grAADrawLine8] = 2;
	}
#endif

	outp(glideport, _grTexTextureMemRequired8);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL grTriStats(FxU32 *trisProcessed, FxU32 *trisDrawn)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grTriStats Called (value: %d)", _grTriStats8);

	TPARAM(trisProcessed);
	TPARAM(trisDrawn);

	outp(glideport, _grTriStats8);
}

FxBool FX_ENTRY FX_CALL gu3dfGetInfo( const char *filename, Gu3dfInfo *info )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("gu3dfGetInfo Called (value: %d)", _gu3dfGetInfo8);

	TPARAM(filename);
	TPARAM(info);

#ifdef _HEAVYDEBUG
	if(print_func[_gu3dfGetInfo8-_grAADrawLine8] == 1) {
		WRITESTRING("This function containes pointer to a pointer. Value: 0x%x", info->data);
		PRINTINFO(tmpstr);
		print_func[_gu3dfGetInfo8-_grAADrawLine8] = 2;
	}
#endif

	outp(glideport, _gu3dfGetInfo8);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxBool FX_ENTRY FX_CALL gu3dfLoad( const char *filename, Gu3dfInfo *info )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("gu3dfLoad Called (value: %d)", _gu3dfLoad8);

	TPARAM(filename);
	TPARAM(info);

#ifdef _HEAVYDEBUG
	if(print_func[_gu3dfLoad8-_grAADrawLine8] == 1) {
		WRITESTRING("This function containes pointer to a pointer. Value: 0x%x", info->data);
		PRINTINFO(tmpstr);
		print_func[_gu3dfLoad8-_grAADrawLine8] = 2;
	}
#endif

	outp(glideport, _gu3dfLoad8);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL guAADrawTriangleWithClip( const GrVertex *va, const GrVertex *vb, const GrVertex *vc )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guAADrawTriangleWithClip Called (value: %d)", _guAADrawTriangleWithClip12);

	TPARAM(va);
	TPARAM(vb);
	TPARAM(vc);

	outp(glideport, _guAADrawTriangleWithClip12);
}

void FX_ENTRY FX_CALL guAlphaSource( GrAlphaSource_t mode )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guAlphaSource Called (value: %d)", _guAlphaSource4);

	IPARAM(mode);

	outp(glideport, _guAlphaSource4);
}


void FX_ENTRY FX_CALL guDrawTriangleWithClip(const GrVertex *va, const GrVertex *vb, const GrVertex *vc )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guDrawTriangleWithClip Called (value: %d)", _guDrawTriangleWithClip12);

	TPARAM(va);
	TPARAM(vb);
	TPARAM(vc);

	outp(glideport, _guDrawTriangleWithClip12);
}

void FX_ENTRY FX_CALL guFogGenerateExp2( GrFog_t fogTable[GR_FOG_TABLE_SIZE], int density )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guFogGenerateExp2 Called (value: %d)", _guFogGenerateExp28);

	TPARAM(fogTable);
	FPARAM(density);

	outp(glideport, _guFogGenerateExp28);
}

void FX_ENTRY FX_CALL guFogGenerateExp( GrFog_t fogTable[GR_FOG_TABLE_SIZE], int density )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guFogGenerateExp Called (value: %d)", _guFogGenerateExp8);

	TPARAM(fogTable);
	FPARAM(density);

	outp(glideport, _guFogGenerateExp8);
}

void FX_ENTRY FX_CALL guFogGenerateLinear( GrFog_t fogTable[GR_FOG_TABLE_SIZE], int nearW, int farW )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guFogGenerateLinear Called (value: %d)", _guFogGenerateLinear12);

	TPARAM(fogTable);
	FPARAM(nearW);
	FPARAM(farW);

	outp(glideport, _guFogGenerateLinear12);
}

float FX_ENTRY FX_CALL guFogTableIndexToW( int i )
{
	float ret;

	p_temp = memory; PPARAM(&ret)

	OUTPUTDEBUG("guFogTableIndexToW Called (value: %d)", _guFogTableIndexToW4);

	IPARAM(i);

	outp(glideport, _guFogTableIndexToW4);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

#ifdef _DEBUG
	printf("glidedll:Returning float: 0x%x\n", *(int*)&ret);
#endif

	return ret;
}

GrMipMapId_t FX_ENTRY FX_CALL guTexAllocateMemory( GrChipID_t tmu, FxU8 evenOddMask, int width, int height,
													GrTextureFormat_t format, GrMipMapMode_t mmMode, GrLOD_t smallLod,
													GrLOD_t largeLod, GrAspectRatio_t aspectRatio, GrTextureClampMode_t sClampMode,
													GrTextureClampMode_t tClampMode, GrTextureFilterMode_t minFilterMode,
													GrTextureFilterMode_t magFilterMode, int lodBias, FxBool lodBlend )
{
	GrMipMapId_t ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("guTexAllocateMemory Called (value: %d)", _guTexAllocateMemory60);

	IPARAM(tmu);
	IPARAM(evenOddMask);
	IPARAM(width);
	IPARAM(height);
	IPARAM(format);
	IPARAM(mmMode);
	IPARAM(smallLod);
	IPARAM(largeLod);
	IPARAM(aspectRatio);
	IPARAM(sClampMode);
	IPARAM(tClampMode);
	IPARAM(minFilterMode);
	IPARAM(magFilterMode);
	FPARAM(lodBias);
	IPARAM(lodBlend);

	outp(glideport, _guTexAllocateMemory60);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxBool FX_ENTRY FX_CALL guTexChangeAttributes( GrMipMapId_t mmid, int width, int height, GrTextureFormat_t format,
													GrMipMapMode_t mmMode,GrLOD_t smallLod, GrLOD_t largeLod,
													GrAspectRatio_t aspectRatio,GrTextureClampMode_t sClampMode,
													GrTextureClampMode_t tClampMode,GrTextureFilterMode_t minFilterMode,
													GrTextureFilterMode_t magFilterMode )
{
	FxBool ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("guTexChangeAttributes Called (value: %d)", _guTexChangeAttributes48);

	IPARAM(mmid);
	IPARAM(width);
	IPARAM(height);
	IPARAM(format);
	IPARAM(mmMode);
	IPARAM(smallLod);
	IPARAM(largeLod);
	IPARAM(aspectRatio);
	IPARAM(sClampMode);
	IPARAM(tClampMode);
	IPARAM(minFilterMode);
	IPARAM(magFilterMode);

	outp(glideport, _guTexChangeAttributes48);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL guTexCombineFunction( GrChipID_t tmu, GrTextureCombineFnc_t func )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guTexCombineFunction Called (value: %d)", _guTexCombineFunction8);

	IPARAM(tmu);
	IPARAM(func);

	outp(glideport, _guTexCombineFunction8);
}

void FX_ENTRY FX_CALL guTexDownloadMipMap( GrMipMapId_t mmid, const void *src, const GuNccTable *nccTable )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guTexDownloadMipMap Called (value: %d)", _guTexDownloadMipMap12);

	IPARAM(mmid);
	TPARAM(src);
	TPARAM(nccTable);

	outp(glideport, _guTexDownloadMipMap12);
}

void FX_ENTRY FX_CALL guTexDownloadMipMapLevel( GrMipMapId_t mmid, GrLOD_t lod, const void **src )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guTexDownloadMipMapLevel Called (value: %d)", _guTexDownloadMipMapLevel12);

	IPARAM(mmid);
	IPARAM(lod);
	//CHECK!! ??
	TPARAM(*src);

	outp(glideport, _guTexDownloadMipMapLevel12);
}

GrMipMapId_t FX_ENTRY FX_CALL guTexGetCurrentMipMap ( GrChipID_t tmu )
{
	GrMipMapId_t ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("guTexGetCurrentMipMap Called (value: %d)", _guTexGetCurrentMipMap4);

	IPARAM(tmu);

	outp(glideport, _guTexGetCurrentMipMap4);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

#ifdef __WATCOMC__
GrMipMapInfo* FX_ENTRY FX_CALL guTexGetMipMapInfo( GrMipMapId_t mmid )
#else
/* MSVC6 doesn't like the above line */
FX_ENTRY GrMipMapInfo* FX_CALL guTexGetMipMapInfo( GrMipMapId_t mmid )
#endif
{
	GrMipMapInfo *ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("guTexGetMipMapInfo Called (value: %d)", _guTexGetMipMapInfo4);

	IPARAM(mmid);

	outp(glideport, _guTexGetMipMapInfo4);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

FxU32 FX_ENTRY FX_CALL guTexMemQueryAvail( GrChipID_t tmu )
{
	FxU32 ret;

	p_temp = memory; PPARAM(&ret)
	
	OUTPUTDEBUG("guTexMemQueryAvail Called (value: %d)", _guTexMemQueryAvail4);

	IPARAM(tmu);

	outp(glideport, _guTexMemQueryAvail4);

	if(!inp(glideport)) {
		PRINTERROR("Error getting return information!");
	}

	CHECKRETURN(ret);

	return ret;
}

void FX_ENTRY FX_CALL guTexMemReset( void )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guTexMemReset Called (value: %d)", _guTexMemReset0);

	outp(glideport, _guTexMemReset0);
}

void FX_ENTRY FX_CALL guTexSource( GrMipMapId_t mmid )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guTexSource Called (value: %d)", _guTexSource4);

	IPARAM(mmid);

	outp(glideport, _guTexSource4);
}

// Unknown functions

void FX_ENTRY FX_CALL grSstConfigPipeline(long a, long b, long c)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grSstConfigPipeline Called (value: %d)", _grSstConfigPipeline12);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _grSstConfigPipeline12);
}

void FX_ENTRY FX_CALL grSstVidMode(long a, long b)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("grSstVidMode Called (value: %d)", _grSstVidMode8);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _grSstVidMode8);
}

void FX_ENTRY FX_CALL guDrawPolygonVertexListWithClip(int nverts, const GrVertex vlist[])
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guDrawPolygonVertexListWithClip Called (value: %d)", _guDrawPolygonVertexListWithClip8);

	IPARAM(nverts);
	TPARAM(vlist);

	outp(glideport, _guDrawPolygonVertexListWithClip8);
}

/*int FX_ENTRY FX_CALL guEncodeRLE16( void *dst, void *src, FxU32 width, FxU32 height )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guEncodeRLE16 Called (value: %d)", _guEncodeRLE1616);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guEncodeRLE1616);
}

FxU16 FX_ENTRY FX_CALL guEndianSwapBytes( FxU16 value )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guEndianSwapBytes Called (value: %d)", _guEndianSwapBytes4);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guEndianSwapBytes4);
}

FxU32 FX_ENTRY FX_CALL guEndianSwapWords( FxU32 value )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guEndianSwapWords Called (value: %d)", _guEndianSwapWords4);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guEndianSwapWords4);
}
*/
void FX_ENTRY FX_CALL guMPDrawTriangle(long a, long b, long c)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guMPDrawTriangle Called (value: %d)", _guMPDrawTriangle12);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guMPDrawTriangle12);
}

void FX_ENTRY FX_CALL guMPInit(void)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guMPInit Called (value: %d)", _guMPInit0);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guMPInit0);
}

void FX_ENTRY FX_CALL guMPTexCombineFunction(long a)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guMPTexCombineFunction Called (value: %d)", _guMPTexCombineFunction4);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guMPTexCombineFunction4);
}

void FX_ENTRY FX_CALL guMPTexSource(long a, long b)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guMPTexSource Called (value: %d)", _guMPTexSource8);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guMPTexSource8);
}

void FX_ENTRY FX_CALL guMovieSetName(long a)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guMovieSetName Called (value: %d)", _guMovieSetName4);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guMovieSetName4);
}

void FX_ENTRY FX_CALL guMovieStart(void)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guMovieStart Called (value: %d)", _guMovieStart0);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guMovieStart0);
}

void FX_ENTRY FX_CALL guMovieStop(void)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guMovieStop Called (value: %d)", _guMovieStop0);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guMovieStop0);
}

/*FxU16* FX_ENTRY FX_CALL guTexCreateColorMipMap(void)
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("guTexCreateColorMipMap Called (value: %d)", _guTexCreateColorMipMap0);
	PRINTERROR("Unsupported GLIDE call!");

	outp(glideport, _guTexCreateColorMipMap0);
}
*/
void FX_ENTRY FX_CALL ConvertAndDownloadRle( GrChipID_t tmu, FxU32 startAddress, GrLOD_t thisLod,
							GrLOD_t largeLod, GrAspectRatio_t aspectRatio, GrTextureFormat_t format,
                       FxU32 evenOdd, FxU8 *bm_data, long bm_h, FxU32 u0, FxU32 v0, FxU32 width,
                       FxU32 height, FxU32 dest_width, FxU32 dest_height, FxU16 *tlut )
{
	p_temp = memory; PPARAM(0)
	
	OUTPUTDEBUG("ConvertAndDownloadRle Called (value: %d)", _ConvertAndDownloadRle64);

	IPARAM(tmu);
	IPARAM(startAddress);
	IPARAM(thisLod);
	IPARAM(largeLod);
	IPARAM(aspectRatio);
	IPARAM(format);
	IPARAM(evenOdd);
	TPARAM(bm_data);
	IPARAM(bm_h);
	IPARAM(u0);
	IPARAM(v0);
	IPARAM(width);
	IPARAM(height);
	IPARAM(dest_width);
	IPARAM(dest_height);
	TPARAM(tlut);

	outp(glideport, _ConvertAndDownloadRle64);
}
