/*============================================================================*/
/* Magnitudes.h -- Various Useful Magnitude Values                            */
/*============================================================================*/
#pragma once

#include "Common.h"

//namespace Magic { }

using namespace Common;

class Magnitudes {
public:
  static void vMagnitude(i8 iValue, char* psBuf);
  static void vMagnitude(i8 iValue, i8 iDecimals, char* psBuf);
  static bool bBytes(const char* psBuf, i8& iBytes);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
