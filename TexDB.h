// TexDB.h: interface for the TexDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXDB_H__4D8DBC18_A31F_4E9E_9863_E0DC9635872E__INCLUDED_)
#define AFX_TEXDB_H__4D8DBC18_A31F_4E9E_9863_E0DC9635872E__INCLUDED_

#include <windows.h>
#include "glide.h"
#include "GL/gl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TexDB  
{
public:
	void Clear();
    struct Record
    {
        FxU32 startAddress;
        FxU32 endAddress;
        GrTexInfo info;
        FxU32 hash;
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

        bool Match(FxU32 stt, GrTexInfo *inf, FxU32 h)
        {
            return (startAddress == stt
                && inf->largeLod == info.largeLod
                && inf->aspectRatio == info.aspectRatio
                && inf->format == info.format
                && (hash == h || h == 0));
        };
    };
	GLuint Add(FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 hash);
	void WipeRange(FxU32 startAddress, FxU32 endAddress, FxU32 hash);
	bool Find(FxU32 startAddress, GrTexInfo *info, FxU32 hash, GLuint *pTexNum, bool *pal_change);
	TexDB();
	virtual ~TexDB();
private:
    enum {TEX_SECTIONS = 256};
    Record *m_first[TEX_SECTIONS];
};

#endif // !defined(AFX_TEXDB_H__4D8DBC18_A31F_4E9E_9863_E0DC9635872E__INCLUDED_)
