//**************************************************************
//*				OpenGLide - Glide->OpenGL Wrapper
//*						Render Class
//*					 Made by Glorfindel
//**************************************************************

#include "GlOGl.h"
#include "GLRender.h"
#include "GLextensions.h"
#include "amd3dx.h"
#include "profile.h"

bool	SecondPass;
float (*AlphaFactorFunc)( float LocalAlpha, float OtherAlpha );
void (*ColorFactor3Func)( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha );
void (*ColorFunctionFunc)( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other );

// extern variables

void MMXCopyByteFlip( void *Dst, void *Src, DWORD NumberOfBytes );

// Snapping constant
const float vertex_snap = float(3L << 18);

RenderStruct OGLRender;
TColorStruct CFactor;

float * ConstantForMultiply = NULL;
float * ConstantForAnd = NULL;
float * ConstantForSubtract = NULL;

void __fastcall ColorFactor3Zero( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	Result->ar = Result->ag = Result->ab = 0.0f;
	Result->br = Result->bg = Result->bb = 0.0f;
	Result->cr = Result->cg = Result->cb = 0.0f;
}

void __fastcall ColorFactor3Local( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	Result->ar = ColorComponent->ar;
	Result->ag = ColorComponent->ag;
	Result->ab = ColorComponent->ab;
	Result->br = ColorComponent->br;
	Result->bg = ColorComponent->bg;
	Result->bb = ColorComponent->bb;
	Result->cr = ColorComponent->cr;
	Result->cg = ColorComponent->cg;
	Result->cb = ColorComponent->cb;
}

void __fastcall ColorFactor3OtherAlpha( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	Result->ar = Result->ag = Result->ab = OtherAlpha->aa;
	Result->br = Result->bg = Result->bb = OtherAlpha->ba;
	Result->cr = Result->cg = Result->cb = OtherAlpha->ca;
}

void __fastcall ColorFactor3LocalAlpha( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	Result->ar = Result->ag = Result->ab = ColorComponent->aa;
	Result->br = Result->bg = Result->bb = ColorComponent->ba;
	Result->cr = Result->cg = Result->cb = ColorComponent->ca;
}

void __fastcall ColorFactor3OneMinusLocal( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	Result->ar = 1.0f - ColorComponent->ar;
	Result->ag = 1.0f - ColorComponent->ag;
	Result->ab = 1.0f - ColorComponent->ab;
	Result->br = 1.0f - ColorComponent->br;
	Result->bg = 1.0f - ColorComponent->bg;
	Result->bb = 1.0f - ColorComponent->bb;
	Result->cr = 1.0f - ColorComponent->cr;
	Result->cg = 1.0f - ColorComponent->cg;
	Result->cb = 1.0f - ColorComponent->cb;
}

void __fastcall ColorFactor3OneMinusOtherAlpha( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	Result->ar = Result->ag = Result->ab = 1.0f - OtherAlpha->aa;
	Result->br = Result->bg = Result->bb = 1.0f - OtherAlpha->ba;
	Result->cr = Result->cg = Result->cb = 1.0f - OtherAlpha->ca;
}

void __fastcall ColorFactor3OneMinusLocalAlpha( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	Result->ar = Result->ag = Result->ab = 1.0f - ColorComponent->aa;
	Result->br = Result->bg = Result->bb = 1.0f - ColorComponent->ba;
	Result->cr = Result->cg = Result->cb = 1.0f - ColorComponent->ca;
}

void __fastcall ColorFactor3One( TColorStruct *Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	Result->ar = Result->ag = Result->ab = 1.0f;
	Result->br = Result->bg = Result->bb = 1.0f;
	Result->cr = Result->cg = Result->cb = 1.0f;
}


float AlphaFactorZero( float LocalAlpha, float OtherAlpha )
{
	return 0.0f;
}

float AlphaFactorLocal( float LocalAlpha, float OtherAlpha )
{
	return LocalAlpha;
}

float AlphaFactorOther( float LocalAlpha, float OtherAlpha )
{
	return OtherAlpha;
}

float AlphaFactorOneMinusLocal( float LocalAlpha, float OtherAlpha )
{
	return 1.0f - LocalAlpha;
}

float AlphaFactorOneMinusOther( float LocalAlpha, float OtherAlpha )
{
	return 1.0f - OtherAlpha;
}

float AlphaFactorOne( float LocalAlpha, float OtherAlpha )
{
	return 1.0f;
}

void ColorFunctionZero( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	pC->ar = pC->ag = pC->ab = 0.0f; 
	pC->br = pC->bg = pC->bb = 0.0f; 
	pC->cr = pC->cg = pC->cb = 0.0f; 
}

void ColorFunctionLocal( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	pC->ar = Local->ar;
	pC->ag = Local->ag;
	pC->ab = Local->ab;
	pC->br = Local->br;
	pC->bg = Local->bg;
	pC->bb = Local->bb;
	pC->cr = Local->cr;
	pC->cg = Local->cg;
	pC->cb = Local->cb;
}

void ColorFunctionLocalAlpha( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	pC->ar = pC->ag = pC->ab = Local->aa;
	pC->br = pC->bg = pC->bb = Local->ba;
	pC->cr = pC->cg = pC->cb = Local->ca;
}

void ColorFunctionScaleOther( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	ColorFactor3Func( &CFactor, Local, Other );
	pC->ar = CFactor.ar * Other->ar;
	pC->ag = CFactor.ag * Other->ag;
	pC->ab = CFactor.ab * Other->ab;
	pC->br = CFactor.br * Other->br;
	pC->bg = CFactor.bg * Other->bg;
	pC->bb = CFactor.bb * Other->bb;
	pC->cr = CFactor.cr * Other->cr;
	pC->cg = CFactor.cg * Other->cg;
	pC->cb = CFactor.cb * Other->cb;
}

void ColorFunctionScaleOtherAddLocal( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	ColorFactor3Func( &CFactor, Local, Other );
	pC->ar = CFactor.ar * Other->ar + Local->ar;
	pC->ag = CFactor.ag * Other->ag + Local->ag;
	pC->ab = CFactor.ab * Other->ab + Local->ab;
	pC->br = CFactor.br * Other->br + Local->br;
	pC->bg = CFactor.bg * Other->bg + Local->bg;
	pC->bb = CFactor.bb * Other->bb + Local->bb;
	pC->cr = CFactor.cr * Other->cr + Local->cr;
	pC->cg = CFactor.cg * Other->cg + Local->cg;
	pC->cb = CFactor.cb * Other->cb + Local->cb;
	pC2->ar = Local->ar;
	pC2->ag = Local->ag;
	pC2->ab = Local->ab;
	pC2->br = Local->br;
	pC2->bg = Local->bg;
	pC2->bb = Local->bb;
	pC2->cr = Local->cr;
	pC2->cg = Local->cg;
	pC2->cb = Local->cb;
	SecondPass = true;
}

void ColorFunctionScaleOtherAddLocalAlpha( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	ColorFactor3Func( &CFactor, Local, Other );
	pC->ar = CFactor.ar * Other->ar + Local->aa;
	pC->ag = CFactor.ag * Other->ag + Local->aa;
	pC->ab = CFactor.ab * Other->ab + Local->aa;
	pC->br = CFactor.br * Other->br + Local->ba;
	pC->bg = CFactor.bg * Other->bg + Local->ba;
	pC->bb = CFactor.bb * Other->bb + Local->ba;
	pC->cr = CFactor.cr * Other->cr + Local->ca;
	pC->cg = CFactor.cg * Other->cg + Local->ca;
	pC->cb = CFactor.cb * Other->cb + Local->ca;
	pC2->ar = pC2->ag = pC2->ab = Local->aa;
	pC2->br = pC2->bg = pC2->bb = Local->ba;
	pC2->cr = pC2->cg = pC2->cb = Local->ca;
	SecondPass = true;
}

void ColorFunctionScaleOtherMinusLocal( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	ColorFactor3Func( &CFactor, Local, Other );
	pC->ar = CFactor.ar * (Other->ar - Local->ar);
	pC->ag = CFactor.ag * (Other->ag - Local->ag);
	pC->ab = CFactor.ab * (Other->ab - Local->ab);
	pC->br = CFactor.br * (Other->br - Local->br);
	pC->bg = CFactor.bg * (Other->bg - Local->bg);
	pC->bb = CFactor.bb * (Other->bb - Local->bb);
	pC->cr = CFactor.cr * (Other->cr - Local->cr);
	pC->cg = CFactor.cg * (Other->cg - Local->cg);
	pC->cb = CFactor.cb * (Other->cb - Local->cb);
}

void ColorFunctionScaleOtherMinusLocalAddLocal( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	if ((( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
		( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB )) &&
		(  Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
	{
		pC->ar = Local->ar;
		pC->ag = Local->ag;
		pC->ab = Local->ab;
		pC->br = Local->br;
		pC->bg = Local->bg;
		pC->bb = Local->bb;
		pC->cr = Local->cr;
		pC->cg = Local->cg;
		pC->cb = Local->cb;
	}
	else
	{
		ColorFactor3Func( &CFactor, Local, Other );
		pC->ar = CFactor.ar * (Other->ar - Local->ar) + Local->ar;
		pC->ag = CFactor.ag * (Other->ag - Local->ag) + Local->ag;
		pC->ab = CFactor.ab * (Other->ab - Local->ab) + Local->ab;
		pC->br = CFactor.br * (Other->br - Local->br) + Local->br;
		pC->bg = CFactor.bg * (Other->bg - Local->bg) + Local->bg;
		pC->bb = CFactor.bb * (Other->bb - Local->bb) + Local->bb;
		pC->cr = CFactor.cr * (Other->cr - Local->cr) + Local->cr;
		pC->cg = CFactor.cg * (Other->cg - Local->cg) + Local->cg;
		pC->cb = CFactor.cb * (Other->cb - Local->cb) + Local->cb;
		pC2->ar = Local->ar;
		pC2->ag = Local->ag;
		pC2->ab = Local->ab;
		pC2->br = Local->br;
		pC2->bg = Local->bg;
		pC2->bb = Local->bb;
		pC2->cr = Local->cr;
		pC2->cg = Local->cg;
		pC2->cb = Local->cb;
		SecondPass = true;
	}
}

void ColorFunctionScaleOtherMinusLocalAddLocalAlpha( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	if ((( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
		( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB )) &&
		(  Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
	{
		pC->ar = pC->ag = pC->ab = Local->aa;
		pC->br = pC->bg = pC->bb = Local->ba;
		pC->cr = pC->cg = pC->cb = Local->ca;
	}
	else
	{
		ColorFactor3Func( &CFactor, Local, Other );
		pC->ar = CFactor.ar * (Other->ar - Local->ar) + Local->aa;
		pC->ag = CFactor.ag * (Other->ag - Local->ag) + Local->aa;
		pC->ab = CFactor.ab * (Other->ab - Local->ab) + Local->aa;
		pC->br = CFactor.br * (Other->br - Local->br) + Local->ba;
		pC->bg = CFactor.bg * (Other->bg - Local->bg) + Local->ba;
		pC->bb = CFactor.bb * (Other->bb - Local->bb) + Local->ba;
		pC->cr = CFactor.cr * (Other->cr - Local->cr) + Local->ca;
		pC->cg = CFactor.cg * (Other->cg - Local->cg) + Local->ca;
		pC->cb = CFactor.cb * (Other->cb - Local->cb) + Local->ca;
		pC2->ar = pC2->ag = pC2->ab = Local->aa;
		pC2->br = pC2->bg = pC2->bb = Local->ba;
		pC2->cr = pC2->cg = pC2->cb = Local->ca;
		SecondPass = true;
	}
}

void ColorFunctionMinusLocalAddLocal( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	ColorFactor3Func( &CFactor, Local, Other );
	pC->ar = ( 1.0f - CFactor.ar ) * Local->ar;
	pC->ag = ( 1.0f - CFactor.ag ) * Local->ag;
	pC->ab = ( 1.0f - CFactor.ab ) * Local->ab;
	pC->br = ( 1.0f - CFactor.br ) * Local->br;
	pC->bg = ( 1.0f - CFactor.bg ) * Local->bg;
	pC->bb = ( 1.0f - CFactor.bb ) * Local->bb;
	pC->cr = ( 1.0f - CFactor.cr ) * Local->cr;
	pC->cg = ( 1.0f - CFactor.cg ) * Local->cg;
	pC->cb = ( 1.0f - CFactor.cb ) * Local->cb;
	pC2->ar = Local->ar;
	pC2->ag = Local->ag;
	pC2->ab = Local->ab;
	pC2->br = Local->br;
	pC2->bg = Local->bg;
	pC2->bb = Local->bb;
	pC2->cr = Local->cr;
	pC2->cg = Local->cg;
	pC2->cb = Local->cb;
	SecondPass = true;
}

void ColorFunctionMinusLocalAddLocalAlpha( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	ColorFactor3Func( &CFactor, Local, Other );
	pC->ar = CFactor.ar * -Local->ar + Local->aa;
	pC->ag = CFactor.ag * -Local->ag + Local->aa;
	pC->ab = CFactor.ab * -Local->ab + Local->aa;
	pC->br = CFactor.br * -Local->br + Local->ba;
	pC->bg = CFactor.bg * -Local->bg + Local->ba;
	pC->bb = CFactor.bb * -Local->bb + Local->ba;
	pC->cr = CFactor.cr * -Local->cr + Local->ca;
	pC->cg = CFactor.cg * -Local->cg + Local->ca;
	pC->cb = CFactor.cb * -Local->cb + Local->ca;
	pC2->ar = pC2->ag = pC2->ab = Local->aa;
	pC2->br = pC2->bg = pC2->bb = Local->ba;
	pC2->cr = pC2->cg = pC2->cb = Local->ca;
	SecondPass = true;
}

inline void ColorFunction( TColorStruct * pC, TColorStruct * pC2, TColorStruct Local, TColorStruct Other )
{
	static TColorStruct CFactor;
	switch(Glide.State.ColorCombineFunction)
	{
	case GR_COMBINE_FUNCTION_ZERO:
		pC->ar = pC->ag = pC->ab = 0.0f; 
		pC->br = pC->bg = pC->bb = 0.0f; 
		pC->cr = pC->cg = pC->cb = 0.0f; 
		break;
	case GR_COMBINE_FUNCTION_LOCAL:
		pC->ar = Local.ar;
		pC->ag = Local.ag;
		pC->ab = Local.ab;
		pC->br = Local.br;
		pC->bg = Local.bg;
		pC->bb = Local.bb;
		pC->cr = Local.cr;
		pC->cg = Local.cg;
		pC->cb = Local.cb;
		break;
	case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
		pC->ar = pC->ag = pC->ab = Local.aa;
		pC->br = pC->bg = pC->bb = Local.ba;
		pC->cr = pC->cg = pC->cb = Local.ca;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar;
		pC->ag = CFactor.ag * Other.ag;
		pC->ab = CFactor.ab * Other.ab;
		pC->br = CFactor.br * Other.br;
		pC->bg = CFactor.bg * Other.bg;
		pC->bb = CFactor.bb * Other.bb;
		pC->cr = CFactor.cr * Other.cr;
		pC->cg = CFactor.cg * Other.cg;
		pC->cb = CFactor.cb * Other.cb;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar + Local.ar;
		pC->ag = CFactor.ag * Other.ag + Local.ag;
		pC->ab = CFactor.ab * Other.ab + Local.ab;
		pC->br = CFactor.br * Other.br + Local.br;
		pC->bg = CFactor.bg * Other.bg + Local.bg;
		pC->bb = CFactor.bb * Other.bb + Local.bb;
		pC->cr = CFactor.cr * Other.cr + Local.cr;
		pC->cg = CFactor.cg * Other.cg + Local.cg;
		pC->cb = CFactor.cb * Other.cb + Local.cb;
		pC2->ar = Local.ar;
		pC2->ag = Local.ag;
		pC2->ab = Local.ab;
		pC2->br = Local.br;
		pC2->bg = Local.bg;
		pC2->bb = Local.bb;
		pC2->cr = Local.cr;
		pC2->cg = Local.cg;
		pC2->cb = Local.cb;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar + Local.aa;
		pC->ag = CFactor.ag * Other.ag + Local.aa;
		pC->ab = CFactor.ab * Other.ab + Local.aa;
		pC->br = CFactor.br * Other.br + Local.ba;
		pC->bg = CFactor.bg * Other.bg + Local.ba;
		pC->bb = CFactor.bb * Other.bb + Local.ba;
		pC->cr = CFactor.cr * Other.cr + Local.ca;
		pC->cg = CFactor.cg * Other.cg + Local.ca;
		pC->cb = CFactor.cb * Other.cb + Local.ca;
		pC2->ar = pC2->ag = pC2->ab = Local.aa;
		pC2->br = pC2->bg = pC2->bb = Local.ba;
		pC2->cr = pC2->cg = pC2->cb = Local.ca;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * (Other.ar - Local.ar);
		pC->ag = CFactor.ag * (Other.ag - Local.ag);
		pC->ab = CFactor.ab * (Other.ab - Local.ab);
		pC->br = CFactor.br * (Other.br - Local.br);
		pC->bg = CFactor.bg * (Other.bg - Local.bg);
		pC->bb = CFactor.bb * (Other.bb - Local.bb);
		pC->cr = CFactor.cr * (Other.cr - Local.cr);
		pC->cg = CFactor.cg * (Other.cg - Local.cg);
		pC->cb = CFactor.cb * (Other.cb - Local.cb);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
		if ((( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
			( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB )) &&
			(  Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
		{
			pC->ar = Local.ar;
			pC->ag = Local.ag;
			pC->ab = Local.ab;
			pC->br = Local.br;
			pC->bg = Local.bg;
			pC->bb = Local.bb;
			pC->cr = Local.cr;
			pC->cg = Local.cg;
			pC->cb = Local.cb;
		}
		else
		{
			ColorFactor3Func( &CFactor, &Local, &Other );
			pC->ar = CFactor.ar * (Other.ar - Local.ar) + Local.ar;
			pC->ag = CFactor.ag * (Other.ag - Local.ag) + Local.ag;
			pC->ab = CFactor.ab * (Other.ab - Local.ab) + Local.ab;
			pC->br = CFactor.br * (Other.br - Local.br) + Local.br;
			pC->bg = CFactor.bg * (Other.bg - Local.bg) + Local.bg;
			pC->bb = CFactor.bb * (Other.bb - Local.bb) + Local.bb;
			pC->cr = CFactor.cr * (Other.cr - Local.cr) + Local.cr;
			pC->cg = CFactor.cg * (Other.cg - Local.cg) + Local.cg;
			pC->cb = CFactor.cb * (Other.cb - Local.cb) + Local.cb;
			pC2->ar = Local.ar;
			pC2->ag = Local.ag;
			pC2->ab = Local.ab;
			pC2->br = Local.br;
			pC2->bg = Local.bg;
			pC2->bb = Local.bb;
			pC2->cr = Local.cr;
			pC2->cg = Local.cg;
			pC2->cb = Local.cb;
			SecondPass = true;
		}
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		if ((( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
			( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB )) &&
			(  Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
		{
			pC->ar = pC->ag = pC->ab = Local.aa;
			pC->br = pC->bg = pC->bb = Local.ba;
			pC->cr = pC->cg = pC->cb = Local.ca;
		}
		else
		{
			ColorFactor3Func( &CFactor, &Local, &Other );
			pC->ar = CFactor.ar * (Other.ar - Local.ar) + Local.aa;
			pC->ag = CFactor.ag * (Other.ag - Local.ag) + Local.aa;
			pC->ab = CFactor.ab * (Other.ab - Local.ab) + Local.aa;
			pC->br = CFactor.br * (Other.br - Local.br) + Local.ba;
			pC->bg = CFactor.bg * (Other.bg - Local.bg) + Local.ba;
			pC->bb = CFactor.bb * (Other.bb - Local.bb) + Local.ba;
			pC->cr = CFactor.cr * (Other.cr - Local.cr) + Local.ca;
			pC->cg = CFactor.cg * (Other.cg - Local.cg) + Local.ca;
			pC->cb = CFactor.cb * (Other.cb - Local.cb) + Local.ca;
			pC2->ar = pC2->ag = pC2->ab = Local.aa;
			pC2->br = pC2->bg = pC2->bb = Local.ba;
			pC2->cr = pC2->cg = pC2->cb = Local.ca;
			SecondPass = true;
		}
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = ( 1.0f - CFactor.ar ) * Local.ar;
		pC->ag = ( 1.0f - CFactor.ag ) * Local.ag;
		pC->ab = ( 1.0f - CFactor.ab ) * Local.ab;
		pC->br = ( 1.0f - CFactor.br ) * Local.br;
		pC->bg = ( 1.0f - CFactor.bg ) * Local.bg;
		pC->bb = ( 1.0f - CFactor.bb ) * Local.bb;
		pC->cr = ( 1.0f - CFactor.cr ) * Local.cr;
		pC->cg = ( 1.0f - CFactor.cg ) * Local.cg;
		pC->cb = ( 1.0f - CFactor.cb ) * Local.cb;
		pC2->ar = Local.ar;
		pC2->ag = Local.ag;
		pC2->ab = Local.ab;
		pC2->br = Local.br;
		pC2->bg = Local.bg;
		pC2->bb = Local.bb;
		pC2->cr = Local.cr;
		pC2->cg = Local.cg;
		pC2->cb = Local.cb;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * -Local.ar + Local.aa;
		pC->ag = CFactor.ag * -Local.ag + Local.aa;
		pC->ab = CFactor.ab * -Local.ab + Local.aa;
		pC->br = CFactor.br * -Local.br + Local.ba;
		pC->bg = CFactor.bg * -Local.bg + Local.ba;
		pC->bb = CFactor.bb * -Local.bb + Local.ba;
		pC->cr = CFactor.cr * -Local.cr + Local.ca;
		pC->cg = CFactor.cg * -Local.cg + Local.ca;
		pC->cb = CFactor.cb * -Local.cb + Local.ca;
		pC2->ar = pC2->ag = pC2->ab = Local.aa;
		pC2->br = pC2->bg = pC2->bb = Local.ba;
		pC2->cr = pC2->cg = pC2->cb = Local.ca;
		SecondPass = true;
		break;
	}
}

void RenderInitialize()
{
	OGLRender.NumberOfTriangles = 0;

#ifdef DEBUG
	OGLRender.FrameTriangles = 0;
	OGLRender.MaxTriangles = 0;
	OGLRender.MaxSequencedTriangles = 0;
	OGLRender.MinX = OGLRender.MinY = OGLRender.MinZ = OGLRender.MinW = 99999999.0f;
	OGLRender.MaxX = OGLRender.MaxY = OGLRender.MaxZ = OGLRender.MaxW = -99999999.0f;
	OGLRender.MinS = OGLRender.MinT = OGLRender.MinF = 99999999.0f;
	OGLRender.MaxS = OGLRender.MaxT = OGLRender.MaxF = -99999999.0f;

	OGLRender.MinR = OGLRender.MinG = OGLRender.MinB = OGLRender.MinA = 99999999.0f;
	OGLRender.MaxR = OGLRender.MaxG = OGLRender.MaxB = OGLRender.MaxA = -99999999.0f;
#endif

	OGLRender.TColor = new TColorStruct[MAXTRIANGLES+1]; // One more for Lines and Point
	OGLRender.TColor2 = new TColorStruct[MAXTRIANGLES+1];
	OGLRender.TTexture = new TTextureStruct[MAXTRIANGLES+1];
	OGLRender.TVertex = new TVertexStruct[MAXTRIANGLES+1];
	OGLRender.TFog = new TFogStruct[MAXTRIANGLES+1];

	ConstantForAnd = new float[ 2 ];
	ConstantForMultiply = new float[ 2 ];
	ConstantForSubtract = new float[ 2 ];

	ConstantForAnd[ 0 ] = 0xFFFFF;
	ConstantForAnd[ 1 ] = 0xFFFFF;

	ConstantForMultiply[ 0 ] = D1OVER65536;
	ConstantForMultiply[ 1 ] = D1OVER65536;

	ConstantForSubtract[ 0 ] = 8.9375f;
	ConstantForSubtract[ 1 ] = 8.9375f;
}

//Render::~Render()
void RenderFree()
{
	delete[] OGLRender.TColor;
	delete[] OGLRender.TColor2;
	delete[] OGLRender.TTexture;
	delete[] OGLRender.TVertex;
	delete[] OGLRender.TFog;
	delete[] ConstantForAnd;
	delete[] ConstantForMultiply;
	delete[] ConstantForSubtract;
}

//void Render::UpdateArrays()
void RenderUpdateArrays()
{
	glVertexPointer( 3, GL_FLOAT, 4 * sizeof( GLfloat ), &OGLRender.TVertex[0] );
#ifdef OPENGL_DEBUG
	GLErro( "Render::UpdateArrays - Vertex" );
#endif
	glColorPointer( 4, GL_FLOAT, 0, &OGLRender.TColor[0] );
#ifdef OPENGL_DEBUG
	GLErro( "Render::UpdateArrays - Color" );
#endif
	glTexCoordPointer( 4, GL_FLOAT, 0, &OGLRender.TTexture[0] );
#ifdef OPENGL_DEBUG
	GLErro( "Render::UpdateArrays - TexCoord" );
#endif
	if ( InternalConfig.SecondaryColorEXTEnable )
	{
		glSecondaryColorPointerEXT( 3, GL_FLOAT, 4 * sizeof( GLfloat ), &OGLRender.TColor2[0] );
#ifdef OPENGL_DEBUG
		GLErro( "Render::UpdateArrays - Secondary Color" );
#endif
	}
	if ( InternalConfig.FogCoordEXTEnable )
	{
//		glFogCoordPointerEXT( 1, GL_FLOAT, 0, &OGLRender.TFog[0] );
		glFogCoordPointerEXT( 1, GL_FLOAT, &OGLRender.TFog[0] );
#ifdef OPENGL_DEBUG
		GLErro( "Render::UpdateArrays - FogCoord" );
#endif
	}

#ifdef OPENGL_DEBUG
	GLErro( "Render::UpdateArrays" );
#endif
}


//void Render::DrawTriangles()
void RenderDrawTriangles()
{
	static int i;
	static DWORD Pixels;
	static BYTE *Buffer1,
				*Buffer2;
	static GLuint TNumber;

	if ( !OGLRender.NumberOfTriangles )
		return;

	ProfileBegin( "DrawTriangles" );

	if ( OpenGL.Texture )
	{
		glEnable( GL_TEXTURE_2D );

        Textures->MakeReady();
	}
	else
	{
		glDisable( GL_TEXTURE_2D );
	}

	if (OpenGL.Blend )
	{
		glEnable( GL_BLEND );
	}
	else
	{
		glDisable( GL_BLEND );
	}

	// Alpha Fix
	if (Glide.State.AlphaOther != GR_COMBINE_OTHER_TEXTURE)
	{
		glDisable( GL_ALPHA_TEST );
	}
	else 
	{
		if (Glide.State.AlphaTestFunction != GR_CMP_ALWAYS)
		{
			glEnable( GL_ALPHA_TEST );
		}
	}
	
    if(!OpenGL.Blend && Glide.State.ChromaKeyMode)
    {
        glAlphaFunc(GL_GEQUAL, 0.5);
        glEnable(GL_ALPHA_TEST);
        
        glBegin( GL_TRIANGLES );
        for ( i = 0; i < OGLRender.NumberOfTriangles; i++ )
        {
            glColor3fv( &OGLRender.TColor[i].ar );
            glSecondaryColor3fvEXT( &OGLRender.TColor2[i].ar );
            glFogCoordfEXT( OGLRender.TFog[i].af );
            glTexCoord4fv( &OGLRender.TTexture[i].as );
            glVertex3fv( &OGLRender.TVertex[i].ax );
            
            glColor3fv( &OGLRender.TColor[i].br );
            glSecondaryColor3fvEXT( &OGLRender.TColor2[i].br );
            glFogCoordfEXT( OGLRender.TFog[i].bf );
            glTexCoord4fv( &OGLRender.TTexture[i].bs );
            glVertex3fv( &OGLRender.TVertex[i].bx );
            
            glColor3fv( &OGLRender.TColor[i].cr );
            glSecondaryColor3fvEXT( &OGLRender.TColor2[i].cr );
            glFogCoordfEXT( OGLRender.TFog[i].cf );
            glTexCoord4fv( &OGLRender.TTexture[i].cs );
            glVertex3fv( &OGLRender.TVertex[i].cx );
        }
        glEnd();
        
        glDisable(GL_ALPHA_TEST);
    }
    else
    {
        if (InternalConfig.VertexArrayEXTEnable )
        {
            glDrawArrays( GL_TRIANGLES, 0, OGLRender.NumberOfTriangles * 3 );
        }
        else
        {
            glBegin( GL_TRIANGLES );
            for ( i = 0; i < OGLRender.NumberOfTriangles; i++ )
            {
                glColor4fv( &OGLRender.TColor[i].ar );
                glSecondaryColor3fvEXT( &OGLRender.TColor2[i].ar );
                glFogCoordfEXT( OGLRender.TFog[i].af );
                glTexCoord4fv( &OGLRender.TTexture[i].as );
                glVertex3fv( &OGLRender.TVertex[i].ax );
                
                glColor4fv( &OGLRender.TColor[i].br );
                glSecondaryColor3fvEXT( &OGLRender.TColor2[i].br );
                glFogCoordfEXT( OGLRender.TFog[i].bf );
                glTexCoord4fv( &OGLRender.TTexture[i].bs );
                glVertex3fv( &OGLRender.TVertex[i].bx );
                
                glColor4fv( &OGLRender.TColor[i].cr );
                glSecondaryColor3fvEXT( &OGLRender.TColor2[i].cr );
                glFogCoordfEXT( OGLRender.TFog[i].cf );
                glTexCoord4fv( &OGLRender.TTexture[i].cs );
                glVertex3fv( &OGLRender.TVertex[i].cx );
            }
            glEnd();
        }
    }


	if ( ( SecondPass ) && ( !InternalConfig.SecondaryColorEXTEnable ) )
	{
		glBlendFunc( GL_ONE, GL_ONE );
		glEnable( GL_BLEND );
		glDisable( GL_TEXTURE_2D );

		if ( OpenGL.DepthBufferType )
		{
			glPolygonOffset( 1.0f, 0.5f );
		}
		else
		{
			glPolygonOffset( -1.0f, -0.5f );
		}

		glEnable( GL_POLYGON_OFFSET_FILL );

		if (0 &&  InternalConfig.VertexArrayEXTEnable )
		{
			glColorPointer( 4, GL_FLOAT, 0, &OGLRender.TColor2 );
			glDrawArrays( GL_TRIANGLES, 0, OGLRender.NumberOfTriangles * 3 );
			glColorPointer( 4, GL_FLOAT, 0, &OGLRender.TColor );
		}
		else
		{
			glBegin( GL_TRIANGLES );
			for ( i = 0; i < OGLRender.NumberOfTriangles; i++ )
			{
				glColor4fv( &OGLRender.TColor2[i].ar );
				glVertex3fv( &OGLRender.TVertex[i].ax );

				glColor4fv( &OGLRender.TColor2[i].br );
				glVertex3fv( &OGLRender.TVertex[i].bx );

				glColor4fv( &OGLRender.TColor2[i].cr );
				glVertex3fv( &OGLRender.TVertex[i].cx );
			}
			glEnd();
		}

		if ( Glide.State.DepthBiasLevel )
		{
			glPolygonOffset( 1.0f, OpenGL.DepthBiasLevel );
		}
		else
		{
			glDisable( GL_POLYGON_OFFSET_FILL );
		}

		if ( OpenGL.Blend )
		{
			glBlendFunc( OpenGL.SrcBlend, OpenGL.DstBlend );
		}
	}

#ifdef DEBUG
	if ( OGLRender.NumberOfTriangles > OGLRender.MaxSequencedTriangles )
		OGLRender.MaxSequencedTriangles = OGLRender.NumberOfTriangles;
#endif

	OGLRender.NumberOfTriangles = 0;

#ifdef OPENGL_DEBUG
	GLErro( "Render::DrawTriangles" );
#endif

	ProfileEnd( "DrawTriangles" );

}


//void Render::AddTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )
void RenderAddTriangle( const GrVertex *a, const GrVertex *b, const GrVertex *c )
{
	static TColorStruct Local, Other, CFactor;
	static float AFactor[3];
	static TColorStruct *pC, *pC2;
	static TVertexStruct *pV;
	static TTextureStruct *pTS;
	static TFogStruct *pF;
	static void *pt1, *pt2, *pt3;
    float  atmuoow;
    float  btmuoow;
    float  ctmuoow;

	ProfileBegin( "RenderAddTriangle" );

	ProfileBegin( "Init" );

	pC = &OGLRender.TColor[OGLRender.NumberOfTriangles];
	pC2 = &OGLRender.TColor2[OGLRender.NumberOfTriangles];
	pV = &OGLRender.TVertex[OGLRender.NumberOfTriangles];
	pTS = &OGLRender.TTexture[OGLRender.NumberOfTriangles];

	// Color Stuff, need to optimize it
	ZeroMemory( pC2, sizeof( TColorStruct ) );
	SecondPass	= false;

    if((Glide.State.STWHint & GR_STWHINT_W_DIFF_TMU0) == 0)
    {
        atmuoow = a->oow;
        btmuoow = b->oow;
        ctmuoow = c->oow;
    }
    else
    {
        atmuoow = a->tmuvtx[0].oow;
        btmuoow = b->tmuvtx[0].oow;
        ctmuoow = c->tmuvtx[0].oow;
    }

	if ( Glide.ALocal )
	{
		switch ( Glide.State.AlphaLocal )
		{
		case GR_COMBINE_LOCAL_ITERATED:
			Local.aa = a->a * D1OVER255;
			Local.ba = b->a * D1OVER255;
			Local.ca = c->a * D1OVER255;
 			break;
		case GR_COMBINE_LOCAL_CONSTANT:
			Local.aa = Local.ba = Local.ca = OpenGL.ConstantColor[3];
			break;
		case GR_COMBINE_LOCAL_DEPTH:
			Local.aa = a->z;
			Local.ba = b->z;
			Local.ca = c->z;
			break;
		}
	}

	if ( Glide.AOther )
	{
		switch ( Glide.State.AlphaOther )
		{
		case GR_COMBINE_OTHER_ITERATED:
			Other.aa = a->a * D1OVER255;
			Other.ba = b->a * D1OVER255;
			Other.ca = c->a * D1OVER255;
			break;
		case GR_COMBINE_OTHER_CONSTANT:
			Other.aa = Other.ba = Other.ca = OpenGL.ConstantColor[3];
			break;
		case GR_COMBINE_OTHER_TEXTURE:
			Other.aa = Other.ba = Other.ca = 1.0f;
			break;
		}
	}

	if ( Glide.CLocal )
	{
		switch ( Glide.State.ColorCombineLocal )
		{
		case GR_COMBINE_LOCAL_ITERATED:
			Local.ar = a->r * D1OVER255;
			Local.ag = a->g * D1OVER255;
			Local.ab = a->b * D1OVER255;
			Local.br = b->r * D1OVER255;
			Local.bg = b->g * D1OVER255;
			Local.bb = b->b * D1OVER255;
			Local.cr = c->r * D1OVER255;
			Local.cg = c->g * D1OVER255;
			Local.cb = c->b * D1OVER255;
			break;
		case GR_COMBINE_LOCAL_CONSTANT:
			Local.ar = Local.br = Local.cr = OpenGL.ConstantColor[0];
			Local.ag = Local.bg = Local.cg = OpenGL.ConstantColor[1];
			Local.ab = Local.bb = Local.cb = OpenGL.ConstantColor[2];
			break;
		}
	}

	if ( Glide.COther )
	{
		switch ( Glide.State.ColorCombineOther )
		{
		case GR_COMBINE_OTHER_ITERATED:
			Other.ar = a->r * D1OVER255;
			Other.ag = a->g * D1OVER255;
			Other.ab = a->b * D1OVER255;
			Other.br = b->r * D1OVER255;
			Other.bg = b->g * D1OVER255;
			Other.bb = b->b * D1OVER255;
			Other.cr = c->r * D1OVER255;
			Other.cg = c->g * D1OVER255;
			Other.cb = c->b * D1OVER255;
			break;
		case GR_COMBINE_OTHER_CONSTANT:
			Other.ar = Other.br = Other.cr = OpenGL.ConstantColor[0];
			Other.ag = Other.bg = Other.cg = OpenGL.ConstantColor[1];
			Other.ab = Other.bb = Other.cb = OpenGL.ConstantColor[2];
			break;
		case GR_COMBINE_OTHER_TEXTURE:
			Other.ar = Other.ag = Other.ab = 1.0f;
			Other.br = Other.bg = Other.bb = 1.0f;
			Other.cr = Other.cg = Other.cb = 1.0f;
			break;
		}
	}
	ProfileEnd( "Init" );

	ProfileBegin( "ColorFunc" );

	ColorFunctionFunc( pC, pC2, &Local, &Other );

	ProfileEnd( "ColorFunc" );

	ProfileBegin( "AlphaFunc" );

	pC2->aa = 0.0f;
	pC2->ba = 0.0f;
	pC2->ca = 0.0f;

	switch ( Glide.State.AlphaFunction )
	{
	case GR_COMBINE_FUNCTION_ZERO:
		pC->aa = pC->ba = pC->ca = 0.0f;
		break;
	case GR_COMBINE_FUNCTION_LOCAL:
	case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
		pC->aa = Local.aa;
		pC->ba = Local.ba;
		pC->ca = Local.ca;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		pC->aa = ((1.0f - AlphaFactorFunc( Local.aa, Other.aa )) * Local.aa);
		pC->ba = ((1.0f - AlphaFactorFunc( Local.ba, Other.ba )) * Local.ba);
		pC->ca = ((1.0f - AlphaFactorFunc( Local.ca, Other.ca )) * Local.ca);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * Other.aa);
		pC->ba = (AlphaFactorFunc( Local.ba, Other.ba ) * Other.ba);
		pC->ca = (AlphaFactorFunc( Local.ca, Other.ca ) * Other.ca);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * Other.aa + Local.aa);
		pC->ba = (AlphaFactorFunc( Local.ba, Other.ba ) * Other.ba + Local.ba);
		pC->ca = (AlphaFactorFunc( Local.ca, Other.ca ) * Other.ca + Local.ca);
//		pC2->aa =  Local.aa;
//		pC2->ba =  Local.ba;
//		pC2->ca =  Local.ca;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * ( Other.aa - Local.aa ));
		pC->ba = (AlphaFactorFunc( Local.ba, Other.ba ) * ( Other.ba - Local.ba ));
		pC->ca = (AlphaFactorFunc( Local.ca, Other.ca ) * ( Other.ca - Local.ca ));
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * ( Other.aa - Local.aa ) + Local.aa);
		pC->ba = (AlphaFactorFunc( Local.ba, Other.ba ) * ( Other.ba - Local.ba ) + Local.ba);
		pC->ca = (AlphaFactorFunc( Local.ca, Other.ca ) * ( Other.ca - Local.ca ) + Local.ca);
//		pC2->aa =  Local.ba;
//		pC2->ba =  Local.ba;
//		pC2->ca =  Local.ca;
		break;
	}
	ProfileEnd( "AlphaFunc" );

	ProfileBegin( "Other" );
	if ( Glide.State.ColorCombineInvert )
	{
		pC->ar = 1.0f - pC->ar;
		pC->ag = 1.0f - pC->ag;
		pC->ab = 1.0f - pC->ab;
		pC->br = 1.0f - pC->br;
		pC->bg = 1.0f - pC->bg;
		pC->bb = 1.0f - pC->bb;
		pC->cr = 1.0f - pC->cr;
		pC->cg = 1.0f - pC->cg;
		pC->cb = 1.0f - pC->cb;
		if ( SecondPass )
		{
			pC2->ar = 1.0f - pC2->ar;
			pC2->ag = 1.0f - pC2->ag;
			pC2->ab = 1.0f - pC2->ab;
			pC2->br = 1.0f - pC2->br;
			pC2->bg = 1.0f - pC2->bg;
			pC2->bb = 1.0f - pC2->bb;
			pC2->cr = 1.0f - pC2->cr;
			pC2->cg = 1.0f - pC2->cg;
			pC2->cb = 1.0f - pC2->cb;
		}
	}

	if ( Glide.State.AlphaInvert )
	{
		pC->aa = 1.0f - pC->aa;
		pC->ba = 1.0f - pC->ba;
		pC->ca = 1.0f - pC->ca;
		if ( SecondPass )
		{
			pC2->aa = 1.0f - pC2->aa;
			pC2->ba = 1.0f - pC2->ba;
			pC2->ca = 1.0f - pC2->ca;
		}
	}
	
	static float w, aoow, boow, coow;

    // Z-Buffering
    if(Glide.State.DepthBufferMode == GR_DEPTHBUFFER_DISABLE || Glide.State.DepthFunction == GR_CMP_ALWAYS)
    {
		pV->az = 0.0f;
		pV->bz = 0.0f;
		pV->cz = 0.0f;
    }
	else if ( OpenGL.DepthBufferType )
	{
		pV->az = a->ooz * D1OVER65536;
		pV->bz = b->ooz * D1OVER65536;
		pV->cz = c->ooz * D1OVER65536;
	}
	else
	{
       /*
        * For silly values of oow, depth buffering doesn't
        * seem to work, so map them to a sensible z.  When
        * games use these silly values, they probably don't
        * use z buffering anyway.
        */
        if( a->oow > 1.0)
        {
            pV->az = pV->bz = pV->cz = 0.9f;
        }
		else if ( InternalConfig.PrecisionFixEnable )
		{
			w = 1.0f / a->oow;
			pV->az = 8.9375f - ((float)( ( (*(DWORD *)&w >> 11) & 0xFFFFF ) * D1OVER65536) );
			w = 1.0f / b->oow;
			pV->bz = 8.9375f - (float(((*(DWORD *)&w >> 11) & 0xFFFFF) * D1OVER65536) );
			w = 1.0f / c->oow;
			pV->cz = 8.9375f - (float(((*(DWORD *)&w >> 11) & 0xFFFFF) * D1OVER65536) );
		}
		else
		{
			pV->az = a->oow;
			pV->bz = b->oow;
			pV->cz = c->oow;
		}
	}

	if (0 && a->x > 2048)
	{
		pV->ax = a->x - vertex_snap;
		pV->ay = a->y - vertex_snap;
		pV->bx = b->x - vertex_snap;
		pV->by = b->y - vertex_snap;
		pV->cx = c->x - vertex_snap;
		pV->cy = c->y - vertex_snap;
	}
	else
	{
		pV->ax = a->x;
		pV->ay = a->y;
		pV->bx = b->x;
		pV->by = b->y;
		pV->cx = c->x;
		pV->cy = c->y;
	}

	if (OpenGL.Texture)
	{
        float maxoow = max(atmuoow, max(btmuoow, ctmuoow));
        float hAspect, wAspect;

        Textures->GetAspect(&hAspect, &wAspect);

		pTS->as = a->tmuvtx[0].sow * wAspect / maxoow;
		pTS->at = a->tmuvtx[0].tow * hAspect / maxoow;
		pTS->bs = b->tmuvtx[0].sow * wAspect / maxoow;
		pTS->bt = b->tmuvtx[0].tow * hAspect / maxoow;
		pTS->cs = c->tmuvtx[0].sow * wAspect / maxoow;
		pTS->ct = c->tmuvtx[0].tow * hAspect / maxoow;

		pTS->aq = pTS->bq = pTS->cq = 0.0f;
		pTS->aoow = atmuoow / maxoow;
		pTS->boow = btmuoow / maxoow;
		pTS->coow = ctmuoow / maxoow;
	}

	if( InternalConfig.FogEnable )
	{
		pF = &OGLRender.TFog[OGLRender.NumberOfTriangles];
		if ( Glide.State.FogMode == GR_FOG_WITH_TABLE )
		{
            pF->af = (float)OpenGL.FogTable[ (WORD)(1.0f / a->oow) ] * D1OVER255;
			pF->bf = (float)OpenGL.FogTable[ (WORD)(1.0f / b->oow) ] * D1OVER255;
			pF->cf = (float)OpenGL.FogTable[ (WORD)(1.0f / c->oow) ] * D1OVER255;
		}
		else
		{
			pF->af = a->a * D1OVER255;
			pF->bf = b->a * D1OVER255;
			pF->cf = c->a * D1OVER255;
		}
	#ifdef DEBUG
			if ( pF->af > OGLRender.MaxF ) OGLRender.MaxF = pF->af;
			if ( pF->bf > OGLRender.MaxF ) OGLRender.MaxF = pF->bf;
			if ( pF->cf > OGLRender.MaxF ) OGLRender.MaxF = pF->cf;
			if ( pF->af < OGLRender.MinF ) OGLRender.MinF = pF->af;
			if ( pF->bf < OGLRender.MinF ) OGLRender.MinF = pF->bf;
			if ( pF->cf < OGLRender.MinF ) OGLRender.MinF = pF->cf;
	#endif
	}

#ifdef DEBUG
		if ( pC->ar > OGLRender.MaxR ) OGLRender.MaxR = pC->ar;
		if ( pC->br > OGLRender.MaxR ) OGLRender.MaxR = pC->br;
		if ( pC->cr > OGLRender.MaxR ) OGLRender.MaxR = pC->cr;
		if ( pC->ar < OGLRender.MinR ) OGLRender.MinR = pC->ar;
		if ( pC->br < OGLRender.MinR ) OGLRender.MinR = pC->br;
		if ( pC->cr < OGLRender.MinR ) OGLRender.MinR = pC->cr;
		
		if ( pC->ag > OGLRender.MaxG ) OGLRender.MaxG = pC->ag;
		if ( pC->bg > OGLRender.MaxG ) OGLRender.MaxG = pC->bg;
		if ( pC->cg > OGLRender.MaxG ) OGLRender.MaxG = pC->cg;
		if ( pC->ag < OGLRender.MinG ) OGLRender.MinG = pC->ag;
		if ( pC->bg < OGLRender.MinG ) OGLRender.MinG = pC->bg;
		if ( pC->cg < OGLRender.MinG ) OGLRender.MinG = pC->cg;

		if ( pC->ab > OGLRender.MaxB ) OGLRender.MaxB = pC->ab;
		if ( pC->bb > OGLRender.MaxB ) OGLRender.MaxB = pC->bb;
		if ( pC->cb > OGLRender.MaxB ) OGLRender.MaxB = pC->cb;
		if ( pC->ab < OGLRender.MinB ) OGLRender.MinB = pC->ab;
		if ( pC->bb < OGLRender.MinB ) OGLRender.MinB = pC->bb;
		if ( pC->cb < OGLRender.MinB ) OGLRender.MinB = pC->cb;

		if ( pC->aa > OGLRender.MaxA ) OGLRender.MaxA = pC->aa;
		if ( pC->ba > OGLRender.MaxA ) OGLRender.MaxA = pC->ba;
		if ( pC->ca > OGLRender.MaxA ) OGLRender.MaxA = pC->ca;
		if ( pC->aa < OGLRender.MinA ) OGLRender.MinA = pC->aa;
		if ( pC->ba < OGLRender.MinA ) OGLRender.MinA = pC->ba;
		if ( pC->ca < OGLRender.MinA ) OGLRender.MinA = pC->ca;

		if ( pV->az > OGLRender.MaxZ ) OGLRender.MaxZ = pV->az;
		if ( pV->bz > OGLRender.MaxZ ) OGLRender.MaxZ = pV->bz;
		if ( pV->cz > OGLRender.MaxZ ) OGLRender.MaxZ = pV->cz;
		if ( pV->az < OGLRender.MinZ ) OGLRender.MinZ = pV->az;
		if ( pV->bz < OGLRender.MinZ ) OGLRender.MinZ = pV->bz;
		if ( pV->cz < OGLRender.MinZ ) OGLRender.MinZ = pV->cz;

		if ( pV->ax > OGLRender.MaxX ) OGLRender.MaxX = pV->ax;
		if ( pV->bx > OGLRender.MaxX ) OGLRender.MaxX = pV->bx;
		if ( pV->cx > OGLRender.MaxX ) OGLRender.MaxX = pV->cx;
		if ( pV->ax < OGLRender.MinX ) OGLRender.MinX = pV->ax;
		if ( pV->bx < OGLRender.MinX ) OGLRender.MinX = pV->bx;
		if ( pV->cx < OGLRender.MinX ) OGLRender.MinX = pV->cx;

		if ( pV->ay > OGLRender.MaxY ) OGLRender.MaxY = pV->ay;
		if ( pV->by > OGLRender.MaxY ) OGLRender.MaxY = pV->by;
		if ( pV->cy > OGLRender.MaxY ) OGLRender.MaxY = pV->cy;
		if ( pV->ay < OGLRender.MinY ) OGLRender.MinY = pV->ay;
		if ( pV->by < OGLRender.MinY ) OGLRender.MinY = pV->by;
		if ( pV->cy < OGLRender.MinY ) OGLRender.MinY = pV->cy;

		if ( pTS->as > OGLRender.MaxS ) OGLRender.MaxS = pTS->as;
		if ( pTS->bs > OGLRender.MaxS ) OGLRender.MaxS = pTS->bs;
		if ( pTS->cs > OGLRender.MaxS ) OGLRender.MaxS = pTS->cs;
		if ( pTS->as < OGLRender.MinS ) OGLRender.MinS = pTS->as;
		if ( pTS->bs < OGLRender.MinS ) OGLRender.MinS = pTS->bs;
		if ( pTS->cs < OGLRender.MinS ) OGLRender.MinS = pTS->cs;

		if ( pTS->at > OGLRender.MaxT ) OGLRender.MaxT = pTS->at;
		if ( pTS->bt > OGLRender.MaxT ) OGLRender.MaxT = pTS->bt;
		if ( pTS->ct > OGLRender.MaxT ) OGLRender.MaxT = pTS->ct;
		if ( pTS->at < OGLRender.MinT ) OGLRender.MinT = pTS->at;
		if ( pTS->bt < OGLRender.MinT ) OGLRender.MinT = pTS->bt;
		if ( pTS->ct < OGLRender.MinT ) OGLRender.MinT = pTS->ct;
#endif

	
	OGLRender.NumberOfTriangles++;

	if ( OGLRender.NumberOfTriangles >= MAXTRIANGLES-1 )
		RenderDrawTriangles();
#ifdef DEBUG
	OGLRender.FrameTriangles++;
#endif

#ifdef OPENGL_DEBUG
	GLErro( "Render::AddTriangle" );
#endif
	ProfileEnd( "Other" );

	ProfileEnd( "RenderAddTriangle" );
}


//void Render::AddLine( const GrVertex *a, const GrVertex *b )
void RenderAddLine( const GrVertex *a, const GrVertex *b )
{
	static TColorStruct Local, Other, CFactor;
	static float AFactor[2];
	static TColorStruct *pC, *pC2;
	static TVertexStruct *pV;
	static TTextureStruct *pTS;
	static TFogStruct *pF;
	static void *pt1, *pt2, *pt3;
    float  atmuoow;
    float  btmuoow;

	ProfileBegin( "RenderAddLine" );

	pC = &OGLRender.TColor[MAXTRIANGLES];
	pC2 = &OGLRender.TColor2[MAXTRIANGLES];
	pV = &OGLRender.TVertex[MAXTRIANGLES];
	pTS = &OGLRender.TTexture[MAXTRIANGLES];
	pF = &OGLRender.TFog[MAXTRIANGLES];

	// Color Stuff, need to optimize it
	ZeroMemory( pC2, sizeof( TColorStruct ) );
	SecondPass	= false;

    if((Glide.State.STWHint & GR_STWHINT_W_DIFF_TMU0) == 0)
    {
        atmuoow = a->oow;
        btmuoow = b->oow;
    }
    else
    {
        atmuoow = a->tmuvtx[0].oow;
        btmuoow = b->tmuvtx[0].oow;
    }

	if ( Glide.ALocal )
	{
		switch ( Glide.State.AlphaLocal )
		{
		case GR_COMBINE_LOCAL_ITERATED:
			Local.aa = a->a * D1OVER255;
			Local.ba = b->a * D1OVER255;
 			break;
		case GR_COMBINE_LOCAL_CONSTANT:
			Local.aa = Local.ba = OpenGL.ConstantColor[3];
			break;
		case GR_COMBINE_LOCAL_DEPTH:
			Local.aa = a->z;
			Local.ba = b->z;
			break;
		}
	}

	if ( Glide.AOther )
	{
		switch ( Glide.State.AlphaOther )
		{
		case GR_COMBINE_OTHER_ITERATED:
			Other.aa = a->a * D1OVER255;
			Other.ba = b->a * D1OVER255;
			break;
		case GR_COMBINE_OTHER_CONSTANT:
			Other.aa = Other.ba = OpenGL.ConstantColor[3];
			break;
		case GR_COMBINE_OTHER_TEXTURE:
			Other.aa = Other.ba = 1.0f;
			break;
		}
	}

	if ( Glide.CLocal )
	{
		switch ( Glide.State.ColorCombineLocal )
		{
		case GR_COMBINE_LOCAL_ITERATED:
			Local.ar = a->r * D1OVER255;
			Local.ag = a->g * D1OVER255;
			Local.ab = a->b * D1OVER255;
			Local.br = b->r * D1OVER255;
			Local.bg = b->g * D1OVER255;
			Local.bb = b->b * D1OVER255;
			break;
		case GR_COMBINE_LOCAL_CONSTANT:
			Local.ar = Local.br = OpenGL.ConstantColor[0];
			Local.ag = Local.bg = OpenGL.ConstantColor[1];
			Local.ab = Local.bb = OpenGL.ConstantColor[2];
			break;
		}
	}

	if ( Glide.COther )
	{
		switch ( Glide.State.ColorCombineOther )
		{
		case GR_COMBINE_OTHER_ITERATED:
			Other.ar = a->r * D1OVER255;
			Other.ag = a->g * D1OVER255;
			Other.ab = a->b * D1OVER255;
			Other.br = b->r * D1OVER255;
			Other.bg = b->g * D1OVER255;
			Other.bb = b->b * D1OVER255;
			break;
		case GR_COMBINE_OTHER_CONSTANT:
			Other.ar = Other.br = OpenGL.ConstantColor[0];
			Other.ag = Other.bg = OpenGL.ConstantColor[1];
			Other.ab = Other.bb = OpenGL.ConstantColor[2];
			break;
		case GR_COMBINE_OTHER_TEXTURE:
			Other.ar = Other.ag = Other.ab = 1.0f;
			Other.br = Other.bg = Other.bb = 1.0f;
			break;
		}
	}

	switch(Glide.State.ColorCombineFunction)
	{
	case GR_COMBINE_FUNCTION_ZERO:
		pC->ar = pC->ag = pC->ab = 0.0f; 
		pC->br = pC->bg = pC->bb = 0.0f; 
		break;
	case GR_COMBINE_FUNCTION_LOCAL:
		pC->ar = Local.ar;
		pC->ag = Local.ag;
		pC->ab = Local.ab;
		pC->br = Local.br;
		pC->bg = Local.bg;
		pC->bb = Local.bb;
		break;
	case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
		pC->ar = pC->ag = pC->ab = Local.aa;
		pC->br = pC->bg = pC->bb = Local.ba;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar;
		pC->ag = CFactor.ag * Other.ag;
		pC->ab = CFactor.ab * Other.ab;
		pC->br = CFactor.br * Other.br;
		pC->bg = CFactor.bg * Other.bg;
		pC->bb = CFactor.bb * Other.bb;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar + Local.ar;
		pC->ag = CFactor.ag * Other.ag + Local.ag;
		pC->ab = CFactor.ab * Other.ab + Local.ab;
		pC->br = CFactor.br * Other.br + Local.br;
		pC->bg = CFactor.bg * Other.bg + Local.bg;
		pC->bb = CFactor.bb * Other.bb + Local.bb;
		pC2->ar = Local.ar;
		pC2->ag = Local.ag;
		pC2->ab = Local.ab;
		pC2->br = Local.br;
		pC2->bg = Local.bg;
		pC2->bb = Local.bb;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar + Local.aa;
		pC->ag = CFactor.ag * Other.ag + Local.aa;
		pC->ab = CFactor.ab * Other.ab + Local.aa;
		pC->br = CFactor.br * Other.br + Local.ba;
		pC->bg = CFactor.bg * Other.bg + Local.ba;
		pC->bb = CFactor.bb * Other.bb + Local.ba;
		pC2->ar = pC2->ag = pC2->ab = Local.aa;
		pC2->br = pC2->bg = pC2->bb = Local.ba;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * (Other.ar - Local.ar);
		pC->ag = CFactor.ag * (Other.ag - Local.ag);
		pC->ab = CFactor.ab * (Other.ab - Local.ab);
		pC->br = CFactor.br * (Other.br - Local.br);
		pC->bg = CFactor.bg * (Other.bg - Local.bg);
		pC->bb = CFactor.bb * (Other.bb - Local.bb);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
		if ((( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
			( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB )) &&
			(  Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
		{
			pC->ar = Local.ar;
			pC->ag = Local.ag;
			pC->ab = Local.ab;
			pC->br = Local.br;
			pC->bg = Local.bg;
			pC->bb = Local.bb;
		}
		else
		{
			ColorFactor3Func( &CFactor, &Local, &Other );
			pC->ar = CFactor.ar * (Other.ar - Local.ar) + Local.ar;
			pC->ag = CFactor.ag * (Other.ag - Local.ag) + Local.ag;
			pC->ab = CFactor.ab * (Other.ab - Local.ab) + Local.ab;
			pC->br = CFactor.br * (Other.br - Local.br) + Local.br;
			pC->bg = CFactor.bg * (Other.bg - Local.bg) + Local.bg;
			pC->bb = CFactor.bb * (Other.bb - Local.bb) + Local.bb;
			pC2->ar = Local.ar;
			pC2->ag = Local.ag;
			pC2->ab = Local.ab;
			pC2->br = Local.br;
			pC2->bg = Local.bg;
			pC2->bb = Local.bb;
			SecondPass = true;
		}
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		if ((( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
			( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB )) &&
			(  Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
		{
			pC->ar = pC->ag = pC->ab = Local.aa;
			pC->br = pC->bg = pC->bb = Local.ba;
		}
		else
		{
			ColorFactor3Func( &CFactor, &Local, &Other );
			pC->ar = CFactor.ar * (Other.ar - Local.ar) + Local.aa;
			pC->ag = CFactor.ag * (Other.ag - Local.ag) + Local.aa;
			pC->ab = CFactor.ab * (Other.ab - Local.ab) + Local.aa;
			pC->br = CFactor.br * (Other.br - Local.br) + Local.ba;
			pC->bg = CFactor.bg * (Other.bg - Local.bg) + Local.ba;
			pC->bb = CFactor.bb * (Other.bb - Local.bb) + Local.ba;
			pC2->ar = pC2->ag = pC2->ab = Local.aa;
			pC2->br = pC2->bg = pC2->bb = Local.ba;
			SecondPass = true;
		}
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = ( 1.0f - CFactor.ar ) * Local.ar;
		pC->ag = ( 1.0f - CFactor.ag ) * Local.ag;
		pC->ab = ( 1.0f - CFactor.ab ) * Local.ab;
		pC->br = ( 1.0f - CFactor.br ) * Local.br;
		pC->bg = ( 1.0f - CFactor.bg ) * Local.bg;
		pC->bb = ( 1.0f - CFactor.bb ) * Local.bb;
		pC2->ar = Local.ar;
		pC2->ag = Local.ag;
		pC2->ab = Local.ab;
		pC2->br = Local.br;
		pC2->bg = Local.bg;
		pC2->bb = Local.bb;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * -Local.ar + Local.aa;
		pC->ag = CFactor.ag * -Local.ag + Local.aa;
		pC->ab = CFactor.ab * -Local.ab + Local.aa;
		pC->br = CFactor.br * -Local.br + Local.ba;
		pC->bg = CFactor.bg * -Local.bg + Local.ba;
		pC->bb = CFactor.bb * -Local.bb + Local.ba;
		pC2->ar = pC2->ag = pC2->ab = Local.aa;
		pC2->br = pC2->bg = pC2->bb = Local.ba;
		SecondPass = true;
		break;
	}


	switch ( Glide.State.AlphaFunction )
	{
	case GR_COMBINE_FUNCTION_ZERO:
		pC->aa = pC->ba = 0.0f;
		break;
	case GR_COMBINE_FUNCTION_LOCAL:
	case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
		pC->aa = Local.aa;
		pC->ba = Local.ba;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		pC->aa = ((1.0f - AlphaFactorFunc( Local.aa, Other.aa )) * Local.aa);
		pC->ba = ((1.0f - AlphaFactorFunc( Local.ba, Other.ba )) * Local.ba);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * Other.aa);
		pC->ba = (AlphaFactorFunc( Local.ba, Other.ba ) * Other.ba);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * Other.aa + Local.aa);
		pC->ba = (AlphaFactorFunc( Local.ba, Other.ba ) * Other.ba + Local.ba);
//		pC2->aa =  Local.aa;
//		pC2->ba =  Local.ba;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * ( Other.aa - Local.aa ));
		pC->ba = (AlphaFactorFunc( Local.ba, Other.ba ) * ( Other.ba - Local.ba ));
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * ( Other.aa - Local.aa ) + Local.aa);
		pC->ba = (AlphaFactorFunc( Local.ba, Other.ba ) * ( Other.ba - Local.ba ) + Local.ba);
//		pC2->aa =  Local.aa;
//		pC2->ba =  Local.ba;
		break;
	}

	if ( Glide.State.ColorCombineInvert )
	{
		pC->ar = 1.0f - pC->ar;
		pC->ag = 1.0f - pC->ag;
		pC->ab = 1.0f - pC->ab;
		pC->br = 1.0f - pC->br;
		pC->bg = 1.0f - pC->bg;
		pC->bb = 1.0f - pC->bb;
		if ( SecondPass )
		{
			pC2->ar = 1.0f - pC2->ar;
			pC2->ag = 1.0f - pC2->ag;
			pC2->ab = 1.0f - pC2->ab;
			pC2->br = 1.0f - pC2->br;
			pC2->bg = 1.0f - pC2->bg;
			pC2->bb = 1.0f - pC2->bb;
		}
	}

	if ( Glide.State.AlphaInvert )
	{
		pC->aa = 1.0f - pC->aa;
		pC->ba = 1.0f - pC->ba;
		if ( SecondPass )
		{
			pC2->aa = 1.0f - pC2->aa;
			pC2->ba = 1.0f - pC2->ba;
		}
	}
	
	static float w;
	// Z-Buffering
    if(Glide.State.DepthBufferMode == GR_DEPTHBUFFER_DISABLE || Glide.State.DepthBufferMode == GR_CMP_ALWAYS)
    {
		pV->az = 0.0f;
		pV->bz = 0.0f;
    }
	else if ( OpenGL.DepthBufferType )
	{
		pV->az = a->ooz * D1OVER65536;
		pV->bz = b->ooz * D1OVER65536;
	}
	else
	{
       /*
        * For silly values of oow, depth buffering doesn't
        * seem to work, so map them to a sensible z.  When
        * games use these silly values, they probably don't
        * use z buffering anyway.
        */
        if( a->oow > 1.0)
        {
            pV->az = pV->bz = pV->cz = 0.9f;
        }
		else if ( InternalConfig.PrecisionFixEnable )
		{
			w = 1.0f / a->oow;
			pV->az = 1.0f - (float(((*(DWORD *)&w >> 11) & 0xFFFFF) - (127 << 12)) * D1OVER65536);
			w = 1.0f / b->oow;
			pV->bz = 1.0f - (float(((*(DWORD *)&w >> 11) & 0xFFFFF) - (127 << 12)) * D1OVER65536);
		}
		else
		{
			pV->az = a->oow;
			pV->bz = b->oow;
		}
	}

	if (0 && a->x > 2048)
	{
		pV->ax = a->x - vertex_snap;
		pV->ay = a->y - vertex_snap;
		pV->bx = b->x - vertex_snap;
		pV->by = b->y - vertex_snap;
	}
	else
	{
		pV->ax = a->x;
		pV->ay = a->y;
		pV->bx = b->x;
		pV->by = b->y;
	}

	if (OpenGL.Texture)
	{
        float hAspect, wAspect;

        Textures->GetAspect(&hAspect, &wAspect);

		pTS->as = a->tmuvtx[0].sow * wAspect;// / a->oow;
		pTS->at = a->tmuvtx[0].tow * hAspect;// / a->oow;
		pTS->bs = b->tmuvtx[0].sow * wAspect;// / b->oow;
		pTS->bt = b->tmuvtx[0].tow * hAspect;// / b->oow;

		pTS->aq = pTS->bq = 0.0f;
		pTS->aoow = atmuoow;
		pTS->boow = btmuoow;
	}

	if( InternalConfig.FogEnable )
	{
		pF->af = (float)OpenGL.FogTable[ (WORD)(1.0f / a->oow) ] * D1OVER255;
		pF->bf = (float)OpenGL.FogTable[ (WORD)(1.0f / b->oow) ] * D1OVER255;
	#ifdef DEBUG
			if ( pF->af > OGLRender.MaxF ) OGLRender.MaxF = pF->af;
			if ( pF->bf > OGLRender.MaxF ) OGLRender.MaxF = pF->bf;
			if ( pF->af < OGLRender.MinF ) OGLRender.MinF = pF->af;
			if ( pF->bf < OGLRender.MinF ) OGLRender.MinF = pF->bf;
	#endif
	}

#ifdef DEBUG
		if ( pC->ar > OGLRender.MaxR ) OGLRender.MaxR = pC->ar;
		if ( pC->br > OGLRender.MaxR ) OGLRender.MaxR = pC->br;
		if ( pC->ar < OGLRender.MinR ) OGLRender.MinR = pC->ar;
		if ( pC->br < OGLRender.MinR ) OGLRender.MinR = pC->br;
		
		if ( pC->ag > OGLRender.MaxG ) OGLRender.MaxG = pC->ag;
		if ( pC->bg > OGLRender.MaxG ) OGLRender.MaxG = pC->bg;
		if ( pC->ag < OGLRender.MinG ) OGLRender.MinG = pC->ag;
		if ( pC->bg < OGLRender.MinG ) OGLRender.MinG = pC->bg;

		if ( pC->ab > OGLRender.MaxB ) OGLRender.MaxB = pC->ab;
		if ( pC->bb > OGLRender.MaxB ) OGLRender.MaxB = pC->bb;
		if ( pC->ab < OGLRender.MinB ) OGLRender.MinB = pC->ab;
		if ( pC->bb < OGLRender.MinB ) OGLRender.MinB = pC->bb;

		if ( pC->aa > OGLRender.MaxA ) OGLRender.MaxA = pC->aa;
		if ( pC->ba > OGLRender.MaxA ) OGLRender.MaxA = pC->ba;
		if ( pC->aa < OGLRender.MinA ) OGLRender.MinA = pC->aa;
		if ( pC->ba < OGLRender.MinA ) OGLRender.MinA = pC->ba;

		if ( pV->az > OGLRender.MaxZ ) OGLRender.MaxZ = pV->az;
		if ( pV->bz > OGLRender.MaxZ ) OGLRender.MaxZ = pV->bz;
		if ( pV->az < OGLRender.MinZ ) OGLRender.MinZ = pV->az;
		if ( pV->bz < OGLRender.MinZ ) OGLRender.MinZ = pV->bz;

		if ( pV->ax > OGLRender.MaxX ) OGLRender.MaxX = pV->ax;
		if ( pV->bx > OGLRender.MaxX ) OGLRender.MaxX = pV->bx;
		if ( pV->ax < OGLRender.MinX ) OGLRender.MinX = pV->ax;
		if ( pV->bx < OGLRender.MinX ) OGLRender.MinX = pV->bx;

		if ( pV->ay > OGLRender.MaxY ) OGLRender.MaxY = pV->ay;
		if ( pV->by > OGLRender.MaxY ) OGLRender.MaxY = pV->by;
		if ( pV->ay < OGLRender.MinY ) OGLRender.MinY = pV->ay;
		if ( pV->by < OGLRender.MinY ) OGLRender.MinY = pV->by;
#endif


	static DWORD Pixels;
	static BYTE *Buffer1;
	static BYTE *Buffer2;

	if ( OpenGL.Texture )
	{
		glEnable( GL_TEXTURE_2D );

        Textures->MakeReady();
	}
	else
	{
		glDisable( GL_TEXTURE_2D );
	}


	if ( OpenGL.Blend )
		glEnable( GL_BLEND );
	else
		glDisable( GL_BLEND );

	// Alpha Fix
	if (Glide.State.AlphaOther != GR_COMBINE_OTHER_TEXTURE)
		glDisable( GL_ALPHA_TEST );
	else 
		if (Glide.State.AlphaTestFunction != GR_CMP_ALWAYS)
			glEnable( GL_ALPHA_TEST );
	
	glBegin( GL_LINES );
	glColor4fv( &pC->ar );//, tp->a.g, tp->a.b, tp->a.a );
	glSecondaryColor3fvEXT( &pC2->ar );
	glTexCoord4fv( &pTS->as );
	glFogCoordfEXT( pF->af );
	glVertex3fv( &pV->ax );

	glColor4fv( &pC->br );//, tp->a.g, tp->a.b, tp->a.a );
	glSecondaryColor3fvEXT( &pC2->br );
	glTexCoord4fv( &pTS->bs );
	glFogCoordfEXT( pF->bf );
	glVertex3fv( &pV->bx );
	glEnd();

	if ( ( SecondPass ) && ( !InternalConfig.SecondaryColorEXTEnable ) )
	{
		glDisable( GL_TEXTURE_2D );

		glBlendFunc( GL_ONE, GL_ONE );
		glEnable( GL_BLEND );

		if ( OpenGL.DepthBufferType )
			glPolygonOffset( 0.4f, 0.4f );
		else
			glPolygonOffset( -0.4f, -0.4f );

		glEnable( GL_POLYGON_OFFSET_FILL );

		glBegin( GL_LINES );
		glColor4fv( &pC2->ar );
		glVertex3fv( &pV->ax );

		glColor4fv( &pC2->br );
		glVertex3fv( &pV->bx );
		glEnd();

		if ( Glide.State.DepthBiasLevel )
		{
			glPolygonOffset( 1.0f, OpenGL.DepthBiasLevel );
		}
		else
			glDisable( GL_POLYGON_OFFSET_FILL );

		if ( OpenGL.Blend )
			glBlendFunc( OpenGL.SrcBlend, OpenGL.DstBlend );
	}

#ifdef OPENGL_DEBUG
	GLErro( "Render::AddLine" );
#endif

	ProfileEnd( "RenderAddLine" );
}

//void Render::AddPoint( const GrVertex *a )
void RenderAddPoint( const GrVertex *a )
{
	static TColorStruct Local, Other, CFactor;
	static float AFactor[1];
	static TColorStruct *pC, *pC2;
	static TVertexStruct *pV;
	static TTextureStruct *pTS;
	static TFogStruct *pF;
	static void *pt1, *pt2, *pt3;

	ProfileBegin( "RenderAddPoint" );

	pC = &OGLRender.TColor[MAXTRIANGLES];
	pC2 = &OGLRender.TColor2[MAXTRIANGLES];
	pV = &OGLRender.TVertex[MAXTRIANGLES];
	pTS = &OGLRender.TTexture[MAXTRIANGLES];
	pF = &OGLRender.TFog[MAXTRIANGLES];

	// Color Stuff, need to optimize it
	ZeroMemory( pC2, sizeof( TColorStruct ) );
	SecondPass	= false;

	if ( Glide.ALocal )
	{
		switch ( Glide.State.AlphaLocal )
		{
		case GR_COMBINE_LOCAL_ITERATED:
			Local.aa = a->a * D1OVER255;
 			break;
		case GR_COMBINE_LOCAL_CONSTANT:
			Local.aa = OpenGL.ConstantColor[3];
			break;
		case GR_COMBINE_LOCAL_DEPTH:
			Local.aa = a->z;
			break;
		}
	}

	if ( Glide.AOther )
	{
		switch ( Glide.State.AlphaOther )
		{
		case GR_COMBINE_OTHER_ITERATED:
			Other.aa = a->a * D1OVER255;
			break;
		case GR_COMBINE_OTHER_CONSTANT:
			Other.aa = OpenGL.ConstantColor[3];
			break;
		case GR_COMBINE_OTHER_TEXTURE:
			Other.aa = 1.0f;
			break;
		}
	}

	if ( Glide.CLocal )
	{
		switch ( Glide.State.ColorCombineLocal )
		{
		case GR_COMBINE_LOCAL_ITERATED:
			Local.ar = a->r * D1OVER255;
			Local.ag = a->g * D1OVER255;
			Local.ab = a->b * D1OVER255;
			break;
		case GR_COMBINE_LOCAL_CONSTANT:
			Local.ar = OpenGL.ConstantColor[0];
			Local.ag = OpenGL.ConstantColor[1];
			Local.ab = OpenGL.ConstantColor[2];
			break;
		}
	}

	if ( Glide.COther )
	{
		switch ( Glide.State.ColorCombineOther )
		{
		case GR_COMBINE_OTHER_ITERATED:
			Other.ar = a->r * D1OVER255;
			Other.ag = a->g * D1OVER255;
			Other.ab = a->b * D1OVER255;
			break;
		case GR_COMBINE_OTHER_CONSTANT:
			Other.ar = OpenGL.ConstantColor[0];
			Other.ag = OpenGL.ConstantColor[1];
			Other.ab = OpenGL.ConstantColor[2];
			break;
		case GR_COMBINE_OTHER_TEXTURE:
			Other.ar = Other.ag = Other.ab = 1.0f;
			break;
		}
	}

	switch(Glide.State.ColorCombineFunction)
	{
	case GR_COMBINE_FUNCTION_ZERO:
		pC->ar = pC->ag = pC->ab = 0.0f; 
		break;
	case GR_COMBINE_FUNCTION_LOCAL:
		pC->ar = Local.ar;
		pC->ag = Local.ag;
		pC->ab = Local.ab;
		break;
	case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
		pC->ar = pC->ag = pC->ab = Local.aa;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar;
		pC->ag = CFactor.ag * Other.ag;
		pC->ab = CFactor.ab * Other.ab;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar + Local.ar;
		pC->ag = CFactor.ag * Other.ag + Local.ag;
		pC->ab = CFactor.ab * Other.ab + Local.ab;
		pC2->ar = Local.ar;
		pC2->ag = Local.ag;
		pC2->ab = Local.ab;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * Other.ar + Local.aa;
		pC->ag = CFactor.ag * Other.ag + Local.aa;
		pC->ab = CFactor.ab * Other.ab + Local.aa;
		pC2->ar = pC2->ag = pC2->ab = Local.aa;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * (Other.ar - Local.ar);
		pC->ag = CFactor.ag * (Other.ag - Local.ag);
		pC->ab = CFactor.ab * (Other.ab - Local.ab);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
		if ((( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
			( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB )) &&
			(  Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
		{
			pC->ar = Local.ar;
			pC->ag = Local.ag;
			pC->ab = Local.ab;
		}
		else
		{
			ColorFactor3Func( &CFactor, &Local, &Other );
			pC->ar = CFactor.ar * (Other.ar - Local.ar) + Local.ar;
			pC->ag = CFactor.ag * (Other.ag - Local.ag) + Local.ag;
			pC->ab = CFactor.ab * (Other.ab - Local.ab) + Local.ab;
			pC2->ar = Local.ar;
			pC2->ag = Local.ag;
			pC2->ab = Local.ab;
			SecondPass = true;
		}
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		if ((( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_ALPHA ) ||
			( Glide.State.ColorCombineFactor == GR_COMBINE_FACTOR_TEXTURE_RGB )) &&
			(  Glide.State.ColorCombineOther == GR_COMBINE_OTHER_TEXTURE ) )
		{
			pC->ar = pC->ag = pC->ab = Local.aa;
		}
		else
		{
			ColorFactor3Func( &CFactor, &Local, &Other );
			pC->ar = CFactor.ar * (Other.ar - Local.ar) + Local.aa;
			pC->ag = CFactor.ag * (Other.ag - Local.ag) + Local.aa;
			pC->ab = CFactor.ab * (Other.ab - Local.ab) + Local.aa;
			pC2->ar = pC2->ag = pC2->ab = Local.aa;
			SecondPass = true;
		}
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = ( 1.0f - CFactor.ar ) * Local.ar;
		pC->ag = ( 1.0f - CFactor.ag ) * Local.ag;
		pC->ab = ( 1.0f - CFactor.ab ) * Local.ab;
		pC2->ar = Local.ar;
		pC2->ag = Local.ag;
		pC2->ab = Local.ab;
		SecondPass = true;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		ColorFactor3Func( &CFactor, &Local, &Other );
		pC->ar = CFactor.ar * -Local.ar + Local.aa;
		pC->ag = CFactor.ag * -Local.ag + Local.aa;
		pC->ab = CFactor.ab * -Local.ab + Local.aa;
		pC2->ar = pC2->ag = pC2->ab = Local.aa;
		SecondPass = true;
		break;
	}


	switch ( Glide.State.AlphaFunction )
	{
	case GR_COMBINE_FUNCTION_ZERO:
		pC->aa = 0.0f;
		break;
	case GR_COMBINE_FUNCTION_LOCAL:
	case GR_COMBINE_FUNCTION_LOCAL_ALPHA:
		pC->aa = Local.aa;
		break;
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		pC->aa = ((1.0f - AlphaFactorFunc( Local.aa, Other.aa )) * Local.aa);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * Other.aa);
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * Other.aa + Local.aa);
//		pC2->aa =  Local.aa;
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * ( Other.aa - Local.aa ));
		break;
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL:
	case GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA:
		pC->aa = (AlphaFactorFunc( Local.aa, Other.aa ) * ( Other.aa - Local.aa ) + Local.aa);
//		pC2->aa =  Local.aa;
		break;
	}

	if ( Glide.State.ColorCombineInvert )
	{
		pC->ar = 1.0f - pC->ar;
		pC->ag = 1.0f - pC->ag;
		pC->ab = 1.0f - pC->ab;
		if ( SecondPass )
		{
			pC2->ar = 1.0f - pC2->ar;
			pC2->ag = 1.0f - pC2->ag;
			pC2->ab = 1.0f - pC2->ab;
		}
	}

	if ( Glide.State.AlphaInvert )
	{
		pC->aa = 1.0f - pC->aa;
		if ( SecondPass )
		{
			pC2->aa = 1.0f - pC2->aa;
		}
	}
	
	static float w;
	// Z-Buffering
    if(Glide.State.DepthBufferMode == GR_DEPTHBUFFER_DISABLE || Glide.State.DepthBufferMode == GR_CMP_ALWAYS)
    {
		pV->az = 0.0f;
    }
	else if ( OpenGL.DepthBufferType )
	{
		pV->az = a->ooz * D1OVER65536;
	}
	else
	{
		if ( InternalConfig.PrecisionFixEnable )
		{
			w = 1.0f / a->oow;
			pV->az = 1.0f - (float(((*(DWORD *)&w >> 11) & 0xFFFFF) - (127 << 12)) * D1OVER65536);
		}
		else
		{
			pV->az = a->oow;
		}
	}

	if (0 && a->x > 2048)
	{
		pV->ax = a->x - vertex_snap;
		pV->ay = a->y - vertex_snap;
	}
	else
	{
		pV->ax = a->x;
		pV->ay = a->y;
	}

	if (OpenGL.Texture)
	{
        float hAspect, wAspect;

        Textures->GetAspect(&hAspect, &wAspect);

		pTS->as = a->tmuvtx[0].sow * wAspect;// / a->oow;
		pTS->at = a->tmuvtx[0].tow * hAspect;// / a->oow;

		pTS->aq = 0.0f;
		pTS->aoow = a->oow;
	}

	if( InternalConfig.FogEnable )
	{
		pF->af = (float)OpenGL.FogTable[ (WORD)(1.0f / a->oow) ] * D1OVER255;
	#ifdef DEBUG
			if ( pF->af > OGLRender.MaxF ) OGLRender.MaxF = pF->af;
			if ( pF->af < OGLRender.MinF ) OGLRender.MinF = pF->af;
	#endif
	}

#ifdef DEBUG
		if ( pC->ar > OGLRender.MaxR ) OGLRender.MaxR = pC->ar;
		if ( pC->ar < OGLRender.MinR ) OGLRender.MinR = pC->ar;
		
		if ( pC->ag > OGLRender.MaxG ) OGLRender.MaxG = pC->ag;
		if ( pC->ag < OGLRender.MinG ) OGLRender.MinG = pC->ag;

		if ( pC->ab > OGLRender.MaxB ) OGLRender.MaxB = pC->ab;
		if ( pC->ab < OGLRender.MinB ) OGLRender.MinB = pC->ab;

		if ( pC->aa > OGLRender.MaxA ) OGLRender.MaxA = pC->aa;
		if ( pC->aa < OGLRender.MinA ) OGLRender.MinA = pC->aa;

		if ( pV->az > OGLRender.MaxZ ) OGLRender.MaxZ = pV->az;
		if ( pV->az < OGLRender.MinZ ) OGLRender.MinZ = pV->az;

		if ( pV->ax > OGLRender.MaxX ) OGLRender.MaxX = pV->ax;
		if ( pV->ax < OGLRender.MinX ) OGLRender.MinX = pV->ax;

		if ( pV->ay > OGLRender.MaxY ) OGLRender.MaxY = pV->ay;
		if ( pV->ay < OGLRender.MinY ) OGLRender.MinY = pV->ay;
#endif


	static DWORD Pixels;
	static BYTE *Buffer1;
	static BYTE *Buffer2;

	if ( OpenGL.Texture )
	{
		glEnable( GL_TEXTURE_2D );

        Textures->MakeReady();
	}
	else
	{
		glDisable( GL_TEXTURE_2D );
	}


	if ( OpenGL.Blend )
		glEnable( GL_BLEND );
	else
		glDisable( GL_BLEND );

	// Alpha Fix
	if (Glide.State.AlphaOther != GR_COMBINE_OTHER_TEXTURE)
		glDisable( GL_ALPHA_TEST );
	else 
		if (Glide.State.AlphaTestFunction != GR_CMP_ALWAYS)
			glEnable( GL_ALPHA_TEST );
	
	glBegin( GL_POINTS );
	glColor4fv( &pC->ar );
	glSecondaryColor3fvEXT( &pC2->ar );
	glTexCoord4fv( &pTS->as );
	glFogCoordfEXT( pF->af );
	glVertex3fv( &pV->ax );
	glEnd();

	if ( ( SecondPass ) && ( !InternalConfig.SecondaryColorEXTEnable ) )
	{
		glDisable( GL_TEXTURE_2D );

		glBlendFunc( GL_ONE, GL_ONE );
		glEnable( GL_BLEND );

		if ( OpenGL.DepthBufferType )
			glPolygonOffset( 0.4f, 0.4f );
		else
			glPolygonOffset( -0.4f, -0.4f );

		glEnable( GL_POLYGON_OFFSET_FILL );

		glBegin( GL_LINES );
		glColor4fv( &pC2->ar );
		glVertex3fv( &pV->ax );
		glEnd();

		if ( Glide.State.DepthBiasLevel )
		{
			glPolygonOffset( 1.0f, OpenGL.DepthBiasLevel );
		}
		else
			glDisable( GL_POLYGON_OFFSET_FILL );

		if ( OpenGL.Blend )
			glBlendFunc( OpenGL.SrcBlend, OpenGL.DstBlend );
	}

#ifdef OPENGL_DEBUG
	GLErro( "Render::AddPoint" );
#endif

	ProfileEnd( "RenderAddPoint" );
}


void TColorFunctionScaleOtherAddLocalTDNow( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	ColorFactor3Func( &CFactor, Local, Other );
	__asm
	{
		mov eax, pC
		femms
		mov ebx, offset CFactor
		mov edx, Other
		mov ecx, Local
		movq mm0, [ebx]
		pfmul (mm0, edx)
		pfadd (mm0, ecx)
		movq mm1, [ebx+8]
		pfmulm (mm1, edx, 8)
		pfaddm (mm1, ecx, 8)
		movq mm2, [ebx+16]
		movq [eax], mm0
		pfmulm (mm2, edx, 16)
		pfaddm (mm2, ecx, 16)
		movq mm3, [ebx+24]
		movq [eax+8], mm1
		pfmulm (mm3, edx, 24)
		pfaddm (mm3, ecx, 24)
		movq mm4, [ebx+32]
		movq [eax+16], mm2
		pfmulm (mm4, edx, 32)
		pfaddm (mm4, ecx, 32)
		movq mm5, [ebx+40]
		movq [eax+24], mm3
		pfmulm (mm5, edx, 40)
		pfaddm (mm5, ecx, 40)
		movq [eax+32], mm4
		mov edx, pC2
		movq [eax+40], mm5
		movq mm0, [ecx]
		movq mm1, [ecx+8]
		movq [edx], mm0
		movq [edx+8], mm1
		movq mm2, [ecx+16]
		movq mm3, [ecx+24]
		movq [edx+16], mm2
		movq [edx+24], mm3
		movq mm4, [ecx+32]
		movq mm5, [ecx+40]
		movq [edx+32], mm4
		movq [edx+40], mm5
		femms
		mov SecondPass, 1
	}
//	SecondPass = true;
}

void TColorFunctionScaleOtherMinusLocalAddLocalTDNow( TColorStruct * pC, TColorStruct * pC2, TColorStruct * Local, TColorStruct * Other )
{
	ColorFactor3Func( &CFactor, Local, Other );
	__asm
	{
		mov ecx, Local
		mov	eax, DWORD PTR Glide.State.ColorCombineFactor
		cmp	eax, GR_COMBINE_FACTOR_TEXTURE_ALPHA
		je	SHORT cont
		cmp	eax, GR_COMBINE_FACTOR_TEXTURE_RGB
		jne	SHORT SecondLoop
cont:
		cmp	Glide.State.ColorCombineOther, GR_COMBINE_OTHER_TEXTURE
		jne	SHORT SecondLoop

		mov edx, pC
		movq mm0, [ecx]
		movq [edx], mm0
		movq mm0, [ecx+8]
		movq [edx+8], mm0
		movq mm0, [ecx+16]
		movq [edx+16], mm0
		movq mm0, [ecx+24]
		movq [edx+24], mm0
		movq mm0, [ecx+32]
		movq [edx+32], mm0
		movq mm0, [ecx+40]
		movq [edx+40], mm0
		jmp EndFunction

SecondLoop:
		femms
		mov eax, pC
		mov edx, Other
		mov ebx, offset CFactor
		movq mm0, [edx]
		pfsub (mm0, ecx)
		pfmul (mm0, ebx)
		pfadd (mm0, ecx)
		movq mm1, [edx+8]
		pfsubm (mm1, ecx, 8)
		pfmulm (mm1, ebx, 8)
		pfaddm (mm1, ecx, 8)
		movq mm2, [edx+16]
		pfsubm (mm2, ecx, 16)
		pfmulm (mm2, ebx, 16)
		pfaddm (mm2, ecx, 16)
		movq mm3, [edx+24]
		pfsubm (mm3, ecx, 24)
		pfmulm (mm3, edx, 24)
		pfaddm (mm3, ecx, 24)
		movq mm4, [edx+32]
		pfsubm (mm4, ecx, 32)
		pfmulm (mm4, ebx, 32)
		pfaddm (mm4, ecx, 32)
		movq mm5, [edx+40]
		pfsubm (mm5, ecx, 40)
		pfmulm (mm5, ebx, 40)
		pfaddm (mm5, ecx, 40)
		movq [eax], mm0
		movq [eax+8], mm1
		movq [eax+16], mm2
		movq [eax+24], mm3
		movq [eax+32], mm4
		movq [eax+40], mm5
		mov edx, pC2
		movq mm0, [ecx]
		movq mm1, [ecx+8]
		movq [edx], mm0
		movq [edx+8], mm1
		movq mm2, [ecx+16]
		movq mm3, [ecx+24]
		movq [edx+16], mm2
		movq [edx+24], mm3
		movq mm4, [ecx+32]
		movq mm5, [ecx+40]
		movq [edx+32], mm4
		movq [edx+40], mm5
		mov SecondPass, 1
EndFunction:
		femms
	}
}



/*
inline void ColorFactor3( TColorStruct &Result, TColorStruct *ColorComponent, TColorStruct *OtherAlpha )
{
	switch(Glide.State.ColorCombineFactor)
	{
	case GR_COMBINE_FACTOR_ZERO:
		Result.ar = Result.ag = Result.ab = 0.0f;
		Result.br = Result.bg = Result.bb = 0.0f;
		Result.cr = Result.cg = Result.cb = 0.0f;
		return;
	case GR_COMBINE_FACTOR_LOCAL:
		Result.ar = ColorComponent->ar;
		Result.ag = ColorComponent->ag;
		Result.ab = ColorComponent->ab;
		Result.br = ColorComponent->br;
		Result.bg = ColorComponent->bg;
		Result.bb = ColorComponent->bb;
		Result.cr = ColorComponent->cr;
		Result.cg = ColorComponent->cg;
		Result.cb = ColorComponent->cb;
		return;
	case GR_COMBINE_FACTOR_OTHER_ALPHA:
		Result.ar = Result.ag = Result.ab = OtherAlpha->aa;
		Result.br = Result.bg = Result.bb = OtherAlpha->ba;
		Result.cr = Result.cg = Result.cb = OtherAlpha->ca;
		return;
	case GR_COMBINE_FACTOR_LOCAL_ALPHA:
		Result.ar = Result.ag = Result.ab = ColorComponent->aa;
		Result.br = Result.bg = Result.bb = ColorComponent->ba;
		Result.cr = Result.cg = Result.cb = ColorComponent->ca;
		return;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
		Result.ar = 1.0f - ColorComponent->ar;
		Result.ag = 1.0f - ColorComponent->ag;
		Result.ab = 1.0f - ColorComponent->ab;
		Result.br = 1.0f - ColorComponent->br;
		Result.bg = 1.0f - ColorComponent->bg;
		Result.bb = 1.0f - ColorComponent->bb;
		Result.cr = 1.0f - ColorComponent->cr;
		Result.cg = 1.0f - ColorComponent->cg;
		Result.cb = 1.0f - ColorComponent->cb;
		return;
	case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
		Result.ar = Result.ag = Result.ab = 1.0f - OtherAlpha->aa;
		Result.br = Result.bg = Result.bb = 1.0f - OtherAlpha->ba;
		Result.cr = Result.cg = Result.cb = 1.0f - OtherAlpha->ca;
		return;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
		Result.ar = Result.ag = Result.ab = 1.0f - ColorComponent->aa;
		Result.br = Result.bg = Result.bb = 1.0f - ColorComponent->ba;
		Result.cr = Result.cg = Result.cb = 1.0f - ColorComponent->ca;
		return;
	}
//	case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
//	case GR_COMBINE_FACTOR_TEXTURE_RGB:		//GR_COMBINE_FACTOR_LOD_FRACTION:
//	case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
//	case GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION:
//	case GR_COMBINE_FACTOR_ONE:
		Result.ar = Result.ag = Result.ab = 1.0f;
		Result.br = Result.bg = Result.bb = 1.0f;
		Result.cr = Result.cg = Result.cb = 1.0f;
//		return;
//	}
}

inline float ColorFactor( float ColorComponent, float LocalAlpha, float OtherAlpha )
{
	switch ( Glide.State.ColorCombineFactor )
	{
	case GR_COMBINE_FACTOR_ZERO:
		return 0.0f;
	case GR_COMBINE_FACTOR_LOCAL:
		return ColorComponent;
	case GR_COMBINE_FACTOR_OTHER_ALPHA:
		return OtherAlpha;
	case GR_COMBINE_FACTOR_LOCAL_ALPHA:
		return LocalAlpha;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
		return 1.0f - ColorComponent;
	case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
		return 1.0f - OtherAlpha;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
		return 1.0f - LocalAlpha;
//	case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
//	case GR_COMBINE_FACTOR_TEXTURE_RGB:		//GR_COMBINE_FACTOR_LOD_FRACTION:
//	case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
//	case GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION:
//	case GR_COMBINE_FACTOR_ONE:
//		return 1.0f;
	}
	return 1.0f;
}

*/
/*
inline float AlphaFactor( float LocalAlpha, float OtherAlpha )
{
	switch ( Glide.State.AlphaFactor )
	{
	case GR_COMBINE_FACTOR_ZERO:
		return 0.0f;
	case GR_COMBINE_FACTOR_LOCAL:
	case GR_COMBINE_FACTOR_LOCAL_ALPHA:
		return LocalAlpha;
	case GR_COMBINE_FACTOR_OTHER_ALPHA:
		return OtherAlpha;
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL:
	case GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA:
		return 1.0f - LocalAlpha;
	case GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA:
		return 1.0f - OtherAlpha;
//	case GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA:
//	case GR_COMBINE_FACTOR_ONE:
//	case GR_COMBINE_FACTOR_TEXTURE_ALPHA:
//		return 1.0f;
	}
	return 1.0f;
}
*/
