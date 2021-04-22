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

  // First try to extract out file and line numbers and print that.
  // If this fails then we will fall back to a secondary approach.
  s256 asModules[C_STACK_BACKTRACE];
  s256 asFileNames[C_STACK_BACKTRACE];
  s2K  asFuncNames[C_STACK_BACKTRACE];
  i8   aiLines[C_STACK_BACKTRACE];
  i8   aiOffsets[C_STACK_BACKTRACE];

  if (bAddrsToLines(aaFramePtr, nPtrs, asModules, asFileNames, asFuncNames, aiLines, aiOffsets)) {
    for (i8 iFrame = 0; iFrame < nPtrs; iFrame++) {

      if (asFileNames[iFrame].nLength() > 0) {
        vTrace("Frame %2lld:  %-26s  %018p  %s  (%s:%lld)\n"_Stz,
               iFrame,
               asModules[iFrame].azChar(),
               aaFramePtr[iFrame],
               asFuncNames[iFrame].azChar(),
               asFileNames[iFrame].azChar(),
               aiLines[iFrame]);
      } else {
        // Filename and line numbers not available. Print the offset instead.
        vTrace("Frame %2lld:  %-26s  %018p  %s  (+%lld)\n"_Stz,
               iFrame,
               asModules[iFrame].azChar(),
               aaFramePtr[iFrame],
               asFuncNames[iFrame].azChar(),
               aiOffsets[iFrame]);
      }
    }
    return;
  }


  // Secondary approach - Get the manged symbols, parse them and try to demangle the functions.
  // We won't have file and line numbers, but we should be able to get offsets into the functions.
  // Worst case is we just print the mangled function names and offsets.

  // The string format of this symbol list is different on Mac vs Linux, so we need to parse them differently
  char** acSymbolList = backtrace_symbols(aaFramePtr,nPtrs);

  for (i8 i=0; i<nPtrs; i++) {

    char* p = acSymbolList[i];
    //vTrace("\nLine: %s\n"_Stz, p);

    i8    iFrame;
    s256  sModule;
    void* pAddr;
    s2K   sSymbol;
    i8    iOffset;

#ifdef WITH_APPLE
    // Line format: <frameNum> <space> <binaryName> <space> <hexAddress> <space> <symbolName> '+' <offset>
    i8 iConv = sscanf(p, "%lld %s %p %s + %lld", &iFrame, sModule.azChar(), &pAddr, sSymbol.azChar(), &iOffset);

    if (iConv == 5) {
#endif
#ifdef WITH_LINUX
    // Line format: <binaryName> <space> '(' [<symbol>] '+' <hexOffset> ')' '[' <hexAddress> ']'

    iFrame=i;
    s2K sSymbolAndOffset;

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
      s2K sDemangledSymbol;
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
      // Could extract out the parameters, so just print the whole line with mangled function names
      vTrace( "Frame %2lld:  %s\n"_Stz, i, acSymbolList[i]);
    }
  }

  free(acSymbolList);
}

/*----------------------------------------------------------------------------*/
/* Try to convert an instruction address to a line in a file                  */
/*----------------------------------------------------------------------------*/
b1 BASE::bAddrToLine(void *pvAddr, s256& sFileName, s2K& sFuncName, i8& iLine)
{
  s256 sModule;

#ifdef WITH_APPLE
  // Is atos available? Work this out once and save the result.
  static i8 iAtosAvailable = -1;  // some 3-state value

  if (iAtosAvailable == -1) {
    int iResult = system("which atos &>/dev/null");
    if (iResult != 0) {
      iAtosAvailable = 0;
      return false;
    }

    iAtosAvailable = 1;
  }

  if (iAtosAvailable == 0) {
    return false;
  }

  static pid_t iPid = getpid();

  s256 sCmd;
  sCmd.vSprint("atos -p %d %p"_Stz, iPid, pvAddr);

  FILE* pF = popen(sCmd.azChar(), "r");

  if (!pF) {
    return false;
  }

  // Read a line
  s2K sBuf;
  if (!fgets(sBuf.azChar(), sBuf.nSpace(), pF)) {
    pclose(pF);
    return false;
  }

  // We should have a line that looks like this:
  // BASE::vThreadCallStackDump_Demangled(params...) (in libRapidsSE.dylib) (Correct.cpp:57)\n

  // Because the parameters have spaces, we need to manually parse out the function name.
  // Look for "(in"
  char* p = sBuf.azChar();
  char* pEnd = p + sBuf.nSize();
  while (*p &&
         p < (pEnd - 4) &&
         !(
           *p     == '(' &&
           *(p+1) == 'i' &&
           *(p+2) == 'n' &&
           *(p+3) == ' ' )
         ) {
    ++p;
  }

  // Copy out the function name
  sFuncName.vCopy(sBuf.szStz(), p - sBuf.azChar());

  // Now we can use sscanf to get the rest of the information as they are all single tokens
  i8 iConv = sscanf(p, "(in %[^)]) (%[^:]:%lld)", sModule.azChar(), sFileName.azChar(), &iLine);
  if (iConv != 3) {
    pclose(pF);
    return false;
  }

  while (fread(sBuf.azChar(), 1, sBuf.nSpace(), pF) == sBuf.nSpace());

  pclose(pF);
  return true;
#endif

#ifdef WITH_LINUX
//  sFnName = "????????"_Stz;
//  sFuncName = "????????"_Stz;
//  iLine = -1;
  return false;
#endif
}

/*----------------------------------------------------------------------------*/
/* Try to convert an array of instruction addresses to lines in files         */
/*----------------------------------------------------------------------------*/
b1 BASE::bAddrsToLines(void** pvAddrs,
                       i8 iArrSize,
                       s256*  psModules,
                       s256*  psFileNames,
                       s2K*   psFuncNames,
                       i8*    piLines,
                       i8*    piOffsets)
{
#ifdef WITH_APPLE
  // Is atos available? Work this out once and save the result.
  static i8 iAtosAvailable = -1;  // some 3-state value

  if (iAtosAvailable == -1) {
    int iResult = system("which atos &>/dev/null");
    if (iResult != 0) {
      iAtosAvailable = 0;
      return false;
    }

    iAtosAvailable = 1;
  }

  if (iAtosAvailable == 0) {
    return false;
  }

  static pid_t iPid = getpid();

  s2K sCmd;
  sCmd.vSprint("atos -p %d"_Stz, iPid);
  for (i8 i = 0; i < iArrSize; i++) {
    sCmd.vCatSprint(" %p"_Stz, pvAddrs[i]);
  }


  FILE* pF = popen(sCmd.azChar(), "r");

  if (!pF) {
    return false;
  }

  // Read a line
  s2K sBuf;
  i8 iLinesRead = 0;
  while (iLinesRead < iArrSize && fgets(sBuf.azChar(), sBuf.nSpace(), pF)) {

    // We should have a line that looks like this:
    // BASE::vThreadCallStackDump_Demangled(params...) (in libRapidsSE.dylib) (Correct.cpp:57)\n

    // Because the parameters have spaces, we need to manually parse out the function name.
    // Look for "(in"
    char *p = sBuf.azChar();
    char *pEnd = p + sBuf.nSize();
    while (*p &&
           p < (pEnd - 4) &&
           !(
               *p == '(' &&
               *(p + 1) == 'i' &&
               *(p + 2) == 'n' &&
               *(p + 3) == ' ')
        ) {
      ++p;
    }

    if (p >= pEnd - 4) {
      // Couldn't extract out the function name
      pclose(pF);
      return false;
    }

    // Copy out the function name
    psFuncNames[iLinesRead].vCopy(sBuf.szStz(), p - sBuf.azChar());

    // Now we can use sscanf to get the rest of the information as they are all single tokens
    i8 iConv = sscanf(p, "(in %[^)]) (%[^:]:%lld)", psModules[iLinesRead].azChar(), psFileNames[iLinesRead].azChar(), &(piLines[iLinesRead]));

    if (iConv == 1) {
      // We got the module but no filename and line. Try to extract out an offset
      psFileNames[iLinesRead].vReset();
      iConv = sscanf(p, "(in %[^)]) + %lld", psModules[iLinesRead].azChar(), &(piOffsets[iLinesRead]));

      if (iConv != 2) {
        pclose(pF);
        return false;
      }

    } else if (iConv != 3) {
      pclose(pF);
      return false;
    }

    ++iLinesRead;
  }

  // Should be nothing else left to read, but clear the pipe anyway
  while (fread(sBuf.azChar(), 1, sBuf.nSpace(), pF) == sBuf.nSpace());

  pclose(pF);
  return true;
#endif

#ifdef WITH_LINUX
//  sFnName = "????????"_Stz;
//  sFuncName = "????????"_Stz;
//  iLine = -1;
  return false;
#endif
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
