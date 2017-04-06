/*============================================================================*/
/* Debug.cpp -- Debugging Functions                                           */
/*============================================================================*/

#include "Debugging.h"

/*----------------------------------------------------------------------------*/
/* assert trap                                                                */
/*----------------------------------------------------------------------------*/
void Debugging::vAssertTrap(void)
{
  // put a breakpoint here to trap asserts
  sleep(0);
  
  void *pArray[10];
  int iSize;
  
  // get void*'s for all entries on the stack
  iSize = backtrace(pArray, 10);
  
  // print out all the frames to stderr
  //fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(pArray, iSize, STDERR_FILENO);
}

/*----------------------------------------------------------------------------*/
/* debug trap                                                                 */
/*----------------------------------------------------------------------------*/
void Debugging::vDebugTrap(void)
{
  // put a breakpoint here to trap specific debug points
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* throw trap                                                                 */
/*----------------------------------------------------------------------------*/
void Debugging::vThrowTrap(void)
{
  // put a breakpoint here to trap throws
  sleep(0);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
