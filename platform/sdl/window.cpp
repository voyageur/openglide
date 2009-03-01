//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*     SDL specific functions for handling display window
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef C_USE_SDL

#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_opengl.h"

#include "GlOgl.h"

#include "platform/window.h"

static struct
{
    Uint16 red[ 256 ];
    Uint16 green[ 256 ];
    Uint16 blue[ 256 ];
} old_ramp;

static bool ramp_stored  = false;

void InitialiseOpenGLWindow(FxU wnd, int x, int y, int width, int height)
{
    if(!SDL_WasInit(SDL_INIT_VIDEO))
    {
        bool err = false;
        char *oldWindowId = 0;

        if (wnd)
        {   // Set SDL window ID
            char windowId[40];
            sprintf (windowId, "SDL_WINDOWID=%ld", (long)wnd);
            oldWindowId = getenv("SDL_WINDOWID");
            if (oldWindowId)
                oldWindowId = strdup(oldWindowId);
            putenv(windowId);
        }

        if (SDL_Init(SDL_INIT_VIDEO))
        {
            GlideMsg("Can't init SDL %s",SDL_GetError());
            err = true;
        }

        if (wnd)
        {   // Restore old value
            if (!oldWindowId)
                putenv("SDL_WINDOWID");
            else
            {
                setenv("SDL_WINDOWID", oldWindowId, 1);
                free (oldWindowId);
            }
        }

        if (err)
            return;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if((SDL_SetVideoMode(width, height, 32, UserConfig.InitFullScreen ? SDL_OPENGL|SDL_FULLSCREEN : SDL_OPENGL)) == 0)
    {
        GlideMsg("Video mode set failed: %s\n", SDL_GetError());
        return;
    }

    UserConfig.PrecisionFix = false;

    if(SDL_GetGammaRamp(old_ramp.red, old_ramp.green, old_ramp.blue) != -1)
        ramp_stored = true;
}

void FinaliseOpenGLWindow( void)
{
    if ( ramp_stored )
        SDL_SetGammaRamp(old_ramp.red, old_ramp.green, old_ramp.blue);
}

void SetGamma(float value)
{
    struct
    {
        Uint16 red[256];
        Uint16 green[256];
        Uint16 blue[256];
    } ramp;
    int i;

    for ( i = 0; i < 256; i++ )
    {
        Uint16 v = (Uint16)( 0xffff * pow( i / 255.0, 1.0 / value ) );

        ramp.red[ i ] = ramp.green[ i ] = ramp.blue[ i ] = ( v & 0xff00 );
    }

    SDL_SetGammaRamp(ramp.red, ramp.green, ramp.blue);
}

void RestoreGamma()
{
}

bool SetScreenMode(int &xsize, int &ysize)
{
    return true;
}

void ResetScreenMode()
{
}

void SwapBuffers()
{
    SDL_GL_SwapBuffers();
}

#endif // C_USE_SDL
