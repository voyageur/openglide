//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*                    OpenGL Extensions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#include <stdio.h>
#include <string.h>

#include "GlOgl.h"
#include "GLRender.h"
#include "Glextensions.h"

enum enExtensionType
{
    OGL_EXT_UNUSED = 0,
    OGL_EXT_REQUIRED,
    OGL_EXT_DESIRED,
};

struct stExtensionSupport
{
    char *          name;
    enExtensionType type;
    bool *          userVar;
    bool *          internalVar;
};

// It is important that dummyExtVariable retains the value true, so
// we pass dummyExtVariable2 in places where the value may be altered.
bool dummyExtVariable = true;
bool dummyExtVariable2 = true;

stExtensionSupport glNecessaryExt[] =
{
    { "GL_EXT_packed_pixels",           OGL_EXT_REQUIRED,   &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_abgr",                    OGL_EXT_REQUIRED,   &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_bgra",                    OGL_EXT_REQUIRED,   &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_secondary_color",         OGL_EXT_DESIRED,    &dummyExtVariable,                  &InternalConfig.EXT_secondary_color },
    { "GL_ARB_multitexture",            OGL_EXT_DESIRED,    &UserConfig.ARB_multitexture,       &InternalConfig.ARB_multitexture },
    { "GL_EXT_fog_coord",               OGL_EXT_DESIRED,    &dummyExtVariable,                  &InternalConfig.EXT_fog_coord },
    { "GL_EXT_texture_env_add",         OGL_EXT_DESIRED,    &dummyExtVariable,                  &InternalConfig.EXT_texture_env_add },
    { "GL_EXT_texture_env_combine",     OGL_EXT_DESIRED,    &dummyExtVariable,                  &InternalConfig.EXT_texture_env_combine },
    { "GL_EXT_texture_lod_bias",        OGL_EXT_DESIRED,    &dummyExtVariable,                  &InternalConfig.EXT_texture_lod_bias },
    { "GL_SGIS_generate_mipmap",        OGL_EXT_DESIRED,    &UserConfig.EnableMipMaps,          &InternalConfig.BuildMipMaps },
    { "GL_EXT_paletted_texture",        OGL_EXT_DESIRED,    &UserConfig.EXT_paletted_texture,   &InternalConfig.EXT_paletted_texture },
    { "GL_EXT_vertex_array",            OGL_EXT_DESIRED,    &UserConfig.EXT_vertex_array,       &InternalConfig.EXT_vertex_array },
    { "GL_EXT_blend_func_separate",     OGL_EXT_DESIRED,    &dummyExtVariable,                  &InternalConfig.EXT_blend_func_separate },
    { "GL_ARB_imaging",                 OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_ARB_texture_env_add",         OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_ARB_transpose_matrix",        OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_compiled_vertex_array",   OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_draw_range_elements",     OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_multi_draw_arrays",       OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_point_parameters",        OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_rescale_normal",          OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_separate_specular_color", OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_stencil_wrap",            OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_texture_edge_clamp",      OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_texture_object",          OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_EXT_vertex_weighting",        OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_IBM_texture_mirrored_repeat", OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_KTX_buffer_region",           OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_NV_blend_square",             OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_NV_evaluators",               OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_NV_fog_distance",             OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_NV_packed_depth_stencil",     OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_NV_texgen_reflection",        OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_NV_texture_env_combine4",     OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_SGIS_multitexture",           OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "GL_WIN_swap_hint",               OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "WGL_EXT_swap_control",           OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 },
    { "",                               OGL_EXT_UNUSED,     &dummyExtVariable,                  &dummyExtVariable2 }
}; 

//Functions
PFNGLCLIENTACTIVETEXTUREPROC            glClientActiveTexture = NULL;

PFNGLMULTITEXCOORD4FARBPROC             glMultiTexCoord4fARB = NULL;
PFNGLMULTITEXCOORD4FVARBPROC            glMultiTexCoord4fvARB = NULL;
PFNGLACTIVETEXTUREARBPROC               glActiveTextureARB = NULL;

PFNGLSECONDARYCOLOR3UBVEXTPROC          glSecondaryColor3ubvEXT = NULL;
PFNGLSECONDARYCOLOR3UBEXTPROC           glSecondaryColor3ubEXT = NULL;
PFNGLSECONDARYCOLOR3FVEXTPROC           glSecondaryColor3fvEXT = NULL;
PFNGLSECONDARYCOLOR3FEXTPROC            glSecondaryColor3fEXT = NULL;
PFNGLSECONDARYCOLORPOINTEREXTPROC       glSecondaryColorPointerEXT = NULL;

PFNGLFOGCOORDFEXTPROC                   glFogCoordfEXT = NULL;
PFNGLFOGCOORDPOINTEREXTPROC             glFogCoordPointerEXT = NULL;

PFNGLCOLORTABLEEXTPROC                  glColorTableEXT = NULL;
PFNGLCOLORSUBTABLEEXTPROC               glColorSubTableEXT = NULL;
PFNGLGETCOLORTABLEEXTPROC               glGetColorTableEXT = NULL;
PFNGLGETCOLORTABLEPARAMETERIVEXTPROC    glGetColorTableParameterivEXT = NULL;
PFNGLGETCOLORTABLEPARAMETERFVEXTPROC    glGetColorTableParameterfvEXT = NULL;

PFNGLBLENDFUNCSEPARATEEXTPROC           glBlendFuncSeparateEXT = NULL;

// Declarations
void GLExtensions( void );

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
// code by Mark J. Kilgard of NVidia modified by Fabio Barros
bool OGLIsExtensionSupported( const char * extension )
{
    const char  * extensions;
    const char  * start;
    char        * where, 
                * terminator;

    where = (char *) strchr( extension, ' ' );
    if ( where || ( *extension == '\0' ) )
    {
        return false;
    }

    extensions = (char*)glGetString( GL_EXTENSIONS );

    start = extensions;

    if ( *start == '\0' )
    {
        Error( "No OpenGL extension supported, using all emulated.\n" );
        return false;
    }

    while ( true )
    {
        where = (char *)strstr( start, extension );
        if ( !where )
        {
            break;
        }
        terminator = where + strlen( extension );
        if ( ( where == start ) || ( *( where - 1 ) == ' ' ) )
        {
            if ( ( *terminator == ' ' ) || ( *terminator == '\0' ) )
            {
                return true;
            }
        }
        start = terminator;
    }

    return false;
}

void ResetInternalConfig( void )
{
    InternalConfig.OGLVersion                   = 1;
    InternalConfig.Priority                     = 2;

    InternalConfig.FogEnable                    = false;
    InternalConfig.InitFullScreen               = false;
    InternalConfig.PrecisionFix                 = false;
    InternalConfig.EnableMipMaps                = false;
    InternalConfig.BuildMipMaps                 = false;
    InternalConfig.IgnorePaletteChange          = false;
    InternalConfig.Wrap565to5551                = false;
    InternalConfig.TextureEnv                   = false;

    InternalConfig.ARB_multitexture             = false;
    InternalConfig.EXT_paletted_texture         = false;
    InternalConfig.EXT_texture_env_add          = false;
    InternalConfig.EXT_texture_env_combine      = false;
    InternalConfig.EXT_vertex_array             = false;
    InternalConfig.EXT_fog_coord                = false;
    InternalConfig.EXT_blend_func_separate      = false;
    InternalConfig.EXT_texture_lod_bias         = false;
    InternalConfig.EXT_secondary_color          = false;

    InternalConfig.TextureMemorySize            = 16;
    InternalConfig.FrameBufferMemorySize        = 8;

    InternalConfig.MMXEnable                    = false;
    InternalConfig.CreateWindow                 = false;
}

void ValidateUserConfig( void )
{
    ResetInternalConfig( );

    InternalConfig.InitFullScreen               = UserConfig.InitFullScreen;
    InternalConfig.PrecisionFix                 = UserConfig.PrecisionFix;
    InternalConfig.EnableMipMaps                = UserConfig.EnableMipMaps;
    InternalConfig.IgnorePaletteChange          = UserConfig.IgnorePaletteChange;
    InternalConfig.Wrap565to5551                = UserConfig.Wrap565to5551;
    
    if ( ( UserConfig.TextureMemorySize >= OGL_MIN_TEXTURE_BUFFER ) && 
         ( UserConfig.TextureMemorySize <= OGL_MAX_TEXTURE_BUFFER ) )
    {
        InternalConfig.TextureMemorySize        = UserConfig.TextureMemorySize;
    }

    if ( ( UserConfig.FrameBufferMemorySize >= OGL_MIN_FRAME_BUFFER ) && 
         ( UserConfig.FrameBufferMemorySize <= OGL_MAX_FRAME_BUFFER ) )
    {
        InternalConfig.FrameBufferMemorySize    = UserConfig.FrameBufferMemorySize;
    }

    GlideMsg( OGL_LOG_SEPARATE );
    GlideMsg( "** OpenGL Information **\n" );
    GlideMsg( OGL_LOG_SEPARATE );
    GlideMsg( "Vendor:      %s\n", glGetString( GL_VENDOR ) );
    GlideMsg( "Renderer:    %s\n", glGetString( GL_RENDERER ) );
    GlideMsg( "Version:     %s\n", glGetString( GL_VERSION ) );
    GlideMsg( "Extensions:  %s\n", glGetString( GL_EXTENSIONS ) );

    GlideMsg( OGL_LOG_SEPARATE );
    InternalConfig.OGLVersion = glGetString( GL_VERSION )[ 2 ] - '0';

    GlideMsg( "OpenGL Extensions:\n" );
    GlideMsg( OGL_LOG_SEPARATE );

    int index = 0;
    while ( strlen( glNecessaryExt[ index ].name ) > 0 )
    {
        switch ( glNecessaryExt[ index ].type )
        {
        case OGL_EXT_REQUIRED:
            if ( ! OGLIsExtensionSupported( glNecessaryExt[ index ].name ) )
            {
                char szError[ 256 ];
                sprintf( szError, "Severe Problem: OpenGL %s extension is required for OpenGLide!", 
                    glNecessaryExt[ index ].name );
                Error( szError );
                GlideMsg( szError );
                MessageBox( 0, szError, "Warning", MB_OK ); 
            }
            break;

        case OGL_EXT_DESIRED:
            if ( ! OGLIsExtensionSupported( glNecessaryExt[ index ].name ) )
            {
                char szError[ 256 ];
                sprintf( szError, "Note: OpenGL %s extension is not supported, emulating behavior.\n", 
                    glNecessaryExt[ index ].name );
                GlideMsg( szError );
            }
            else
            {
                if ( *glNecessaryExt[ index ].userVar )
                {
                    *glNecessaryExt[ index ].internalVar = true;
                    GlideMsg( "Extension %s is present and ENABLED\n", glNecessaryExt[ index ].name );
                }
                else
                {
                    char szError[ 256 ];
                    sprintf( szError, "Note: OpenGL %s extension is supported but disabled by user\n", 
                        glNecessaryExt[ index ].name );
                    GlideMsg( szError );

                    *glNecessaryExt[ index ].internalVar = false;
                }
            }
            break;

        case OGL_EXT_UNUSED:
            break;
        }
        ++index;
    }
    // The above loop leaves InternalConfig.BuildMipMaps set to the negation of its correct value.
    InternalConfig.BuildMipMaps = !InternalConfig.BuildMipMaps;

    GlideMsg( OGL_LOG_SEPARATE );

    if ( InternalConfig.EXT_texture_env_add  && 
         InternalConfig.EXT_texture_env_combine )
    {
        InternalConfig.TextureEnv   = true;
    }

    if ( InternalConfig.EXT_fog_coord )
    {
        InternalConfig.FogEnable    = true;
    }

    if ( DetectMMX( ) )
    {
        InternalConfig.MMXEnable    = true;
    }
    else
    {
        char szError[ 256 ];
        sprintf( szError, "Severe Problem: MMX is required for OpenGLide!" );
        Error( szError );
        GlideMsg( szError );
        MessageBox( 0, szError, "PROBLEM", MB_OK );
        exit( 1 );
    }

    GLExtensions( );
}

void GLExtensions( void )
{
    GLint NumberOfTMUs;

    glActiveTextureARB      = NULL;
    glMultiTexCoord4fARB    = NULL;

    glSecondaryColor3ubvEXT = (PFNGLSECONDARYCOLOR3UBVEXTPROC) DummyV;
    glSecondaryColor3fvEXT  = (PFNGLSECONDARYCOLOR3FVEXTPROC) DummyV;
    glFogCoordfEXT          = (PFNGLFOGCOORDFEXTPROC) DummyF;

    if ( InternalConfig.ARB_multitexture )
    {
        glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &NumberOfTMUs );
        GlideMsg( "MultiTexture Textures Units = %x\n", NumberOfTMUs );

        OpenGL.MultiTextureTMUs     = NumberOfTMUs;
        glClientActiveTexture       = (PFNGLCLIENTACTIVETEXTUREPROC) wglGetProcAddress( "glClientActiveTexture" );
        glActiveTextureARB          = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress( "glActiveTextureARB" );
        glMultiTexCoord4fARB        = (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress( "glMultiTexCoord4fARB" );
        glMultiTexCoord4fvARB       = (PFNGLMULTITEXCOORD4FVARBPROC) wglGetProcAddress( "glMultiTexCoord4fvARB" );

        if ( ( glActiveTextureARB == NULL ) || 
             ( glMultiTexCoord4fARB == NULL ) || 
             ( glMultiTexCoord4fvARB == NULL ) )
        {
            Error( "Could not get the address of MultiTexture functions!\n" );
            InternalConfig.ARB_multitexture = false;
        }
    }

    if ( InternalConfig.EXT_secondary_color )
    {
        glSecondaryColor3ubvEXT     = (PFNGLSECONDARYCOLOR3UBVEXTPROC) wglGetProcAddress( "glSecondaryColor3ubvEXT" );
        glSecondaryColor3ubEXT      = (PFNGLSECONDARYCOLOR3UBEXTPROC) wglGetProcAddress( "glSecondaryColor3ubEXT" );
        glSecondaryColor3fvEXT      = (PFNGLSECONDARYCOLOR3FVEXTPROC) wglGetProcAddress( "glSecondaryColor3fvEXT" );
        glSecondaryColorPointerEXT  = (PFNGLSECONDARYCOLORPOINTEREXTPROC) wglGetProcAddress( "glSecondaryColorPointerEXT" );
        if ( ( glSecondaryColor3ubvEXT == NULL ) || 
             ( glSecondaryColor3ubEXT == NULL )  || 
             ( glSecondaryColorPointerEXT == NULL ) || 
             ( glSecondaryColor3fvEXT == NULL ) )
        {
            Error( "Could not get address of function glSecondaryColorEXT.\n" );
            InternalConfig.EXT_secondary_color = false;
        }
        else
        {
            glEnable( GL_COLOR_SUM_EXT );
        }
    }

    if ( InternalConfig.EXT_fog_coord )
    {
        glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress( "glFogCoordfEXT" );
        glFogCoordPointerEXT = (PFNGLFOGCOORDPOINTEREXTPROC) wglGetProcAddress( "glFogCoordPointerEXT" );
        if ( ( glFogCoordfEXT == NULL ) || 
             ( glFogCoordPointerEXT == NULL ) )
        {
            Error( "Could not get address of function glFogCoordEXT.\n" );
            InternalConfig.EXT_fog_coord = false;
        }
        else
        {
            glFogi( GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT );
            glFogf( GL_FOG_MODE, GL_LINEAR );
            glFogf( GL_FOG_START, 0.0f );
            glFogf( GL_FOG_END, 1.0f );
        }
    }

    if ( InternalConfig.EXT_vertex_array )
    {
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        if ( InternalConfig.EXT_secondary_color )
        {
            glEnableClientState( GL_SECONDARY_COLOR_ARRAY_EXT );
        }
        if ( InternalConfig.EXT_fog_coord )
        {
            glEnableClientState( GL_FOG_COORDINATE_ARRAY_EXT );
        }

        RenderUpdateArrays( );
    }

    if ( InternalConfig.EXT_paletted_texture )
    {
        glColorTableEXT                 = (PFNGLCOLORTABLEEXTPROC) wglGetProcAddress( "glColorTableEXT" );
        glColorSubTableEXT              = (PFNGLCOLORSUBTABLEEXTPROC) wglGetProcAddress( "glColorSubTableEXT" );
        glGetColorTableEXT              = (PFNGLGETCOLORTABLEEXTPROC) wglGetProcAddress( "glGetColorTableEXT" );
        glGetColorTableParameterivEXT   = (PFNGLGETCOLORTABLEPARAMETERIVEXTPROC) wglGetProcAddress( "glGetColorTableParameterivEXT" );
        glGetColorTableParameterfvEXT   = (PFNGLGETCOLORTABLEPARAMETERFVEXTPROC) wglGetProcAddress( "glGetColorTableParameterfvEXT" );

        if ( ( glColorTableEXT == NULL ) || 
             ( glColorSubTableEXT == NULL ) || 
             ( glGetColorTableEXT == NULL ) || 
             ( glGetColorTableParameterivEXT == NULL ) || 
             ( glGetColorTableParameterfvEXT == NULL ) )
        {
            Error( "Could not get address of function for PaletteEXT.\n" );
            InternalConfig.EXT_paletted_texture = false;
        }
        else
        {
            GlideMsg( "Using Palette Extension.\n" );
        }
    }

#ifdef OPENGL_DEBUG
    GLErro( "GLExtensions" );
#endif
}
