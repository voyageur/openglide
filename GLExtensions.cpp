//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*					OpenGL Extensions
//*					Made by Glorfindel
//**************************************************************

#include "GlOgl.h"
#include "Glextensions.h"
#include <string.h>

//Functions
PFNGLMULTITEXCOORD4FARBPROC	glMultiTexCoord4fARB = NULL;
PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB = NULL;

PFNGLSECONDARYCOLOR3UBVEXTPROC glSecondaryColor3ubvEXT = NULL;
PFNGLSECONDARYCOLOR3UBEXTPROC glSecondaryColor3ubEXT = NULL;
PFNGLSECONDARYCOLOR3FVEXTPROC glSecondaryColor3fvEXT = NULL;
PFNGLSECONDARYCOLOR3FEXTPROC glSecondaryColor3fEXT = NULL;
PFNGLSECONDARYCOLORPOINTEREXTPROC glSecondaryColorPointerEXT = NULL;

PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL;
PFNGLFOGCOORDPOINTEREXTPROC glFogCoordPointerEXT = NULL;

PFNGLCOLORTABLEEXTPROC					glColorTableEXT = NULL;
PFNGLCOLORSUBTABLEEXTPROC				glColorSubTableEXT = NULL;
PFNGLGETCOLORTABLEEXTPROC				glGetColorTableEXT = NULL;
PFNGLGETCOLORTABLEPARAMETERIVEXTPROC	glGetColorTableParameterivEXT = NULL;
PFNGLGETCOLORTABLEPARAMETERFVEXTPROC	glGetColorTableParameterfvEXT = NULL;

// Declarations
void GLExtensions();

void APIENTRY DummyV( const void *a )
{
}

void APIENTRY DummyF( GLfloat a )
{
}

void APIENTRY Dummy3ub( GLubyte a, GLubyte b, GLubyte c )
{
}


// check to see if Extension is Supported
// code by Mark J. Kilgard of NVidia
int isExtensionSupported( const char *extension )
{
	const char *extensions;
	const char *start;
	char *where, *terminator;

	where = (char *) strchr( extension, ' ' );
	if ( where || *extension == '\0' )
		return 0;

	extensions = (char*)glGetString( GL_EXTENSIONS );

	start = extensions;

	if (*start == '\0')
	{
		Error( "No OpenGL extension supported, using all emulated.\n" );
		return 0;
	}

	for (;;)
	{
		where = (char *) strstr( start, extension );
		if ( !where )
			break;
		terminator = where + strlen( extension );
		if ( (where == start) || (*(where - 1) == ' ' ) )
			if ( *terminator == ' ' || *terminator == '\0' )
				return 1;
		start = terminator;
	}

	return 0;
}

void ValidateUserConfig()
{
	InternalConfig.FogEnable				= UserConfig.FogEnable;
	InternalConfig.InitFullScreen			= UserConfig.InitFullScreen;
	InternalConfig.PrecisionFixEnable		= UserConfig.PrecisionFixEnable;
	InternalConfig.Wrap565Enable			= UserConfig.Wrap565Enable;
	InternalConfig.BuildMipMaps				= UserConfig.BuildMipMaps;

	InternalConfig.MultiTextureEXTEnable	= false;
	InternalConfig.PaletteEXTEnable			= false;
	InternalConfig.PackedPixelsEXTEnable	= false;
	InternalConfig.TextureEnvEXTEnable		= false;
	InternalConfig.VertexArrayEXTEnable		= false;
	InternalConfig.SecondaryColorEXTEnable	= false;
	InternalConfig.FogCoordEXTEnable		= false;

	InternalConfig.PalettePrecision			= 8;
	InternalConfig.TextureMemorySize		= 4;
	InternalConfig.FrameBufferMemorySize	= 2;

	InternalConfig.MMXEnable				= false;
	InternalConfig.TDnowEnable				= false;
	
	int PalPrec = UserConfig.PalettePrecision;
	if ((PalPrec > 0) && (PalPrec < 128))
	{
		InternalConfig.PalettePrecision		= UserConfig.PalettePrecision;
	}

	int TexSize = UserConfig.TextureMemorySize;
	if ((TexSize > 1) && (TexSize <= 16))
	{
		InternalConfig.TextureMemorySize	= UserConfig.TextureMemorySize;
	}

	int FrameSize = UserConfig.FrameBufferMemorySize;
	if ((FrameSize > 1) && (FrameSize <= 8))
	{
		InternalConfig.FrameBufferMemorySize	= UserConfig.FrameBufferMemorySize;
	}

	InternalConfig.OGLVersion = glGetString( GL_VERSION )[ 2 ] - '0';
	GlideMsg( "Using OpenGL version = %d\n", InternalConfig.OGLVersion );
	if ( InternalConfig.OGLVersion >= 2 )
	{
		InternalConfig.BuildMipMaps				= false;
	}

	if ( UserConfig.MultiTextureEXTEnable )
	{
		if ( isExtensionSupported( "GL_ARB_multitexture" ) )
		{
			InternalConfig.MultiTextureEXTEnable	= true;
		}
	}

//	if ( UserConfig.PaletteEXTEnable )
	{
		if ( isExtensionSupported( "GL_EXT_shared_texture_palette" ) )
		{
			InternalConfig.PaletteEXTEnable			= false;
		}
	}

	if ( UserConfig.PackedPixelsEXTEnable )
	{
		if ( isExtensionSupported( "GL_EXT_packed_pixels" ) )
		{
			InternalConfig.PackedPixelsEXTEnable	= true;
		}
	}

	if ( UserConfig.TextureEnvEXTEnable )
	{
		if ( isExtensionSupported( "GL_EXT_texture_env_add" )  && 
			 isExtensionSupported( "GL_EXT_texture_env_combine" ) )
		{
			InternalConfig.TextureEnvEXTEnable		= true;
		}
	}

	if ( UserConfig.VertexArrayEXTEnable )
	{
		if ( isExtensionSupported( "GL_EXT_vertex_array" ) )
		{
			InternalConfig.VertexArrayEXTEnable		= true;
		}
	}

	if ( UserConfig.SecondaryColorEXTEnable )
	{
		if ( isExtensionSupported( "GL_EXT_secondary_color" ) )
		{
			InternalConfig.SecondaryColorEXTEnable		= true;
		}
	}

	if ( UserConfig.FogCoordEXTEnable )
	{
		if ( isExtensionSupported( "GL_EXT_fog_coord" ) )
		{
			InternalConfig.FogCoordEXTEnable		= true;
		}
	}

	if ( UserConfig.MMXEnable )
	{
		if ( DetectMMX() )
		{
			InternalConfig.MMXEnable		= true;
		}
	}

	if ( UserConfig.TDnowEnable )
	{
		InternalConfig.TDnowEnable = true;
	}
	
	GLExtensions();
}

void GLExtensions()
{
	GLint NumberOfTMUs;

	glActiveTextureARB         = NULL;
	glMultiTexCoord4fARB       = NULL;

	glSecondaryColor3ubvEXT		= (PFNGLSECONDARYCOLOR3UBVEXTPROC) DummyV;
	glSecondaryColor3fvEXT		= (PFNGLSECONDARYCOLOR3FVEXTPROC) DummyV;
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) DummyF;

	if ( InternalConfig.MultiTextureEXTEnable )
	{
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &NumberOfTMUs );
		GlideMsg( "MultiTexture Textures Units = %x\n", NumberOfTMUs );

		OpenGL.MultiTextureTMUs		= NumberOfTMUs;
		glActiveTextureARB			= (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord4fARB		= (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress("glMultiTexCoord4fARB");

		if ((glActiveTextureARB == NULL) || (glMultiTexCoord4fARB == NULL))
		{
			Error( "Could not get the address of MultiTexture functions!\n" );
			InternalConfig.MultiTextureEXTEnable = false;
		}
	}

	if ( InternalConfig.SecondaryColorEXTEnable )
	{
		glSecondaryColor3ubvEXT = (PFNGLSECONDARYCOLOR3UBVEXTPROC) wglGetProcAddress("glSecondaryColor3ubvEXT");
		glSecondaryColor3ubEXT = (PFNGLSECONDARYCOLOR3UBEXTPROC) wglGetProcAddress("glSecondaryColor3ubEXT");
		glSecondaryColor3fvEXT = (PFNGLSECONDARYCOLOR3FVEXTPROC) wglGetProcAddress("glSecondaryColor3fvEXT");
		glSecondaryColorPointerEXT = (PFNGLSECONDARYCOLORPOINTEREXTPROC) wglGetProcAddress("glSecondaryColorPointerEXT");
		if ( ( glSecondaryColor3ubvEXT == NULL ) || ( glSecondaryColor3ubEXT == NULL )  || 
			 (glSecondaryColorPointerEXT == NULL ) || (glSecondaryColor3fvEXT == NULL))
		{
			Error( "Could not get address of function glSecondaryColorEXT.\n" );
			InternalConfig.SecondaryColorEXTEnable		= false;
		}
		else
		{
			glEnable( GL_COLOR_SUM_EXT );
			if ( InternalConfig.VertexArrayEXTEnable )
			{
				glEnableClientState( GL_SECONDARY_COLOR_ARRAY_EXT );
			}
		}
	}

	if ( InternalConfig.FogCoordEXTEnable )
	{
		glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");
		glFogCoordPointerEXT = (PFNGLFOGCOORDPOINTEREXTPROC) wglGetProcAddress("glFogCoordPointerEXT");
		if ( (glFogCoordfEXT == NULL ) || ( glFogCoordPointerEXT == NULL ) )
		{
			Error( "Could not get address of function glFogCoordEXT.\n" );
			InternalConfig.FogCoordEXTEnable = false;
		}
		else
		{
			glFogi( GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT );
			glFogf( GL_FOG_MODE, GL_LINEAR );
			glFogf( GL_FOG_START, 0.0f );
			glFogf( GL_FOG_END, 1.0f );
			if ( InternalConfig.VertexArrayEXTEnable )
			{
				glEnableClientState( GL_FOG_COORDINATE_ARRAY_EXT );
			}
		}
	}

	if ( InternalConfig.VertexArrayEXTEnable )
	{
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		RenderUpdateArrays();
	}

	if ( InternalConfig.PaletteEXTEnable )
	{
		glColorTableEXT =					(PFNGLCOLORTABLEEXTPROC) wglGetProcAddress("glColorTableEXT");
		glColorSubTableEXT =				(PFNGLCOLORSUBTABLEEXTPROC) wglGetProcAddress("glColorSubTableEXT");
		glGetColorTableEXT =				(PFNGLGETCOLORTABLEEXTPROC) wglGetProcAddress("glGetColorTableEXT");
		glGetColorTableParameterivEXT =		(PFNGLGETCOLORTABLEPARAMETERIVEXTPROC) wglGetProcAddress("glGetColorTableParameterivEXT");
		glGetColorTableParameterfvEXT =		(PFNGLGETCOLORTABLEPARAMETERFVEXTPROC) wglGetProcAddress("glGetColorTableParameterfvEXT");

		if ( ( glColorTableEXT == NULL ) || ( glColorSubTableEXT == NULL ) || ( glGetColorTableEXT == NULL ) ||
			( glGetColorTableParameterivEXT == NULL ) || ( glGetColorTableParameterfvEXT == NULL ) )
		{
			Error( "Could not get address of function for PaletteEXT.\n" );
			InternalConfig.PaletteEXTEnable = false;
		}
		else
		{
			GlideMsg( "Using Palette Extension.\n" );
			glEnable( GL_SHARED_TEXTURE_PALETTE_EXT );

			if ( glIsEnabled( GL_SHARED_TEXTURE_PALETTE_EXT ) == GL_FALSE )
			{
				GlideMsg( "Nao habilitou a Shared Palette Texture" );
			}
		}
	}

#ifdef OPENGL_DEBUG
	GLErro( "GLExtensions" );
#endif
}
