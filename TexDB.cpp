// TexDB.cpp: implementation of the TexDB class.
//
//////////////////////////////////////////////////////////////////////

#include "glogl.h"
#include "TexDB.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TexDB::TexDB()
{
    int i;

    for(i = 0; i < TEX_SECTIONS; i++)
        m_first[i] = NULL;
}

TexDB::~TexDB()
{
    Record *r;
    int i;

    for(i = 0; i < TEX_SECTIONS; i++)
    {
        r = m_first[i];
        
        while(r != NULL)
        {
            Record *tmp = r;
            r = r->next;
            delete tmp;
        }
    }
}

bool TexDB::Find(FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 hash, GLuint *pTexNum)
{
    Record *r;
    FxU32 sect;

    sect = startAddress / (32*1024);
    if(sect >= TEX_SECTIONS)
        sect = TEX_SECTIONS - 1;

    if(info->format != GR_TEXFMT_P_8 && info->format != GR_TEXFMT_AP_88)
        hash = 0;

    for(r = m_first[sect]; r != NULL; r = r->next)
    {
        if(r->Match(startAddress, info, hash))
        {
            *pTexNum = r->texNum;
            return true;
        }
    }

    return false;
}

void TexDB::WipeRange(FxU32 startAddress, FxU32 endAddress)
{
    Record **p;
    FxU32 stt_sect;
    FxU32 end_sect;
    FxU32 i;

    stt_sect = startAddress / (32*1024);
    if(stt_sect >= TEX_SECTIONS)
        stt_sect = TEX_SECTIONS - 1;

    end_sect = endAddress / (32*1024);
    if(end_sect >= TEX_SECTIONS)
        end_sect = TEX_SECTIONS - 1;

    for(i = stt_sect; i <= end_sect; i++)
    {
        p = &(m_first[i]);
        while(*p != NULL)
        {
            Record *r = *p;
            
            if(startAddress < r->endAddress && r->startAddress < endAddress)
            {
                *p = r->next;
                delete r;
            }
            else
            {
                p = &(r->next);
            }
        }
    }
}

GLuint TexDB::Add(FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 hash)
{
    Record *r = new Record;
    FxU32 sect;

    sect = startAddress / (32*1024);
    if(sect >= TEX_SECTIONS)
        sect = TEX_SECTIONS - 1;

    if(info->format != GR_TEXFMT_P_8 && info->format != GR_TEXFMT_AP_88)
        hash = 0;

    r->startAddress = startAddress;
    r->endAddress = endAddress;
    r->info = *info;
    r->hash = hash;

    r->next = m_first[sect];
    m_first[sect] = r;

    return r->texNum;
}

