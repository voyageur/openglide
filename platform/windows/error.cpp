//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*        Windows specific functions for reporting errors
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originally made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifdef WIN32

#include <windows.h>
#include "platform/error.h"

void ReportWarning(const char *message)
{
    MessageBox( NULL, message, "Warning", MB_OK );
}

void ReportError(const char *message)
{
    MessageBox( NULL, message, "Error", MB_OK );
}

#endif // WIN32
