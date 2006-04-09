#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "glide.h"
#include "glide2x.h"

#define VERSION 11

#define __export
#define __stdcall __attribute__((__stdcall__))

#define __fptr(type) \
struct fptr_##type##_t \
{ \
    void          *entry; \
    type##_t      *thunk; \
    unsigned short segment; \
    unsigned int   offset; \
}

/*******************************************************************************************************/
#ifdef DOSEMU

#include "config.h"
#include "cpu.h"

extern unsigned long SEL_ADR(unsigned short sel, unsigned long reg);


#define THUNK(func) THUNK_##func

#define ENDDECLARE }}

#define DECLARE_THUNK0(func,ret) \
static void THUNK(func)(struct sigcontext *scp) { \

#define DECLARE_THUNK1(func,ret,t1,v1) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
    }

#define DECLARE_THUNK2(func,ret,t1,v1,t2,v2) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; t2 v2; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
        v2 = (t2) DWORD(stack[1]); \
    }

#define DECLARE_THUNK3(func,ret,t1,v1,t2,v2,t3,v3) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; t2 v2; t3 v3; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
        v2 = (t2) DWORD(stack[1]); \
        v3 = (t3) DWORD(stack[2]); \
    }

#define DECLARE_THUNK4(func,ret,t1,v1,t2,v2,t3,v3,t4,v4) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; t2 v2; t3 v3; t4 v4; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
        v2 = (t2) DWORD(stack[1]); \
        v3 = (t3) DWORD(stack[2]); \
        v4 = (t4) DWORD(stack[3]); \
    }

#define DECLARE_THUNK5(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; t2 v2; t3 v3; t4 v4; t5 v5; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
        v2 = (t2) DWORD(stack[1]); \
        v3 = (t3) DWORD(stack[2]); \
        v4 = (t4) DWORD(stack[3]); \
        v5 = (t5) DWORD(stack[4]); \
    }

#define DECLARE_THUNK6(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; t2 v2; t3 v3; t4 v4; t5 v5; t6 v6; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
        v2 = (t2) DWORD(stack[1]); \
        v3 = (t3) DWORD(stack[2]); \
        v4 = (t4) DWORD(stack[3]); \
        v5 = (t5) DWORD(stack[4]); \
        v6 = (t6) DWORD(stack[5]); \
    }

#define DECLARE_THUNK7(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; t2 v2; t3 v3; t4 v4; t5 v5; t6 v6; t7 v7; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
        v2 = (t2) DWORD(stack[1]); \
        v3 = (t3) DWORD(stack[2]); \
        v4 = (t4) DWORD(stack[3]); \
        v5 = (t5) DWORD(stack[4]); \
        v6 = (t6) DWORD(stack[5]); \
        v7 = (t7) DWORD(stack[6]); \
    }

#define DECLARE_THUNK8(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7,t8,v8) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; t2 v2; t3 v3; t4 v4; t5 v5; t6 v6; t7 v7; t8 v8; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
        v2 = (t2) DWORD(stack[1]); \
        v3 = (t3) DWORD(stack[2]); \
        v4 = (t4) DWORD(stack[3]); \
        v5 = (t5) DWORD(stack[4]); \
        v6 = (t6) DWORD(stack[5]); \
        v7 = (t7) DWORD(stack[6]); \
        v8 = (t8) DWORD(stack[7]); \
    }

#define DECLARE_THUNK15(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7,t8,v8,t9,v9,t10,v10,t11,v11,t12,v12,t13,v13,t14,v14,t15,v15) \
static void THUNK(func)(struct sigcontext *scp) { \
    t1 v1; t2 v2; t3 v3; t4 v4; t5 v5; t6 v6; t7 v7; t8 v8; t9 v9; \
    t10 v10; t11 v11; t12 v12; t13 v13; t14 v14; t15 v15; \
    {   unsigned long *stack = (unsigned long *)SEL_ADR(_ss, _esp) + 2; \
        v1 = (t1) DWORD(stack[0]); \
        v2 = (t2) DWORD(stack[1]); \
        v3 = (t3) DWORD(stack[2]); \
        v4 = (t4) DWORD(stack[3]); \
        v5 = (t5) DWORD(stack[4]); \
        v6 = (t6) DWORD(stack[5]); \
        v7 = (t7) DWORD(stack[6]); \
        v8 = (t8) DWORD(stack[7]); \
        v9 = (t9) DWORD(stack[8]); \
        v10 = (t10) DWORD(stack[9]); \
        v11 = (t11) DWORD(stack[10]); \
        v12 = (t12) DWORD(stack[11]); \
        v13 = (t13) DWORD(stack[12]); \
        v14 = (t14) DWORD(stack[13]); \
        v15 = (t15) DWORD(stack[14]); \
    }

#define RETURNI(v) _eax = v;


/*******************************************************************************************************/
#else /* WRAPPERS DJGPP */

#define __faddr(addr) asm volatile ("mov %1, %%eax; add %%eax, %0": "=g"(addr): "m"(fptr.offset): "%eax")

#define __fcall(...) \
    (*(fptr.thunk)) (__VA_ARGS__)

#define DECLARE_THUNK(func,ret,...) \
typedef ret (func##_t) (__VA_ARGS__); \
ret func (__VA_ARGS__) \
{ \
    static __fptr(func) fptr = {0}; \
    printf ("%s\n", __func__); \
    if (!fptr.entry) \
    { \
        fptr.entry = entry (CMD_##func, &fptr.segment, &fptr.offset); \
        assert (fptr.entry); \
        asm volatile ( \
            "movl $1f, %0\n\t" \
            "jmp  2f\n\t" \
            "1:\n\t" \
            "pop  %%eax\n\t" \
            "push %%cs\n\t" \
            "push %%eax\n\t" \
            "push %2\n\t" \
            "push %3\n\t" \
            "mov  %4, %%eax\n\t" \
            "lret\n\t" \
            "2:" \
            :"=m"(fptr.thunk), "=m"(fptr.offset) \
            /* For now don't have unique entry per command */ \
            :"m"(fptr.segment), "m"(vxd.offset), "m"(fptr.entry)); \
            /*:"m"(fptr.segment), "m"(fptr.entry),  "g"(CMD_##func));*/ \
    } {

#define DECLARE_STUB(func,ret,...) \
ret func (__VA_ARGS__) \
{ \
    printf ("%s\n", __func__); {

#define ENDDECLARE }}

#define DECLARE_THUNK0(func,ret) \
    DECLARE_THUNK(func,ret)
#define DECLARE_THUNK1(func,ret,t1,v1) \
    DECLARE_THUNK(func,ret,t1 v1)
#define DECLARE_THUNK2(func,ret,t1,v1,t2,v2) \
    DECLARE_THUNK(func,ret,t1 v1,t2 v2)
#define DECLARE_THUNK3(func,ret,t1,v1,t2,v2,t3,v3) \
    DECLARE_THUNK(func,ret,t1 v1,t2 v2,t3 v3)
#define DECLARE_THUNK4(func,ret,t1,v1,t2,v2,t3,v3,t4,v4) \
    DECLARE_THUNK(func,ret,t1 v1,t2 v2,t3 v3,t4 v4)
#define DECLARE_THUNK5(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5) \
    DECLARE_THUNK(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5)
#define DECLARE_THUNK6(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6) \
    DECLARE_THUNK(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6)
#define DECLARE_THUNK7(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7) \
    DECLARE_THUNK(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6,t7 v7)
#define DECLARE_THUNK8(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7,t8,v8) \
    DECLARE_THUNK(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6,t7 v7,t8 v8)
#define DECLARE_THUNK15(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7,t8,v8,t9,v9,t10,v10,t11,v11,t12,v12,t13,v13,t14,v14,t15,v15) \
    DECLARE_THUNK(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6,t7 v7,t8 v8,t9 v9,t10 v10,t11 v11,t12 v12,t13 v13,t14 v14,t15 v15)

#define DECLARE_STUB0(func,ret) \
    DECLARE_STUB(func,ret)
#define DECLARE_STUB1(func,ret,t1,v1) \
    DECLARE_STUB(func,ret,t1 v1)
#define DECLARE_STUB2(func,ret,t1,v1,t2,v2) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2)
#define DECLARE_STUB3(func,ret,t1,v1,t2,v2,t3,v3) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2,t3 v3)
#define DECLARE_STUB4(func,ret,t1,v1,t2,v2,t3,v3,t4,v4) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2,t3 v3,t4 v4)
#define DECLARE_STUB5(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5)
#define DECLARE_STUB6(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6)
#define DECLARE_STUB7(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6,t7 v7)
#define DECLARE_STUB8(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7,t8,v8) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6,t7 v7,t8 v8)
#define DECLARE_STUB10(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7,t8,v8,t9,v9,t10,v10) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6,t7 v7,t8 v8,t9 v9,t10 v10)
#define DECLARE_STUB12(func,ret,t1,v1,t2,v2,t3,v3,t4,v4,t5,v5,t6,v6,t7,v7,t8,v8,t9,v9,t10,v10,t11,v11,t12,v12) \
    DECLARE_STUB(func,ret,t1 v1,t2 v2,t3 v3,t4 v4,t5 v5,t6 v6,t7 v7,t8 v8,t9 v9,t10 v10,t11 v11,t12 v12)

#define RETURNI(v) return v

static struct vxd_driver
{
    const    char  name[8];
    unsigned short segment;
    unsigned int   offset;
} vxd =
{
    .name[0] = 'O',
    .name[1] = 'P',
    .name[2] = 'E',
    .name[3] = 'N',
    .name[4] = 'G',
    .name[5] = 'L',
    .name[6] = 'D',
    .name[7] = '1',
    .segment = 0,
    .offset  = 0
};

static void *entry (unsigned int cmd, unsigned short *segment, unsigned int *offset)
{
    void *func;

    if (!(vxd.segment || vxd.offset))
    {
        asm volatile (
             "push %%es\n\t"
             "push %%edi\n\t"
             "mov %2, %%edi\n\t"
             "mov $0x1684, %%eax\n\t"
             "mov $0, %%ebx\n\t"
             "int $0x2f\n\t"
             "mov %%es, %0\n\t"
             "mov %%edi, %1\n\t"
             "pop %%edi\n\t"
             "pop %%es"
             : "=m"(vxd.segment), "=m"(vxd.offset) /* Outputs */
             : "r"(vxd.name)                       /* Inputs */
             : "%eax", "%ebx", "memory"            /* Clobbers */
            );
        assert (vxd.segment || vxd.offset);
    }

    asm volatile (
         "mov  %2, %%eax\n\t"
         "push %%cs\n\t"
         "push $1f\n\t"
         "push %3\n\t"
         "push %4\n\t"
         "lret\n"
         "1:\n\t"
         "mov  %%eax, %0\n\t"
         "mov  %%edx, %1"
         : "=r"(func), "=g"(*offset)         /* Outputs */
         : "r"(cmd),
           "m"(vxd.segment), "m"(vxd.offset) /* Inputs */
         : "%eax", "%edx", "memory"          /* Clobbers */
        );

    *segment = vxd.segment;
    return func;
}

/*******************************************************************************************************/
#endif /* END WRAPPERS */

#include "thunks.i"


DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_4_8, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_4_WIDES, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_8_1, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_8_2, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_8_4, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_8_WIDES, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_16_1, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_16_2, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_16_WIDES, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_32_1, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(_GRTEXDOWNLOAD_DEFAULT_32_WIDES, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB5(GRSPLASH, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB2(GU3DFGETINFO, FxBool, char*, filename, Gu3dfInfo*, file_info)
    /*
     * Pretend file doesn't exist to get TRI to
     * generate textures on the fly.
     * FIXME: wont work on most games, probably
     */
    return FXFALSE;
/*
    FxBool ret;
    __faddr (filename);
    __faddr (file_info);
    ret = __fcall (filename, file_info);
    RETURNI(ret);
*/
ENDDECLARE

DECLARE_STUB2(GU3DFLOAD, FxBool, char*, filename, Gu3dfInfo*, file_info)
    return FXFALSE;
/*
    FxBool ret;
    __faddr (filename);
    __faddr (file_info);
    ret = __fcall (filename, file_info);
    RETURNI(ret);
*/
ENDDECLARE

DECLARE_STUB1(GUENDIANSWAPWORDS, void, int, a)
ENDDECLARE

DECLARE_STUB1(GUENDIANSWAPBYTES, void, int, a)
ENDDECLARE

DECLARE_STUB1(GUFOGTABLEINDEXTOW, void, int, a)
ENDDECLARE

DECLARE_STUB2(GUFOGGENERATEEXP2, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB3(GUFOGGENERATELINEAR, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB0(GUTEXCREATECOLORMIPMAP, void)
ENDDECLARE

DECLARE_STUB4(GUENCODERLE16, void, int, a, int, b, int, c, int, d)
ENDDECLARE

DECLARE_STUB3(GUAADRAWTRIANGLEWITHCLIP, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB2(GUDRAWPOLYGONVERTEXLISTWITHCLIP, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB0(GUMPINIT, void)
ENDDECLARE

DECLARE_STUB1(GUMPTEXCOMBINEFUNCTION, void, int, a)
ENDDECLARE

DECLARE_STUB2(GUMPTEXSOURCE, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB6(GUFBREADREGION, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(GUFBWRITEREGION, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB1(GRGLIDEGETVERSION, void, char*, version)
    memcpy(version, "2.48.00.0455", 13);
ENDDECLARE

DECLARE_STUB1(GRGLIDEGETSTATE, void, int, a)
ENDDECLARE

DECLARE_STUB1(_GRGLIDEINIT, void, int, version)
#warning TODO
ENDDECLARE

DECLARE_STUB1(GRGLIDESHAMELESSPLUG, void, int, a)
ENDDECLARE

DECLARE_STUB0(GRRESETTRISTATS, void)
ENDDECLARE

DECLARE_STUB2(GRTRISTATS, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB1(GRSSTQUERYBOARDS, void, int, a)
ENDDECLARE

DECLARE_STUB0(GRSSTSCREENWIDTH, void)
ENDDECLARE

DECLARE_STUB0(GRSSTSCREENHEIGHT, void)
ENDDECLARE

DECLARE_STUB2(GRSSTVIDMODE, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB4(GRTEXCALCMEMREQUIRED, FxU32, GrLOD_t, lodmin, GrLOD_t, lodmax, GrAspectRatio_t, aspect, GrTextureFormat_t, fmt)
#if 0
    int size, i;

    size = 0;

    for(i = lodmax; i <= lodmin; i++)
        size += texture_size(i, aspect, fmt);

    DPRINTF("size = %x\n", size);

    return size;
#endif
    return 0;
ENDDECLARE

/* FIXME? */
DECLARE_STUB4(GRTEXDETAILCONTROL, void, int, a, int, b, int, c, int, d)
#if 0
    send_char(CMD_GrTexDetailControl);
#endif
ENDDECLARE

/* FIXME? */
DECLARE_STUB1(GRTEXMINADDRESS, FxU32, int, a)
    return 8;
ENDDECLARE

/* FIXME? */
DECLARE_STUB1(GRTEXMAXADDRESS, FxU32, int, a)
    return 8*1024*1024 - 128*1024;
ENDDECLARE

/* FIXME */
DECLARE_STUB2(GRTEXTEXTUREMEMREQUIRED, int, FxU32, evenOdd, GrTexInfo*, info)
    return 0;
ENDDECLARE

DECLARE_STUB5(GRTEXDOWNLOADTABLEPARTIAL, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

/* FIXME? */
DECLARE_STUB8(GRTEXDOWNLOADMIPMAPLEVEL, void,
                                        GrChipID_t,        tmu,
                                        FxU32,             startAddress,
                                        GrLOD_t,           thisLod,
                                        GrLOD_t,           largeLod,
                                        GrAspectRatio_t,   aspectRatio,
                                        GrTextureFormat_t, format,
                                        FxU32,             evenOdd,
                                        void*,             data)
    GrTexInfo info;

    if(thisLod == largeLod)
    {
        info.aspectRatio = aspectRatio;
        info.data        = data;
        info.format      = format;
        info.largeLod    = largeLod;
        info.smallLod    = largeLod;

        GRTEXDOWNLOADMIPMAP(tmu, startAddress, evenOdd, &info);
    }
ENDDECLARE

DECLARE_STUB1(GRERRORSETCALLBACK, void, int, a)
ENDDECLARE

DECLARE_STUB0(GUMOVIESTART, void)
ENDDECLARE

DECLARE_STUB0(GUMOVIESTOP, void)
ENDDECLARE

DECLARE_STUB1(GUMOVIESETNAME, void, int, a)
ENDDECLARE

DECLARE_STUB12(GUTEXCHANGEATTRIBUTES, void, int, a, int, b, int, c, int, d,
                                      int, e, int, f, int, g, int, h,
                                      int, i, int, j, int, k, int, l)
ENDDECLARE

/* FIXME */
DECLARE_STUB2(GUTEXCOMBINEFUNCTION, void, int, a, int, b)
/*    __fcall (a, b); */
ENDDECLARE

DECLARE_STUB3(GUTEXDOWNLOADMIPMAPLEVEL, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB1(GUTEXGETCURRENTMIPMAP, void, int, a)
ENDDECLARE

DECLARE_STUB1(GUTEXGETMIPMAPINFO, void, int, a)
ENDDECLARE

/* FIXME */
DECLARE_STUB1(GUTEXMEMQUERYAVAIL, FxU32, int, a)
    return 16 * 1024 * 1024;
ENDDECLARE

DECLARE_STUB1(_GUMPTEXCOMBINEFUNCTION, void, int, a)
ENDDECLARE

DECLARE_STUB3(GUMPDRAWTRIANGLE, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB1(GRAADRAWPOINT, void, int, a)
ENDDECLARE

DECLARE_STUB6(GRAADRAWTRIANGLE, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB3(GRAADRAWPOLYGON, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB2(GRAADRAWPOLYGONVERTEXLIST, void, int, nverts, GrVertex*, vlist)
ENDDECLARE

DECLARE_STUB1(GRDRAWPOINT, void, int, a)
ENDDECLARE

DECLARE_STUB3(GRDRAWPLANARPOLYGON, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB2(GRDRAWPLANARPOLYGONVERTEXLIST, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB3(GRDRAWPOLYGON, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB1(GRALPHACONTROLSITRGBLIGHTING, void, int, a)
ENDDECLARE

DECLARE_STUB1(GRALPHATESTREFERENCEVALUE, void, int, a)
ENDDECLARE

/* FIXME? */
DECLARE_STUB0(GRBUFFERNUMPENDING, int)
    return 0;
ENDDECLARE

DECLARE_STUB1(GRGLIDESETSTATE, void, int, a)
ENDDECLARE

DECLARE_STUB1(GRCHECKFORROOM, void, int, a)
ENDDECLARE

DECLARE_STUB0(_GRUPDATEPARAMINDEX, void)
ENDDECLARE

DECLARE_STUB0(_GRREBUILDDATALIST, void)
ENDDECLARE

DECLARE_STUB1(GRLFBCONSTANTALPHA, void, int, a)
ENDDECLARE

DECLARE_STUB1(GRLFBCONSTANTDEPTH, void, int, a)
ENDDECLARE

DECLARE_STUB1(_GRCOLORCOMBINEDELTA0MODE, void, int, a)
ENDDECLARE

/* FIXME! */
DECLARE_STUB2(GRLFBUNLOCK, FxBool, GrLock_t, type, GrBuffer_t, buffer)
#if 0
    if((type & 1) == 1)
    {
       /*
        * If we are delaying LFB writes and some triangles
        * have been drawn this frame, then just mark that
        * there are writes yet to be sent.  If no triangles
        * have yet been drawn then the purpose of the writes
        * is probably to create a background so send them now.
        */
        if(delay_lfb_write && frame_started)
        {
            lfb_write_pending = 1;
        }

        else
        {
            send_blue_screen(buffer);
            buffer_is_locked = 0;
        }

    }

#endif
    return FXTRUE;
ENDDECLARE

DECLARE_STUB1(GRLFBWRITECOLORFORMAT, void, int, a)
ENDDECLARE

DECLARE_STUB2(GRLFBWRITECOLORSWIZZLE, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB8(GRLFBWRITEREGION, void, int, a, int, b, int, c, int, d,
                                int, e, int, f, int, g, int, h)
ENDDECLARE

DECLARE_STUB7(GRLFBREADREGION, void, int, a, int, b, int, c, int, d,
                               int, e, int, f, int, g)
ENDDECLARE

DECLARE_STUB1(GRSSTCONTROL, void, int, a)
ENDDECLARE

DECLARE_STUB1(GRSSTPERFSTATS, void, int, a)
ENDDECLARE

DECLARE_STUB0(GRSSTRESETPERFSTATS, void)
ENDDECLARE

/* FIXME */
DECLARE_STUB0(GRSSTSTATUS, int)
    return 0x0FFFF43F;
ENDDECLARE

DECLARE_STUB0(GRSSTVIDEOLINE, void)
ENDDECLARE

DECLARE_STUB0(GRSSTVRETRACEON, void)
ENDDECLARE

DECLARE_STUB0(GRSSTIDLE, void)
ENDDECLARE

DECLARE_STUB0(GRSSTISBUSY, void)
ENDDECLARE

DECLARE_STUB1(GRSSTORIGIN, void, int, a)
ENDDECLARE

DECLARE_STUB3(GRSSTCONFIGPIPELINE, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB2(_GRTEXDETAILCONTROL, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB2(GRTEXNCCTABLE, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB2(GRTEXMULTIBASE, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB5(GRTEXMULTIBASEADDRESS, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB5(_GRTEXDOWNLOADNCCTABLE, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB4(_GRTEXDOWNLOADPALETTE, void, int, a, int, b, int, c, int, d)
ENDDECLARE

DECLARE_STUB10(GRTEXDOWNLOADMIPMAPLEVELPARTIAL, void, int, a, int, b, int, c, int, d,
                                                int, e, int, f, int, g, int, h,
                                                int, i, int, j)
ENDDECLARE

DECLARE_STUB3(_GRCOMMANDTRANSPORTMAKEROOM, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB1(PIOINBYTE, void, int, a)
ENDDECLARE

DECLARE_STUB1(PIOINWORD, void, int, a)
ENDDECLARE

DECLARE_STUB1(PIOINLONG, void, int, a)
ENDDECLARE

DECLARE_STUB2(PIOOUTBYTE, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB2(PIOOUTWORD, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB2(PIOOUTLONG, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB0(PCIPRINTDEVICELIST, void)
ENDDECLARE

DECLARE_STUB4(_PCIFETCHREGISTER, void, int, a, int, b, int, c, int, d)
ENDDECLARE

DECLARE_STUB5(_PCIUPDATEREGISTER, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB0(PCIGETERRORSTRING, void)
ENDDECLARE

DECLARE_STUB0(PCIGETERRORCODE, void)
ENDDECLARE

DECLARE_STUB1(PCIOPENEX, void, int, a)
ENDDECLARE

DECLARE_STUB0(PCIOPEN, void)
ENDDECLARE

DECLARE_STUB0(PCICLOSE, void)
ENDDECLARE

DECLARE_STUB1(PCIDEVICEEXISTS, void, int, a)
ENDDECLARE

DECLARE_STUB5(PCIGETCONFIGDATA, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB5(PCIGETCONFIGDATARAW, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB5(PCISETCONFIGDATA, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB5(PCISETCONFIGDATARAW, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB4(PCIFINDCARDMULTI, void, int, a, int, b, int, c, int, d)
ENDDECLARE

DECLARE_STUB5(PCIFINDCARDMULTIFUNC, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB3(PCIFINDCARD, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB6(PCIMAPCARDMULTI, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB6(PCIMAPCARDMULTIFUNC, void, int, a, int, b, int, c, int, d, int, e, int, f)
ENDDECLARE

DECLARE_STUB5(PCIMAPCARD, void, int, a, int, b, int, c, int, d, int, e)
ENDDECLARE

DECLARE_STUB3(PCIMAPPHYSICALTOLINEAR, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB4(PCIMAPPHYSICALDEVICETOLINEAR, void, int, a, int, b, int, c, int, d)
ENDDECLARE

DECLARE_STUB2(PCIUNMAPPHYSICAL, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB2(PCISETPASSTHROUGHBASE, void, int, a, int, b)
ENDDECLARE

DECLARE_STUB1(PCIOUTPUTDEBUGSTRING, void, int, a)
ENDDECLARE

DECLARE_STUB3(PCILINEARRANGESETPERMISSION, void, int, a, int, b, int, c)
ENDDECLARE


int main ()
{
GRDEPTHMASK (0);
GRDEPTHMASK (0);
return 0;
}
