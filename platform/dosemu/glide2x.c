#include <stdio.h>

#define VERSION 11
#define __far  asm("push %%cs;": : :"memory");
#define __stdcall __attribute__(stdcall)
#define __export
#define ENTRY(type, cmd, entry) \
    if (!entry) \
    { \
        entry = (type *) cmd_entry (type); \
        assert  (entry); \
    }
#define D_printf(fmt,arg...) printf(fmt,##arg)

extern void *CMD_entry (unsigned int cmd);


__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_4_8(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_4_8@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_4_WIDES(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_4_WIDES@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_8_1(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_8_1@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_8_2(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_8_2@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_8_4(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_8_4@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_8_WIDES(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_8_WIDES@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_16_1(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_16_1@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_16_2(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_16_2@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_16_WIDES(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_16_WIDES@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_32_1(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_32_1@24\n");
}

__export __stdcall void _GRTEXDOWNLOAD_DEFAULT_32_WIDES(int a, int b, int c, int d, int e, int f)
{
    D_printf("_GRTEXDOWNLOAD_DEFAULT_32_WIDES@24\n");
}

__export __stdcall void GRSPLASH(int a, int b, int c, int d, int e)
{
    D_printf("GRSPLASH@20\n");
}

typedef FxBool (GU3DFGETINFO)(char *filename, Gu3dfInfo *file_info);
__export __stdcall GU3DFGETINFO
{
/*    static GU3DFGETINFO *entry = 0; */
    D_printf("GU3DFGETINFO@8\n");
    /*
     * Pretend file doesn't exist to get TRI to
     * generate textures on the fly.
     * FIXME: wont work on most games, probably
     */
    return FXFALSE;
/*
    ENTRY (GU3DFGETINFO, CMD_Gu3dfGetInfo, entry);
    return __far (*entry) (filename, file_info);
*/
}

typedef FxBool (GU3DFLOAD)(char *filename, Gu3dfInfo *file_info);
__export __stdcall GU3DFLOAD
{
/*    static GU3DFLOAD *entry = 0; */
    D_printf("GU3DFLOAD@8\n");
    return FXFALSE;
/*
    ENTRY (GU3DFLOAD, CMD_Gu3dfLoad, entry);
    return __far (*entry) (filename, file_info);
*/
}

typedef void (GUALPHASOURCE)(int a);
__export __stdcall GUALPHASOURCE
{
    static GUALPHASOURCE *entry = 0;
    D_printf("GUALPHASOURCE@4\n");
    ENTRY (GUALPHASOURCE, GUALPHASOURCE, entry);
    __far (*entry) (a);
}

typedef void (GUCOLORCOMBINEFUNCTION)(int a);
__export __stdcall GUCOLORCOMBINEFUNCTION
{
    static GUCOLORCOMBINEFUNCTION *entry = 0;
    D_printf("GUCOLORCOMBINEFUNCTION@4\n");
    ENTRY (GUCOLORCOMBINEFUNCTION,
           CMD_GuColorCombineFunction, entry);
    __far (*entry) (a);
}

__export __stdcall void GUENDIANSWAPWORDS(int a)
{
    D_printf("GUENDIANSWAPWORDS@4\n");
}

__export __stdcall void GUENDIANSWAPBYTES(int a)
{
    D_printf("GUENDIANSWAPBYTES@4\n");
}

__export __stdcall void GUFOGTABLEINDEXTOW(int a)
{
    D_printf("GUFOGTABLEINDEXTOW@4\n");
}

typedef void (GUFOGGENERATEEXP)(GrFog_t *table, int density);
__export __stdcall GUFOGGENERATEEXP
{
    static GUFOGGENERATEEXP *entry = 0;
    D_printf("GUFOGGENERATEEXP@8\n");
    ENTRY (GUFOGGENERATEEXP,
           CMD_GuFogGenerateExp, entry);
    __far (*entry) (table, density);
}

__export __stdcall void GUFOGGENERATEEXP2(int a, int b)
{
    D_printf("GUFOGGENERATEEXP2@8\n");
}

__export __stdcall void GUFOGGENERATELINEAR(int a, int b, int c)
{
    D_printf("GUFOGGENERATELINEAR@12\n");
}

__export __stdcall void GUTEXCREATECOLORMIPMAP(void)
{
    D_printf("GUTEXCREATECOLORMIPMAP@0\n");
}

__export __stdcall void GUENCODERLE16(int a, int b, int c, int d)
{
    D_printf("GUENCODERLE16@16\n");
}

typedef void (GUDRAWTRIANGLEWITHCLIP)(GrVertex *v1, GrVertex *v2, GrVertex *v3);
__export __stdcall GUDRAWTRIANGLEWITHCLIP
{
    static GUDRAWTRIANGLEWITHCLIP *entry = 0;
    D_printf("GUDRAWTRIANGLEWITHCLIP@12\n");
    ENTRY (GUDRAWTRIANGLEWITHCLIP,
           CMD_GuDrawTriangleWithClip, entry);
    __far (*entry) (v1, v2, v3);
}

__export __stdcall void GUAADRAWTRIANGLEWITHCLIP(int a, int b, int c)
{
    D_printf("GUAADRAWTRIANGLEWITHCLIP@12\n");
}

__export __stdcall void GUDRAWPOLYGONVERTEXLISTWITHCLIP(int a, int b)
{
    D_printf("GUDRAWPOLYGONVERTEXLISTWITHCLIP@8\n");
}

__export __stdcall void GUMPINIT(void)
{
    D_printf("GUMPINIT@0\n");
}

__export __stdcall void GUMPTEXCOMBINEFUNCTION(int a)
{
    D_printf("GUMPTEXCOMBINEFUNCTION@4\n");
}

__export __stdcall void GUMPTEXSOURCE(int a, int b)
{
    D_printf("GUMPTEXSOURCE@8\n");
}

__export __stdcall void GUFBREADREGION(int a, int b, int c, int d, int e, int f)
{
    D_printf("GUFBREADREGION@24\n");
}

__export __stdcall void GUFBWRITEREGION(int a, int b, int c, int d, int e, int f)
{
    D_printf("GUFBWRITEREGION@24\n");
}

__export __stdcall void GRGLIDEGETVERSION(char version[80])
{
    D_printf("GRGLIDEGETVERSION@4\n");
    memcpy(version, "2.48.00.0455", 13);
}

__export __stdcall void GRGLIDEGETSTATE(int a)
{
    D_printf("GRGLIDEGETSTATE@4\n");
}

typedef void (GRHINTS)(int a, int b);
__export __stdcall GRHINTS
{
    static GRHINTS *entry = 0;
    D_printf("GRHINTS@8\n");
    ENTRY (GRHINTS,
           CMD_GrHints, entry);
    __far (*entry) (a, b);
}

typedef void (_GRGLIDEINIT)(int);
__export __stdcall void GRGLIDEINIT(void)
{
    static _GRGLIDEINIT *entry = 0;
    D_printf("GRGLIDEINIT@0\n");
    ENTRY (_GRGLIDEINIT,
           CMD_GrGlideInit, entry);
    __far (*entry) (VERSION);
}

__export __stdcall void GRGLIDESHAMELESSPLUG(int a)
{
    D_printf("GRGLIDESHAMELESSPLUG@4\n");
}

__export __stdcall void GRRESETTRISTATS(void)
{
    D_printf("GRRESETTRISTATS@0\n");
}

__export __stdcall void GRTRISTATS(int a, int b)
{
    D_printf("GRTRISTATS@8\n");
}

__export __stdcall void GRSSTQUERYBOARDS(int a)
{
    D_printf("GRSSTQUERYBOARDS@4\n");
}

typedef FxBool (GRSSTQUERYHARDWARE)(GrHwConfiguration *hwConfig);
__export __stdcall GRSSTQUERYHARDWARE
{
    static GRSSTQUERYHARDWARE *entry = 0;
    D_printf("GRSSTQUERYHARDWARE@4\n");
    ENTRY (GRSSTQUERYHARDWARE,
           CMD_GrSstQueryHardware, entry);
    __far return (*entry) (VERSION);
}

typedef void (GRSSTSELECT)(int which_sst);
__export __stdcall GRSSTSELECT
{
    static GRSSTSELECT *entry = 0;
    D_printf("GRSSTSELECT@4\n");
    ENTRY (GRSSTSELECT,
           CMD_GrSstSelect, entry);
    __far (*entry) (which_sst);
}

__export __stdcall void GRSSTSCREENWIDTH(void)
{
    D_printf("GRSSTSCREENWIDTH@0\n");
}

__export __stdcall void GRSSTSCREENHEIGHT(void)
{
    D_printf("GRSSTSCREENHEIGHT@0\n");
}

__export __stdcall void GRSSTVIDMODE(int a, int b)
{
    D_printf("GRSSTVIDMODE@8\n");
}

FxU32 __export __stdcall GRTEXCALCMEMREQUIRED(GrLOD_t lodmin, GrLOD_t lodmax, GrAspectRatio_t aspect, GrTextureFormat_t fmt)
{
    int size, i;
    D_printf("GRTEXCALCMEMREQUIRED@16\n");

    size = 0;

    for(i = lodmax; i <= lodmin; i++)
        size += texture_size(i, aspect, fmt);

    D_printf("size = %x\n", size);

    return size;
}

__export __stdcall void GRTEXDETAILCONTROL(int a, int b, int c, int d)
{
    D_printf("GRTEXDETAILCONTROL@16\n");
    send_char(CMD_GrTexDetailControl);
}

/* FIXME? */
FxU32 __export __stdcall GRTEXMINADDRESS(int a)
{
    D_printf("GRTEXMINADDRESS@4\n");

    return 8;
}

/* FIXME? */
FxU32 __export __stdcall GRTEXMAXADDRESS(int a)
{
    D_printf("GRTEXMAXADDRESS@4\n");

    return 8*1024*1024 - 128*1024;
}

/* FIXME */
int __export __stdcall GRTEXTEXTUREMEMREQUIRED(FxU32 evenOdd,
                                       GrTexInfo *info)
{
    int size, i;
    D_printf("GRTEXTEXTUREMEMREQUIRED@8\n");
    return 0;
}

typedef void (GRTEXDOWNLOADMIPMAP)(GrChipID_t tmu, FxU32 startAddress,
                                   FxU32 evenOdd, GrTexInfo *info);
__export __stdcall GRTEXDOWNLOADMIPMAP
{
    static GRTEXDOWNLOADMIPMAP *entry = 0;
    D_printf("GRTEXDOWNLOADMIPMAP@16\n");
    ENTRY (GRTEXDOWNLOADMIPMAP,
           CMD_GrTexDownloadMipMap, entry);
    __far (*entry) (tmu, startAddress, evenOdd, info);
}

__export __stdcall void GRTEXDOWNLOADTABLEPARTIAL(int a, int b, int c, int d, int e)
{
    D_printf("GRTEXDOWNLOADTABLEPARTIAL@20\n");
}

/* FIXME? */
__export __stdcall void GRTEXDOWNLOADMIPMAPLEVEL(
                          GrChipID_t        tmu,
                          FxU32             startAddress,
                          GrLOD_t           thisLod,
                          GrLOD_t           largeLod,
                          GrAspectRatio_t   aspectRatio,
                          GrTextureFormat_t format,
                          FxU32             evenOdd,
                          void              *data)
{
    GrTexInfo info;

    D_printf("GRTEXDOWNLOADMIPMAPLEVEL@32");

    if(thisLod == largeLod)
    {
        info.aspectRatio = aspectRatio;
        info.data        = data;
        info.format      = format;
        info.largeLod    = largeLod;
        info.smallLod    = largeLod;

        GRTEXDOWNLOADMIPMAP(tmu, startAddress, evenOdd, &info);
    }
}

__export __stdcall void GRERRORSETCALLBACK(int a)
{
    D_printf("GRERRORSETCALLBACK@4\n");
}

__export __stdcall void GUMOVIESTART(void)
{
    D_printf("GUMOVIESTART@0\n");
}

__export __stdcall void GUMOVIESTOP(void)
{
    D_printf("GUMOVIESTOP@0\n");
}

__export __stdcall void GUMOVIESETNAME(int a)
{
    D_printf("GUMOVIESETNAME@4\n");
}

typedef int (GUTEXALLOCATEMEMORY)(GrChipID_t tmu, FxU8 odd_even_mask,
                                  int width, int height, GrTextureFormat_t fmt,
                                  GrMipMapMode_t mm_mode, GrLOD_t smallest_lod,
                                  GrLOD_t largest_lod, GrAspectRatio_t aspect,
                                  GrTextureClampMode_t s_clamp_mode,
                                  GrTextureClampMode_t t_clamp_mode,
                                  GrTextureFilterMode_t minfilter_mode,
                                  GrTextureFilterMode_t magfilter_mode,
                                  int lod_bias, FxBool trilinear);
__export __stdcall GUTEXALLOCATEMEMORY
{
    static GUTEXALLOCATEMEMORY *entry = 0;
    D_printf("GUTEXALLOCATEMEMORY@60\n");
    ENTRY (GUTEXALLOCATEMEMORY,
           CMD_GuTexAllocateMemory, entry);
    __far return (*entry) (tmu, odd_even_mask, width, height, fmt,
                           mm_mode, smallest_lod, largest_lod, aspect,
                           s_clamp_mode, t_clamp_mode, minfilter_mode,
                           magfilter_mode, lod_bias, trilinear);
}

__export __stdcall void GUTEXCHANGEATTRIBUTES(int a, int b, int c, int d,
                           int e, int f, int g, int h,
                           int i, int j, int k, int l)
{
    D_printf("GUTEXCHANGEATTRIBUTES@48\n");
}

__export __stdcall void GRTEXCOMBINEFUNCTION(int a, int b)
{
    static GRTEXCOMBINEFUNCTION *entry = 0;
    D_printf("GRTEXCOMBINEFUNCTION@8\n");
    ENTRY (GRTEXCOMBINEFUNCTION,
           CMD_GrTexCombineFunction, entry);
    __far (*entry) (a, b);
}

typedef void (GUTEXCOMBINEFUNCTION)(int a, int b);
__export __stdcall GUTEXCOMBINEFUNCTION
{
    static GUTEXCOMBINEFUNCTION *entry = 0;
    D_printf("GUTEXCOMBINEFUNCTION@8\n");
    ENTRY (GUTEXCOMBINEFUNCTION,
           CMD_GrTexCombineFunction, entry);
    __far (*entry) (a, b);
}

typedef void (GUTEXDOWNLOADMIPMAP)(GrMipMapId_t mmid, void *src, GuNccTable *table);
__export __stdcall GUTEXDOWNLOADMIPMAP
{
    static GUTEXDOWNLOADMIPMAP *entry = 0;
    D_printf("GUTEXDOWNLOADMIPMAP@12\n");
    ENTRY (GUTEXDOWNLOADMIPMAP,
           CMD_GuTexDownloadMipMap, entry);
    __far (*entry) (mmid, src, table);
}

__export __stdcall void GUTEXDOWNLOADMIPMAPLEVEL(int a, int b, int c)
{
    D_printf("GUTEXDOWNLOADMIPMAPLEVEL@12\n");
}

__export __stdcall void GUTEXGETCURRENTMIPMAP(int a)
{
    D_printf("GUTEXGETCURRENTMIPMAP@4\n");
}

__export __stdcall void GUTEXGETMIPMAPINFO(int a)
{
    D_printf("GUTEXGETMIPMAPINFO@4\n");
}

/* FIXME */
__export __stdcall FxU32 GUTEXMEMQUERYAVAIL(int a)
{
    D_printf("GUTEXMEMQUERYAVAIL@4\n");
    return 16 * 1024 * 1024;
}

typedef void (GUTEXMEMRESET)(void);
__export __stdcall GUTEXMEMRESET
{
    static GUTEXMEMRESET *entry = 0;
    D_printf("GUTEXMEMRESET@0\n");
    ENTRY (GUTEXMEMRESET,
           CMD_GuTexMemReset, entry);
    __far (*entry) ();
}

__export __stdcall void _GUMPTEXCOMBINEFUNCTION(int a)
{
    D_printf("_GUMPTEXCOMBINEFUNCTION@4\n");
}

__export __stdcall void GUMPDRAWTRIANGLE(int a, int b, int c)
{
    D_printf("GUMPDRAWTRIANGLE@12\n");
}

typedef void GRAADRAWPOINT(int a);
__export __stdcall GRAADRAWPOINT
{
    D_printf("GRAADRAWPOINT@4\n");
}

typedef void GRAADRAWLINE(GrVertex *v1, GrVertex *v2);
__export __stdcall GRAADRAWLINE
{
    static GRAADRAWLINE *entry = 0;
    D_printf("GRAADRAWLINE@8\n");
    ENTRY (GRAADRAWLINE,
           CMD_GrAADrawLine, entry);
    __far (*entry) (v1, v2);
}

__export __stdcall void GRAADRAWTRIANGLE(int a, int b, int c, int d, int e, int f)
{
    D_printf("GRAADRAWTRIANGLE@24\n");
}

__export __stdcall void GRAADRAWPOLYGON(int a, int b, int c)
{
    D_printf("GRAADRAWPOLYGON@12\n");
}

__export __stdcall void GRAADRAWPOLYGONVERTEXLIST(int nverts, GrVertex vlist[])
{
    D_printf("GRAADRAWPOLYGONVERTEXLIST@8\n");
}

__export __stdcall void GRDRAWPOINT(int a)
{
    D_printf("GRDRAWPOINT@4\n");
}

typedef void GRDRAWLINE(GrVertex *v1, GrVertex *v2);
__export __stdcall GRDRAWLINE
{
    static GRDRAWLINE *entry = 0;
    D_printf("GRDRAWLINE@8\n");
    ENTRY (GRAADRAWLINE,
           CMD_GrDrawLine, entry);
    __far (*entry) (v1, v2);
}

typedef void GRDRAWTRIANGLE(GrVertex *v1, GrVertex *v2, GrVertex *v3);
__export __stdcall GRDRAWTRIANGLE
{
    static GRDRAWTRIANGLE *entry = 0;
    D_printf("GRDRAWTRIANGLE@12\n");
    ENTRY (GRDRAWTRIANGLE,
           CMD_GrDrawTriangle, entry);
    __far (*entry) (v1, v2, v2);
}

__export __stdcall void GRDRAWPLANARPOLYGON(int a, int b, int c)
{
    D_printf("GRDRAWPLANARPOLYGON@12\n");
}

__export __stdcall void GRDRAWPLANARPOLYGONVERTEXLIST(int a, int b)
{
    D_printf("GRDRAWPLANARPOLYGONVERTEXLIST@8\n");
}

__export __stdcall void GRDRAWPOLYGON(int a, int b, int c)
{
    D_printf("GRDRAWPOLYGON@12\n");
}

typedef void GRDRAWPOLYGONVERTEXLIST(int nverts, GrVertex vlist[]);
__export __stdcall GRDRAWPOLYGONVERTEXLIST
{
    static GRDRAWTRIANGLE *entry = 0;
    D_printf("GRDRAWPOLYGONVERTEXLIST@8\n");
    ENTRY (GRDRAWPOLYGONVERTEXLIST,
           CMD_GrDrawPolygonVertexList, entry);
    __far (*entry) (nverts, vlist);
}

__export __stdcall void _GRCOLORCOMBINEDELTA0MODE(int a)
{
    D_printf("_GRCOLORCOMBINEDELTA0MODE@4\n");
}

typedef void GRALPHABLENDFUNCTION(int a, int b, int c, int d);
__export __stdcall GRALPHABLENDFUNCTION
{
    static GRALPHABLENDFUNCTION *entry = 0;
    D_printf("GRALPHABLENDFUNCTION@16\n");
    ENTRY (GRALPHABLENDFUNCTION,
           CMD_GrAlphaBlendFunction, entry);
    __far (*entry) (a, b, c, d);
}

typedef void GRALPHACOMBINE(int a, int b, int c, int d, int e);
__export __stdcall GRALPHACOMBINE
{
    static GRALPHACOMBINE *entry = 0;
    D_printf("GRALPHACOMBINE@20\n");
    ENTRY (GRALPHACOMBINE,
           CMD_GrAlphaCombine, entry);
    __far (*entry) (a, b, c, d, e);
}

__export __stdcall void GRALPHACONTROLSITRGBLIGHTING(int a)
{
    D_printf("GRALPHACONTROLSITRGBLIGHTING@4\n");
}

typedef void GRALPHATESTFUNCTION(int a);
__export __stdcall GRALPHATESTFUNCTION
{
    static GRALPHATESTFUNCTION *entry = 0;
    D_printf("GRALPHATESTFUNCTION@4\n");
    ENTRY (GRALPHATESTFUNCTION,
           CMD_GrAlphaTestFunction, entry);
    __far (*entry) (a);
}

__export __stdcall void GRALPHATESTREFERENCEVALUE(int a)
{
    D_printf("GRALPHATESTREFERENCEVALUE@4\n");
}

__export __stdcall void GRBUFFERCLEAR(int a, int b, int c)
{
    D_printf("GRBUFFERCLEAR@12\n");
    send_char(CMD_GrBufferClear);
    send_int(a);
    send_int(b);
    send_int(c);
}

__export __stdcall void GRBUFFERSWAP(int a)
{
    D_printf("GRBUFFERSWAP@4\n");
    
    send_cd_cmds();

    flush_lfb_writes();
    frame_started = 0;

    if(running_nt && judder_fix == 1)
        timesync_init();

    if(buffer_is_locked)
    {
        send_blue_screen(locked_buffer);
        setup_blue_screen();
    }

    send_char(CMD_GrBufferSwap);
    send_int(a);

    synchronise_with_server();
}

int __export __stdcall GRBUFFERNUMPENDING(void)
{
    D_printf("GRBUFFERNUMPENDING@0\n");

    synchronise_with_server();

    return 0;
}

__export __stdcall void GRCHROMAKEYMODE(int a)
{
    D_printf("GRCHROMAKEYMODE@4\n");
    send_char(CMD_GrChromaKeyMode);
    send_int(a);
}

__export __stdcall void GRCHROMAKEYVALUE(int a)
{
    D_printf("GRCHROMAKEYVALUE@4\n");
    send_char(CMD_GrChromaKeyValue);
    send_int(a);
}

__export __stdcall void GRCLIPWINDOW(int a, int b, int c, int d)
{
    D_printf("GRCLIPWINDOW@16\n");
    send_char(CMD_GrClipWindow);
    send_int(a);
    send_int(b);
    send_int(c);
    send_int(d);
}

__export __stdcall void GRCOLORCOMBINE(int a, int b, int c, int d, int e)
{
    D_printf("GRCOLORCOMBINE@20\n");
    send_char(CMD_GrColorCombine);
    send_int(a);
    send_int(b);
    send_int(c);
    send_int(d);
    send_int(e);
}

__export __stdcall void GRCOLORMASK(int a, int b)
{
    D_printf("GRCOLORMASK@8\n");
    send_char(CMD_GrColorMask);
    send_int(a);
    send_int(b);
}

__export __stdcall void GRCONSTANTCOLORVALUE(int a)
{
    D_printf("GRCONSTANTCOLORVALUE@4\n");
    send_char(CMD_GrConstantColorValue);
    send_int(a);
}

__export __stdcall void GRCONSTANTCOLORVALUE4(int a, int b, int c, int d)
{
    D_printf("GRCONSTANTCOLORVALUE4@16\n");
    send_char(CMD_GrConstantColorValue4);
    send_int(a);
    send_int(b);
    send_int(c);
    send_int(d);
}

__export __stdcall void GRCULLMODE(int a)
{
    D_printf("GRCULLMODE@4\n");
    send_char(CMD_GrCullMode);
    send_int(a);
}

__export __stdcall void GRDEPTHBIASLEVEL(int a)
{
    D_printf("GRDEPTHBIASLEVEL@4\n");
    send_char(CMD_GrDepthBiasLevel);
    send_int(a);
}

__export __stdcall void GRDEPTHBUFFERFUNCTION(int a)
{
    D_printf("GRDEPTHBUFFERFUNCTION@4\n");
    send_char(CMD_GrDepthBufferFunction);
    send_int(a);
}

__export __stdcall void GRDEPTHBUFFERMODE(int a)
{
    D_printf("GRDEPTHBUFFERMODE@4\n");
    send_char(CMD_GrDepthBufferMode);
    send_int(a);
}

__export __stdcall void GRDEPTHMASK(int a)
{
    D_printf("GRDEPTHMASK@4\n");
    send_char(CMD_GrDepthMask);
    send_int(a);
}

__export __stdcall void GRDISABLEALLEFFECTS(void)
{
    D_printf("GRDISABLEALLEFFECTS@0\n");
    send_char(CMD_GrDisableAllEffects);
}

__export __stdcall void GRDITHERMODE(int a)
{
    D_printf("GRDITHERMODE@4\n");
}

__export __stdcall void GRFOGMODE(int a)
{
    D_printf("GRFOGMODE@4\n");
    send_char(CMD_GrFogMode);
    send_int(a);
}

__export __stdcall void GRFOGCOLORVALUE(int a)
{
    D_printf("GRFOGCOLORVALUE@4\n");
    send_char(CMD_GrFogColorValue);
    send_int(a);
}

__export __stdcall void GRFOGTABLE(GrFog_t *ft)
{
    D_printf("GRFOGTABLE@4\n");
    send_char(CMD_GrFogTable);
    send_buffer((char *)ft, GR_FOG_TABLE_SIZE * sizeof(GrFog_t));
}

__export __stdcall void GRGLIDESHUTDOWN(void)
{
    D_printf("GRGLIDESHUTDOWN@0\n");
    send_cd_cmds();
    get_off_33();
    if(running_nt)
        get_off_31();
    send_char(CMD_GrGlideShutdown);
    send_context_switch();
}

__export __stdcall void GRGLIDESETSTATE(int a)
{
    D_printf("GRGLIDESETSTATE@4\n");
}

__export __stdcall void GRRENDERBUFFER(int a)
{
    D_printf("GRRENDERBUFFER@4\n");
    send_char(CMD_GrRenderBuffer);
    send_int(a);
    render_buffer = a;
}

__export __stdcall void GRCHECKFORROOM(int a)
{
    D_printf("GRCHECKFORROOM@4");
}

__export __stdcall void _GRUPDATEPARAMINDEX(void)
{
    D_printf("_GRUPDATEPARAMINDEX@0\n");
}

__export __stdcall void _GRREBUILDDATALIST(void)
{
    D_printf("_GRREBUILDDATALIST@0\n");
}

__export __stdcall void GRLFBCONSTANTALPHA(int a)
{
    D_printf("GRLFBCONSTANTALPHA@4\n");
}

__export __stdcall void GRLFBCONSTANTDEPTH(int a)
{
    D_printf("GRLFBCONSTANTDEPTH@4\n");
}

FxBool __export __stdcall GRLFBLOCK(GrLock_t type, GrBuffer_t buffer, GrLfbWriteMode_t writeMode, GrOriginLocation_t origin, FxBool pixelPipeline, GrLfbInfo_t *info)
{
    D_printf("GRLFBLOCK@24 type = %d, mode = %d, buffer = %d\n", type, writeMode, buffer);
    info->strideInBytes = 2 * 1024;
    info->writeMode = GR_LFBWRITEMODE_565;
    info->origin = GR_ORIGIN_UPPER_LEFT;

    if((type & 1) == 1)
    {
        info->lfbPtr = lfb_write;

        if(!lfb_write_pending)
        {
            setup_blue_screen();
            buffer_is_locked = 1;
            locked_buffer = buffer;
        }

        lfb_write_pending = 0;
    }
    else
    {
        char c;
        int x, y;

        info->lfbPtr = lfb_read;

        if(!disable_lfb_read)
        {
            if(buffer == GR_BUFFER_AUXBUFFER)
            {
                for(y = 0; y < 480; y++)
                    for(x = 0; x < 640; x++)
                        lfb_read[y][x] = 0xB000;
            }
            else
            {
                send_char(CMD_GrLFBLock);
                send_int(buffer);
                send_context_switch();

                for(y = 0; y < 480; y++)
                    pipe->get((char *)lfb_read[y], 640*2);
            }
        }
    }

    return FXTRUE;
}

FxBool __export __stdcall GRLFBUNLOCK(GrLock_t type, GrBuffer_t buffer)
{
    D_printf("GRLFBUNLOCK@8 type = %d, buffer = %d\n", type, buffer);

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

    return FXTRUE;
}

__export __stdcall void GRLFBWRITECOLORFORMAT(int a)
{
    D_printf("GRLFBWRITECOLORFORMAT@4\n");
}

__export __stdcall void GRLFBWRITECOLORSWIZZLE(int a, int b)
{
    D_printf("GRLFBWRITECOLORSWIZZLE@8\n");
}

__export __stdcall void GRLFBWRITEREGION(int a, int b, int c, int d,
                      int e, int f, int g, int h)
{
    D_printf("GRLFBWRITEREGION@32\n");
}

__export __stdcall void GRLFBREADREGION(int a, int b, int c, int d,
                     int e, int f, int g)
{
    D_printf("GRLFBREADREGION@28\n");
}

typedef FxBool GRSSTWINOPEN(int a, int b, int c, int d,
                            int e, int f, int g);
__export __stdcall GRSSTWINOPEN
{
    D_printf("GRSSTWINOPEN@28\n");
    ENTRY (GRSSTWINOPEN,
           CMD_GrSstWinOpen, entry);
    __far return (*entry) (a, b, c, d, e, f, g);
}

typedef void GRSSTWINCLOSE(void);
__export __stdcall GRSSTWINCLOSE
{
   /*
    * We request a confirmation and wait for it,
    * to give Glidos the chance to put us (remember
    * we are running in a DOS box) in fullscreen
    * mode.
    */
    static GRSSTWINCLOSE *entry = 0;
    D_printf("GRSSTWINCLOSE@0\n");
    ENTRY (GRSSTWINCLOSE,
           CMD_GrSstWinClose, entry);
    __far (*entry) ();
}

__export __stdcall void GRSSTCONTROL(int a)
{
    D_printf("GRSSTCONTROL@4\n");
}

__export __stdcall void GRSSTPERFSTATS(int a)
{
    D_printf("GRSSTPERFSTATS@4\n");
}

__export __stdcall void GRSSTRESETPERFSTATS(void)
{
    D_printf("GRSSTRESETPERFSTATS@0\n");
}

/* FIXME */
__export __stdcall int GRSSTSTATUS(void)
{
    D_printf("GRSSTSTATUS@0\n");

    return 0x0FFFF43F;
}

__export __stdcall void GRSSTVIDEOLINE(void)
{
    D_printf("GRSSTVIDEOLINE@0\n");
}

__export __stdcall void GRSSTVRETRACEON(void)
{
    D_printf("GRSSTVRETRACEON@0\n");
}

__export __stdcall void GRSSTIDLE(void)
{
    D_printf("GRSSTIDLE@0\n");
}

__export __stdcall void GRSSTISBUSY(void)
{
    D_printf("GRSSTISBUSY@0\n");
}

__export __stdcall void GRGAMMACORRECTIONVALUE(int a)
{
    static GRGAMMACORRECTIONVALUE *entry = 0;
    D_printf("GRGAMMACORRECTIONVALUE@4\n");
    ENTRY (GRGAMMACORRECTIONVALUE,
           CMD_GrGammaCorrectionValue, entry);
    __far (*entry) (a);
}

__export __stdcall void GRSSTORIGIN(int a)
{
    D_printf("GRSSTORIGIN@4\n");
}

__export __stdcall void GRSSTCONFIGPIPELINE(int a, int b, int c)
{
    D_printf("GRSSTCONFIGPIPELINE@12\n");
}

typedef void GRTEXCLAMPMODE(int a, int b, int c);
__export __stdcall GRTEXCLAMPMODE
{
    static GRTEXCLAMPMODE *entry = 0;
    D_printf("GRTEXCLAMPMODE@12\n");
    ENTRY (GRTEXCLAMPMODE,
           CMD_GrTexClampMode, entry);
    __far (*entry) (a, b, c);
}

typedef void GRTEXCOMBINE(int a, int b, int c, int d,
                          int e, int f, int g);
__export __stdcall GRTEXCOMBINE
{
    static GRTEXCOMBINE *entry = 0;
    D_printf("GRTEXCOMBINE@28\n");
    ENTRY (GRTEXCOMBINE,
           CMD_GrTexCombine, entry);
    __far (*entry) (a, b, c, d, e, f, g);
}

__export __stdcall void _GRTEXDETAILCONTROL(int a, int b)
{
    D_printf("_GRTEXDETAILCONTROL@8\n");
}

typedef void GRTEXFILTERMODE(int a, int b, int c);
__export __stdcall GRTEXFILTERMODE
{
    static GRTEXFILTERMODE *entry = 0;
    D_printf("GRTEXFILTERMODE@12\n");
    ENTRY (GRTEXFILTERMODE,
           CMD_GrTexFilterMode, entry);
    __far (*entry) (a, b, c);
}

typedef void GRTEXLODBIASVALUE(int a, int b);
__export __stdcall GRTEXLODBIASVALUE
{
    static GRTEXLODBIASVALUE *entry = 0;
    D_printf("GRTEXLODBIASVALUE@8\n");
    ENTRY (GRTEXLODBIASVALUE,
           CMD_GrTexLodBiasValue, entry);
    __far (*entry) (a, b);
}

typedef void GRTEXMIPMAPMODE(GrChipID_t tmu, GrMipMapMode_t mode, FxBool lodBlend);
__export __stdcall GRTEXMIPMAPMODE
{
    static GRTEXMIPMAPMODE *entry = 0;
    D_printf("GRTEXMIPMAPMODE@12\n");
    ENTRY (GRTEXMIPMAPMODE,
           CMD_GrTexMipMapMode, entry);
    __far (*entry) (tmu, mode, lodBlend);
}

__export __stdcall void GRTEXNCCTABLE(int a, int b)
{
    D_printf("GRTEXNCCTABLE@8\n");
}

typedef void GRTEXSOURCE(GrChipID_t tmu, FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info);
__export __stdcall GRTEXSOURCE
{
    static GRTEXSOURCE *entry = 0;
    D_printf("GRTEXSOURCE@16\n");
    ENTRY (GRTEXSOURCE,
           CMD_GrTexSource, entry);
    __far (*entry) (tmu, startAddress, evenOdd, info);
}

__export __stdcall void GRTEXMULTIBASE(int a, int b)
{
    D_printf("GRTEXMULTIBASE@8\n");
}

__export __stdcall void GRTEXMULTIBASEADDRESS(int a, int b, int c, int d, int e)
{
    D_printf("GRTEXMULTIBASEADDRESS@20\n");
}

__export __stdcall void _GRTEXDOWNLOADNCCTABLE(int a, int b, int c, int d, int e)
{
    D_printf("_GRTEXDOWNLOADNCCTABLE@20\n");
}

__export __stdcall void _GRTEXDOWNLOADPALETTE(int a, int b, int c, int d)
{
    D_printf("_GRTEXDOWNLOADPALETTE@16\n");
}

typedef void GRTEXDOWNLOADTABLE(GrChipID_t tmu, GrTexTable_t type, void *data);
__export __stdcall GRTEXDOWNLOADTABLE
{
    static GRTEXDOWNLOADTABLE *entry = 0;
    D_printf("GRTEXDOWNLOADTABLE@12\n");
    ENTRY (GRTEXDOWNLOADTABLE,
           CMD_GrTexDownloadTable, entry);
    __far (*entry) (tmu, type, data);
}

__export __stdcall void GRTEXDOWNLOADMIPMAPLEVELPARTIAL(int a, int b, int c, int d,
                                     int e, int f, int g, int h,
                                     int i, int j)
{
    D_printf("GRTEXDOWNLOADMIPMAPLEVELPARTIAL@40\n");
}

typedef void (GUTEXSOURCE)(int a);
__export __stdcall GUTEXSOURCE
{
    static GUTEXSOURCE *entry = 0;
    D_printf("GUTEXSOURCE@4\n");
    ENTRY (GUTEXSOURCE,
           CMD_GrTexSource, entry);
    __far (*entry) (a);
}

__export __stdcall void _GRCOMMANDTRANSPORTMAKEROOM(int a, int b, int c)
{
    D_printf("_GRCOMMANDTRANSPORTMAKEROOM@12\n");
}

__export __stdcall void PIOINBYTE(int a)
{
    D_printf("PIOINBYTE@4\n");
}

__export __stdcall void PIOINWORD(int a)
{
    D_printf("PIOINWORD@4\n");
}

__export __stdcall void PIOINLONG(int a)
{
    D_printf("PIOINLONG@4\n");
}

__export __stdcall void PIOOUTBYTE(int a, int b)
{
    D_printf("PIOOUTBYTE@8\n");
}

__export __stdcall void PIOOUTWORD(int a, int b)
{
    D_printf("PIOOUTWORD@8\n");
}

__export __stdcall void PIOOUTLONG(int a, int b)
{
    D_printf("PIOOUTLONG@8\n");
}

__export __stdcall void PCIPRINTDEVICELIST(void)
{
    D_printf("PCIPRINTDEVICELIST@0\n");
}

__export __stdcall void _PCIFETCHREGISTER(int a, int b, int c, int d)
{
    D_printf("_PCIFETCHREGISTER@16\n");
}

__export __stdcall void _PCIUPDATEREGISTER(int a, int b, int c, int d, int e)
{
    D_printf("_PCIUPDATEREGISTER@20\n");
}

__export __stdcall void PCIGETERRORSTRING(void)
{
    D_printf("PCIGETERRORSTRING@0\n");
}

__export __stdcall void PCIGETERRORCODE(void)
{
    D_printf("PCIGETERRORCODE@0\n");
}

__export __stdcall void PCIOPENEX(int a)
{
    D_printf("PCIOPENEX@4\n");
}

__export __stdcall void PCIOPEN(void)
{
    D_printf("PCIOPEN@0\n");
}

__export __stdcall void PCICLOSE(void)
{
    D_printf("PCICLOSE@0\n");
}

__export __stdcall void PCIDEVICEEXISTS(int a)
{
    D_printf("PCIDEVICEEXISTS@4\n");
}

__export __stdcall void PCIGETCONFIGDATA(int a, int b, int c, int d, int e)
{
    D_printf("PCIGETCONFIGDATA@20\n");
}

__export __stdcall void PCIGETCONFIGDATARAW(int a, int b, int c, int d, int e)
{
    D_printf("PCIGETCONFIGDATARAW@20\n");
}

__export __stdcall void PCISETCONFIGDATA(int a, int b, int c, int d, int e)
{
    D_printf("PCISETCONFIGDATA@20\n");
}

__export __stdcall void PCISETCONFIGDATARAW(int a, int b, int c, int d, int e)
{
    D_printf("PCISETCONFIGDATARAW@20\n");
}

__export __stdcall void PCIFINDCARDMULTI(int a, int b, int c, int d)
{
    D_printf("PCIFINDCARDMULTI@16\n");
}

__export __stdcall void PCIFINDCARDMULTIFUNC(int a, int b, int c, int d, int e)
{
    D_printf("PCIFINDCARDMULTIFUNC@20\n");
}

__export __stdcall void PCIFINDCARD(int a, int b, int c)
{
    D_printf("PCIFINDCARD@12\n");
}

__export __stdcall void PCIMAPCARDMULTI(int a, int b, int c, int d, int e, int f)
{
    D_printf("PCIMAPCARDMULTI@24\n");
}

__export __stdcall void PCIMAPCARDMULTIFUNC(int a, int b, int c, int d, int e, int f)
{
    D_printf("PCIMAPCARDMULTIFUNC@24\n");
}

__export __stdcall void PCIMAPCARD(int a, int b, int c, int d, int e)
{
    D_printf("PCIMAPCARD@20\n");
}

__export __stdcall void PCIMAPPHYSICALTOLINEAR(int a, int b, int c)
{
    D_printf("PCIMAPPHYSICALTOLINEAR@12\n");
}

__export __stdcall void PCIMAPPHYSICALDEVICETOLINEAR(int a, int b, int c, int d)
{
    D_printf("PCIMAPPHYSICALDEVICETOLINEAR@16\n");
}

__export __stdcall void PCIUNMAPPHYSICAL(int a, int b)
{
    D_printf("PCIUNMAPPHYSICAL@8\n");
}

__export __stdcall void PCISETPASSTHROUGHBASE(int a, int b)
{
    D_printf("PCISETPASSTHROUGHBASE@8\n");
}

__export __stdcall void PCIOUTPUTDEBUGSTRING(int a)
{
    D_printf("PCIOUTPUTDEBUGSTRING@4\n");
}

__export __stdcall void PCILINEARRANGESETPERMISSION(int a, int b, int c)
{
    D_printf("PCILINEARRANGESETPERMISSION@12\n");
}
