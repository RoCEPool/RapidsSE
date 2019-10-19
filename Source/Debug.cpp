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
/* assert debug point                                                         */
/*----------------------------------------------------------------------------*/
void Correctness::vAssertPoint(void)
{
  sleep(0);
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
  AM::vWriteLog();
  
  AM::vDumpApi();
  
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
           eExpectingClass, aCN[eExpectingClass]->azChar(),
           eGotClass,       aCN[eGotClass]->azChar());
  else
    printf("  Expecting class %d %s; Got %d\n",
           eExpectingClass, aCN[eExpectingClass]->azChar(),
           eGotClass);

//  if (CN_FREECHUNK == eGotClass) {
//    FreeChunk* aFreeChunk = static_cast<FreeChunk*>(aObject);
//    eCN eWasClass = aFreeChunk->eWasClass();
//    
//    printf("  FreeChunk was %d\n",eWasClass);
//  }
  
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

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* handle an assertion failure                                                */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Correctness::vAssertFailed(const char* azCondition,
                                const char* azFilePath, i8 iLineNo)
{
  // put a breakpoint here to trap throws
  sleep(0);
  
  char zFileNm[256];
  
  StrLib::vSubPath(zFileNm,(char*)azFilePath);

  Exception xException(FK_ASSERT,azCondition,zFileNm,iLineNo);
  
  printf("Exception : %s of (%s) in job %s failed (file %s line %lld)\n",
         aFK[xException.eKind()]->azChar(),
         xException.azText(),
         xException.pJob()
         ? aCN[((Object*)xException.pJob())->eClass()]->azChar()
         : "<null>",
         xException.azFileNm(),
         xException.iLineNo());
  
  // dump the call stack
  vStackDump();
  

  
  throw xException;
}

/*----------------------------------------------------------------------------*/
/* handle an assertion failure                                                */
/*----------------------------------------------------------------------------*/
void Correctness::vExceptionCaught(Exception xException)
{
  // put a breakpoint here to trap throws
  sleep(0);
  
  xException.vPrint();
//  printf("Exception : %s of (%s) in job %s caught (file %s line %lld)\n",
//         aFK[xException.eKind()]->azChar(),
//         xException.azText(),
//         xException.pJob()
//           ? aCN[((Object*)xException.pJob())->eClass()]->azChar()
//           : "<null>",
//         xException.azFileNm(),
//         xException.iLineNo());
  
  // dump the call stack
  vStackDump();

  Correctness::vAssertTrap();
}



/*----------------------------------------------------------------------------*/
/* throw an exception of given kind (step-form)                               */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Correctness::vThrow(eFK eKind, Stz* szFormat,...)
{
  // put a breakpoint here to trap throws
  sleep(0);
  
  va_list xArgs;
  va_start(xArgs,szFormat);
  char zCondition[256];
  StrLib::vSprint(zCondition,xArgs,szFormat->azChar());
  va_end(xArgs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Exception xException(eKind,zCondition);
  
  xException.vPrint();
  
  // dump the call stack
  vStackDump();
  
  throw xException;
}

/*----------------------------------------------------------------------------*/
/* throw an FK_ASSERT exception (long-form)                                   */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Correctness::vThrow(const char* azFilePath, i8 iLineNo,
                                        Stz* szFormat,...)
{
  // put a breakpoint here to trap throws
  sleep(0);
  
  char zFileNm[256];
  StrLib::vSubPath(zFileNm,(char*)azFilePath);

  va_list xArgs;
  va_start(xArgs,szFormat);
  char zCondition[256];
  StrLib::vSprint(zCondition,xArgs,szFormat->azChar());
  va_end(xArgs);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Exception xException(FK_ASSERT,zCondition,zFileNm,iLineNo);
  
  xException.vPrint();
  
  // dump the call stack
  vStackDump();
  
  throw xException;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
