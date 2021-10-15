
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
#include "LogicUnit.hpp"
#include "TagConstants.h"

RowRef xRowRef[]
{
    { "none"_Stz },   // SR_NONE

    {"SR_IJ"_Stz},    // SR_IJ
    {"SR_IT"_Stz},    // SR_IT
    {"SR_PJ"_Stz},    // SR_PJ
    {"SR_PT"_Stz},    // SR_PT
    {"SR_TJ"_Stz},    // SR_TJ
    {"SR_H"_Stz},     // SR_H

    {nullptr}
};

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
void CodeSynth::vAppendFragment(Stz* szFragment, i8* piLineNumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar(), piLineNumber);
#else
  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar());
#endif
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragment(CodeSynth* pFragment, i8* piLineNumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
#ifdef WITH_DEBUG_VERSION
  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode(), piLineNumber);
#else
  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode());
#endif
}

/*----------------------------------------------------------------------------*/
/* print the synthesized code                                                 */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPrint(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  vTrace("%s\n"_Stz,m_caCode);
}

/*----------------------------------------------------------------------------*/
/* synthesize the filter expression                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthFilter(Op* pOp, eSR eStencilRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  TagSet* pOpTags = pOp->pOpTags();

  // get expr from OP
  Stz* sFilterExpr = pOpTags->szValue(C_TAG_EXPR);

  //vTrace("Postfix: %s\n"_Stz, sFilterExpr);
  //vTrace("Infix: %s\n"_Stz, (Stz*)szToInfix((char*)sFilterExpr));

  s256 sCastRow;
  sCastRow.vSprint("   pRowStruct = (%s*)&aRow->w_wxaColumnCell[0];\n   if ( "_Stz, xRowRef[eStencilRow].m_szInclude);

  StrLib::vAppendString(&m_azCodeCrs, "\n\nL_FilterRow:\n");
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());
  StrLib::vAppendString(&m_azCodeCrs, szToInfix((char*)sFilterExpr));
  StrLib::vAppendString(&m_azCodeCrs, " ) \n");
  StrLib::vAppendString(&m_azCodeCrs, "     goto L_AdvanceNextCrw;\n");
  StrLib::vAppendString(&m_azCodeCrs, "   else { \n");
  StrLib::vAppendString(&m_azCodeCrs, "     pRowBuffer->vDiscardInvalidRow();\n");
  StrLib::vAppendString(&m_azCodeCrs, "     goto L_AdvanceNextCrw;\n");
  StrLib::vAppendString(&m_azCodeCrs, "   }\n");
}

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthStruct(Op* pOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDef = *pLogicUnit->m_xpStreamDefOut;

  StrLib::vSprint((Stz*)m_caCode, "\n    struct %s { \n"_Stz, xRowRef[SR_IT]);

  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);
    StrLib::vSprintCat((Stz*)m_caCode, "      i8 %s;\n"_Stz, pColumn->szColNm());
  }

  StrLib::vSprintCat((Stz*)m_caCode, "    };"_Stz);

  //vTrace("\n\nStruct generated: \n%s\n"_Stz, m_caCode);
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

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

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
      Token* pTknNew = new Token;
      pTknNew->vChars(sInFix);
      pTknNew->vToken(TN_STRING);
      m_pTypedStack->vPushPtr(pTknNew);

    }
    else {
      // save the column name
      s256 sColumn;
      sColumn.vSprint("pRowStruct->%s"_Stz, pTkn->szChars());
      pTkn->vChars(sColumn);
      m_pTypedStack->vPushPtr(pTkn);
    }
  }

  pTkn = _pToken(m_pTypedStack->pPopPtr(__func__));

  return pTkn->azChars();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
