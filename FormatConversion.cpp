
#include "FormatConversion.h"


void Convert565to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
	while ( Pixels )
	{
		*Buffer2++ = 0xFF000000 |					// A
			((*Buffer1)		& 0x001F) << 19 |		// B
			((*Buffer1)		& 0x07E0) << 5  |		// G
			((*Buffer1++)	& 0xF800) >> 8;			// R
		Pixels--;
	}
}

// This functions processes 2 pixels at a time, there is no problem in
// passing odd numbers or a number less than 2 for the pixels, but
// the buffers should be large enough
void Convert565to5551( DWORD *Buffer1, DWORD *Buffer2, int Pixels )
{
	while ( Pixels > 0 )
	{
		*Buffer2++ = (   (*Buffer1) & 0xFFC0FFC0 ) |
					 ( ( (*Buffer1++) & 0x001F001F ) << 1 ) |
                     0x00010001;
		Pixels -= 2;
	}
}

// This functions processes 2 pixels at a time, there is no problem in
// passing odd numbers or a number less than 2 for the pixels, but
// the buffers should be large enough
void Convert5551to565( DWORD *Buffer1, DWORD *Buffer2, int Pixels )
{
	while ( Pixels > 0 )
	{
		*Buffer2++ = (   (*Buffer1) & 0xFFC0FFC0 ) |
					 ( ( (*Buffer1++) & 0x003E003E ) >> 1 );
		Pixels -= 2;
	}
}

// This functions processes 2 pixels at a time, there is no problem in
// passing odd numbers or a number less than 2 for the pixels, but
// the buffers should be large enough
void Convert4444to4444special( DWORD *Buffer1, DWORD *Buffer2, int Pixels )
{
	while ( Pixels > 0 )
	{
		*Buffer2++ = ( ( (*Buffer1) & 0x0FFF0FFF ) << 4 )|
					 ( ( (*Buffer1++) & 0xF000F000 ) >> 12 );
		Pixels -= 2;
	}
}

void Convert1555to5551( DWORD *Buffer1, DWORD *Buffer2, int Pixels )
{
	while ( Pixels > 0 )
	{
		*Buffer2++ = ( ( (*Buffer1) & 0x7FFF7FFF ) << 1 )|
					 ( ( (*Buffer1++) & 0x80008000 ) >> 15 );
		Pixels -= 2;
	}
}

unsigned __int64 Mask565_5551_1 = 0xFFC0FFC0FFC0FFC0;
unsigned __int64 Mask565_5551_2 = 0x001F001F001F001F;
unsigned __int64 Mask565_5551_3 = 0x0001000100010001;

// This functions processes 4 pixels at a time, there is no problem in
// passing odd numbers or a number less than 4 for the pixels, but
// the buffers should be large enough
void MMXConvert565to5551( void *Src, void *Dst, int NumberOfPixels )
{
	__asm
	{
		mov ecx, NumberOfPixels
		mov eax, Src
        shl ecx, 1
		mov edx, Dst
		movq mm6, [Mask565_5551_3]
		movq mm5, [Mask565_5551_2]
		movq mm4, [Mask565_5551_1]
    align 16
copying:
        movq mm0, [eax + ecx]
        movq mm1, mm6
        movq mm2, mm0

        pand mm0, mm5
        pand mm2, mm4
        psllq mm0, 1
        por mm1, mm2
        por mm0, mm1
        
		movq [edx + ecx], mm0
		sub ecx, 8
		jg copying
		EMMS
	}
}

unsigned __int64 Mask5551_565_1 = 0xFFC0FFC0FFC0FFC0;
unsigned __int64 Mask5551_565_2 = 0x003E003E003E003E;

// This functions processes 4 pixels at a time, there is no problem in
// passing odd numbers or a number less than 4 for the pixels, but
// the buffers should be large enough
void MMXConvert5551to565( void *Src, void *Dst, int NumberOfPixels )
{
	__asm
	{
		mov ecx, NumberOfPixels
		mov eax, Src
        shl ecx, 1
		mov edx, Dst
		movq mm5, [Mask5551_565_2]
		movq mm4, [Mask5551_565_1]
    align 16
copying:
        movq mm0, [eax + ecx]
        movq mm2, mm0

        pand mm0, mm5
        pand mm2, mm4
        psrlq mm0, 1
        por mm0, mm2
        
		movq [edx + ecx], mm0
		sub ecx, 8
		jg copying
		EMMS
	}
}

unsigned __int64 Mask4444_1 = 0x0FFF0FFF0FFF0FFF;
unsigned __int64 Mask4444_2 = 0xF000F000F000F000;

// This functions processes 4 pixels at a time, there is no problem in
// passing odd numbers or a number less than 4 for the pixels, but
// the buffers should be large enough
void MMXConvert4444to4444special( void *Src, void *Dst, int NumberOfPixels )
{
	__asm
	{
		mov ecx, NumberOfPixels
		mov eax, Src
        shl ecx, 1
		mov edx, Dst
		movq mm7, [Mask4444_2]
		movq mm6, [Mask4444_1]
    align 16
copying:
        movq mm0, [eax + ecx]
        movq mm1, mm0

        pand mm0, mm6
        pand mm1, mm7
        psllq mm0, 4
        psrlq mm1, 12
        por mm0, mm1
        
		movq [edx + ecx], mm0
		sub ecx, 8
		jg copying
		EMMS
	}
}

unsigned __int64 Mask5551_1 = 0x7FFF7FFF7FFF7FFF;
unsigned __int64 Mask5551_2 = 0x8000800080008000;

// This functions processes 4 pixels at a time, there is no problem in
// passing odd numbers or a number less than 4 for the pixels, but
// the buffers should be large enough
void MMXConvert1555to5551( void *Src, void *Dst, int NumberOfPixels )
{
	__asm
	{
		mov ecx, NumberOfPixels
		mov eax, Src
        shl ecx, 1
		mov edx, Dst
		movq mm7, [Mask4444_2]
		movq mm6, [Mask4444_1]
    align 16
copying:
        movq mm0, [eax + ecx]
        movq mm1, mm0

        pand mm0, mm6
        pand mm1, mm7
        psllq mm0, 1
        psrlq mm1, 15
        por mm0, mm1
        
		movq [edx + ecx], mm0
		sub ecx, 8
		jg copying
		EMMS
	}
}

BYTE Mask565A[8] = { 0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF };
BYTE Mask565B[8] = { 0x00,0xF8,0x00,0xF8,0x00,0xF8,0x00,0xF8 };
BYTE Mask565G[8] = { 0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07 };
BYTE Mask565R[8] = { 0x1F,0x00,0x1F,0x00,0x1F,0x00,0x1F,0x00 };

void MMXConvert565to8888( void *Src, void *Dst, DWORD NumberOfPixels )
{
	// Word entered is ARGB
	// Has to be ABGR
	__asm
	{
		MOVQ MM7, [Mask565A]
		mov ECX, NumberOfPixels
		MOVQ MM6, [Mask565B]
		mov EAX, Src
		MOVQ MM5, [Mask565G]
		MOVQ MM4, [Mask565R]
		mov EDX, Dst
copying:
		MOVQ MM0, [EAX]
		add EAX, 8
		MOVQ MM2, MM0
		MOVQ MM1, MM0

		PAND MM0, MM4 // Mask R
		PAND MM2, MM6 // Mask B
		PSLLW MM0, 11 // Shift R
		PAND MM1, MM5 // Mask G

		PSRLW MM2, 8  // Shift B

		MOVQ MM3, MM1
		PSLLW MM1, 13
		POR MM0, MM2
		PSRLW MM3, 3
		POR MM1, MM3

		POR MM1, MM7

		MOVQ MM2, MM0
		PUNPCKHBW MM0, MM1
		PUNPCKLBW MM2, MM1

		// Storing Unpacked 
		MOVQ [EDX], MM2
		add EDX, 16
		MOVQ [EDX-8], MM0
		sub ECX, 4
		jg copying
		EMMS
	}
}

void ConvertA8toAP88( BYTE *Buffer1, WORD *Buffer2, DWORD Pixels )
{
    while ( Pixels )
    {
        *Buffer2 = ( ( ( *Buffer1 ) << 8 ) | ( *Buffer1 ) );
        Buffer1++;
        Buffer2++;
        Pixels--;
    }
}

void Convert8332to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels )
{
    static DWORD    R, 
                    G, 
                    B, 
                    A;
    for ( DWORD i = Pixels; i > 0; i-- )
    {
        A = ( ( ( *Buffer1 ) >> 8 ) & 0xFF );
        R = ( ( ( *Buffer1 ) >> 5 ) & 0x07 ) << 5;
        G = ( ( ( *Buffer1 ) >> 2 ) & 0x07 ) << 5;
        B = (   ( *Buffer1 ) & 0x03 ) << 6;
        *Buffer2 = ( A << 24 ) | ( B << 16 ) | ( G << 8 ) | R;
        Buffer1++;
        Buffer2++;
    }
}

void ConvertP8to8888( BYTE *Buffer1, DWORD *Buffer2, DWORD Pixels, DWORD *palette )
{
    while ( Pixels-- )
    {
        *Buffer2++ = palette[ *Buffer1++ ];
    }
}

void ConvertAP88to8888( WORD *Buffer1, DWORD *Buffer2, DWORD Pixels, DWORD *palette )
{
    DWORD   RGB, 
            A;
    for ( DWORD i = Pixels; i > 0; i-- )
    {
        RGB = ( palette[ *Buffer1 & 0x00FF ] & 0x00FFFFFF );
        A = *Buffer1 >> 8;
        *Buffer2 = ( A << 24 ) | RGB;
        Buffer1++;
        Buffer2++;
    }
}

void ConvertYIQto8888( BYTE *in, DWORD *out, DWORD Pixels, GuNccTable *ncc )
{
    LONG   R;
    LONG   G;
    LONG   B;

    for ( DWORD i = Pixels; i > 0; i-- )
    {
        R = ncc->yRGB[ *in >> 4 ] + ncc->iRGB[ ( *in >> 2 ) & 0x3 ][ 0 ]
                                  + ncc->qRGB[ ( *in      ) & 0x3 ][ 0 ];

        G = ncc->yRGB[ *in >> 4 ] + ncc->iRGB[ ( *in >> 2 ) & 0x3 ][ 1 ]
                                  + ncc->qRGB[ ( *in      ) & 0x3 ][ 1 ];

        B = ncc->yRGB[ *in >> 4 ] + ncc->iRGB[ ( *in >> 2 ) & 0x3 ][ 2 ]
                                  + ncc->qRGB[ ( *in      ) & 0x3 ][ 2 ];

        R = ( ( R < 0 ) ? 0 : ( ( R > 255 ) ? 255 : R ) );
        G = ( ( G < 0 ) ? 0 : ( ( G > 255 ) ? 255 : G ) );
        B = ( ( B < 0 ) ? 0 : ( ( B > 255 ) ? 255 : B ) );

        *out = ( R | ( G << 8 ) | ( B << 16 ) | 0xff000000 );

        in++;
        out++;
    }
}

void ConvertAYIQto8888( WORD *in, DWORD *out, DWORD Pixels, GuNccTable *ncc)
{
    LONG   R;
    LONG   G;
    LONG   B;

    for ( DWORD i = Pixels; i > 0; i-- )
    {
        R = ncc->yRGB[ ( *in >> 4 ) & 0xf ] + ncc->iRGB[ ( *in >> 2 ) & 0x3 ][ 0 ]
                                            + ncc->qRGB[ ( *in      ) & 0x3 ][ 0 ];

        G = ncc->yRGB[ ( *in >> 4 ) & 0xf ] + ncc->iRGB[ ( *in >> 2 ) & 0x3 ][ 1 ]
                                            + ncc->qRGB[ ( *in      ) & 0x3 ][ 1 ];

        B = ncc->yRGB[ ( *in >> 4 ) & 0xf ] + ncc->iRGB[ ( *in >> 2 ) & 0x3 ][ 2 ]
                                            + ncc->qRGB[ ( *in      ) & 0x3 ][ 2 ];

        R = ( ( R < 0 ) ? 0 : ( ( R > 255 ) ? 255 : R ) );
        G = ( ( G < 0 ) ? 0 : ( ( G > 255 ) ? 255 : G ) );
        B = ( ( B < 0 ) ? 0 : ( ( B > 255 ) ? 255 : B ) );

        *out = ( R | ( G << 8 ) | ( B << 16 ) | ( 0xff000000 & ( *in << 16 ) ) );

        in++;
        out++;
    }
}

void SplitAP88( WORD *ap88, BYTE *index, BYTE *alpha, DWORD pixels )
{
    for ( DWORD i = pixels; i > 0; i-- )
    {
        *alpha++ = ( *ap88 >> 8 );
        *index++ = ( *ap88++ & 0xff );
    }
}
