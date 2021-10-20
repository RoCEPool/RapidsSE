
/*============================================================================*/
/* CodeSynth.cpp -- Code Synthesizer                                          */
/*============================================================================*/

#include "BASE.h"

#include "Stream.hpp"
#include "StreamDef.hpp"
#include "TokenVec.hpp"
#include "Token.hpp"
#include "LogicUnit.hpp"

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
void CodeSynth::vSynthFilter(Stz* sFilterExpr, eSR eStencilRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  vTrace("Postfix: %s\n"_Stz, sFilterExpr);
  vTrace("Infix: %s\n"_Stz, (Stz*)szToInfix((char*)sFilterExpr, eStencilRow));

  s256 sCastRow;
  sCastRow.vSprint("   p%s = (struct %s*)aRow;\n   if ( "_Stz, xRowRef[eStencilRow], xRowRef[eStencilRow].m_szInclude);

  StrLib::vAppendString(&m_azCodeCrs, "\n\nL_FilterRow:\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n  if (bTrace)");
  StrLib::vAppendString(&m_azCodeCrs, "\n     vRowSingle_Print(pRowSingle);\n");
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());
  StrLib::vAppendString(&m_azCodeCrs, szToInfix((char*)sFilterExpr, eStencilRow));

  StrLib::vAppendString(&m_azCodeCrs, " ) { \n");
  StrLib::vAppendString(&m_azCodeCrs, "     goto L_ProjectRow;\n");
  StrLib::vAppendString(&m_azCodeCrs, "   } else { \n");
  StrLib::vAppendString(&m_azCodeCrs, "     vRowSingle_DiscardStr(pRowSingle);\n");
  StrLib::vAppendString(&m_azCodeCrs, "     goto L_AdvanceNextRow;\n");
  StrLib::vAppendString(&m_azCodeCrs, "   }\n");
}

/*----------------------------------------------------------------------------*/
/* synthesize the inflate                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthInflate(StreamDef* pStreamDefOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, "   \n\nL_InflateRow:");

  for (i8 i_Col = 0; i_Col < pStreamDefOut->nColumns(); i_Col++) {
    StrLib::vSprintCat((Stz*)m_caCode, "\n"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n  i_Col = %d;"_Stz , i_Col);
    StrLib::vSprintCat((Stz*)m_caCode, "\n\n  uMetaCode = *auMetaCode++;"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n  uHigh = uMetaCode >> 4;"_Stz);
    StrLib::vSprintCat((Stz*)m_caCode, "\n  uLow  = uMetaCode & 0x0F;"_Stz);

    if (i_Col < pStreamDefOut->nColumns() - 1) {
      StrLib::vSprintCat((Stz*)m_caCode, "\n\n  aSubReturnLabel = && L_RetsubInflateColumn%d;"_Stz, i_Col+1);
      StrLib::vSprintCat((Stz*)m_caCode, "\n  goto L_InflateAColumn;"_Stz);
      StrLib::vSprintCat((Stz*)m_caCode, "\n\nL_RetsubInflateColumn%d:"_Stz, i_Col+1);
    }
    else {
      StrLib::vSprintCat((Stz*)m_caCode, "\n  aSubReturnLabel = && L_FilterRow;"_Stz);
      StrLib::vSprintCat((Stz*)m_caCode, "\n  goto L_InflateAColumn;"_Stz);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* synthesize the projection                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthProject(StreamDef* pStreamDefOut, Stz* sProjectExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, "   \n\nL_ProjectRow:");
  i8 nColumns = pStreamDefOut->nColumns();

  StrLib::vAppendString(&m_azCodeCrs, "\n   #include \"../src/store/vFLAT_RowSingleToRow.h\"");
  StrLib::vAppendString(&m_azCodeCrs, "\n     goto L_AdvanceNextRow;\n");
}

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthStruct(StreamDef* pStreamDefOut, eSR eStencilRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vSprint((Stz*)m_caCode, "\n    struct %s { \n"_Stz, xRowRef[eStencilRow]);

  for (i8 i_Col = 0; i_Col < pStreamDefOut->nColumns(); i_Col++) {
    Column *pColumn = pStreamDefOut->pGetColumn(i_Col);
    StrLib::vSprintCat((Stz*)m_caCode, "      i8 %s;\n"_Stz, pColumn->szColNm());
  }

  StrLib::vSprintCat((Stz*)m_caCode, "    };"_Stz);
  StrLib::vSprintCat((Stz*)m_caCode, "\n    struct %s* p%s;"_Stz, xRowRef[eStencilRow], xRowRef[eStencilRow]);
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
char* CodeSynth::szToInfix(char* sExpr, eSR eStencilRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sInfix;
  s4K sOperand1;
  s4K sOperand2;
  TypedCell *pTypedCell;

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

      // pop operand1
      pTypedCell = m_pTypedStack->aPeekItem(0);
      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *pOperand1 = _pToken(m_pTypedStack->pPopPtr(__func__));
        sOperand1 = pOperand1->szChars();
      }
      else {
        m_pTypedStack->vPopStr(sOperand1, __func__);
      }

      // pop operand2
      pTypedCell = m_pTypedStack->aPeekItem(0);
      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *pOperand2 = _pToken(m_pTypedStack->pPopPtr(__func__));
        sOperand2 = pOperand2->szChars();
      }
      else {
        m_pTypedStack->vPopStr(sOperand2, __func__);
      }

      if (pTkn->bSame("AND"_Stz))
        pTkn->vChars("&&"_Stz);
      else
      if (pTkn->bSame("OR"_Stz))
        pTkn->vChars("||"_Stz);
      else
      if (pTkn->bSame("NOT"_Stz))
        pTkn->vChars("!"_Stz);

      // combine the two operatans and operator
      // operand2 op operand1
      sOperand2.vConcat((Stz*)pTkn->azChars());
      sOperand2.vConcat(sOperand1);

      // push new infix string on the TknVec
      m_pTypedStack->vPushStr(sOperand2);
    }
    else {
      // save the column name
      s256 sColumn;
      sColumn.vSprint("p%s->%s"_Stz, xRowRef[eStencilRow], pTkn->szChars());
      m_pTypedStack->vPushStr(sColumn);
    }
  }

  pTypedCell = m_pTypedStack->aPeekItem(0);
  if (pTypedCell->eType() == CN_PTRVAL) {
    Token *Tkn = _pToken(m_pTypedStack->pPopPtr(__func__));
    sInfix = Tkn->szChars();
  }
  else {
    m_pTypedStack->vPopStr(sInfix, __func__);
  }

  return sInfix.azChar();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
