
/*============================================================================*/
/* Fragments.cpp -- code fragments utility                                    */
/*============================================================================*/

#include "BASE.h"

#include "Fragments.hpp"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append banner head                                                         */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendBannerHead(char *azStr) {
  _static_

  char *azFrag = (char *)
      "\n  /*====================================================================*/"
      "\n  /* Flattened code for Func0                                           */"
      "\n  /*====================================================================*/"
      "\n";

  StrLib::vAppendString(&azStr,azFrag);
}

/*----------------------------------------------------------------------------*/
/* append preliminary fragment code                                           */
/*----------------------------------------------------------------------------*/
  void Fragments::vAppendBannerTail(char *azStr) {
  _static_

  char *azFrag = (char *)
      "\n"
      "\n  /*====================================================================*/"
      "\n  /* end                                                                */"
      "\n  /*====================================================================*/"
      "";

  StrLib::vAppendString(&azStr, azFrag);
}

/*----------------------------------------------------------------------------*/
/* the concluding code fragment for this compilation unit                     */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendConcluding(char *azStr)
{
  _static_

  char* azFrag = (char*)
      "\n   #include \"../src/store/vFLAT_InflateColumn.h\""
      "\n   #include \"../src/store/vFLAT_SkipColumn.h\""
      "\n"
      "\nL_Return:"
      "\n    return 1;"
      "\n  }"
      "\n"
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the crw iteration fragment                                                 */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendCrwIteration(char *azStr)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n"
      "\n    char* aFirstRow;"
      "\n    i8    nRowSize;"
      "\n    i8    nRows;"
      "\n"
      "\n    vSE_CrwBuffer(aCrwBuffer, &aFirstRow, &nRowSize, &nRows);"
      "\n"
      "\n    printf(\"CrwBuffer %x; aFirstRow: %x; nRowSize: %lld; nRows: %lld; \\n\", "
      "         aCrwBuffer, aFirstRow, nRowSize, nRows);"
      "";

  StrLib::vAppendString(&azStr,azFrag);
}
/*----------------------------------------------------------------------------*/
/* the inflate fragment                                                       */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendInflate(char *azStr)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n#include \"vFLAT_InflateColumn.h\""
      "\n#include \"vFLAT_SkipColumn.h\""
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag);
}

/*----------------------------------------------------------------------------*/
/* append preliminary fragment code                                           */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendPreliminary(char *azStr)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n  #include \"tcclib.h\" // include libc header for TCC"
      "\n  #include \"../src/base/BaseTypes_tcc.h\""
      "\n  #include \"../src/base/TccStuff.h\""
      "\n"
      "\n  void vNeverReach(i8 iLine);"
      "\n  #define NEVER_REACH vNeverReach(__LINE__)"
      "\n"
      "\n"
      "\n  typedef char i1;"
      "\n  typedef unsigned short u2;"
      "\n  typedef long long i8;"
      "\n  typedef unsigned char byte;"
      "\n  typedef double d8;"
      "\n  typedef unsigned long long m8;"
      "\n  typedef unsigned long long u8;"
      "\n  typedef char Stz;"
      "\n"
      "\n  typedef unsigned char eEO;"
      "\n  typedef unsigned short tET;"
      "\n"
      "\n  static const i8 C_1MB = 1048576;"
      "\n"
      "\n  static const void* C_ADR_REGION = (void*)0x204800000000;"
      "\n  static const i8 C_DATUM_NULL = -9223372036854775807;"
      "\n  static const i8 C_CRW_HEADBYTES = 32;"
      "\n  static const i8 C_CRW_FIRSTHEAD = 640;"
      "\n  static const tET C_ENT_END_TICKET = 0xFFFF;"
      "\n"
      "\n  int strcmp(const char *__s1, const char *__s2);"
      "\n  void strncpy(const char *__s1, const char *__s2, size_t);"

      "\n  #define C_BITS_PER_WORD 64"
      "\n  #define C_MAX_LVL_LEVELS 6"
      "\n  #define C_LOGIC_LOCALS 32"
      "\n  #define C_UNKNOWN -1"   "\n"
      "\n  #include \"../src/store/Ent_int.h\""
      "\n"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  /* flattened function                                                 */"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  int iFun0(struct Combined_T* aCombinedT)"
      "\n  {"
      "\n"
      "\n   #include \"../src/store/vFLAT_Parameters.h\""
      "\n   #include \"../src/store/vFLAT_PreliminaryRow.h\""
      "\n"
      "";

    StrLib::vAppendString(&azStr,azFrag);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
