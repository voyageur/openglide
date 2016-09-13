/* $Id$ */
#include "glidedef.h"

/* DOS WATCOM compiler does not work with C-library (?) */
#ifdef __WATCOMC__

#ifndef NULL
#define NULL (void*)0
#endif

void putc(char c);
#pragma aux putc =	\
	"mov	ah, 2"	\
	"int	21h"	\
	parm [dl]		\
	modify [ah];

unsigned int strlen(const char *s)
{
	unsigned int i = 0;
	while(s[i] != 0) i++;
	return i;
}

void printi(int i, unsigned int base)
{
	if(i < 0) {
		putc('-');
		i = -i;
	}

	if(i > base-1)
		printi(i / base, base);

	if(i % base < 10)
		putc((i % base) + '0');
	else
		putc((i % base) - 10 + 'a');
}

void printu(unsigned int i, unsigned int base)
{
	if(i > base-1)
		printi(i / base, base);

	if(i % base < 10)
		putc((i % base) + '0');
	else
		putc((i % base) - 10 + 'a');
}

void __cdecl printf(const char *s, ...)
{
	unsigned int i = 0;
	unsigned int j = 0;

	char *tmp;
	// Offset for the parameter stack
	unsigned int off=0x18;

	i = strlen(s);

	for(j=0;j<i;j++) {
		if(s[j] == '%') {
			j++;
			if(j<i) {
				__asm {
					push	eax
					push	esi
					mov		esi, off
					mov		eax, [ebp+esi]			// Take parameters from the stack
					add		esi, 0x4
					mov		tmp, eax
					mov		off, esi
					pop		esi
					pop		eax
				}
				switch(s[j]) {
					case 'd': printi((int)tmp, 10);
							  break;
					case 'u': printu((unsigned int)tmp, 10);
							  break;
					case 'x': printu((unsigned int)tmp, 16);
							  break;
					case 's': printf(tmp);
							  break;
					default : putc(s[j]);
							  off -= 0x4;			// Decrement stack offset since we didn't use the parameter
							  break;
				}
			}
			continue;
		}
		putc(s[j]);
	}
}

void outp(short port, char value);
#pragma aux outp =	\
	"out	dx, al" \
	parm [dx] [al];

char inp(short port);
#pragma aux inp =	\
	"in		al, dx"	\
	parm [dx]		\
	value [al];

const char * strchr(const char *s, const char c)
{
	while(*s != c) {
		if(*s == 0) return NULL;
		s++;
	}
	return s;
}

int strncmp(const char *s1, const char *s2, unsigned int len)
{
	unsigned int i = strlen(s1);
	unsigned int j = strlen(s2);
	if((i < len) || (j < len)) return 1;

	for(j = 0; j < len; j++) {
		if(s1[j] != s2[j]) return 1;
	}

	return 0;
}

// DOSBox define (!)
#define DOS_FIRST_SHELL 0x118

const char* getenv(const char *s)
{
	const char* val=NULL;
	const char* env=(const char*)((DOS_FIRST_SHELL+19)<<4);

	if(!s) return NULL;

	while(*env != 0) {
		//printf("Found ENV: %s\n", env);
		if(!strncmp(env, s, strlen(s))) break;
		env += strlen(env) + 1;
	}

	if(*env != 0) {
		val = strchr(env, '=');
		if(val != NULL) {
			val++;
			//printf("Found %s value: %s\n", s, val);
		}
	}

	return val;
}

#endif // __WATCOMC__

unsigned int atoh(const char *s)
{
	unsigned int i=0, val=0;

	while(s[i] != 0) {
		val <<= 4;
		if((s[i] <= '9') && (s[i] >= '0'))
			val += s[i] - '0';
		if((s[i] <= 'F') && (s[i] >= 'A'))
			val += s[i] - 'A';
		if((s[i] <= 'f') && (s[i] >= 'a'))
			val += s[i] - 'a';
		i++;
	}

	return val;
}


#ifndef __WATCOMC__

// VWIN32 DPMI services
#define VWIN32_DPMI_CALL 0x2A0029

// KERNEL32's undocumented VxDCall function is exported as ordinal #1
DWORD WINAPI GetK32ProcAddress(int ord);
DWORD (WINAPI *VxDCall)(DWORD Service, DWORD EAX_Reg, DWORD ECX_Reg) = NULL;
#define DPMICall(EAX_Reg, ECX_Reg) VxDCall(VWIN32_DPMI_CALL, EAX_Reg, ECX_Reg)

#endif


void *MapPhysToLin(void *physaddr, unsigned int size)
{
	void *linaddr;
	unsigned int l = size;

	__asm
	{
		push ebx;
		push esi;
		push edi;
		mov bx, WORD PTR [physaddr + 2]
		mov cx, WORD PTR [physaddr]
		mov si, WORD PTR [l + 2]
		mov di, WORD PTR [l]

#ifdef __WATCOMC__

		// Call DPMI function MapPhysicalToLinear (0x800)
		mov ax, 800h
		int 31h

#else
	}

		if(VxDCall == NULL) VxDCall = (DWORD (WINAPI *)(DWORD,DWORD,DWORD))GetK32ProcAddress(1);
		DPMICall(0x800, (DWORD)physaddr);

	__asm
	{
#endif

		jnc success
		xor bx, bx
		xor cx, cx

success:mov WORD PTR [linaddr + 2], bx
		mov WORD PTR [linaddr], cx
		pop edi;
		pop esi;
		pop ebx;
	}

	return linaddr;
}
