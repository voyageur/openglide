DECLARE_THUNK1(GUALPHASOURCE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GUCOLORCOMBINEFUNCTION, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK2(GUFOGGENERATEEXP, void, GrFog_t*, table, int, density)
    __faddr (table);
    __fcall (table, density);
ENDDECLARE

DECLARE_THUNK3(GUDRAWTRIANGLEWITHCLIP, void, GrVertex*, v1, GrVertex*, v2, GrVertex*, v3)
    __faddr (v1);
    __faddr (v2);
    __faddr (v3);
    __fcall (v1, v2, v3);
ENDDECLARE

DECLARE_THUNK2(GRHINTS, void, int, a, int, b)
    __fcall (a, b);
ENDDECLARE

DECLARE_THUNK1(GRSSTQUERYHARDWARE, FxBool, GrHwConfiguration*, hwConfig)
    FxBool ret;
    __faddr (hwConfig);
    ret = __fcall (hwConfig);
    RETURNI(ret);
ENDDECLARE

DECLARE_THUNK1(GRSSTSELECT, void, int, which_sst)
    __fcall (which_sst);
ENDDECLARE

DECLARE_THUNK4(GRTEXDOWNLOADMIPMAP, void, GrChipID_t, tmu, FxU32, startAddress,
                                    FxU32, evenOdd, GrTexInfo*, info)
    __faddr (info);
    __fcall (tmu, startAddress, evenOdd, info);
ENDDECLARE

DECLARE_THUNK15(GUTEXALLOCATEMEMORY, int,
                                     GrChipID_t, tmu, FxU8, odd_even_mask,
                                     int, width, int, height, GrTextureFormat_t, fmt,
                                     GrMipMapMode_t, mm_mode, GrLOD_t, smallest_lod,
                                     GrLOD_t, largest_lod, GrAspectRatio_t, aspect,
                                     GrTextureClampMode_t, s_clamp_mode,
                                     GrTextureClampMode_t, t_clamp_mode,
                                     GrTextureFilterMode_t, minfilter_mode,
                                     GrTextureFilterMode_t, magfilter_mode,
                                     int, lod_bias, FxBool, trilinear)
    int ret;
    ret = __fcall (tmu, odd_even_mask, width, height, fmt,
                   mm_mode, smallest_lod, largest_lod, aspect,
                   s_clamp_mode, t_clamp_mode, minfilter_mode,
                   magfilter_mode, lod_bias, trilinear);
    return ret;
ENDDECLARE

DECLARE_THUNK2(GRTEXCOMBINEFUNCTION, void, int, a, int, b)
    __fcall (a, b);
ENDDECLARE

DECLARE_THUNK3(GUTEXDOWNLOADMIPMAP, void, GrMipMapId_t, mmid, void*, src, GuNccTable*, table)
    __faddr (src);
    __faddr (table);
    __fcall (mmid, src, table);
ENDDECLARE

DECLARE_THUNK0(GUTEXMEMRESET, void)
    __fcall ();
ENDDECLARE

DECLARE_THUNK2(GRAADRAWLINE, void, GrVertex*, v1, GrVertex*, v2)
    __faddr (v1);
    __faddr (v2);
    __fcall (v1, v2);
ENDDECLARE

DECLARE_THUNK2(GRDRAWLINE, void, GrVertex*, v1, GrVertex*, v2)
    __faddr (v1);
    __faddr (v2);
    __fcall (v1, v2);
ENDDECLARE

DECLARE_THUNK3(GRDRAWTRIANGLE, void, GrVertex*, v1, GrVertex*, v2, GrVertex*, v3)
    __faddr (v1);
    __faddr (v2);
    __faddr (v3);
    __fcall (v1, v2, v3);
ENDDECLARE

DECLARE_THUNK2(GRDRAWPOLYGONVERTEXLIST, void, int, nverts, GrVertex*, vlist)
    __faddr (vlist);
    __fcall (nverts, vlist);
ENDDECLARE

DECLARE_THUNK4(GRALPHABLENDFUNCTION, void, int, a, int, b, int, c, int, d)
    __fcall (a, b, c, d);
ENDDECLARE

DECLARE_THUNK5(GRALPHACOMBINE, void, int, a, int, b, int, c, int, d, int, e)
    __fcall (a, b, c, d, e);
ENDDECLARE

DECLARE_THUNK1(GRALPHATESTFUNCTION, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK3(GRBUFFERCLEAR, void, int, a, int, b, int, c)
    __fcall (a, b, c);
ENDDECLARE

/* FIXME? */
DECLARE_THUNK1(GRBUFFERSWAP, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GRCHROMAKEYMODE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GRCHROMAKEYVALUE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK4(GRCLIPWINDOW, void, int, a, int, b, int, c, int, d)
    __fcall (a, b, c, d);
ENDDECLARE

DECLARE_THUNK5(GRCOLORCOMBINE, void, int, a, int, b, int, c, int, d, int, e)
    __fcall (a, b, c, d, e);
ENDDECLARE

DECLARE_THUNK2(GRCOLORMASK, void, int, a, int, b)
    __fcall (a, b);
ENDDECLARE

DECLARE_THUNK1(GRCONSTANTCOLORVALUE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK4(GRCONSTANTCOLORVALUE4, void, int, a, int, b, int, c, int, d)
    __fcall (a, b, c, d);
ENDDECLARE

DECLARE_THUNK1(GRCULLMODE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GRDEPTHBIASLEVEL, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GRDEPTHBUFFERFUNCTION, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GRDEPTHBUFFERMODE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GRDEPTHMASK, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK0(GRDISABLEALLEFFECTS, void)
    __fcall ();
ENDDECLARE

DECLARE_STUB1(GRDITHERMODE, void, int, a)
ENDDECLARE

DECLARE_THUNK1(GRFOGMODE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GRFOGCOLORVALUE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK1(GRFOGTABLE, void, GrFog_t*, ft)
    __faddr (ft);
    __fcall (ft);
ENDDECLARE

DECLARE_THUNK1(GRRENDERBUFFER, void, int, a)
    __fcall (a);
ENDDECLARE

/* FIXME? */
DECLARE_THUNK0(GRGLIDESHUTDOWN, void)
    __fcall ();
ENDDECLARE

/* FIXME! */
DECLARE_THUNK6(GRLFBLOCK, FxBool,
                          GrLock_t,           type,
                          GrBuffer_t,         buffer,
                          GrLfbWriteMode_t,   writeMode,
                          GrOriginLocation_t, origin,
                          FxBool,             pixelPipeline,
                          GrLfbInfo_t*,       info)
#if 0
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

#endif
    return FXTRUE;
ENDDECLARE

DECLARE_THUNK7(GRSSTWINOPEN, FxBool, int, a, int, b, int, c,
                             int, d, int, e, int, f, int, g)
    FxBool ret;
    ret = __fcall (a, b, c, d, e, f, g);
    RETURNI(ret);
ENDDECLARE

DECLARE_THUNK0(GRSSTWINCLOSE, void)
    __fcall ();
ENDDECLARE

DECLARE_THUNK1(GRGAMMACORRECTIONVALUE, void, int, a)
    __fcall (a);
ENDDECLARE

DECLARE_THUNK3(GRTEXCLAMPMODE, void, int, a, int, b, int, c)
    __fcall (a, b, c);
ENDDECLARE

DECLARE_THUNK7(GRTEXCOMBINE, void, int, a, int, b, int, c, int, d,
                             int, e, int, f, int, g)
    __fcall (a, b, c, d, e, f, g);
ENDDECLARE

DECLARE_THUNK3(GRTEXFILTERMODE, void, int, a, int, b, int, c)
    __fcall (a, b, c);
ENDDECLARE

DECLARE_THUNK2(GRTEXLODBIASVALUE, void, int, a, int, b)
    __fcall (a, b);
ENDDECLARE

DECLARE_THUNK3(GRTEXMIPMAPMODE, void, GrChipID_t, tmu, GrMipMapMode_t, mode, FxBool, lodBlend)
    __fcall (tmu, mode, lodBlend);
ENDDECLARE

DECLARE_THUNK4(GRTEXSOURCE, void, GrChipID_t, tmu, FxU32, startAddress, FxU32, evenOdd, GrTexInfo*, info)
    __faddr (info);
    __fcall (tmu, startAddress, evenOdd, info);
ENDDECLARE

DECLARE_THUNK3(GRTEXDOWNLOADTABLE, void, GrChipID_t, tmu, GrTexTable_t, type, void*, data)
    __faddr (data);
    __fcall (tmu, type, data);
ENDDECLARE

DECLARE_THUNK1(GUTEXSOURCE, void, int, a)
    __fcall (a);
ENDDECLARE
