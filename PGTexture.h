// PGTexture.h: interface for the PGTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PGTEXTURE_H__01BF3022_514F_4D5E_B0DF_F188A6B61846__INCLUDED_)
#define AFX_PGTEXTURE_H__01BF3022_514F_4D5E_B0DF_F188A6B61846__INCLUDED_

#include "Glide.h"	// Added by ClassView
#include "TexDB.h"	// Added by ClassView
#include "3dfx.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PGTexture  
{
    struct TexValues
    {
        GrLOD_t lod;
        FxU32 width;
        FxU32 height;
        FxU32 nPixels;
    };

private:
	FxU32 m_palette_hash;
	void ApplyKeyToPalette(FxU32 *pal);
	TexDB m_db;
	GrChromakeyMode_t m_chromakey_mode;
	GrColor_t m_chromakey_value;
	float m_wAspect;
	float m_hAspect;
	void GetTexValues(TexValues *tval);

	FxU32 m_tex_temp[256*256];
	static FxU32 MipMapMemRequired(GrLOD_t lod, GrAspectRatio_t aspectRatio, GrTextureFormat_t format);
	bool m_valid;
    enum {TEX_MEMORY = 8 * 1024 * 1024};
    FxU8 m_memory[TEX_MEMORY];
    FxU32 m_startAddress;
    FxU32 m_evenOdd;
    GrTexInfo m_info;
    FxU32 m_palette[256];

public:
	void ChromakeyMode(GrChromakeyMode_t mode);
	void ChromakeyValue(GrColor_t value);
	void GetAspect(float *hAspect, float *wAspect);
	void Clear();
	static FxU32 TextureMemRequired(FxU32 evenOdd, GrTexInfo *info);
	void MakeReady();
	void DownloadTable(GrTexTable_t type, void *data);
	void Source(FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info);
	void DownloadMipMap(FxU32 startAddress, FxU32 evenOdd, GrTexInfo *info);
	PGTexture();
	virtual ~PGTexture();
};

#endif // !defined(AFX_PGTEXTURE_H__01BF3022_514F_4D5E_B0DF_F188A6B61846__INCLUDED_)
