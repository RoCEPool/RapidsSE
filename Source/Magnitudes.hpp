/*============================================================================*/
/* Magnitudes.h -- Various Useful Magnitude Values                            */
/*============================================================================*/
#pragma once

namespace System
{
  class Magnitudes {
  public:
    static void vMagnitude(i8 iValue, Stz* psBuf);
    static void vMagnitude(i8 iValue, i8 iDecimals, Stz* psBuf);
    static bool bBytes(Stz* psBuf, i8& iBytes);
    
    static void vTimeMag(i8 iNanosecs, Stz* psBuf);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
