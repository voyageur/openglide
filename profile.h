/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"
 */

#ifndef _PROFILE_H
#define _PROFILE_H

void ProfileInit( void );
void ProfileBegin( char* name );
void ProfileEnd( char* name );
void ProfileDumpOutputToBuffer( void );
void StoreProfileInHistory( char* name, float percent );
void GetProfileFromHistory( char* name, float* ave, float* min, float* max );
void ProfileDraw( void );

#endif
