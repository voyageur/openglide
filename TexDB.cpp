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
    m_first = NULL;
}

TexDB::~TexDB()
{
    Record *r;

    r = m_first;
    while(r != NULL)
    {
        Record *tmp = r;
        r = r->next;
        delete tmp;
    }
}

bool TexDB::Find(FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 *palette, GLuint *pTexNum)
{
    Record *r;

    for(r = m_first; r != NULL; r = r->next)
    {
        if(r->Match(startAddress, info, palette))
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

    p = &m_first;
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

GLuint TexDB::Add(FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 *palette)
{
    Record *r = new Record;

    r->startAddress = startAddress;
    r->endAddress = endAddress;
    r->info = *info;
    memcpy(r->palette, palette, sizeof(r->palette));

    r->next = m_first;
    m_first = r;

    return r->texNum;
}
