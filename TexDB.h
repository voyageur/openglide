// TexDB.h: interface for the TexDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXDB_H__4D8DBC18_A31F_4E9E_9863_E0DC9635872E__INCLUDED_)
#define AFX_TEXDB_H__4D8DBC18_A31F_4E9E_9863_E0DC9635872E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TexDB  
{
public:
    struct Record
    {
        FxU32 startAddress;
        FxU32 endAddress;
        GrTexInfo info;
        FxU32 palette[256];
        GLuint texNum;
        Record *next;

        Record()
        {
            glGenTextures(1, &texNum);
        };

        ~Record()
        {
            glDeleteTextures(1, &texNum);
        };

        bool Match(FxU32 stt, GrTexInfo *inf, FxU32 *pal)
        {
            return startAddress == stt
                && inf->largeLod == info.largeLod
                && inf->aspectRatio == info.aspectRatio
                && inf->format == info.format
                && ((inf->format == GR_TEXFMT_P_8 || inf->format == GR_TEXFMT_AP_88)
                                  ? memcmp(palette, pal, sizeof(palette)) == 0
                                  : true);
        };
    };
	GLuint Add(FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 *palette);
	void WipeRange(FxU32 startAddress, FxU32 endAddress);
	bool Find(FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 *palette, GLuint *pTexNum);
	TexDB();
	virtual ~TexDB();
private:
    Record *m_first;
};

#endif // !defined(AFX_TEXDB_H__4D8DBC18_A31F_4E9E_9863_E0DC9635872E__INCLUDED_)
