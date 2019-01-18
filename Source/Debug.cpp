/*============================================================================*/
/* Debug.cpp -- Debugging Functions                                           */
/*============================================================================*/

#include "Debugging.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* assert for debug                                                           */
/*----------------------------------------------------------------------------*/
void Debugging::vAssertDebug(void)
{
  vAssertTrap();
}

/*----------------------------------------------------------------------------*/
/* assert for release                                                         */
/*----------------------------------------------------------------------------*/
void Debugging::vAssertRelease(void)
{
  // dump the call stack
  vStackDump();
  
  // put a breakpoint here to trap asserts
  sleep(0);
}

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
/* handle an rassert                                                          */
/*----------------------------------------------------------------------------*/
void Debugging::vAssertType(void* aObject, eCN eExpectingClass)
{
  Object* pObject = static_cast<Object*>(aObject);
  eCN eGotClass = pObject->eClass();
  
  if (eGotClass >= CN_NONE && eGotClass < CN_ENUMS)
    printf("  Expecting class %d %s; Got %d %s\n",
           eExpectingClass, aCN[eExpectingClass]->aChar(),
           eGotClass,       aCN[eGotClass]->aChar());
  else
    printf("  Expecting class %d %s; Got %d\n",
           eExpectingClass, aCN[eExpectingClass]->aChar(),
           eGotClass);

  if (CN_FREECHUNK == eGotClass) {
    FreeChunk* aFreeChunk = static_cast<FreeChunk*>(aObject);
    eCN eWasClass = aFreeChunk->eWasClass();
    
    printf("  FreeChunk was %d\n",eWasClass);
  }
  
  vAssertTrap();
}

/*----------------------------------------------------------------------------*/
/* assert warning                                                             */
/*----------------------------------------------------------------------------*/
void Debugging::vAssertWarn(void)
{
  // dump the call stack
  vStackDump();
  
  // put a breakpoint here to trap asserts
  sleep(0);
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
