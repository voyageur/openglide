//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*				   Depth (Z/W-Buffer) Functions
//*					   Made by Glorfindel
//**************************************************************

#include "GlOgl.h"
#include "GLRender.h"

//*************************************************
//* Changes Depth Buffer Mode
//*************************************************
DLLEXPORT void __stdcall
grDepthBufferMode( GrDepthBufferMode_t mode )
{
#ifdef DONE
	GlideMsg( "grDepthBufferMode( %d )\n", mode );
#endif

	RenderDrawTriangles();

	Glide.State.DepthBufferMode = mode;

   /*
    * In AddTriangle etc.  Use of z or w for
    * depth buffering is determined by the
    * value of OpenGL.DepthBufferType.  So
    * I set it here.
    */
	switch ( mode )
	{
	case GR_DEPTHBUFFER_DISABLE:
        OpenGL.DepthBufferType = 0;
		glDisable( GL_DEPTH_TEST );
		return;
	case GR_DEPTHBUFFER_ZBUFFER:
	case GR_DEPTHBUFFER_ZBUFFER_COMPARE_TO_BIAS:
        OpenGL.DepthBufferType = 1;
		OpenGL.ZNear = ZBUFFERNEAR;
		OpenGL.ZFar = ZBUFFERFAR;
		break;
	case GR_DEPTHBUFFER_WBUFFER:
	case GR_DEPTHBUFFER_WBUFFER_COMPARE_TO_BIAS:
        OpenGL.DepthBufferType = 0;
		OpenGL.ZNear = WBUFFERNEAR;
		OpenGL.ZFar = WBUFFERFAR;
		break;
	}

	glEnable( GL_DEPTH_TEST );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	if (Glide.State.OriginInformation == GR_ORIGIN_LOWER_LEFT)
	{
		glOrtho( 0, Glide.WindowWidth, 0, Glide.WindowHeight, OpenGL.ZNear, OpenGL.ZFar );
	}
	else
	{
		glOrtho( 0, Glide.WindowWidth, Glide.WindowHeight, 0, OpenGL.ZNear, OpenGL.ZFar );
	}
	glViewport( 0,0, OpenGL.WindowWidth, OpenGL.WindowHeight );	

	glMatrixMode( GL_MODELVIEW );

#ifdef OPENGL_DEBUG
	GLErro( "grDepthBufferMode" );
#endif
}

//*************************************************
//* Enables or Disables Depth Buffer Writting
//*************************************************
DLLEXPORT void __stdcall
grDepthMask( FxBool enable )
{
#ifdef DONE
	GlideMsg( "grDepthMask( %d )\n", enable );
#endif

	RenderDrawTriangles();

	Glide.State.DepthBufferWritting = enable;

	if (Glide.State.DepthBufferWritting == FXFALSE)
		OpenGL.DepthBufferWritting = GL_FALSE;
	else
		OpenGL.DepthBufferWritting = GL_TRUE;

	glDepthMask( OpenGL.DepthBufferWritting );

#ifdef OPENGL_DEBUG
	GLErro( "grDepthMask" );
#endif
}

//*************************************************
//* Sets the Depth Function to use
//*************************************************
DLLEXPORT void __stdcall
grDepthBufferFunction( GrCmpFnc_t func )
{
#ifdef DONE
	GlideMsg( "grDepthBufferFunction( %d )\n", func );
#endif

	RenderDrawTriangles();

	Glide.State.DepthFunction = func;
	switch ( func )
	{
	case GR_CMP_NEVER:		OpenGL.DepthFunction = GL_NEVER;		break;
	case GR_CMP_LESS:		OpenGL.DepthFunction = GL_LESS;			break;
	case GR_CMP_EQUAL: 		OpenGL.DepthFunction = GL_EQUAL;		break;
	case GR_CMP_LEQUAL: 	OpenGL.DepthFunction = GL_LEQUAL;		break;
	case GR_CMP_GREATER: 	OpenGL.DepthFunction = GL_GREATER;		break;
	case GR_CMP_NOTEQUAL: 	OpenGL.DepthFunction = GL_NOTEQUAL;		break;
	case GR_CMP_GEQUAL: 	OpenGL.DepthFunction = GL_GEQUAL;		break;
	case GR_CMP_ALWAYS:		OpenGL.DepthFunction = GL_ALWAYS;		break;
	}

	glDepthFunc( OpenGL.DepthFunction );

#ifdef OPENGL_DEBUG
	GLErro( "grDepthBufferFunction" );
#endif
}

//*************************************************
//* Set the depth bias level
//*************************************************
DLLEXPORT void __stdcall
grDepthBiasLevel( FxI16 level )
{
#ifdef PARTDONE
	GlideMsg( "grDepthBiasLevel( %d )\n", level );
#endif

	RenderDrawTriangles();

	Glide.State.DepthBiasLevel = level;
	//OpenGL.DepthBiasLevel = level * D1OVER65536;
	OpenGL.DepthBiasLevel = level * 10.0f;

	glPolygonOffset( 1.0f, OpenGL.DepthBiasLevel );

	if ( level != 0 )
	{
		glEnable( GL_POLYGON_OFFSET_FILL );
	}
	else
	{
		glDisable( GL_POLYGON_OFFSET_FILL );
	}

#ifdef OPENGL_DEBUG
	GLErro( "grDepthBiasLevel" );
#endif
}

