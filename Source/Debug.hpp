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
  
  
  [[ noreturn ]] void vAssertFailed(const char* azCondition,
                                    const char* azFilePath, i8 iLineNo);
  // refer: https://en.cppreference.com/w/cpp/language/attributes/noreturn
  
  
  void vExceptionCaught(Exception xException);
  
  [[ noreturn ]] void vThrow(const char* azFilePath, i8 iLineNo,
                             Stz* szFormat,...);

  [[ noreturn ]] void vThrow(const char* azFilePath, i8 iLineNo,
                             eFK eKind, Stz* szStepNm);
  
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// invalid auxiliary stack

#define THROW(azDescription) \
{ \
  Correctness::vAssertFailed(azDescription,__FILE__,__LINE__); \
}

/*--------------------------------------------------------------------------*/
/* NEVER_REACH macro (point in code which should never be reached           */
/*--------------------------------------------------------------------------*/
#define NEVER_REACH \
  Correctness::vAssertFailed("NEVER_REACH",__FILE__,__LINE__);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
