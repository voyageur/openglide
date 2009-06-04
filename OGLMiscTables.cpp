
#include "OGLTables.h"

/* Move to grguSstGlide, since only use there.
 * These are unsafe as all users have to
 * take care not to buffer overrun.  Is better
 * to have a safe accessing function e.g
 * (InterpretScreenResolution, etc).
OGLWindow windowDimensions[ 16 ] =
{
    {  320,  200 }, // GR_RESOLUTION_320x200
    {  320,  240 }, // GR_RESOLUTION_320x240
    {  400,  256 }, // GR_RESOLUTION_400x256
    {  512,  384 }, // GR_RESOLUTION_512x384
    {  640,  200 }, // GR_RESOLUTION_640x200
    {  640,  350 }, // GR_RESOLUTION_640x350
    {  640,  400 }, // GR_RESOLUTION_640x400
    {  640,  480 }, // GR_RESOLUTION_640x480
    {  800,  600 }, // GR_RESOLUTION_800x600
    {  960,  720 }, // GR_RESOLUTION_960x720
    {  856,  480 }, // GR_RESOLUTION_856x480:
    {  512,  256 }, // GR_RESOLUTION_512x256:
    { 1024,  768 }, // GR_RESOLUTION_1024x768:
    { 1280, 1024 }, // GR_RESOLUTION_1280x1024:
    { 1600, 1200 }, // GR_RESOLUTION_1600x1200:
    {  400,  300 }  // GR_RESOLUTION_400x300:
};

int windowRefresh[ 9 ] =
{
     60, // GR_REFRESH_60Hz
     70, // GR_REFRESH_70Hz
     72, // GR_REFRESH_72Hz
     75, // GR_REFRESH_75Hz
     80, // GR_REFRESH_80Hz
     90, // GR_REFRESH_90Hz
    100, // GR_REFRESH_100Hz
     85, // GR_REFRESH_85Hz
    120  // GR_REFRESH_120Hz
};
*/
