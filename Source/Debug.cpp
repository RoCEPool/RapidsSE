/*============================================================================*/
/* Debug.cpp -- Debugging Functions                                           */
/*============================================================================*/

#include "Debugging.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* assert trap                                                                */
/*----------------------------------------------------------------------------*/
void Debugging::vAssertTrap(void)
{
  
  // dump the call stack
  vStackDump();
  
  // add activity dump and api state dump
  ActivityMap::vDump();
  
  gpAttachMap->m_pApiState->vDump();
  
  // put a breakpoint here to trap asserts
  sleep(0);
  
  // generate a core
  abort();
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
/* error handler                                                              */
/*----------------------------------------------------------------------------*/
void Debugging::vStackDump(void)
{
  void *pArray[C_STACK_BACKTRACE];
  int iSize;
  
  // get void*'s for all entries on the stack
  iSize = backtrace(pArray, C_STACK_BACKTRACE);
  
  // print out all the frames to stderr
  //fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(pArray, iSize, STDERR_FILENO);
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
