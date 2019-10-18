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
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
