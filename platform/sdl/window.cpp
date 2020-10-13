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

#include "GlOgl.h"

#include "platform/window.h"

static struct
{
    Uint16 red[ 256 ];
    Uint16 green[ 256 ];
    Uint16 blue[ 256 ];
} old_ramp;

static bool ramp_stored  = false;
static bool wasInit      = false;

bool InitialiseOpenGLWindow(FxU wnd, int x, int y, int width, int height)
{
    bool FullScreen = UserConfig.InitFullScreen;
    wasInit = SDL_WasInit(SDL_INIT_VIDEO)!=0;
    if(!wasInit)
    {
        bool err = false;
        char *oldWindowId = 0;
        char windowId[40];

        if (wnd)
        {   // Set SDL window ID
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
                sprintf (windowId, "SDL_WINDOWID=%s", oldWindowId);
                putenv(windowId);
                free (oldWindowId);
            }
        }

        if (err)
            return false;
    } else {
        SDL_Surface* tmpSurface = SDL_GetVideoSurface();
        if (tmpSurface)
        {
            // Preserve window/fullscreen mode in SDL apps and override config file entry
           (tmpSurface->flags&SDL_FULLSCREEN) ? (FullScreen = true) : (FullScreen = false);

            // When in fullscreen, render at the same resolution
            if((FullScreen) && (UserConfig.Resolution < 1.0f)) {
                // Oneday perhaps a proper support for widescreen and 5:4 displays?
                if((float)tmpSurface->w/tmpSurface->h < 1.33f) {
                    OpenGL.WindowWidth = width = tmpSurface->w;
                    OpenGL.WindowHeight = height = tmpSurface->w * 3 / 4;
                } else {
                    OpenGL.WindowWidth = width = tmpSurface->h * 4 / 3;
                    OpenGL.WindowHeight = height = tmpSurface->h;
                }
                UserConfig.Resolution = OpenGL.WindowWidth;
            }
        }
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if((SDL_SetVideoMode(width, height, 32, FullScreen ? SDL_OPENGL|SDL_FULLSCREEN : SDL_OPENGL)) == 0)
    {
        GlideMsg("Video mode set failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &height);
    if ( height > 16 ) {
        UserConfig.PrecisionFix = false;
    }

    if(SDL_GetGammaRamp(old_ramp.red, old_ramp.green, old_ramp.blue) != -1)
        ramp_stored = true;

    return true;
}

void FinaliseOpenGLWindow( void)
{
    if ( ramp_stored )
        SDL_SetGammaRamp(old_ramp.red, old_ramp.green, old_ramp.blue);
    if (!wasInit)
        SDL_Quit();
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
