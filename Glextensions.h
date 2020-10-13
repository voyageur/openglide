//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*                 OpenGL Extensions Header
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************

#ifndef __GLEXTENSIONS__
#define __GLEXTENSIONS__

#ifndef GL_GLEXT_VERSION
#include "glext.h"
#endif

void ValidateUserConfig();

// Extensions Functions Declarations
// This cannot have the same name as the prototype functions
// in glext.h, else compiler compiains about object being
// redeclared as different type of symbol (func->variable).
extern PFNGLCLIENTACTIVETEXTUREARBPROC          p_glClientActiveTexture;
extern PFNGLMULTITEXCOORD4FARBPROC              p_glMultiTexCoord4fARB;
extern PFNGLMULTITEXCOORD4FVARBPROC             p_glMultiTexCoord4fvARB;
extern PFNGLACTIVETEXTUREARBPROC                p_glActiveTextureARB;
extern PFNGLSECONDARYCOLOR3UBVEXTPROC           p_glSecondaryColor3ubvEXT;
extern PFNGLSECONDARYCOLOR3UBEXTPROC            p_glSecondaryColor3ubEXT;
extern PFNGLSECONDARYCOLORPOINTEREXTPROC        p_glSecondaryColorPointerEXT;
extern PFNGLFOGCOORDFEXTPROC                    p_glFogCoordfEXT;
extern PFNGLFOGCOORDPOINTEREXTPROC              p_glFogCoordPointerEXT;
extern PFNGLSECONDARYCOLOR3FVEXTPROC            p_glSecondaryColor3fvEXT;
extern PFNGLSECONDARYCOLOR3FEXTPROC             p_glSecondaryColor3fEXT;

extern PFNGLCOLORTABLEEXTPROC                   p_glColorTableEXT;
extern PFNGLCOLORSUBTABLEEXTPROC                p_glColorSubTableEXT;
extern PFNGLGETCOLORTABLEEXTPROC                p_glGetColorTableEXT;
extern PFNGLGETCOLORTABLEPARAMETERIVEXTPROC     p_glGetColorTableParameterivEXT;
extern PFNGLGETCOLORTABLEPARAMETERFVEXTPROC     p_glGetColorTableParameterfvEXT;

extern PFNGLBLENDFUNCSEPARATEEXTPROC            p_glBlendFuncSeparateEXT;


void APIENTRY DummyV( const void *a );

#endif
