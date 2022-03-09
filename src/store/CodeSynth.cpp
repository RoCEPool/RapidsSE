
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
#include "SN_Dte.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Bol.hpp"
#include "SN_Flt.hpp"
#include "SN_Bcd.hpp"
#include "eFM.h"

extern i8 iMathOp_MaxValue(eDM eDatum);
extern i8 iMathOp_MinValue(eDM eDatum);

extern void vMathOp_MaxValue(Stz* sInitValue);
extern void vMathOp_MinValue(Stz* sInitValue);

RowRef xRowRef[]
{
  { "none"_Stz },   // SR_NONE

  {"SR_AT"_Stz},    // SR_AT
  {"SR_IJ"_Stz},    // SR_IJ
  {"SR_IT"_Stz},    // SR_IT
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
  m_pNullStack  = new TypedStack(KK_DATA_STACK);
  m_pGrpByStack = new TypedStack(KK_DATA_STACK);
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack = nullptr;
  m_pStreamDefLft = nullptr;
  m_pStreamDefOut = nullptr;
  m_pIndexDef     = nullptr;
  m_eStencilFrom  = SR_NONE;
  m_eStencilTo    = SR_NONE;

  m_iLimitRows    = 0;

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
  m_pNullStack  = new TypedStack(KK_DATA_STACK);
  m_pGrpByStack = new TypedStack(KK_DATA_STACK);
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack = nullptr;
  m_pStreamDefLft = nullptr;
  m_pStreamDefOut = pStreamDefRht;
  m_pIndexDef     = nullptr;
  m_eStencilFrom  = SR_NONE;
  m_eStencilTo    = eStencilTo;

  m_iLimitRows    = 0;

  _checkInitialised_(CN_CODESYNTH);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CodeSynth::CodeSynth(StreamDef* pStreamDefLft, StreamDef* pStreamDefRht, IndexDef* pIndexDef,
                     IntStack* pPreExpStack, eSR eStencilFrom, eSR eStencilTo)
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
  m_pNullStack  = new TypedStack(KK_DATA_STACK);
  m_pGrpByStack = new TypedStack(KK_DATA_STACK);
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack = pPreExpStack;
  m_pStreamDefLft = pStreamDefLft;
  m_pStreamDefOut = pStreamDefRht;
  m_pIndexDef     = pIndexDef;
  m_eStencilFrom  = eStencilFrom;
  m_eStencilTo    = eStencilTo;

  m_iLimitRows    = 0;

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
  delete m_pNullStack;
  delete m_pGrpByStack;
  delete m_pLowerStack;
  delete m_pUpperStack;

  delete m_pInitStack;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vMakeColumnNm(StreamDef* pStreamDef, Stz* sColumn, eSR eStencilRow, i8 i_Column)
{
  _static_

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  // convert column name to uppercase
  s256 sColNm;
  sColNm = pColumn->szColNm();
  StrLib::vToUpper((char*)sColNm.azChar());

  // form the column name
  sColumn->vSprint("p%s->%s.m_iAsI8"_Stz, xRowRef[eStencilRow], sColNm.azChar());
}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vMakeColumnNm(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
{
  _static_

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  if (pStreamDef->bIsDuplicateName(pColumn))
    sColNm.vSprint("%s%lld"_Stz, pColumn->szColNm(), i_DupCtr++);
  else
    sColNm = pColumn->szColNm();

  StrLib::vToUpper((char *) sColNm.azChar());
}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  if (pStreamDef->bIsDuplicateName(pColumn))
    sColNm.vSprint("%s%lld.m_iAsI8"_Stz, sUpper.azChar(), i_DupCtr++);
  else
    sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());

}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vMakeColumnNmStruct_O(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  if (pColumn->eColumnType() == CT_VARCHAR ||
      pColumn->eColumnType() == CT_CHAR) {
    if (pStreamDef->bIsDuplicateName(pColumn))
      sColNm.vSprint("%s%lld.m_iOffset"_Stz, sUpper.azChar(), i_DupCtr++);
    else
      sColNm.vSprint("%s.m_iOffset"_Stz, sUpper.azChar());
  }
  else {
    if (pStreamDef->bIsDuplicateName(pColumn))
      sColNm.vSprint("%s%lld.m_iAsI8"_Stz, sUpper.azChar(), i_DupCtr++);
    else
      sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());
  }

}
/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vMakeIdxColNmStruct(IndexDef* pIndexDef, s256& sColNm, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the IndexDef
  Column* pColumn = pIndexDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());

}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vMakeIdxColNmStruct_O(IndexDef* pIndexDef, s256& sColNm, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the IndexDef
  Column* pColumn = pIndexDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  if (pColumn->eColumnType() == CT_VARCHAR ||
      pColumn->eColumnType() == CT_CHAR) {
    sColNm.vSprint("%s.m_iOffset"_Stz, sUpper.azChar());
  }
  else {
    sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());
  }

}

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
/* synthesize the aggregegate groupby operator                                */
/*----------------------------------------------------------------------------*/
TypedStack* CodeSynth::pSynthAggGby(Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop, bool bHashRows) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  TypedStack* pPostStack;

  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("AGGREG-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the stencil
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create post aggregation stack
  pPostStack = new TypedStack(KK_DATA_STACK);

  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the payload key");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aEntIntPL->m_iIntElm0 = 0;");

  // generate the expression code
  vGenerateExpr((char *) sProjectExpr, pPostStack, EK_AGGGBY, bHashRows, m_pStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow(sLabelLoop);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the project group by code
  vGeneratePrjGby(m_pIndexDef);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto label
  vGenerateGoto(sLabelLoop);

  // cleanup
  m_pExprStack->vReset();
  m_pInitStack->vReset();

  return pPostStack;
}

/*----------------------------------------------------------------------------*/
/* synthesize the EOS                                                         */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthEosPayload(IndexDef* pIndexDef, TypedStack* pPostStack) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate payload traversal
  vPayloadTraversal(pIndexDef, pPostStack);
}

/*----------------------------------------------------------------------------*/
/* synthesize the EOS                                                         */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthEosTop(IndexDef* pIndexDef) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate index traversal
  vTopIndexTraversal(pIndexDef);
}

/*----------------------------------------------------------------------------*/
/* synthesize the filter expression                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthFilter(Stz* sFilterExpr, bool bOrdAgg, Stz* sLabel, Stz* sLabelPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("FILTER-OP"_Stz, sLabel);

  // generate the code for the projected expressions
  vGenerateExpr((char *) sFilterExpr, nullptr, EK_FILTER, false, m_pStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  if ( ! sLabelPrev->bSame(""_Stz) )
    vGenerateProcessRow("sLabelPrev"_Stz);
  else
    vGenerateProcessRow("L_AdvanceNextRow"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for the expression
  vGenerateExprCode(m_pExprStack, ""_Stz);

  StrLib::vAppendString(&m_azCodeCrs, "\n    if (bFilterResult) { ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 1;");
  if (bOrdAgg)
    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_AddRowToTub;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    } else { ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto sLabelPrev for filter after ordered aggregate
  if ( ! sLabelPrev->bSame(""_Stz) )
    vGenerateGoto(sLabelPrev);

  // cleanup
  m_pExprStack->vReset();
}

//TODO: to go
/*----------------------------------------------------------------------------*/
/* synthesize the filter expression after an ordAgg op                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthFilterOrdAgg(Stz* sFilterExpr, Stz* sLabel, Stz* sLabelPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sInfix;
  s4K sCopy;

  // generate the init stub
  vGenerateInitStub("FILTER-OP (ORDAGG)"_Stz, sLabel);

  // generate inFix from postFix
  //vToInfix((char*)sFilterExpr, nullptr, EK_NONE, false, m_pStreamDefLft);
  RASSERT(m_pExprStack->nValues() == 1);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the infix for the column 0
  vGetInfix(sInfix, m_pExprStack, 0);

  sCopy.vSprint("\n   if ( %s ) {"_Stz, sInfix.azChar());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n    goto L_AddRowToTub;");
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto label
  vGenerateGoto(sLabelPrev);

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

  s4K sCopy;

  // generate the init stub
  vGenerateInitStub("INDEXJOIN-OP"_Stz, sLabel);

  // generate the code for the index join expr
  vGenerateExpr((char *) sJoinExpr, nullptr, EK_INDEXJOIN, false, m_pStreamDefLft);

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow("L_AdvanceNextRow"_Stz);

  if (m_pNullStack->nValues()) {

    StrLib::vAppendString(&m_azCodeCrs, "\n    // check for nulls ");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate code for the null checks
    vGenerateExprCode(m_pNullStack, ""_Stz);

    // check for nulls
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (bNullResult) {");
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 0;");
    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_AdvanceNextRow;");
    StrLib::vAppendString(&m_azCodeCrs, "\n    }");
    StrLib::vAppendString(&m_azCodeCrs, "\n");

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for the expression
  vGenerateExprCode(m_pExprStack, ""_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for join key expression
  vGenerateExprCode(m_pExprStack, ""_Stz);

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto label
  vGenerateGoto(sLabelNext);

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
void CodeSynth::vSynthIndexScan(Stz* sIndexScanExpr, Stz* sLabel,
                                Stz* sLabelNext,u8 uSelectColumns) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  // generate the init stub
  vGenerateInitStub("INDEXSCAN-OP"_Stz, sLabel);

  StrLib::vAppendString(&m_azCodeCrs, "\n    i_CurrentRow = -1;");

  // generate the code for the index scan expr
  vGenerateExpr((char *) sIndexScanExpr, nullptr, EK_INDEXSCAN, false, m_pStreamDefLft);

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

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the lower bound expr
    vGenerateExprCode(m_pLowerStack, ""_Stz);

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
  StrLib::vAppendString(&m_azCodeCrs, "\n");

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
    StrLib::vAppendString(&m_azCodeCrs, "\n    // process the row");
    StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  } else {

     StrLib::vAppendString(&m_azCodeCrs, "\n    // populate EntKeyT with upper bound value");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the upper bound expr
    vGenerateExprCode(m_pUpperStack, ""_Stz);

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto label
  vGenerateGoto(sLabelNext);

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
/* synthesize the inflate of a crw row                                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthInflateCrw(Stz *sTableNm, Stz *sLabel, Stz *sReturnLabel, u8 *puSelectColumns) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("INFLATE-CRW"_Stz, sLabel);

  StrLib::vAppendString(&m_azCodeCrs, "\n    i_CurrentRow = -1;");

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
      StrLib::vSprintCat((Stz *) m_caCode, "\n\n  L_Ret%s_Col%lld:"_Stz, sTableNm, i_Col + 1);
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
/* synthesize inflate of a row of a row buffer                                */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthInflateRow(Stz* sLabel) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  // generate the init stub
  vGenerateInitStub("INFLATE-ROW"_Stz, sLabel);

  StrLib::vAppendString(&m_azCodeCrs, "\n    i_CurrentRow = -1;");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each row
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\nL_AdvanceNextRow:");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // process the row");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    i_CurrentRow++;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if (i_CurrentRow  >= aCombinedT->m_nRows)");
  StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_Return;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // get the next row (should be able to add rowsize)");
  StrLib::vAppendString(&m_azCodeCrs,
                        "\n    aCombinedT->m_aRowCellArrayT = aRowBuffer_GetCellArray(aCombinedT->m_aRowBuffer, i_CurrentRow);");

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
/* synthesize the keyExits expression with filter                             */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthKeyExists(StreamDef* pStreamDefRht, IndexDef* pIndexDefJ,
                                Stz* sFilterExpr, Stz* sKeyExpr,
                                Stz* sLabel, u8 uSelectColumns)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sInfix;
  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("KEYEXISTS-OP"_Stz, sLabel);

  // generate inFix from postFix for the key expr
  eSR eStencilFrom = m_eStencilFrom;
  m_eStencilFrom = m_eStencilTo;

  // generate the code for the key expression
  vGenerateExpr((char *) sKeyExpr, nullptr, EK_INDEXJOIN, false, m_pStreamDefLft);
  m_eStencilFrom = eStencilFrom;

  StrLib::vAppendString(&m_azCodeCrs,"\n");
  sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;"_Stz, xRowRef[m_eStencilTo], xRowRef[m_eStencilTo].m_szInclude);
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow("L_AdvanceNextRow"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the join key
  vGenerateExprCode(m_pExprStack, ""_Stz);

  // position at entry greater than equal to the join key (lbound)
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // position at entry >= key (lbound)");
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
  StrLib::vAppendString(&m_azCodeCrs, "\n L_KeyExistsDeref:");

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

  StrLib::vAppendString(&m_azCodeCrs, "\n    aHeadLine = (struct HeadLine_T *)aCrwJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    auMetaCode = META_AREA(aHeadLine);");

  sCopy.vSprint("\n    ayDstByte = (byte*)aCombinedT->m_aRowCellArrayJ;"_Stz);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  //aCombinedT->m_aRowCellArrayT = ayDstByte;
  StrLib::vAppendString(&m_azCodeCrs, "\n    acStrArea = (char*)aCombinedT->m_racStrAreaJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    racStrArea = (char*)aCombinedT->m_racStrAreaJ;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    eStrInflateRule = SI_STRAREA_ADR;");

  sCopy.vSprint("\n    i_BaseOffsetDataJ = C_1B  + C_8B  + %lld;"_Stz, pStreamDefRht->nColumns());
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n    ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetDataJ);");

  vGenerateInflate(pStreamDefRht, "L_RetT_ColKE"_Stz, 0, uSelectColumns);
  StrLib::vAppendString(&m_azCodeCrs, "\n    aHeadLine = aHeadLineSav;");

  delete m_pTokenVec;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate filter
  vGenerateKeyExistsFilter(pStreamDefRht, sFilterExpr);

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the keyExits expression with filter                             */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthLabel(Stz* szOp, Stz* sLabel, Stz* sLabelNext) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub(szOp, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto label
  vGenerateGoto(sLabelNext);
}

/*----------------------------------------------------------------------------*/
/* synthesize limit operator                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthLimit(i8 iLimit, Stz* sLabel, Stz* sLabelNext, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("LIMIT-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // init top
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow(sLabelLoop);

  sCopy.vSprint("\n   if (aCombinedT->m_iLimitRows <= %lld) { \n"_Stz, iLimit);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "     aCombinedT->m_iLimitRows++;");
  sCopy.vSprint("\n     goto %s;"_Stz, sLabelNext);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  StrLib::vAppendString(&m_azCodeCrs, "\n   else ");
  StrLib::vAppendString(&m_azCodeCrs, "\n     goto L_Return;");

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the projection                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthOrdAgg(eEK eExprKind, Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext,
                               Stz* sLabelLoop, bool bHashRows) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;
  TypedStack* pPostStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("ORDAGG-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create post aggregation stack
  pPostStack = new TypedStack(KK_DATA_STACK);

  // generate the expression code
  vGenerateExpr((char *) sProjectExpr, pPostStack, EK_AGGORD, bHashRows, m_pStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow(sLabelLoop);

  //RASSERT(eExprKind == EK_AGGORD);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate ordered gby
  vOrdAggGby(sLabelNext, sLabelLoop);

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the projection of aggregate expressions                         */
/*----------------------------------------------------------------------------*/
TypedStack* CodeSynth::pSynthPrjAgg(Stz *sProjectExpr, Stz *sLabel, Stz *sLabelLoop, bool bHashRows) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  TypedStack* pPostStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("PROJECT(AGG)-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create post aggregation stack
  pPostStack = new TypedStack(KK_DATA_STACK);

  // generate the expression code
  vGenerateExpr((char *) sProjectExpr, pPostStack, EK_AGGREG, bHashRows, m_pStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for peoject aggregates
  vGeneratePrjAgg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto label
  vGenerateGoto(sLabelLoop);

  // cleanup
  m_pExprStack->vReset();
  m_pInitStack->vReset();

  return pPostStack;
}

/*----------------------------------------------------------------------------*/
/* synthesize the projection of expressions                                   */
/*----------------------------------------------------------------------------*/
bool CodeSynth::bSynthPrjExp(Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop, bool bHashRows) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("PROJECT-OP"_Stz, sLabel);

  // generate the code for the projected expressions
  vGenerateExpr((char *) sProjectExpr, nullptr, EK_PROJECT, bHashRows, m_pStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the cast to stencil
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow(sLabelLoop);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for projecting expressions
  vGeneratePrjExp();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto label
  vGenerateGoto(sLabelNext);

  // cleanup
  m_pExprStack->vReset();
  m_pInitStack->vReset();

  return true;
}

/*----------------------------------------------------------------------------*/
/* synthesize sort                                                            */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthSort(eEK eExprKind,Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext,
                             bool bHashRows) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;
  TypedStack* pPostStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("ORDAGG-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create post aggregation stack
  pPostStack = new TypedStack(KK_DATA_STACK);

  // generate the expression code
  vGenerateExpr((char *) sProjectExpr, pPostStack, EK_SORT, bHashRows, m_pStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  //vGenerateProcessRow(sLabelLoop);

  //RASSERT(eExprKind == EK_AGGORD);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate sort
  vGenerateSort(m_pIndexDef);

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthStruct(u8 uSelectColumns) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the structure definition
  vGenerateStruct(uSelectColumns);

  StrLib::vSprintCat((Stz *) m_caCode, "\n    struct %s* p%s;\n"_Stz, xRowRef[m_eStencilTo], xRowRef[m_eStencilTo]);
}

/*----------------------------------------------------------------------------*/
/* synthesize the streamDef into a struct                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthStructArray(u8 uSelectColumns) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the structure definition
  vGenerateStruct(uSelectColumns);

  StrLib::vSprintCat((Stz *) m_caCode, "\n    struct %s* p%s[256];\n"_Stz, xRowRef[m_eStencilTo], xRowRef[m_eStencilTo]);
}

/*----------------------------------------------------------------------------*/
/* synthesize top operator                                                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthTop(IndexDef* pIndexDef, Stz* sKeyExpr, i8 iTopCount,
                          Stz* sLabel, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("TOP-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil
  vGenerateStencilCast();

  // generate the code for the projected expressions
  vGenerateExpr((char *) sKeyExpr, nullptr, EK_TOP, false, m_pStreamDefLft);

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the payload key");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // reset the key size / m_nElmSeqSize
  StrLib::vAppendString(&m_azCodeCrs, "    aCombinedT->m_aEntIntPL->m_iIntElm0 = 0;");

  vGenerateExprCode(m_pExprStack, ""_Stz);

  // create the payload
  i8 nPayloadBytes = (m_pStreamDefOut->nColumns() - pIndexDef->nColumns()) * 8;
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // create the payload");
  sCopy.vSprint("\n    vEnt_payloadCreate(aCombinedT->m_aEntIntTOP, %lld);"_Stz, nPayloadBytes);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  sCopy.vSprint("\n    aEntInserted = aIndexTree_insertEntTop(aCombinedT->m_aIndexTreeTOP, aCombinedT->m_aEntIntTOP, &aCombinedT->m_aEntBottomTOP,%lld);"_Stz,
                iTopCount);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n    if (aEntInserted) {");

  // calculate the payload address
  if (pIndexDef->eKeyKind()) {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiTopPayload = (i8 *) ((char *) aEntInserted");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + 8);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiTopPayload = (i8 *) ((char *) aEntInserted");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + ((struct Ent_int*)aEntInserted)->m_iIntElm0");
    StrLib::vAppendString(&m_azCodeCrs, "\n                + 8);");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // populate the top payload");

  i8 iPayLoad = 0;
  i8 i_DupCtr = 1;
  s256 sColNm;
  for (i8 i_Col = 0; i_Col < m_pStreamDefOut->nColumns(); i_Col++) {

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n       // - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n       // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    // is the column part of the aggregate group by key ?
    Column* pIndexColumn = pIndexDef->pFindColumn(pColumn->szColNm());

    if (pIndexColumn) {
      continue;
    }

    // make the column name
    vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

    if ((pColumn->eColumnType() == CT_VARCHAR) ||
        (pColumn->eColumnType() == CT_CHAR)) {

      sCopy.vSprint("\n      szSrcStz = (char*)p%s->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      sCopy.vSprint("\n       nSrcSize = 8 - 1; //strlen(szSrcStz);"_Stz);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      sCopy.vSprint("\n      szDstStz = (char*)&aCombinedT->m_aiTopPayload[%lld];"_Stz, iPayLoad );
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs,"\n      strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

    } else {
      sCopy.vSprint("\n       aCombinedT->m_aiTopPayload[%lld] = p%s->%s;"_Stz, iPayLoad,
                    xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }

    iPayLoad++;
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n      }");

  sCopy.vSprint("\n    goto %s;"_Stz, sLabelLoop);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // cleanup
  m_pExprStack->vReset();
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
/* generate code for assigning values                                         */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateAssign(u8 uSelectColumns) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;
  i8 nColumns = m_pStreamDefLft->nColumns();

  // for each column of row of left table T
  s256 sColNm;
  s256 sJoinColNm;
  i8 i_DupCtr = 1;
  i8 i_DupCtrJoin = 1;

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    // skip columns in join not needed
    if (BitLib::bIsOff(uSelectColumns, i_Col))
      continue;

    // make the column name
    vMakeColumnNmStruct(m_pStreamDefLft, sColNm, i_DupCtr, i_Col);

    // make the column name
    vMakeColumnNmStruct(m_pStreamDefLft, sJoinColNm, i_DupCtrJoin, i_Col);

    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n    // copy column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
    StrLib::vAppendString(&m_azCodeCrs, "\n    ");

    Column* pColumn = m_pStreamDefLft->pGetColumn(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n    // get the source string");
      sCopy.vSprint("\n     szSrcStz = (char*)p%s->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    if (szSrcStz == (char*)C_DATUM_NULL) {");
      StrLib::vAppendString(&m_azCodeCrs, "\n      // copy the source to the destination");
      sCopy.vSprint("\n      p%s->%s = C_DATUM_NULL;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sJoinColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    }");
      StrLib::vAppendString(&m_azCodeCrs, "\n    else {");
      StrLib::vAppendString(&m_azCodeCrs, "\n      nSrcSize = strlen(szSrcStz) + 1;");

      StrLib::vAppendString(&m_azCodeCrs, "\n      // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n      szDstStz = (char*)aCombinedT->m_racStrAreaTJ;");

      StrLib::vAppendString(&m_azCodeCrs, "\n      // copy the source to the destination");
      StrLib::vAppendString(&m_azCodeCrs, "\n      strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      sCopy.vSprint("\n      p%s->%s = (i8)szDstStz;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sJoinColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n      // update the tub string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_racStrAreaTJ += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n    }");
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
/* generate error check                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateErrorCheck(void) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      return 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
}

/*----------------------------------------------------------------------------*/
/* generate code for inflate                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateInflate(StreamDef *pStreamDef, Stz* sReturnPrefix, i8 nColumnsBefore, u8 uSelectColumns) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;
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
/* generate code for initial operator stub                                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateInitStub(Stz* szComment, Stz* sLabel) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
  sCopy.vSprint("\n    // %s "_Stz, szComment);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n  %s:"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n    paW = &aCombinedT->m_aW[0];");

  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[0] = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[1] = 10;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[2] = 100;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[3] = 1000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[4] = 10000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[5] = 100000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[6] = 1000000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[7] = 10000000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[8] = 100000000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[9] = 1000000000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[10] = 10000000000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[11] = 100000000000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[12] = 1000000000000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[13] = 10000000000000;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iaPow10[14] = 100000000000000;");
}

/*----------------------------------------------------------------------------*/
/* generate code for stencil cast                                             */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateStencilCast(void) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sCopy;

  switch (m_eStencilFrom) {

    case SR_AT:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz,
        xRowRef[m_eStencilFrom].m_szInclude,
        xRowRef[m_eStencilFrom].m_szInclude);
      break;

    case SR_IJ:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayJ;"_Stz,
        xRowRef[m_eStencilFrom].m_szInclude,
        xRowRef[m_eStencilFrom].m_szInclude);
      break;

    case SR_IT:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;\n"_Stz,
        xRowRef[m_eStencilFrom].m_szInclude,
        xRowRef[m_eStencilFrom].m_szInclude);
      break;

    case SR_PT:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayP;\n"_Stz,
        xRowRef[m_eStencilFrom].m_szInclude,
        xRowRef[m_eStencilFrom].m_szInclude);
      break;

    case SR_TJ:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;\n"_Stz,
        xRowRef[m_eStencilFrom].m_szInclude,
        xRowRef[m_eStencilFrom].m_szInclude);
      break;

    default:
      NEVER_REACH;
  }

  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
}

/*----------------------------------------------------------------------------*/
/* generate code for stencil cast                                             */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateStencilCastTo(void) {

  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sCopy;

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  switch (m_eStencilTo) {

    case SR_AT:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz,
                    xRowRef[m_eStencilTo].m_szInclude,
                    xRowRef[m_eStencilTo].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_racStrAreaAgg = aCombinedT->m_racStrAreaSavAgg;");
      StrLib::vAppendString(&m_azCodeCrs, "\n    szStrArea = aCombinedT->m_racStrAreaAgg;");
      break;

    case SR_IJ:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayJ;"_Stz,
                    xRowRef[m_eStencilTo].m_szInclude,
                    xRowRef[m_eStencilTo].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_racStrAreaJ = aCombinedT->m_racStrAreaSavJ;");
      StrLib::vAppendString(&m_azCodeCrs, "\n    szStrArea = aCombinedT->m_racStrAreaJ;");
      break;

    case SR_IT:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayT;\n"_Stz,
                    xRowRef[m_eStencilTo].m_szInclude,
                    xRowRef[m_eStencilTo].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_racStrAreaT = aCombinedT->m_racStrAreaSavT;");
      StrLib::vAppendString(&m_azCodeCrs, "\n    szStrArea = aCombinedT->m_racStrAreaT;");
      break;

    case SR_PT:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayP;\n"_Stz,
                    xRowRef[m_eStencilTo].m_szInclude,
                    xRowRef[m_eStencilTo].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_racStrAreaP = aCombinedT->m_racStrAreaSavP;");
      StrLib::vAppendString(&m_azCodeCrs, "\n    szStrArea = aCombinedT->m_racStrAreaP;");
      break;

    case SR_TJ:
      sCopy.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayTJ;\n"_Stz,
                    xRowRef[m_eStencilTo].m_szInclude,
                    xRowRef[m_eStencilTo].m_szInclude);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_racStrAreaTJ = aCombinedT->m_racStrAreaSavTJ;");
      StrLib::vAppendString(&m_azCodeCrs, "\n    szStrArea = aCombinedT->m_racStrAreaTJ;");
      break;

    default:
      NEVER_REACH;
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n");
}

/*----------------------------------------------------------------------------*/
/* generate code for Project aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vFindKey(IndexDef* pIndexDef, ePL ePayLoad) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sIndexTree;
  s256 sEntKey;
  s256 sCopy;

  // find the key in the indexTree

  switch(ePayLoad) {

    case PL_GBY:
      sIndexTree = "m_aIndexTreeGby"_Stz;
      sEntKey = "m_aEntKeyPL"_Stz;
      break;

    case PL_SRT:
      sIndexTree = "m_aIndexTreeGby"_Stz;
      sEntKey = "m_aEntKeySRT"_Stz;
      break;

    default:
      NEVER_REACH;
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // find key in payload index");
  sCopy.vSprint("\n    aEntFound = aIndexTree_findEntry(aCombinedT->%s, aCombinedT->%s);"_Stz,
                sIndexTree.azChar(), sEntKey.azChar());
  StrLib::vAppendString(&m_azCodeCrs,sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( !aEntFound ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // key not found");

  vPayloadStore(pIndexDef);

  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  vPayloadAgg(pIndexDef);
}

/*----------------------------------------------------------------------------*/
/* get key value                                                              */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGetValue(eDM eDatum, char* sValue, i8& iValue, d8& dValue) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  switch (eDatum) {
    case DM_BCD: {
      SN_Bcd snBcd;
      iValue = snBcd.i(&sValue, AM::pFmt());
    }
      break;
    case DM_DTE: {
      SN_Dte snDate;
      iValue = snDate.i(&sValue, AM::pFmt());
    }
      break;
    case DM_FLT: {
      SN_Flt snFlt;
      dValue = snFlt.d(&sValue, AM::pFmt());
    }
      break;
    case DM_TME: {
      SN_Tme snTime;
      iValue = snTime.i(&sValue, AM::pFmt());
    }
      break;
    case DM_TSP: {
      SN_Tsp snTsp;
      iValue = snTsp.i(&sValue, AM::pFmt());
    }
      break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* generate code for ordered aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vOrdAggGby(Stz* sLabelNext, Stz* sLabelLoop) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K  sInfix;
  s256 sCastRow;
  s4K sCopy;
  s256 sColNm;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  StrLib::vAppendString(&m_azCodeCrs,"\n");
  sCastRow.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude,
                   xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate aggregate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( aCombinedT->m_iProjectInit == 0 ) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // allocate a projectRow from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProjectInit = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init code for each aggregate column expression
  //vGenerateAggInitCode(pPreExpStack);

  vGenerateExprCode(m_pGrpByStack, "  "_Stz);

  vGenerateExprCode(m_pInitStack, "  "_Stz);

  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // is it a new key ?

  s256 sColOutNm;
  i8 i_DupCtr = 1;
  for (i8 i_Col = 0; i_Col < m_pIndexDef->nColumns(); i_Col++) {
    Column* pColumn = m_pIndexDef->pGetColumn(i_Col);

    // get the index col no matching the streamDef column no
    i8 iIndexCol = m_pIndexDef->iGetColumn(pColumn->iColumnNo());

    vMakeColumnNmStruct(m_pStreamDefLft, sColNm, i_DupCtr, iIndexCol);

    // NOT SURE: COME BACK
    // make the column name
    vMakeColumnNmStruct(m_pStreamDefOut, sColOutNm, i_DupCtr, i_Col);

    if (m_pIndexDef->eKeyKind()) {
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
      }
      else {
        sCopy.vSprint("\n   if (p%s->%s == p%s->%s) { "_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, sColOutNm.azChar(),
                      xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate Agg code for each projected agg column
  vGenerateExprCode(m_pExprStack, ""_Stz);

  sCopy.vSprint("\n     goto %s;"_Stz, sLabelLoop);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  StrLib::vAppendString(&m_azCodeCrs, "\n   else {");
  StrLib::vAppendString(&m_azCodeCrs, "\n     aCombinedT->m_iProjectInit = 0;");
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
void CodeSynth::vPayloadKey(IndexDef* pIndexDef, Stz* szEntWrk, Stz* szEntKey) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;
  s256 sColNm;

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the payload key");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // reset the key size / m_nElmSeqSize
  StrLib::vAppendString(&m_azCodeCrs, "    aCombinedT->m_aEntIntPL->m_iIntElm0 = 0;");

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    vMakeIdxColNmStruct(pIndexDef, sColNm, i_Col);
    //s256 sColNm;
    //sColNm = pColumn->szColNm();
    //StrLib::vToUpper(sColNm.azChar());

    if (pIndexDef->eKeyKind()) {
        sCopy.vSprint("\n    *(i8*)aCombinedT->%s = p%s->%s;"_Stz,
                      szEntKey,
                      xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
    else {
      if (pColumn->eColumnType() == CT_VARCHAR ||
          pColumn->eColumnType() == CT_CHAR) {
        sCopy.vSprint("\n    vEntWrk_appendElementEntStz(aCombinedT->%s, (char*)p%s->%s);"_Stz,
                      szEntWrk,
                      xRowRef[m_eStencilFrom].m_szInclude,
                      sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }
      else {
        sCopy.vSprint("\n    vEntWrk_appendElementEntInt(aCombinedT->%s, p%s->%s);"_Stz,
                      szEntWrk,
                      xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }
    }
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for payload store for groupby                                */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadStore(IndexDef* pIndexDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K  sInfix;
  s4K  sCopy;

  // create the payload

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // PAYLOAD STORE");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // create the payload
  i8 nPayloadBytes = (m_pStreamDefOut->nColumns() - pIndexDef->nColumns()) * 8;
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate expression code for init of each projected agg column
  vGenerateExprCode(m_pInitStack, "  "_Stz);
}

/*----------------------------------------------------------------------------*/
/* generate code for payload aggregates for groupby                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadAgg(IndexDef* pIndexDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sColNm;
  s4K  sInfix;
  s4K  sCopy;

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
  // generate the code for the postt aggregatation
  vGenerateExprCode(m_pExprStack, ""_Stz);
}

/*----------------------------------------------------------------------------*/
/* generate code for processing EOF                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadToBuffer(IndexDef* pIndexDef, TypedStack* pPostStack, ePL ePayLoad)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s4K  sCopy;

  s256 sPayLoad;
  s256 sPayLoadRow;
  s256 sEntInt;

  i8 nColumns = m_pStreamDefOut->nColumns();

  switch(ePayLoad) {

    case PL_GBY:
      sPayLoad = "m_aiPayload"_Stz;
      sPayLoadRow = "m_aPayloadRow"_Stz;
      sEntInt = "m_aEntIntPL"_Stz;
      break;

    case PL_SRT:
      sPayLoad = "m_aiSrtPayload"_Stz;
      sPayLoadRow = "m_aSrtPayloadRow"_Stz;
      sEntInt = "m_aEntIntSRT"_Stz;
      break;

    case PL_TOP:
      sPayLoad = "m_aiTopPayLoad"_Stz;
      sPayLoadRow = "m_aTopPayloadRow"_Stz;
      sEntInt = "m_aEntIntTOP"_Stz;
      break;

    default:
      NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n      p%s = (struct %s*)aCombinedT->%s;\n"_Stz,
                   xRowRef[m_eStencilTo].m_szInclude,
                   xRowRef[m_eStencilTo].m_szInclude,
                   sPayLoadRow.azChar());
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the post aggregation
  vGenerateExprCode(pPostStack, ""_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate error check
  vGenerateErrorCheck();

  i8 iPayLoad = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column
  s256 sColNm;
  i8 i_DupCtr = 1;

  StrLib::vAppendString(&m_azCodeCrs, "\n      iElemSize = 0;");

  ePE ePreExp = PE_NONE;
  i8 iExprStack = pIndexDef->nColumns();

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++, iExprStack++) {

    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    // make the column name
    vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is column in the key ?
    //Column* pIndexColumn = pIndexDef->pFindColumn(pColumn->szColNm());

    ePreExp = (ePE)m_pPreExpStack->iPeek(iExprStack);

    if (ePreExp == PE_GKY) {

      // cannot be an aggregate
      //RASSERT(ePreExp == PE_NONE);

      if (pIndexDef->eKeyKind() == KE_GENERAL) {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // VARCHAR column
        if (pColumn->eColumnType() == CT_VARCHAR ||
            pColumn->eColumnType() == CT_CHAR) {

          RASSERT(pIndexDef->eKeyKind() == KE_GENERAL);

          sCopy.vSprint(
              "\n      szSrcStz = ((char*)(&(((struct Ent_int*)aCombinedT->%s)->m_iIntElm0)) + 8 + iElemSize);"_Stz,
              sEntInt.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

          StrLib::vAppendString(&m_azCodeCrs, "\n      szDstStz = aCombinedT->m_racTubStrArea;");
          StrLib::vAppendString(&m_azCodeCrs, "\n");

          StrLib::vAppendString(&m_azCodeCrs, "\n      nSrcSize = StrLib_iWordFlipBack(szDstStz,(char*)szSrcStz);");

          sCopy.vSprint("\n      amCellAddress = (m8*) &p%s->%s;"_Stz, xRowRef[m_eStencilTo].m_szInclude,
                        sColNm.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n");

          StrLib::vAppendString(&m_azCodeCrs,
                                "\n      iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);");


          StrLib::vAppendString(&m_azCodeCrs,"\n      _PSH_INT((i8)szSrcStz)");
          StrLib::vAppendString(&m_azCodeCrs,"\n      _ISNULL();");
          StrLib::vAppendString(&m_azCodeCrs,"\n      _ASSIGN(bNullResult);");
          StrLib::vAppendString(&m_azCodeCrs,"\n      if (!bNullResult) {");

          sCopy.vSprint("\n        p%s->%s = iOffset;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

          StrLib::vAppendString(&m_azCodeCrs, "\n");
          StrLib::vAppendString(&m_azCodeCrs, "\n        aCombinedT->m_racTubStrArea += nSrcSize;");
          StrLib::vAppendString(&m_azCodeCrs, "\n        vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);");
          StrLib::vAppendString(&m_azCodeCrs, "\n");
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n        iElemSize += nSrcSize;");
          StrLib::vAppendString(&m_azCodeCrs, "\n      }");
          StrLib::vAppendString(&m_azCodeCrs, "\n      else {");
          sCopy.vSprint("\n        p%s->%s = C_DATUM_NULL;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n        iElemSize += 8;");

          StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        } else {
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          // all other types


          sCopy.vSprint(
              "\n       p%s->%s = *(i8*)((char*)(&((struct Ent_int*)aCombinedT->%s)->m_iIntElm0) + 8 + iElemSize);\n"_Stz,
              xRowRef[m_eStencilTo].m_szInclude,
              sColNm.azChar(),
              sEntInt.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n      iElemSize += 8;");
        }
      }
      else {
        // get the index col no maching the streamDef column no
        i8 iIndexCol = pIndexDef->iGetColumn(pColumn->iColumnNo());
        sCopy.vSprint("\n      p%s->%s = ((struct Ent_int*)aCombinedT->%s)->m_iIntElm0 + (%lld*8);\n"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude,
                      sColNm.azChar(),
                      sEntInt.azChar(),
                      iIndexCol);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }

      continue;
    }

    // must be an aggregate
    //RASSERT(ePreExp != PE_NONE);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      sCopy.vSprint("\n      szSrcStz = (char*)&aCombinedT->%s[%lld];"_Stz,
                    sPayLoad.azChar(), iPayLoad);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      sCopy.vSprint("\n      nSrcSize = strlen((char*)&aCombinedT->%s[%lld]) + 1;"_Stz,
                    sPayLoad.azChar(), iPayLoad);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs,"\n      szDstStz = aCombinedT->m_racTubStrArea;");
      StrLib::vAppendString(&m_azCodeCrs,"\n      strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      sCopy.vSprint("\n      amCellAddress = (m8*) &p%s->%s;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n      iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);");
      sCopy.vSprint("\n      p%s->%s = iOffset;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_racTubStrArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n      vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types

      sCopy.vSprint("\n      p%s->%s = aCombinedT->%s[%lld];\n"_Stz,
                    xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                    sPayLoad.azChar(), iPayLoad);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
    iPayLoad++;
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for processing EOF                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadTraversal(IndexDef* pIndexDef, TypedStack* pPostStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sCopy;

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

  // generate code if limitOP
  if (m_iLimitRows) {
    sCopy.vSprint("\n   if (aCombinedT->m_iLimitRows <= %lld) { \n"_Stz, m_iLimitRows);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "     aCombinedT->m_iLimitRows++;");
    StrLib::vAppendString(&m_azCodeCrs, "\n   }");
    StrLib::vAppendString(&m_azCodeCrs, "\n   else {");
    StrLib::vAppendString(&m_azCodeCrs, "\n     break;");
    StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  }


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

  vPayloadToBuffer(pIndexDef, pPostStack, PL_GBY);

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aEntIntPL = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferPL, aCombinedT->m_aEntIntPL);");

  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
}

/*----------------------------------------------------------------------------*/
/* generate code for Project Aggregates                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGeneratePrjAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCastTo();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate aggregate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( aCombinedT->m_iProjectInit == 0 ) {");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate expression code for init of each projected agg column
  vGenerateExprCode(m_pInitStack, "  "_Stz);

  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProjectInit = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_iProcessRow) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iRowsProcessed++;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate aggregate code for each projected agg column
  vGenerateExprCode(m_pExprStack, ""_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate error check
  vGenerateErrorCheck();

  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
}

/*----------------------------------------------------------------------------*/
/* generate code for Project Expressions                                      */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGeneratePrjExp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K  sInfix;
  s256 sCastRow;
  s4K  sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCastTo();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for each projected column expression
  vGenerateExprCode(m_pExprStack, ""_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate error check
  vGenerateErrorCheck();
}

/*----------------------------------------------------------------------------*/
/* generate code for Project aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGeneratePrjGby(IndexDef* pIndexDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the payload key
  //vPayloadKey(pIndexDef, "m_aEntWrkPL"_Stz, "m_aEntKeyPL"_Stz);
  vGenerateExprCode(m_pGrpByStack, ""_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the key in the indexTree
  vFindKey(pIndexDef, PL_GBY);
}

/*----------------------------------------------------------------------------*/
/* generate code for sort                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateSort(IndexDef* pIndexDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the expr stack code
  vGenerateExprCode(m_pExprStack, ""_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the key in the indexTree
  vFindKey(pIndexDef, PL_SRT);
}

/*----------------------------------------------------------------------------*/
/* add the init aggregate                                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vInitAgg(ePE ePreExp, eDM eExprType)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sOperation;
  s32  sInitValue;
  Bcd  xBcd;
  i8   iInitValue = 0;

  switch(ePreExp) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PE_ALL:
      sOperation.vSprintCat("_PSH_INT(0)"_Stz);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PE_CNT:
      sOperation.vSprintCat("_PSH_INT(0)"_Stz);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PE_MIN:
      switch(eExprType) {
        case DM_BCD: iInitValue = Bcd::iMaxValue();
          sOperation.vSprintCat("_PSH_BCD(%lld)"_Stz, iInitValue);
          break;
        case DM_BOL: iInitValue = Bol::iMaxValue();
          sOperation.vSprintCat("_PSH_BOL(%lld)"_Stz, iInitValue);
          break;
        case DM_DTE: iInitValue = Dte::iMaxValue();
          sOperation.vSprintCat("_PSH_DTE(%lld)"_Stz, iInitValue);
          break;
        case DM_FLT: iInitValue = Flt::iMaxValue();
          sOperation.vSprintCat("_PSH_FLT(%lld)"_Stz, iInitValue);
          break;
        case DM_INT: iInitValue = Int::iMaxValue();
          sOperation.vSprintCat("_PSH_INT(%lld)"_Stz, iInitValue);
          break;
        case DM_IVL: iInitValue = Ivl::iMaxValue();
          sOperation.vSprintCat("_PSH_IVL(%lld)"_Stz, iInitValue);
          break;
        case DM_IVY: iInitValue = Ivl::iMaxValue();
          sOperation.vSprintCat("_PSH_IVY(%lld)"_Stz, iInitValue);
          break;
        case DM_TME: iInitValue = Tme::iMaxValue();
          sOperation.vSprintCat("_PSH_TME(%lld)"_Stz, iInitValue);
          break;
        case DM_TSP: iInitValue = Tsp::iMaxValue();
          sOperation.vSprintCat("_PSH_TSP(%lld)"_Stz, iInitValue);
          break;
        case DM_VCH: Vch::vMaxValue(sInitValue.azChar());
          sOperation.vSprintCat("_PSH_VCH(\"%s\")"_Stz, sInitValue.azChar());
          break;
        case DM_UNK:
        default:
          NEVER_REACH;
          break;
      }
      break;


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PE_MAX:
      switch(eExprType) {
        case DM_BCD: iInitValue = Bcd::iMinValue();
          sOperation.vSprintCat("_PSH_BCD(%lld)"_Stz, iInitValue);
          break;
        case DM_BOL: iInitValue = Bol::iMinValue();
          sOperation.vSprintCat("_PSH_BOL(%lld)"_Stz, iInitValue);
          break;
        case DM_DTE: iInitValue = Dte::iMinValue();
          sOperation.vSprintCat("_PSH_DTE(%lld)"_Stz, iInitValue);
          break;
        case DM_FLT: iInitValue = Flt::iMinValue();
          sOperation.vSprintCat("_PSH_FLT(%lld)"_Stz, iInitValue);
          break;
        case DM_INT: iInitValue = Int::iMinValue();
          sOperation.vSprintCat("_PSH_INT(%lld)"_Stz, iInitValue);
          break;
        case DM_IVL: iInitValue = Ivl::iMinValue();
          sOperation.vSprintCat("_PSH_IVL(%lld)"_Stz, iInitValue);
          break;
        case DM_IVY: iInitValue = Ivl::iMinValue();
          sOperation.vSprintCat("_PSH_IVY(%lld)"_Stz, iInitValue);
          break;
        case DM_TME: iInitValue = Tme::iMinValue();
          sOperation.vSprintCat("_PSH_TME(%lld)"_Stz, iInitValue);
          break;
        case DM_TSP: iInitValue = Tsp::iMinValue();
          sOperation.vSprintCat("_PSH_TSP(%lld)"_Stz, iInitValue);
          break;
        case DM_VCH: Vch::vMinValue(sInitValue.azChar());
          sOperation.vSprintCat("_PSH_VCH(\"%s\")"_Stz, sInitValue.azChar());
          break;
        case DM_UNK:
        default:
          NEVER_REACH;
          break;
      }
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PE_SUM:

      switch(eExprType) {
        case DM_BCD:
          xBcd.vSet(0,0);
          sOperation.vSprintCat("_PSH_BCD(%lld)"_Stz, xBcd.T.m_iAsI8);
          break;
        case DM_FLT:
          sOperation.vSprintCat("_PSH_FLT(0)"_Stz);
          break;
        case DM_INT:
          sOperation.vSprintCat("_PSH_INT(0)"_Stz);
          break;
        case DM_BOL:
        case DM_DTE:
        case DM_IVL:
        case DM_IVY:
        case DM_TME:
        case DM_TSP:
        case DM_UNK:
        case DM_VCH:
        default:
          NEVER_REACH;
      }
      break;

    default:
      break;
      //NEVER_REACH;
  }
  // push on the aggregate init stack
  m_pInitStack->vPushStz(sOperation);
}

/*----------------------------------------------------------------------------*/
/* convert postfix to expr macros                                             */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateExpr(char* sExpr, TypedStack* pPostStack, eEK eExprKind, bool bHashRows, StreamDef* pStreamDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  eDM eExprType = DM_UNK;

  bool bIndexJoin = false;
  bool bLowerBound = false;
  bool bUpperBound = false;
  bool bGroupBy    = false;

  TypedCell *pTypedCell;
  TypedStack *pExprStack;
  IntStack *pExprType;

  Token *pTkn;

  i8 i_Col = 0;
  i8 i_ExprStack = 0;
  i8 i_DupCtr = 1;
  i8 iValue;
  eDM eType;
  eDM eCastType;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenize the expression
  m_pTokenVec = TokenVec::pTokeniseSen((Stz *) sExpr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the ExprType stack
  pExprType = new IntStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the predicate expression
  //RASSERT(m_pPreExpStack);
  ePE ePreExp = m_pPreExpStack ? (ePE)m_pPreExpStack->iPeek(i_ExprStack) : PE_NONE;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set working ExprStack to be the post aggregate expression stack
  if (ePreExp == PE_ALL ||
      ePreExp == PE_CNT ||
      ePreExp == PE_MIN ||
      ePreExp == PE_MAX ||
      ePreExp == PE_SUM ) {
    pExprStack = pPostStack;
  }
  else
    pExprStack = m_pExprStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate tokens of the expression
  for (i8 i_Tkn = 0; i_Tkn < m_pTokenVec->nItems(); i_Tkn++) {

    pTkn = m_pTokenVec->pGet(i_Tkn);
#ifdef WITH_DEBUG_VERSION
    vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    // dump the TypeStack
    vTrace("--------------------------------------\n"_Stz);
    vTrace("EXPRTYPE:\n"_Stz);
    i8 nExpr = pExprType->nItems() - 1;

    while ( nExpr >= 0) {
      eDM eType = (eDM)pExprType->iPeek(nExpr--);
      vTrace("%s\n"_Stz, aDM[eType]);
    }
    vTrace("--------------------------------------\n"_Stz);
#endif

    if (pTkn->bSame(","_Stz)) {
      // skip the comma

      if (ePreExp != PE_GBY)
        pExprStack = m_pExprStack;

      // push assign column
      vPushAssignCol(pExprStack, pPostStack, pExprType, eExprKind, ePreExp, pTkn, i_Col,
                     i_DupCtr);

      if (ePreExp != PE_GKY || (ePreExp == PE_GKY && eExprKind == EK_AGGORD))
        i_Col++;


      i_ExprStack++;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to the next predicate expression
      ePreExp = (ePE)m_pPreExpStack->iPeek(i_ExprStack);

      if ( ! bGroupBy) {
        if (ePreExp == PE_ALL ||
            ePreExp == PE_CNT ||
            ePreExp == PE_MIN ||
            ePreExp == PE_MAX ||
            ePreExp == PE_SUM) {
          pExprStack = pPostStack;
        } else
          pExprStack = m_pExprStack;
      }

      continue;
    }

    if (pTkn->bSame("groupby"_Stz)) {

      bGroupBy = true;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the groupby  stack
      pExprStack = m_pGrpByStack;
      eExprKind = EK_GROUPBY;

      continue;
    }

    if (pTkn->bSame("ordGroupby"_Stz)) {

      bGroupBy = true;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the groupby  stack
      pExprStack = m_pGrpByStack;
      eExprKind = EK_ORDGROUPBY;

      continue;
    }

    if (pTkn->bSame("aggregates"_Stz)) {

      bGroupBy = false;

      // push assign column
      vPushAssignCol(pExprStack, pPostStack, pExprType, eExprKind, ePreExp, pTkn, i_Col,
                     i_DupCtr);

      if (eExprKind == EK_GROUPBY)
        eExprKind = EK_AGGGBY;
      else {
        RASSERT(eExprKind == EK_ORDGROUPBY);
        eExprKind = EK_AGGORD;
      }


      i_Col = 0;
      i_ExprStack++;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to the next predicate expression
      ePreExp = (ePE)m_pPreExpStack->iPeek(i_ExprStack);

      if (ePreExp == PE_ALL ||
          ePreExp == PE_CNT ||
          ePreExp == PE_MIN ||
          ePreExp == PE_MAX ||
          ePreExp == PE_SUM ) {
        pExprStack = pPostStack;
      }
      else
        pExprStack = m_pExprStack;

      continue;
    }

    if (pTkn->bSame("sort"_Stz)) {

      bGroupBy = true;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the expression  stack
      pExprStack = m_pExprStack;
      eExprKind = EK_SORT;

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // bound
    if (pTkn->bSame("bound"_Stz)) {
      bIndexJoin = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // lbound
    if (pTkn->bSame("lbound"_Stz)) {

      // push upperBound assign column
      if (bUpperBound)
        vPushAssignCol(pExprStack, pPostStack, pExprType, eExprKind, ePreExp, pTkn, i_Col, i_DupCtr);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the lower bound  stack
      pExprStack = m_pLowerStack;
      bLowerBound = true;
      bUpperBound = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ubound
    if (pTkn->bSame("ubound"_Stz)) {

      // push lowerBound assign column
      if (bLowerBound)
        vPushAssignCol(pExprStack, pPostStack, pExprType, eExprKind, ePreExp, pTkn, i_Col, i_DupCtr);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the upper bound stack
      pExprStack = m_pUpperStack;
      bUpperBound = true;
      bLowerBound = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // push the hash in expr list count
    // strings in query can have "#Brand3" etc
    // need a better solution...
    // TODO: ALKA cleanup
    if (pTkn->szChars()->bContains("#"_Stz)) {
      char *sHash = (char*)(pTkn->szChars());
      if (sHash[0] == '#') {
        char *sCount = (char*)(pTkn->szChars() + 1);
        if (StrLib::bIsDigit(sCount[0])) {
          vPushCount(pExprStack, pExprType, pTkn);
          continue;
        }
      }

    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // push the ordinal column on the expr stack
    // TODO: ALKA cleanup
    if (pTkn->szChars()->bContains("$"_Stz)) {
      char *sDollar = (char*)(pTkn->szChars());
      if (sDollar[0] == '$') {
        char *sNext = (char*)(pTkn->szChars() + 1);
        if (StrLib::bIsDigit(sNext[0])) {
          vPushOrdCol(pExprStack, pExprType, eExprKind, pTkn);
          continue;
        }
        else {
          char *sNext = (char*)(pTkn->szChars() + 2);
          if (StrLib::bIsDigit(sNext[0])) {
            vPushOrdCol(pExprStack, pExprType, eExprKind, pTkn);
            continue;
          }
        }
      }

    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the final expression type
    if (pTkn->bSame("type"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      eExprType = StreamDefBase::eDatum(pTkn->szChars());
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get asc / desc
    if (pTkn->bSame("asc"_Stz) || pTkn->bSame("desc"_Stz)) {
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the in expression
    if (pTkn->bSame("in"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the between expression
    if (pTkn->bSame("between"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_day expression
    if (pTkn->bSame("extract_day"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_hour expression
    if (pTkn->bSame("extract_hour"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_min expression
    if (pTkn->bSame("extract_min"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_month expression
    if (pTkn->bSame("extract_month"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_second expression
    if (pTkn->bSame("extract_second"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the between expression
    if (pTkn->bSame("extract_year"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the in expression
    if (pTkn->bSame("in"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the like expression
    if (pTkn->bSame("like2"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    if (pTkn->bSame("like3"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    if (pTkn->bSame("matchLeading"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    if (pTkn->bSame("matchMiddle"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    if (pTkn->bSame("matchMiddle1"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    if (pTkn->bSame("matchMiddle2"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    if (pTkn->bSame("matchTrailing"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the substring2 expression
    if (pTkn->bSame("substring2"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the substring3 expression
    if (pTkn->bSame("substring3"_Stz)) {
      vPushFunc(pExprStack, pExprType, ePreExp);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip the alias
    if (pTkn->bSame("as"_Stz)) {
      i_Tkn++;
      continue;
    }

    if (pTkn->bSame("}"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the normal expression stack
      pExprStack = m_pExprStack;
      continue;
    }

    // get the cast expression type
    if (pTkn->bSame("casttype"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      eCastType = StreamDefBase::eDatum(pTkn->szChars());
      continue;
    }

    if (pTkn->bSame("cast"_Stz)) {
      vPushCast(pExprStack, pExprType, pTkn, eCastType);
      continue;
    }

    if (pTkn->bSame(")"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the normal expression stack
      pExprStack = m_pExprStack;
      continue;
    }

    if ( pTkn->bSame("count"_Stz)    ||
         pTkn->bSame("countall"_Stz) ||
         pTkn->bSame("min"_Stz)      ||
         pTkn->bSame("max"_Stz)      ||
         pTkn->bSame("sum"_Stz)
        ) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // aggregate follows, generate init expressions in the m_aInitStack
      vPushAggCol(pExprStack, pPostStack, ePreExp, eExprKind, eExprType, bHashRows, i_Col, i_DupCtr);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the post aggregate expression stack
      RASSERT(pPostStack);
      pExprStack = pPostStack;

      continue;
    }

    if (pTkn->bIsFloat()) {
      vPushLiteral(pExprStack, pExprType, pTkn, DM_FLT);
      continue;
    }

    if (pTkn->bIsInteger()) {
      vPushLiteral(pExprStack, pExprType, pTkn, DM_INT);
      continue;
    }

//    if (pTkn->bSame("negate"_Stz) ) {
//      pExprStack->vPushPtr(pTkn);
//      continue;
//    }
//
    if (pTkn->bIsDecimal()) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      vPushLiteral(pExprStack, pExprType, pTkn, DM_BCD);
      continue;
    }

    if (pTkn->bSame("DATE"_Stz) ) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      vPushLiteral(pExprStack, pExprType, pTkn, DM_DTE);
      continue;
    }

    if (pTkn->bSame("TIME"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      vPushLiteral(pExprStack, pExprType, pTkn, DM_TME);
      continue;
    }

    if (pTkn->bSame("TIMESTAMP"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      vPushLiteral(pExprStack, pExprType, pTkn, DM_TSP);
      continue;
    }

    if (pTkn->bSame("null"_Stz) ) {
      pTkn = m_pTokenVec->pGet(i_Tkn);
      vPushLiteral(pExprStack, pExprType, pTkn, DM_UNK);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // arithmetic operator
    if (pTkn->bIsArithmetic()) {

      RASSERT( ! pTkn->bIsString() );
      RASSERT( ! bLowerBound );
      RASSERT( ! bUpperBound );

      vPushArithmetic(pExprStack, pExprType, pTkn);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // logical operator
    if (pTkn->bIsLogical()) {

      RASSERT( ! pTkn->bIsString() );
      RASSERT( ! bLowerBound );
      RASSERT( ! bUpperBound );

      vPushLogical(pExprStack, pExprType, pTkn);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // relational operator ?
    if (pTkn->bIsRelational()) {

      // skip == op if indexJoin expr
      if (bIndexJoin) {
        RASSERT(pTkn->bSame("=="_Stz));
        continue;
      }

      if (bLowerBound || bUpperBound) {

        RASSERT(pTkn->bIsRelational());

        if (pTkn->bSame(">"_Stz)) {
          RASSERT(bLowerBound);
          m_sLowerPosition = "aIndexTree_positionGT"_Stz;
        } else if (pTkn->bSame(">="_Stz)) {
          RASSERT(bLowerBound);
          m_sLowerPosition = "aIndexTree_positionGE"_Stz;
        } else if (pTkn->bSame("<"_Stz)) {
          RASSERT(bUpperBound);
          m_sUpperPosition = "aIndexTree_positionLT"_Stz;
        } else if (pTkn->bSame("<="_Stz)) {
          RASSERT(bUpperBound);
          m_sUpperPosition = "aIndexTree_positionLE"_Stz;
        } else {
          RASSERT(0);
        }

        continue;
      }

//      if (pTkn->bIsString()) {
//        pExprStack->vPushPtr(pTkn);
//        continue;
//      }

      vPushRelational(pExprStack, pExprType, pTkn);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // save the column name
    if (pTkn->bIsString()) {
//      sStructColumn = pTkn->szChars();
//      bStringOperand = true;
//      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      // push token on the exprStack and type on the ExprType
//      pExprType->vPush(DM_VCH);

      vPushLiteral(pExprStack, pExprType, pTkn, DM_VCH);
    }
    else {
      vPushColumn(pExprStack, pExprType, pStreamDef, pTkn);
    }

  }

  if (pExprStack == pPostStack)
    pExprStack = m_pExprStack;

  vPushAssignCol(pExprStack, pPostStack, pExprType, eExprKind, ePreExp, pTkn, i_Col, i_DupCtr);

#ifdef WITH_DEBUG_VERSION

  // dump the TypeStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("EXPRTYPE:\n"_Stz);
  i8 nExpr = pExprType->nItems() - 1;

  while ( nExpr >= 0) {
    eDM eType = (eDM)pExprType->iPeek(nExpr--);
    vTrace("%s\n"_Stz, aDM[eType]);
  }

  // dump the ExprStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("EXPRSTACK:\n"_Stz);
  nExpr = m_pExprStack->nValues() - 1;

  while ( nExpr >= 0) {
    pTypedCell = m_pExprStack->aPeekItem(nExpr--);
    vTrace("%s\n"_Stz, pTypedCell->szStz());
  }

  // dump the NullStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("NULLSTACK:\n"_Stz);
  nExpr = m_pNullStack->nValues() - 1;

  while ( nExpr >= 0) {
    pTypedCell = m_pNullStack->aPeekItem(nExpr--);
    vTrace("%s\n"_Stz, pTypedCell->szStz());
  }

  // dump the InitStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("INITSTACK:\n"_Stz);
  nExpr = m_pInitStack->nValues() - 1;

  while ( nExpr >= 0) {
    pTypedCell = m_pInitStack->aPeekItem(nExpr--);
    vTrace("%s\n"_Stz, pTypedCell->szStz());
  }

  // dump the LowerStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("LOWERSTACK:\n"_Stz);
  nExpr = m_pLowerStack->nValues() - 1;

  while ( nExpr >= 0) {
    pTypedCell = m_pLowerStack->aPeekItem(nExpr--);
    vTrace("%s\n"_Stz, pTypedCell->szStz());
  }

  // dump the UpperStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("UPPERSTACK:\n"_Stz);
  nExpr = m_pUpperStack->nValues() - 1;

  while ( nExpr >= 0) {
    pTypedCell = m_pUpperStack->aPeekItem(nExpr--);
    vTrace("%s\n"_Stz, pTypedCell->szStz());
  }
  vTrace("--------------------------------------\n"_Stz);

  // dump the m_pGrpByStack stack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("GROUPBYSTACK:\n"_Stz);
  nExpr = m_pGrpByStack ? m_pGrpByStack->nValues() - 1 : -1;

  while ( nExpr >= 0) {
    pTypedCell = m_pGrpByStack->aPeekItem(nExpr--);
    vTrace("%s\n"_Stz, pTypedCell->szStz());
  }

  // dump the PostStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("POSTSTACK:\n"_Stz);
  nExpr = pPostStack ? pPostStack->nValues() - 1 : -1;

  while ( nExpr >= 0) {
    pTypedCell = pPostStack->aPeekItem(nExpr--);
    vTrace("%s\n"_Stz, pTypedCell->szStz());
  }
#endif

  delete pExprType;
}

/*----------------------------------------------------------------------------*/
/* allocate row in the pot                                                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthAllocPotRow(TypedStack* pPostStack, Stz* sHashExpr, bool bEosHandler,
                                  Stz* sLabel, Stz* sLabelNext) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCastRow;
  s4K sCopy;
  s4K sInfix;
  i8 i_Aggr = pPostStack ? pPostStack->nValues() - 1 : C_UNKNOWN;

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n   struct %s* p%s_pot;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("POT"_Stz, sLabel);

  // generate inFix from postFix
  //vToInfix((char*)sHashExpr, nullptr, EK_NONE, true, m_pStreamDefLft);
  //RASSERT(m_pExprStack->nValues() == 1);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the infix for the column 0
  vGetInfix(sInfix, m_pExprStack, 0);

  if ( ! bEosHandler) {
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (!aCombinedT->m_iProcessRow)");
    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_AdvanceNextRow;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  }

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCast();

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
  s256 sColNm;
  i8 i_DupCtr = 1;
  for (i8 i_Col = 0; i_Col < m_pStreamDefOut->nColumns(); i_Col++) {

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    // make the column name
    vMakeColumnNmStruct_O(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

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

      StrLib::vAppendString(&m_azCodeCrs, "\n    if (szSrcStz == (char*)C_DATUM_NULL) {");
      StrLib::vAppendString(&m_azCodeCrs, "\n      // copy the source to the destination");
      sCopy.vSprint("\n      p%s->%s = C_DATUM_NULL;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    }");
      StrLib::vAppendString(&m_azCodeCrs, "\n    else {");

      StrLib::vAppendString(&m_azCodeCrs, "\n     nSrcSize = strlen(szSrcStz) + 1;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n     // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n     szDstStz = aCombinedT->m_racPotStrArea;");
      StrLib::vAppendString(&m_azCodeCrs, "\n     strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n     // copy the source to the destination");
      sCopy.vSprint("\n     amCellAddress = (m8*) &p%s_pot->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n     iOffset = - nDifference(aCombinedT->m_racPotStrArea,amCellAddress);");
      sCopy.vSprint("\n     p%s_pot->%s = iOffset;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n     // update the pot string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n     aCombinedT->m_racPotStrArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n     vPot_RowStrSize(aCombinedT->m_pPot,iMixIn, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n    }");
      StrLib::vAppendString(&m_azCodeCrs, "\n");
    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types
      sCopy.vSprint("\n   p%s_pot->%s = p%s->%s;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                    sColNm.azChar(), xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }

    //ePE ePreExp = m_pPreExpStack ? (ePE)m_pPreExpStack->iPeek(i_Col) : PE_NONE;

//    if (ePreExp != PE_NONE) {
//
//      // get the aggregate expression for the projection
//      TypedCell *pTypedCell = pPostStack->aPeekItem(i_Aggr--);
//
//      RASSERT(pTypedCell->eType() != CN_PTRVAL);
//      StrLib::vAppendString(&m_azCodeCrs, "\n   ");
//      StrLib::vAppendString(&m_azCodeCrs, (char *) pTypedCell->szStz());
//      StrLib::vAppendString(&m_azCodeCrs, ";\n");
//
//    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto
  vGenerateGoto(sLabelNext);
}

/*----------------------------------------------------------------------------*/
/* push aggregate column on the exprStack                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushAggCol(TypedStack* pExprStack, TypedStack* pPostStack, ePE ePreExp, eEK eExprKind, eDM eExprType, bool bHashRows, i8 i_Col, i8& i_DupCtr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sColumn;
  s256 sStructColumn;
  s256 sOperation;
  TypedCell* pTypedCell;

  // make the column name
  vMakeColumnNmStruct(m_pStreamDefOut, sColumn, i_DupCtr, i_Col);

  sStructColumn.vSprint("p%s->%s"_Stz,
                        xRowRef[m_eStencilTo], sColumn.azChar());

  if (eExprKind == EK_AGGGBY) {
//    if (bHashRows)
//      sStructColumn.vSprint("p%s_pot->%s"_Stz,
//                            xRowRef[m_eStencilTo], sColumn.azChar());
//    else
//      sStructColumn.vSprint("p%s_tub->%s"_Stz,
//                            xRowRef[m_eStencilTo], sColumn.azChar());

    vInitAgg(ePreExp, eExprType);

    sOperation.vSprint("_ASSIGN(aCombinedT->m_aiPayload[%lld])"_Stz, i_Col);
    m_pInitStack->vPushStr(sOperation);
  } else {
    vInitAgg(ePreExp, eExprType);

    RASSERT(eExprKind == EK_AGGREG ||
            eExprKind == EK_AGGORD)
    sOperation.vSprint("_ASSIGN(%s)"_Stz, sStructColumn.azChar());
    m_pInitStack->vPushStr(sOperation);
  }

  switch (ePreExp) {
    case PE_ALL:
      // push col on both stacks
      sOperation.vSprint("_PSH_COL(%s)"_Stz, sStructColumn.azChar());
      pExprStack->vPushStr(sOperation);
      pPostStack->vPushStr(sOperation);

      sOperation.vSprint("_INC_INT(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStr(sOperation);
      break;

    case PE_CNT:
      if (eExprKind == EK_AGGGBY) {
        // push col on both stacks
        sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_Col);
        pExprStack->vPushStr(sOperation);
        pPostStack->vPushStr(sOperation);
      }
      else {
        RASSERT(eExprKind == EK_AGGREG ||
                eExprKind == EK_AGGORD);
        // push col on both stacks
        sOperation.vSprint("_PSH_COL(%s)"_Stz, sStructColumn.azChar());
        pExprStack->vPushStr(sOperation);
        pPostStack->vPushStr(sOperation);
      }

      sOperation.vSprint("_INC_IFNOTNULL(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStr(sOperation);
      break;

    case PE_MIN:
      // duplicate column on stack
      sOperation = aFM[eMacro_Dup[eExprType]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);

      // push col on both stacks
      sOperation.vSprint("_PSH_COL(%s)"_Stz, sStructColumn.azChar());
      pExprStack->vPushStr(sOperation);
      pPostStack->vPushStr(sOperation);

      sOperation = aFM[eMacro_Lt[eExprType][eExprType]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStr(sOperation);
      break;

    case PE_MAX:
      // duplicate column on stack
      sOperation = aFM[eMacro_Dup[eExprType]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);

      // push col on both stacks
      sOperation.vSprint("_PSH_COL(%s)"_Stz, sStructColumn.azChar());
      pExprStack->vPushStr(sOperation);
      pPostStack->vPushStr(sOperation);

      sOperation = aFM[eMacro_Gt[eExprType][eExprType]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStr(sOperation);
      break;

    case PE_SUM:
      if (eExprKind == EK_AGGGBY) {
        // push col on both stacks
        sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_Col);
        pExprStack->vPushStr(sOperation);
        pPostStack->vPushStr(sOperation);
      }
      else {
        RASSERT(eExprKind == EK_AGGREG ||
                eExprKind == EK_AGGORD);
        // push col on both stacks
        sOperation.vSprint("_PSH_COL(%s)"_Stz, sStructColumn.azChar());
        pExprStack->vPushStr(sOperation);
        pPostStack->vPushStr(sOperation);
      }

      sOperation = aFM[eMacro_Add[eExprType][eExprType]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStr(sOperation);
      break;

    case PE_NONE:
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* push arithmetic expression                                                 */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushArithmetic(TypedStack* pExprStack, IntStack* pExprType,
                                Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sOperation;
  eDM eResult;

  TypedCell* pTypedCell;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get column types
  eDM eColType1 = (eDM) pExprType->iPop();
  eDM eColType2 = (eDM) pExprType->iPop();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the logical operator
  if (pTkn->bSame("+"_Stz)) {
    sOperation = aFM[eMacro_Add[eColType1][eColType2]];
    eResult = eMacro_AddResult[eColType1][eColType2];
  }
  else
  if (pTkn->bSame("/"_Stz)) {
    sOperation = aFM[eMacro_Div[eColType1][eColType2]];
    eResult = eMacro_DivResult[eColType1][eColType2];
  }
  else
  if (pTkn->bSame("%"_Stz)) {
    sOperation = aFM[eMacro_Mod[eColType1][eColType2]];
    eResult = eMacro_ModResult[eColType1][eColType2];
  }
  else
  if (pTkn->bSame("*"_Stz)) {
    sOperation = aFM[eMacro_Mul[eColType1][eColType2]];
    eResult = eMacro_MulResult[eColType1][eColType2];
  }
  else
  if (pTkn->bSame("-"_Stz)) {
    sOperation = aFM[eMacro_Sub[eColType1][eColType2]];
    eResult = eMacro_SubResult[eColType1][eColType2];
  }
  else
    RASSERT(0);

//  if (m_pNullStack->nValues()) {
//    sOperation.vConcat("_NC"_Stz);
//  }

  sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push new expr string on the ExprStack
  pExprStack->vPushStr(sOperation);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push result type string on the ExprType stack
  pExprType->vPush((i8)eResult);
}

/*----------------------------------------------------------------------------*/
/* push cast expression                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushCast(TypedStack* pExprStack, IntStack* pExprType,
                                Token* pTkn, eDM eCastType) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sOperation;
  eDM eResult;

  TypedCell* pTypedCell;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get column type
  eDM eColType1 = (eDM) pExprType->iPop();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the cast operator

  sOperation = aFM[eMacro_Cast[eColType1][eCastType]];
  eResult = eMacro_CastResult[eColType1][eCastType];

  if (eResult == DM_VCH) {
    sOperation.vConcat("(aCombinedT->m_iErrorCode, szStrArea)"_Stz);
  }
  else {
    sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push new expr string on the ExprStack
  pExprStack->vPushStr(sOperation);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push result type string on the ExprType stack
  pExprType->vPush((i8)eResult);
}
/*----------------------------------------------------------------------------*/
/* push column                                                                */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushColumn(TypedStack* pExprStack, IntStack* pTypeStack, StreamDef* pStreamDef, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sOperation;
  s256 sColumn;

  Column *pColumn = pStreamDef->pFindColumn(pTkn->szChars());

  vMakeColumnNm(pStreamDef, sColumn, m_eStencilFrom, pColumn->iColumnNo());

  sOperation.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push token on the exprStack and type on the typeStack
  pTypeStack->vPush(Column::eDatum(pColumn->eColumnType()));
  pExprStack->vPushStz(sOperation);

  // if column nullable generate null check expressions
  if (pColumn->bIsNullable()) {
    sOperation.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());
    m_pNullStack->vPushStz(sOperation);
    sOperation.vSprint("_ISNULL()"_Stz);
    m_pNullStack->vPushStz(sOperation);
    m_pNullStack->vPushStz("_ASSIGN(bNullResult)\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* push count                                                                 */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushCount(TypedStack* pExprStack, IntStack* pTypeStack, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sOperation;
  eDM eExprType;

  Stz *sCount = pTkn->szChars() + 1;
  i8 iCount = sCount->iToI8();
  sOperation.vSprintCat("_PSH_INT(%lld)"_Stz, iCount);
  pExprStack->vPushStz(sOperation);

  // pop out iCount values off the typeStack
  while (iCount--)
    eExprType = (eDM)pTypeStack->iPop();

  // push type once
  pTypeStack->vPush(eExprType);
}

/*----------------------------------------------------------------------------*/
/* push integer literal                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushLiteral(TypedStack* pExprStack, IntStack* pTypeStack, Token* pTkn, eDM eLiteral) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sOperation;
  i8 iValue;
  d8 dValue;

  sOperation = aFM[eMacro_Push[eLiteral]];

  switch(eLiteral) {
    case DM_BCD:
      vGetValue(DM_BCD, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      break;
    case DM_DTE:
      vGetValue(DM_DTE, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      break;
    case DM_FLT:
      vGetValue(DM_FLT, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%e)"_Stz, dValue);
      break;
    case DM_INT:
      sOperation.vSprintCat("(%s)"_Stz, pTkn->szChars());
      break;
    case DM_TME:
      vGetValue(DM_TME, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      break;
    case DM_TSP:
      vGetValue(DM_TSP, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      break;
    case DM_UNK:
      sOperation.vSprintCat("(%lld)"_Stz, C_DATUM_NULL);
      break;
    case DM_VCH:
      sOperation.vSprintCat("(\"%s\")"_Stz, pTkn->szChars());
      break;
    default:
      NEVER_REACH;
  }

  pExprStack->vPushStz(sOperation);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push type on the ExprType
  pTypeStack->vPush(eLiteral);
}

/*----------------------------------------------------------------------------*/
/* push logical expression                                                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushLogical(TypedStack* pExprStack, IntStack* pExprType,
                                Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sOperation;
  eDM eResult;

  TypedCell* pTypedCell;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the logical operator
  eResult = DM_BOL;

  if (pTkn->bSame("NOT"_Stz)) {
    sOperation = aFM[FM_NOT_BOLBOL];

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get columnn types
    eDM eColType1 = (eDM) pExprType->iPop();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // expect only booleans or integers
    RASSERT(eColType1 == DM_BOL || eColType1 == DM_INT);

    sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
  }
  else {

    if (pTkn->bSame("AND"_Stz)) {
      sOperation = aFM[FM_AND_BOLBOL];
    }
    else
    if (pTkn->bSame("OR"_Stz)) {
      sOperation = aFM[FM_OR_BOLBOL];
    }
    else
    if (pTkn->bSame("XOR"_Stz)) {
      sOperation = aFM[FM_XOR_BOLBOL];

    }
    else
      RASSERT(0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get columnn types
    eDM eColType2 = (eDM) pExprType->iPop();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // expect only booleans or integers
    RASSERT(eColType2 == DM_BOL || eColType2 == DM_INT);

    sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push new infix string on the ExprStack
  pExprStack->vPushStr(sOperation);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push result type string on the ExprType stack
  pExprType->vPush((i8)eResult);
}

/*----------------------------------------------------------------------------*/
/* push aggregate column on the exprStack                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushOrdCol(TypedStack* pExprStack, IntStack* pTypeStack, eEK eExprKind, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sColumn;
  s256 sColumnPL;
  Stz* sOrdinalColumn;
  Column* pColumn;
  s256 sPush;
  bool bInitStack = false;
  bool bReturn = true;

  i8 i_ColumnOrdinal = C_UNKNOWN;
  i8 nColumns = m_pStreamDefLft->nColumns();

  // skip index column
  if (pTkn->szChars()->bContains("I"_Stz)) {
    return;
  }

  // ignore right columns
  if (pTkn->szChars()->bContains("R"_Stz)) {
    RASSERT(0);
    return;
  }

  // ignore if column ordinal relative to the group by key
  if (pTkn->szChars()->bContains("K"_Stz)) {
     return;
  }

  // ignore if column ordinal relative to the group by key
  if (pTkn->szChars()->bContains("O"_Stz)) {
    return;
  }

  // column ordinal relative to the left join table
  if (pTkn->szChars()->bContains("L"_Stz)) {

   // RASSERT(0);
    // get the column ordinal
    sOrdinalColumn = pTkn->szChars() + 2;
    i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;
//
//    // look for the column in the streamDefLft
//    if (i_ColumnOrdinal > nColumns)
//      RASSERT(0);
//
//    // select left column
//    vFormColumn(sColumn, m_pStreamDefLft, m_eStencilFrom, i_ColumnOrdinal);
//    pColumn = m_pStreamDefLft->pGetColumn(i_ColumnOrdinal);
//    //pExprStack->vPushStz(sColumn);

//    return true;
  }
  else {

    // get the column ordinal from symbolic column name
    sOrdinalColumn = pTkn->szChars() + 1;
    i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

    // look for the column in the streamDefLft
    if (i_ColumnOrdinal > nColumns)
      RASSERT(0);
  }

  // get the column
  pColumn = m_pStreamDefLft->pGetColumn(i_ColumnOrdinal);

  // select left column
  vMakeColumnNm(m_pStreamDefLft, sColumn, m_eStencilFrom, i_ColumnOrdinal);

  // push PSHCOL
  sPush.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());
  pExprStack->vPushStz(sPush);

  //if (eExprKind != EK_GROUPBY)
  //  m_pInitStack->vPushStz(sPush);

  // push the type
  pTypeStack->vPush((i8)Column::eDatum(pColumn->eColumnType()));

  // if column nullable generate null check expressions
  if (pColumn->bIsNullable()) {
    sPush.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());
    m_pNullStack->vPushStz(sPush);
    sPush.vSprint("_ISNULL()"_Stz);
    m_pNullStack->vPushStz(sPush);
    m_pNullStack->vPushStz("_ASSIGN(bNullResult)\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* push relational expression                                                 */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushRelational(TypedStack* pExprStack, IntStack* pExprType,
                                Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sOperation;

  TypedCell* pTypedCell;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get column types
  eDM eColType1 = (eDM) pExprType->iPop();
  eDM eColType2 = (eDM) pExprType->iPop();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the relational operator
  if (pTkn->bSame("=="_Stz)) {
    sOperation = aFM[eMacro_Eq[eColType1][eColType2]];
  }
  else
  if (pTkn->bSame(">="_Stz)) {
    sOperation = aFM[eMacro_Ge[eColType1][eColType2]];
  }
  else
  if (pTkn->bSame(">"_Stz)) {
    sOperation = aFM[eMacro_Gt[eColType1][eColType2]];
  }
  else
  if (pTkn->bSame("<="_Stz)) {
    sOperation = aFM[eMacro_Le[eColType1][eColType2]];
  }
  else
  if (pTkn->bSame("<"_Stz)) {
    sOperation = aFM[eMacro_Lt[eColType1][eColType2]];
  }
  else
  if (pTkn->bSame("!="_Stz)) {
    sOperation = aFM[eMacro_Ne[eColType1][eColType2]];
  }
  else
    RASSERT(0);

  sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push new expr string on the ExprStack
  pExprStack->vPushStr(sOperation);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push result type string on the ExprType stack
  pExprType->vPush((i8)DM_BOL);
}

/*----------------------------------------------------------------------------*/
/* push assign column on the exprStack                                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushAssignCol(TypedStack* pExprStack, TypedStack* pPostStack, IntStack* pTypeStack, eEK eExprKind, ePE ePreExp, Token* pTkn, i8 i_Col, i8& i_DupCtr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sColNm;
  s256 sPush;
  Column* pColumn;

  //RASSERT(pExprStack != pPostStack);

  switch(eExprKind) {

    case EK_AGGREG:
      // make the column name
      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

      if (ePreExp == PE_MIN || ePreExp == PE_MAX)
        sPush.vSprint("_ASSIGN_IFTRUE(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
      else
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());

      pExprStack->vPushStz(sPush);

      if (pPostStack->nValues() > 1) {
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
        pPostStack->vPushStz(sPush);
      }
      else
        pPostStack->vReset();
      break;

    case EK_AGGORD:

      if (ePreExp == PE_GKY)
        break;

      // make the column name
      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

      if (ePreExp == PE_MIN || ePreExp == PE_MAX)
        sPush.vSprint("_ASSIGN_IFTRUE(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
      else
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());

      if (ePreExp == PE_NONE)
        m_pInitStack->vPushStz(sPush);
      else
        pExprStack->vPushStz(sPush);

      if (pPostStack->nValues() > 1) {
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
        pPostStack->vPushStz(sPush);
      }
      else
        pPostStack->vReset();
      break;

    case EK_AGGGBY:

      if (ePreExp == PE_GKY)
        break;

      // make the column name
      vMakeColumnNm(m_pStreamDefOut, sColNm, m_eStencilTo, i_Col);

      if (ePreExp == PE_MIN || ePreExp == PE_MAX)
        sPush.vSprint("_ASSIGN_IFTRUE(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_Col);
      else
        sPush.vSprint("_ASSIGN(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_Col);

      pExprStack->vPushStz(sPush);

      if (pPostStack->nValues() > 1) {
        sPush.vSprint("_ASSIGN(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_Col);
        pPostStack->vPushStz(sPush);
      }
      else
        pPostStack->vReset();
      break;

    case EK_GROUPBY:
      RASSERT(ePreExp == PE_GBY);

      if (m_pIndexDef->eKeyKind()) {
        sPush.vSprint("_ASSIGN(*(i8*)aCombinedT->m_aEntKeyPL)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      else {
        pColumn = m_pIndexDef->pGetColumn(i_Col);

        if (pColumn->eColumnType() == CT_VARCHAR ||
            pColumn->eColumnType() == CT_CHAR) {
          sPush.vSprint("_ASSIGN_TO_ENTSTZ(aCombinedT->m_aEntWrkPL)\n"_Stz);
        }
        else {
          sPush.vSprint("_ASSIGN_TO_ENTINT(aCombinedT->m_aEntWrkPL)\n"_Stz);
        }
        pExprStack->vPushStz(sPush);
      }
      break;

    case EK_ORDGROUPBY:
      RASSERT(ePreExp == PE_GBY);

//      if (m_pIndexDef->eKeyKind()) {
//        sPush.vSprint("_ASSIGN(*(i8*)aCombinedT->m_aEntKeyPL)\n"_Stz);
//        pExprStack->vPushStz(sPush);
//      }

      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);
      sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
      pExprStack->vPushStz(sPush);
      break;

    case EK_FILTER:
      pExprStack->vPushStz("_ASSIGN(bFilterResult)\n"_Stz);
      break;

    case EK_INDEXJOIN:
      if (m_pIndexDef->nColumns() > 1) {
        vTrace("Warning: Multicolumn joins not supported\n"_Stz);
        RASSERT(m_pIndexDef->nColumns() == 1);
      }

      if (m_pIndexDef->eKeyKind()) {
        sPush.vSprint("_ASSIGN(*(i8*)aCombinedT->m_aEntKeyJ)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      else {
        sPush.vSprint("_ASSIGN_TO_ENTSTZ(aCombinedT->m_aEntWrkJ)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      break;

    case EK_INDEXSCAN:
      // make the column name
      //vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

      if (m_pIndexDef->nColumns() > 1) {
        vTrace("Warning: Multicolumn indexes not supported\n"_Stz);
        RASSERT(m_pIndexDef->nColumns() == 1);
      }

      if (m_pIndexDef->eKeyKind()) {
        sPush.vSprint("_ASSIGN(*(i8*)aCombinedT->m_aEntKeyT)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      else {
        sPush.vSprint("_ASSIGN_TO_ENTSTZ(aCombinedT->m_aEntWrkT)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      break;

    case EK_PROJECT:

      // make the column name
      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);
      sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());

      pExprStack->vPushStz(sPush);
      break;

    case EK_SORT:
      RASSERT(ePreExp == PE_SRT);

      if (m_pIndexDef->eKeyKind()) {
        sPush.vSprint("_ASSIGN(*(i8*)aCombinedT->m_aEntKeySRT)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      else {
        pColumn = m_pIndexDef->pGetColumn(i_Col);

        if (pColumn->eColumnType() == CT_VARCHAR ||
            pColumn->eColumnType() == CT_CHAR) {
          sPush.vSprint("_ASSIGN_TO_ENTSTZ(aCombinedT->m_aEntWrkSRT)\n"_Stz);
        }
        else {
          sPush.vSprint("_ASSIGN_TO_ENTINT(aCombinedT->m_aEntWrkSRT)\n"_Stz);
        }
        pExprStack->vPushStz(sPush);
      }
      break;

    case EK_TOP:
      if (m_pIndexDef->nColumns() > 1) {
        vTrace("Warning: Multicolumn indexes not supported\n"_Stz);
        RASSERT(m_pIndexDef->nColumns() == 1);
      }

      if (m_pIndexDef->eKeyKind()) {
        sPush.vSprint("_ASSIGN(*(i8*)aCombinedT->m_aEntKeyTOP)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      else {
        sPush.vSprint("_ASSIGN_TO_ENTSTZ(aCombinedT->m_aEntWrkTOP)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      break;

    default:
      NEVER_REACH;
  }


}

/*----------------------------------------------------------------------------*/
/* push assign column on the exprStack                                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushFunc(TypedStack* pExprStack, IntStack* pTypeStack, ePE ePreExp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sOperation;
  eDM eExpr1;
  eDM eExpr2;
  eDM eExpr3;

  switch(ePreExp) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // BETWEEN OPERATOR
    case PE_BET:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_Bet[eExpr1][eExpr2]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // IN EXPRESSIONS LIST OPERATOR
    case PE_INX:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_Inx[eExpr1][eExpr2]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR
    case PE_LK2:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_LIKE2(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR with ESCAPE
    case PE_LK3:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      eExpr3 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_LIKE3(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // MATCH LEADING (ABD%)
    case PE_MLE:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_MATCH_LEADING(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // MATCH  MIDDLE (%ABC%)
    case PE_MMD:
      // match leading
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_MATCH_MIDDLE(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // MATCH MIDDLE 1 (%ABC%DEF%)
    case PE_MM1:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_MATCH_MIDDLE1(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // MATCH MIDDLE 2 (%ABC%DEF%GHI%)
//    case PE_MM2:
//      eExpr1 = (eDM)pTypeStack->iPop();
//      eExpr2 = (eDM)pTypeStack->iPop();
//      sOperation.vConcat("_MATCH_MIDDLE2(aCombinedT->m_iErrorCode)"_Stz);
//      pExprStack->vPushStz(sOperation);
//      pTypeStack->vPush(DM_BOL);
//      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // MATCH TRAILING (%ABC)
    case PE_MTR:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_MATCH_TRAILING(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR
    case PE_SS2:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_STR_SUBSTRING2((char*)szStrArea, aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_VCH);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR
    case PE_SS3:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      eExpr3 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_STR_SUBSTRING3((char*)szStrArea, aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_VCH);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR
    case PE_XDY:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractDay[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR
    case PE_XHR:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractHour[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR
    case PE_XMN:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractMinute[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR
    case PE_XMT:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractMonth[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LIKE OPERATOR
    case PE_XSC:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractSecond[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // LIKE OPERATOR
    case PE_XYR:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractYear[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* allocate row in tub                                                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthAllocTubRow(TypedStack* pPostStack, bool bEosHandler,
                                  Stz* sLabel, Stz* sLabelNext)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCastRow;
  s4K sCopy;

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  sCopy.vSprint("\n    struct %s* p%s_tub;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("TUB"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate rows processed check
  if (bEosHandler) {
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    if (!aCombinedT->m_iRowsProcessed)");
    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_ReturnFinal;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  }
  else {
    vGenerateProcessRow("L_AdvanceNextRow"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate row from the tub
  StrLib::vAppendString(&m_azCodeCrs, "\n    // allocate a row from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aRowTub = (byte*)aTub_AllotRow(aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea);");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n    p%s_tub = (struct %s*)aCombinedT->m_aRowTub;\n"_Stz,xRowRef[m_eStencilFrom].m_szInclude,
                   xRowRef[m_eStencilFrom].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column
  s256 sColNm;
  i8 i_DupCtr = 1;
  for (i8 i_Col = 0; i_Col < m_pStreamDefOut->nColumns(); i_Col++) {

    // make the column name
    vMakeColumnNmStruct_O(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n    // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    // In case of only filter there is no AggTypeStack
    //ePE ePreExp = m_pPreExpStack ? (ePE)m_pPreExpStack->iPeek(i_Col) : PE_NONE;

    // POST PROJECTION
    //if (ePreExp != PE_NONE) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the post aggregation
      vGenerateExprCode(pPostStack, ""_Stz);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate error check
      vGenerateErrorCheck();

    //}

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      StrLib::vAppendString(&m_azCodeCrs, "\n    // get the source string");
      sCopy.vSprint("\n    szSrcStz = (char*)p%s->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n    if (szSrcStz == (char*)C_DATUM_NULL) {");
      StrLib::vAppendString(&m_azCodeCrs, "\n      // copy the source to the destination");
      sCopy.vSprint("\n      p%s_tub->%s = C_DATUM_NULL;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n    }");
      StrLib::vAppendString(&m_azCodeCrs, "\n    else {");

      StrLib::vAppendString(&m_azCodeCrs, "\n      nSrcSize = strlen(szSrcStz) + 1;");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n      // get the destination string");
      StrLib::vAppendString(&m_azCodeCrs, "\n      szDstStz = aCombinedT->m_racTubStrArea;");
      StrLib::vAppendString(&m_azCodeCrs, "\n      strncpy(szDstStz, szSrcStz, nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      StrLib::vAppendString(&m_azCodeCrs, "\n      // copy the source to the destination");
      sCopy.vSprint("\n      amCellAddress = (m8*) &p%s_tub->%s;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n      iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);");
      sCopy.vSprint("\n      p%s_tub->%s = iOffset;"_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n      // update the tub string size");
      StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_racTubStrArea += nSrcSize;");
      StrLib::vAppendString(&m_azCodeCrs, "\n      vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);");
      StrLib::vAppendString(&m_azCodeCrs, "\n    }");
      StrLib::vAppendString(&m_azCodeCrs, "\n");
    } else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // all other types
      sCopy.vSprint("\n    p%s_tub->%s = p%s->%s;\n"_Stz, xRowRef[m_eStencilFrom].m_szInclude,
                    sColNm.azChar(), xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto
  vGenerateGoto(sLabelNext);
}

/*----------------------------------------------------------------------------*/
/* generate code for processing EOF with the TOP index                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vTopIndexTraversal(IndexDef* pIndexDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // TOP INDEX TRAVERSAL");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // begin iterating the payload index
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_aEntIntTOP = (struct Ent_int*)aIndexTree_begin(aCombinedT->m_aIndexTreeTOP, &aCombinedT->m_aEntBufferTOP);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n    while (aCombinedT->m_aEntIntTOP) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      // allocate a topPayloadRow from the tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aTopPayloadRow = (byte*)aTub_AllotRow(aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // calculate the payload address
  if (pIndexDef->eKeyKind()) {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiTopPayload = (i8 *) ((char *) aCombinedT->m_aEntIntTOP");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + 8);");
  }
  else {
    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aiTopPayload = (i8 *) ((char *) aCombinedT->m_aEntIntTOP");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + sizeof(struct Ent_int)");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + ((struct Ent_int*)aCombinedT->m_aEntIntTOP)->m_iIntElm0");
    StrLib::vAppendString(&m_azCodeCrs, "\n                  + 8);");
  }

  vPayloadToBuffer(pIndexDef, nullptr, PL_TOP);

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n     aCombinedT->m_aEntIntTOP = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferTOP, aCombinedT->m_aEntIntTOP);");
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
}

/*----------------------------------------------------------------------------*/
/* generate code for IN expr                                                  */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateInExpr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, "\n    bFilterResult = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iArg1 = *(--paW);");
  StrLib::vAppendString(&m_azCodeCrs, "\n    iInCount = *(--paW);");

  StrLib::vAppendString(&m_azCodeCrs, "\n    while(iInCount--) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      iArg2 = *(--paW);");
  StrLib::vAppendString(&m_azCodeCrs, "\n      if (iArg1 == iArg2) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n         bFilterResult = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n         break;");
  StrLib::vAppendString(&m_azCodeCrs, "\n      }");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n    while(iInCount-- > 0) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n      iArg2 = *(--paW);");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
  StrLib::vAppendString(&m_azCodeCrs, "\n    *(paW++) = bFilterResult;");
}

/*----------------------------------------------------------------------------*/
/* generate code for check for process row                                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateProcessRow(Stz* sLabel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  StrLib::vAppendString(&m_azCodeCrs, "\n    if (!aCombinedT->m_iProcessRow)");
  sCopy.vSprint("\n      goto %s;"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  StrLib::vAppendString(&m_azCodeCrs, "\n");
}

/*----------------------------------------------------------------------------*/
/* generate code for goto label                                               */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateGoto(Stz* sLabel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  sCopy.vSprint("\n    goto %s;"_Stz, sLabel);
  StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
}

/*----------------------------------------------------------------------------*/
/* generate code for the expression                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateExprCode(TypedStack* pTypedStack, Stz* sIndent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  i8 nExpr = pTypedStack ? pTypedStack->nValues() - 1 : C_UNKNOWN ;

  TypedCell *pTypedCell;
  while ( nExpr >= 0) {
    pTypedCell = pTypedStack->aPeekItem(nExpr--);
    Stz* sExpr = pTypedCell->szStz();
//    if (sExpr->bSame("_INX"_Stz)) {
//      vGenerateInExpr();
//    }
//    else {
      sCopy.vSprint("\n    %s%s"_Stz, sIndent, sExpr->azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
//  }
}

/*----------------------------------------------------------------------------*/
/* generate code for keyExists filter expression                              */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateKeyExistsFilter(StreamDef* pStreamDef, Stz* sFilterExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // generate the code for the projected expressions
  vGenerateExpr((char *) sFilterExpr, nullptr, EK_FILTER, false, pStreamDef);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for the expression
  vGenerateExprCode(m_pExprStack, ""_Stz);

  StrLib::vAppendString(&m_azCodeCrs, "\n    if (bFilterResult) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  StrLib::vAppendString(&m_azCodeCrs, "\n    } else { ");
  StrLib::vAppendString(&m_azCodeCrs, "\n       aCombinedT->m_iProcessRow = 0;");
  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n       // move to next entry");
  StrLib::vAppendString(&m_azCodeCrs, "\n       aCombinedT->m_aEntIntJ = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferJ, aCombinedT->m_aEntIntJ);");

  StrLib::vAppendString(&m_azCodeCrs, "\n       goto L_KeyExistsDeref;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");
}

/*----------------------------------------------------------------------------*/
/* generate the streamDef into a struct                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateStruct(u8 uSelectColumns) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  StrLib::vSprint((Stz *) m_caCode, "\n\n    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "_Stz);

  switch (m_eStencilTo) {
    case SR_IJ:
      StrLib::vSprintCat((Stz *) m_caCode, "\n    // Inflate J-Table"_Stz);
      break;
    case SR_IT:
      StrLib::vSprintCat((Stz *) m_caCode, "\n    // Inflate T-Table"_Stz);
      break;
    case SR_AT:
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

  i8 i_DupCtr = 1;
  s256 sColNm;
  for (i8 i_Col = 0; i_Col < m_pStreamDefOut->nColumns(); i_Col++) {

    if (uSelectColumns) {
      if (!BitLib::bIsOn(uSelectColumns, i_Col))
        continue;
    }
    vMakeColumnNm(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

    Column* pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    switch(pColumn->eColumnType()) {
      case CT_BOOLEAN:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Bol_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_DATE:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Dte_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_DECIMAL:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Bcd_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_FLOAT:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Flt_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_INTEGER:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Int_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_INTERVAL_DT:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Ivl_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_INTERVAL_YM:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Ivl_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_TIME:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Tme_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_TIMESTAMP:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Tsp_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_UNKNOWN:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Unk_T %s;\n"_Stz, sColNm.azChar());
        break;
      case CT_CHAR:
      case CT_VARCHAR:
        StrLib::vSprintCat((Stz *) m_caCode, "      union Vch_T %s;\n"_Stz, sColNm.azChar());
        break;
      default:
        NEVER_REACH
    }
  }

  StrLib::vSprintCat((Stz *) m_caCode, "    };"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get infix for projection                                                   */
/*----------------------------------------------------------------------------*/
Stz* CodeSynth::szInfix(TypedStack* pExprStack, i8 iProject)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sInfix;

  TypedCell *pTypedCell = pExprStack->aPeekItem(iProject);

  if (pTypedCell->eType() == CN_PTRVAL) {
    Token *Tkn = _pToken(pTypedCell->pObj());
    sInfix = Tkn->szChars();
  } else {
    sInfix = pTypedCell->szStz();
  }

  return sInfix;
}

/*----------------------------------------------------------------------------*/
/* get infix for projection                                                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGetInfix(s4K &szInfix, TypedStack* pExprStack, i8 iProject)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  TypedCell *pTypedCell = pExprStack->aPeekItem(iProject);

  if (pTypedCell->eType() == CN_PTRVAL) {
    Token *Tkn = _pToken(pTypedCell->pObj());
    szInfix = Tkn->szChars();
  } else {
    szInfix = pTypedCell->szStz();
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
