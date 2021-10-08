
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
void Fragments::vAppendConcluding(char *azStr)
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
      "\n  printf(\"Flattended query %x\\n\", aCrwBuffer);"
      "\n"
      "\n  char* aFirstRow;"
      "\n  int   nRowSize;"
      "\n  int   nRows;"
      "\n  vSE_CrwBuffer(aCrwBuffer, &aFirstRow, &nRowSize, &nRows);"
      "\n"
      "\n  printf(\"CrwBuffer %x; aFirstRow: %x; nRowSize: %lld; nRows: %lld; \\n\", "
      "       aCrwBuffer, aFirstRow, nRowSize, nRows);"
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
      "\n  /*====================================================================*/"
      "\n  /* Flattened code                                                     */"
      "\n  /*====================================================================*/"
      "\n"
      "\n  #include \"tcclib.h\" // include libc header for TCC"
      "\n"
      "\n  #include \"../src/store/SE_interface.h\""
      "\n"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  /* flatten function                                                   */"
      "\n  /*--------------------------------------------------------------------*/"
      "\n  int bFlatten(void* aCrwBuffer)"
      "\n  {"
      "\n"
      "";

  StrLib::vAppendString(&azStr,azFrag);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
