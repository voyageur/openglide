
#include "GlOgl.h"
#include "GLRender.h"
#include "OGLTables.h"

OGLColorTable colorCombineTable[ 14 ][ 17 ] =
{
    { // GR_COMBINE_FACTOR_ZERO          0x0
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3Zero, ColorFunctionLocal },                               // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3Zero, ColorFunctionLocalAlpha },                          // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false, false, false, false, ColorFactor3Zero, ColorFunctionScaleOther },                          // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true, false, false, false, ColorFactor3Zero, ColorFunctionScaleOtherAddLocal },                  // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false,  true, false, ColorFactor3Zero, ColorFunctionScaleOtherAddLocalAlpha },             // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, false, false, ColorFactor3Zero, ColorFunctionScaleOtherMinusLocal },                // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true, false, false, false, ColorFactor3Zero, ColorFunctionScaleOtherMinusLocalAddLocal },        // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false, false,  true, false, ColorFactor3Zero, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false, false, ColorFactor3Zero, ColorFunctionMinusLocalAddLocal },                  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xa
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xb
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xc
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xd
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xe
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xf
        { false, false,  true, false, ColorFactor3Zero, ColorFunctionMinusLocalAddLocalAlpha }              // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_LOCAL         0x1
        { false, false, false, false, ColorFactor3Local, ColorFunctionZero },                               // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3Local, ColorFunctionLocal },                              // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false,  true, false, ColorFactor3Local, ColorFunctionLocalAlpha },                         // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        {  true,  true, false, false, ColorFactor3Local, ColorFunctionScaleOther },                         // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false, false, ColorFactor3Local, ColorFunctionScaleOtherAddLocal },                 // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true,  true, false, ColorFactor3Local, ColorFunctionScaleOtherAddLocalAlpha },            // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, false, false, ColorFactor3Local, ColorFunctionScaleOtherMinusLocal },               // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false, false, ColorFactor3Local, ColorFunctionScaleOtherMinusLocalAddLocal },       // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true,  true, false, ColorFactor3Local, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false, false, ColorFactor3Local, ColorFunctionMinusLocalAddLocal },                 // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3Local, ColorFunctionZero },                               // 0xa
        { false, false, false, false, ColorFactor3Local, ColorFunctionZero },                               // 0xb
        { false, false, false, false, ColorFactor3Local, ColorFunctionZero },                               // 0xc
        { false, false, false, false, ColorFactor3Local, ColorFunctionZero },                               // 0xd
        { false, false, false, false, ColorFactor3Local, ColorFunctionZero },                               // 0xe
        { false, false, false, false, ColorFactor3Local, ColorFunctionZero },                               // 0xf
        {  true, false,  true, false, ColorFactor3Local, ColorFunctionMinusLocalAddLocalAlpha }             // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_OTHER_ALPHA   0x2
        { false, false, false, false, ColorFactor3OtherAlpha, ColorFunctionZero },                          // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3OtherAlpha, ColorFunctionLocal },                         // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3OtherAlpha, ColorFunctionLocalAlpha },                    // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, false,  true, ColorFactor3OtherAlpha, ColorFunctionScaleOther },                    // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false,  true, ColorFactor3OtherAlpha, ColorFunctionScaleOtherAddLocal },            // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false,  true,  true,  true, ColorFactor3OtherAlpha, ColorFunctionScaleOtherAddLocalAlpha },       // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, false,  true, ColorFactor3OtherAlpha, ColorFunctionScaleOtherMinusLocal },          // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false,  true, ColorFactor3OtherAlpha, ColorFunctionScaleOtherMinusLocalAddLocal },       // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true,  true,  true, ColorFactor3OtherAlpha, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false,  true, ColorFactor3OtherAlpha, ColorFunctionMinusLocalAddLocal },            // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3OtherAlpha, ColorFunctionZero },                          // 0xa
        { false, false, false, false, ColorFactor3OtherAlpha, ColorFunctionZero },                          // 0xb
        { false, false, false, false, ColorFactor3OtherAlpha, ColorFunctionZero },                          // 0xc
        { false, false, false, false, ColorFactor3OtherAlpha, ColorFunctionZero },                          // 0xd
        { false, false, false, false, ColorFactor3OtherAlpha, ColorFunctionZero },                          // 0xe
        { false, false, false, false, ColorFactor3OtherAlpha, ColorFunctionZero },                          // 0xf
        {  true, false,  true,  true, ColorFactor3OtherAlpha, ColorFunctionMinusLocalAddLocalAlpha }        // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_LOCAL_ALPHA   0x3
        { false, false, false, false, ColorFactor3LocalAlpha, ColorFunctionZero },                          // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3LocalAlpha, ColorFunctionLocal },                         // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3LocalAlpha, ColorFunctionLocalAlpha },                    // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true,  true, false, ColorFactor3LocalAlpha, ColorFunctionScaleOther },                    // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true,  true, false, ColorFactor3LocalAlpha, ColorFunctionScaleOtherAddLocal },            // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false,  true,  true, false, ColorFactor3LocalAlpha, ColorFunctionScaleOtherAddLocalAlpha },       // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true,  true, false, ColorFactor3LocalAlpha, ColorFunctionScaleOtherMinusLocal },          // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true,  true, false, ColorFactor3LocalAlpha, ColorFunctionScaleOtherMinusLocalAddLocal },       // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true,  true, false, ColorFactor3LocalAlpha, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false,  true, false, ColorFactor3LocalAlpha, ColorFunctionMinusLocalAddLocal },            // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3LocalAlpha, ColorFunctionZero },                          // 0xa
        { false, false, false, false, ColorFactor3LocalAlpha, ColorFunctionZero },                          // 0xb
        { false, false, false, false, ColorFactor3LocalAlpha, ColorFunctionZero },                          // 0xc
        { false, false, false, false, ColorFactor3LocalAlpha, ColorFunctionZero },                          // 0xd
        { false, false, false, false, ColorFactor3LocalAlpha, ColorFunctionZero },                          // 0xe
        { false, false, false, false, ColorFactor3LocalAlpha, ColorFunctionZero },                          // 0xf
        {  true, false,  true, false, ColorFactor3LocalAlpha, ColorFunctionMinusLocalAddLocalAlpha }        // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_TEXTURE_ALPHA 0x4
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3One, ColorFunctionLocal },                                // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3One, ColorFunctionLocalAlpha },                           // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, false, false, ColorFactor3One, ColorFunctionScaleOther },                           // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false,  true, false, ColorFactor3One, ColorFunctionScaleOtherAddLocalAlpha },              // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocal },                 // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocal },         // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false,  true,  true, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },    // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false, false, ColorFactor3One, ColorFunctionMinusLocalAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xa
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xb
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xc
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xd
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xe
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xf
        { false, false,  true, false, ColorFactor3One, ColorFunctionMinusLocalAddLocalAlpha }               // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_TEXTURE_RGB   0x5
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3One, ColorFunctionLocal },                                // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3One, ColorFunctionLocalAlpha },                           // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, false, false, ColorFactor3One, ColorFunctionScaleOther },                           // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false,  true, false, ColorFactor3One, ColorFunctionScaleOtherAddLocalAlpha },              // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocal },                 // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocal },         // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false,  true,  true, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },    // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false, false, ColorFactor3One, ColorFunctionMinusLocalAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xa
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xb
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xc
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xd
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xe
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xf
        { false, false,  true, false, ColorFactor3One, ColorFunctionMinusLocalAddLocalAlpha }               // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // 6
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_ZERO        0x0
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xa
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xb
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xc
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xd
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xe
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xf
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero }                                 // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // 7
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_ZERO        0x0
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xa
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xb
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xc
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xd
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xe
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero },                                // 0xf
        { false, false, false, false, ColorFactor3Zero, ColorFunctionZero }                                 // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE           0x8
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3One, ColorFunctionLocal },                                // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3One, ColorFunctionLocalAlpha },                           // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, false, false, ColorFactor3One, ColorFunctionScaleOther },                           // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false,  true, false, ColorFactor3One, ColorFunctionScaleOtherAddLocalAlpha },              // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocal },                 // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocal },         // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false,  true,  true, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },    // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false, false, ColorFactor3One, ColorFunctionMinusLocalAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xa
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xb
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xc
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xd
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xe
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xf
        { false, false,  true, false, ColorFactor3One, ColorFunctionMinusLocalAddLocalAlpha }               // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_LOCAL 0x9
        { false, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionZero },                       // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionLocal },                      // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3OneMinusLocal, ColorFunctionLocalAlpha },                 // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        {  true,  true, false, false, ColorFactor3OneMinusLocal, ColorFunctionScaleOther },                 // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false, false, ColorFactor3OneMinusLocal, ColorFunctionScaleOtherAddLocal },         // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true,  true, false, ColorFactor3OneMinusLocal, ColorFunctionScaleOtherAddLocalAlpha },    // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, false, false, ColorFactor3OneMinusLocal, ColorFunctionScaleOtherMinusLocal },       // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false, false, ColorFactor3OneMinusLocal, ColorFunctionScaleOtherMinusLocalAddLocal },       // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true,  true, false, ColorFactor3OneMinusLocal, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionMinusLocalAddLocal },         // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionZero },                       // 0xa
        { false, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionZero },                       // 0xb
        { false, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionZero },                       // 0xc
        { false, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionZero },                       // 0xd
        { false, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionZero },                       // 0xe
        { false, false, false, false, ColorFactor3OneMinusLocal, ColorFunctionZero },                       // 0xf
        {  true, false,  true, false, ColorFactor3OneMinusLocal, ColorFunctionMinusLocalAddLocalAlpha }     // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA 0xa
        { false, false, false, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionZero },                  // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionLocal },                 // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionLocalAlpha },            // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, false,  true, ColorFactor3OneMinusOtherAlpha, ColorFunctionScaleOther },            // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false,  true, ColorFactor3OneMinusOtherAlpha, ColorFunctionScaleOtherAddLocal },    // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false,  true,  true,  true, ColorFactor3OneMinusOtherAlpha, ColorFunctionScaleOtherAddLocalAlpha },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, false,  true, ColorFactor3OneMinusOtherAlpha, ColorFunctionScaleOtherMinusLocal },      // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false,  true, ColorFactor3OneMinusOtherAlpha, ColorFunctionScaleOtherMinusLocalAddLocal },       // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true,  true,  true, ColorFactor3OneMinusOtherAlpha, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false,  true, ColorFactor3OneMinusOtherAlpha, ColorFunctionMinusLocalAddLocal },         // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionZero },                  // 0xa
        { false, false, false, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionZero },                  // 0xb
        { false, false, false, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionZero },                  // 0xc
        { false, false, false, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionZero },                  // 0xd
        { false, false, false, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionZero },                  // 0xe
        { false, false, false, false, ColorFactor3OneMinusOtherAlpha, ColorFunctionZero },                  // 0xf
        {  true, false,  true,  true, ColorFactor3OneMinusOtherAlpha, ColorFunctionMinusLocalAddLocalAlpha }     // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA 0xb
        { false, false, false, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionZero },                  // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionLocal },                 // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionLocalAlpha },            // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionScaleOther },            // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionScaleOtherAddLocal },    // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false,  true,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionScaleOtherAddLocalAlpha },    // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionScaleOtherMinusLocal },       // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionScaleOtherMinusLocalAddLocal },       // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionMinusLocalAddLocal },         // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionZero },                  // 0xa
        { false, false, false, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionZero },                  // 0xb
        { false, false, false, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionZero },                  // 0xc
        { false, false, false, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionZero },                  // 0xd
        { false, false, false, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionZero },                  // 0xe
        { false, false, false, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionZero },                  // 0xf
        {  true, false,  true, false, ColorFactor3OneMinusLocalAlpha, ColorFunctionMinusLocalAddLocalAlpha }     // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA 0xc
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3One, ColorFunctionLocal },                                // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3One, ColorFunctionLocalAlpha },                           // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, false, false, ColorFactor3One, ColorFunctionScaleOther },                           // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false,  true, false, ColorFactor3One, ColorFunctionScaleOtherAddLocalAlpha },              // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocal },                 // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocal },         // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false,  true,  true, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },    // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false, false, ColorFactor3One, ColorFunctionMinusLocalAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xa
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xb
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xc
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xd
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xe
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xf
        { false, false,  true, false, ColorFactor3One, ColorFunctionMinusLocalAddLocalAlpha }               // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION 0xd
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, false, false, ColorFactor3One, ColorFunctionLocal },                                // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false,  true, false, ColorFactor3One, ColorFunctionLocalAlpha },                           // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, false, false, ColorFactor3One, ColorFunctionScaleOther },                           // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false,  true, false, ColorFactor3One, ColorFunctionScaleOtherAddLocalAlpha },              // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocal },                 // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, false, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocal },         // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false,  true,  true, false, ColorFactor3One, ColorFunctionScaleOtherMinusLocalAddLocalAlpha },    // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, false, false, ColorFactor3One, ColorFunctionMinusLocalAddLocal },                   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xa
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xb
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xc
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xd
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xe
        { false, false, false, false, ColorFactor3One, ColorFunctionZero },                                 // 0xf
        { false, false,  true, false, ColorFactor3One, ColorFunctionMinusLocalAddLocalAlpha }               // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    }
};

OGLAlphaTable alphaCombineTable[ 14 ][ 17 ] =
{
    { // GR_COMBINE_FACTOR_ZERO          0x0
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorZero },  // 0xa
        { false, false, AlphaFactorZero },  // 0xb
        { false, false, AlphaFactorZero },  // 0xc
        { false, false, AlphaFactorZero },  // 0xd
        { false, false, AlphaFactorZero },  // 0xe
        { false, false, AlphaFactorZero },  // 0xf
        {  true, false, AlphaFactorZero }   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_LOCAL         0x1
        { false, false, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorLocal }, // 0xa
        { false, false, AlphaFactorLocal }, // 0xb
        { false, false, AlphaFactorLocal }, // 0xc
        { false, false, AlphaFactorLocal }, // 0xd
        { false, false, AlphaFactorLocal }, // 0xe
        { false, false, AlphaFactorLocal }, // 0xf
        {  true, false, AlphaFactorLocal }  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_OTHER_ALPHA   0x2
        { false, false, AlphaFactorOther }, // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOther }, // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOther }, // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, AlphaFactorOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true,  true, AlphaFactorOther }, // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOther }, // 0xa
        { false, false, AlphaFactorOther }, // 0xb
        { false, false, AlphaFactorOther }, // 0xc
        { false, false, AlphaFactorOther }, // 0xd
        { false, false, AlphaFactorOther }, // 0xe
        { false, false, AlphaFactorOther }, // 0xf
        {  true,  true, AlphaFactorOther }  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_LOCAL_ALPHA   0x3
        { false, false, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorLocal }, // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorLocal }, // 0xa
        { false, false, AlphaFactorLocal }, // 0xb
        { false, false, AlphaFactorLocal }, // 0xc
        { false, false, AlphaFactorLocal }, // 0xd
        { false, false, AlphaFactorLocal }, // 0xe
        { false, false, AlphaFactorLocal }, // 0xf
        {  true, false, AlphaFactorLocal }  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_TEXTURE_ALPHA 0x4
        { false, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOne },   // 0xa
        { false, false, AlphaFactorOne },   // 0xb
        { false, false, AlphaFactorOne },   // 0xc
        { false, false, AlphaFactorOne },   // 0xd
        { false, false, AlphaFactorOne },   // 0xe
        { false, false, AlphaFactorOne },   // 0xf
        {  true, false, AlphaFactorOne }    // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_TEXTURE_RGB   0x5
        { false, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOne },   // 0xa
        { false, false, AlphaFactorOne },   // 0xb
        { false, false, AlphaFactorOne },   // 0xc
        { false, false, AlphaFactorOne },   // 0xd
        { false, false, AlphaFactorOne },   // 0xe
        { false, false, AlphaFactorOne },   // 0xf
        {  true, false, AlphaFactorOne }    // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // 6
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_ZERO        0x0
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorZero },  // 0xa
        { false, false, AlphaFactorZero },  // 0xb
        { false, false, AlphaFactorZero },  // 0xc
        { false, false, AlphaFactorZero },  // 0xd
        { false, false, AlphaFactorZero },  // 0xe
        { false, false, AlphaFactorZero },  // 0xf
        { false, false, AlphaFactorZero }   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // 7
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_ZERO        0x0
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_LOCAL       0x1
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        { false, false, AlphaFactorZero },  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorZero },  // 0xa
        { false, false, AlphaFactorZero },  // 0xb
        { false, false, AlphaFactorZero },  // 0xc
        { false, false, AlphaFactorZero },  // 0xd
        { false, false, AlphaFactorZero },  // 0xe
        { false, false, AlphaFactorZero },  // 0xf
        { false, false, AlphaFactorZero }   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE           0x8
        { false, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOne },   // 0xa
        { false, false, AlphaFactorOne },   // 0xb
        { false, false, AlphaFactorOne },   // 0xc
        { false, false, AlphaFactorOne },   // 0xd
        { false, false, AlphaFactorOne },   // 0xe
        { false, false, AlphaFactorOne },   // 0xf
        {  true, false, AlphaFactorOne }    // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_LOCAL 0x9
        { false, false, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOneMinusLocal }, // 0xa
        { false, false, AlphaFactorOneMinusLocal }, // 0xb
        { false, false, AlphaFactorOneMinusLocal }, // 0xc
        { false, false, AlphaFactorOneMinusLocal }, // 0xd
        { false, false, AlphaFactorOneMinusLocal }, // 0xe
        { false, false, AlphaFactorOneMinusLocal }, // 0xf
        {  true, false, AlphaFactorOneMinusLocal }  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_OTHER_ALPHA 0xa
        { false, false, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true,  true, AlphaFactorOneMinusOther }, // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOneMinusOther }, // 0xa
        { false, false, AlphaFactorOneMinusOther }, // 0xb
        { false, false, AlphaFactorOneMinusOther }, // 0xc
        { false, false, AlphaFactorOneMinusOther }, // 0xd
        { false, false, AlphaFactorOneMinusOther }, // 0xe
        { false, false, AlphaFactorOneMinusOther }, // 0xf
        {  true,  true, AlphaFactorOneMinusOther }  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_LOCAL_ALPHA 0xb
        { false, false, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorOneMinusLocal }, // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOneMinusLocal }, // 0xa
        { false, false, AlphaFactorOneMinusLocal }, // 0xb
        { false, false, AlphaFactorOneMinusLocal }, // 0xc
        { false, false, AlphaFactorOneMinusLocal }, // 0xd
        { false, false, AlphaFactorOneMinusLocal }, // 0xe
        { false, false, AlphaFactorOneMinusLocal }, // 0xf
        {  true, false, AlphaFactorOneMinusLocal }  // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_TEXTURE_ALPHA 0xc
        { false, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOne },   // 0xa
        { false, false, AlphaFactorOne },   // 0xb
        { false, false, AlphaFactorOne },   // 0xc
        { false, false, AlphaFactorOne },   // 0xd
        { false, false, AlphaFactorOne },   // 0xe
        { false, false, AlphaFactorOne },   // 0xf
        {  true, false, AlphaFactorOne }    // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    },
    { // GR_COMBINE_FACTOR_ONE_MINUS_LOD_FRACTION 0xd
        { false, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_ZERO        0x0
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL       0x1
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_LOCAL_ALPHA 0x2
        { false,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER 0x3
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL 0x4
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_ADD_LOCAL_ALPHA 0x5 
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL 0x6
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL 0x7
        {  true,  true, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_OTHER_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x8
        {  true, false, AlphaFactorOne },   // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL 0x9
        { false, false, AlphaFactorOne },   // 0xa
        { false, false, AlphaFactorOne },   // 0xb
        { false, false, AlphaFactorOne },   // 0xc
        { false, false, AlphaFactorOne },   // 0xd
        { false, false, AlphaFactorOne },   // 0xe
        { false, false, AlphaFactorOne },   // 0xf
        {  true, false, AlphaFactorOne }    // GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL_ALPHA 0x10
    }
};
