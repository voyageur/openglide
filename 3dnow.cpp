
#include "amd3dx.h"



//******************************************************************************
// Routine:  void _glTrans_4x4 (float *r, const float *a, const float *m)
// Input:    r - result vector (4x1), 4 floats
//           a - input  vector (4x1), 4 floats
//           m - matrix (4x4), 16 floats
// Output:   r = a * m (matrix multiplication rules)
// Uses:     eax, ecx, edx, mm0 - mm7
//******************************************************************************
void _glTrans_4x4 (float *r, const float *a, const float *m)
{
	__asm
	{
//r       =           4
//a       =           8
//m       =           12
        femms
        mov         eax,r
        mov         edx,m
        mov         ecx,a

        movq        mm0,[ecx]		// y   |  x
        movq        mm4,[ecx+8]		// rhw |  z
        movq        mm1,mm0			// y   |  x
        pfmul       (mm0,edx)		// y * m_21 | x * m_11
        movq        mm5,mm4			// rhw |  z
        pfmulm       (mm4,edx,8)		// rhw * m_41 | z * m_31
        movq        mm2,mm1			// y   |  x
        pfmulm       (mm1,edx,16)	// y * m_22 | x * m_12
        movq        mm6,mm5			// rhw |  z
        pfmulm       (mm5,edx,24)	// rhw * m_42 | z * m_32
        movq        mm3,mm2			// y   |  x
        pfmulm       (mm2,edx,32)	// y * m_23 | x * m_13
        movq        mm7,mm6			// rhw | z
        pfmulm       (mm6,edx,40)	// rhw * m_43 | z * m_33
        pfmulm       (mm3,edx,48)	// y * m_24 | x * m_14
        pfacc       (mm0,mm4)			// rhw * m_41 + z * m_31 | y * m_21 + x * m_11 
        pfmulm       (mm7,edx,56)	// rhw * m_44 | z * m_34
        pfacc       (mm1,mm5)			// rhw * m_42 + z * m_32 | y * m_22 + x * m_12
        pfacc       (mm2,mm6)			// rhw * m_43 + z * m_33 | y * m_23 + x * m_13
        pfacc       (mm3,mm7)			// rhw * m_44 + z * m_34 | y * m_24 | x * m_14 
        pfacc       (mm0,mm1)			// rhw * m_41 + z * m_31 + y * m_21 + x * m_11 | rhw * m_42 + z * m_32 + y * m_22 + x * m_12
        pfacc       (mm2,mm3)			// rhw * m_44 + z * m_34 + y * m_24 | x * m_14 | rhw * m_43 + z * m_33 + y * m_23 + x * m_13
        movq        [eax],mm0		// r_y   | r_x
        movq        [eax+8],mm2		// r_rhw | r_z
        femms
	}
}



//******************************************************************************
// Routine:  void _glMul_4x4 (const float *r, const float *a, const float *b)
// Input:    r - matrix (4x4) address
//           a - matrix (4x4) address
//           b - matrix (4x4) address
// Output:   r = a * b, using standard matrix multiplication rules
// Uses:     eax, ecx, edx, mm0 - mm7
//******************************************************************************
void _glMul_4x4 (const float *r, const float *a, const float *b)
{
	__asm
	{
//r       =           4
//a       =           8
//b       =           12
        femms
        mov         ecx, a //[esp+a]
        mov         eax, r //[esp+r]
        mov         edx, b //[esp+b]

        movq        mm0,[ecx]		// a_21	| a_11
        movq        mm1,[ecx+8]		// a_41	| a_31
        movq        mm4,[edx]		// b_21	| b_11
        punpckhdq   mm2,mm0			// a_21	| 
        movq        mm5,[edx+16]	// b_22	| b_12
        punpckhdq   mm3,mm1			// a_41	| 
        movq        mm6,[edx+32]	// b_23	| b_13
        punpckldq   mm0,mm0			// a_11	| a_11
        punpckldq   mm1,mm1			// a_31	| a_31
        pfmul       (mm4,mm0)			// a_11 * b_21 | a_11 * b_11
        punpckhdq   mm2,mm2			// a_21	| a_21
        pfmulm       (mm0,edx,8)		// a_11 * b_41 | a_11 * b_31
        movq        mm7,[edx+48]	// b_24	| b_14
        pfmul       (mm5,mm2)			// a_21 * b_22 | a_21 * b_12
        punpckhdq   mm3,mm3			// a_41	| a_41
        pfmulm       (mm2,edx,24)	// a_21 * b_42 | a_21 * b_32
        pfmul       (mm6,mm1)			// a_31 * b_23 | a_31 * b_13 
        pfadd       (mm5,mm4)			// a_21 * b_22 + a_11 * b_21 | a_21 * b_12 + a_11 * b_11
        pfmulm       (mm1,edx,40)	// a_31 * b_43 | a_31 * b_33
        pfadd       (mm2,mm0)			// a_21 * b_42 + a_11 * b_41 | a_21 * b_32 + a_11 * b_31
        pfmul       (mm7,mm3)			// a_41 * b_24 | a_41 * b_14 
        pfadd       (mm6,mm5)			// a_21 * b_22 + a_11 * b_21 + a_31 * b_23 | a_21 * b_12 + a_11 * b_11 + a_31 * b_13
        pfmulm       (mm3,edx,56)	// a_41 * b_44 | a_41 * b_34
        pfadd       (mm2,mm1)			// a_21 * b_42 + a_11 * b_41 + a_31 * b_43 | a_21 * b_32 + a_11 * b_31 + a_31 * b_33 
        pfadd       (mm7,mm6)			// a_41 * b_24 + a_21 * b_22 + a_11 * b_21 + a_31 * b_23 |  a_41 * b_14 + a_21 * b_12 + a_11 * b_11 + a_31 * b_13
        movq        mm0,[ecx+16]	// a_22	| a_12
        pfadd       (mm3,mm2)			// a_41 * b_44 + a_21 * b_42 + a_11 * b_41 + a_31 * b_43 | a_41 * b_34 + a_21 * b_32 + a_11 * b_31 + a_31 * b_33 
        movq        mm1,[ecx+24]	// a_42	| a_32
        movq        [eax],mm7		// r_21	| r_11 
        movq        mm4,[edx]		// b_21	| b_11
        movq        [eax+8],mm3		// r_41	| r_31

        punpckhdq   mm2,mm0			// a_22	| XXX
        movq        mm5,[edx+16]	// b_22	| b_12
        punpckhdq   mm3,mm1			// a_42	| XXX
        movq        mm6,[edx+32]	// b_23	| b_13
        punpckldq   mm0,mm0			// a_12	| a_12
        punpckldq   mm1,mm1			// a_32	| a_32
        pfmul       (mm4,mm0)			// a_12 * b_21 | a_12 * b_11
        punpckhdq   mm2,mm2			// a_22	| a_22
        pfmulm       (mm0,edx,8)		// a_12 * b_41 | a_12 * b_31
        movq        mm7,[edx+48]	// b_24	| b_14
        pfmul       (mm5,mm2)			// a_22 * b_22 | a_22 * b12
        punpckhdq   mm3,mm3			// a_42	| a_42
        pfmulm       (mm2,edx,24)	// a_22 * b_42 | a_22 * b_32
        pfmul       (mm6,mm1)			// a_32 * b_23 | a_32 * b_13
        pfadd       (mm5,mm4)			// a_12 * b_21 + a_22 * b_22 | a_12 * b_11 + a_22 * b12
        pfmulm       (mm1,edx,40)	// a_32 * b_43 | a_32 * b_33
        pfadd       (mm2,mm0)			// a_12 * b_41 + a_22 * b_42 | a_12 * b_11 + a_22 * b_32
        pfmul       (mm7,mm3)			// a_42 * b_24 | a_42 * b_14
        pfadd       (mm6,mm5)			// a_32 * b_23 + a_12 * b_21 + a_22 * b_22 | a_32 * b_13 + a_12 * b_11 + a_22 * b12
        pfmulm       (mm3,edx,56)	// a_42 * b_44 | a_42 * b_34
        pfadd       (mm2,mm1)			// a_32 * b_43 + a_12 * b_41 + a_22 * b_42 | a_32 * b_33 + a_12 * b_11 + a_22 * b_32
        pfadd       (mm7,mm6)			// a_42 * b_24 + a_32 * b_23 + a_12 * b_21 + a_22 * b_22 | a_42 * b_14 + a_32 * b_13 + a_12 * b_11 + a_22 * b12
        movq        mm0,[ecx+32]	// a_23 | a_13
        pfadd       (mm3,mm2)			// a_42 * b_44 + a_32 * b_43 + a_12 * b_41 + a_22 * b_42 | a_42 * b_34 + a_32 * b_33 + a_12 * b_11 + a_22 * b_32
        movq        mm1,[ecx+40]	// a_43 | a_33
        movq        [eax+16],mm7	// r_22 | r_12
        movq        mm4,[edx]		// b_21	| b_11
        movq        [eax+24],mm3	// r_42 | r_32

        punpckhdq   mm2,mm0			// a_23 | XXX
        movq        mm5,[edx+16]	// b_22 | b_12
        punpckhdq   mm3,mm1			// a_43 | XXX
        movq        mm6,[edx+32]	// b_23 | b_13
        punpckldq   mm0,mm0			// a_13 | a_13
        punpckldq   mm1,mm1			// a_33 | a_33
        pfmul       (mm4,mm0)			// a_13 * b_21 | a_13 * b_11
        punpckhdq   mm2,mm2			// a_23 | a_23
        pfmulm       (mm0,edx,8)		// a_13 * b_41 | a_13 * b_31
        movq        mm7,[edx+48]	// b_24 | b_14
        pfmul       (mm5,mm2)			// a_23 * b_22 | a_23 * b_12
        punpckhdq   mm3,mm3			// a_43 | a_43
        pfmulm       (mm2,edx,24)	// a_23 * b_42 | a_23 * b_32
        pfmul       (mm6,mm1)			// a_33 * b_23 | a_33 * b_13
        pfadd       (mm5,mm4)			// a_23 * b_22 + a_13 * b_21 | a_23 * b_12 + a_13 * b_11
        pfmulm       (mm1,edx,40)	// a_33 * b_43 | a_33 * b_33 
        pfadd       (mm2,mm0)			// a_13 * b_41 + a_23 * b_42 | a_13 * b_31 + a_23 * b_32
        pfmul       (mm7,mm3)			// a_43 * b_24 | a_43 * b_14
        pfadd       (mm6,mm5)			// a_33 * b_23 + a_23 * b_22 + a_13 * b_21 | a_33 * b_13 + a_23 * b_12 + a_13 * b_11
        pfmulm       (mm3,edx,56)	// a_43 * b_44 | a_43 * b_34
        pfadd       (mm2,mm1)			// a_33 * b_43 * a_13 * b_41 + a_23 * b_42 | a_33 * b_33 + a_13 * b_31 + a_23 * b_32
        pfadd       (mm7,mm6)			// a_43 * b_24 + a_33 * b_23 + a_23 * b_22 + a_13 * b_21 | a_43 * b_14 + a_33 * b_13 + a_23 * b_12 + a_13 * b_11
        movq        mm0,[ecx+48]	// a_24 | a_14
        pfadd       (mm3,mm2)			// a_43 * b_44 + a_33 * b_43 * a_13 * b_41 + a_23 * b_42 | a_43 * b_34 + a_33 * b_33 + a_13 * b_31 + a_23 * b_32
        movq        mm1,[ecx+56]	// a_44 | a_34
        movq        [eax+32],mm7	// r_23 | r_13
        movq        mm4,[edx]		// b_21 | b_11
        movq        [eax+40],mm3	// r_43 | r_33

        punpckhdq   mm2,mm0			// a_24 | XXX
        movq        mm5,[edx+16]	// b_22 | b_12
        punpckhdq   mm3,mm1			// a_44 | XXX
        movq        mm6,[edx+32]	// b_23 | b_13
        punpckldq   mm0,mm0			// a_14 | a_14
        punpckldq   mm1,mm1			// a_34 | a_34
        pfmul       (mm4,mm0)			// a_14 * b_21 | a_14 * b_11
        punpckhdq   mm2,mm2			// a_24 | a_24
        pfmulm       (mm0,edx,8)		// a_14 * b_41 | a_14 * b_31
        movq        mm7,[edx+48]	// b_24 | b_14
        pfmul       (mm5,mm2)			// a_24 * b_22 | a_24 * b_12
        punpckhdq   mm3,mm3			// a_44 | a_44
        pfmulm       (mm2,edx,24)	// a_24 * b_ 42 | a_24 * b_32
        pfmul       (mm6,mm1)			// a_34 * b_23 | a_34 * b_13
        pfadd       (mm5,mm4)			// a_14 * b_21 + a_24 * b_22 | a_14 * b_11 + a_24 * b_12
        pfmulm       (mm1,edx,40)	// a_34 * b_43 | a_34 * b_33
        pfadd       (mm2,mm0)			// a_14 * b_41 + a_24 * b_ 42 | a_14 * b_31 + a_24 * b_32
        pfmul       (mm7,mm3)			// a_44 * b_24 | a_44 * b_14
        pfadd       (mm6,mm5)			// a_34 * b_23 + a_14 * b_21 + a_24 * b_22 | a_34 * b_13 + a_14 * b_11 + a_24 * b_12
        pfmulm       (mm3,edx,56)	// a_44 * b_44 | a_44 * b_34
        pfadd       (mm2,mm1)			// a_34 * b_43 + a_14 * b_41 + a_24 * b_ 42 | a_34 * b_33 + a_14 * b_31 + a_24 * b_32
        pfadd       (mm7,mm6)			// a_44 * b_24 + a_14 * b_23 + a_24 * b_ 42 | a_44 * b_14 + a_14 * b_31 + a_24 * b_32
        pfadd       (mm3,mm2)			// a_44 * b_44 + a_34 * b_43 + a_14 * b_41 + a_24 * b_42 | a_44 * b_34 + a_34 * b_33 + a_14 * b_31 + a_24 * b_32
        movq        [eax+48],mm7	// r_24 | r_14
        movq        [eax+56],mm3	// r_44 | r_34
        femms
	}
}


void MultSum3( float *r, float *m, float *s, float *v )
{
	__asm
	{
//		femms
		mov		edx, v
		mov		ecx, s
		mov		ebx, m
		
		movq	mm0, [edx]
		mov		eax, r
		movq	mm1, [edx+8]
		movq	mm3, [edx+24]
		pfmul	(mm0, ebx)
		pfadd	(mm0, ecx)
		movq	mm2, [edx+16]
		pfmulm	(mm1, ebx, 8)
		pfaddm	(mm1, ecx, 8)
		movq	[eax], mm0
		pfmulm	(mm2, ebx, 16)
		pfaddm	(mm2, ecx, 16)
		movq	[eax+8], mm1
		pfmulm	(mm3, ebx, 24)
		pfaddm	(mm3, ecx, 24)
		movq	[eax+16], mm2
		movq	[eax+24], mm3
		movq	mm4, [edx+32]
		movq	mm5, [edx+40]
		pfmulm	(mm4, ebx, 32)
		pfaddm	(mm4, ecx, 32)
		movq	[eax+32], mm4
		pfmulm	(mm5, ebx, 40)
		pfaddm	(mm5, ecx, 40)
		movq	[eax+40], mm5
		femms
	}
}

