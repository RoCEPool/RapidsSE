/*============================================================================*/
/* Random.hpp -- random number generator                                      */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  class Random : public Static
  {
  public:
    static i8 iRandom(void);
    static i8 iRandom(i8 iMax);
    static i8 iRandom(i8 iLow, i8 iHigh);
    
    static u8 uRandom(void);
    static u8 uRandom(u8 uMax);
    static u8 uRandom(u8 uLow, u8 uHigh);
    
    static s8K sRandomString(i8 iLength);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
