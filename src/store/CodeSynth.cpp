
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
/* debug macro                                                                */
/*----------------------------------------------------------------------------*/
#ifdef WITH_DEBUG
  #define TEST_TOINFIX()                                              \
    if (m_pExprStack->nValues() != m_pStreamDefOut->nColumns()) {     \
      for (i8 i_Val = 0; i_Val < m_pExprStack->nValues(); i_Val++) {  \
        TypedCell *pTypedCell = m_pExprStack->aPeekItem(i_Val);       \
        vTrace("**%s\n"_Stz, pTypedCell->azChar());                   \
      }                                                               \
    }                                                                 \
    ASSERT(m_pExprStack->nValues() == m_pStreamDefOut->nColumns());
#else
  #define TEST_TOINFIX()                                              \
    RASSERT(m_pExprStack->nValues() == m_pStreamDefOut->nColumns());
#endif

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
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

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
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

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
                     eSR eStencilFrom, eSR eStencilTo)
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
  m_pLowerStack = new TypedStack(KK_DATA_STACK);
  m_pUpperStack = new TypedStack(KK_DATA_STACK);
  m_pInitStack  = new TypedStack(KK_DATA_STACK);

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

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
/* set post stack                                                             */
/*----------------------------------------------------------------------------*/
//void CodeSynth::vPostStack(TypedStack* pPostStack) {
//
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
//
//  m_pPostStack = pPostStack;
//}
//
///*----------------------------------------------------------------------------*/
///* get post stack                                                             */
///*----------------------------------------------------------------------------*/
//TypedStack* CodeSynth::pPostStack(void) {
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
//
//  return m_pPostStack;
//}

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
bool CodeSynth::bSynthAggGby(IndexDef *pIndexDef, IntStack *pAggTypeStack, Stz *sProjectExpr,
                             Stz *sLabel, Stz *sLabelLoop, bool bHashRows) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
  s4K sCopy;
  bool bProjectResult = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("AGGREG-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the stencil
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the infix from eStencilFrom
  //vToInfix((char *) sProjectExpr, pAggTypeStack, PJ_GBY, bHashRows, m_pStreamDefLft);
  //TEST_TOINFIX();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow(sLabelLoop);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the project group by code
  vGeneratePrjGby(pIndexDef, pAggTypeStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto label
  vGenerateGoto(sLabelLoop);

  // cleanup
  m_pExprStack->vReset();
  m_pInitStack->vReset();

  return bProjectResult;
}

/*----------------------------------------------------------------------------*/
/* synthesize the EOS                                                         */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthEosPayload(IndexDef* pIndexDef, IntStack* pAggTypeStack,
                                 TypedStack* pPostStack) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate payload traversal
  vPayloadTraversal(pIndexDef, pAggTypeStack, pPostStack);
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
void CodeSynth::vSynthFilter(Stz* sFilterExpr, Stz* sLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K sInfix;
  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("FILTER-OP"_Stz, sLabel);

  // generate the code for the projected expressions
  vGenerateExpr((char *) sFilterExpr, nullptr, nullptr, EK_FILTER, false, m_pStreamDefLft);

  // convert postfix to infix
  //vToInfix((char*)sFilterExpr, nullptr, EK_NONE, false, m_pStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the infix for the column 0
  //vGetInfix(sInfix, m_pExprStack, 0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate stencil cast
  vGenerateStencilCast();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for the expression
  vGenerateExprCode(m_pExprStack, ""_Stz);

  StrLib::vAppendString(&m_azCodeCrs, "\n    if (bFilterResult) { ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 1;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    } else { ");
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    }");

  // cleanup
  m_pExprStack->vReset();
}

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
  s256 sCastRow;
  s256 sInfix;

  // generate the init stub
  vGenerateInitStub("INDEXJOIN-OP"_Stz, sLabel);

  // generate the infix from eStencilFrom
  //vToInfix((char*)sJoinExpr, nullptr, EK_NONE, false, m_pStreamDefLft);
  //RASSERT(m_pExprStack->nValues() == 1);

  // generate the code for the index join expr
  vGenerateExpr((char *) sJoinExpr, nullptr, nullptr, EK_INDEXJOIN, false, m_pStreamDefLft);

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the join key
//  if (pIndexDefJ->eKeyKind()) {
//
//    TypedCell *pTypedCell = m_pExprStack->aPeekItem(0);
//    RASSERT(pTypedCell->eType() != CN_PTRVAL);
//    sInfix = pTypedCell->szStz();
//
    // ALKA: COME BACK TO THIS
//    // check for nulls
//    StrLib::vAppendString(&m_azCodeCrs, "\n");
//    sCopy.vSprint("\n    if (%s == %lld) {"_Stz, sInfix.azChar(), C_DATUM_NULL);
//    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//    StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_iProcessRow = 0;");
//    StrLib::vAppendString(&m_azCodeCrs, "\n      goto L_AdvanceNextRow;");
//    StrLib::vAppendString(&m_azCodeCrs, "\n    }");
//    StrLib::vAppendString(&m_azCodeCrs, "\n");
//
//    StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the entKey from T-table");
//    sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKeyJ = %s;"_Stz, sInfix.azChar());
//    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//    StrLib::vAppendString(&m_azCodeCrs, "\n      if (aCombinedT->m_bTrace)");
//    StrLib::vAppendString(&m_azCodeCrs, "\n        vEntWrk_print(aCombinedT->m_aEntWrkJ, aCombinedT->m_aIndexDefJ);");
//  }
//  else {
//    StrLib::vAppendString(&m_azCodeCrs, "\nassert(0);");
//  }

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
void CodeSynth::vSynthIndexScan(IndexDef* pIndexDef, Stz* sIndexScanExpr, Stz* sLabel,
                                Stz* sLabelNext, u8 uSelectColumns) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
  s4K sCopy;
  s256 sCastRow;
  s4K sInfix;

  // generate the init stub
  vGenerateInitStub("INDEXSCAN-OP"_Stz, sLabel);

  // generate the code for the index scan expr
  vGenerateExpr((char *) sIndexScanExpr, nullptr, nullptr, EK_INDEXSCAN, false, m_pStreamDefLft);

  //vToInfix((char *) sIndexScanExpr, nullptr, EK_NONE, false, m_pStreamDefLft);
  //RASSERT(m_pLowerStack->nValues() == 1 || m_pUpperStack->nValues() == 1);

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

//    i8 nExpr = m_pLowerStack->nValues() - 1;
//
//    TypedCell *pTypedCell;
//    while ( nExpr >= 0) {
//      pTypedCell = m_pLowerStack->aPeekItem(nExpr--);
//      Stz* sInfix = pTypedCell->szStz();
//      sCopy.vSprint("\n    %s"_Stz, sInfix->azChar());
//      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//    }

    // populate the lower bound key
//    if (pIndexDef->eKeyKind()) {
//
//      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      // get the infix for the column 0
//      vGetInfix(sInfix, m_pLowerStack, 0);
//
//      RASSERT(pIndexDef->nColumns() == 1);
//
//      StrLib::vAppendString(&m_azCodeCrs, "\n");
//      StrLib::vAppendString(&m_azCodeCrs, "\n    // populate EntKeyT with lower bound value");
//
//      sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKeyT = %s;"_Stz, sInfix.azChar());
//      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//
//    } else {
//      StrLib::vAppendString(&m_azCodeCrs, "\nassert(0);");
//    }

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
    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n    // process the row");
    StrLib::vAppendString(&m_azCodeCrs, "\n    aCombinedT->m_iProcessRow = 1;");
    StrLib::vAppendString(&m_azCodeCrs, "\n");
  } else {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // populate the upper bound key
//    if (pIndexDef->eKeyKind()) {
//
//      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      // get the infix for the column 0
//      vGetInfix(sInfix, m_pUpperStack, 0);
//
//      RASSERT(pIndexDef->nColumns() == 1);
//
//      StrLib::vAppendString(&m_azCodeCrs, "\n");
//      StrLib::vAppendString(&m_azCodeCrs, "\n    // populate EntKeyT with upper bound value");
//
//      sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKeyT = %s;"_Stz, sInfix.azChar());
//      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//
//    } else {
//      StrLib::vAppendString(&m_azCodeCrs, "\nassert(0);");
//    }

     StrLib::vAppendString(&m_azCodeCrs, "\n    // populate EntKeyT with upper bound value");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the upper bound expr
     vGenerateExprCode(m_pUpperStack, ""_Stz);
//      i8 nExpr = m_pUpperStack->nValues() - 1;
//
//      TypedCell *pTypedCell;
//      while ( nExpr >= 0) {
//        pTypedCell = m_pUpperStack->aPeekItem(nExpr--);
//        Stz* sInfix = pTypedCell->szStz();
//        sCopy.vSprint("\n    %s"_Stz, sInfix->azChar());
//        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//      }

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
  //vToInfix((char*)sKeyExpr, nullptr, EK_NONE, false, m_pStreamDefLft);

  // generate the code for the key expression
  vGenerateExpr((char *) sKeyExpr, nullptr, nullptr, EK_INDEXJOIN, false, m_pStreamDefLft);

  m_eStencilFrom = eStencilFrom;
  //RASSERT(m_pExprStack->nValues() == 1);

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the join key
//  if (pIndexDefJ->eKeyKind()) {
//
//    TypedCell *pTypedCell = m_pExprStack->aPeekItem(0);
//    RASSERT(pTypedCell->eType() != CN_PTRVAL);
//    sInfix = pTypedCell->szStz();
//
//    StrLib::vAppendString(&m_azCodeCrs, "\n");
//    StrLib::vAppendString(&m_azCodeCrs, "\n     // populate the entKey from T-table");
//    sCopy.vSprint("\n    *(i8*)aCombinedT->m_aEntKeyJ = %s;"_Stz, sInfix.azChar());
//    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//    StrLib::vAppendString(&m_azCodeCrs, "\n    if (aCombinedT->m_bTrace)");
//    StrLib::vAppendString(&m_azCodeCrs, "\n      vEntWrk_print(aCombinedT->m_aEntWrkJ, aCombinedT->m_aIndexDefJ);");
//  }
//  else {
//    StrLib::vAppendString(&m_azCodeCrs, "\nassert(0);");
//  }

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

  // generate inFix from postFix for the filter expr
  delete m_pTokenVec;
  //vToInfix((char*)sFilterExpr, nullptr, EK_NONE, false, pStreamDefRht);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the infix for the column 0
  //vGetInfix(sInfix, m_pExprStack, 0);

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
  void CodeSynth::vSynthOrdAgg(IndexDef *pIndexDef, eEK eExprKind,
                               IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel, Stz *sLabelNext,
                               Stz* sLabelLoop, bool bHashRows) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("ORDAGG-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateStencilCast();
  
  // generate the infix from eStencilFrom
  //vTrace("Postfix: %s\n"_Stz, sProjectExpr);
  vToInfix((char *) sProjectExpr, pAggTypeStack, eExprKind, bHashRows, m_pStreamDefLft);
  TEST_TOINFIX();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow(sLabelLoop);

  RASSERT(eExprKind == EK_AGGGBY);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate ordered gby
  vOrdAggGby(pIndexDef, pAggTypeStack, sLabelNext, sLabelLoop);

  // cleanup
  m_pExprStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* synthesize the projection of aggregate expressions                         */
/*----------------------------------------------------------------------------*/
TypedStack* CodeSynth::pSynthPrjAgg(IntStack *pAggTypeStack, Stz *sProjectExpr,
                                    Stz *sLabel, Stz *sLabelLoop, bool bHashRows) {
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
  //vToInfix((char *) sProjectExpr, pAggTypeStack, PJ_AGG, bHashRows, m_pStreamDefLft);
  vGenerateExpr((char *) sProjectExpr, pAggTypeStack, pPostStack, EK_AGGREG, bHashRows, m_pStreamDefLft);
  //TEST_TOINFIX();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for peoject aggregates
  vGeneratePrjAgg(pAggTypeStack);

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
bool CodeSynth::bSynthPrjExp(IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel,
                             Stz *sLabelNext, Stz *sLabelLoop, bool bHashRows) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("PROJECT-OP"_Stz, sLabel);

  // generate the code for the projected expressions
  vGenerateExpr((char *) sProjectExpr, pAggTypeStack, nullptr, EK_PROJECT, bHashRows, m_pStreamDefLft);
  //TEST_TOINFIX();

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
/* synthesize the small aggregate                                             */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthSmlAgg(IndexDef *pIndexDef, eEK eExprKind,
                             IntStack *pAggTypeStack, Stz *sProjectExpr, Stz *sLabel,
                             Stz* sLabelLoop, bool bHashRows) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sCopy;
  s256 sColNm;

  RASSERT(eExprKind == EK_AGGGBY);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init stub
  vGenerateInitStub("SMLAGG-OP"_Stz, sLabel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the stencil
  vGenerateStencilCast();

  // generate the infix from eStencilFrom
  //vTrace("Postfix: %s\n"_Stz, sProjectExpr);
  vToInfix((char *) sProjectExpr, pAggTypeStack, eExprKind,
           bHashRows, m_pStreamDefLft);
  RASSERT(m_pExprStack->nValues() == m_pStreamDefOut->nColumns());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate processRow check
  vGenerateProcessRow(sLabelLoop);

  // find the key in the tub
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // find key in tub");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    bFound = 0;");
  StrLib::vAppendString(&m_azCodeCrs, "\n    for (iIndex = 0; iIndex < aCombinedT->m_nAggRows; iIndex++) {");
  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n      pSR_AT[iIndex] = ADDR_ADD(struct SR_AT*, aCombinedT->m_aRowTub , -1*iIndex*aCombinedT->m_iTubRowSize);");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  for (i8 i_iCol=0; i_iCol < pIndexDef->nColumns(); i_iCol++) {
    Column* pColumn = pIndexDef->pGetColumn(i_iCol);

    vMakeIdxColNmStruct_O(pIndexDef, sColNm, i_iCol);

    switch(pColumn->eColumnType()) {
      case CT_VARCHAR:
        sCopy.vSprint("\n      iOffset = p%s[iIndex]->%s;"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());//,
                      //xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      amCellAddress = (m8*)&p%s[iIndex]->%s;"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar()); //,
                      //xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
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
                      xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
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

  // generate inFix from postFix
  //vToInfix((char*)sKeyExpr, nullptr, EK_NONE, false, m_pStreamDefLft);

  // generate the code for the projected expressions
  vGenerateExpr((char *) sKeyExpr, nullptr, nullptr, EK_TOP, false, m_pStreamDefLft);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the infix for the column 0
  //vGetInfix(sInfix, m_pExprStack, 0);

  StrLib::vAppendString(&m_azCodeCrs, "\n");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // - - - - - - - - - - - - - - ");
  StrLib::vAppendString(&m_azCodeCrs, "\n    // populate the payload key");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // reset the key size / m_nElmSeqSize
  StrLib::vAppendString(&m_azCodeCrs, "    aCombinedT->m_aEntIntPL->m_iIntElm0 = 0;");

  vGenerateExprCode(m_pExprStack, ""_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the payload key
  //vPayloadKey(pIndexDef, "m_aEntWrkTOP"_Stz, "m_aEntKeyTOP"_Stz);



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
/* generate code for aggregate                                                */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateAggCode(IntStack *pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sColNm;
  s4K  sInfix;
  s4K  sCopy;

  i8 nColumns = m_pStreamDefOut->nColumns();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column
  i8 i_Col = 0;
  i8 i_DupCtr = 1;
  i8 iExpr = m_pExprStack->nValues() - 1;

  TypedCell *pTypedCell;
  while ( iExpr >= 0) {
    pTypedCell = m_pExprStack->aPeekItem(iExpr--);
    Stz* sInfix = pTypedCell->szStz();
    sCopy.vSprint("\n    %s"_Stz, sInfix->azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }

  /*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each projected column
  i8 i_DupCtr = 1;
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    i8 i_Project = nColumns - i_Col - 1;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the infix for the projection
    vGetInfix(sInfix, m_pExprStack, i_Project);

    // make the column name
    vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);
    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregation code for the column
    switch (eAggType) {

      case AG_ALL:
        sCopy.vSprint("\n      p%s->%s++;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_CNT:
        RASSERT(pColumn->eColumnType() != CT_VARCHAR);
        RASSERT(pColumn->eColumnType() != CT_CHAR);

        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n     p%s->%s++;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                      sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_MIN:
        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n        if ( %s < p%s->%s) {\n"_Stz, sInfix.azChar(), xRowRef[m_eStencilTo].m_szInclude,
                      sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n          p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                      sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n        }");
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_MAX:
        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n        if ( %s > p%s->%s) {\n"_Stz, sInfix.azChar(), xRowRef[m_eStencilTo].m_szInclude,
                      sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n          p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                      sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n        }");
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_SUM:
        RASSERT(pColumn->eColumnType() != CT_VARCHAR);
        RASSERT(pColumn->eColumnType() != CT_CHAR);

        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n     p%s->%s += %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                      sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      default:
        break;
    }
  }
   */
}

/*----------------------------------------------------------------------------*/
/* generate code for aggregate initialization                                 */
/*----------------------------------------------------------------------------*/
  void CodeSynth::vGenerateAggInitCode(IntStack *pAggTypeStack) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

    s4K sCopy;
    s4K sInfix;
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

    s256 sColNm;
    i8 i_DupCtr = 1;
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

      StrLib::vAppendString(&m_azCodeCrs, "\n");
      StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
      sCopy.vSprint("\n      // init column %lld"_Stz, i_Col);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, "\n");

      // make the column name
      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

      eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);
      i8 i_Project = nColumns - i_Col - 1;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // get the infix for the projection
      vGetInfix(sInfix, m_pExprStack, i_Project);

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
          sCopy.vSprint("\n      p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                        sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n      if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {");
          StrLib::vAppendString(&m_azCodeCrs, "\n        return 0;");
          StrLib::vAppendString(&m_azCodeCrs, "\n      }");

          break;
      }
    }

  }

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

  StrLib::vAppendString(&m_azCodeCrs, "\n");
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
    case DM_BCD: {
      SN_Bcd snBcd;
      iKeyValue = snBcd.i(&sValue, AM::pFmt());
    }
      break;
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

  s4K  sInfix;
  s256 sCastRow;
  s4K sCopy;
  s256 sColNm;
  //i8 nColumns = m_pStreamDefOut->nColumns();

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the init code for each aggregate column expression
  vGenerateAggInitCode(pAggTypeStack);

  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
  StrLib::vAppendString(&m_azCodeCrs, "\n");

  // is it a new key ?

  s256 sColOutNm;
  i8 i_DupCtr = 1;
  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);
    //sColNm = pColumn->szColNm();
    //StrLib::vToUpper((char*)sColNm.azChar());
    vMakeIdxColNmStruct(pIndexDef, sColNm, i_Col);

    // NOT SURE: COME BACK
    // make the column name
    vMakeColumnNmStruct(m_pStreamDefOut, sColOutNm, i_DupCtr, i_Col);

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
  vGenerateAggCode(pAggTypeStack);

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
void CodeSynth::vPayloadStore(IndexDef* pIndexDef, IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K  sInfix;
  s4K  sCopy;
  i8   i_Col;

  i8 nColumns = m_pStreamDefOut->nColumns();

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
  i_Col = 0;
  i8 iPayLoad = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the infix for the projection
    vGetInfix(sInfix, m_pExprStack, i_Project);

    StrLib::vAppendString(&m_azCodeCrs, "\n");
    StrLib::vAppendString(&m_azCodeCrs, "\n       // - - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n       // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    // is the column part of the aggregate group by key ?
    Column* pIndexColumn = pIndexDef->pFindColumn(pColumn->szColNm());

    if (pIndexColumn) {
      i_Col++;
      continue;
    }

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // initialization of aggregate columns
    switch(eAggType) {

      case AG_SUM:
        RASSERT ((pColumn->eColumnType() != CT_VARCHAR) &&
                 (pColumn->eColumnType() != CT_CHAR));
        sCopy.vSprint("\n       // init agg_sum payload %lld"_Stz, iPayLoad);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n       aCombinedT->m_aiPayload[%lld] = %s;"_Stz, iPayLoad, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_CNT:
        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n       aCombinedT->m_aiPayload[%lld] = 1;"_Stz, iPayLoad);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_ALL:
        sCopy.vSprint("\n       aCombinedT->m_aiPayload[%lld] = 1;"_Stz, iPayLoad);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        break;

      default:
        if ((pColumn->eColumnType() == CT_VARCHAR) ||
            (pColumn->eColumnType() == CT_CHAR)) {
          sCopy.vSprint("\n       nSrcSize = strlen((char*)%s);"_Stz, sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

          sCopy.vSprint("\n       strncpy((char*)&aCombinedT->m_aiPayload[%lld],(char*)%s,nSrcSize);"_Stz, iPayLoad,
                        sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        } else {
          sCopy.vSprint("\n       aCombinedT->m_aiPayload[%lld] = %s;"_Stz, iPayLoad, sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        }
        StrLib::vAppendString(&m_azCodeCrs, "\n      if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {");
        StrLib::vAppendString(&m_azCodeCrs, "\n        return 0;");
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;
    }

    iPayLoad++;
    i_Col++;
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for small agg store for groupby                              */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSmlAggLoad(IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K  sInfix;
  s256 sColNm;
  s4K  sCopy;
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
  i8 i_DupCtr = 1;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the infix for the projection
    vGetInfix(sInfix, m_pExprStack, i_Project);

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
                      xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n      if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {");
        StrLib::vAppendString(&m_azCodeCrs, "\n        return 0;");
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      case AG_CNT:
        sCopy.vSprint("\n        // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        // agg_cnt column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        p%s[iIndex]->%s++;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      case AG_ALL:
        sCopy.vSprint("\n        // column %lld"_Stz, i_Col);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        // agg_cntall column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n        p%s[iIndex]->%s++;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
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

  s4K  sInfix;
  s4K  sCopy;
  s256 sColNm;
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
  i8 i_DupCtr = 0;
  for (i8 i_Project = nColumns - 1; i_Project >= 0; i_Project--) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the infix for the projection
    vGetInfix(sInfix, m_pExprStack, i_Project);

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);
    vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);
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
                      xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n      if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {");
        StrLib::vAppendString(&m_azCodeCrs, "\n        return 0;");
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      case AG_CNT:
        sCopy.vSprint("\n       p%s[aCombinedT->m_nAggRows]->%s = 1;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n");
        break;

      case AG_ALL:
        sCopy.vSprint("\n       p%s[aCombinedT->m_nAggRows]->%s = 1;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
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
          sCopy.vSprint("\n       amCellAddress = (m8*) &p%s[aCombinedT->m_nAggRows]->%s;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n       iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);");


          sCopy.vSprint("\n       p%s[aCombinedT->m_nAggRows]->%s = iOffset;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n");
          StrLib::vAppendString(&m_azCodeCrs, "\n       // update the tub string size");
          StrLib::vAppendString(&m_azCodeCrs, "\n       aCombinedT->m_racTubStrArea += nSrcSize;");
          StrLib::vAppendString(&m_azCodeCrs, "\n       vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);");
          StrLib::vAppendString(&m_azCodeCrs, "\n");
        } else {
          sCopy.vSprint("\n           p%s[aCombinedT->m_nAggRows]->%s = %s;"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(), sInfix.azChar());
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

  s256 sColNm;
  s4K  sInfix;
  s4K  sCopy;

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

  i8 iPayLoad = 0;

  i8 i_DupCtr = 0;
  for (i8 i_Project = nColumns - 1, i_Col = 0; i_Project >= 0; i_Project--, i_Col++) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the infix for the projection
    vGetInfix(sInfix, m_pExprStack, i_Project);

    // make the column name
    vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

    eAG eAggType = (eAG) pAggTypeStack->iPeek(i_Col);

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    // is the column part of the aggregate group by key ?
    Column* pIndexColumn = pIndexDef->pFindColumn(pColumn->szColNm());

    if (pIndexColumn) {
      continue;
    }

    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - -  ");
    sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    StrLib::vAppendString(&m_azCodeCrs, "\n");

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggregate columns
    switch(eAggType) {

      case AG_ALL:
        sCopy.vSprint("\n      // agg_cntall column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      aCombinedT->m_aiPayload[%lld]++;\n"_Stz, iPayLoad);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        break;

      case AG_CNT:
        sCopy.vSprint("\n      // agg_cnt column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n      aCombinedT->m_aiPayload[%lld]++;\n"_Stz, iPayLoad);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_MIN:
        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n        if ( %s < p%s->%s) {\n"_Stz, sInfix.azChar(), xRowRef[m_eStencilTo].m_szInclude,
                      sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n          p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                      sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n        }");
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_MAX:
        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n        if ( %s > p%s->%s) {\n"_Stz, sInfix.azChar(), xRowRef[m_eStencilTo].m_szInclude,
                      sColNm.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n          p%s->%s = %s;\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar(),
                      sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n        }");
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      case AG_SUM:
        sCopy.vSprint("\n      // agg_sum column %lld"_Stz, i_Col-1);
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

        sCopy.vSprint("\n      if (FM_ISNOTNULL(%s, &aCombinedT->m_iErrorCode)) {\n"_Stz, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        sCopy.vSprint("\n      aCombinedT->m_aiPayload[%lld] += %s;\n"_Stz, iPayLoad, sInfix.azChar());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
        StrLib::vAppendString(&m_azCodeCrs, "\n      }");
        break;

      default:
        //StrLib::vAppendString(&m_azCodeCrs, "\nprintf(\"PAYLOAD AGG KEY: %%lld\\n\",aCombinedT->m_aiPayload[1]);");
        break;
    }

    iPayLoad++;
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for processing EOF                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadToBuffer(IndexDef* pIndexDef, TypedStack* pPostStack, IntStack* pAggTypeStack, bool bTop)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s256 sInfix;
  s256 sCastRow;
  s4K sCopy;
  i8 nColumns = m_pStreamDefOut->nColumns();

  // no postStack and aggTypeStack when traversing TOP payload index
  if (bTop) {
    //RASSERT( !m_pPostStack );
    RASSERT( !pAggTypeStack )
  }
  i8 i_Aggr = pPostStack ? pPostStack->nValues() - 1 : C_UNKNOWN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cast the ProjectRow to eStencilTo structure
  sCastRow.vSprint("\n      p%s = (struct %s*)aCombinedT->%s;\n"_Stz,
                   xRowRef[m_eStencilTo].m_szInclude,
                   xRowRef[m_eStencilTo].m_szInclude,
                   bTop ? "m_aTopPayloadRow"_Stz : "m_aPayloadRow"_Stz);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  StrLib::vAppendString(&m_azCodeCrs, "\n");

  i8 iPayLoad = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column
  s256 sColNm;
  i8 i_DupCtr = 1;

  StrLib::vAppendString(&m_azCodeCrs, "\n      iElemSize = 0;");

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    StrLib::vAppendString(&m_azCodeCrs, "\n      // - - - - - - - - - - - - - - ");
    sCopy.vSprint("\n      // column %lld"_Stz, i_Col);
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

    // make the column name
    vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

    Column *pColumn = m_pStreamDefOut->pGetColumn(i_Col);

    // get aggregate type
    // no AggTypeStack when traversing TOP payload index
    eAG eAggType = pAggTypeStack ? (eAG)pAggTypeStack->iPeek(i_Col) : AG_NONE;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is column in the key ?
    Column* pIndexColumn = pIndexDef->pFindColumn(pColumn->szColNm());

    if (pIndexColumn) {

      // cannot be an aggregate
      RASSERT(eAggType == AG_NONE);

      if (pIndexDef->eKeyKind() == KE_GENERAL) {

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // VARCHAR column
        if (pColumn->eColumnType() == CT_VARCHAR ||
            pColumn->eColumnType() == CT_CHAR) {

          RASSERT(pIndexDef->eKeyKind() == KE_GENERAL);

          sCopy.vSprint(
              "\n      szSrcStz = ((char*)(&(((struct Ent_int*)aCombinedT->%s)->m_iIntElm0)) + 8 + iElemSize);"_Stz,
              bTop ? "m_aEntIntTOP"_Stz : "m_aEntIntPL"_Stz);
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


          StrLib::vAppendString(&m_azCodeCrs,"\n      if (FM_ISNOTNULL((i8)szSrcStz, &aCombinedT->m_iErrorCode)) {");

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
              bTop ? "m_aEntIntTOP"_Stz : "m_aEntIntPL"_Stz);
          StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
          StrLib::vAppendString(&m_azCodeCrs, "\n      iElemSize += 8;");
        }
      }
      else {
        sCopy.vSprint("\n      p%s->%s = ((struct Ent_int*)aCombinedT->%s)->m_iIntElm0 + (%lld*8);\n"_Stz,
                      xRowRef[m_eStencilTo].m_szInclude,
                      sColNm.azChar(),
                      bTop ? "m_aEntIntTOP"_Stz : "m_aEntIntPL"_Stz,
                      pIndexColumn->iColumnNo());
        StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      }

      continue;
    }

    // must be an aggregate
    //RASSERT(eAggType != AG_NONE);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // VARCHAR column
    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {

      sCopy.vSprint("\n      szSrcStz = (char*)&aCombinedT->%s[%lld];"_Stz,
                    bTop ? "m_aiTopPayload"_Stz : "m_aiPayload"_Stz, iPayLoad);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());

      sCopy.vSprint("\n      nSrcSize = strlen((char*)&aCombinedT->%s[%lld]) + 1;"_Stz,
                    bTop ? "m_aiTopPayload"_Stz : "m_aiPayload"_Stz, iPayLoad);
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
                    bTop ? "m_aiTopPayload"_Stz : "m_aiPayload"_Stz, iPayLoad);
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
    }
    iPayLoad++;

    if (eAggType != AG_NONE) {

      // get the aggregate expression for the projection
      TypedCell *pTypedCell = pPostStack->aPeekItem(i_Aggr--);

      RASSERT(pTypedCell->eType() != CN_PTRVAL);
      StrLib::vAppendString(&m_azCodeCrs, "\n      ");
      sCopy.vSprint("\n      p%s->%s = "_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
      StrLib::vAppendString(&m_azCodeCrs, (char *) pTypedCell->szStz());
      StrLib::vAppendString(&m_azCodeCrs, ";\n");

    }
  }

}

/*----------------------------------------------------------------------------*/
/* generate code for processing EOF                                           */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPayloadTraversal(IndexDef* pIndexDef,IntStack* pAggTypeStack, TypedStack* pPostStack)
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

  vPayloadToBuffer(pIndexDef, pPostStack, pAggTypeStack, false);

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n      aCombinedT->m_aEntIntPL = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferPL, aCombinedT->m_aEntIntPL);");

  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
}

/*----------------------------------------------------------------------------*/
/* generate code for Project Aggregates                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGeneratePrjAgg(IntStack* pAggTypeStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CODESYNTH,_FOOTER_);

  s4K  sInfix;
  s256 sCastRow;
  s4K sCopy;
  //i8 nColumns = m_pStreamDefOut->nColumns();

  sCastRow.vSprint("\n    p%s = (struct %s*)aCombinedT->m_aRowCellArrayAgg;\n"_Stz,xRowRef[m_eStencilTo].m_szInclude, xRowRef[m_eStencilTo].m_szInclude);
  StrLib::vAppendString(&m_azCodeCrs, sCastRow.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allocate aggregate ProjectRow
  StrLib::vAppendString(&m_azCodeCrs, "\n    if ( aCombinedT->m_iProjectInit == 0 ) {");

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate init code for each projected agg column
  //vGenerateAggInitCode(pAggTypeStack);
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
  vGenerateAggCode(pAggTypeStack);

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

//  i8 nExpr = m_pExprStack->nValues() - 1;
//
//  TypedCell *pTypedCell;
//  while ( nExpr >= 0) {
//    pTypedCell = m_pExprStack->aPeekItem(nExpr--);
//    Stz* sInfix = pTypedCell->szStz();
//    sCopy.vSprint("\n    %s"_Stz, sInfix->azChar());
//    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate error check
  vGenerateErrorCheck();
}

/*----------------------------------------------------------------------------*/
/* generate code for Project aggregates with groupby                          */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGeneratePrjGby(IndexDef* pIndexDef, IntStack* pAggTypeStack) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the payload key
  vPayloadKey(pIndexDef, "m_aEntWrkPL"_Stz, "m_aEntKeyPL"_Stz);

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

  s256 sOperation;
  s32  sInitValue;
  Bcd  xBcd;
  i8   iInitValue = 0;

  switch(eAggType) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_ALL:
      sOperation.vSprintCat("_PSH_INT(0)"_Stz);
      m_pInitStack->vPushStz(sOperation);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_CNT:
      sOperation.vSprintCat("_PSH_INT(0)"_Stz);
      m_pInitStack->vPushStz(sOperation);
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case AG_MIN:
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
    case AG_MAX:
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
    case AG_SUM:

      switch(eExprType) {
        case DM_BCD:
          xBcd.vSet(0,0);
          sOperation.vSprintCat("_PSH_BCD(xBcd.T.m_i8)"_Stz);
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
      NEVER_REACH;
  }
  // push on the aggregate init stack
  m_pInitStack->vPushStz(sOperation);
}


/*----------------------------------------------------------------------------*/
/* convert postfix to infix                                                   */
/*----------------------------------------------------------------------------*/
void CodeSynth::vToInfix(char* sExpr, IntStack* pAggTypeStack, eEK eExprKind,
                         bool bHashRows, StreamDef* pStreamDef) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);
/*
  s4K sInfix;
  s4K sOperand1;
  s4K sOperand2;
  s4K sStrCmp;
  s4K sOperation;

  s256 sColumn;
  s256 sStructColumn;
  s256 sValue;

  eDM eExprType = DM_UNK;
  eAG eAggType = AG_NONE;

  bool bIndexJoin = false;
  bool bLowerBound = false;
  bool bUpperBound = false;
  bool bStringOperand = false;

  TypedCell *pTypedCell;
  TypedStack *pExprStack;
  IntStack *pExprType;

  Token *pTkn;

  i8 i_Col = 0;
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set working ExprStack to be m_pExprStack
  pExprStack = m_pExprStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the post aggregate expression stack
  if (eExprKind != EK_NONE) {
    m_pPostStack = new TypedStack(KK_DATA_STACK);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // set working ExprStack to be the post aggregate expression stack
    if (AG_NONE != (eAG)pAggTypeStack->iPeek(i_Col)) {
      pExprStack = m_pPostStack;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate tokens of the expression
  for (i8 i_Tkn = 0; i_Tkn < m_pTokenVec->nItems(); i_Tkn++) {

    pTkn = m_pTokenVec->pGet(i_Tkn);

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

    if (pTkn->bSame("groupby"_Stz)) {
      break;
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

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the upper bound stack
      pExprStack = m_pUpperStack;
      bUpperBound = true;
      bLowerBound = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // push the ordinal column on the expr stack
    if (pTkn->szChars()->bContains("$"_Stz)) {

      vPushOrdCol(pExprStack, pExprType, pTkn);
      continue;
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
    // get the cast expression type
    if (pTkn->bSame("casttype"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      eCastType = StreamDefBase::eDatum(pTkn->szChars());
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip the alias
    if (pTkn->bSame("as"_Stz)) {
      i_Tkn++;
      continue;
    }

    if (pTkn->bSame(","_Stz)) {
      // skip the comma
      i_Col++;
      if (AG_NONE != (eAG)pAggTypeStack->iPeek(i_Col)) {
        if (!m_pPostStack)
          m_pPostStack = new TypedStack(KK_DATA_STACK);
        pExprStack = m_pPostStack;
      }
      else {
        pExprStack = m_pExprStack;
      }

      continue;
    }

    if (pTkn->bSame("}"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the normal expression stack
      pExprStack = m_pExprStack;
      continue;
    }

    if (pTkn->bSame("cast"_Stz)) {
      vPushCast(pExprStack, pExprType, pTkn, eCastType);
      continue;
    }

    if (pTkn->bSame(")"_Stz)) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // aggregate follows, generate init expressions in the m_aInitStack
      eAggType = (eAG)pAggTypeStack->iPeek(i_Col);
      vInitAgg(eAggType, eExprType);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the normal expression stack
      pExprStack = m_pExprStack;
      continue;
    }

    if (pTkn->bSame("count"_Stz)) {
      eAggType = AG_NONE;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the post aggregate expression stack
      RASSERT(m_pPostStack);
      pExprStack = m_pPostStack;

      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
      continue;
    }

    if (pTkn->bSame("countall"_Stz)) {
      eAggType = AG_NONE;

      // push a dummy pointer for asserts
      // we assert that for every projection there is a value in the expr stack
      m_pExprStack->vPushPtr(pTkn);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the post aggregate expression stack
      RASSERT(m_pPostStack);
      pExprStack = m_pPostStack;

      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
      continue;
    }

    if (pTkn->bSame("min"_Stz)) {
      eAggType = AG_NONE;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the post aggregate expression stack
      RASSERT(m_pPostStack);
      pExprStack = m_pPostStack;

      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
      continue;
    }

    if (pTkn->bSame("max"_Stz)) {
      eAggType = AG_NONE;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the post aggregate expression stack
      RASSERT(m_pPostStack);
      pExprStack = m_pPostStack;

      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
      continue;
    }

    if (pTkn->bSame("sum"_Stz)) {
      eAggType = AG_NONE;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the post aggregate expression stack
      RASSERT(m_pPostStack);
      pExprStack = m_pPostStack;

      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
      continue;
    }

    if (pTkn->bIsInteger()) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      pExprType->vPush(DM_INT);
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("negate"_Stz) ) {
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bIsDecimal()) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      pExprType->vPush(DM_BCD);
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("DATE"_Stz) ) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      iValue = iGetValue(DM_DTE, pTkn->azChars());
      pTkn->vUpdateChars((Stz*)StrLib::wToString(iValue, sValue.azChar()), m_pTokenVec);
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      pExprType->vPush(DM_DTE);
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("TIME"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      iValue = iGetValue(DM_TME, pTkn->azChars());
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      pExprType->vPush(DM_TME);
      pTkn->vUpdateChars((Stz*)StrLib::wToString(iValue, sValue.azChar()), m_pTokenVec);
      pExprStack->vPushPtr(pTkn);
      continue;
    }

    if (pTkn->bSame("TIMESTAMP"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      iValue = iGetValue(DM_TSP, pTkn->azChars());
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      pExprType->vPush(DM_TSP);
      pTkn->vUpdateChars((Stz*)StrLib::wToString(iValue, sValue.azChar()), m_pTokenVec);
      pExprStack->vPushPtr(pTkn);
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

      // skip == op if indexJoinn expr
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

      if (pTkn->bIsString()) {
        pExprStack->vPushPtr(pTkn);
        continue;
      }

      // pop operand1
      pTypedCell = pExprStack->aPeekItem(0);
      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *pOperand1 = _pToken(pExprStack->pPopPtr(__func__));
        sOperand1 = pOperand1->szChars();
      } else {
        pExprStack->vPopStr(sOperand1, __func__);
      }
      eType = (eDM)pExprType->iPop();

      // pop operand2
      pTypedCell = pExprStack->aPeekItem(0);
      if (pTypedCell->eType() == CN_PTRVAL) {
        Token *pOperand2 = _pToken(pExprStack->pPopPtr(__func__));
        sOperand2 = pOperand2->szChars();
      } else {
        pExprStack->vPopStr(sOperand2, __func__);
      }
      eType = (eDM)pExprType->iPop();

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
        // push new infix string on the ExprStack
        pExprStack->vPushStr(sOperation);
      }

      pExprType->vPush((i8)DM_BOL);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // save the column name
    if (pTkn->bIsString()) {
      sStructColumn = pTkn->szChars();
      bStringOperand = true;
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      pExprType->vPush(DM_VCH);
      pExprStack->vPushStr(sStructColumn);
    }
    else {
      Column* pColumn = pStreamDef->pFindColumn(pTkn->szChars());

      //StrLib::vToUpper(pTkn->azChars());
      //sColumn = pTkn->szChars();
      //sStructColumn.vSprint("p%s->%s"_Stz, xRowRef[m_eStencilFrom], sColumn.azChar());

      vMakeColumnNm(pStreamDef, sColumn, m_eStencilFrom, pColumn->iColumnNo());

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      pExprType->vPush(Column::eDatum(pColumn->eColumnType()));
      pExprStack->vPushStr(sColumn);
    }

  }

  //RASSERT(1 == pExprType->nItems());
  //RASSERT(pExprType->iPop() == eExprKind);
  delete pExprType;
  */
}

/*----------------------------------------------------------------------------*/
/* convert postfix to expr macros                                             */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateExpr(char* sExpr, IntStack* pAggTypeStack, TypedStack* pPostStack,
                              eEK eExprKind, bool bHashRows, StreamDef* pStreamDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sInfix;
  s4K sOperand1;
  s4K sOperand2;
  s4K sStrCmp;
  s4K sOperation;

  s256 sColumn;
  s256 sStructColumn;
  s256 sValue;

  eDM eExprType = DM_UNK;
  eAG eAggType = AG_NONE;

  bool bIndexJoin = false;
  bool bLowerBound = false;
  bool bUpperBound = false;
  bool bStringOperand = false;

  TypedCell *pTypedCell;
  TypedStack *pExprStack;
  IntStack *pExprType;

  Token *pTkn;

  i8 i_Col = 0;
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set working ExprStack to be m_pExprStack
  pExprStack = m_pExprStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the post aggregate expression stack
//  if (eExprKind != EK_NONE) {
//    m_pPostStack = new TypedStack(KK_DATA_STACK);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set working ExprStack to be the post aggregate expression stack
  if (pAggTypeStack && AG_NONE != (eAG)pAggTypeStack->iPeek(i_Col)) {
    pExprStack = pPostStack;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate tokens of the expression
  for (i8 i_Tkn = 0; i_Tkn < m_pTokenVec->nItems(); i_Tkn++) {

    pTkn = m_pTokenVec->pGet(i_Tkn);

    //vTrace("TKN: %s\n"_Stz, pTkn->azChars());

//    if (pTkn->bSame("groupby"_Stz)) {
//      break;
//    }

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
        vPushAssignCol(pExprStack, pPostStack, pExprType, eExprKind, pTkn, i_Col, i_DupCtr);

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
        vPushAssignCol(pExprStack, pPostStack, pExprType, eExprKind, pTkn, i_Col, i_DupCtr);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the upper bound stack
      pExprStack = m_pUpperStack;
      bUpperBound = true;
      bLowerBound = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // push the ordinal column on the expr stack
    if (pTkn->szChars()->bContains("$"_Stz)) {
      vPushOrdCol(pExprStack, pExprType, pTkn);
      continue;
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
    // get the cast expression type
    if (pTkn->bSame("casttype"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      eCastType = StreamDefBase::eDatum(pTkn->szChars());
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip the alias
    if (pTkn->bSame("as"_Stz)) {
      i_Tkn++;
      continue;
    }

    if (pTkn->bSame(","_Stz)) {
      // skip the comma

      // push assign column
      vPushAssignCol(m_pExprStack, pPostStack, pExprType, eExprKind, pTkn, i_Col, i_DupCtr);

      i_Col++;
      if (pAggTypeStack && AG_NONE != (eAG)pAggTypeStack->iPeek(i_Col)) {
        //if (!pPostStack)
        //  m_pPostStack = new TypedStack(KK_DATA_STACK);
        pExprStack = pPostStack;
      }
      else {
        pExprStack = m_pExprStack;
      }

      continue;
    }

    if (pTkn->bSame("}"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the normal expression stack
      pExprStack = m_pExprStack;
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

//    if (pTkn->bSame("count"_Stz)) {
//      eAggType = AG_NONE;
//
//      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      // set working ExprStack to be the post aggregate expression stack
//      RASSERT(m_pPostStack);
//      pExprStack = m_pPostStack;
//
//      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
//      continue;
//    }
//
//    if (pTkn->bSame("countall"_Stz)) {
//      eAggType = AG_NONE;
//
//      // push a dummy pointer for asserts
//      // we assert that for every projection there is a value in the expr stack
//      m_pExprStack->vPushPtr(pTkn);
//
//      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      // set working ExprStack to be the post aggregate expression stack
//      RASSERT(m_pPostStack);
//      pExprStack = m_pPostStack;
//
//      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
//      continue;
//    }
//
//    if (pTkn->bSame("min"_Stz)) {
//      eAggType = AG_NONE;
//
//      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      // set working ExprStack to be the post aggregate expression stack
//      RASSERT(m_pPostStack);
//      pExprStack = m_pPostStack;
//
//      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
//      continue;
//    }
//
//    if (pTkn->bSame("max"_Stz)) {
//      eAggType = AG_NONE;
//
//      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//      // set working ExprStack to be the post aggregate expression stack
//      RASSERT(m_pPostStack);
//      pExprStack = m_pPostStack;
//
//      vPushAggCol(pExprStack, eExprKind, bHashRows, i_Col, i_DupCtr);
//      continue;
//    }
//
    if (pTkn->bSame("sum"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // aggregate follows, generate init expressions in the m_aInitStack
      eAggType = (eAG)pAggTypeStack->iPeek(i_Col);
      RASSERT(AG_SUM == eAggType);

      vPushAggCol(pExprStack, pPostStack, eAggType, eExprKind, eExprType, bHashRows, i_Col, i_DupCtr);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the post aggregate expression stack
      RASSERT(pPostStack);
      pExprStack = pPostStack;


      eAggType = AG_NONE;

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

  //RASSERT(1 == pExprType->nItems());
  //RASSERT(pExprType->iPop() == eExprType);

  vPushAssignCol(m_pExprStack, pPostStack, pExprType, eExprKind, pTkn, i_Col, i_DupCtr);
/*
  // dump the ExprStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("EXPRSTACK:\n"_Stz);
  i8 nExpr = m_pExprStack->nValues() - 1;

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

  // dump the PostStack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("POSTSTACK:\n"_Stz);
  nExpr = pPostStack ? pPostStack->nValues() - 1 : -1;

  while ( nExpr >= 0) {
    pTypedCell = pPostStack->aPeekItem(nExpr--);
    vTrace("%s\n"_Stz, pTypedCell->szStz());
  }
*/
  delete pExprType;
}

/*----------------------------------------------------------------------------*/
/* allocate row in the pot                                                    */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthAllocPotRow(IntStack* pAggTypeStack, TypedStack* pPostStack, Stz* sHashExpr, bool bEosHandler,
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
  vToInfix((char*)sHashExpr, nullptr, EK_NONE, true, m_pStreamDefLft);
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

    eAG eAggType = pAggTypeStack ? (eAG)pAggTypeStack->iPeek(i_Col) : AG_NONE;

    if (eAggType != AG_NONE) {

      // get the aggregate expression for the projection
      TypedCell *pTypedCell = pPostStack->aPeekItem(i_Aggr--);

      RASSERT(pTypedCell->eType() != CN_PTRVAL);
      StrLib::vAppendString(&m_azCodeCrs, "\n   ");
      StrLib::vAppendString(&m_azCodeCrs, (char *) pTypedCell->szStz());
      StrLib::vAppendString(&m_azCodeCrs, ";\n");

    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate goto
  vGenerateGoto(sLabelNext);
}

/*----------------------------------------------------------------------------*/
/* push aggregate column on the exprStack                                     */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushAggCol(TypedStack* pExprStack, TypedStack* pPostStack, eAG eAggType, eEK eExprKind, eDM eExprType, bool bHashRows, i8 i_Col, i8& i_DupCtr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sColumn;
  s256 sStructColumn;
  s256 sOperation;

  // make the column name
  vMakeColumnNmStruct(m_pStreamDefOut, sColumn, i_DupCtr, i_Col);

  //sColumn = m_pStreamDefOut->pGetColumn(i_Col)->szColNm();
  //StrLib::vToUpper(sColumn.azChar());

  vInitAgg(eAggType, eExprType);

  if (eExprKind == EK_AGGGBY) {
    if (bHashRows)
      sStructColumn.vSprint("p%s_pot->%s"_Stz,
                            xRowRef[m_eStencilTo], sColumn.azChar());
    else
      sStructColumn.vSprint("p%s_tub->%s"_Stz,
                            xRowRef[m_eStencilTo], sColumn.azChar());
  } else {
    RASSERT(eExprKind == EK_AGGREG)
    sStructColumn.vSprint("p%s->%s"_Stz,
                          xRowRef[m_eStencilTo], sColumn.azChar());
  }
  sOperation.vSprint("_ASSIGN(%s)"_Stz, sStructColumn.azChar());
  m_pInitStack->vPushStr(sOperation);

  sOperation.vSprint("_PSH_COL(%s)"_Stz, sStructColumn.azChar());

  // push col on both stacks
  pExprStack->vPushStr(sOperation);
  pPostStack->vPushStr(sOperation);

  sOperation.vSprint("_ADD_INTINT(aCombinedT->m_iErrorCode)"_Stz);
  pExprStack->vPushStr(sOperation);

}

/*----------------------------------------------------------------------------*/
/* push arithmetic expression                                                 */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushArithmetic(TypedStack* pExprStack, IntStack* pExprType,
                                Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s4K sOperand1;
  s4K sOperand2;
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
/* push integer literal                                                       */
/*----------------------------------------------------------------------------*/
void CodeSynth::vPushLiteral(TypedStack* pExprStack, IntStack* pTypeStack, Token* pTkn, eDM eLiteral) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sOperation;
  i8 iValue;

  sOperation = aFM[eMacro_Push[eLiteral]];

  switch(eLiteral) {
    case DM_BCD:
      iValue = iGetValue(DM_BCD, pTkn->azChars());
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      break;
    case DM_DTE:
      iValue = iGetValue(DM_DTE, pTkn->azChars());
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      break;
    case DM_INT:
      sOperation.vSprintCat("(%s)"_Stz, pTkn->szChars());
      break;
    case DM_TME:
      iValue = iGetValue(DM_TME, pTkn->azChars());
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      break;
    case DM_TSP:
      iValue = iGetValue(DM_TSP, pTkn->azChars());
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

  s4K sOperand1;
  s4K sOperand2;
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
void CodeSynth::vPushOrdCol(TypedStack* pExprStack, IntStack* pTypeStack, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sColumn;
  Stz* sOrdinalColumn;
  Column* pColumn;
  s256 sPush;

  i8 i_ColumnOrdinal = C_UNKNOWN;
  i8 nColumns = m_pStreamDefLft->nColumns();

  // skip index column
  if (pTkn->szChars()->bContains("I"_Stz)) {
    return;
  }

  // ignore if column ordinal relative to the group by key
  if (pTkn->szChars()->bContains("K"_Stz)) {
    RASSERT(0);
    return;
  }

  // ignore right columns
  if (pTkn->szChars()->bContains("R"_Stz)) {
    RASSERT(0);
    return;
  }

  // column ordinal relative to the left join table
  if (pTkn->szChars()->bContains("L"_Stz)) {

    RASSERT(0);
//    // get the column ordinal
//    sOrdinalColumn = pTkn->szChars() + 2;
//    i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;
//
//    // look for the column in the streamDefLft
//    if (i_ColumnOrdinal > nColumns)
//      RASSERT(0);
//
//    // select left column
//    vFormColumn(sColumn, m_pStreamDefLft, m_eStencilFrom, i_ColumnOrdinal);
//    pColumn = m_pStreamDefLft->pGetColumn(i_ColumnOrdinal);
//    //pExprStack->vPushStz(sColumn);

    return;
  }
  else {

    // get the column ordinal from symbolic column name
    sOrdinalColumn = pTkn->szChars() + 1;
    i_ColumnOrdinal = sOrdinalColumn->iToI8() - 1;

    // look for the column in the streamDefLft
    if (i_ColumnOrdinal > nColumns)
      RASSERT(0);

    // get the column
    pColumn = m_pStreamDefLft->pGetColumn(i_ColumnOrdinal);

    // select left column
    vMakeColumnNm(m_pStreamDefLft, sColumn, m_eStencilFrom, i_ColumnOrdinal);

  }

  // push PSHCOL
  sPush.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());
  pExprStack->vPushStz(sPush);

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
  if (pTkn->bSame("!"_Stz)) {
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
void CodeSynth::vPushAssignCol(TypedStack* pExprStack, TypedStack* pPostStack, IntStack* pTypeStack, eEK eExprKind, Token* pTkn, i8 i_Col, i8& i_DupCtr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sColNm;
  s256 sPush;

  RASSERT(pExprStack != pPostStack);

  switch(eExprKind) {

    case EK_AGGREG:
      // make the column name
      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);

      sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
      pExprStack->vPushStz(sPush);

      if (pPostStack->nValues() > 1) {
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef[m_eStencilTo].m_szInclude, sColNm.azChar());
        pPostStack->vPushStz(sPush);
      }
      else
        pPostStack->vReset();
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
/* allocate row in tub                                                        */
/*----------------------------------------------------------------------------*/
void CodeSynth::vSynthAllocTubRow(IntStack* pAggTypeStack, TypedStack* pPostStack, bool bEosHandler,
                                  Stz* sLabel, Stz* sLabelNext)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  s256 sCastRow;
  s4K sCopy;
  i8 i_Aggr = pPostStack ? pPostStack->nValues() - 1 : C_UNKNOWN;

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
    eAG eAggType = pAggTypeStack ? (eAG)pAggTypeStack->iPeek(i_Col) : AG_NONE;

    // POST PROJECTION
    if (eAggType != AG_NONE) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // generate the code for the postt aggregatation
      vGenerateExprCode(pPostStack, ""_Stz);

//      // get the aggregate expression for the projection
//      TypedCell *pTypedCell = m_pPostStack->aPeekItem(i_Aggr--);
//
//      RASSERT(pTypedCell->eType() != CN_PTRVAL);
//      sCopy.vSprint("\n    p%s_tub->%s = "_Stz, xRowRef[m_eStencilFrom].m_szInclude, sColNm.azChar());
//      StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
//      StrLib::vAppendString(&m_azCodeCrs, (char *) pTypedCell->szStz());
//      StrLib::vAppendString(&m_azCodeCrs, ";");

    }

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

  vPayloadToBuffer(pIndexDef, nullptr, nullptr, true);

  // go to next entry
  StrLib::vAppendString(&m_azCodeCrs, "\n     aCombinedT->m_aEntIntTOP = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferTOP, aCombinedT->m_aEntIntTOP);");
  StrLib::vAppendString(&m_azCodeCrs, "\n   }");
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

  i8 nExpr = pTypedStack->nValues() - 1;

  TypedCell *pTypedCell;
  while ( nExpr >= 0) {
    pTypedCell = pTypedStack->aPeekItem(nExpr--);
    Stz* sInfix = pTypedCell->szStz();
    sCopy.vSprint("\n    %s%s"_Stz, sIndent, sInfix->azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }
}

/*----------------------------------------------------------------------------*/
/* generate code for keyExists filter expression                              */
/*----------------------------------------------------------------------------*/
void CodeSynth::vGenerateKeyExistsFilter(StreamDef* pStreamDef, Stz* sFilterExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH, _FOOTER_);

  // generate the code for the projected expressions
  vGenerateExpr((char *) sFilterExpr, nullptr, nullptr, EK_FILTER, false, pStreamDef);

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
