/*============================================================================*/
/* Correct.hpp -- Correctness Functions                                       */
/*============================================================================*/
#pragma once

namespace Resources
{
  static const i8 C_STACK_BACKTRACE = 16;

  void vDebugPoint(void);
  void vThreadCallStackDump(void);


  // the [[ noreturn ]] attribute is the C++11 standard for indicating a
  // function never returns
  //
  // refer: https://en.cppreference.com/w/cpp/language/attributes/noreturn
  
  [[ noreturn ]] void vAbortProcess(void);
    
  [[ noreturn ]] void vThrow(eFK   eKind,
                             eVL   eSeverity,
                             const char* azFunc,
                             const char* azFilePath,
                             i8    iLineNo,
                             Stz*  szFormat,...);
  
  [[ noreturn ]] void vThrowUser(eFK   eKind,
                             eVL   eSeverity,
                             const char* azFunc,
                             const char* azFilePath,
                             i8    iLineNo,
                             Stz*  szFormat,...);
  
  [[ noreturn ]] void vThrowType(eFK   eKind,
                                 eVL   eSeverity,
                                 const char* azFunc,
                                 const char* azFilePath,
                                 i8    iLineNo,
                                 void* aObject,
                                 eCN   eExpectingClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
