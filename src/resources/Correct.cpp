/*============================================================================*/
/* Correct.cpp -- Correctness functions                                       */
/*============================================================================*/

#include "Correctness.h"

/*----------------------------------------------------------------------------*/
/* debug point                                                                */
/*----------------------------------------------------------------------------*/
void Resources::vDebugPoint(void)
{
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* dump current thread's call stack                                           */
/*----------------------------------------------------------------------------*/
void Resources::vThreadCallStackDump(void)
{
  // get void*'s for all frames on the stack
  void* aaFramePtr[C_STACK_BACKTRACE];
  int nPtrs = backtrace(aaFramePtr,C_STACK_BACKTRACE);
  
  vTrace("\n"_Stz);
  vTrace("Call Stack Backtrace\n"_Stz);
  vTrace("--------------------\n"_Stz);

  // print out all the frames to stderr
  backtrace_symbols_fd(aaFramePtr,nPtrs,STDERR_FILENO);
}

////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* abort the process                                                          */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Resources::vAbortProcess(void)
{
  // dump the call stack
  vThreadCallStackDump();
  
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
/* throw anexception                                                          */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Resources::vThrow(eFK   eKind,
                                        eVL   eSeverity,
                                        const char* azFunc,
                                        const char* azFilePath,
                                        i8    iLineNo,
                                        Stz*  szFormat,...)
{
  fprintf(stderr,
          "Resources::vThrow:\n"
          "eKind = %lld\n"
          "eSeverity = %lld\n"
          "azFunc = %s\n"
          "azFilePath = %s\n"
          "iLineNo = %lld\n"
          "szFormat = %s\n",
          (i8)eKind,
          (i8)eSeverity,
          azFunc,
          azFilePath,
          (i8)iLineNo,
          szFormat->azChar()
          );
  
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
  
  Exception xException(eKind,eSeverity,azFunc,zCondition,zFileNm,iLineNo);
  
  //xException.vPrint();
  
  // dump the call stack
  
  if (AM::bDeveloper()) {
    xException.vPrint();
    AM::vTaskDump();
    vThreadCallStackDump();
  }
  
  throw xException;
}

/*----------------------------------------------------------------------------*/
/* throw a user exception                                                     */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Resources::vThrowUser(eFK   eKind,
                                        eVL   eSeverity,
                                        const char* azFunc,
                                        const char* azFilePath,
                                        i8    iLineNo,
                                        Stz*  szFormat,...)
{
  if (AM::bDeveloper()) {
    fprintf(stderr,
            "Resources::vThrow:\n"
            "eKind = %lld\n"
            "eSeverity = %lld\n"
            "azFunc = %s\n"
            "azFilePath = %s\n"
            "iLineNo = %lld\n"
            "szFormat = %s\n",
            (i8)eKind,
            (i8)eSeverity,
            azFunc,
            azFilePath,
            (i8)iLineNo,
            szFormat->azChar()
            );
    
    // put a breakpoint here to trap throws
    sleep(0);
  }
  
  char zFileNm[256];
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szFormat);
  char zCondition[256];
  StrLib::vSprint(zCondition,xArgs,(char*)szFormat);
  va_end(xArgs);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Exception xException(eKind,eSeverity,azFunc,zCondition,zFileNm,iLineNo);
  
  throw xException;
}

/*----------------------------------------------------------------------------*/
/* have a type error                                                          */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Resources::vThrowType(eFK   eKind,
                                            eVL   eSeverity,
                                            const char* azFunc,
                                            const char* azFilePath,
                                            i8    iLineNo,
                                            void* aObject,
                                            eCN   eExpectingClass)
{
  Object* pObject = static_cast<Object*>(aObject);
  eCN eGotClass = pObject->eClass();
  
  char zCondition[256];

  // put a breakpoint here to trap throws
  sleep(0);
  
  if (eGotClass >= CN_NONE && eGotClass < CN_ENUMS)
    StrLib::vSprint(zCondition,
                    "Expecting class %d %s; Got %d %s\n"_Stz,
                    eExpectingClass, aCN[eExpectingClass]->azChar(),
                    eGotClass,       aCN[eGotClass]->azChar());
  else
    StrLib::vSprint(zCondition,
                    "Expecting class %d %s; Got %d\n"_Stz,
                    eExpectingClass, aCN[eExpectingClass]->azChar(),
                    eGotClass);
  
  //  if (CN_FREECHUNK == eGotClass) {
  //    FreeChunk* aFreeChunk = static_cast<FreeChunk*>(aObject);
  //    eCN eWasClass = aFreeChunk->eWasClass();
  //
  //    printf("  FreeChunk was %d\n",eWasClass);
  //  }
  
  if (AM::bDeveloper()) {
    AM::vTaskDump();
    vThreadCallStackDump();
  }

  vThrow(eKind,eSeverity,azFunc,azFilePath,iLineNo,(Stz*)zCondition);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
