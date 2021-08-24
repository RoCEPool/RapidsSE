/*============================================================================*/
/* Correct.cpp -- Correctness functions                                       */
/*============================================================================*/

#include "BASE.h"

#include <execinfo.h> // backtrace etc
#include <cxxabi.h>

#ifdef WITH_LINUX
  #include <unistd.h>
  #include <dlfcn.h>
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
  vTrace("Thread Backtrace\n"_Stz);
  vTrace("----------------\n"_Stz);

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
  vTrace("Thread Backtrace:\n"_Stz);

  // First try to extract out file and line numbers and print that.
  // If this fails then we will fall back to a secondary approach.
  s256 asModules[C_STACK_BACKTRACE];
  s256 asFileNames[C_STACK_BACKTRACE];
  s2K  asFuncNames[C_STACK_BACKTRACE];
  i8   aiLines[C_STACK_BACKTRACE];
  i8   aiOffsets[C_STACK_BACKTRACE];

  b1 bGotLines = bAddrsToLines(aaFramePtr, nPtrs, asModules, asFileNames, asFuncNames, aiLines, aiOffsets);

  if (bGotLines) {
    for (i8 iFrame = 0; iFrame < nPtrs; iFrame++) {

      if (asFileNames[iFrame].nLength() > 0) {
        vTrace("  [%02lld]  %-26s  %018p  %s  (%s:%lld)\n"_Stz,
               iFrame,
               asModules[iFrame].azChar(),
               aaFramePtr[iFrame],
               asFuncNames[iFrame].azChar(),
               asFileNames[iFrame].azChar(),
               aiLines[iFrame]);
      } else {
        // Filename and line numbers not available. Print the offset instead.
        vTrace("  [%02lld]  %-26s  %018p  %s  (+%lld)\n"_Stz,
               iFrame,
               asModules[iFrame].azChar(),
               aaFramePtr[iFrame],
               asFuncNames[iFrame].azChar(),
               aiOffsets[iFrame]);
      }
    }
    vTrace("End Thread Backtrace\n"_Stz);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Secondary approach - Get the manged symbols, parse them and try to demangle the functions.
  // We won't have file and line numbers, but we should be able to get offsets into the functions.
  // Worst case is we just print the mangled function names and offsets.

  // The string format of this symbol list is different on Mac vs Linux, so we need to parse them differently
  char** acSymbolList = backtrace_symbols(aaFramePtr,nPtrs);

  for (i8 i=0; i<nPtrs; i++) {

    s256  sModule;
    void* pvAddr = aaFramePtr[i];
    s2K   sSymbol;
    i8    iOffset;

    b1 bParsed = bParseBacktraceSymbol(acSymbolList[i], sModule, sSymbol, iOffset);

    if (bParsed || (sModule.nLength() > 0 && sSymbol.nLength() > 0 && iOffset > 0)) {
      vTrace("  [%02lld]  %-26s  %018p  %s  (+%lld)\n"_Stz, i, sModule.azChar(), pvAddr, sSymbol.azChar(), iOffset);

    } else {
      vTrace("  [%02lld]  %s\n"_Stz, i, acSymbolList[i]);
    }
  }

  vTrace("End Thread Backtrace\n"_Stz);

  free(acSymbolList);
}

/*----------------------------------------------------------------------------*/
/* Try to convert an array of instruction addresses to lines in files         */
/*----------------------------------------------------------------------------*/
b1 BASE::bAddrsToLines(void** pvAddrs,
                       i8     iArrSize,
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
  i8 iFrame = 0;
  while (iFrame < iArrSize && fgets(sBuf.azChar(), sBuf.nSpace(), pF)) {

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
    psFuncNames[iFrame].vCopy(sBuf.szStz(), p - sBuf.azChar());

    // Now we can use sscanf to get the rest of the information as they are all single tokens
    i8 iConv = sscanf(p, "(in %[^)]) (%[^:]:%lld)", psModules[iFrame].azChar(), psFileNames[iFrame].azChar(), &(piLines[iFrame]));

    if (iConv == 1) {
      // We got the module but no filename and line. Try to extract out an offset only
      psFileNames[iFrame].vReset();
      iConv = sscanf(p, "(in %[^)]) + %lld", psModules[iFrame].azChar(), &(piOffsets[iFrame]));

      if (iConv != 2) {
        // Couldn't get filename and offset. Make do without.
        piOffsets[iFrame] = 0;
        continue;
      }

    } else if (iConv != 3) {
      pclose(pF);
      return false;
    }

    ++iFrame;
  }

  // Should be nothing else left to read, but clear the pipe anyway
  while (fread(sBuf.azChar(), 1, sBuf.nSpace(), pF) == (size_t) sBuf.nSpace());

  pclose(pF);
  return true;
#endif

#ifdef WITH_LINUX

  // The Linux approach is a little different from Mac. We use
  // the utility addr2line, but in order to look up some instruction
  // pointer we need to tell addr2line not only which binary (exe
  // or .so) to look it up in, but we also need to convert the
  // instruction pointer to a relative address that is relative to
  // the base address in memory where the .so was loaded into the
  // process. Due to address space layout randomisation this is
  // different each time we run a process.
  //
  // So we will first use dladdr() to tell us which .so a given
  // instruction pointer comes from and its base address. Then we
  // can invoke addr2line and parse its output.

  // Is addr2line available?
  if (system("which addr2line &>/dev/null") != 0) {
    vTrace("addr2line not available. Cannot calculate backtrace file names and line numbers.\n"_Stz);
    return false;
  }

  // These backtrace symbols may be useful to fill in missing bits of information
  char** acSymbolList = backtrace_symbols(pvAddrs,iArrSize);

  for (i8 i=0; i<iArrSize; i++) {

    // Get information about the library that corresponds to this instruction pointer address.
    Dl_info xDlInfo;
    int iResult = dladdr(pvAddrs[i], &xDlInfo);

    if (iResult == 0) {
      vTrace("Frame $lld could not get dladdr()\n"_Stz, i);
      free(acSymbolList);
      return false;
    }

    psModules[i].vCopy((Stz*)xDlInfo.dli_fname);

    if (!xDlInfo.dli_sname || !xDlInfo.dli_saddr) {
//      vTrace("Frame %lld address %p was matched to shared object but not to a symbol: %s\n"_Stz, i, xDlInfo.dli_fname);
      piOffsets[i] = (i8)pvAddrs[i] - (i8)xDlInfo.dli_fbase;  // This is offset from the base library
      psFuncNames[i].vCopy("????????"_Stz);

      // Try to extract out the function name and offset from the backtrace_symbols()
      s256 sModule;
      s2K sFuncName;
      i8 iOffset;

      bParseBacktraceSymbol(acSymbolList[i], sModule, sFuncName, iOffset);

      if (sFuncName.nLength() > 0) {
        psFuncNames[i].vCopy(sFuncName);
      }

      if (iOffset > 0) {
        piOffsets[i] = iOffset;
      }

      continue;
    }

    psFuncNames[i].vCopy((Stz*)xDlInfo.dli_sname);

    // We can calculate the address of the instruction pointer relative
    // to where this .so was loaded into memory. Then we can use that
    // with addr2line to find debugging information.
    void* pvOffsetAddr = (void*)( (u8)pvAddrs[i] - (u8)xDlInfo.dli_fbase );

    // Invoke addr2line for this relative address. It will print out two
    // lines. The first is the name of the function (demangled) and the
    // second is the name of the file:line (without full paths).
    // We just need to parse this information out.

    // The path of the binary may be relative, and this can be different
    // depending on whether we are running from the debugger or from
    // the installer. So if we can't find the relative path then try
    // prepending "../" before it and look again.
    s256 sFilePath;
    if (File::bExists((Stz*) xDlInfo.dli_fname, false)) {
      sFilePath.vCopy((Stz*) xDlInfo.dli_fname);

    } else {
      // Cannot find the binary. If it is a relative file, let's try prepending "../"
      if (xDlInfo.dli_fname[0] == 0 || xDlInfo.dli_fname[0] == '/') {
        vTrace("Cannot find file to convert address to line numbers: %s\n"_Stz, xDlInfo.dli_fname);
        free(acSymbolList);
        return false;
      }

      sFilePath = "../"_Stz;
      sFilePath.vConcat((Stz*) xDlInfo.dli_fname);

      if (!File::bExists(sFilePath.szStz(), false)) {
        vTrace("Cannot find file to convert address to line numbers: %s\n"_Stz, sFilePath.szStz());
        free(acSymbolList);
        return false;
      }
    }

    ASSERT(sFilePath.nLength() > 0);

    s2K sCmd;
    sCmd.vSprint("addr2line -e %s --functions --demangle --basename %p"_Stz, sFilePath.szStz(), pvOffsetAddr);
    //vTrace("Cmd: %s\n"_Stz, sCmd.azChar());

    FILE* pF = popen(sCmd.azChar(), "r");

    if (!pF) {
      free(acSymbolList);
      return false;
    }

    s2K sBuf;

    // Get the first line, the function name.
    if (!fgets(sBuf.azChar(), sBuf.nSpace(), pF)) {
      free(acSymbolList);
      pclose(pF);
      return false;
    }

    // Remove the trailing newline and save the function name
    //vTrace("Line: %s"_Stz, sBuf.azChar());
    StrLib::vStrip(sBuf.azChar(), '\n');
    psFuncNames[i].vCopy(sBuf);

    // Get the second line - the file name and line number
    if (!fgets(sBuf.azChar(), sBuf.nSpace(), pF)) {
      free(acSymbolList);
      pclose(pF);
      return false;
    }

    //vTrace("Line: %s"_Stz, sBuf.azChar());

    // Extract out the file and line number
    i8 iConv = sscanf(sBuf.azChar(), "%[^:]:%lld", psFileNames[i].azChar(), &(piLines[i]));

    if (iConv != 2) {
      // File and line number not available
      psFileNames[i].vReset();
      piOffsets[i] = (i8)pvAddrs[i] - (i8)xDlInfo.dli_saddr;  // Offset from symbol
      continue;
    }

    pclose(pF);
  }

  free(acSymbolList);
  return true;
#endif
}

/*----------------------------------------------------------------------------*/
/* Parses one line from the output of backtrace_symbols() and extracts out    */
/* the various fields. Returns true if all fields were extracted or false if  */
/* one or more failed.                                                        */
/*----------------------------------------------------------------------------*/
b1 BASE::bParseBacktraceSymbol(const char* pBtLine, s256& sModule, s2K& sFuncName, i8& iOffset)
{
  sModule.vReset();
  sFuncName.vReset();
  iOffset = -1;
  void* pvAddr;

  // The output of backtrace_symbols() is different on Linux compared to Mac.
#ifdef WITH_APPLE
  // Line format: <frameNum> <space> <binaryName> <space> <hexAddress> <space> <symbolName> '+' <offset>

  b1  bAllParsed = false;
  i8 iFrame;
  i8 iConv = sscanf(pBtLine, "%lld %s %p %s + %lld", &iFrame, sModule.azChar(), &pvAddr, sFuncName.azChar(), &iOffset);

  if (iConv == 5) {
    bAllParsed = true;
  }

  if (sFuncName.nLength() > 0) {
    // Try to demangle the name
    int status = 0;
    s2K sDemangledSymbol;
    size_t iSymbolSize = sDemangledSymbol.nSpace();
    abi::__cxa_demangle(sFuncName.azChar(), sDemangledSymbol.azChar(), &iSymbolSize, &status);

    if (status == 0) {
      sFuncName.vCopy(sDemangledSymbol.szStz());
    }
  }

  return bAllParsed;
#endif

#ifdef WITH_LINUX
  // Line format: <binaryName> <space> '(' [<symbol>] '+' <hexOffset> ')' '[' <hexAddress> ']'
  // e.g.:
  // ../bin/libRapidsSE.so(_ZN4BASE30vThreadCallStackDump_DemangledEv+0x28) [0x7fce26e86f63]

  s2K sSymbolAndOffset;
  b1  bAllParsed = false;

  // Sometimes there is no symbol printed, so we need to handle that separately.
  i8 iConv = sscanf(pBtLine, "%[^(](%[^)]) [%p]", sModule.azChar(), sSymbolAndOffset.azChar(), &pvAddr);

  if (iConv == 3) {
    bAllParsed = true;
  }

  if (iConv >= 2) {
    // Extract out the function name and offset
    iConv = sscanf(sSymbolAndOffset.azChar(), "%[^+]+%llx", sFuncName.azChar(), &iOffset);

    if (iConv != 2) {
      bAllParsed = false;
      sFuncName.vCopy("????????"_Stz);

      // try to extract out the offset only
      sscanf(sSymbolAndOffset.azChar(), "+%llx", &iOffset);

    } else {
      // Try to demangle the function name
      int status = 0;
      s2K sDemangledSymbol;
      size_t iSymbolSize = sDemangledSymbol.nSpace();
      abi::__cxa_demangle(sFuncName.azChar(), sDemangledSymbol.azChar(), &iSymbolSize, &status);

      if (status == 0) {
        sFuncName.vCopy(sDemangledSymbol.szStz());
      }
    }
  }

  return bAllParsed;
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
/* a fatal assert has occurred                                                */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vFatalAssert(eFK   eFailure,
                                       const char* azFunc,
                                       const char* azFilePath,
                                       i8    iLineNo,
                                       Stz*  szCondition,
                                       ...)
{
  char zFileNm[1024];
  char zCondition[1024];
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the exception
  
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szCondition);
  StrLib::vSprint(zCondition,xArgs,(char*)szCondition);
  va_end(xArgs);
  
  Exception xException(eFailure,azFunc,zCondition,zFileNm,iLineNo);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  xException.vPrint();

  Process::vExitFailure();
}

/*----------------------------------------------------------------------------*/
/* throw an xException                                                        */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrow_toHandler(Exception xException)
{

  if (AM::SHOW::bException()) {

    fprintf(stderr,"\n");
    fprintf(stderr,
            // 0         1         2         3
            // 0123456789012345678901234567890123456789"
              "@THROW @THROW @THROW @THROW @THROW @THRO" // 40
              "W @THROW @THROW @THROW @THROW @THROW @TH" // 80
              "ROW @THROW @THROW @THROW @THROW @THROW"   // 120
              "\n"
            );
    fprintf(stderr,"\n");

    xException.vPrint();
    
    vThreadCallStackDump();
  }

  throw xException;
}

/*----------------------------------------------------------------------------*/
/* throw an xException                                                        */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrow(eFK   eFailure,
                                 const char* azFunc,
                                 const char* azFilePath,
                                 i8    iLineNo,
                                 Stz*  szCondition,
                                 ...)
{
  char zFileNm[1024];
  char zCondition[1024];

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
//  fprintf(stderr,
//          "\n\n"
//        // 0         1         2         3
//        // 0123456789012345678901234567890123456789"
//          "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" // 40
//          "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" // 80
//          "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" // 120
//          "\n"
//          "BASE::vThrow(\n"
//          "  eFailure    = %lld\n"
//          "  azFunc      = %s\n"
//          "  azFilePath  = %s\n"
//          "  iLineNo     = %lld\n"
//          "  szCondition = %s\n"
//          ")\n"
//
//          ,(i8)eFailure
//          ,azFunc
//          ,azFilePath
//          ,(i8)iLineNo
//          ,szCondition->azChar()
//          );
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the exception
  
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szCondition);
  StrLib::vSprint(zCondition,xArgs,(char*)szCondition);
  va_end(xArgs);
  
  Exception xException(eFailure,azFunc,zCondition,zFileNm,iLineNo);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the call stack
  
//  if (AM::bDeveloper() || AM::bDebugMode()) {
//    xException.vPrint();
//    vThreadCallStackDump();
//    vDebugPoint();
//  }
//
//  fprintf(stderr,
//          // 0         1         2         3
//          // 0123456789012345678901234567890123456789"
//            "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" // 40
//            "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" // 80
//            "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" // 120
//            "\n"
//          );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // throw to Task::vExceptionHandler()
  
  vThrow_toHandler(xException);
}

/*----------------------------------------------------------------------------*/
/* throw an always exception                                                  */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowAlways(eFK   eFailure,
                                       const char* azFunc,
                                       const char* azFilePath,
                                       i8    iLineNo,
                                       Stz*  szCondition,
                                       ...)
{
  char zFileNm[1024];
  char zCondition[1024];
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the exception
  
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szCondition);
  StrLib::vSprint(zCondition,xArgs,(char*)szCondition);
  va_end(xArgs);
  
  Exception xException(eFailure,azFunc,zCondition,zFileNm,iLineNo);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // throw to Task::vExceptionHandler()
  
  vThrow_toHandler(xException);
}

/*----------------------------------------------------------------------------*/
/* throw an assert exception                                                  */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowAssert(eFK   eFailure,
                                       const char* azFunc,
                                       const char* azFilePath,
                                       i8    iLineNo,
                                       Stz*  szCondition,
                                       ...)
{
  char zFileNm[1024];
  char zCondition[1024];
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the exception
  
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szCondition);
  StrLib::vSprint(zCondition,xArgs,(char*)szCondition);
  va_end(xArgs);
  
  Exception xException(eFailure,azFunc,zCondition,zFileNm,iLineNo);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // throw to Task::vExceptionHandler()
  
  vThrow_toHandler(xException);
}

/*----------------------------------------------------------------------------*/
/* throw a user exception                                                     */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowLowMemory(eFK   eFailure,
                                          const char* azFunc,
                                          const char* azFilePath,
                                          i8    iLineNo,
                                          Stz*  szFormat,...)
{
//  if (AM::bDeveloper()) {
//    fprintf(stderr,
//            "BASE::vThrow:\n"
//            "eFailure = %lld\n"
//            "azFunc = %s\n"
//            "azFilePath = %s\n"
//            "iLineNo = %lld\n"
//            "szFormat = %s\n",
//            (i8)eFailure,
//            azFunc,
//            azFilePath,
//            (i8)iLineNo,
//            szFormat->azChar()
//    );
//
//    // put a breakpoint here to trap throws
//    sleep(0);
//  }

  char zFileNm[256];
  StrLib::vSubPath(zFileNm,(char*)azFilePath);

  va_list xArgs;
  va_start(xArgs,szFormat);
  char zCondition[256];
  StrLib::vSprint(zCondition,xArgs,(char*)szFormat);
  va_end(xArgs);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Exception xException(eFailure,azFunc,zCondition,zFileNm,iLineNo);

  vThrow_toHandler(xException);
}

/*----------------------------------------------------------------------------*/
/* throw a reached exception                                                  */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowReached(eFK   eFailure,
                                        const char* azFunc,
                                        const char* azFilePath,
                                        i8    iLineNo,
                                        Stz*  szCondition,
                                        ...)
{
  char zFileNm[1024];
  char zCondition[1024];
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the exception
  
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szCondition);
  StrLib::vSprint(zCondition,xArgs,(char*)szCondition);
  va_end(xArgs);
  
  Exception xException(eFailure,azFunc,zCondition,zFileNm,iLineNo);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // throw to Task::vExceptionHandler()
  
  vThrow_toHandler(xException);
}

/*----------------------------------------------------------------------------*/
/* throw an unimplemented exception                                           */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowUnimplemented(eFK   eFailure,
                                              const char* azFunc,
                                              const char* azFilePath,
                                              i8    iLineNo,
                                              Stz*  szCondition,
                                              ...)
{
  char zFileNm[1024];
  char zCondition[1024];
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the exception
  
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szCondition);
  StrLib::vSprint(zCondition,xArgs,(char*)szCondition);
  va_end(xArgs);
  
  Exception xException(eFailure,azFunc,zCondition,zFileNm,iLineNo);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // throw to Task::vExceptionHandler()
  
  vThrow_toHandler(xException);
}

/*----------------------------------------------------------------------------*/
/* throw a user exception                                                     */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowUser(eFK   eFailure,
                                     const char* azFunc,
                                     const char* azFilePath,
                                     i8    iLineNo,
                                     Stz*  szFormat,...)
{
//  if (AM::bDeveloper()) {
//    fprintf(stderr,
//            "BASE::vThrow:\n"
//            "eFailure = %lld\n"
//            "azFunc = %s\n"
//            "azFilePath = %s\n"
//            "iLineNo = %lld\n"
//            "szFormat = %s\n",
//            (i8)eFailure,
//            azFunc,
//            azFilePath,
//            (i8)iLineNo,
//            szFormat->azChar()
//            );
//
//    // put a breakpoint here to trap throws
//    sleep(0);
//  }
  
  char zFileNm[256];
  StrLib::vSubPath(zFileNm,(char*)azFilePath);
  
  va_list xArgs;
  va_start(xArgs,szFormat);
  char zCondition[256];
  StrLib::vSprint(zCondition,xArgs,(char*)szFormat);
  va_end(xArgs);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Exception xException(eFailure,azFunc,zCondition,zFileNm,iLineNo);
  
  vThrow_toHandler(xException);
}

/*----------------------------------------------------------------------------*/
/* have a type error                                                          */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowType(eFK eFailure,
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
  
//  if (AM::bDeveloper()) {
//    AM::vTaskDump();
//    vThreadCallStackDump();
//  }

  vThrow(eFailure,azFunc,azFilePath,iLineNo,(Stz*)zCondition);
}

/*----------------------------------------------------------------------------*/
/* have a type error                                                          */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void BASE::vThrowType2(eFK eFailure,
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
  
//  if (AM::bDeveloper()) {
//    AM::vTaskDump();
//    vThreadCallStackDump();
//  }

  vThrow(eFailure,azFunc,azFilePath,iLineNo,(Stz*)zCondition);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
