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
  
  void vExceptionCaught(Exception xException);
  

  // refer: https://en.cppreference.com/w/cpp/language/attributes/noreturn
  
  [[ noreturn ]] void vAssert(eFK eKind, const char* azFunc,
                              const char* azFilePath, i8 iLineNo,
                              const char* azCondition);
  
  [[ noreturn ]] void vThrow(eFK eKind, const char* azFunc,
                             const char* azFilePath, i8 iLineNo);
  
  [[ noreturn ]] void vThrow(eFK eKind, const char* azFunc,
                             const char* azFilePath, i8 iLineNo,
                             Stz* szFormat,...);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// invalid auxiliary stack

#define THROW(azDescription) \
  Correctness::vAssert(FK_FAILED,__func__,__FILE__,__LINE__,azDescription);

/*--------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached           */
/*--------------------------------------------------------------------------*/
#define NEVER_REACH \
  Correctness::vAssert(FK_ASSERT,__func__,__FILE__,__LINE__,"NEVER_REACH");


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
