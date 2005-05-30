//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*     Windows specific functions for library initialisation
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#include <windows.h>
#include "GlOgl.h"

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
