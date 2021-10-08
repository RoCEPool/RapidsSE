
/*============================================================================*/
/* Fragments.cpp -- code fragments utility                                    */
/*============================================================================*/

#include "BASE.h"

#include "Fragments.hpp"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* the concluding code fragment for this compilation unit                     */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendConcluding(char *azStr, i8* piLineNumber)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n    return 1; "
      "\n  }"
      "\n"
      "\n  /*====================================================================*/"
      "\n  /* end                                                                */"
      "\n  /*====================================================================*/"
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag,piLineNumber);
}

/*----------------------------------------------------------------------------*/
/* the crw iteration fragment                                                 */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendCrwIteration(char *azStr, i8* piLineNumber)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n    printf(\"Flattended query %x \", aCrwBuffer);"
      "\n"
      "\n    char* aFirstRow;"
      "\n    i8    nRowSize;"
      "\n    i8    nRows;"
      "\n"
      "\n    vSE_CrwBuffer(aCrwBuffer, &aFirstRow, &nRowSize, &nRows);"
      "\n"
      "\n    printf(\"CrwBuffer %x; aFirstRow: %x; nRowSize: %lld; nRows: %lld; \", "
      "         aCrwBuffer, aFirstRow, nRowSize, nRows);"
      "";

  StrLib::vAppendString(&azStr,azFrag, piLineNumber);
}
/*----------------------------------------------------------------------------*/
/* the inflate fragment                                                       */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendInflate(char *azStr, i8* piLineNumber)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n    #include \"../src/store/vInflate_FLAT.h\""
      "";

  StrLib::vAppendString(&azStr,azFrag, piLineNumber);
}

/*----------------------------------------------------------------------------*/
/* append preliminary fragment code                                           */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendPreliminary(char *azStr, i8* piLineNumber)
{
  _static_

  char* azFrag = (char*)
      "\n  /*====================================================================*/"
      "\n  /* Flattened code                                                     */"
      "\n  /*====================================================================*/"
      "\n"
      "\n  #include \"tcclib.h\" // include libc header for TCC"
      "\n"
      "\n  int strcmp(const char *__s1, const char *__s2);"
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
      "\n  #define C_BITS_PER_WORD 64"
      "\n  #define C_MAX_LVL_LEVELS 6"
      "\n  #define C_LOGIC_LOCALS 32"
      "\n  #define C_UNKNOWN -1"   "\n"
      "\n  #include \"../src/store/SE_interface.h\""
      "\n"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  /* flatten function                                                   */"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  i8 bFlatten(void* aCrwBuffer)"
      "\n  {"
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag, piLineNumber);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
