
#include <ddraw.h>
#include "GlOgl.h"


// DirectDraw Stuff
LPDIRECTDRAW			DDraw			= 0;
LPDIRECTDRAWSURFACE		FrontSurface	= 0;
LPDIRECTDRAWSURFACE		BackSurface		= 0;
LPDIRECTDRAWSURFACE		DepthSurface	= 0;


void DirectXError( HRESULT ErrorCode )
{
	switch (ErrorCode)
	{
	case DDERR_INCOMPATIBLEPRIMARY:
		Error( "DirectDraw: DDERR_INCOMPATIBLEPRIMARY\n");
		break;
	case DDERR_INVALIDCAPS:
		Error( "DirectDraw: DDERR_INVALIDCAPS\n");
		break;
	case DDERR_INVALIDOBJECT:
		Error( "DirectDraw: DDERR_INVALIDOBJECT\n");
		break;
	case DDERR_INVALIDPARAMS:
		Error( "DirectDraw: DDERR_INVALIDPARAMS\n");
		break;
	case DDERR_INVALIDPIXELFORMAT:
		Error( "DirectDraw: DDERR_INVALIDPIXELFORMAT\n");
		break;
	case DDERR_NOALPHAHW:
		Error( "DirectDraw: DDERR_NOALPHAHW\n");
		break;
	case DDERR_NOCOOPERATIVELEVELSET:
		Error( "DirectDraw: DDERR_NOCOOPERATIVELEVELSET\n");
		break;
	case DDERR_NODIRECTDRAWHW:
		Error( "DirectDraw: DDERR_NODIRECTDRAWHW\n");
		break;
	case DDERR_NOEMULATION:
		Error( "DirectDraw: DDERR_NOEMULATION\n");
		break;
	case DDERR_NOEXCLUSIVEMODE:
		Error( "DirectDraw: DDERR_NOEXCLUSIVEMODE\n");
		break;
	case DDERR_NOFLIPHW:
		Error( "DirectDraw: DDERR_NOFLIPHW\n");
		break;
	case DDERR_NOMIPMAPHW:
		Error( "DirectDraw: DDERR_NOMIPMAPHW\n");
		break;
	case DDERR_NOZBUFFERHW:
		Error( "DirectDraw: DDERR_NOZBUFFERHW\n");
		break;
	case DDERR_OUTOFMEMORY:
		Error( "DirectDraw: DDERR_OUTOFMEMORY\n");
		break;
	case DDERR_OUTOFVIDEOMEMORY: 
		Error( "DirectDraw: DDERR_OUTOFVIDEOMEMORY\n");
		break;
	case DDERR_PRIMARYSURFACEALREADYEXISTS:
		Error( "DirectDraw: DDERR_PRIMARYSURFACEALREADYEXISTS\n");
		break;
	case DDERR_UNSUPPORTEDMODE:
		Error( "DirectDraw: DDERR_UNSUPPORTEDMODE\n");
		break;
	case DDERR_SURFACEBUSY:
		Error( "DirectDraw: DDERR_SURFACEBUSY\n");
		break;
	case DDERR_SURFACELOST:
		Error( "DirectDraw: DDERR_SURFACELOST\n");
		break;
	case DDERR_WASSTILLDRAWING:
		Error( "DirectDraw: DDERR_WASSTILLDRAWING\n");
		break;
	case DDERR_GENERIC:
		Error( "DirectDraw: DDERR_GENERIC\n");
		break;
	case DDERR_INVALIDRECT:
		Error( "DirectDraw: DDERR_INVALIDRECT\n");
		break;
	case DDERR_NOTLOCKED:
		Error( "DirectDraw: DDERR_NOTLOCKED\n");
		break;
	}
}

bool InitDirectDraw( int Width, int Height )
{
	HRESULT ErrorCode;

	if (FrontSurface)
	{
		FrontSurface->Release();
		FrontSurface = 0;
	}
	if (DepthSurface)
	{
		DepthSurface->Release();
		DepthSurface = 0;
	}

	DDSURFACEDESC	desc;
	desc.dwSize = sizeof( desc );
	desc.dwFlags = DDSD_BACKBUFFERCOUNT | DDSD_CAPS;
	desc.dwBackBufferCount = 1;
	desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;

	if ((ErrorCode = DDraw->CreateSurface( &desc, &FrontSurface, 0 )) != DD_OK)
	{
		DirectXError( ErrorCode );
		return false;
	}

	DDSCAPS	ddscaps;
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	FrontSurface->GetAttachedSurface( &ddscaps, &BackSurface );

	memset( &desc, 0, sizeof(desc) );
	desc.dwSize = sizeof( desc );
	desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_ZBUFFERBITDEPTH;
	desc.dwWidth = Width;
	desc.dwHeight = Height;
	desc.dwZBufferBitDepth = 16;
	desc.ddsCaps.dwCaps = DDSCAPS_ZBUFFER;
	DDraw->CreateSurface( &desc, &DepthSurface, 0 );
	BackSurface->AddAttachedSurface( DepthSurface );

	return true;
}

