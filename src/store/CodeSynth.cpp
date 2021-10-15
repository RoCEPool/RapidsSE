
/*============================================================================*/
/* CodeSynth.cpp -- Code Synthesizer                                          */
/*============================================================================*/

#include "BASE.h"

#include "CodeSynth.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "StreamDef.hpp"
#include "TokenVec.hpp"
#include "Token.hpp"

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

  m_caCode[0]    = '\0';
  m_azCodeCrs    = m_caCode;
  m_pTypedStack  = new TypedStack(KK_DATA_STACK);
  m_pInFixTknVec = new TokenVec;

  _checkInitialised_(CN_CODESYNTH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CodeSynth::~CodeSynth(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  delete m_pTypedStack;
  delete m_pInFixTknVec;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragment(Stz* szFragment)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragment(CodeSynth* pFragment)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode());
}

/*----------------------------------------------------------------------------*/
/* synthesize the filter expression                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthFilter(Stz* sFilterExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  vTrace("Postfix: %s\n"_Stz, sFilterExpr);
  vTrace("Infix: %s\n"_Stz, (Stz*)szToInfix((char*)sFilterExpr));
}

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthStruct(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  pStreamDef->vPrint();

  StrLib::vAppendString(&m_azCodeCrs, "struct Row { \n");

  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);
    StrLib::vSprintCat((Stz*)m_caCode, "  i8 %s;\n"_Stz, pColumn->szColNm());
  }

  StrLib::vSprintCat((Stz*)m_caCode, "};"_Stz);

  vTrace("\n\nStruct generated: \n%s\n"_Stz, m_caCode);
}

/*----------------------------------------------------------------------------*/
/* return source code                                                         */
/*----------------------------------------------------------------------------*/
char* CodeSynth::azCode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  return m_caCode;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void CodeSynth::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_CODESYNTH);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert postfix to infix                                                   */
/*----------------------------------------------------------------------------*/
char* CodeSynth::szToInfix(char* sExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  char cSrc = '\0';
  s32 s;
  s4K sInFix;

  TokenVec *pTknVec = TokenVec::pTokenise((Stz*)sExpr);
  Token* pTkn;

  for (i8 i_Tkn = 0; i_Tkn < pTknVec->nItems(); i_Tkn++) {

    pTkn = pTknVec->pGet(i_Tkn);

    vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame("type"_Stz)) {
      // skip the data type
      i_Tkn++;
      continue;
    }

    if (pTkn->bIsNumeric()) {
      m_pTypedStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("negate"_Stz) ) {
      m_pTypedStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bIsDecimalNo()) {
      m_pTypedStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("DATE"_Stz) ||  pTkn->bSame("TIME"_Stz) || pTkn->bSame("TIMESTAMP"_Stz)) {
      m_pTypedStack->vPushPtr(pTkn);
      i_Tkn++;
      pTkn = pTknVec->pGet(i_Tkn);
      m_pTypedStack->vPushPtr(pTkn);
      continue;
    }

    // operator ?
    if (pTkn->bIsArithmetic() || pTkn->bIsLogical() || pTkn->bIsRelational() ) {

      // pop two operands for this operator
      Token* pOperand1 = _pToken(m_pTypedStack->pPopPtr(__func__));
      Token* pOperand2 = _pToken(m_pTypedStack->pPopPtr(__func__));

      // combine in Infix format
      m_pInFixTknVec->vAppendToken(pOperand2);
      m_pInFixTknVec->vAppendToken(pTkn);
      m_pInFixTknVec->vAppendToken(pOperand1);

      // form the indix string
      m_pInFixTknVec->vToTknString(sInFix);
      m_pInFixTknVec->vSoftReset();

      // push new infix string on the TknVec
      Token* pTkn = new Token;
      pTkn->vChars(sInFix);
      pTkn->vToken(TN_STRING);
      m_pTypedStack->vPushPtr(pTkn);

    }
    else {
      // save the column name
      m_pTypedStack->vPushPtr(pTkn);
    }
  }

  pTkn = _pToken(m_pTypedStack->pPopPtr(__func__));

  return pTkn->azChars();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
