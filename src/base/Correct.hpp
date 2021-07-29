/*============================================================================*/
/* Correct.hpp -- Correctness Functions                                       */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_STACK_BACKTRACE = 16;

  void vDebugPoint(void);
  void vThreadCallStackDump(void);
  void vThreadCallStackDump_Mangled(void);
  void vThreadCallStackDump_Demangled(void);
  b1   bAddrsToLines(void** pvAddrs,
                     i8     iArrSize,
                     s256*  psModules,
                     s256*  psFileNames,
                     s2K*   psFuncNames,
                     i8*    piLines,
                     i8*    piOffsets);
  b1   bParseBacktraceSymbol(const char* pBtLine, s256& sModule, s2K& sFuncName, i8& iOffset);

  // the [[ noreturn ]] attribute is the C++11 standard for indicating a
  // function never returns
  //
  // refer: https://en.cppreference.com/w/cpp/language/attributes/noreturn
  
  [[ noreturn ]] void vAbortProcess(void);
    
  [[ noreturn ]] void vThrow(eFK   eKind,
                              const char* azFunc,
                             const char* azFilePath,
                             i8    iLineNo,
                             Stz*  szCondition,...);

  [[ noreturn ]] void vThrowLowMemory(eFK   eKind,
                                 const char* azFunc,
                                 const char* azFilePath,
                                 i8    iLineNo,
                                 Stz*  szFormat,...);

  [[ noreturn ]] void vThrowUser(eFK   eKind,
                             const char* azFunc,
                             const char* azFilePath,
                             i8    iLineNo,
                             Stz*  szFormat,...);
  
  [[ noreturn ]] void vThrowType(eFK   eKind,
                                 const char* azFunc,
                                 const char* azFilePath,
                                 i8    iLineNo,
                                 void* aObject,
                                 eCN   eExpectingClass);

  [[ noreturn ]] void vThrowType2(eFK   eKind,
                                  const char* azFunc,
                                  const char* azFilePath,
                                  i8    iLineNo,
                                  void* aObject,
                                  eCN   eExpectingClass,
                                  eCN   eGotClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
