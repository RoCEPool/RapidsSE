/*============================================================================*/
/* Correct.cpp -- Correctness functions                                       */
/*============================================================================*/

#include "Correctness.h"
#include "AM.hpp"


/*----------------------------------------------------------------------------*/
/* debug point                                                                */
/*----------------------------------------------------------------------------*/
void Correctness::vDebugPoint(void)
{
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* dump current thread's call stack                                           */
/*----------------------------------------------------------------------------*/
void Correctness::vStackDump(void)
{
  // get void*'s for all frames on the stack
  void* aaFramePtr[C_STACK_BACKTRACE];
  int nPtrs = backtrace(aaFramePtr,C_STACK_BACKTRACE);
  
  // print out all the frames to stderr
  backtrace_symbols_fd(aaFramePtr,nPtrs,STDERR_FILENO);
}

////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* abort the process                                                          */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Correctness::vAbortProcess(void)
{
  // dump the call stack
  vStackDump();
  
  // write activity log
  AM::vWriteLog();
  
  // dump api state
  AM::vDumpApi();
  
  // put a breakpoint here
  sleep(0);
  
  // generate a core
  abort();
}

/*----------------------------------------------------------------------------*/
/* throw an FK_ASSERT exception (long-form)                                   */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Correctness::vThrow(eFK eKind, const char* azFunc,
                                        const char* azFilePath, i8 iLineNo,
                                        Stz* szFormat,...)
{
  // put a breakpoint here to trap throws
  sleep(0);
  
  char zFileNm[256];
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szFormat);
  char zCondition[256];
  StrLib::vSprint(zCondition,xArgs,(char*)szFormat);
  va_end(xArgs);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Exception xException(FK_ASSERT,azFunc,zCondition,zFileNm,iLineNo);
  
  //xException.vPrint();
  
  // dump the call stack
  //vStackDump();
  
  throw xException;
}

/*----------------------------------------------------------------------------*/
/* have a fatal type error                                                    */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Correctness::vTypeFatal(eFK eKind, const char* azFunc,
                                            const char* azFilePath, i8 iLineNo,
                                            void* aObject, eCN eExpectingClass)
{
  Object* pObject = static_cast<Object*>(aObject);
  eCN eGotClass = pObject->eClass();
  
  char zCondition[256];

  // put a breakpoint here to trap throws
  sleep(0);
  
  if (eGotClass >= CN_NONE && eGotClass < CN_ENUMS)
    StrLib::vSprint(zCondition,
                    "  Expecting class %d %s; Got %d %s\n"_Stz,
                    eExpectingClass, aCN[eExpectingClass]->azChar(),
                    eGotClass,       aCN[eGotClass]->azChar());
  else
    StrLib::vSprint(zCondition,
                    "  Expecting class %d %s; Got %d\n"_Stz,
                    eExpectingClass, aCN[eExpectingClass]->azChar(),
                    eGotClass);
  
  //  if (CN_FREECHUNK == eGotClass) {
  //    FreeChunk* aFreeChunk = static_cast<FreeChunk*>(aObject);
  //    eCN eWasClass = aFreeChunk->eWasClass();
  //
  //    printf("  FreeChunk was %d\n",eWasClass);
  //  }
  
  vThrow(eKind,azFunc,azFilePath,iLineNo,(Stz*)zCondition);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
