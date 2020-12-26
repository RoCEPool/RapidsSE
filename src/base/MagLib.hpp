/*============================================================================*/
/* MagLib.hpp -- Magnitude Conversions                                        */
/*============================================================================*/
#pragma once

namespace Resources
{
  class MagLib {
  public:
    static bool bToBytes(Stz* psBuf, i8& iBytes);
    
    static void vByteMag(i8 iValue, Stz* psBuf);
    static void vByteMag(i8 iValue, i8 nDecimals, Stz* psBuf);
    static void vHzMag(i8 iValue, Stz* psBuf);
    static void vTimeMag(i8 iValue, Stz* psBuf);
    static void vUnitMag(i8 iValue, Stz* psBuf);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
