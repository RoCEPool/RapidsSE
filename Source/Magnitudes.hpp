/*============================================================================*/
/* Magnitudes.h -- Various Useful Magnitude Values                            */
/*============================================================================*/
#pragma once

namespace System
{
  class Magnitudes {
  public:
    static void vMagnitude(i8 iValue, Str* psBuf);
    static void vMagnitude(i8 iValue, i8 iDecimals, Str* psBuf);
    static bool bBytes(Str* psBuf, i8& iBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
