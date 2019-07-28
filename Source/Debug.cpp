/*============================================================================*/
/* Debug.cpp -- Debug functions                                               */
/*============================================================================*/

#include "Correctness.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/* assert for been called                                                     */
/*----------------------------------------------------------------------------*/
void Correctness::vAssertCalled(void)
{
  vAssertTrap();
}

/*----------------------------------------------------------------------------*/
/* assert for debug                                                           */
/*----------------------------------------------------------------------------*/
void Correctness::vAssertDebug(void)
{
  vAssertTrap();
}

/*----------------------------------------------------------------------------*/
/* assert for release                                                         */
/*----------------------------------------------------------------------------*/
void Correctness::vAssertRelease(void)
{
  // dump the call stack
  vStackDump();
  
  // put a breakpoint here to trap asserts
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* assert trap                                                                */
/*----------------------------------------------------------------------------*/
void Correctness::vAssertTrap(void)
{
  // dump the call stack
  vStackDump();
  
  // add activity dump and api state dump
  StorageEngine::AM::vWriteLog();
  
  StorageEngine::AM::vDumpApi();
  
  // put a breakpoint here to trap asserts
  sleep(0);
  
  // generate a core
  abort();
}

/*----------------------------------------------------------------------------*/
/* handle an rassert                                                          */
/*----------------------------------------------------------------------------*/
void Correctness::vAssertType(void* aObject, eCN eExpectingClass)
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
void Correctness::vAssertWarn(void)
{
  // dump the call stack
  vStackDump();
  
  // put a breakpoint here to trap asserts
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* debug point                                                                */
/*----------------------------------------------------------------------------*/
void Correctness::vDebugPoint(void)
{
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* debug trap                                                                 */
/*----------------------------------------------------------------------------*/
void Correctness::vDebugTrap(void)
{
  // put a breakpoint here to trap specific debug points
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* error handler                                                              */
/*----------------------------------------------------------------------------*/
void Correctness::vStackDump(void)
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
void Correctness::vThrowTrap(void)
{
  // put a breakpoint here to trap throws
  sleep(0);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
