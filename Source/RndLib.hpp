/*============================================================================*/
/* RndLib.hpp -- random number/string generator                               */
/*============================================================================*/
#pragma once

namespace Library
{
  class RndLib
  {
  public:
    static i8 iInteger(void);
    static i8 iInteger(i8 iMax);
    static i8 iInteger(i8 iLow, i8 iHigh);
    
    static u8 uInteger(void);
    static u8 uInteger(u8 uMax);
    static u8 uInteger(u8 uLow, u8 uHigh);
    
    static i8 iLogLevel(i8 nLvlMaximum, i8 iLvlRatio, i8& i_NextRnd);
    static i8 iSkipLevel(i8 nLvlMaximum, i8 iLvlRatio);
    static i8 iSkipLevel(i8 nLvlMaximum, i8 iLvlRatio, i8& i_NextRnd);

    static void vString(s256& sStr, i8 iLength);

    static u8 uRepeatable(i8& i_Next);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
