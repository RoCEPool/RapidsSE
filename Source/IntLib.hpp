/*============================================================================*/
/* IntLib.hpp -- integer utility                                              */
/*============================================================================*/

#pragma once

namespace Base
{
  const i8 C_I8_MIN = LLONG_MIN + 1;
  const i8 C_I8_MAX = LLONG_MAX;
  const i8 C_NULL_INT = LLONG_MIN;
  
  class IntLib
  {
  private:
  
  public:
    static i8 iDecile(i8 iValue, i8 iMax);
    static i8 iIntToVlq(i8 iInt, byte yVlqVec[16]);
    static i8 iPercent(i8 iNumerator, i8 iDenominator);
    static i8 iVlqToInt(byte yVlqVec[16]);
  
    static char* sAsText(i8 iValue);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
