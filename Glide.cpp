//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*                         Main File
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#include <stdio.h>

#include "GlOgl.h"
#include "GLextensions.h"
#include "PGTexture.h"
#include "PGUTexture.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Version ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


char * OpenGLideVersion = "0.09rc4";


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// Main Structs
GlideStruct     Glide;
OpenGLStruct    OpenGL;

// Classes
PGTexture       *Textures;
PGUTexture       UTextures;

#ifdef OGL_DEBUG
    // Profiling variables
    __int64         InitialTick;
    __int64         FinalTick;
    DWORD           Frame;
    double          Fps;
    double          FpsAux;
#endif

double          ClockFreq;

// Error Function variable
GLIDEERRORFUNCTION ExternErrorFunction;

// Number of Errors
unsigned long NumberOfErrors;

// Support DLL functions

void InitMainVariables( void )
{
    OpenGL.WinOpen = false;
    OpenGL.GlideInit = false;
    NumberOfErrors = 0;
    GetOptions( );
}

//*************************************************
//* Initializes the DLL
//*************************************************
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvreserved )
{
    int Priority;

    switch ( dwReason )
    {
    case DLL_THREAD_ATTACH:
        break;

    case DLL_PROCESS_ATTACH:
        if ( !ClearAndGenerateLogFile( ) )
        {
            return false;
        }
        InitMainVariables( );

        if ( SetPriorityClass( GetCurrentProcess( ), NORMAL_PRIORITY_CLASS ) == 0 )
        {
            Error( "Could not set Class Priority.\n" );
        }
        else
        {
            GlideMsg( OGL_LOG_SEPARATE );
            GlideMsg( "Wrapper Class Priority of %d\n", NORMAL_PRIORITY_CLASS );
        }

        switch ( UserConfig.Priority )
        {
        case 0:     Priority = THREAD_PRIORITY_HIGHEST;         break;
        case 1:     Priority = THREAD_PRIORITY_ABOVE_NORMAL;    break;
        case 2:     Priority = THREAD_PRIORITY_NORMAL;          break;
        case 3:     Priority = THREAD_PRIORITY_BELOW_NORMAL;    break;
        case 4:     Priority = THREAD_PRIORITY_LOWEST;          break;
        case 5:     Priority = THREAD_PRIORITY_IDLE;            break;
        default:    Priority = THREAD_PRIORITY_NORMAL;          break;
        }
        if ( SetThreadPriority( GetCurrentThread(), Priority ) == 0 )
        {
            Error( "Could not set Thread Priority.\n" );
        }
        else
        {
            GlideMsg( "Wrapper Priority of %d\n", UserConfig.Priority );
            GlideMsg( OGL_LOG_SEPARATE );
        }
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        grGlideShutdown( );
        CloseLogFile( );
        break;
    }
    return TRUE;
}

bool InitWindow( HWND hwnd )
{
    InitialiseOpenGLWindow( hwnd, 0, 0,  OpenGL.WindowWidth, OpenGL.WindowHeight );

    if ( !strcmp( (char*)glGetString( GL_RENDERER ), "GDI Generic" ) )
    {
        MessageBox( NULL, "You are running in a Non-Accelerated OpenGL!!!\nThings can become really slow", "Warning", MB_OK );
    }

    ValidateUserConfig( );

    GlideMsg( OGL_LOG_SEPARATE );
    GlideMsg( " Setting in Use: \n" );
    GlideMsg( OGL_LOG_SEPARATE );
    GlideMsg( "Init Full Screen = %s\n", InternalConfig.InitFullScreen ? "true" : "false" );
    GlideMsg( "Fog = %s\n", InternalConfig.FogEnable ? "true" : "false" );
    GlideMsg( "Precision Fix = %s\n", InternalConfig.PrecisionFix ? "true" : "false" );
    GlideMsg( "Wrap 565 to 5551 = %s\n", InternalConfig.Wrap565to5551 ? "true" : "false" );
    GlideMsg( "Texture Memory Size = %d Mb\n", InternalConfig.TextureMemorySize );
    GlideMsg( "Frame Buffer Memory Size = %d Mb\n", InternalConfig.FrameBufferMemorySize );
    GlideMsg( "MMX is %s\n", InternalConfig.MMXEnable ? "enabled" : "disabled" );
    GlideMsg( OGL_LOG_SEPARATE );

#ifdef OGL_DEBUG
    GlideMsg( OGL_LOG_SEPARATE );
    GlideMsg( "GlideState size = %d\n", sizeof( GlideState ) );
    GlideMsg( "GrState size = %d\n", sizeof( GrState ) );
    GlideMsg( OGL_LOG_SEPARATE );
#endif

    GlideMsg( OGL_LOG_SEPARATE );
    GlideMsg( "** Glide Calls **\n" );
    GlideMsg( OGL_LOG_SEPARATE );

    return true;
}

//*************************************************
//* Initializes OpenGL
//*************************************************
void InitOpenGL( void )
{
    OpenGL.ZNear = ZBUFFERNEAR;
    OpenGL.ZFar = ZBUFFERFAR;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glOrtho( 0, Glide.WindowWidth, 0, Glide.WindowHeight, OpenGL.ZNear, OpenGL.ZFar );
    glViewport( 0, 0, OpenGL.WindowWidth, OpenGL.WindowHeight );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
//  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
//  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
//  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD );
//  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

    glPixelStorei( GL_PACK_ALIGNMENT, 2);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 2);
}
