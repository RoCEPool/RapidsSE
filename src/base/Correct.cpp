/*============================================================================*/
/* Correct.cpp -- Correctness functions                                       */
/*============================================================================*/

#include "BASE.h"

#include <execinfo.h> // backtrace etc
#include <cxxabi.h>

#ifdef WITH_LINUX
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
#endif

/*----------------------------------------------------------------------------*/
/* debug point                                                                */
/*----------------------------------------------------------------------------*/
void BASE::vDebugPoint(void)
{
  sleep(0);
}

/*----------------------------------------------------------------------------*/
/* dump current thread's call stack                                           */
/*----------------------------------------------------------------------------*/
void BASE::vThreadCallStackDump(void)
{
//  vThreadCallStackDump_Mangled();
  vThreadCallStackDump_Demangled();
}

/*----------------------------------------------------------------------------*/
/* dump current thread's call stack. Preserve the mangling                    */
/*----------------------------------------------------------------------------*/
void BASE::vThreadCallStackDump_Mangled(void)
{
  // get void*'s for all frames on the stack
  void* aaFramePtr[C_STACK_BACKTRACE];
  int nPtrs = backtrace(aaFramePtr,C_STACK_BACKTRACE);

  vTrace("\n"_Stz);
  vTrace("Call TypedStack Backtrace\n"_Stz);
  vTrace("--------------------\n"_Stz);

  // print out all the frames to stderr
  backtrace_symbols_fd(aaFramePtr,nPtrs,STDERR_FILENO);
}

/*----------------------------------------------------------------------------*/
/* dump current thread's call stack and demangle it                           */
/*----------------------------------------------------------------------------*/
void BASE::vThreadCallStackDump_Demangled(void)
{
  // get void*'s for all frames on the stack
  void* aaFramePtr[C_STACK_BACKTRACE];
  int nPtrs = backtrace(aaFramePtr,C_STACK_BACKTRACE);

  vTrace("\n"_Stz);
  vTrace("Call TypedStack Backtrace\n"_Stz);
  vTrace("--------------------\n"_Stz);

  char** acSymbolList = backtrace_symbols(aaFramePtr,nPtrs);

  for (i8 i=0; i<nPtrs; i++) {

    char* p = acSymbolList[i];
    //vTrace("\nLine: %s\n"_Stz, p);

    i8    iFrame;
    s256  sModule;
    void* pAddr;
    s256  sSymbol;
    i8    iOffset;

#ifdef WITH_APPLE
    i8 iConv = sscanf(p, "%lld %s %p %s + %lld", &iFrame, sModule.azChar(), &pAddr, sSymbol.azChar(), &iOffset);

    if (iConv == 5) {
#endif
#ifdef WITH_LINUX
    iFrame=i;
    s256 sSymbolAndOffset;

    // Sometimes there is no symbol printed, so we need to handle that separately.
    i8 iConv = sscanf(p, "%[^(](%[^)]) [%p]", sModule.azChar(), sSymbolAndOffset.azChar(), &pAddr);

    if (iConv == 3) {

      iConv = sscanf(sSymbolAndOffset.azChar(), "%[^+]+%llx", sSymbol.azChar(), &iOffset);
      if (iConv != 2) {

        sSymbol = "????????"_Stz;
        // try to extract out the offset only
        iConv = sscanf(sSymbolAndOffset.azChar(), "+%llx", &iOffset);

        if (iConv != 1) {
          vTrace("Frame %2lld:  %s\n"_Stz, i, acSymbolList[i]);
          continue;
        }
      }

#endif

      int status = 0;
      s256 sDemangledSymbol;
      size_t iSymbolSize = sDemangledSymbol.nSpace();
      char* ret = abi::__cxa_demangle(sSymbol.azChar(), sDemangledSymbol.azChar(), &iSymbolSize, &status);

      if (status == 0) {
        sDemangledSymbol.vCopy((Stz*)ret); // use possibly realloc()-ed string
        vTrace("Frame %2lld:  %-26s  %018p  %s  (+%lld)\n"_Stz, iFrame, sModule.azChar(), pAddr, sDemangledSymbol.azChar(), iOffset);
      }
      else {
        // Demangling failed.
        vTrace("Frame %2lld:  %-26s  %018p  %s  (+%lld)\n"_Stz, iFrame, sModule.azChar(), pAddr, sSymbol.azChar(), iOffset);
      }

    } else {
      vTrace( "Frame %2lld:  %s\n"_Stz, i, acSymbolList[i]);
    }
  }

  free(acSymbolList);
}

////////////////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* abort the process                                                          */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vAbortProcess(void)
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
[[ noreturn ]] void BASE::vThrow(eFK   eKind,
                                        eVL   eSeverity,
                                        const char* azFunc,
                                        const char* azFilePath,
                                        i8    iLineNo,
                                        Stz*  szFormat,...)
{
  fprintf(stderr,
          "BASE::vThrow:\n"
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
  
  if (AM::bDeveloper() || AM::bDebugMode()) {
    xException.vPrint();
    AM::vTaskDump();
    vThreadCallStackDump();

    vDebugPoint();
  }
  
  throw xException;
}

/*----------------------------------------------------------------------------*/
/* throw a user exception                                                     */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowUser(eFK   eKind,
                                        eVL   eSeverity,
                                        const char* azFunc,
                                        const char* azFilePath,
                                        i8    iLineNo,
                                        Stz*  szFormat,...)
{
  if (AM::bDeveloper()) {
    fprintf(stderr,
            "BASE::vThrow:\n"
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
[[ noreturn ]] void BASE::vThrowType(eFK   eKind,
                                            eVL   eSeverity,
                                            const char* azFunc,
                                            const char* azFilePath,
                                            i8    iLineNo,
                                            void* aObject,
                                            eCN   eExpectingClass)
{
  Object* pObject = static_cast<Object*>(aObject);
  eCN eGotClass = pObject->eGetClassNo();
  
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

/*----------------------------------------------------------------------------*/
/* have a type error                                                          */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowType2(eFK   eKind,
                                            eVL   eSeverity,
                                            const char* azFunc,
                                            const char* azFilePath,
                                            i8    iLineNo,
                                            void* aObject,
                                            eCN   eExpectingClass,
                                            eCN   eGotClass)
{
  Object* pObject = static_cast<Object*>(aObject);
  UNUSED(pObject);
  //eCN eGotClass = pObject->eGetClassNo();
  
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
