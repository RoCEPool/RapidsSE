/*============================================================================*/
/* Magnitudes.h -- Various Useful Magnitude Values                            */
/*============================================================================*/
#pragma once

namespace Base
{
  class Magnitudes {
  public:
    static bool bBytes(Stz* psBuf, i8& iBytes);
    static void vHz(i8 iValue, Stz* psBuf);
    static void vMagnitude(i8 iValue, Stz* psBuf);
    static void vMagnitude(i8 iValue, i8 iDecimals, Stz* psBuf);
    static void vTimeMag(i8 iNanosecs, Stz* psBuf);
    static void vUnitMag(i8 iValue, Stz* psBuf);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
