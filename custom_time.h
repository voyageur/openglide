/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"
 */
#ifndef _CUSTOM_TIME_H
#define _CUSTOM_TIME_H

void InitTime( void );
void MarkTimeThisTick( void );
float GetElapsedTime( void );
float GetExactTime( void );

#endif
