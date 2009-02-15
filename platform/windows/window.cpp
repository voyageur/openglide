//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*   Windows specific functions for handling display window
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifndef C_USE_SDL

#include <windows.h>
#include <io.h>
#include <math.h>

#include "GlOgl.h"

#include "platform/window.h"

static HDC   hDC;
static HGLRC hRC;
static HWND  hWND;
static struct
{
    FxU16 red[ 256 ];
    FxU16 green[ 256 ];
    FxU16 blue[ 256 ];
} old_ramp;

static BOOL ramp_stored  = false;
static BOOL mode_changed = false;

void InitialiseOpenGLWindow(FxU wnd, int x, int y, int width, int height)
{
    PIXELFORMATDESCRIPTOR   pfd;
    int                     PixFormat;
    unsigned int            BitsPerPixel;
    HWND                    hwnd = (HWND) wnd;

    if( hwnd == NULL )
    {
        hwnd = GetActiveWindow();
    }

    if ( hwnd == NULL )
    {
        MessageBox( NULL, "NULL window specified", "Error", MB_OK );
        exit( 1 );
    }

    mode_changed = false;

    if ( UserConfig.InitFullScreen )
    {
        SetWindowLong( hwnd, 
                       GWL_STYLE, 
                       WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
        MoveWindow( hwnd, 0, 0, width, height, false );
        mode_changed = SetScreenMode( width, height );
    }
    else
    {
       RECT rect;
       rect.left = 0;
       rect.right = width;
       rect.top = 0;
       rect.bottom = height;

       AdjustWindowRectEx( &rect, 
                           GetWindowLong( hwnd, GWL_STYLE ),
                           GetMenu( hwnd ) != NULL,
                           GetWindowLong( hwnd, GWL_EXSTYLE ) );
       MoveWindow( hwnd, 
                   x, y, 
                   x + ( rect.right - rect.left ),
                   y + ( rect.bottom - rect.top ),
                   true );
    }

    hWND = hwnd;

    hDC = GetDC( hwnd );
    BitsPerPixel = GetDeviceCaps( hDC, BITSPIXEL );

    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize        = sizeof( pfd );
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = BitsPerPixel;
    pfd.cDepthBits   = BitsPerPixel;

    if ( !( PixFormat = ChoosePixelFormat( hDC, &pfd ) ) )
    {
        MessageBox( NULL, "ChoosePixelFormat() failed:  "
                    "Cannot find a suitable pixel format.", "Error", MB_OK );
        exit( 1 );
    } 

    // the window must have WS_CLIPCHILDREN and WS_CLIPSIBLINGS for this call to
    // work correctly, so we SHOULD set this attributes, not doing that yet
    if ( !SetPixelFormat( hDC, PixFormat, &pfd ) )
    {
        MessageBox( NULL, "SetPixelFormat() failed:  "
                    "Cannot set format specified.", "Error", MB_OK );
        exit( 1 );
    } 

    DescribePixelFormat( hDC, PixFormat, sizeof( PIXELFORMATDESCRIPTOR ), &pfd );
    GlideMsg( "ColorBits	= %d\n", pfd.cColorBits );
    GlideMsg( "DepthBits	= %d\n", pfd.cDepthBits );

    if ( pfd.cDepthBits > 16 )
    {
        UserConfig.PrecisionFix = false;
    }

    hRC = wglCreateContext( hDC );
    wglMakeCurrent( hDC, hRC );

    HDC pDC = GetDC( NULL );

    ramp_stored = GetDeviceGammaRamp( pDC, &old_ramp );

    ReleaseDC( NULL, pDC );
}

void FinaliseOpenGLWindow( void)
{
    if ( ramp_stored )
    {
        HDC pDC = GetDC( NULL );

        BOOL res = SetDeviceGammaRamp( pDC, &old_ramp );

        ReleaseDC( NULL, pDC );
    }

    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
    ReleaseDC( hWND, hDC );

    if( mode_changed )
    {
        ResetScreenMode( );
    }
}

void SetGamma(float value)
{
    struct
    {
        WORD red[256];
        WORD green[256];
        WORD blue[256];
    } ramp;
    int i;
    HDC pDC = GetDC( NULL );

    for ( i = 0; i < 256; i++ )
    {
        WORD v = (WORD)( 0xffff * pow( i / 255.0, 1.0 / value ) );

        ramp.red[ i ] = ramp.green[ i ] = ramp.blue[ i ] = ( v & 0xff00 );
    }

    BOOL res = SetDeviceGammaRamp( pDC, &ramp );

    ReleaseDC( NULL, pDC );
}

void RestoreGamma()
{
}

bool SetScreenMode(int &xsize, int &ysize)
{
    HDC     hdc;
    FxU32   bits_per_pixel;
    bool    found;
    DEVMODE DevMode;

    hdc = GetDC( hWND );
    bits_per_pixel = GetDeviceCaps( hdc, BITSPIXEL );
    ReleaseDC( hWND, hdc );
    
    found = false;
    DevMode.dmSize = sizeof( DEVMODE );
    
    for ( int i = 0; 
          !found && EnumDisplaySettings( NULL, i, &DevMode ) != false; 
          i++ )
    {
        if ( ( DevMode.dmPelsWidth == (FxU32)xsize ) && 
             ( DevMode.dmPelsHeight == (FxU32)ysize ) && 
             ( DevMode.dmBitsPerPel == bits_per_pixel ) )
        {
            found = true;
        }
    }
    
    return ( found && ChangeDisplaySettings( &DevMode, CDS_RESET|CDS_FULLSCREEN ) == DISP_CHANGE_SUCCESSFUL );
}

void ResetScreenMode()
{
    ChangeDisplaySettings( NULL, 0 );
}

void SwapBuffers()
{
    SwapBuffers(hDC);
}

#endif // C_USE_SDL
