/*============================================================================*/
/* Correct.hpp -- Correctness Functions                                       */
/*============================================================================*/
#pragma once

namespace Correctness
{
  static const i8 C_STACK_BACKTRACE = 16;

  void vDebugPoint(void);
  void vStackDump(void);


  // the [[ noreturn ]] attribute is the C++11 standard for indicating a
  // function never returns
  //
  // refer: https://en.cppreference.com/w/cpp/language/attributes/noreturn
  
  [[ noreturn ]] void vAbortProcess(void);
  
  [[ noreturn ]] void vThrow(eFK eKind, const char* azFunc,
                             const char* azFilePath, i8 iLineNo,
                             const char* azFormat,...);
  
  [[ noreturn ]] void vTypeFatal(void* aObject, eCN eExpectingClass);
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
