#ifdef HAVE_CONFIG_H
#include "config.h"

#if SIZEOF_INT_P == 4
#define OP "l"
#else
#define OP "q"
#endif

#endif

#include "FormatConversion.h"

#if defined _MSC_VER
#define __UINT64_C(x) x
#elif !defined __UINT64_C
#define __UINT64_C(c) c ## ULL
#endif

void Convert565Kto8888( FxU16 *Buffer1, FxU16 key, FxU32 *Buffer2, FxU32 Pixels )
{
    while ( Pixels )
    {
        *Buffer2++ = ( ( (*Buffer1) == key) ? 0x00000000 : 0xFF000000 ) |   // A
                       ( (*Buffer1)    & 0x001F ) << 19 |                   // B
                       ( (*Buffer1)    & 0x07E0 ) << 5  |                   // G
                       ( (*Buffer1++)  & 0xF800 ) >> 8;                     // R
        Pixels--;
    }
}

FxU64 Mask565_5551_1 = __UINT64_C(0xFFC0FFC0FFC0FFC0);
FxU64 Mask565_5551_2 = __UINT64_C(0x001F001F001F001F);
FxU64 Mask565_5551_3 = __UINT64_C(0x0001000100010001);

#ifdef HAVE_MMX

// This functions processes 4 pixels at a time, there is no problem in
// passing odd numbers or a number less than 4 for the pixels, but
// the buffers should be large enough
void Convert565to5551( FxU32 *Src, FxU32 *Dst, int NumberOfPixels )
{
#ifdef _MSC_VER
    __asm
    {
        mov ecx, NumberOfPixels
        mov eax, Src
        shl ecx, 1
        sub ecx, 8
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
        jge copying
        EMMS
    }
#endif

#ifdef __GNUC__
    asm ("shl" OP " $1, %0;"
         "sub" OP " $8, %0;"
         "movq  %5, %%mm6;"
         "movq  %4, %%mm5;"
         "movq  %3, %%mm4;"
         ".align 16;"
         "MMXConvert565to5551_copying:"
         "movq  (%1,%0), %%mm0;"
         "movq  %%mm6, %%mm1;"
         "movq  %%mm0, %%mm2;"
         "pand  %%mm5, %%mm0;"
         "pand  %%mm4, %%mm2;"
         "psllq $1, %%mm0;"
         "por   %%mm2, %%mm1;"
         "por   %%mm1, %%mm0;"
         "movq  %%mm0, (%2,%0);"
         "sub" OP " $8, %0;"
         "jge   MMXConvert565to5551_copying;"
         "EMMS;"
         : /* No outputs */
         : "r" ((FxU)NumberOfPixels), "r" (Src), "r" (Dst), /*Inputs */
           "m" (Mask565_5551_1), "m" (Mask565_5551_2), "m" (Mask565_5551_3)
         : "%mm0", "%mm1", "%mm2", "%mm4", "%mm5", "%mm6", "memory" /* Clobbers */
        );
#endif
}

#else

// This functions processes 2 pixels at a time, there is no problem in
// passing odd numbers or a number less than 2 for the pixels, but
// the buffers should be large enough
void Convert565to5551( FxU32 *Buffer1, FxU32 *Buffer2, int Pixels )
{
   while ( Pixels > 0 )
   {
      *Buffer2++ = (   (*Buffer1) & 0xFFC0FFC0 ) |
                ( ( (*Buffer1++) & 0x001F001F ) << 1 ) |
                     0x00010001;
      Pixels -= 2;
   }
}

#endif

FxU64 Mask5551_565_1 = __UINT64_C(0xFFC0FFC0FFC0FFC0);
FxU64 Mask5551_565_2 = __UINT64_C(0x003E003E003E003E);

#ifdef HAVE_MMX

// This functions processes 4 pixels at a time, there is no problem in
// passing odd numbers or a number less than 4 for the pixels, but
// the buffers should be large enough
void Convert5551to565( FxU32 *Src, FxU32 *Dst, int NumberOfPixels )
{

#ifdef _MSC_VER
   __asm
   {
      mov ecx, NumberOfPixels
      mov eax, Src
      shl ecx, 1
      sub ecx, 8
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
      jge copying
      EMMS
   }
#endif

#ifdef __GNUC__
    asm ("shl" OP " $1, %0;"
         "sub" OP " $8, %0;"
         "movq  %4, %%mm5;"
         "movq  %3, %%mm4;"
         ".align 16;"
         "MMXConvert5551to565_copying:"
         "movq  (%1,%0), %%mm0;"
         "movq  %%mm0, %%mm2;"
         "pand  %%mm5, %%mm0;"
         "pand  %%mm4, %%mm2;"
         "psllq $1, %%mm0;"
         "por   %%mm2, %%mm0;"
         "movq  %%mm0, (%2,%0);"
         "sub" OP " $8, %0;"
         "jge   MMXConvert5551to565_copying;"
         "EMMS;"
         : /* No outputs */
         : "r" ((FxU)NumberOfPixels), "r" (Src), "r" (Dst), /*Inputs */
           "m" (Mask5551_565_1), "m" (Mask5551_565_2)
         : "%mm0", "%mm2", "%mm4", "%mm5", "memory" /* Clobbers */
        );
#endif
}

#else

// This functions processes 2 pixels at a time, there is no problem in
// passing odd numbers or a number less than 2 for the pixels, but
// the buffers should be large enough
void Convert5551to565( FxU32 *Buffer1, FxU32 *Buffer2, int Pixels )
{
   while ( Pixels > 0 )
   {
      *Buffer2++ = (   (*Buffer1) & 0xFFC0FFC0 ) |
                ( ( (*Buffer1++) & 0x003E003E ) >> 1 );
      Pixels -= 2;
   }
}

#endif

FxU64 Mask4444_1 = __UINT64_C(0x0FFF0FFF0FFF0FFF);
FxU64 Mask4444_2 = __UINT64_C(0xF000F000F000F000);

#ifdef HAVE_MMX

// This functions processes 4 pixels at a time, there is no problem in
// passing odd numbers or a number less than 4 for the pixels, but
// the buffers should be large enough
void Convert4444to4444special( FxU32 *Src, FxU32 *Dst, int NumberOfPixels )
{
#ifdef _MSC_VER
   __asm
   {
      mov ecx, NumberOfPixels
      mov eax, Src
      shl ecx, 1
      sub ecx, 8
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
      jge copying
      EMMS
   }
#endif

#ifdef __GNUC__
    asm ("shl" OP " $1, %0;"
         "sub" OP " $8, %0;"
         "movq  %4, %%mm7;"
         "movq  %3, %%mm6;"
         ".align 16;"
         "MMXConvert4444to4444special_copying:"
         "movq  (%1,%0), %%mm0;"
         "movq  %%mm0, %%mm1;"
         "pand  %%mm6, %%mm0;"
         "pand  %%mm7, %%mm1;"
         "psllq $4, %%mm0;"
         "psllq $12, %%mm1;"
         "por   %%mm1, %%mm0;"
         "movq  %%mm0, (%2,%0);"
         "sub" OP " $8, %0;"
         "jge   MMXConvert4444to4444special_copying;"
         "EMMS;"
         : /* No outputs */
         : "r" ((FxU)NumberOfPixels), "r" (Src), "r" (Dst), /*Inputs */
           "m" (Mask4444_1), "m" (Mask4444_2)
         : "%mm0", "%mm1", "%mm6", "%mm7", "memory" /* Clobbers */
        );
#endif

}

#else

// This functions processes 2 pixels at a time, there is no problem in
// passing odd numbers or a number less than 2 for the pixels, but
// the buffers should be large enough
void Convert4444to4444special( FxU32 *Buffer1, FxU32 *Buffer2, int Pixels )
{
   while ( Pixels > 0 )
   {
      *Buffer2++ = ( ( (*Buffer1) & 0x0FFF0FFF ) << 4 )|
                ( ( (*Buffer1++) & 0xF000F000 ) >> 12 );
      Pixels -= 2;
   }
}

#endif

FxU64 Mask5551_1 = __UINT64_C(0x7FFF7FFF7FFF7FFF);
FxU64 Mask5551_2 = __UINT64_C(0x8000800080008000);

#ifdef HAVE_MMX

// This functions processes 4 pixels at a time, there is no problem in
// passing odd numbers or a number less than 4 for the pixels, but
// the buffers should be large enough
void Convert1555to5551( FxU32 *Src, FxU32 *Dst, int NumberOfPixels )
{

#ifdef _MSC_VER
   __asm
   {
      mov ecx, NumberOfPixels
      mov eax, Src
      shl ecx, 1
      sub ecx, 8
      mov edx, Dst
      movq mm7, [Mask5551_2]
      movq mm6, [Mask5551_1]
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
      jge copying
      EMMS
   }
#endif

#ifdef __GNUC__
    asm ("shl" OP " $1, %0;"
         "sub" OP " $8, %0;"
         "movq  %4, %%mm7;"
         "movq  %3, %%mm6;"
         ".align 16;"
         "MMXConvert1555to5551_copying:"
         "movq  (%1,%0), %%mm0;"
         "movq  %%mm0, %%mm1;"
         "pand  %%mm6, %%mm0;"
         "pand  %%mm7, %%mm1;"
         "psllq $1, %%mm0;"
         "psllq $15, %%mm1;"
         "por   %%mm1, %%mm0;"
         "movq  %%mm0, (%2,%0);"
         "sub" OP " $8, %0;"
         "jge   MMXConvert1555to5551_copying;"
         "EMMS;"
         : /* No outputs */
         : "r" ((FxU)NumberOfPixels), "r" (Src), "r" (Dst), /*Inputs */
           "m" (Mask5551_1), "m" (Mask5551_2)
         : "%mm0", "%mm1", "%mm6", "%mm7", "memory" /* Clobbers */
        );
#endif

}

#else

void Convert1555to5551( FxU32 *Buffer1, FxU32 *Buffer2, int Pixels )
{
   while ( Pixels > 0 )
   {
      *Buffer2++ = ( ( (*Buffer1) & 0x7FFF7FFF ) << 1 )|
                ( ( (*Buffer1++) & 0x80008000 ) >> 15 );
      Pixels -= 2;
   }
}

#endif

FxU64 Mask565A = __UINT64_C(0xFF00FF00FF00FF00);
FxU64 Mask565B = __UINT64_C(0xF800F800F800F800);
FxU64 Mask565G = __UINT64_C(0x07E007E007E007E0);
FxU64 Mask565R = __UINT64_C(0x001F001F001F001F);

#ifdef HAVE_MMX

void Convert565to8888( FxU16 *Src, FxU32 *Dst, FxU32 NumberOfPixels )
{

#ifdef _MSC_VER
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
#endif

#ifdef __GNUC__
    asm ("movq  %3, %%mm7;"
         "movq  %4, %%mm6;"
         "movq  %5, %%mm5;"
         "movq  %6, %%mm4;"
         ".align 16;"
         "MMXConvert565to8888_copying:"
         "movq  (%1), %%mm0;"
         "add" OP " $8, %1;"
         "movq  %%mm0, %%mm2;"
         "movq  %%mm0, %%mm1;"
         "pand  %%mm4, %%mm0;" /* Mask R */
         "pand  %%mm6, %%mm2;" /* Mask B */
         "psllw $11, %%mm0;"   /* Shift R */
         "pand  %%mm5, %%mm1;" /* Mask G */
         "psrlw $8, %%mm2;"    /* Shift B */

         "movq  %%mm1, %%mm3;"
         "psllw $13, %%mm1;"
         "por   %%mm2, %%mm0;"
         "psrlw $3, %%mm3;"
         "por   %%mm3, %%mm1;"
         "por   %%mm7, %%mm1;"

         "movq  %%mm0, %%mm2;"
         "punpckhbw %%mm1, %%mm0;"
         "punpckhbw %%mm1, %%mm2;"

         "movq  %%mm2, (%2);"  /* Storing Unpacked */
         "add" OP " $16, %2;"
         "movq  %%mm0, -8(%2);"
         "sub" OP " $4, %0;"
         "jg    MMXConvert565to8888_copying;"
         "EMMS;"
         : /* No outputs */
         : "r" ((FxU)NumberOfPixels), "r" (Src), "r" (Dst), /*Inputs */
           "m" (Mask565A), "m" (Mask565B), "m" (Mask565G), "m" (Mask565R)
         : "%mm0", "%mm1", "%mm2", "%mm3", "%mm4", "%mm5",
           "%mm6", "%mm7", "memory" /* Clobbers */
        );
#endif

}

#else

void Convert565to8888( FxU16 *Buffer1, FxU32 *Buffer2, FxU32 Pixels )
{
   while ( Pixels )
   {
      *Buffer2++ = 0xFF000000 |              // A
         ( (*Buffer1)    & 0x001F ) << 19 |  // B
         ( (*Buffer1)    & 0x07E0 ) << 5  |  // G
         ( (*Buffer1++)  & 0xF800 ) >> 8;    // R
      Pixels--;
   }
}

#endif

void ConvertA8toAP88( FxU8 *Buffer1, FxU16 *Buffer2, FxU32 Pixels )
{
    while ( Pixels )
    {
#if defined(CONSTANT_LUMINOSITY_ALPHA_TEXTURE)
        *Buffer2 = ( ( ( *Buffer1 ) << 8 ) | 0xFF );
#else
        *Buffer2 = ( ( ( *Buffer1 ) << 8 ) | ( *Buffer1 ) );
#endif
        Buffer1++;
        Buffer2++;
        Pixels--;
    }
}

void Convert8332to8888( FxU16 *Buffer1, FxU32 *Buffer2, FxU32 Pixels )
{
    static FxU32    R, 
                    G, 
                    B, 
                    A;
    for ( FxU32 i = Pixels; i > 0; i-- )
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

void ConvertP8to8888( FxU8 *Buffer1, FxU32 *Buffer2, FxU32 Pixels, FxU32 *palette )
{
    while ( Pixels-- )
    {
        *Buffer2++ = palette[ *Buffer1++ ];
    }
}

void ConvertAI44toAP88( FxU8 *Buffer1, FxU16 *Buffer2, FxU32 Pixels )
{
    for ( FxU32 i = Pixels; i > 0; i-- )
    {
        *Buffer2 = ( ( ( ( *Buffer1 ) & 0xF0 ) << 8 ) | ( ( ( *Buffer1 ) & 0x0F ) << 4 ) );
        Buffer2++;
        Buffer1++;
    }
}

void ConvertAP88to8888( FxU16 *Buffer1, FxU32 *Buffer2, FxU32 Pixels, FxU32 *palette )
{
    FxU32   RGB, 
            A;
    for ( FxU32 i = Pixels; i > 0; i-- )
    {
        RGB = ( palette[ *Buffer1 & 0x00FF ] & 0x00FFFFFF );
        A = *Buffer1 >> 8;
        *Buffer2 = ( A << 24 ) | RGB;
        Buffer1++;
        Buffer2++;
    }
}

void ConvertYIQto8888( FxU8 *in, FxU32 *out, FxU32 Pixels, GuNccTable *ncc )
{
    FxU32  R;
    FxU32  G;
    FxU32  B;

    for ( FxU32 i = Pixels; i > 0; i-- )
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

void ConvertAYIQto8888( FxU16 *in, FxU32 *out, FxU32 Pixels, GuNccTable *ncc)
{
    FxU32  R;
    FxU32  G;
    FxU32  B;

    for ( FxU32 i = Pixels; i > 0; i-- )
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

void SplitAP88( FxU16 *ap88, FxU8 *index, FxU8 *alpha, FxU32 pixels )
{
    for ( FxU32 i = pixels; i > 0; i-- )
    {
        *alpha++ = ( *ap88 >> 8 );
        *index++ = ( *ap88++ & 0xff );
    }
}
