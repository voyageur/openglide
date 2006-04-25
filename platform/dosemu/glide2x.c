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
/* WRAPPERS */

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

DECLARE_STUB5(GRTEXDOWNLOADTABLEPARTIAL, void, int, a, int, b, int, c, int, d, int, e)
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

DECLARE_STUB3(GUTEXDOWNLOADMIPMAPLEVEL, void, int, a, int, b, int, c)
ENDDECLARE

DECLARE_STUB1(GUTEXGETCURRENTMIPMAP, void, int, a)
ENDDECLARE

DECLARE_STUB1(GUTEXGETMIPMAPINFO, void, int, a)
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

DECLARE_STUB1(GRDITHERMODE, void, int, a)
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
