//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*						 Fog Functions
//*					   Made by Glorfindel
//**************************************************************

#include <math.h>
#include "GlOgl.h"
#include "GLRender.h"


// extern functions
void ConvertColorB( GrColor_t GlideColor, BYTE &R, BYTE &G, BYTE &B, BYTE &A );
void ConvertColorF( GrColor_t GlideColor, float &R, float &G, float &B, float &A );
void MMXCopyMemory( void *Dst, void *Src, DWORD NumberOfBytes );

//*************************************************
//* download a fog table
//* Fog is applied after color combining and before alpha blending.
//*************************************************
DLLEXPORT void __stdcall
grFogTable( GrFog_t *ft )
{
#ifdef DONE
	GlideMsg( "grFogTable( --- )\n" );
#endif
	DWORD i, j;
    DWORD s_i, e_i;
    DWORD s, e;
    DWORD forth_root[4] = {0x10000, 0x1306f, 0x16a09, 0x1ae89};

	if ( InternalConfig.FogEnable )
	{
		if ( InternalConfig.MMXEnable )
		{
			MMXCopyMemory( Glide.FogTable, ft, GR_FOG_TABLE_SIZE * sizeof( FxU8 ) );
		}
		else
		{
			CopyMemory( Glide.FogTable, ft, GR_FOG_TABLE_SIZE * sizeof( FxU8 ) );
		}

        for(i = 0; i < GR_FOG_TABLE_SIZE; i++)
        {
            s_i = (forth_root[i&3] >> (16 - (i>>2)));
            e_i = (forth_root[(i+1)&3] >> (16 - ((i+1)>>2)));
            s = ft[i];
            e = (i+1 < GR_FOG_TABLE_SIZE) ? ft[i+1] : 255;

            for(j = s_i; j < e_i; j++)
                OpenGL.FogTable[j] = (BYTE)(s + (e - s) * (j - s_i) / (e_i - s_i));
        }
	}
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
grFogColorValue( GrColor_t fogcolor )
{
#ifdef PARTDONE
	GlideMsg( "grFogColorValue( %x )\n", fogcolor );
#endif

	RenderDrawTriangles();

	Glide.State.FogColorValue = fogcolor;
	ConvertColorF( fogcolor, OpenGL.FogColor[0], OpenGL.FogColor[1], OpenGL.FogColor[2], OpenGL.FogColor[3] );
	glFogfv( GL_FOG_COLOR, &OpenGL.FogColor[0] );
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
grFogMode( GrFogMode_t mode )
{
#ifdef PARTDONE
	GlideMsg( "grFogMode( %d )\n", mode );
#endif

	RenderDrawTriangles();

	static GrFogMode_t modeSource, modeAdd;
	static GLfloat ZeroColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	modeSource = mode & ( GR_FOG_WITH_TABLE | GR_FOG_WITH_ITERATED_ALPHA );
	modeAdd = mode & ( GR_FOG_MULT2 | GR_FOG_ADD2 );

	if ( modeSource )
	{
		OpenGL.Fog = true;
		if ( InternalConfig.FogCoordEXTEnable )
		{
			glEnable( GL_FOG );
		}
	}
	else
	{
		OpenGL.Fog = false;
		glDisable( GL_FOG );
	}

	switch ( modeAdd )
	{
	case GR_FOG_MULT2:
	case GR_FOG_ADD2:
		glFogfv( GL_FOG_COLOR, &ZeroColor[0] );
		break;
	default:
		glFogfv( GL_FOG_COLOR, &OpenGL.FogColor[0] );
		break;
	}
	
	Glide.State.FogMode = modeSource;
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
guFogGenerateExp( GrFog_t *fogtable, float density )
{
#ifdef PARTDONE
	GlideMsg("guFogGenerateExp( ---, %-4.2f )\n", density );
#endif
	static float Temp;

	for( int i = 0; i < GR_FOG_TABLE_SIZE; i++ )
	{
		Temp = (1.0f - (float) exp((- density) * guFogTableIndexToW(i))) * 255.0f;
		fogtable[i] = (BYTE) Temp;
	}
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
guFogGenerateExp2( GrFog_t *fogtable, float density )
{
#ifdef PARTDONE
	GlideMsg("guFogGenerateExp2( ---, %-4.2f )\n", density );
#endif
	static float Temp;

	for( int i = 0; i < GR_FOG_TABLE_SIZE; i++ )
	{
		Temp = (1.0f - (float) exp( (- density)  * guFogTableIndexToW(i) ) * (float)exp((- density)  * guFogTableIndexToW(i)))  * 255.0f;
		fogtable[i] = (BYTE) Temp;
	}
}

//----------------------------------------------------------------
DLLEXPORT void __stdcall
guFogGenerateLinear(	GrFog_t *fogtable,
						float nearZ, float farZ )
{
#ifdef PARTDONE
	GlideMsg("guFogGenerateLinear( ---, %-4.2f, %-4.2f )\n", nearZ, farZ );
#endif
	int Start, End, i;
	static float Temp;

	for( Start = 0; Start < GR_FOG_TABLE_SIZE; Start++ )
	{
		if (guFogTableIndexToW( Start ) >= nearZ )
		{
			break;
		}
	}
	for( End = 0; End < GR_FOG_TABLE_SIZE; End++ )
	{
		if ( guFogTableIndexToW( End ) >= farZ )
		{
			break;
		}
	}

	ZeroMemory( fogtable, GR_FOG_TABLE_SIZE );
	for( i = Start; i <= End; i++ )
	{
		Temp = ((float)(End-Start) / 255.0f * (float)(i-Start));
		fogtable[i] = (BYTE)Temp;
	}

	for( i = End; i < GR_FOG_TABLE_SIZE; i++ )
	{
		fogtable[i] = 255;
	}
}

//*************************************************
//* convert a fog table index to a floating point eye-space w value
//*************************************************
DLLEXPORT float __stdcall
guFogTableIndexToW( int i )
{
#ifdef DONE
	GlideMsg( "guFogTableIndexToW( %d )\n", i );
#endif

	if ( i > GR_FOG_TABLE_SIZE )
	{
		Error( "Error on guFogTableIndexToW( %d )\n", i );
	}

	return (float) pow(2.0, 3.0+(double)(i>>2)) / (8-(i&3));
}

