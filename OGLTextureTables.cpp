
#include "OGLTables.h"


OGLAspect texAspects[ 7 ] =
{
    { D8OVER256, D1OVER256 }, // GR_ASPECT_8x1
    { D4OVER256, D1OVER256 }, // GR_ASPECT_4x1
    { D2OVER256, D1OVER256 }, // GR_ASPECT_2x1
    { D1OVER256, D1OVER256 }, // GR_ASPECT_1x1
    { D1OVER256, D2OVER256 }, // GR_ASPECT_1x2
    { D1OVER256, D4OVER256 }, // GR_ASPECT_1x4
    { D1OVER256, D8OVER256 }  // GR_ASPECT_1x8
};

DWORD nSquareLod[ 2 ][ 7 ][ 9 ] =
{
    {
        {   8192,  2048,  512,  128,  32,   8,  2, 1, 1 },
        {  16384,  4096, 1024,  256,  64,  16,  4, 1, 1 },
        {  32768,  8192, 2048,  512, 128,  32,  8, 2, 1 },
        {  65536, 16384, 4096, 1024, 256,  64, 16, 4, 1 },
        {  32768,  8192, 2048,  512, 128,  32,  8, 2, 1 },
        {  16384,  4096, 1024,  256,  64,  16,  4, 1, 1 },
        {   8192,  2048,  512,  128,  32,   8,  2, 1, 1 }
    },
    {
        {  16384,  4096, 1024,  256,  64,  16,  4, 1, 1 },
        {  32768,  8192, 2048,  512, 128,  32,  8, 2, 1 },
        {  65536, 16384, 4096, 1024, 256,  64, 16, 4, 1 },
        { 131072, 32768, 8192, 2048, 512, 128, 32, 8, 2 },
        {  65536, 16384, 4096, 1024, 256,  64, 16, 4, 1 },
        {  32768,  8192, 2048,  512, 128,  32,  8, 2, 1 },
        {  16384,  4096, 1024,  256,  64,  16,  4, 1, 1 }
    }
};

// Necessary memory for textures
// Big table to minimize calculation
DWORD nSquareTexLod[ 2 ][ 7 ][ 9 ][ 9 ] = // Tex type, aspect, largelod, smalllod
{
    { // Dual byte Textures
        { // GR_ASPECT_8x1
            {  16384,  20480,  21504,  21760,  21824,  21840,  21848,  21848,  21848 },
            {      0,   4096,   5120,   5376,   5440,   5456,   5464,   5464,   5464 },
            {      0,      0,   1024,   1280,   1344,   1360,   1368,   1368,   1368 },
            {      0,      0,      0,    256,    320,    336,    344,    344,    344 },
            {      0,      0,      0,      0,     64,     80,     88,     88,     88 },
            {      0,      0,      0,      0,      0,     16,     24,     24,     24 },
            {      0,      0,      0,      0,      0,      0,      8,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_4x1
            {  32768,  40960,  43008,  43520,  43648,  43680,  43688,  43696,  43696 },
            {      0,   8192,  10240,  10752,  10880,  10912,  10920,  10928,  10928 },
            {      0,      0,   2048,   2560,   2688,   2720,   2728,   2736,   2736 },
            {      0,      0,      0,    512,    640,    672,    680,    688,    688 },
            {      0,      0,      0,      0,    128,    160,    168,    176,    176 },
            {      0,      0,      0,      0,      0,     32,     40,     48,     48 },
            {      0,      0,      0,      0,      0,      0,      8,     16,     16 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_2x1
            {  65536,  81920,  86016,  87040,  87296,  87360,  87376,  87384,  87384 },
            {      0,  16384,  20480,  21504,  21760,  21824,  21840,  21848,  21848 },
            {      0,      0,   4096,   5120,   5376,   5440,   5456,   5464,   5464 },
            {      0,      0,      0,   1024,   1280,   1344,   1360,   1368,   1368 },
            {      0,      0,      0,      0,    256,    320,    336,    344,    344 },
            {      0,      0,      0,      0,      0,     64,     80,     88,     88 },
            {      0,      0,      0,      0,      0,      0,     16,     24,     24 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_1x1
            { 131072, 163840, 172032, 174080, 174592, 174720, 174752, 174760, 174768 },
            {      0,  32768,  40960,  43008,  43520,  43648,  43680,  43688,  43696 },
            {      0,      0,   8192,  10240,  10752,  10880,  10912,  10920,  10928 },
            {      0,      0,      0,   2048,   2560,   2688,   2720,   2728,   2736 },
            {      0,      0,      0,      0,    512,    640,    672,    680,    688 },
            {      0,      0,      0,      0,      0,    128,    160,    168,    176 },
            {      0,      0,      0,      0,      0,      0,     32,     40,     48 },
            {      0,      0,      0,      0,      0,      0,      0,      8,     16 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_1x2
            {  65536,  81920,  86016,  87040,  87296,  87360,  87376,  87384,  87384 },
            {      0,  16384,  20480,  21504,  21760,  21824,  21840,  21848,  21848 },
            {      0,      0,   4096,   5120,   5376,   5440,   5456,   5464,   5464 },
            {      0,      0,      0,   1024,   1280,   1344,   1360,   1368,   1368 },
            {      0,      0,      0,      0,    256,    320,    336,    344,    344 },
            {      0,      0,      0,      0,      0,     64,     80,     88,     88 },
            {      0,      0,      0,      0,      0,      0,     16,     24,     24 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_1x4
            {  32768,  40960,  43008,  43520,  43648,  43680,  43688,  43696,  43696 },
            {      0,   8192,  10240,  10752,  10880,  10912,  10920,  10928,  10928 },
            {      0,      0,   2048,   2560,   2688,   2720,   2728,   2736,   2736 },
            {      0,      0,      0,    512,    640,    672,    680,    688,    688 },
            {      0,      0,      0,      0,    128,    160,    168,    176,    176 },
            {      0,      0,      0,      0,      0,     32,     40,     48,     48 },
            {      0,      0,      0,      0,      0,      0,      8,     16,     16 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_1x8
            {  16384,  20480,  21504,  21760,  21824,  21840,  21848,  21848,  21848 },
            {      0,   4096,   5120,   5376,   5440,   5456,   5464,   5464,   5464 },
            {      0,      0,   1024,   1280,   1344,   1360,   1368,   1368,   1368 },
            {      0,      0,      0,    256,    320,    336,    344,    344,    344 },
            {      0,      0,      0,      0,     64,     80,     88,     88,     88 },
            {      0,      0,      0,      0,      0,     16,     24,     24,     24 },
            {      0,      0,      0,      0,      0,      0,      8,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        }
    },
    { // Single byte Textures
        { // GR_ASPECT_8x1
            {   8192,  10240,  10752,  10880,  10912,  10920,  10928,  10928,  10928 },
            {      0,   2048,   2560,   2688,   2720,   2728,   2736,   2736,   2736 },
            {      0,      0,    512,    640,    672,    680,    688,    688,    688 },
            {      0,      0,      0,    128,    160,    168,    176,    176,    176 },
            {      0,      0,      0,      0,     32,     40,     48,     48,     48 },
            {      0,      0,      0,      0,      0,      8,     16,     16,     16 },
            {      0,      0,      0,      0,      0,      0,      8,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_4x1
            {  16384,  20480,  21504,  21760,  21824,  21840,  21848,  21848,  21848 },
            {      0,   4096,   5120,   5376,   5440,   5456,   5464,   5464,   5464 },
            {      0,      0,   1024,   1280,   1344,   1360,   1368,   1368,   1368 },
            {      0,      0,      0,    256,    320,    336,    344,    344,    344 },
            {      0,      0,      0,      0,     64,     80,     88,     88,     88 },
            {      0,      0,      0,      0,      0,     16,     24,     24,     24 },
            {      0,      0,      0,      0,      0,      0,      8,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_2x1
            {  32768,  40960,  43008,  43520,  43648,  43680,  43688,  43696,  43696 },
            {      0,   8192,  10240,  10752,  10880,  10912,  10920,  10928,  10928 },
            {      0,      0,   2048,   2560,   2688,   2720,   2728,   2736,   2736 },
            {      0,      0,      0,    512,    640,    672,    680,    688,    688 },
            {      0,      0,      0,      0,    128,    160,    168,    176,    176 },
            {      0,      0,      0,      0,      0,     32,     40,     48,     48 },
            {      0,      0,      0,      0,      0,      0,      8,     16,     16 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_1x1
            {  65536,  81920,  86016,  87040,  87296,  87360,  87376,  87384,  87384 },
            {      0,  16384,  20480,  21504,  21760,  21824,  21840,  21848,  21848 },
            {      0,      0,   4096,   5120,   5376,   5440,   5456,   5464,   5464 },
            {      0,      0,      0,   1024,   1280,   1344,   1360,   1368,   1368 },
            {      0,      0,      0,      0,    256,    320,    336,    344,    344 },
            {      0,      0,      0,      0,      0,     64,     80,     88,     88 },
            {      0,      0,      0,      0,      0,      0,     16,     24,     24 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_1x2
            {  32768,  40960,  43008,  43520,  43648,  43680,  43688,  43696,  43696 },
            {      0,   8192,  10240,  10752,  10880,  10912,  10920,  10928,  10928 },
            {      0,      0,   2048,   2560,   2688,   2720,   2728,   2736,   2736 },
            {      0,      0,      0,    512,    640,    672,    680,    688,    688 },
            {      0,      0,      0,      0,    128,    160,    168,    176,    176 },
            {      0,      0,      0,      0,      0,     32,     40,     48,     48 },
            {      0,      0,      0,      0,      0,      0,      8,     16,     16 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_1x4
            {  16384,  20480,  21504,  21760,  21824,  21840,  21848,  21848,  21848 },
            {      0,   4096,   5120,   5376,   5440,   5456,   5464,   5464,   5464 },
            {      0,      0,   1024,   1280,   1344,   1360,   1368,   1368,   1368 },
            {      0,      0,      0,    256,    320,    336,    344,    344,    344 },
            {      0,      0,      0,      0,     64,     80,     88,     88,     88 },
            {      0,      0,      0,      0,      0,     16,     24,     24,     24 },
            {      0,      0,      0,      0,      0,      0,      8,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        },
        { // GR_ASPECT_1x8
            {   8192,  10240,  10752,  10880,  10912,  10920,  10928,  10928,  10928 },
            {      0,   2048,   2560,   2688,   2720,   2728,   2736,   2736,   2736 },
            {      0,      0,    512,    640,    672,    680,    688,    688,    688 },
            {      0,      0,      0,    128,    160,    168,    176,    176,    176 },
            {      0,      0,      0,      0,     32,     40,     48,     48,     48 },
            {      0,      0,      0,      0,      0,      8,     16,     16,     16 },
            {      0,      0,      0,      0,      0,      0,      8,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      8,      8 },
            {      0,      0,      0,      0,      0,      0,      0,      0,      8 },
        }
    }
};
