/*============================================================================*/
/* Random.hpp -- random number/string generator                               */
/*============================================================================*/
#pragma once

namespace System
{
  class Random
  {
  public:
    static i8 iRandom(void);
    static i8 iRandom(i8 iMax);
    static i8 iRandom(i8 iLow, i8 iHigh);
    
    static u8 uRandom(void);
    static u8 uRandom(u8 uMax);
    static u8 uRandom(u8 uLow, u8 uHigh);
    
    static void vRandomString(StrVal& sStr, i8 iLength);
  
    static i8 iLogLevel(i8 nLevels, i8 iFrequency);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
