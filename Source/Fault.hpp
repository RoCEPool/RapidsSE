/*============================================================================*/
/* Fault.hpp -- Fault Handler                                                 */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class Fault
  {
  public:
    // the [[ noreturn ]] attribute is the C++11 standard for indicating a
    // function never returns
    //
    // refer: https://en.cppreference.com/w/cpp/language/attributes/noreturn
  
    [[ noreturn ]] static void SwitchDefault(i8 eVal,
                                             i8 e_Enum,
                                             const char* azFunc,
                                             const char* azFilePath,
                                             i8 iLineNo);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
