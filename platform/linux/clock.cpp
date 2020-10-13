//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*            Linux specific clock functions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originally made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifndef WIN32

#include <stdio.h>
#include <string.h>
#include "platform/clock.h"

float ClockFrequency( void )
{
    char  str[35];
    float freq = 0.0;
    FILE *f = fopen ("/proc/cpuinfo", "r");
    // Be nice and don't crash
    if ( f == NULL ) return freq;
    while (fgets (str, sizeof (str), f))
    {
        if (!strncmp ("cpu MHz", str, 7))
        {
            sscanf (strchr(str, ':')+1, "%f", &freq);
            break;
        }
    }
    fclose (f);
    return freq * 1000000.0;
}

#endif // WIN32
