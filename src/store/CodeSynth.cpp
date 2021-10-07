
/*============================================================================*/
/* CodeSynth.cpp -- Code Synthesizer                                          */
/*============================================================================*/

#include "BASE.h"

#include "CodeSynth.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CodeSynth,CN_CODESYNTH)
__aNewFunction(CodeSynth,CN_CODESYNTH)
__vDeleteFunction(CodeSynth,CN_CODESYNTH)
__aFooterFunction(CodeSynth)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CodeSynth::CodeSynth(void)
: Object(CN_CODESYNTH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
  
#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caCode,0xFF,C_FLATTEN_SOURCE,CN_CODESYNTH);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_CODESYNTH);
#endif

  m_caCode[0] = '\0';
  m_azCodeCrs = m_caCode;
  
  _checkInitialised_(CN_CODESYNTH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CodeSynth::~CodeSynth(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return source code                                                         */
/*----------------------------------------------------------------------------*/
Stz* CodeSynth::szCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
  
  return (Stz*)m_caCode;
}

/*----------------------------------------------------------------------------*/
/* the preliminary code fragment for this compilation unit                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendCrwFragment(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

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

  StrLib::vAppendString(&m_azCodeCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragment(Stz* szFragment)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs,szFragment->azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragment(CodeSynth* pFragment)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs,(char*)pFragment->szCode());
}

/*----------------------------------------------------------------------------*/
/* the preliminary code fragment for this compilation unit                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendPreliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
  
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

  StrLib::vAppendString(&m_azCodeCrs,azFrag);
}

/*----------------------------------------------------------------------------*/
/* the concluding code fragment for this compilation unit                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendConcluding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
  
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
  
  StrLib::vAppendString(&m_azCodeCrs,azFrag);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
