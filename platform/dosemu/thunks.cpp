#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glide2x.h"

struct fptr_t
{
    unsigned short seg;
    unsigned long  off;
    fptr_t () : seg(0), off(0) {;}
};

static bool find_vxd (const char *driver, fptr_t &fptr);

static fptr_t vxd;
static void *cmds[CMD_MAX];

#define fcall(cmd, fptr) \
    asm ("mov %0, %%eax;" \
         "push %%cs;" \
         "push $1f;" \
         "push %1;" \
         "push %2;" \
         "lret;" \
         "1:" \
         : \
         : "r"((cmd)), "m"((fptr).seg), \
           "m"((fptr).off)              /* Inputs */ \
         : "%eax", "memory"             /* Clobbers */ \
        )

#define THUNK(cmd) THUNK_##cmd
#define THUNK_INIT(cmd) \
class Thunk##cmd \
{ \
private: \
    static const unsigned int m_cmd; \
    static unsigned int m_entry; \
public: \
    Thunk##cmd () \
    { \
        cmds[cmd] = (void *) &Thunk##cmd::entry; \
    } \
    static void entry () \
    { \
        if (!m_entry) \
        { \
            fcall (m_cmd, vxd); \
            asm ("mov %%eax, %0" :"=m"(m_entry) : :"memory"); \
        } \
        asm ("push %0;" \
             "push %1;" \
             "lret;" \
             : \
             : "m"(vxd.seg), "m"(m_entry) \
            ); \
    } \
}; \
const unsigned int Thunk##cmd::m_cmd = cmd; \
unsigned int Thunk##cmd::m_entry = 0; \
static Thunk##cmd THUNK_##cmd

THUNK_INIT(CMD_GrGlideInit);
THUNK_INIT(CMD_GrSstQueryHardware);
THUNK_INIT(CMD_GrSstSelect);
THUNK_INIT(CMD_GrSstWinOpen);
THUNK_INIT(CMD_GrDepthBufferMode);
THUNK_INIT(CMD_GrDepthBufferFunction);
THUNK_INIT(CMD_GrDepthMask);
THUNK_INIT(CMD_GrTexCombineFunction);
THUNK_INIT(CMD_GrConstantColorValue);
THUNK_INIT(CMD_GuAlphaSource);
THUNK_INIT(CMD_GrChromaKeyMode);
THUNK_INIT(CMD_GrChromaKeyValue);
THUNK_INIT(CMD_GrGammaCorrectionValue);
THUNK_INIT(CMD_GrTexDownloadTable);
THUNK_INIT(CMD_GuTexMemReset);
THUNK_INIT(CMD_Gu3dfGetInfo);
THUNK_INIT(CMD_Gu3dfLoad);
THUNK_INIT(CMD_GuTexAllocateMemory);
THUNK_INIT(CMD_GuTexDownloadMipMap);
THUNK_INIT(CMD_GrBufferClear);
THUNK_INIT(CMD_GuColorCombineFunction);
THUNK_INIT(CMD_GuTexSource);
THUNK_INIT(CMD_GrDrawPolygonVertexList);
THUNK_INIT(CMD_GrBufferSwap);
THUNK_INIT(CMD_GrTexFilterMode);
THUNK_INIT(CMD_GrSstWinClose);
THUNK_INIT(CMD_GrDepthBiasLevel);
THUNK_INIT(CMD_GrColorCombine);
THUNK_INIT(CMD_GrAlphaBlendFunction);
THUNK_INIT(CMD_GrDrawLine);
THUNK_INIT(CMD_GrConstantColorValue4);
THUNK_INIT(CMD_GrGlideShutdown);
THUNK_INIT(CMD_GrAADrawLine);
THUNK_INIT(CMD_GrRenderBuffer);
THUNK_INIT(CMD_GrTexDownloadMipMap);
THUNK_INIT(CMD_GrTexSource);
THUNK_INIT(CMD_GrDrawTriangle);
THUNK_INIT(CMD_GrDisableAllEffects);
THUNK_INIT(CMD_GrTexMipMapMode);
THUNK_INIT(CMD_GrTexLodBiasValue);
THUNK_INIT(CMD_GrTexClampMode);
THUNK_INIT(CMD_GrAlphaCombine);
THUNK_INIT(CMD_GrFogMode);
THUNK_INIT(CMD_GrAlphaTestFunction);
THUNK_INIT(CMD_GrClipWindow);
THUNK_INIT(CMD_GrCullMode);
THUNK_INIT(CMD_GrFogColorValue);
THUNK_INIT(CMD_GrFogTable);
THUNK_INIT(CMD_GrColorMask);
THUNK_INIT(CMD_GrLFBLock);
THUNK_INIT(CMD_GrTexDetailControl);
THUNK_INIT(CMD_GrHints);
THUNK_INIT(CMD_GuDrawTriangleWithClip);
THUNK_INIT(CMD_GuFogGenerateExp);
THUNK_INIT(CMD_GrTexCombine);

bool find_vxd (const char *driver, fptr_t &fptr)
{
    char   name[8];
    size_t len = strlen (driver);

    if (len > sizeof (name))
        len = sizeof (name);
    memset (name, ' ', sizeof(name));
    memcpy (name, driver, len);

    asm ("push %%es;"
         "mov %2,      %%edi;"
         "mov $0x1684, %%eax;"
         "mov $0,      %%ebx;"
         "int $0x2f;"
         "mov %%es,    %0;"
         "mov %%edi,   %1;"
         "pop %%es;"
         : "=m"(fptr.seg), "=m"(fptr.off)   /* Outputs */
         : "r"(name)                        /* Inputs */
         : "%eax", "%ebx", "%edi", "memory" /* Clobbers */
        );

    return fptr.seg || fptr.off;
}

void *CMD_entry (unsigned int cmd)
{
    if (vxd.seg || vxd.off)
    {
        if (cmd < CMD_MAX)
            return cmds[cmd];
        return 0;
    }

    {
        static const char driver[] = "GLDPIPE1";
        if (!find_vxd (driver, vxd))
        {
            fprintf (stderr, "GrGlideInit: Cannot find vxd \"%s\"\n", driver);
            exit (-1);
            return 0;
        }
    }
    return CMD_entry (cmd);
}
