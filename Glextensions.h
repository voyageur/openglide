
#ifndef __GLEXTENSIONS__
#define __GLEXTENSIONS__

#include "Glext.h"

void ValidateUserConfig();

// Extensions Functions Declarations
extern PFNGLMULTITEXCOORD4FARBPROC				glMultiTexCoord4fARB;
extern PFNGLACTIVETEXTUREARBPROC				glActiveTextureARB;
extern PFNGLSECONDARYCOLOR3UBVEXTPROC			glSecondaryColor3ubvEXT;
extern PFNGLSECONDARYCOLOR3UBEXTPROC			glSecondaryColor3ubEXT;
extern PFNGLSECONDARYCOLORPOINTEREXTPROC		glSecondaryColorPointerEXT;
extern PFNGLFOGCOORDFEXTPROC					glFogCoordfEXT;
extern PFNGLFOGCOORDPOINTEREXTPROC				glFogCoordPointerEXT;
extern PFNGLSECONDARYCOLOR3FVEXTPROC			glSecondaryColor3fvEXT;
extern PFNGLSECONDARYCOLOR3FEXTPROC				glSecondaryColor3fEXT;

extern PFNGLCOLORTABLEEXTPROC					glColorTableEXT;
extern PFNGLCOLORSUBTABLEEXTPROC				glColorSubTableEXT;
extern PFNGLGETCOLORTABLEEXTPROC				glGetColorTableEXT;
extern PFNGLGETCOLORTABLEPARAMETERIVEXTPROC		glGetColorTableParameterivEXT;
extern PFNGLGETCOLORTABLEPARAMETERFVEXTPROC		glGetColorTableParameterfvEXT;


void APIENTRY DummyV( const void *a );

#endif