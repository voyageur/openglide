//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*            Linux specific clock functions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#include "platform/clock.h"

float ClockFrequency( void )
{
    char  str[35];
    float freq = 0.0;
    FILE *f = fopen ("/proc/cpuinfo", "r");
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
