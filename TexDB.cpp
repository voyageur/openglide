//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*           implementation of the TexDB class
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//**************************************************************

#include "glogl.h"
#include "TexDB.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TexDB::TexDB( void )
{
    for ( int i = 0; i < TEX_SECTIONS; i++ )
    {
        m_first[ i ] = NULL;
    }
}

TexDB::~TexDB( void )
{
    Record * r;

    for ( int i = 0; i < TEX_SECTIONS; i++ )
    {
        r = m_first[ i ];
        
        while ( r != NULL )
        {
            Record *tmp = r;
            r = r->next;
            delete tmp;
        }
    }
}

bool TexDB::Find( FxU32 startAddress, GrTexInfo *info, FxU32 hash, 
                  GLuint *pTexNum, bool *pal_change )
{
    Record  * r;
    FxU32   sect = startAddress / ( 32 * 1024 );

    if ( sect >= TEX_SECTIONS )
    {
        sect = TEX_SECTIONS - 1;
    }

    for ( r = m_first[ sect ]; r != NULL; r = r->next )
    {
        if ( r->Match( startAddress, info, ( pal_change == NULL ) ? hash : 0 ) )
        {
            *pTexNum = r->texNum;

            if ( ( pal_change != NULL ) && ( r->hash != hash ) )
            {
                r->hash = hash;
                *pal_change = true;
            }

#ifdef UTEXSS
            GlideMsg( "Found tex %d\n", r->texNum );
#endif
            return true;
        }
    }

#ifdef UTEX
    GlideMsg( "Tex not found\n" );
#endif

    return false;
}

void TexDB::WipeRange(FxU32 startAddress, FxU32 endAddress, FxU32 hash)
{
    Record  ** p;
    FxU32   stt_sect;
    FxU32   end_sect;

    stt_sect = startAddress / ( 32 * 1024 );
 
    if ( stt_sect >= TEX_SECTIONS )
    {
        stt_sect = TEX_SECTIONS - 1;
    }

    end_sect = endAddress / ( 32 * 1024 );

    if ( end_sect >= TEX_SECTIONS )
    {
        end_sect = TEX_SECTIONS - 1;
    }

    for ( FxU32 i = stt_sect; i <= end_sect; i++ )
    {
        p = &( m_first[ i ] );
        while ( *p != NULL )
        {
            Record * r = *p;
            
            if ( ( startAddress < r->endAddress ) && 
                 ( r->startAddress < endAddress ) && 
                 ( ( hash == 0 ) || ( r->hash == hash ) ) )
            {
                *p = r->next;
#ifdef UTEX
                GlideMsg( "Wipe tex %d\n", r->texNum );
#endif
                delete r;
            }
            else
            {
                p = &(r->next);
            }
        }
    }
}

GLuint TexDB::Add( FxU32 startAddress, FxU32 endAddress, GrTexInfo *info, FxU32 hash )
{
    Record  *r = new Record;
    FxU32   sect;

    sect = startAddress / ( 32 * 1024 );
    if ( sect >= TEX_SECTIONS )
    {
        sect = TEX_SECTIONS - 1;
    }

    r->startAddress = startAddress;
    r->endAddress = endAddress;
    r->info = *info;
    r->hash = hash;

    r->next = m_first[ sect ];
    m_first[ sect ] = r;

#ifdef UTEX
    GlideMsg( "Add tex %d\n", r->texNum );
#endif

    return r->texNum;
}


void TexDB::Clear( void )
{
    Record  * r;

    for ( int i = 0; i < TEX_SECTIONS; i++ )
    {
        r = m_first[ i ];
        
        while ( r != NULL )
        {
            Record *tmp = r;
            r = r->next;
            delete tmp;
        }

        m_first[ i ] = NULL;
    }
}
