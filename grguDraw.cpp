//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*					   Drawing Functions
//*					   Made by Glorfindel
//**************************************************************

#include "GlOgl.h"
#include "GLRender.h"

//*************************************************
//* Draws a Triangle on the screen
//*************************************************
DLLEXPORT void __stdcall
grDrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )
{
#ifdef CRITICAL
	GlideMsg( "grDrawTriangle( ---, ---, --- )\n" );
#endif

	RenderAddTriangle( a, b, c );
}

//*************************************************
//* Draws a Triangle on the screen
//*************************************************
DLLEXPORT void __stdcall
grDrawPlanarPolygonVertexList(int nVertices, const GrVertex vlist[])
{
#ifdef CRITICAL
	GlideMsg("grDrawPlanarPolygonVertexList( %d, --- )\n", nVertices );
#endif

	static int i;
	for( i = 2; i < nVertices; i++ )
		RenderAddTriangle(&vlist[0], &vlist[i-1], &vlist[i]);
}

//*************************************************
//* Draws a Line on the screen
//*************************************************
DLLEXPORT void __stdcall
grDrawLine( const GrVertex *a, const GrVertex *b )
{
#ifdef CRITICAL
	GlideMsg("grDrawLine( ---, --- )\n");
#endif
	
   /*
    * RenderAddLine actually renders the line, so
    * we must render the queued triangles first to
    * avoid out-of-order rendering.
    */
    RenderDrawTriangles();
	RenderAddLine( a, b );
}

//*************************************************
//* Draws a Point on the screen
//*************************************************
DLLEXPORT void __stdcall
grDrawPoint( const GrVertex *a )
{
#ifdef CRITICAL
	GlideMsg("grDrawPoint( --- )\n");
#endif

	RenderAddPoint( a );
}

//*************************************************
//* Draw a convex non-planar polygon
//*************************************************
DLLEXPORT void __stdcall
grDrawPolygon( int nverts, const int ilist[], const GrVertex vlist[] )
{
#ifdef CRITICAL
	GlideMsg("grDrawPolygon( %d, ---, --- )\n");
#endif

	static int i;
	for( i = 2; i < nverts; i++ )
		RenderAddTriangle(&vlist[ilist[0]], &vlist[ilist[i-1]], &vlist[ilist[i]]);
}

//*************************************************
//* Draw a convex planar polygon
//*************************************************
DLLEXPORT void __stdcall
grDrawPlanarPolygon( int nverts, const int ilist[], const GrVertex vlist[] )
{
#ifdef CRITICAL
	GlideMsg("grDrawPlanarPolygon( %d, ---, --- )\n", nverts );
#endif

	static int i;
	for( i = 2; i < nverts; i++ )
		RenderAddTriangle(&vlist[ilist[0]], &vlist[ilist[i-1]], &vlist[ilist[i]]);
}

//*************************************************
//* Draw a convex non-planar polygon
//*************************************************
DLLEXPORT void __stdcall
grDrawPolygonVertexList(int nVertices, const GrVertex vlist[] )
{
#ifdef CRITICAL
	GlideMsg("grDrawPolygonVertexList( %d, --- )\n", nVertices );
#endif

	static int i;
	for( i = 2; i < nVertices; i++ )
		RenderAddTriangle(&vlist[0], &vlist[i-1], &vlist[i]);
}

DLLEXPORT void __stdcall
guAADrawTriangleWithClip( const GrVertex *a, const GrVertex *b, 
						  const GrVertex *c )
{
#ifdef CRITICAL
	GlideMsg("guAADrawTriangleWithClip( ---, ---, --- )\n");
#endif

	RenderAddTriangle( a, b, c );
}

DLLEXPORT void __stdcall
guDrawTriangleWithClip( const GrVertex *a,
                        const GrVertex *b,
                        const GrVertex *c )
{
#ifdef CRITICAL
	GlideMsg("guDrawTriangleWithClip( ---, ---, --- )\n");
#endif

	RenderAddTriangle( a, b, c );
}

DLLEXPORT void __stdcall
guDrawPolygonVertexListWithClip( int nverts, const GrVertex vlist[] )
{
#ifdef CRITICAL
	GlideMsg("guDrawPolygonVertexListWithClip( %d, --- )\n", nverts );
#endif

	static int i;
	for( i = 2; i < nverts; i++ )
		RenderAddTriangle(&vlist[0], &vlist[i-1], &vlist[i]);
}

DLLEXPORT void __stdcall
grAADrawLine(const GrVertex *a, const GrVertex *b)
{
#ifdef CRITICAL
	GlideMsg("grAADrawLine( ---, --- )\n");
#endif

	RenderAddLine( a, b );
}

DLLEXPORT void __stdcall
grAADrawPoint(const GrVertex *a )
{
#ifdef CRITICAL
	GlideMsg("grAADrawPoint( --- )\n");
#endif

	RenderAddPoint( a );
}

DLLEXPORT void __stdcall
grAADrawPolygon(const int nverts, const int ilist[], const GrVertex vlist[])
{
#ifdef CRITICAL
	GlideMsg("grAADrawPolygon( %d, ---, --- )\n", nverts );
#endif

	static int i;
	for( i = 2; i < nverts; i++ )
		RenderAddTriangle(&vlist[ilist[0]], &vlist[ilist[i-1]], &vlist[ilist[i]]);
}

DLLEXPORT void __stdcall
grAADrawPolygonVertexList(const int nverts, const GrVertex vlist[])
{
#ifdef CRITICAL
	GlideMsg("grAADrawPolygonVertexList( %d, --- )\n", nverts );
#endif

	static int i;
	for( i = 2; i < nverts; i++ )
		RenderAddTriangle(&vlist[0], &vlist[i-1], &vlist[i]);
}

DLLEXPORT void __stdcall
grAADrawTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c,
                  FxBool ab_antialias, FxBool bc_antialias, FxBool ca_antialias )
{
#ifdef CRITICAL
	GlideMsg("grAADrawTriangle( ---, ---, ---, %d, %d, %d )\n",
		ab_antialias, bc_antialias, ca_antialias );
#endif

	RenderAddTriangle( a, b, c );
}

