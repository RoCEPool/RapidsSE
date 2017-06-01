/*============================================================================*/
/* IntLib.hpp -- integer utility                                              */
/*============================================================================*/

#pragma once

namespace Base
{
  const i8 C_NULL_INT = LLONG_MIN;
  
  class IntLib
  {
  private:
  
  public:
    static i8 iIntToVlq(i8 iInt, byte* aVlq);
    static i8 iIntToVlq2(i8 iInt, byte yVlqVec[16]);
    static i8 iVlqToInt(byte* aVlq);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
