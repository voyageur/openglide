/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"
 */
#ifdef _WIN32
#include <windows.h>
#include <time.h>
#else
#include <stdio.h>
#include <sys/timeb.h>
#endif


#ifdef _WIN32
float g_StartTime = -1.0f;
#else
struct timeb   g_StartTimeb;
#endif
float g_CurrentTime = -1.0f;
float g_TimeLastTick = -1.0f;


void InitTime( void )
{
#ifdef _WIN32
	g_StartTime = (1.0) / 1000.0f;
#else
	ftime(&g_StartTimeb);
#endif
	g_CurrentTime = 0.0f;
	g_TimeLastTick = 0.001f;
}

void MarkTimeThisTick( void )
{
#ifdef _WIN32
	float newTime = ((1.0) / 1000.0f) - g_StartTime;
#else
	struct timeb tp;

	ftime(&tp);

	float newTime = tp.time - g_StartTimeb.time +
	  (tp.millitm/1000.0) - (g_StartTimeb.millitm/1000.0);

#endif

	g_TimeLastTick = newTime - g_CurrentTime;
	g_CurrentTime = newTime;

	if( g_TimeLastTick <= 0.0f ) {
		g_TimeLastTick = 0.001f;
	}

}

float GetElapsedTime( void )
{
	return( g_TimeLastTick );
}


float GetExactTime( void )
{
#ifdef _WIN32
	return( (1.0)/1000.0f );
#else
	struct timeb tp;

	ftime(&tp);

	return tp.time - g_StartTimeb.time +
	  (tp.millitm/1000.0) - (g_StartTimeb.millitm/1000.0);
#endif
}

