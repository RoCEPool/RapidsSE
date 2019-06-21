/*============================================================================*/
/* Null.hpp -- SQL Null ForthValue                                                 */
/*============================================================================*/
#pragma once

namespace Base
{
  class Null
  {
  public:
    static i8   nCopy(byte*  ayDst);
    static i8   nCopy(byte** aayDst);
    static void vCopy(byte*  ayDst);
    static void vCopy(byte** aayDst);
  
    static bool bIsNull(i8 iValue);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
