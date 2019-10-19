/*============================================================================*/
/* Debug.hpp -- Debug Macro Definitions and Functions                         */
/*============================================================================*/
#pragma once

namespace Correctness
{
  static const i8 C_STACK_BACKTRACE = 10;

  void vAssertCalled(void);
  void vAssertDebug(void);
  void vAssertRelease(void);
  void vAssertPoint(void);
  void vAssertTrap(void);
  void vAssertType(void* aObject, eCN eExpectingClass);
  void vAssertWarn(void);
  void vDebugPoint(void);
  void vDebugTrap(void);
  void vStackDump(void);
  void vThrowTrap(void);
    

  // the [[ noreturn ]] attribute is the C++11 standard for indicating a
  // function never returns
  //
  // refer: https://en.cppreference.com/w/cpp/language/attributes/noreturn
  
  [[ noreturn ]] void vThrow(eFK eKind, const char* azFunc,
                             const char* azFilePath, i8 iLineNo,
                             const char* azFormat,...);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// invalid auxiliary stack

#define THROW(azDescription) \
  Correctness::vThrow(FK_FAILED,__func__,__FILE__,__LINE__,azDescription);

/*--------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached           */
/*--------------------------------------------------------------------------*/
#define NEVER_REACH \
  Correctness::vThrow(FK_REACHED,__func__,__FILE__,__LINE__,"NEVER_REACH point reached");


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
