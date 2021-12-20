
/*============================================================================*/
/* CodeSynth.cpp -- Code Synthesizer                                          */
/*============================================================================*/

#include "BASE.h"

#include "Stream.hpp"
#include "StreamDef.hpp"
#include "TokenVec.hpp"
#include "Token.hpp"
#include "LogicUnit.hpp"
#include "Index.hpp"
#include "MathLib.h"
#include "SN_Dte.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Bol.hpp"
#include "SN_Flt.hpp"
#include "SN_Bcd.hpp"
//#include "MathOp.h"

extern i8 iMathOp_MaxValue(eDM eDatum);
extern i8 iMathOp_MinValue(eDM eDatum);

extern void vMathOp_MaxValue(Stz* sInitValue);
extern void vMathOp_MinValue(Stz* sInitValue);

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

  m_caCode[0]   = '\0';
  m_azCodeCrs   = m_caCode;
  m_pTokenVec   = nullptr;

  m_pExprStack  = new TypedStack(KK_DATA_STACK);
  m_pAggrStack  = nullptr;
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pPostExpr   = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pStreamDefLft = nullptr;
  m_pStreamDefOut = nullptr;
  m_eStencilFrom  = SR_NONE;
  m_eStencilTo    = SR_NONE;

  _checkInitialised_(CN_CODESYNTH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CodeSynth::CodeSynth(StreamDef* pStreamDefRht, eSR eStencilTo)
    : Object(CN_CODESYNTH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caCode,0xFF,C_FLATTEN_SOURCE,CN_CODESYNTH);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_CODESYNTH);
#endif

  m_caCode[0]   = '\0';
  m_azCodeCrs   = m_caCode;
  m_pTokenVec   = nullptr;

  m_pExprStack  = new TypedStack(KK_DATA_STACK);
  m_pAggrStack  = nullptr;
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pPostExpr   = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pStreamDefLft = nullptr;
  m_pStreamDefOut = pStreamDefRht;
  m_eStencilFrom  = SR_NONE;
  m_eStencilTo    = eStencilTo;

  _checkInitialised_(CN_CODESYNTH);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CodeSynth::CodeSynth(StreamDef* pStreamDefLft, StreamDef* pStreamDefRht, eSR eStencilFrom, eSR eStencilTo)
    : Object(CN_CODESYNTH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caCode,0xFF,C_FLATTEN_SOURCE,CN_CODESYNTH);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_CODESYNTH);
#endif

  m_caCode[0]   = '\0';
  m_azCodeCrs   = m_caCode;
  m_pTokenVec   = nullptr;

  m_pExprStack  = new TypedStack(KK_DATA_STACK);
  m_pAggrStack  = nullptr;
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pPostExpr   = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pStreamDefLft = pStreamDefLft;
  m_pStreamDefOut = pStreamDefRht;
  m_eStencilFrom  = eStencilFrom;
  m_eStencilTo    = eStencilTo;

  _checkInitialised_(CN_CODESYNTH);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CodeSynth::~CodeSynth(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  delete m_pTokenVec;

  delete m_pExprStack;
  delete m_pAggrStack;
  delete m_pLowerStack;
  delete m_pUpperStack;

  delete m_pPostExpr;
  delete m_pInitStack;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vFormColumn(Stz* sColumn, StreamDef* pStreamDef, eSR eStencilRow, i8 i_Column)
{
  _static_

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  // convert column name to uppercase
  s256 sColNm;
  sColNm = pColumn->szColNm();
  StrLib::vToUpper((char*)sColNm.azChar());

  // form the column name
  sColumn->vSprint("p%s->%s"_Stz, xRowRef[eStencilRow], sColNm.azChar());
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set aggregate stack                                                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAggrStack(TypedStack* pAggrStack) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  m_pAggrStack = pAggrStack;
}

/*----------------------------------------------------------------------------*/
/* get aggregate stack                                                        */
/*----------------------------------------------------------------------------*/
TypedStack* CodeSynth::pAggrStack(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  return m_pAggrStack;
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragmentSemiColon(Stz* szFragment)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar());
  StrLib::vAppendString(&m_azCodeCrs, ";");
}

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
void CodeSynth::vAppendFragment(Stz* szFragment, i8* piLineNumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar(), piLineNumber);
#else
  UNUSED(piLineNumber);
  StrLib::vAppendString(&m_azCodeCrs, szFragment->azChar());
#endif
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
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vAppendFragment(CodeSynth* pFragment, i8* piLineNumber)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

#ifdef WITH_DEBUG_VERSION
  StrLib::vAppendString(&m_azCodeCrs, pFragment->azCode(), piLineNumber);
#else
  UNUSED(piLineNumber);
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
/* synthesize the EOS                                                         */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthEos(IndexDef* pIndexDef, IntStack* pAggTypeStack) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  vPayloadTraversal(pIndexDef, pAggTypeStack);

}

/*----------------------------------------------------------------------------*/
/* synthesize the filter expression                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthFilter(Stz* sFilterExpr, Stz* sLabel, Stz* sLabelNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCopy;

  StrLib::vAppendString(&m_azCodeCrs,"\n");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // FILTER-OP ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  // generate inFix from postFix
  vToInfix(m_pStreamDefLft, nullptr, (char*)sFilterExpr, m_eStencilFrom, m_eStencilTo, nullptr, PJ_NONE, false);

  // convert postfix to inFix
  TypedCell* pTypedCell = m_pExprStack->aPeekItem(0);
  if (pTypedCell->eType() == CN_PTRVAL) {
    Token *Tkn = _pToken(m_pExprStack->pPopPtr(__func__));
    sInfix = Tkn->szChars();
  }
  else {
    m_pExprStack->vPopStr(sInfix, __func__);
  }

  sCopy.vSprint("\n\n%s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  switch(m_eStencilFrom) {
    case SR_IJ:
      sCopy.vSprint("\n   p%s = (struct %s*)aCombinedT->m_aRowCellArrayJ;\n   if ( "_Stz, xRowRef[m_eStencilFrom], xRowRef[m_eStencilFrom].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      break;
    case SR_IT:
      sCopy.vSprint("\n   p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;\n   if ( "_Stz, xRowRef[m_eStencilFrom], xRowRef[m_eStencilFrom].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      break;
    case SR_PJ:
      sCopy.vSprint("\n   p%s = (struct %s*)aCombinedT->m_aRowCellArrayJ;\n   if ( "_Stz, xRowRef[m_eStencilFrom], xRowRef[m_eStencilFrom].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      break;
    case SR_PT:
      sCopy.vSprint("\n   p%s = (struct %s*)aCombinedT->m_aRowCellArrayP;\n   if ( "_Stz, xRowRef[m_eStencilFrom], xRowRef[m_eStencilFrom].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      break;
    case SR_TJ:
      sCopy.vSprint("\n   p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;\n   if ( "_Stz, xRowRef[m_eStencilFrom], xRowRef[m_eStencilFrom].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      break;
    default:
      RASSERT(0);
  }

  StrLib::vAppendString(&m_azCodeCrs, sInfix.azChar());

  StrLib::vAppendString(&m_azCodeCrs, " ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n   } else { ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");

  sCopy.vSprint("\n    goto %s;"_Stz, sLabelNext);

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the filter expression after an ordAgg op                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthFilterOrdAgg(Stz* sFilterExpr, Stz* sLabel, Stz* sLabelPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCopy;

  StrLib::vAppendString(&m_azCodeCrs,"\n");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // FILTER-OP (ORDAGG)");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  // generate inFix from postFix
  vToInfix(m_pStreamDefLft, nullptr, (char*)sFilterExpr, m_eStencilFrom, SR_NONE, nullptr, PJ_NONE, false);
  RASSERT(m_pExprStack->nValues() == 1);

  // convert postfix to inFix
  TypedCell* pTypedCell = m_pExprStack->aPeekItem(0);
  if (pTypedCell->eType() == CN_PTRVAL) {
    Token *Tkn = _pToken(m_pExprStack->pPopPtr(__func__));
    sInfix = Tkn->szChars();
  }
  else {
    m_pExprStack->vPopStr(sInfix, __func__);
  }

  sCopy.vSprint("\n\n%s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("\n   if ( "_Stz, xRowRef[m_eStencilFrom], xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, " ");
  StrLib::vAppendString(&m_azCodeCrs, sInfix.azChar());

  StrLib::vAppendString(&m_azCodeCrs, " ) {");

  StrLib::vAppendString(&m_azCodeCrs, "\n    goto L_AddRowToTub;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");

  sCopy.vSprint("\n   goto %s;"_Stz, sLabelPrev);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the index join                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthIndexJoin(StreamDef* pStreamDefRht, IndexDef* pIndexDefJ,
                                Stz* sJoinExpr, Stz* sLabel, Stz* sLabelNext, u8 uSelectColumns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
  s256 sCopy;
  s256 sCastRow;
  s256 sInfix;

  StrLib::vAppendString(&m_azCodeCrs,"\n");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // INDEXJOIN-OP ");
  StrLib::vAppendString(&m_azCodeCrs,"\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n");

  // generate the infix from eStencilFrom
  vToInfix(m_pStreamDefLft, nullptr, (char*)sJoinExpr, m_eStencilFrom, SR_NONE, nullptr, PJ_NONE, false);
  RASSERT(m_pExprStack->nValues() == 1);

  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 iBufferJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 iItemJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void* aCrwJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 i_BaseOffsetDataJ;");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the label
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n %s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;"_Stz, xRowRef[m_eStencilFrom], xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;"_Stz, xRowRef[m_eStencilTo], xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // init index join
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // init indexJoin");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( aCombinedT->m_iIndexJoinInit == 0 ) {");
  sCopy.vSprint("\n      vEntWrk_entKind(aCombinedT->m_aEntWrkJ,0,%lld);"_Stz, pIndexDefJ->eKeyKind());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iIndexJoinInit = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n   if (!aCombinedT->m_iProcessRow)");
  StrLib::vAppendString(&m_azCodeCrs, "\n     goto L_AdvanceNextRow;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the join key
  if (pIndexDefJ->eKeyKind()) {

    TypedCell *pTypedCell = m_pExprStack->aPeekItem(0);
    RASSERT(pTypedCell->eType() != CN_PTRVAL);
    sInfix = pTypedCell->szStz();

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the entKey from T-table");
    sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKeyJ = %s;"_Stz, sInfix.azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n      if (aCombinedT->m_bTrace)");
    StrLib::vAppendString(&m_azCodeCrs, "\n        vEntWrk_print(aCombinedT->m_aEntWrkJ, aCombinedT->m_aIndexDefJ);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs, "\nassert(0);");
  }

  // position at entry greater than equal to the join key (lbound)
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // position at entry >= join key (lbound)");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aEntIntJ = (struct Ent_int *)aIndexTree_positionGE(aCombinedT->m_aIndexTreeJ, &aCombinedT->m_aEntBufferJ, aCombinedT->m_aEntKeyJ);");

  // did we find the entry ?
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( ! aCombinedT->m_aEntIntJ ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_AdvanceNextRow;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_bTrace)");
  StrLib::vAppendString(&m_azCodeCrs, "\n      vEntWrk_print(aCombinedT->m_aEntWrkJ, aCombinedT->m_aIndexDefJ);");

  // find the upper bound entry
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // find the upper bound entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aEntUpperJ = aIndexTree_positionLE(aCombinedT->m_aIndexTreeJ, aCombinedT->m_aEntKeyJ);");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n L_IndexJoinDeref:");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // check reached upper bound
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_aEntIntJ == aCombinedT->m_aEntUpperJ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_AdvanceNextRow;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n    aHeadLineSav = aHeadLine;");

  // get the crw from the entry
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // dereference the entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iBufferJ = aCombinedT->m_aEntIntJ->Rrf.m_uBuffer;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iItemJ   = aCombinedT->m_aEntIntJ->Rrf.m_uItem;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCrwJ  = ( (char*)C_ADR_REGION + iBufferJ * C_1MB+ C_CRW_FIRSTHEAD+ iItemJ * C_CRW_HEADBYTES );");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // inflate the derefenced entry");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Join the rows of T and J

  vGenerateAssign(uSelectColumns);

  // join the right row directly from dereferenced entry into join row
  i8 nLeftColumns = m_pStreamDefLft->nColumns();

  StrLib::vAppendString(&m_azCodeCrs, "\n    aHeadLine = (struct HeadLine_T *)aCrwJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    auMetaCode = META_AREA(aHeadLine);");

  sCopy.vSprint("\n    ayDstByte = (byte*)aCombinedT->m_aRowCellArrayTJ + 8*%lld;"_Stz, nLeftColumns);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  //aCombinedT->m_aRowCellArrayT = ayDstByte;
  StrLib::vAppendString(&m_azCodeCrs, "\n    acStrArea = (char*)aCombinedT->m_racStrAreaTJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    racStrArea = (char*)aCombinedT->m_racStrAreaTJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    eStrInflateRule = SI_STRAREA_ADR;");

  sCopy.vSprint("\n    i_BaseOffsetDataJ = C_1B  + C_8B  + %lld;"_Stz, pStreamDefRht->nColumns());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n    ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetDataJ);");

  // for each column of row of right table J
  vGenerateInflate(pStreamDefRht, "L_RetT_ColJ"_Stz, nLeftColumns, uSelectColumns);

  StrLib::vAppendString(&m_azCodeCrs, "\n    aHeadLine = aHeadLineSav;");

  sCopy.vSprint("\n\n    goto %s;"_Stz, sLabelNext->azChar());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs,"\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n L_AdvanceNextRowJ:");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_racStrAreaTJ = aCombinedT->m_racStrAreaTJSav;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // move to next entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aEntIntJ = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferJ, aCombinedT->m_aEntIntJ);");

  StrLib::vAppendString(&m_azCodeCrs, "\n   goto L_IndexJoinDeref;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the index scan                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthIndexScan(IndexDef* pIndexDef, Stz* sIndexScanExpr, Stz* sLabelNext, u8 uSelectColumns) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
  s256 sCopy;
  s256 sCastRow;
  s256 sInfix;

  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // INDEXSCAN-OP ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // generate the infix from eStencilFrom
  vToInfix(m_pStreamDefLft, m_pStreamDefOut, (char *) sIndexScanExpr, m_eStencilFrom, m_eStencilTo, nullptr, PJ_NONE, false);
  RASSERT(m_pLowerStack->nValues() == 1 || m_pUpperStack->nValues() == 1);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // local variables

  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 iIndexScanBuffer;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 iIndexScanItem;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void* aIndexScanCrw;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 i_BaseOffsetDataT;");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the indexScan label

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n L_IndexScan:");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // do we have a lower bound ?

  if (m_pLowerStack->nValues() == 0) {

    // position at beginning of the index
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // position at beginning of the index");
    StrLib::vAppendString(&m_azCodeCrs,
                          "\n    aCombinedT->m_aEntIntT =  (struct Ent_int*)aIndexTree_begin(aCombinedT->m_aIndexTreeT, &aCombinedT->m_aEntBufferT);");

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_bTrace)");
    StrLib::vAppendString(&m_azCodeCrs, "\n      vEntWrk_print(aCombinedT->m_aEntWrkT, aCombinedT->m_aIndexDefT);");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  } else {

    // populate the lower bound key
    if (pIndexDef->eKeyKind()) {

      TypedCell *pTypedCell = m_pLowerStack->aPeekItem(0);
      RASSERT(pTypedCell->eType() == CN_PTRVAL);
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();

      RASSERT(pIndexDef->nColumns() == 1);

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n    // populate EntKeyT with lower bound value");

      sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKeyT = %s;"_Stz, sInfix.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    } else {
      StrLib::vAppendString(&m_azCodeCrs, "\nassert(0);");
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // print lower bound entry");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_bTrace)");
    StrLib::vAppendString(&m_azCodeCrs, "\n      vEntWrk_print(aCombinedT->m_aEntWrkT, aCombinedT->m_aIndexDefT);");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // position at entry greater than equal to the lower bound key

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // position at lower bound entry");
    sCopy.vSprint(
        "\n    aCombinedT->m_aEntIntT =  (struct Ent_int *)%s(aCombinedT->m_aIndexTreeT, &aCombinedT->m_aEntBufferT, aCombinedT->m_aEntKeyT);"_Stz,
        m_sLowerPosition.azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lower bound found ?
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( ! aCombinedT->m_aEntIntT )");
  StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_Return;");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // do we have an upper bound ?

  if (m_pUpperStack->nValues() == 0) {
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n L_IndexScanDeref:");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if ( ! aCombinedT->m_aEntIntT ) {");
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 0;");
    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_Return;");
    StrLib::vAppendString(&m_azCodeCrs, "\n    }");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // process the row");
    StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  } else {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // populate the upper bound key
    if (pIndexDef->eKeyKind()) {

      TypedCell *pTypedCell = m_pUpperStack->aPeekItem(0);
      RASSERT(pTypedCell->eType() == CN_PTRVAL);
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();

      RASSERT(pIndexDef->nColumns() == 1);

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n    // populate EntKeyT with upper bound value");

      sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKeyT = %s;"_Stz, sInfix.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    } else {
      StrLib::vAppendString(&m_azCodeCrs, "\nassert(0);");
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // print upper bound entry");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_bTrace)");
    StrLib::vAppendString(&m_azCodeCrs, "\n      vEntWrk_print(aCombinedT->m_aEntWrkT, aCombinedT->m_aIndexDefT);");

    // find the upper bound entry
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // position at upper bound");
    sCopy.vSprint(
        "\n    aCombinedT->m_aEntUpperT =  (struct Ent_int *)%s(aCombinedT->m_aIndexTreeT, aCombinedT->m_aEntKeyT);"_Stz,
        m_sUpperPosition.azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n L_IndexScanDeref:");

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // reached upper bound ?");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_aEntIntT == aCombinedT->m_aEntUpperT) {");
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 0;");
    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_Return;");
    StrLib::vAppendString(&m_azCodeCrs, "\n    }");

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // process the row");
    StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");

  }

  // get the crw from the entry
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // dereference the entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iIndexScanBuffer = aCombinedT->m_aEntIntT->Rrf.m_uBuffer;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iIndexScanItem   = aCombinedT->m_aEntIntT->Rrf.m_uItem;");
  StrLib::vAppendString(&m_azCodeCrs,
                        "\n    aIndexScanCrw  = ( (char*)C_ADR_REGION + iIndexScanBuffer * C_1MB+ C_CRW_FIRSTHEAD+ iIndexScanItem * C_CRW_HEADBYTES );");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // inflate the derefenced entry");

  StrLib::vAppendString(&m_azCodeCrs, "\n    aHeadLine = (struct HeadLine_T *)aIndexScanCrw;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    auMetaCode = META_AREA(aHeadLine);");
  StrLib::vAppendString(&m_azCodeCrs, "\n    ayDstByte = (byte*)aCombinedT->m_aRowCellArrayT;");

  StrLib::vAppendString(&m_azCodeCrs, "\n    acStrArea = (char*)aCombinedT->m_racStrAreaT;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    racStrArea = (char*)aCombinedT->m_racStrAreaT;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    eStrInflateRule = SI_STRAREA_ADR;");

  sCopy.vSprint("\n    i_BaseOffsetDataT = C_1B  + C_8B  + %lld;"_Stz, m_pStreamDefLft->nColumns());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n    ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetDataT);");

  vGenerateInflate(m_pStreamDefLft, "L_RetT_ColIS"_Stz, 0, uSelectColumns);

  StrLib::vAppendString(&m_azCodeCrs, "\n    aHeadLine = aHeadLineSav;");
  sCopy.vSprint("\n\n    goto %s;"_Stz, sLabelNext->azChar());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs,"\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n L_AdvanceNextRow:");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n     // move to next entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n     aCombinedT->m_aEntIntT = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferT, aCombinedT->m_aEntIntT);");

  StrLib::vAppendString(&m_azCodeCrs, "\n     goto L_IndexScanDeref;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");


  // cleanup
  m_pExprStack->vReset();

}

/*----------------------------------------------------------------------------*/
/* synthesize the inflate                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthInflate(Stz *sTableNm, Stz *sLabel, Stz *sReturnLabel, u8 *puSelectColumns) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCopy;

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // INFLATE ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n  %s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // process the row");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_racStrAreaT = aCombinedT->m_racStrAreaTSav;");

  i8 iHighestBit = BitLib::iHighestBit(*puSelectColumns);

  for (i8 i_Col = 0; i_Col < m_pStreamDefLft->nColumns(); i_Col++) {

    if (i_Col > iHighestBit) {
      StrLib::vSprintCat((Stz *) m_caCode, "\n\n    aSubReturnLabel = && %s;"_Stz, sReturnLabel);
      break;
    }

    StrLib::vSprintCat((Stz *) m_caCode, "\n"_Stz);
    StrLib::vSprintCat((Stz *) m_caCode, "\n    // - - - - - - - - - - - -  "_Stz);
    StrLib::vSprintCat((Stz *) m_caCode, "\n    i_Col = %lld;"_Stz, i_Col);
    StrLib::vSprintCat((Stz *) m_caCode, "\n    uMetaCode = *auMetaCode++;"_Stz);
    StrLib::vSprintCat((Stz *) m_caCode, "\n"_Stz);
    StrLib::vSprintCat((Stz *) m_caCode, "\n    uHigh = uMetaCode >> 4;"_Stz);
    StrLib::vSprintCat((Stz *) m_caCode, "\n    uLow  = uMetaCode & 0x0F;"_Stz);

    if (i_Col < m_pStreamDefLft->nColumns() - 1) {
      StrLib::vSprintCat((Stz *) m_caCode, "\n\n    aSubReturnLabel = && L_Ret%s_Col%lld;"_Stz, sTableNm, i_Col + 1);
      if (BitLib::bIsOn(*puSelectColumns, i_Col))
        StrLib::vSprintCat((Stz *) m_caCode, "\n    goto L_InflateAColumn;"_Stz);
      else {
        StrLib::vSprintCat((Stz *) m_caCode, "\n    ayDstByte += 8;"_Stz);
        StrLib::vSprintCat((Stz *) m_caCode, "\n    goto L_SkipAColumn;"_Stz);
      }
      StrLib::vSprintCat((Stz *) m_caCode, "\n\n    L_Ret%s_Col%lld:"_Stz, sTableNm, i_Col + 1);
    } else {
      StrLib::vSprintCat((Stz *) m_caCode, "\n\n    aSubReturnLabel = && %s;"_Stz, sReturnLabel);
      if (BitLib::bIsOn(*puSelectColumns, i_Col))
        StrLib::vSprintCat((Stz *) m_caCode, "\n    goto L_InflateAColumn;"_Stz);
      else {
        StrLib::vSprintCat((Stz *) m_caCode, "\n    ayDstByte += 8;"_Stz);
        StrLib::vSprintCat((Stz *) m_caCode, "\n    goto L_SkipAColumn;"_Stz);
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* synthesize the projection                                                  */
/*----------------------------------------------------------------------------*/
  void CodeSynth::vSynthOrdAgg(IndexDef *pIndexDef,ePJ eProjectKind,
                               IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext,
                               Stz* sLabelLoop, bool bHashRows) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
    s256 sCopy;

    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // ORDAGG-OP ");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

    // generate the infix from eStencilFrom
    //vTrace("Postfix: %s\n"_Stz, sProjectExpr);
    vToInfix(m_pStreamDefLft, m_pStreamDefOut, (char *) sProjectExpr, m_eStencilFrom, m_eStencilTo, pAggTypeStack, eProjectKind,
             bHashRows);
    RASSERT(m_pExprStack->nValues() == m_pStreamDefOut->nColumns());

    // code for allocating a row in the tub
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    i8 iKind;");
    StrLib::vAppendString(&m_azCodeCrs, "\n    i8   iMixIn;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ProjectRow
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    sCopy.vSprint("\n%s:"_Stz, sLabel);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if ( ! aCombinedT->m_iProcessRow )");
    sCopy.vSprint("\n      goto %s;"_Stz, sLabelLoop);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    switch (m_eStencilFrom) {
      case SR_IT:
        sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                      xRowRef[m_eStencilFrom].m_szInclude);
        break;
      case SR_PT:
        if (eProjectKind == PJ_EXP)
          sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayP;\n"_Stz,
                        xRowRef[m_eStencilFrom].m_szInclude,
                        xRowRef[m_eStencilFrom].m_szInclude);
        else
          sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz,
                        xRowRef[m_eStencilFrom].m_szInclude,
                        xRowRef[m_eStencilFrom].m_szInclude);
        break;
      case SR_TJ:
        sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                      xRowRef[m_eStencilFrom].m_szInclude);
        break;
      case SR_IJ:
      case SR_PJ:
      default:
        NEVER_REACH;
    }

    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    RASSERT(eProjectKind == PJ_GBY);

    vOrdAggGby(pIndexDef, pAggTypeStack, sLabelNext, sLabelLoop);

    // cleanup
    m_pExprStack->vReset();
  }

/*----------------------------------------------------------------------------*/
/* synthesize the projection                                                  */
/*----------------------------------------------------------------------------*/
bool CodeSynth::bSynthProject(IndexDef *pIndexDef, ePJ eProjectKind, IntStack *pAggTypeStack,
                              Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop, bool bHashRows) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
  s256 sCopy;
  bool bProjectResult = true;

  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // PROJECT-OP ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  // generate the infix from eStencilFrom
  vToInfix(m_pStreamDefLft, m_pStreamDefOut, (char *) sProjectExpr, m_eStencilFrom, m_eStencilTo, pAggTypeStack, eProjectKind,
           bHashRows);
  if (m_pExprStack->nValues() != m_pStreamDefOut->nColumns()) {
    for (i8 i_Val = 0; i_Val < m_pExprStack->nValues(); i_Val++) {
      TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Val);
      vTrace("**%s\n"_Stz, pTypedCell->azChar());
    }
  }
  //RASSERT(m_pExprStack->nValues() == pStreamDefOut->nColumns());

  // code for allocating a row in the tub
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 iKind;");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    void *aEntFound;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8   iMixIn;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  //StrLib::vAppendString(&m_azCodeCrs, "\n    static i8 iProjectInit = 0;");


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n%s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  switch (m_eStencilFrom) {
    case SR_IT:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                    xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_PT:
      if (eProjectKind == PJ_EXP)
        sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayP;\n"_Stz,
                      xRowRef[m_eStencilFrom].m_szInclude,
                      xRowRef[m_eStencilFrom].m_szInclude);
      else
        sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz,
                      xRowRef[m_eStencilFrom].m_szInclude,
                      xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_TJ:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                    xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_IJ:
    case SR_PJ:
    default:
      NEVER_REACH;
  }

  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  if (eProjectKind == PJ_EXP) {

    StrLib::vAppendString(&m_azCodeCrs, "\n   if (!aCombinedT->m_iProcessRow)");
    sCopy.vSprint("\n    goto %s;\n"_Stz, sLabelLoop);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    vProjectExp();

    sCopy.vSprint("\n    goto %s;"_Stz, sLabelNext);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  } else if (eProjectKind == PJ_AGG) {
    bProjectResult = bProjectAgg( pAggTypeStack);

    sCopy.vSprint("\n    goto %s;"_Stz, sLabelLoop);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  } else {
    StrLib::vAppendString(&m_azCodeCrs, "\n   if (!aCombinedT->m_iProcessRow)");
    sCopy.vSprint("\n    goto %s;\n"_Stz, sLabelLoop);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    RASSERT(eProjectKind == PJ_GBY);
    vProjectGby(pIndexDef, pAggTypeStack);

    sCopy.vSprint("\n    goto %s;"_Stz, sLabelLoop);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  }



  // cleanup
  m_pExprStack->vReset();
  m_pInitStack->vReset();

  return bProjectResult;
}

/*----------------------------------------------------------------------------*/
/* synthesize the row inflate                                                 */
/*----------------------------------------------------------------------------*/
  void CodeSynth::vSynthRowInflate(void) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

    s256 sCopy;

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // ROW INFLATE ");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    i8 i_Row = -1;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // for each row
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\nL_AdvanceNextRow:");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // process the row");
    StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    i_Row++;");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (i_Row  >= aCombinedT->m_nRows)");
    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_Return;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // get the next row (should be able to add rowsize)");
    StrLib::vAppendString(&m_azCodeCrs,
                          "\n    aCombinedT->m_aRowCellArrayT = aRowBuffer_GetCellArray(aCombinedT->m_aRowBuffer, i_Row);");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // convert string offsets to string pointers
    StrLib::vAppendString(&m_azCodeCrs, "\n    piColumn = (i8*)aCombinedT->m_aRowCellArrayT;");

    for (i8 i_Col = 0; i_Col < m_pStreamDefLft->nColumns(); i_Col++) {
      Column *pColumn = m_pStreamDefLft->pGetColumn(i_Col);
      if (pColumn->eColumnType() == CT_VARCHAR ||
          pColumn->eColumnType() == CT_CHAR) {

        sCopy.vSprint("\n    iOffset = piColumn[%lld];"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n    amCellAddress = (m8*)&piColumn[%lld];"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n    piColumn[%lld] = ADDR_ADD(i8, amCellAddress, iOffset);"_Stz, i_Col, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        StrLib::vAppendString(&m_azCodeCrs, "\n");
      }
    }

  }

/*----------------------------------------------------------------------------*/
/* synthesize the small aggregate                                             */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthSmlAgg(IndexDef *pIndexDef, ePJ eProjectKind,
                             IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel,
                             Stz* sLabelLoop, bool bHashRows) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
  s256 sCopy;

  RASSERT(eProjectKind == PJ_GBY);

  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // SMLAGG-OP ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  // generate the infix from eStencilFrom
  //vTrace("Postfix: %s\n"_Stz, sProjectExpr);
  vToInfix(m_pStreamDefLft, m_pStreamDefOut, (char *) sProjectExpr, m_eStencilFrom, m_eStencilTo, pAggTypeStack, eProjectKind,
           bHashRows);
  RASSERT(m_pExprStack->nValues() == m_pStreamDefOut->nColumns());

  // code for allocating a row in the tub
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8 iKind;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i8   iMixIn;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");


  switch (m_eStencilFrom) {
    case SR_IT:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                    xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_PT:
      if (eProjectKind == PJ_EXP)
        sCopy.vSprint("\n    p%s[%lld] = (struct %s*)aCombinedT->m_aRowCellArrayP;\n"_Stz,
                      xRowRef[m_eStencilFrom].m_szInclude,
                      xRowRef[m_eStencilFrom].m_szInclude);
      else
        sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz,
                      xRowRef[m_eStencilFrom].m_szInclude,
                      xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_TJ:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                    xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_IJ:
    case SR_PJ:
    default:
      NEVER_REACH;
  }

  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n%s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( ! aCombinedT->m_iProcessRow )");
  sCopy.vSprint("\n      goto %s;"_Stz, sLabelLoop);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // find the key in the tub
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // find key in tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    bFound = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    for (iIndex = 0; iIndex < aCombinedT->m_nAggRows; iIndex++) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      pSR_PT[iIndex] = ADDR_ADD(struct SR_PT*, aCombinedT->m_aRowTub , -1*iIndex*aCombinedT->m_iTubRowSize);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  for (i8 i_iCol=0; i_iCol < pIndexDef->nColumns(); i_iCol++) {
    Column* pColumn = pIndexDef->pGetColumn(i_iCol);
    switch(pColumn->eColumnType()) {
      case CT_VARCHAR:
        sCopy.vSprint("\n      iOffset = p%s[iIndex]->%s;"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm(),
                      xRowRef[m_eStencilFrom].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      amCellAddress = (m8*)&p%s[iIndex]->%s;"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm(),
                      xRowRef[m_eStencilFrom].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        StrLib::vAppendString(&m_azCodeCrs, "\n      szSrcStz = ADDR_ADD(char*, amCellAddress, iOffset);");
        StrLib::vAppendString(&m_azCodeCrs, "\n      bFound = 1;");
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        //sCopy.vSprint("\n      if (0 != strcmp(szSrcStz, (char*)p%s->%s))"_Stz,
        //              xRowRef[eStencilFrom].m_szInclude, pColumn->szColNm());
        //StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());


        sCopy.vSprint("\n      for (i8 i=0; i< %lld; i++) {"_Stz, pColumn->iColumnLength()+1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        char* keyStz = (char*)(p%s->%s);"_Stz,
                      xRowRef[m_eStencilFrom].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        StrLib::vAppendString(&m_azCodeCrs, "\n        if (szSrcStz[i] == '\\0' || szSrcStz[i] != keyStz[i]) {");

        StrLib::vAppendString(&m_azCodeCrs, "\n          bFound = 0;");
        StrLib::vAppendString(&m_azCodeCrs, "\n          break;");
        StrLib::vAppendString(&m_azCodeCrs, "\n        }");
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        StrLib::vAppendString(&m_azCodeCrs, "\n      if ( !bFound)");
        StrLib::vAppendString(&m_azCodeCrs, "\n        continue;");
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;
      default:
        NEVER_REACH;
    }
  }
  StrLib::vAppendString(&m_azCodeCrs, "\n      bFound = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n      break;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      if (bFound) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n        // key found");

  vSmlAggLoad(pAggTypeStack);

  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n    else {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // key not found");

  vSmlAggStore(pAggTypeStack);

  StrLib::vAppendString(&m_azCodeCrs, "\n");


  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

  StrLib::vAppendString(&m_azCodeCrs, "\n    goto L_AdvanceNextRow;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs,"\n L_Return:");
  sCopy.vSprint("\n    goto L_ReturnFinal;"_Stz);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
  void CodeSynth::vSynthStruct(u8 uSelectColumns) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

    StrLib::vSprint((Stz *) m_caCode, "\n\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "_Stz);
    switch (m_eStencilTo) {
      case SR_IJ:
        StrLib::vSprintCat((Stz *) m_caCode, "\n    // Inflate J-Table"_Stz);
        break;
      case SR_IT:
        StrLib::vSprintCat((Stz *) m_caCode, "\n    // Inflate T-Table"_Stz);
        break;
      case SR_PJ:
        StrLib::vSprintCat((Stz *) m_caCode, "\n    // Project J-Table"_Stz);
        break;
      case SR_PT:
        StrLib::vSprintCat((Stz *) m_caCode, "\n    // Project T-Table"_Stz);
        break;
      case SR_TJ:
        StrLib::vSprintCat((Stz *) m_caCode, "\n    // IndexJoin TJ-Table"_Stz);
        break;
      default:
        break;
    }
    StrLib::vSprintCat((Stz *) m_caCode, "\n\n    struct %s { \n"_Stz, xRowRef[m_eStencilTo]);

    for (i8 i_Col = 0; i_Col < m_pStreamDefOut->nColumns(); i_Col++) {

      if (uSelectColumns) {
        if (!BitLib::bIsOn(uSelectColumns, i_Col))
          continue;
      }
      Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
      s256 sColNm;
      sColNm = pColumn->szColNm();
      StrLib::vToUpper((char *) sColNm.azChar());
      StrLib::vSprintCat((Stz *) m_caCode, "      i8 %s;\n"_Stz, sColNm.azChar());
    }

    StrLib::vSprintCat((Stz *) m_caCode, "    };"_Stz);
    StrLib::vSprintCat((Stz *) m_caCode, "\n    struct %s* p%s;\n"_Stz, xRowRef[m_eStencilTo], xRowRef[m_eStencilTo]);
  }

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthStructArray( u8 uSelectColumns) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  StrLib::vSprint((Stz *) m_caCode, "\n\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "_Stz);
  switch (m_eStencilTo) {
    case SR_IJ:
      StrLib::vSprintCat((Stz *) m_caCode, "\n    // Inflate J-Table"_Stz);
      break;
    case SR_IT:
      StrLib::vSprintCat((Stz *) m_caCode, "\n    // Inflate T-Table"_Stz);
      break;
    case SR_PJ:
      StrLib::vSprintCat((Stz *) m_caCode, "\n    // Project J-Table"_Stz);
      break;
    case SR_PT:
      StrLib::vSprintCat((Stz *) m_caCode, "\n    // Project T-Table"_Stz);
      break;
    case SR_TJ:
      StrLib::vSprintCat((Stz *) m_caCode, "\n    // IndexJoin TJ-Table"_Stz);
      break;
    default:
      break;
  }
  StrLib::vSprintCat((Stz *) m_caCode, "\n\n    struct %s { \n"_Stz, xRowRef[m_eStencilTo]);

  for (i8 i_Col = 0; i_Col < m_pStreamDefOut->nColumns(); i_Col++) {

    if (uSelectColumns) {
      if (!BitLib::bIsOn(uSelectColumns, i_Col))
        continue;
    }
    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper((char *) sColNm.azChar());
    StrLib::vSprintCat((Stz *) m_caCode, "      i8 %s;\n"_Stz, sColNm.azChar());
  }

  StrLib::vSprintCat((Stz *) m_caCode, "    };"_Stz);
  StrLib::vSprintCat((Stz *) m_caCode, "\n    struct %s* p%s[256];\n"_Stz, xRowRef[m_eStencilTo], xRowRef[m_eStencilTo]);
}

/*----------------------------------------------------------------------------*/
/* return source code                                                         */
/*----------------------------------------------------------------------------*/
  char *CodeSynth::azCode(void) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

    return m_caCode;
  }

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
  void CodeSynth::vReset(void) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

    MemLib::vZero(m_caCode, sizeof(m_caCode), CN_CODESYNTH);
  }

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate code for aggregate initialization                                 */
/*----------------------------------------------------------------------------*/
  void CodeSynth::vGenerateAggInitCode(IntStack *pAggTypeStack) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

    s256 sCopy;
    s256 sInfix;
    i8 nColumns = m_pStreamDefOut->nColumns();
    i8 iAggColumn = 0;
    i8 iAggregates = 0;

    // HACK
    // count the aggregates
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
      eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);
      if (eAggType != AG_NONE)
        iAggregates++;
    }

    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

      StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
      Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
      s256 sColNm;
      sColNm = pColumn->szColNm();
      StrLib::vToUpper(sColNm.azChar());

      eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);
      i8 i_Project = nColumns - i_Col - 1;

      // get the infix for the projection
      TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *Tkn = _pToken(pTypedCell->pObj());
        sInfix = Tkn->szChars();
      } else {
        sInfix = pTypedCell->szStz();
      }

      TypedCell *pInitCell;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // initialization of aggregate columns
      switch (eAggType) {

        case AG_ALL:
        case AG_CNT:
        case AG_MIN:
        case AG_MAX:
        case AG_SUM:
          // get the init value for the aggregate
          pInitCell = m_pInitStack->aPeekItem(iAggregates - iAggColumn - 1);
          sCopy.vSprint("\n      // init agg column %lld"_Stz, i_Col);
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

          sCopy.vSprint("\n      p%s->%s = %lld;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                        pInitCell->iInt());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          iAggColumn++;
          break;

        default:
          sCopy.vSprint("\n     p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                        sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          break;
      }
    }

  }

/*----------------------------------------------------------------------------*/
/* generate code for assigning values                                         */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateAssign(u8 uSelectColumns) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCopy;
  i8 nColumns = m_pStreamDefLft->nColumns();

  // for each column of row of left table T
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    // skip columns in join not needed
    if (BitLib::bIsOff(uSelectColumns, i_Col))
      continue;

    Column *pColumn = m_pStreamDefLft->pGetColumn(i_Col);
    Column *pJoinColumn = m_pStreamDefLft->pGetColumn(i_Col);

    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper((char *) sColNm.azChar());

    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n    // copy column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
    StrLib::vAppendString(&m_azCodeCrs, "\n    ");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n    // get the source string");
      sCopy.vSprint("\n    szSrcStz = (char*)p%s->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    nSrcSize = strlen(szSrcStz) + 1;");

      StrLib::vAppendString(&m_azCodeCrs, "\n    // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n    szDstStz = (char*)aCombinedT->m_racStrAreaTJ;");

      StrLib::vAppendString(&m_azCodeCrs, "\n    // copy the source to the destination");
      StrLib::vAppendString(&m_azCodeCrs, "\n    strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      sCopy.vSprint("\n    p%s->%s = (i8)szDstStz;"_Stz, xRowRef[m_eStencilTo].m_szInclude, pJoinColumn->szColNm());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n    // update the tub string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_racStrAreaTJ += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types
      sCopy.vSprint("\n    p%s->%s = p%s->%s;\n"_Stz,
                    xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                    xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* generate code for inflate                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateInflate(StreamDef *pStreamDef, Stz* sReturnPrefix, i8 nColumnsBefore, u8 uSelectColumns) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCopy;
  i8 iHighestBit = BitLib::iHighestBit(uSelectColumns);

  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++) {

    if (i_Col + nColumnsBefore > iHighestBit)
      break;

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - -  ");
    sCopy.vSprint("\n    i_Col = %lld;"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n    uMetaCode = *auMetaCode++;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    uHigh = uMetaCode >> 4;");
    StrLib::vAppendString(&m_azCodeCrs, "\n    uLow  = uMetaCode & 0x0F;");

    sCopy.vSprint("\n\n    aSubReturnLabel = && %s%lld;"_Stz, sReturnPrefix, i_Col + 1);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    // skip columns in join not needed
    if (BitLib::bIsOn(uSelectColumns, i_Col + nColumnsBefore)) {
      StrLib::vAppendString(&m_azCodeCrs, "\n    goto L_InflateAColumn;");
    } else {
      if (nColumnsBefore)
        StrLib::vAppendString(&m_azCodeCrs, "\n    ayDstByte += 8;");
      StrLib::vAppendString(&m_azCodeCrs, "\n    goto L_SkipAColumn;");
    }
    sCopy.vSprint("\n\n    %s%lld:"_Stz, sReturnPrefix, i_Col + 1);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }
}

/*----------------------------------------------------------------------------*/
/* generate code for Project aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vFindKey(IndexDef* pIndexDef, IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // find the key in the indexTree

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // find key in payload index");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aEntFound = aIndexTree_findEntry(aCombinedT->m_aIndexTreeGby, aCombinedT->m_aEntKeyPL);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( !aEntFound ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // key not found");
  vPayloadStore(pIndexDef, pAggTypeStack);

  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n    else {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // key found");
  vPayloadAgg(pIndexDef, pAggTypeStack);
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

}

/*----------------------------------------------------------------------------*/
/* get key value                                                              */
/*----------------------------------------------------------------------------*/
i8 CodeSynth::iGetValue(eDM eDatum, char* sValue) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  i8 iKeyValue;

  switch (eDatum) {
    case DM_DTE: {
      SN_Dte snDate;
      iKeyValue = snDate.i(&sValue, AM::pFmt());
    }
      break;
    case DM_TME: {
      SN_Tme snTime;
      iKeyValue = snTime.i(&sValue, AM::pFmt());
    }
      break;
    case DM_TSP: {
      SN_Tsp snTsp;
      iKeyValue = snTsp.i(&sValue, AM::pFmt());
    }
      break;
    default:
      NEVER_REACH;
  }
  return iKeyValue;
}

/*----------------------------------------------------------------------------*/
/* generate code for ordered aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vOrdAggGby(IndexDef* pIndexDef, IntStack* pAggTypeStack, Stz* sLabelNext, Stz* sLabelLoop) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 nColumns = m_pStreamDefOut->nColumns();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  StrLib::vAppendString(&m_azCodeCrs,"\n");
  sCastRow.vSprint("\n     p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude,
                   xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate aggregate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( aCombinedT->m_iProjectInit == 0 ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // allocate a projectRow from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProjectInit = 1;");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate init code for each projected agg column
  vGenerateAggInitCode(pAggTypeStack);

  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // is it a new key ?

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper((char*)sColNm.azChar());

    // NOT SURE: COME BACK
    Column *pColumnOut = m_pStreamDefOut->pGetColumn(i_Col);
    s256 sColOutNm;
    sColOutNm = pColumnOut->szColNm();
    StrLib::vToUpper((char*)sColOutNm.azChar());

    if (pIndexDef->eKeyKind()) {
      sCopy.vSprint("\n   if (p%s->%s == p%s->%s) { "_Stz,
                    xRowRef[m_eStencilTo].m_szInclude, sColOutNm.azChar(),
                    xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n");
    }
    else {

      // not tested
      //RASSERT(0);

      if (pColumn->eColumnType() == CT_VARCHAR ||
          pColumn->eColumnType() == CT_CHAR) {
        sCopy.vSprint("\n    if (strcmp(p%s->%s, p%s->%s) == 0)"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, sColOutNm.azChar(),
                      xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
      }
      else {
        sCopy.vSprint("\n   if (p%s->%s == p%s->%s) { "_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, sColOutNm.azChar(),
                      xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
      }
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    i8 i_Project = nColumns - i_Col - 1;

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper((char*)sColNm.azChar());

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregation code for the column
    switch (eAggType) {

      case AG_SUM:
        StrLib::vAppendString(&m_azCodeCrs, "\n     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        sCopy.vSprint("\n     // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");

        RASSERT(pColumn->eColumnType() != CT_VARCHAR);
        RASSERT(pColumn->eColumnType() != CT_CHAR);

        sCopy.vSprint("\n     p%s->%s += %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                      sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_CNT:
        StrLib::vAppendString(&m_azCodeCrs, "\n     // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        sCopy.vSprint("\n     // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");

        RASSERT(pColumn->eColumnType() != CT_VARCHAR);
        RASSERT(pColumn->eColumnType() != CT_CHAR);

        sCopy.vSprint("\n     p%s->%s++;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                      sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      default:
        break;
    }
  }
  sCopy.vSprint("\n     goto %s;"_Stz, sLabelLoop);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  StrLib::vAppendString(&m_azCodeCrs, "\n   else {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n     aCombinedT->m_iProjectInit = 0;");

  // HACK!!! If no having filter then we need to allot a row
  //if (sLabel->bSame(sLabelNext))
  //  StrLib::vAppendString(&m_azCodeCrs, "\n\n   iAllocRow = 1;");

  sCopy.vSprint("\n     goto %s;"_Stz, sLabelNext);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");

  // cleanup
  m_pExprStack->vReset();
  m_pInitStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* generate code for creation of payload key                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadKey(IndexDef* pIndexDef, eSR eStencilFrom) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCopy;

  //StrLib::vAppendString(&m_azCodeCrs, "\n    printf(\"PAYLOAD KEY\\n\");");
  //StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the payload key");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // reset the key size / m_nElmSeqSize
  StrLib::vAppendString(&m_azCodeCrs, "    aCombinedT->m_aEntIntPL->m_iIntElm0 = 0;");

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper(sColNm.azChar());

    if (pIndexDef->eKeyKind()) {
        sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKeyPL = p%s->%s;"_Stz, xRowRef[eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
    else {
      if (pColumn->eColumnType() == CT_VARCHAR ||
          pColumn->eColumnType() == CT_CHAR) {
        sCopy.vSprint("\n    vEntWrk_appendElementEntStz(aCombinedT->m_aEntWrkPL, (char*)p%s->%s);"_Stz,
                      xRowRef[eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }
      else {
        sCopy.vSprint("\n    vEntWrk_appendElementEntInt(aCombinedT->m_aEntWrkPL, p%s->%s);"_Stz,
                      xRowRef[eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }
    }

  }
  StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_bTrace)");
  StrLib::vAppendString(&m_azCodeCrs, "\n      vEntWrk_print(aCombinedT->m_aEntWrkPL, aCombinedT->m_aIndexDefGby);");
}

/*----------------------------------------------------------------------------*/
/* generate code for payload store for groupby                                */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadStore(IndexDef* pIndexDef, IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;
  s256 sCopy;
  i8   i_Col;

  i8 nColumns = m_pStreamDefOut->nColumns();

  // create the payload

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // PAYLOAD STORE");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // create the payload
  i8 nPayloadBytes = m_pStreamDefOut->nColumns() * 8;
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // create the payload");
  sCopy.vSprint("\n      vEnt_payloadCreate(aCombinedT->m_aEntIntPL, %lld);"_Stz, nPayloadBytes);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());


  //StrLib::vAppendString(&m_azCodeCrs, "\n      printf(\"PAYLOAD STORE\\n\");");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // insert key in the payload");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aEntFound = aIndexTree_insertEnt(aCombinedT->m_aIndexTreeGby, aCombinedT->m_aEntIntPL);");

  // calculate the payload address
  if (pIndexDef->eKeyKind()) {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiPayload = (i8 *) ((char *) aEntFound");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + 8);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiPayload = (i8 *) ((char *) aEntFound");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + ((struct Ent_int*)aEntFound)->m_iIntElm0");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + 8);");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // populate the payload");
  i_Col = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n       // - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n       // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // initialization of aggregate columns
    switch(eAggType) {

      case AG_SUM:
        RASSERT ((pColumn->eColumnType() != CT_VARCHAR) &&
                 (pColumn->eColumnType() != CT_CHAR));
        sCopy.vSprint("\n       // init agg_sum column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n       aCombinedT->m_aiPayload[%lld] = %s;"_Stz, i_Col, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_CNT:
        sCopy.vSprint("\n       aCombinedT->m_aiPayload[%lld] = 1;"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_ALL:
        sCopy.vSprint("\n       aCombinedT->m_aiPayload[%lld] = 1;"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      default:
        if ((pColumn->eColumnType() == CT_VARCHAR) ||
            (pColumn->eColumnType() == CT_CHAR)) {
          sCopy.vSprint("\n       nSrcSize = strlen((char*)%s);"_Stz, sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

          sCopy.vSprint("\n       strncpy((char*)&aCombinedT->m_aiPayload[%lld],(char*)%s,nSrcSize);"_Stz, i_Col,
                        sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        } else {
          sCopy.vSprint("\n       aCombinedT->m_aiPayload[%lld] = %s;"_Stz, i_Col, sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        }
        break;
    }

    i_Col++;
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for small agg store for groupby                              */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSmlAggLoad(IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;
  s256 sCopy;
  i8   i_Col;

  i8 nColumns = m_pStreamDefOut->nColumns();

  // create the payload

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n        // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n        // AGGREGATE STORE");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs,sCopy.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  i_Col = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // initialization of aggregate columns
    switch(eAggType) {

      case AG_SUM:
        RASSERT ((pColumn->eColumnType() != CT_VARCHAR) &&
                 (pColumn->eColumnType() != CT_CHAR));
        sCopy.vSprint("\n        // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        // agg_sum column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        p%s[iIndex]->%s += %s;"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      case AG_CNT:
        sCopy.vSprint("\n        // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        // agg_cnt column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        p%s[iIndex]->%s++;"_Stz, xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      case AG_ALL:
        sCopy.vSprint("\n        // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        // agg_cntall column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        p%s[iIndex]->%s++;"_Stz, xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      default:
        break;
    }

    i_Col++;
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for small agg store for groupby                              */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSmlAggStore(IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;
  s256 sCopy;
  i8   i_Col;

  i8 nColumns = m_pStreamDefOut->nColumns();

  // create the payload

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // AGGREGATE STORE");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("\n      p%s[aCombinedT->m_nAggRows] = (struct %s*)aTub_AllotRow(aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea);"_Stz,
                xRowRef[m_eStencilTo].m_szInclude, xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n      if (aCombinedT->m_nAggRows == 0)");
  sCopy.vSprint("\n         aCombinedT->m_aRowTub = p%s[aCombinedT->m_nAggRows];"_Stz, xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  i_Col = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // initialization of aggregate columns
    switch(eAggType) {

      case AG_SUM:
        RASSERT ((pColumn->eColumnType() != CT_VARCHAR) &&
                 (pColumn->eColumnType() != CT_CHAR));
        sCopy.vSprint("\n       // init agg_sum column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n       p%s[aCombinedT->m_nAggRows]->%s = %s;"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      case AG_CNT:
        sCopy.vSprint("\n       p%s[aCombinedT->m_nAggRows]->%s = 1;"_Stz, xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      case AG_ALL:
        sCopy.vSprint("\n       p%s[aCombinedT->m_nAggRows]->%s = 1;"_Stz, xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      default:
        if ((pColumn->eColumnType() == CT_VARCHAR) ||
            (pColumn->eColumnType() == CT_CHAR)) {
          sCopy.vSprint("\n       nSrcSize = strlen((char*)%s) + 1;"_Stz, sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

          StrLib::vAppendString(&m_azCodeCrs, "\n");
          StrLib::vAppendString(&m_azCodeCrs, "\n       // get the destination string");
          StrLib::vAppendString(&m_azCodeCrs, "\n       szDstStz = aCombinedT->m_racTubStrArea;");
          StrLib::vAppendString(&m_azCodeCrs, "\n");
          sCopy.vSprint("\n       strncpy(szDstStz, (char*)%s,nSrcSize);"_Stz, sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n");

          StrLib::vAppendString(&m_azCodeCrs, "\n       // copy the source to the destination");
          sCopy.vSprint("\n       amCellAddress = (m8*) &p%s[aCombinedT->m_nAggRows]->%s;"_Stz, xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n       iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);");


          sCopy.vSprint("\n       p%s[aCombinedT->m_nAggRows]->%s = iOffset;"_Stz, xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n");
          StrLib::vAppendString(&m_azCodeCrs, "\n       // update the tub string size");
          StrLib::vAppendString(&m_azCodeCrs, "\n       aCombinedT->m_racTubStrArea += nSrcSize;");
          StrLib::vAppendString(&m_azCodeCrs, "\n       vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);");
          StrLib::vAppendString(&m_azCodeCrs, "\n");
        } else {
          sCopy.vSprint("\n           p%s[aCombinedT->m_nAggRows]->%s = %s;"_Stz, xRowRef[m_eStencilTo].m_szInclude, pColumn->szColNm(), sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n");
        }
        break;
    }

    i_Col++;
  }
  StrLib::vAppendString(&m_azCodeCrs, "\n         aCombinedT->m_nAggRows++;");

}

/*----------------------------------------------------------------------------*/
/* generate code for payload aggregates for groupby                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadAgg(IndexDef* pIndexDef, IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;
  s256 sCopy;

  i8 nColumns = m_pStreamDefOut->nColumns();

  // create the payload

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // PAYLOAD AGGREGATE");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  if (pIndexDef->eKeyKind()) {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiPayload = (i8 *) ((char *) aEntFound");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + 8);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiPayload = (i8 *) ((char *) aEntFound");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + ((struct Ent_int*)aEntFound)->m_iIntElm0");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + 8);");

  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  for (i8 i_Project = nColumns - 1, i_Col = 0; i_Project >= 0; i_Project--, i_Col++) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregate columns
    switch(eAggType) {

      case AG_SUM:
        StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - -  ");
        sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");

        sCopy.vSprint("\n      // agg_sum column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      aCombinedT->m_aiPayload[%lld] += %s;\n"_Stz, i_Col, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_CNT:
        StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - -  ");
        sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");

        sCopy.vSprint("\n      // agg_cnt column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      aCombinedT->m_aiPayload[%lld]++;\n"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_ALL:
        StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - -  ");
        sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");

        sCopy.vSprint("\n      // agg_cntall column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      aCombinedT->m_aiPayload[%lld]++;\n"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      default:
        //StrLib::vAppendString(&m_azCodeCrs, "\nprintf(\"PAYLOAD AGG KEY: %%lld\\n\",aCombinedT->m_aiPayload[1]);");
        break;
    }
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for processing EOF                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadToBuffer(IntStack* pAggTypeStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 nColumns = m_pStreamDefOut->nColumns();
  i8 i_Aggr = m_pAggrStack->nValues() - 1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n      p%s = (struct %s*)aCombinedT->m_aPayloadRow;\n"_Stz,xRowRef[m_eStencilTo].m_szInclude, xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    s256 sColumnNm;
    sColumnNm = pColumn->szColNm();
    StrLib::vToUpper(sColumnNm.azChar());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      sCopy.vSprint("\n      szSrcStz = (char*)&aCombinedT->m_aiPayload[%lld];"_Stz, i_Col);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      sCopy.vSprint("\n      nSrcSize = strlen((char*)&aCombinedT->m_aiPayload[%lld]) + 1;"_Stz, i_Col);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs,"\n      szDstStz = aCombinedT->m_racTubStrArea;");
      StrLib::vAppendString(&m_azCodeCrs,"\n      strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      sCopy.vSprint("\n      amCellAddress = (m8*) &p%s->%s;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColumnNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n      iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);");
      sCopy.vSprint("\n      p%s->%s = iOffset;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColumnNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_racTubStrArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n      vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");
    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types

      sCopy.vSprint("\n      p%s->%s = aCombinedT->m_aiPayload[%lld];\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColumnNm.azChar(), i_Col);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }

    eAG eAggType = (eAG)pAggTypeStack->iPeek(i_Col);

    if (eAggType != AG_NONE) {

      // get the aggregate expression for the projection
      TypedCell *pTypedCell = m_pAggrStack->aPeekItem(i_Aggr--);

      RASSERT(pTypedCell->eType() != CN_PTRVAL);
      StrLib::vAppendString(&m_azCodeCrs, "\n      ");
      StrLib::vAppendString(&m_azCodeCrs, (char *) pTypedCell->szStz());
      StrLib::vAppendString(&m_azCodeCrs, ";\n");

    }
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for processing EOF                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadTraversal(IndexDef* pIndexDef,IntStack* pAggTypeStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate ProjectRow
  //StrLib::vAppendString(&m_azCodeCrs, "\n    printf(\"PAYLOAD TRAVERSAL\\n\");");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // PAYLOAD TRAVERSAL");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // begin iterating the payload index
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aEntIntPL = (struct Ent_int*)aIndexTree_begin(aCombinedT->m_aIndexTreeGby, &aCombinedT->m_aEntBufferPL);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n    while (aCombinedT->m_aEntIntPL) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // allocate a projectRow from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aPayloadRow = (byte*)aTub_AllotRow(aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // calculate the payload address
  if (pIndexDef->eKeyKind()) {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiPayload = (i8 *) ((char *) aCombinedT->m_aEntIntPL");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + 8);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiPayload = (i8 *) ((char *) aCombinedT->m_aEntIntPL");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + ((struct Ent_int*)aCombinedT->m_aEntIntPL)->m_iIntElm0");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + 8);");
  }

  vPayloadToBuffer(pAggTypeStack);

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n     aCombinedT->m_aEntIntPL = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferPL, aCombinedT->m_aEntIntPL);");

  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
}

/*----------------------------------------------------------------------------*/
/* generate code for Project Aggregates                                       */
/*----------------------------------------------------------------------------*/
bool CodeSynth::bProjectAgg(IntStack* pAggTypeStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 nColumns = m_pStreamDefOut->nColumns();

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCastRow.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz,xRowRef[m_eStencilTo].m_szInclude, xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate aggregate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( aCombinedT->m_iProjectInit == 0 ) {");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate init code for each projected agg column
  vGenerateAggInitCode(pAggTypeStack);

  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProjectInit = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_iProcessRow) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iRowsProcessed++;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    i8 i_Project = nColumns - i_Col - 1;

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper(sColNm.azChar());

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregation code for the column
    switch(eAggType) {

      case AG_ALL:
        // ALKA: COME BACK TO CHECK FOR NULLS
        sCopy.vSprint("\n       p%s->%s++;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_CNT:
        // ALKA: COME BACK TO CHECK FOR NULLS
        sCopy.vSprint("\n       p%s->%s++;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_MIN:
        // ALKA: COME BACK TO CHECK FOR NULLS
        sCopy.vSprint("\n      if ( %s < p%s->%s) {\n"_Stz,  sInfix.azChar(), xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n        p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_MAX:
        // ALKA: COME BACK TO CHECK FOR NULLS
        sCopy.vSprint("\n      if ( %s > p%s->%s) {\n"_Stz,  sInfix.azChar(), xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n        p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_SUM:
        RASSERT(pColumn->eColumnType() != CT_VARCHAR);
        RASSERT(pColumn->eColumnType() != CT_CHAR);

        sCopy.vSprint("\n     p%s->%s += %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      default:
        vTrace("Invalid non-aggregate column with aggregates (no group by)\n"_Stz);
        return false;
        break;
    }
  }
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

  return true;
}

/*----------------------------------------------------------------------------*/
/* generate code for Project Expressions                                      */
/*----------------------------------------------------------------------------*/
void CodeSynth::vProjectExp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s256 sCopy;
  i8 i_Col;
  i8 nColumns = m_pStreamDefOut->nColumns();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n   aCombinedT->m_racStrAreaP = aCombinedT->m_racStrAreaSavP;");
  sCastRow.vSprint("\n   p%s = (struct %s*)aCombinedT->m_aRowCellArrayP;\n"_Stz,xRowRef[m_eStencilTo].m_szInclude,
                   xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column
  i_Col = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // get the infix for the projection
    TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Project);

    if (pTypedCell->eType() == CN_PTRVAL) {
      Token *Tkn = _pToken(pTypedCell->pObj());
      sInfix = Tkn->szChars();
    } else {
      sInfix = pTypedCell->szStz();
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n   // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col++);

    // convert column name to uppercase
    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper((char*)sColNm.azChar());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n   // get the source string");
      sCopy.vSprint("\n   szSrcStz = (char*)%s;"_Stz, sInfix.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n   nSrcSize = strlen(szSrcStz) + 1;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n   // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n   szDstStz = aCombinedT->m_racStrAreaP;");
      StrLib::vAppendString(&m_azCodeCrs, "\n   strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n   // copy the source to the destination");
      sCopy.vSprint("\n   p%s->%s = (i8)szDstStz;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n   // update  tub string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n   aCombinedT->m_racStrAreaP += nSrcSize;");

      StrLib::vAppendString(&m_azCodeCrs, "\n");
    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types
      sCopy.vSprint("\n   p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* generate code for Project aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vProjectGby(IndexDef* pIndexDef, IntStack* pAggTypeStack) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the payload key
  vPayloadKey(pIndexDef, m_eStencilFrom);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the key in the indexTree
  vFindKey(pIndexDef, pAggTypeStack);
}

/*----------------------------------------------------------------------------*/
/* add the init aggregate                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vInitAgg(eAG eAggType, eDM eExprType)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s32 sInitValue;
  i8  iInitValue = 0;

  switch(eAggType) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_ALL:
      m_pInitStack->vPushInt(0);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_CNT:
      m_pInitStack->vPushInt(0);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_MIN:
      switch(eExprType) {
        case DM_VCH:
          // get the min value
          vMathOp_MaxValue(sInitValue);
          m_pInitStack->vPushStr(sInitValue);
          break;

        default:
          iInitValue = iMathOp_MaxValue(eExprType);
          m_pInitStack->vPushInt(iInitValue);
          break;
      }
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_MAX:
      switch(eExprType) {
        case DM_VCH:
          // get the min value
          vMathOp_MinValue(sInitValue);
          m_pInitStack->vPushStr(sInitValue);
          break;

        default:
          iInitValue = iMathOp_MinValue(eExprType);
          m_pInitStack->vPushInt(iInitValue);
          break;
      }
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_SUM:
      RASSERT(eExprType != DM_VCH &&
              eExprType != DM_DTE &&
              eExprType != DM_IVL &&
              eExprType != DM_IVY &&
              eExprType != DM_TME &&
              eExprType != DM_TSP);

          m_pInitStack->vPushInt(0);
      break;

    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* convert postfix to infix                                                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vToInfix(StreamDef* pStreamDefLft, StreamDef* pStreamDefOut, char* sExpr,
                         eSR eStencilFrom, eSR eStencilTo, IntStack* pAggTypeStack, ePJ eProjectKind,
                         bool bHashRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sInfix;
  s4K sOperand1;
  s4K sOperand2;
  s4K sStrCmp;
  s4K sOperation;

  s256 sColumn;
  s256 sColumnOut;
  s256 sStructColumn;
  s256 sValue;

  i8 iValue;

  eDM eExprType = DM_UNK;
  eAG eAggType  = AG_NONE;

  bool bIndexJoin = false;
  bool bLowerBound = false;
  bool bUpperBound = false;

  TypedCell *pTypedCell;
  Stz* sOrdinalColumn;

  m_pTokenVec = TokenVec::pTokenise((Stz*)sExpr);
  Token* pTkn;
  i8 i_Col = 0;

  TypedStack* pExprStack = m_pExprStack;
  bool bStringOperand = false;

  for (i8 i_Tkn = 0; i_Tkn < m_pTokenVec->nItems(); i_Tkn++) {

    pTkn = m_pTokenVec->pGet(i_Tkn);

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame("groupby"_Stz)) {
      break;
    }

    // bound
    if (pTkn->bSame("bound"_Stz)) {
      bIndexJoin = true;
      continue;
    }

    // lbound
    if (pTkn->bSame("lbound"_Stz)) {
      pExprStack = m_pLowerStack;
      bLowerBound = true;
      bUpperBound = false;
      continue;
    }

    // ubound
    if (pTkn->bSame("ubound"_Stz)) {
      pExprStack = m_pUpperStack;
      bUpperBound = true;
      bLowerBound = false;
      continue;
    }

    // skip $I
    if (pTkn->szChars()->bContains("$"_Stz)) {

      // skip index column
      if (pTkn->szChars()->bContains("I"_Stz)) {
        continue;
      }

      // ignore if column ordinal relative to the group by key
      if (pTkn->szChars()->bContains("K"_Stz))
        continue;

      // ignore right columns
      if (pTkn->szChars()->bContains("R"_Stz)) {
        continue;
      }

      i8 i_ColumnOrdinal = C_UNKNOWN;
      // column ordinal relative to the left join table
      if (pTkn->szChars()->bContains("L"_Stz)) {
        // get the column ordinal
        sOrdinalColumn = pTkn->szChars() + 2;
        i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

        // look for the column in the streamDefLft
        i8 nColumns = pStreamDefLft->nColumns();
        if (i_ColumnOrdinal > nColumns) {
          RASSERT(0);
          break;
        }

        // select left column
        vFormColumn(sColumn, pStreamDefLft, eStencilFrom, i_ColumnOrdinal);
        pExprStack->vPushStz(sColumn);

        // hack!!!
        // break out of the loop once column name found
        RASSERT(bIndexJoin);
        break;
      }
      else {
        // get the column ordinal from symbolic column name
        sOrdinalColumn = pTkn->szChars() + 1;
        i8 i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

        // look for the column in the streamDefLft
        i8 nColumns = pStreamDefLft->nColumns();
        if (i_ColumnOrdinal > nColumns) {
          RASSERT(0);
          break;
        }

        // select left column
        vFormColumn(sColumn, pStreamDefLft, eStencilFrom, i_ColumnOrdinal);
        pExprStack->vPushStz(sColumn);
        continue;
      }

    }

    if (pTkn->bSame("type"_Stz)) {
      // get the data type
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      eExprType = StreamDefBase::eDatum(pTkn->szChars());
      continue;
    }

    if (pTkn->bSame("as"_Stz)) {
      // skip the alias
      i_Tkn++;
      continue;
    }

    if (pTkn->bSame(","_Stz)) {
      // skip the comma
      i_Col++;
      pExprStack = m_pExprStack;
      continue;
    }

    if (pTkn->bSame(")"_Stz)) {
      // aggregate follows
      eAggType = (eAG)pAggTypeStack->iPeek(i_Col);
      vInitAgg(eAggType, eExprType);
      continue;
    }

    if (pTkn->bSame("count"_Stz)) {
      eAggType = AG_NONE;
      if (!m_pAggrStack)
        m_pAggrStack  = new TypedStack(KK_DATA_STACK);
      pExprStack = m_pAggrStack;
      sColumnOut = pStreamDefOut->pGetColumn(i_Col)->szColNm();
      StrLib::vToUpper(sColumnOut.azChar());
      if (eProjectKind == PJ_AGG) {
        if (bHashRows)
          sStructColumn.vSprint("p%s_pot->%s = p%s_pot->%s"_Stz,
                                xRowRef[eStencilTo], sColumnOut.azChar(),
                                xRowRef[eStencilTo], sColumnOut.azChar());
        sStructColumn.vSprint("p%s_tub->%s = p%s_tub->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      else {
        RASSERT(eProjectKind == PJ_GBY)
        sStructColumn.vSprint("p%s->%s = p%s->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      pExprStack->vPushStr(sStructColumn);
      continue;
    }

    if (pTkn->bSame("countall"_Stz)) {
      eAggType = AG_NONE;
      // push a dummy pointer for asserts
      // we assert that for every projection there is a value in the expr stack
      m_pExprStack->vPushPtr(pTkn);
      if (!m_pAggrStack)
        m_pAggrStack  = new TypedStack(KK_DATA_STACK);
      pExprStack = m_pAggrStack;
      sColumnOut = pStreamDefOut->pGetColumn(i_Col)->szColNm();
      StrLib::vToUpper(sColumnOut.azChar());
      if (eProjectKind == PJ_AGG) {
        if (bHashRows)
          sStructColumn.vSprint("p%s_pot->%s = p%s_pot->%s"_Stz,
                                xRowRef[eStencilTo], sColumnOut.azChar(),
                                xRowRef[eStencilTo], sColumnOut.azChar());
        sStructColumn.vSprint("p%s_tub->%s = p%s_tub->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      else {
        RASSERT(eProjectKind == PJ_GBY)
        sStructColumn.vSprint("p%s->%s = p%s->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      pExprStack->vPushStr(sStructColumn);
      continue;
    }

    if (pTkn->bSame("min"_Stz)) {
      eAggType = AG_NONE;
      if (!m_pAggrStack)
        m_pAggrStack  = new TypedStack(KK_DATA_STACK);
      pExprStack = m_pAggrStack;
      sColumnOut = pStreamDefOut->pGetColumn(i_Col)->szColNm();
      StrLib::vToUpper(sColumnOut.azChar());
      if (eProjectKind == PJ_AGG) {
        if (bHashRows)
          sStructColumn.vSprint("p%s_pot->%s = p%s_pot->%s"_Stz,
                                xRowRef[eStencilTo], sColumnOut.azChar(),
                                xRowRef[eStencilTo], sColumnOut.azChar());
        sStructColumn.vSprint("p%s_tub->%s = p%s_tub->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      else {
        RASSERT(eProjectKind == PJ_GBY)
        sStructColumn.vSprint("p%s->%s = p%s->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      pExprStack->vPushStr(sStructColumn);
      continue;
    }

    if (pTkn->bSame("max"_Stz)) {
      eAggType = AG_NONE;
      if (!m_pAggrStack)
        m_pAggrStack  = new TypedStack(KK_DATA_STACK);
      pExprStack = m_pAggrStack;
      sColumnOut = pStreamDefOut->pGetColumn(i_Col)->szColNm();
      StrLib::vToUpper(sColumnOut.azChar());
      if (eProjectKind == PJ_AGG) {
        if (bHashRows)
          sStructColumn.vSprint("p%s_pot->%s = p%s_pot->%s"_Stz,
                                xRowRef[eStencilTo], sColumnOut.azChar(),
                                xRowRef[eStencilTo], sColumnOut.azChar());
        sStructColumn.vSprint("p%s_tub->%s = p%s_tub->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      else {
        RASSERT(eProjectKind == PJ_GBY)
        sStructColumn.vSprint("p%s->%s = p%s->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      pExprStack->vPushStr(sStructColumn);
      continue;
    }

    if (pTkn->bSame("sum"_Stz)) {
      eAggType = AG_NONE;
      if (!m_pAggrStack)
        m_pAggrStack  = new TypedStack(KK_DATA_STACK);
      pExprStack = m_pAggrStack;
      sColumnOut = pStreamDefOut->pGetColumn(i_Col)->szColNm();
      StrLib::vToUpper(sColumnOut.azChar());
      if (eProjectKind == PJ_AGG) {
        if (bHashRows)
        sStructColumn.vSprint("p%s_pot->%s = p%s_pot->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
        sStructColumn.vSprint("p%s_tub->%s = p%s_tub->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      else {
        RASSERT(eProjectKind == PJ_GBY)
        sStructColumn.vSprint("p%s->%s = p%s->%s"_Stz,
                              xRowRef[eStencilTo], sColumnOut.azChar(),
                              xRowRef[eStencilTo], sColumnOut.azChar());
      }
      pExprStack->vPushStr(sStructColumn);
      continue;
    }

    if (pTkn->bIsNumeric()) {
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("negate"_Stz) ) {
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bIsDecimalNo()) {
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("DATE"_Stz) ) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      iValue = iGetValue(DM_DTE, pTkn->azChars());
      pTkn->vUpdateChars((Stz*)StrLib::wToString(iValue, sValue.azChar()), m_pTokenVec);
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("TIME"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      iValue = iGetValue(DM_TME, pTkn->azChars());
      pTkn->vUpdateChars((Stz*)StrLib::wToString(iValue, sValue.azChar()), m_pTokenVec);
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("TIMESTAMP"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      iValue = iGetValue(DM_TSP, pTkn->azChars());
      pTkn->vUpdateChars((Stz*)StrLib::wToString(iValue, sValue.azChar()), m_pTokenVec);
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    // operator ?
    if ( (pTkn->bIsArithmetic() || pTkn->bIsLogical() || pTkn->bIsRelational()) ) {

      if (bLowerBound || bUpperBound) {

        RASSERT(pTkn->bIsRelational());

        if (pTkn->bSame(">"_Stz)) {
          RASSERT(bLowerBound);
          m_sLowerPosition = "aIndexTree_positionGT"_Stz;
        }
        else if (pTkn->bSame(">="_Stz)) {
          RASSERT(bLowerBound);
          m_sLowerPosition = "aIndexTree_positionGE"_Stz;
        }
        else if (pTkn->bSame("<"_Stz)) {
          RASSERT(bUpperBound);
          m_sUpperPosition = "aIndexTree_positionLT"_Stz;
        }
        else if (pTkn->bSame("<="_Stz)) {
          RASSERT(bUpperBound);
          m_sUpperPosition = "aIndexTree_positionLE"_Stz;
        }
        else {
          RASSERT(0);
        }

        continue;
      }
      if (pTkn->bIsString() ) {
        pExprStack->vPushPtr(pTkn);
        continue;
      }

      // pop operand1
      pTypedCell = pExprStack->aPeekItem(0);
      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *pOperand1 = _pToken(pExprStack->pPopPtr(__func__));
        sOperand1 = pOperand1->szChars();
      }
      else {
        pExprStack->vPopStr(sOperand1, __func__);
      }

      // pop operand2
      pTypedCell = pExprStack->aPeekItem(0);
      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *pOperand2 = _pToken(pExprStack->pPopPtr(__func__));
        sOperand2 = pOperand2->szChars();
      }
      else {
        pExprStack->vPopStr(sOperand2, __func__);
      }

      if (pTkn->bSame("AND"_Stz))
        pTkn->vChars(" && "_Stz);
      else
      if (pTkn->bSame("OR"_Stz))
        pTkn->vChars(" || "_Stz);
      else
      if (pTkn->bSame("NOT"_Stz))
        pTkn->vChars(" ! "_Stz);

      // combine the two operands and operator
      // operand2 op operand1

      if (bStringOperand) {
        // assert tkn is ==

        StrLib::vSprint(sStrCmp,"(strcmp((char*)%s,\"%s\") == 0) "_Stz, sOperand2.azChar(), sOperand1.azChar());
        // push new infix string on the TknVec
        pExprStack->vPushStr(sStrCmp);
        bStringOperand = false;
      }
      else {
        sOperation = "("_Stz;
        sOperation.vConcat(sOperand2);
        sOperation.vConcat((Stz *) pTkn->azChars());
        sOperation.vConcat(sOperand1);
        sOperation.vConcat(")"_Stz);
        // push new infix string on the TknVec
        pExprStack->vPushStr(sOperation);
      }


    }
    else {
      // save the column name
      if (pTkn->bIsString()) {
        sStructColumn = pTkn->szChars();
        bStringOperand = true;
      }
      else {
        StrLib::vToUpper(pTkn->azChars());
        sColumn = pTkn->szChars();
        sStructColumn.vSprint("p%s->%s"_Stz, xRowRef[eStencilFrom], pTkn->szChars());
      }
      pExprStack->vPushStr(sStructColumn);

      // hack!!!
      // break out of the loop once column name found
      if (bIndexJoin)
        break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* allocate row in the pot                                                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthAllocPotRow(IntStack* pAggTypeStack, Stz* sHashExpr, ePJ eProjectKind, bool bEosHandler,
                                  Stz* sLabel, Stz* sLabelNext, Stz* sReturnLabel) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCastRow;
  s256 sCopy;
  s256 sInfix;
  i8 i_Aggr = m_pAggrStack ? m_pAggrStack->nValues() - 1 : C_UNKNOWN;

  StrLib::vAppendString(&m_azCodeCrs,"\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n   // POT ");
  StrLib::vAppendString(&m_azCodeCrs,"\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  // generate inFix from postFix
  vToInfix(m_pStreamDefOut, nullptr, (char*)sHashExpr, m_eStencilFrom, SR_NONE, nullptr, PJ_NONE, true);
  //RASSERT(m_pExprStack->nValues() == 1);

  // convert postfix to inFix
  TypedCell* pTypedCell = m_pExprStack->aPeekItem(0);
  if (pTypedCell->eType() == CN_PTRVAL) {
    Token *Tkn = _pToken(m_pExprStack->pPopPtr(__func__));
    sInfix = Tkn->szChars();
  }
  else {
    m_pExprStack->vPopStr(sInfix, __func__);
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n   struct %s* p%s_pot;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("\n %s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs,sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  if ( ! bEosHandler) {
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (!aCombinedT->m_iProcessRow)");
    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_AdvanceNextRow;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  switch(m_eStencilFrom) {
    case SR_IT:
      sCastRow.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                       xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_PT:
      if (eProjectKind == PJ_EXP)
        sCastRow.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayP;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                         xRowRef[m_eStencilFrom].m_szInclude);
      else
        sCastRow.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                         xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_TJ:
      sCastRow.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                       xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_IJ:
    case SR_PJ:
    default:
      NEVER_REACH;
  }
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("\n   iMixIn = %s;"_Stz, sInfix.azChar());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate row from the pot
  StrLib::vAppendString(&m_azCodeCrs, "\n   // allocate a row from the pot");
  StrLib::vAppendString(&m_azCodeCrs, "\n   aCombinedT->m_aRowPot = (byte*)aPot_AllotRow(aCombinedT->m_pPot,iMixIn,&aCombinedT->m_racPotStrArea);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n   p%s_pot = (struct %s*)aCombinedT->m_aRowPot;\n"_Stz,xRowRef[m_eStencilFrom].m_szInclude,
                   xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column
  for (i8 i_Col = 0; i_Col < m_pStreamDefOut->nColumns(); i_Col++) {

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper(sColNm.azChar());

    StrLib::vAppendString(&m_azCodeCrs, "\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n   // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n   // get the source string");
      sCopy.vSprint("\n   szSrcStz = (char*)p%s->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n   nSrcSize = strlen(szSrcStz) + 1;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n   // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n   szDstStz = aCombinedT->m_racPotStrArea;");
      StrLib::vAppendString(&m_azCodeCrs, "\n   strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n   // copy the source to the destination");
      sCopy.vSprint("\n   amCellAddress = (m8*) &p%s_pot->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n   iOffset = - nDifference(aCombinedT->m_racPotStrArea,amCellAddress);");
      sCopy.vSprint("\n   p%s_pot->%s = iOffset;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n   // update the pot string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n   aCombinedT->m_racPotStrArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n   vPot_RowStrSize(aCombinedT->m_pPot,nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");
    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types
      sCopy.vSprint("\n   p%s_pot->%s = p%s->%s;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                    sColNm.azChar(), xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }

    eAG eAggType = (eAG)pAggTypeStack->iPeek(i_Col);

    if (eAggType != AG_NONE) {

      // get the aggregate expression for the projection
      TypedCell *pTypedCell = m_pAggrStack->aPeekItem(i_Aggr--);

      RASSERT(pTypedCell->eType() != CN_PTRVAL);
      StrLib::vAppendString(&m_azCodeCrs, "\n   ");
      StrLib::vAppendString(&m_azCodeCrs, (char *) pTypedCell->szStz());
      StrLib::vAppendString(&m_azCodeCrs, ";\n");

    }
  }

  if (sLabelNext->bSame("L_AdvanceNextRow"_Stz)) {
    StrLib::vAppendString(&m_azCodeCrs,"\n");
    sCopy.vSprint("\n    goto %s;"_Stz, sLabelNext);
    StrLib::vAppendString(&m_azCodeCrs,sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs,"\n");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs,"\n");
    sCopy.vSprint("\n    goto %s;"_Stz, sLabelNext);
    StrLib::vAppendString(&m_azCodeCrs,sCopy.azChar());
  }

  if ( ! sReturnLabel->bSame(""_Stz) ) {
    StrLib::vAppendString(&m_azCodeCrs,"\n L_Return:");
    sCopy.vSprint("\n    goto %s;"_Stz, sReturnLabel);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }
}

/*----------------------------------------------------------------------------*/
/* allocate row in tub                                                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthAllocTubRow(IntStack* pAggTypeStack, ePJ eProjectKind, bool bEosHandler,
                                  Stz* sLabel, Stz* sLabelNext, Stz* sReturnLabel) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCastRow;
  s256 sCopy;
  i8 i_Aggr = m_pAggrStack ? m_pAggrStack->nValues() - 1 : C_UNKNOWN;

  StrLib::vAppendString(&m_azCodeCrs,"\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs,"\n   // TUB ");
  StrLib::vAppendString(&m_azCodeCrs,"\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n   struct %s* p%s_tub;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("\n %s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs,sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  if (bEosHandler) {
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n     if (!aCombinedT->m_iRowsProcessed)");
    StrLib::vAppendString(&m_azCodeCrs, "\n       goto L_ReturnFinal;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n     if (!aCombinedT->m_iProcessRow)");
    StrLib::vAppendString(&m_azCodeCrs, "\n       goto L_AdvanceNextRow;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  switch(m_eStencilFrom) {
    case SR_IT:
      sCastRow.vSprint("\n     p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                       xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_PT:
      if (eProjectKind == PJ_EXP) {
        sCastRow.vSprint("\n     p%s = (struct %s*)aCombinedT->m_aRowCellArrayP;\n"_Stz,
                         xRowRef[m_eStencilFrom].m_szInclude,
                         xRowRef[m_eStencilFrom].m_szInclude);
      }
      else
        sCastRow.vSprint("\n     p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                         xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_TJ:
      sCastRow.vSprint("\n     p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                       xRowRef[m_eStencilFrom].m_szInclude);
      break;
    case SR_IJ:
    case SR_PJ:
    default:
      NEVER_REACH;
  }

  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate row from the tub
  StrLib::vAppendString(&m_azCodeCrs, "\n   // allocate a row from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n   aCombinedT->m_aRowTub = (byte*)aTub_AllotRow(aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n   p%s_tub = (struct %s*)aCombinedT->m_aRowTub;\n"_Stz,xRowRef[m_eStencilFrom].m_szInclude,
                   xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column
  for (i8 i_Col = 0; i_Col < m_pStreamDefOut->nColumns(); i_Col++) {

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    s256 sColNm;
    sColNm = pColumn->szColNm();
    StrLib::vToUpper(sColNm.azChar());

    StrLib::vAppendString(&m_azCodeCrs, "\n   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n   // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n   // get the source string");
      sCopy.vSprint("\n   szSrcStz = (char*)p%s->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n   nSrcSize = strlen(szSrcStz) + 1;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n   // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n   szDstStz = aCombinedT->m_racTubStrArea;");
      StrLib::vAppendString(&m_azCodeCrs, "\n   strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n   // copy the source to the destination");
      sCopy.vSprint("\n   amCellAddress = (m8*) &p%s_tub->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n   iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);");
      sCopy.vSprint("\n   p%s_tub->%s = iOffset;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n   // update the tub string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n   aCombinedT->m_racTubStrArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n   vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");
    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types
      sCopy.vSprint("\n   p%s_tub->%s = p%s->%s;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                    sColNm.azChar(), xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }

    // In case of only filter there is no AggTypeStack
    eAG eAggType = pAggTypeStack ? (eAG)pAggTypeStack->iPeek(i_Col) : AG_NONE;

    if (eAggType != AG_NONE) {

      // get the aggregate expression for the projection
      TypedCell *pTypedCell = m_pAggrStack->aPeekItem(i_Aggr--);

      RASSERT(pTypedCell->eType() != CN_PTRVAL);
      StrLib::vAppendString(&m_azCodeCrs, "\n   ");
      StrLib::vAppendString(&m_azCodeCrs, (char *) pTypedCell->szStz());
      StrLib::vAppendString(&m_azCodeCrs, ";\n");

    }
  }

  if (sLabelNext->bSame("L_AdvanceNextRow"_Stz)) {
    StrLib::vAppendString(&m_azCodeCrs,"\n");
    sCopy.vSprint("\n    goto %s;"_Stz, sLabelNext);
    StrLib::vAppendString(&m_azCodeCrs,sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs,"\n");
    //StrLib::vAppendString(&m_azCodeCrs,"\n L_Return:");
    //StrLib::vAppendString(&m_azCodeCrs,"\n    goto L_ReturnFinal;");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs,"\n");
    sCopy.vSprint("\n    goto %s;"_Stz, sLabelNext);
    StrLib::vAppendString(&m_azCodeCrs,sCopy.azChar());
  }

  if ( ! sReturnLabel->bSame(""_Stz) ) {
    StrLib::vAppendString(&m_azCodeCrs,"\n L_Return:");
    sCopy.vSprint("\n    goto %s;"_Stz, sReturnLabel);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
