
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

  char* azHeaders = (char*)
      "\n"
      "\n  /*====================================================================*/"
      "\n  /* Flattened code combinedOp                                          */"
      "\n  /*====================================================================*/"
      "\n"
      "";

  StrLib::vAppendString(&azStr,azHeaders);
}

/*----------------------------------------------------------------------------*/
/* append preliminary defines etc                                             */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendPreliminary(char *azStr) {
  _static_

  char* azHeaders = (char*)
      "\n"
      "\n"
      "\n  //#include <stdio.h>"
      "\n  //#include <string.h>"
      "\n"
      "\n  #include \"tcclib.h\" // include libc header for TCC"
      "\n"
      "\n  #include \"../src/base/BaseTypes_tcc.h\""
      "\n  #include \"../src/base/TccStuff.h\""
      "";

  StrLib::vAppendString(&azStr,azHeaders);

  char* azFrag = (char*)
      "\n"
      "\n  void vNeverReach(i8 iLine);"
      "\n  #define NEVER_REACH vNeverReach(__LINE__)"
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
      "\n  static const i8 C_I8_MIN = -9223372036854775806;"
      "\n  static const i8 C_I8_MAX = 9223372036854775806;"
      "\n  static const i8 C_BCD_MIN = -140737488355328;"
      "\n  static const i8 C_BCD_MAX = 140737488355327;"
      "\n  static const i8 C_CRW_HEADBYTES = 32;"
      "\n  static const i8 C_CRW_FIRSTHEAD = 640;"
      "\n  static const tET C_ENT_END_TICKET = 0xFFFF;"
      "\n  static const u8 C_YEAR_BASE  = 1900;"
      "\n"
      "\n  int strcmp(const char *__s1, const char *__s2);"
      "\n  void strncpy(const char *__s1, const char *__s2, size_t);"
      "\n"
      "\n  #define C_BITS_PER_WORD 64"
      "\n  #define C_MAX_LVL_LEVELS 6"
      "\n  #define C_LOGIC_LOCALS 32"
      "\n  #define C_UNKNOWN -1"   "\n"
      "\n  #include \"../src/store/Ent_int.h\""
      "\n  #include \"../src/base/eEC.h\""
      "\n  #include \"../src/base/CastMacros.h\""
      "\n  #include \"../src/base/LogicalMacros.h\""
      "\n  #include \"../src/base/RelationalMacros.h\""
      "\n  #include \"../src/base/MathMacros.h\""
      "\n  #include \"../src/base/TccMacros.h\""
      "\n"
      "";

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
void Fragments::vAppendConcludingFunc0(char *azStr)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n   L_Return:"
      "\n     goto L_ReturnFinal;"
      "\n"
      "\n   #include \"../src/store/vFLAT_InflateColumn.h\""
      "\n   #include \"../src/store/vFLAT_SkipColumn.h\""
      "\n"
      "\n   L_ReturnFinal:"
      "\n     return 1;"
      "\n  }"
      "";

  StrLib::vAppendString(&azStr,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the concluding code fragment for this compilation unit                     */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendConcludingFunc1(char *azStr)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n   L_Return:"
      "\n     goto L_ReturnFinal;"
      "\n"
      "\n   #include \"../src/store/vFLAT_InflateColumn.h\""
      "\n   #include \"../src/store/vFLAT_SkipColumn.h\""
      "\n"
      "\n   L_ReturnFinal:"
      "\n     return 1;"
      "\n  }"
      "";

  StrLib::vAppendString(&azStr,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the concluding code fragment for this compilation unit                     */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendConcludingFunc2(char *azStr)
{
  _static_

  char* azFrag = (char*)
      "\n   L_Return:"
      "\n     goto L_ReturnFinal;"
      "\n"
      "\n   #include \"../src/store/vFLAT_InflateColumn.h\""
      "\n   #include \"../src/store/vFLAT_SkipColumn.h\""
      "\n"
      "\n   L_ReturnFinal:"
      "\n     return 1;"
      "\n  }"
      "";

  StrLib::vAppendString(&azStr,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the concluding code fragment for this compilation unit                     */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendConcludingFunc3(char *azStr)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n   L_ReturnFinal:"
      "\n     return 1;"
      "\n  }"
      "";

  StrLib::vAppendString(&azStr,azFrag);
}

/*----------------------------------------------------------------------------*/
/* append empty                                                               */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendEmptyFunc(char *azStr, Stz* szDesc, Stz* szFunc) {
  _static_

  s256 sCopy;

  char *azFrag = (char *)
      "\n"
      "\n  /*--------------------------------------------------------------------*/"
      "";

  StrLib::vAppendString(&azStr,azFrag);

  sCopy.vSprint(
      "\n  /* flattened function -- %--12s                                 */"_Stz,
      szDesc);


  StrLib::vAppendString(&azStr,sCopy.azChar());

  azFrag = (char *)
      "\n  /*--------------------------------------------------------------------*/"
      "";
  StrLib::vAppendString(&azStr,azFrag);


  StrLib::vAppendString(&azStr, szFunc->azChar());
  StrLib::vAppendString(&azStr, " { ");
  StrLib::vAppendString(&azStr, "\n");
  StrLib::vAppendString(&azStr, "\n  L_Return:");
  StrLib::vAppendString(&azStr, "\n    return 1;");
  StrLib::vAppendString(&azStr, "\n  }");

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
      "";

  StrLib::vAppendString(&azStr,azFrag);
}
/*----------------------------------------------------------------------------*/
/* the locals fragment                                                        */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendLocals(char *azStr) {
  _static_

  char *azFrag = (char *)
      "\n"
      "\n    char* szSrcStz;"
      "\n    char* szDstStz;"
      "\n    char* szStrArea;"
      "\n    char* szArg1;"
      "\n    char* szArg2;"
      "\n    m8*   amCellAddress;"
      "\n    i8    bFilterResult;"
      "\n    i8    bNullResult;"
      "\n    i8    bFound;"
      "\n    i8*   piColumn;"
      "\n    i8    iIndex;"
      "\n    i8    nSrcSize;"
      "\n    i8    iOffset;"
      "\n    i8    iMixIn;"
      "\n    i8    iBufferJ;"
      "\n    i8    iItemJ;"
      "\n    i8    i_BaseOffsetDataJ;"
      "\n    i8    i_BaseOffsetDataT;"
      "\n    i8    iIndexScanBuffer;"
      "\n    i8    iIndexScanItem;"
      "\n    i8    i_CurrentRow = -1;"
      "\n    i8    iElemSize;"
      "\n    i8    iArg1;"
      "\n    i8    iArg2;"
      "\n    i8*   paW;"
      "\n    union Bcd_T bcdArg1;"
      "\n    union Bcd_T bcdArg2;"
      "\n    i8    iaPow10[15];"
      "\n    i8    iScaleDiff;"
      "\n    union Dte_T xDte1;"
      "\n    union Dte_T xDte2;"
      "\n    union Tsp_T xTsp1;"
      "\n    union Tsp_T xTsp2;"
      "\n    union Tme_T xTme1;"
      "\n    union Tme_T xTme2;"
      "\n    void* aIndexScanCrw;"
      "\n    void* aEntInserted;"
      "\n    void* aEntFound;"
      "\n    void* aCrwJ;";
  "\n"
  "";

  StrLib::vAppendString(&azStr, azFrag);
}

/*----------------------------------------------------------------------------*/
/* append preliminary fragment code for func0                                 */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendPreliminaryFunc0(char *azStr, Stz* szFunSignature)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  /* flattened function -- CRWBUFFER                                    */"
      "\n  /*--------------------------------------------------------------------*/"
      "\n"
      "";

     StrLib::vAppendString(&azStr,azFrag);
     StrLib::vAppendString(&azStr,szFunSignature->azChar());

     azFrag = (char*)
      "\n  {"
      "\n"
      "\n    #include \"../src/store/vFLAT_Parameters.h\""
      "\n    #include \"../src/store/vFLAT_PreliminaryCrw.h\""
      "\n"
      "";
     StrLib::vAppendString(&azStr,azFrag);

  vAppendLocals(azStr);
}

/*----------------------------------------------------------------------------*/
/* append preliminary fragment code for func1 (ROWBUFFER)                     */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendPreliminaryFunc1(char *azStr, Stz* szFunSignature)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  /* flattened function -- ROWBUFFER                                    */"
      "\n  /*--------------------------------------------------------------------*/"
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag);
  StrLib::vAppendString(&azStr,szFunSignature->azChar());

  azFrag = (char*)
      "\n  {"
      "\n"
      "\n    #include \"../src/store/vFLAT_Parameters.h\""
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag);
  vAppendLocals(azStr);
}

/*----------------------------------------------------------------------------*/
/* append preliminary fragment code for func2 ( INDEXBUFFER)                  */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendPreliminaryFunc2(char *azStr, Stz* szFunSignature)
{
  _static_

  char* azFrag = (char*)
      "\n"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  /* flattened function -- INDEXBUFFER                                  */"
      "\n  /*--------------------------------------------------------------------*/"
      "\n"
      "";

      StrLib::vAppendString(&azStr,azFrag);
      StrLib::vAppendString(&azStr,szFunSignature->azChar());

      azFrag = (char*)
      "\n  {"
      "\n"
      "\n    #include \"../src/store/vFLAT_Parameters.h\""
      "\n"
      "";

    StrLib::vAppendString(&azStr,azFrag);
    vAppendLocals(azStr);
}

/*----------------------------------------------------------------------------*/
/* append preliminary fragment code for func3                                 */
/*----------------------------------------------------------------------------*/
void Fragments::vAppendPreliminaryFunc3(char *azStr, Stz* szFunSignature)
{
  _static_

  char* azHeaders = (char*)
      "\n"
      "\n  //#include <stdio.h>"
      "\n  //#include <string.h>"
      "\n"
      "\n  #include \"tcclib.h\" // include libc header for TCC"
      "\n"
      "\n  #include \"../src/base/BaseTypes_tcc.h\""
      "\n  #include \"../src/base/TccStuff.h\""
      "\n  #include \"../src/base/CastMacros.h\""
      "\n  #include \"../src/base/LogicalMacros.h\""
      "\n  #include \"../src/base/MathMacros.h\""
      "\n  #include \"../src/base/RelationalMacros.h\""
      "\n  #include \"../src/base/TccMacros.h\""
      "";
  StrLib::vAppendString(&azStr,azHeaders);

  char* azFrag = (char*)
      "\n"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  /* flattened function -- EOSMARKER                                    */"
      "\n  /*--------------------------------------------------------------------*/"
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag);
  StrLib::vAppendString(&azStr,szFunSignature->azChar());

  azFrag = (char*)
      "\n"
      "\n  {"
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag);
  vAppendLocals(azStr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
