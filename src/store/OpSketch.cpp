
/*============================================================================*/
/* OpSketch.cpp -- Stencil Operator Sketch                                     */
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

#include "OpSketch.hpp"

RowRef_v2 xRowRef_v2__[]
    {
        { "none"_Stz },   // SR_NONE

        {"SR_AT"_Stz},    // SR_AT
        {"SR_IJ"_Stz},    // SR_IJ
        {"SR_IT"_Stz},    // SR_IT
        {"SR_PL"_Stz},    // SR_PL
        {"SR_PT"_Stz},    // SR_PT
        {"SR_TJ"_Stz},    // SR_TJ
        {"SR_H"_Stz},     // SR_H

        {nullptr}
    };

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,OpSketch,CN_OPSKETCH)
__aNewFunction(OpSketch,CN_OPSKETCH)
__vDeleteFunction(OpSketch,CN_OPSKETCH)
__aFooterFunction(OpSketch)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpSketch::OpSketch(void)
: Object(CN_OPSKETCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_eStencilSlot  = LS_NONE;
  m_eExpressionKind = EK_NONE;

  m_pTokenVec = nullptr;
  m_sExprString = ""_Stz;

  // definitions
  m_pStreamDefLft = nullptr;
  m_pStreamDefOut= nullptr;
  m_pIndexDef= nullptr;

  // labels
  m_sLabelOp   = ""_Stz;
  m_sLabelNext = ""_Stz;
  m_sLabelLoop = ""_Stz;

  // stencils
  m_eStencilFrom = SR_NONE;
  m_eStencilTo   = SR_NONE;

  // expression stacks
  m_pExprStack   = nullptr;
  m_pNullStack   = nullptr;
  m_pGbyStack    = nullptr;
  m_pOrdAggStack = nullptr;
  m_pLowerStack  = nullptr;
  m_pUpperStack  = nullptr;
  m_pInitStack   = nullptr;

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack = nullptr;;

  m_iLimitRows = 0;

  /////////////////////////////////
  // TO GO
  m_pStructSynthA = nullptr;
  m_pStructSynthB = nullptr;
  m_pCodeSynthA   = nullptr;
  m_pCodeSynthB   = nullptr;
  m_pCodeSynthC   = nullptr;
  /////////////////////////////////

  _checkInitialised_(CN_OPSKETCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpSketch::OpSketch(Stz* sExprStr, eSR eStencilIn, eSR eStencilOut, StreamDef* pStreamDefIn, StreamDef* pStreamDefOut)
    : Object(CN_OPSKETCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_eStencilSlot  = LS_NONE;
  m_eExpressionKind = EK_NONE;

  m_pTokenVec = nullptr;

  m_sExprString   = sExprStr;

  // definitions
  m_pStreamDefLft = pStreamDefIn;
  m_pStreamDefOut= pStreamDefOut;
  m_pIndexDef= nullptr;

  // labels
  m_sLabelOp   = ""_Stz;
  m_sLabelNext = ""_Stz;
  m_sLabelLoop = ""_Stz;

  // stencils
  m_eStencilFrom = SR_NONE;
  m_eStencilTo   = SR_NONE;

  // expression stacks
  m_pExprStack   = new TypedStack(KK_DATA_STACK);
  m_pNullStack   = nullptr;
  m_pGbyStack    = nullptr;
  m_pOrdAggStack = nullptr;
  m_pLowerStack  = nullptr;
  m_pUpperStack  = nullptr;
  m_pInitStack   = nullptr;

  m_sLowerPosition = ""_Stz;
  m_sUpperPosition = ""_Stz;

  m_pPreExpStack = nullptr;;

  m_iLimitRows = 0;

  /////////////////////////////////
  // TO GO
  m_pStructSynthA = nullptr;
  m_pStructSynthB = nullptr;
  m_pCodeSynthA   = nullptr;
  m_pCodeSynthB   = nullptr;
  m_pCodeSynthC   = nullptr;
  /////////////////////////////////

  _checkInitialised_(CN_OPSKETCH);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch::~OpSketch(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  delete m_pExprStack;

  delete m_pStructSynthA;
  delete m_pStructSynthB;
  delete m_pCodeSynthA;
  delete m_pCodeSynthB;
  delete m_pCodeSynthC;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void OpSketch::vMakeColumnNm(StreamDef* pStreamDef, Stz* sColumn, eSR eStencilRow, i8 i_Column)
{
  _static_

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  // convert column name to uppercase
  s256 sColNm;
  sColNm = pColumn->szColNm();
  StrLib::vToUpper((char*)sColNm.azChar());

  // form the column name
  sColumn->vSprint("p%s->%s.m_iAsI8"_Stz, xRowRef_v2__[eStencilRow], sColNm.azChar());
}

/*----------------------------------------------------------------------------*/
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void OpSketch::vMakeColumnNm(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
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
void OpSketch::vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get OpSketch kind                                                          */
/*----------------------------------------------------------------------------*/
eLS OpSketch::eShape(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_eStencilSlot;
}

/*----------------------------------------------------------------------------*/
/* get structA synthesiser                                                    */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pStructSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pStructSynthA;
}

/*----------------------------------------------------------------------------*/
/* set structA synthesiser                                                    */
/*----------------------------------------------------------------------------*/
void OpSketch::vStructSynthA(CodeSynth_v2* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pStructSynthA = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get structB synthesiser                                                    */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pStructSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pStructSynthB;
}

/*----------------------------------------------------------------------------*/
/* set structB synthesiser                                                    */
/*----------------------------------------------------------------------------*/
void OpSketch::vStructSynthB(CodeSynth_v2* pStructSynth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pStructSynthB = pStructSynth;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pCodeSynthA(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser A                                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vCodeSynthA(CodeSynth_v2* pCodeSynthA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pCodeSynthA = pCodeSynthA;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pCodeSynthB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser B                                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vCodeSynthB(CodeSynth_v2* pCodeSynthB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pCodeSynthB = pCodeSynthB;
}

/*----------------------------------------------------------------------------*/
/* get code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
CodeSynth_v2* OpSketch::pCodeSynthC(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  return m_pCodeSynthC;
}

/*----------------------------------------------------------------------------*/
/* set code synthesiser C                                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vCodeSynthC(CodeSynth_v2* pCodeSynthC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  m_pCodeSynthC = pCodeSynthC;
}

/*----------------------------------------------------------------------------*/
/* generate the expression stacks                                             */
/*----------------------------------------------------------------------------*/
void OpSketch::vGenerateExprStacks(IntStack* pPreExpStack)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPSKETCH,_FOOTER_);

  bool bLeftOuter  = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for each projected column expression
  vGenerateExpr(m_sExprString.azChar(), nullptr, EK_PROJECT, m_pStreamDefLft,
                m_sLabelNext, &bLeftOuter);
  
}

/*----------------------------------------------------------------------------*/
/* convert postfix to expr macros                                             */
/*----------------------------------------------------------------------------*/
void OpSketch::vGenerateExpr(char* sExpr, TypedStack* pPostStack, eEK eExprKind,
                                 StreamDef* pStreamDef, Stz* sLabelNext, bool *pbLeftOuter) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  eDM eExprType = DM_UNK;

  bool bIndexJoin = false;
  bool bAssignJoin = false;
  bool bLowerBound = false;
  bool bUpperBound = false;

  TypedStack *pExprStack;
  IntStack *pExprType;

  Token *pTkn;

  i8 i_Col = 0;
  i8 i_ColPL = 0;
  i8 i_ExprStack = 0;
  i8 i_DupCtr = 1;
  i8 iGbyCols;
  i8 iGkyCols = 0;
  eDM eCastType;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenize the expression
  m_pTokenVec = AM::TOKENISE::pTokeniseSen((Stz *) sExpr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the ExprType stack
  pExprType = new IntStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the predicate expression
  ePE ePreExp = m_pPreExpStack ? (ePE)m_pPreExpStack->iPeek(i_ExprStack) : PE_NONE;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set working ExprStack
  pExprStack = pWorkingStack(EK_NONE, ePreExp, pPostStack);

  if (!pExprStack)
    pExprStack = m_pExprStack;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate tokens of the expression
  for (i8 i_Tkn = 0; i_Tkn < m_pTokenVec->nItems(); i_Tkn++) {

    pTkn = m_pTokenVec->pGet(i_Tkn);


#ifdef WITH_DEBUG_VERSION
    //    vTrace("TKN: %s\n"_Stz, pTkn->azChars());
//
//    // dump the TypeStack
//    vTrace("--------------------------------------\n"_Stz);
//    vTrace("EXPRTYPE:\n"_Stz);
//    i8 nExpr = pExprType->nItems() - 1;
//
//    while ( nExpr >= 0) {
//      eDM eType = (eDM)pExprType->iPeek(nExpr--);
//      vTrace("%s\n"_Stz, aDM[eType]);
//    }
//    vTrace("--------------------------------------\n"_Stz);
#endif

    if (pTkn->bSame(","_Stz)) {
      // skip the comma

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to normal expstack
      if (ePreExp != PE_GBY)
        pExprStack = m_pExprStack;

      if (ePreExp == PE_GBY) {
        // push assign column
        vPushAssignCol(pExprStack, pPostStack, eExprKind, ePreExp, iGbyCols, i_ColPL, i_DupCtr);
        iGbyCols++;
      }
      else
        vPushAssignCol(pExprStack, pPostStack, eExprKind, ePreExp, i_Col+iGkyCols, i_ColPL, i_DupCtr);

      if (ePreExp == PE_GKY)
        iGkyCols++;

      if (ePreExp == PE_GBY && eExprKind == EK_ORDGROUPBY)
        if (iGbyCols >= 2)
          m_pOrdAggStack->vPushStz("_AND_BOLBOL(aCombinedT->m_iErrorCode)"_Stz);


      if (ePreExp != PE_GKY)
        i_Col++;

      if (ePreExp != PE_GBY && ePreExp != PE_GKY)
        i_ColPL++;

      i_ExprStack++;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to the next predicate expression
      ePreExp = (ePE) m_pPreExpStack->iPeek(i_ExprStack);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack
      pExprStack = pWorkingStack(eExprKind, ePreExp, pPostStack);

      continue;
    }

    if (pTkn->bSame("groupby"_Stz)) {

      eExprKind = EK_GROUPBY;
      iGbyCols = 0;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack
      pExprStack = pWorkingStack(eExprKind, ePreExp, pPostStack);
      RASSERT(pExprStack == m_pGbyStack);

      continue;
    }

    if (pTkn->bSame("ordGroupby"_Stz)) {

      eExprKind = EK_ORDGROUPBY;
      iGbyCols = 0;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack
      pExprStack = pWorkingStack(eExprKind, ePreExp, pPostStack);
      RASSERT(pExprStack == m_pGbyStack);

      continue;
    }

    if (pTkn->bSame("aggregates"_Stz)) {

      // push assign column
      vPushAssignCol(pExprStack, pPostStack, eExprKind, ePreExp, iGbyCols, i_ColPL, i_DupCtr);
      iGbyCols++;

      if (eExprKind == EK_GROUPBY)
        eExprKind = EK_AGGGBY;
      else {
        RASSERT(eExprKind == EK_ORDGROUPBY);
        RASSERT(ePreExp == PE_GBY);

        if (iGbyCols == 2 || (iGbyCols > 2 && iGbyCols % 2))
          m_pOrdAggStack->vPushStz("_AND_BOLBOL(aCombinedT->m_iErrorCode)"_Stz);

        eExprKind = EK_AGGORD;
      }

      i_Col = 0;
      i_ExprStack++;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to the next predicate expression
      ePreExp = (ePE)m_pPreExpStack->iPeek(i_ExprStack);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack
      pExprStack = pWorkingStack(eExprKind, ePreExp, pPostStack);

      continue;
    }

    if (pTkn->bSame("sort"_Stz)) {

      eExprKind = EK_SORT;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working stack
      pExprStack = pWorkingStack(eExprKind, ePreExp, pPostStack);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the top expression
    if (pTkn->bSame("top"_Stz)) {

      eExprKind = EK_TOP;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working stack
      pExprStack = pWorkingStack(eExprKind, ePreExp, pPostStack);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the filter expression
    if (pTkn->bSame("filter"_Stz)) {

      eExprKind = EK_FILTER;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working stack
      pExprStack = pWorkingStack(eExprKind, ePreExp, m_pExprStack);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // left_outer
    if (pTkn->bSame("left_outer"_Stz)) {
      *pbLeftOuter = true;
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
        vPushAssignCol(pExprStack, pPostStack, eExprKind, ePreExp, i_Col, i_ColPL, i_DupCtr);

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
        vPushAssignCol(pExprStack, pPostStack, eExprKind, ePreExp, i_Col, i_ColPL, i_DupCtr);

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
          vPushOrdCol(pExprStack, pExprType, ePreExp, eExprKind, pTkn);
          continue;
        }
        else {
          char *sNext = (char*)(pTkn->szChars() + 2);
          if (StrLib::bIsDigit(sNext[0])) {
            vPushOrdCol(pExprStack, pExprType, ePreExp, eExprKind, pTkn);
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
      pExprType->vPush(eExprType);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get asc / desc
    if (pTkn->bSame("asc"_Stz) || pTkn->bSame("desc"_Stz)) {
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the case expression
    if (pTkn->bSame("case"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_CSE);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the in expression
    if (pTkn->bSame("inList"_Stz) || pTkn->bSame("in"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_INX);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the between expression
    if (pTkn->bSame("between"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_BET);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_day expression
    if (pTkn->bSame("extract_day"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_XDY);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_hour expression
    if (pTkn->bSame("extract_hour"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_XHR);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_minute expression
    if (pTkn->bSame("extract_minute"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_XMN);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_month expression
    if (pTkn->bSame("extract_month"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_XMT);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the extract_second expression
    if (pTkn->bSame("extract_second"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_XSC);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the between expression
    if (pTkn->bSame("extract_year"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_XYR);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the like expression
    if (pTkn->bSame("like2"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_LK2);
      continue;
    }

    if (pTkn->bSame("like3"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_LK3);
      continue;
    }

    if (pTkn->bSame("matchLeading"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_MLE);
      continue;
    }

    if (pTkn->bSame("matchMiddle"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_MMD);
      continue;
    }

    if (pTkn->bSame("matchMiddle1"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_MM1);
      continue;
    }

    if (pTkn->bSame("matchMiddle2"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_MM2);
      continue;
    }

    if (pTkn->bSame("matchTrailing"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_MTR);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the substring2 expression
    if (pTkn->bSame("substring2"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_SS2);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the substring3 expression
    if (pTkn->bSame("substring3"_Stz)) {
      vPushFunc(pExprStack, pExprType, PE_SS3);
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
      vPushCast(pExprStack, pExprType, eCastType);
      continue;
    }

    if (pTkn->bSame(")"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack to be the normal expression stack
      pExprStack = m_pExprStack;
      continue;
    }

    if ( pTkn->bSame("count"_Stz)    ||
         pTkn->bSame("countAll"_Stz) ||
         pTkn->bSame("countDistinct"_Stz) ||
         pTkn->bSame("min"_Stz)      ||
         pTkn->bSame("max"_Stz)      ||
         pTkn->bSame("sum"_Stz)
        ) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // aggregate follows, generate init expressions in the m_aInitStack
      eExprType = (eDM)pExprType->iPop();
      vPushAggCol(pExprStack, pPostStack, ePreExp, eExprKind, eExprType, i_Col+iGkyCols, i_ColPL, i_DupCtr);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // set working ExprStack
      RASSERT(pPostStack);
      pExprStack = pWorkingStack(EK_NONE, ePreExp, pPostStack);
      RASSERT(pExprStack == pPostStack);

      continue;
    }

    if (pTkn->bIsBoolean()) {
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_BOL);
      continue;
    }

    if (pTkn->bIsFloat()) {
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_FLT);
      continue;
    }

    if (pTkn->bIsInteger()) {
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_INT);
      continue;
    }

    if (pTkn->bIsDecimal()) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push token on the exprStack and type on the ExprType
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_BCD);
      continue;
    }

    if (pTkn->bSame("DATE"_Stz) ) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_DTE);
      continue;
    }

    if (pTkn->bSame("TIME"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_TME);
      continue;
    }

    if (pTkn->bSame("TIMESTAMP"_Stz)) {
      i_Tkn++;
      pTkn = m_pTokenVec->pGet(i_Tkn);
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_TSP);
      continue;
    }

    if (pTkn->bSame("null"_Stz) ) {
      pTkn = m_pTokenVec->pGet(i_Tkn);
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_UNK);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // negate operator
    if (pTkn->bSame("negate"_Stz)) {

      RASSERT( ! pTkn->bIsString() );
      RASSERT( ! bLowerBound );
      RASSERT( ! bUpperBound );

      vPushArithmetic(pExprStack, pExprType, pTkn);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // else
    if (pTkn->bSame("else"_Stz)) {
      eDM eColType1 = (eDM) pExprType->iPop();
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // endif
    if (pTkn->bSame("endif"_Stz)) {
      pExprStack->vPushStz("}"_Stz);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // then
    if (pTkn->bSame("then"_Stz)) {
      eDM eColType1 = (eDM) pExprType->iPop();
      pExprStack->vPushStz("}"_Stz);
      pExprStack->vPushStz("else {"_Stz);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // when
    if (pTkn->bSame("when"_Stz)) {
      pExprStack->vPushStz("_ASSIGN(bFilterResult)"_Stz);
      eDM eColType1 = (eDM) pExprType->iPop();
      RASSERT(eColType1 == DM_BOL);
      pExprStack->vPushStz("if (bFilterResult && bFilterResult != C_DATUM_NULL) {"_Stz);
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
    if (pTkn->bIsLogical() || pTkn->bSame("isNull"_Stz)) {

      RASSERT( ! pTkn->bIsString() );
      RASSERT( ! bLowerBound );
      RASSERT( ! bUpperBound );

      vPushLogical(pExprStack, pExprType, pTkn);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // !f operator (exit early if true (OR))
    if (pTkn->bSame("!t"_Stz)) {
      s4K sOperation;
      sOperation.vSprintCat("_FILTER_EXIT_ON_TRUE(%s)"_Stz, sLabelNext);
      pExprStack->vPushStz(sOperation);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // !f operator (exit early if false (AND))
    if (pTkn->bSame("!f"_Stz)) {
      s4K sOperation;
      sOperation.vSprintCat("_FILTER_EXIT_ON_FALSE(%s)"_Stz, sLabelNext);
      pExprStack->vPushStz(sOperation);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // relational operator ?
    if (pTkn->bIsRelational()) {

      // skip == op if indexJoin expr
      if (bIndexJoin) {
        if (pTkn->bSame("=="_Stz))
          bAssignJoin = true;
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

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // push literal
      vPushLiteral(pExprStack, pExprType, pTkn, ePreExp, eExprKind, DM_VCH);
    }
    else {
      vPushColumn(pExprStack, pExprType, pStreamDef,  pTkn);
    }

  }

  if (pExprStack == pPostStack)
    pExprStack = m_pExprStack;

  if ( !bIndexJoin || (bIndexJoin && bAssignJoin))
    vPushAssignCol(pExprStack, pPostStack, eExprKind, ePreExp, i_Col+iGkyCols, i_ColPL, i_DupCtr);

/*
#ifdef WITH_DEBUG_VERSION

  TypedCell* pTypedCell;

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

  // dump the m_pGbyStack stack
  vTrace("--------------------------------------\n"_Stz);
  vTrace("GROUPBYSTACK:\n"_Stz);
  nExpr = m_pGbyStack ? m_pGbyStack->nValues() - 1 : -1;

  while ( nExpr >= 0) {
    pTypedCell = m_pGbyStack->aPeekItem(nExpr--);
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
*/

  delete pExprType;
}

/*----------------------------------------------------------------------------*/
/* push aggregate column on the exprStack                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vSetLabels(Stz* sLabelOp, Stz* sLabelNext, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);
  
  m_sLabelOp   = sLabelOp;
  m_sLabelNext = sLabelNext;
  m_sLabelLoop = sLabelLoop;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get key value                                                              */
/*----------------------------------------------------------------------------*/
void OpSketch::vGetValue(eDM eDatum, char* sValue, i8& iValue, d8& dValue) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH_V2, _FOOTER_);

  switch (eDatum) {
    case DM_BCD: {
      SN_Bcd snBcd;
      iValue = snBcd.i(&sValue, AM::pFmt());
    }
      break;
    case DM_BOL: {
      SN_Bol snBol;
      iValue = snBol.i(&sValue, AM::pFmt());
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
/* add the init aggregate                                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vInitAgg(ePE ePreExp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CODESYNTH_V2, _FOOTER_);

  s256 sOperation;
  s32  sInitValue;

  switch(ePreExp) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PE_ALL:
      sOperation.vSprintCat("_PSH_INT(0)"_Stz);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PE_CNT:
      sOperation.vSprintCat("_PSH_INT(0)"_Stz);
      break;

    case PE_MIN:
    case PE_MAX:
      sOperation.vSprintCat("_PSH_NULL"_Stz);
      break;
      /*

          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        case PE_MIN:
          switch (eExprType) {
            case DM_BCD:
              iInitValue = Bcd::iMaxValue();
              sOperation.vSprintCat("_PSH_BCD(%lld)"_Stz, iInitValue);
              break;
            case DM_BOL:
              iInitValue = Bol::iMaxValue();
              sOperation.vSprintCat("_PSH_BOL(%lld)"_Stz, iInitValue);
              break;
            case DM_DTE:
              iInitValue = Dte::iMaxValue();
              sOperation.vSprintCat("_PSH_DTE(%lld)"_Stz, iInitValue);
              break;
            case DM_FLT:
              iInitValue = Flt::iMaxValue();
              sOperation.vSprintCat("_PSH_FLT(%lld)"_Stz, iInitValue);
              break;
            case DM_INT:
              iInitValue = Int::iMaxValue();
              sOperation.vSprintCat("_PSH_INT(%lld)"_Stz, iInitValue);
              break;
            case DM_IVL:
              iInitValue = Ivl::iMaxValue();
              sOperation.vSprintCat("_PSH_IVL(%lld)"_Stz, iInitValue);
              break;
            case DM_IVY:
              iInitValue = Ivl::iMaxValue();
              sOperation.vSprintCat("_PSH_IVY(%lld)"_Stz, iInitValue);
              break;
            case DM_TME:
              iInitValue = Tme::iMaxValue();
              sOperation.vSprintCat("_PSH_TME(%lld)"_Stz, iInitValue);
              break;
            case DM_TSP:
              iInitValue = Tsp::iMaxValue();
              sOperation.vSprintCat("_PSH_TSP(%lld)"_Stz, iInitValue);
              break;
            case DM_VCH:
              Vch::vMaxValue(sInitValue.azChar());
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
    */
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PE_SUM:
      sOperation.vSprintCat("_PSH_NULL"_Stz);
      break;
      /*
      switch(eExprType) {
        case DM_BCD:
          xBcd.vSet(0,0);
          sOperation.vSprintCat("_PSH_BCD(%lld)"_Stz, xBcd.T.m_iAsI8);
          break;
        case DM_FLT:
          sOperation.vSprintCat("_PSH_FLT(0.0e+0)"_Stz);
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
*/

    default:
      break;
      //NEVER_REACH;
  }
  // push on the aggregate init stack
  m_pInitStack->vPushStz(sOperation);
}

/*----------------------------------------------------------------------------*/
/* push aggregate column on the exprStack                                     */
/*----------------------------------------------------------------------------*/
void OpSketch::vPushAggCol(TypedStack* pExprStack, TypedStack* pPostStack,
                               ePE ePreExp, eEK eExprKind,
                               eDM eExprType,i8 i_Col, i8 i_ColPL, i8& i_DupCtr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s256 sColumn;
  s256 sStructColumn;
  s256 sOperation;

  // make the column name
  vMakeColumnNmStruct(m_pStreamDefOut, sColumn, i_DupCtr, i_Col);

  sStructColumn.vSprint("p%s->%s"_Stz,
                        xRowRef_v2__[m_eStencilTo], sColumn.azChar());

  if (eExprKind == EK_AGGGBY) {
//    if (bHashRows)
//      sStructColumn.vSprint("p%s_pot->%s"_Stz,
//                            xRowRef_v2__[m_eStencilTo], sColumn.azChar());
//    else
//      sStructColumn.vSprint("p%s_tub->%s"_Stz,
//                            xRowRef_v2__[m_eStencilTo], sColumn.azChar());

    vInitAgg(ePreExp);
    sOperation.vSprint("_ASSIGN(aCombinedT->m_aiPayload[%lld])"_Stz, i_ColPL);
    m_pInitStack->vPushStr(sOperation);
  } else {
    vInitAgg(ePreExp);

    RASSERT(eExprKind == EK_AGGREG ||
            eExprKind == EK_AGGORD)
    sOperation.vSprint("_ASSIGN(%s)"_Stz, sStructColumn.azChar());
    m_pInitStack->vPushStr(sOperation);
  }

  switch (ePreExp) {
    case PE_ALL:
      if (eExprKind == EK_AGGGBY) {
        // push col on both stacks
        sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_ColPL);
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

      sOperation.vSprint("_INC_INT(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStr(sOperation);
      break;

    case PE_CNT:
      if (eExprKind == EK_AGGGBY) {
        // push col on both stacks
        sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_ColPL);
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
      if (eExprKind == EK_AGGGBY) {

        // duplicate column on stack
        sOperation = aFM[eMacro_Dup[eExprType]];
        sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
        pExprStack->vPushStz(sOperation);

        // push col on both stacks
        if (eExprType == DM_VCH) {
          sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_ColPL);
          pExprStack->vPushStr(sOperation);
          pPostStack->vPushStr(sOperation);
          sOperation = "_LTA_VCHVCH"_Stz;
          sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
          pExprStack->vPushStr(sOperation);
        }
        else {
          sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_ColPL);
          pExprStack->vPushStr(sOperation);
          pPostStack->vPushStr(sOperation);
          sOperation = aFM[eMacro_Lta[eExprType][eExprType]];
          sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
          pExprStack->vPushStr(sOperation);
        }
      }
      else {
        RASSERT(eExprKind == EK_AGGREG ||
                eExprKind == EK_AGGORD);
        // duplicate column on stack
        sOperation = aFM[eMacro_Dup[eExprType]];
        sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
        pExprStack->vPushStz(sOperation);

        // push col on both stacks
        sOperation.vSprint("_PSH_COL(%s)"_Stz, sStructColumn.azChar());
        pExprStack->vPushStr(sOperation);
        pPostStack->vPushStr(sOperation);

        sOperation = aFM[eMacro_Lta[eExprType][eExprType]];
        sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
        pExprStack->vPushStr(sOperation);
      }

      break;

    case PE_MAX:
      if (eExprKind == EK_AGGGBY) {

        // duplicate column on stack
        sOperation = aFM[eMacro_Dup[eExprType]];
        sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
        pExprStack->vPushStz(sOperation);

        // push col on both stacks
        if (eExprType == DM_VCH) {
          sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_ColPL);
          pExprStack->vPushStr(sOperation);
          pPostStack->vPushStr(sOperation);
          sOperation = "_GTA_VCHVCH"_Stz;
          sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
          pExprStack->vPushStr(sOperation);
        }
        else {
          sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_ColPL);
          pExprStack->vPushStr(sOperation);
          pPostStack->vPushStr(sOperation);
          sOperation = aFM[eMacro_Gta[eExprType][eExprType]];
          sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
          pExprStack->vPushStr(sOperation);
        }
      }
      else {
        RASSERT(eExprKind == EK_AGGREG ||
                eExprKind == EK_AGGORD);

        // duplicate column on stack
        sOperation = aFM[eMacro_Dup[eExprType]];
        sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
        pExprStack->vPushStz(sOperation);

        // push col on both stacks
        sOperation.vSprint("_PSH_COL(%s)"_Stz, sStructColumn.azChar());
        pExprStack->vPushStr(sOperation);
        pPostStack->vPushStr(sOperation);

        sOperation = aFM[eMacro_Gta[eExprType][eExprType]];
        sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
        pExprStack->vPushStr(sOperation);
      }

      break;

    case PE_SUM:
      if (eExprKind == EK_AGGGBY) {
        // push col on both stacks
        sOperation.vSprint("_PSH_COL(aCombinedT->m_aiPayload[%lld])"_Stz, i_ColPL);
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

      sOperation = aFM[eMacro_Sum[eExprType][eExprType]];
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
void OpSketch::vPushArithmetic(TypedStack* pExprStack, IntStack* pExprType,
                                   Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s4K sOperation;
  eDM eResult;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get first operand type
  eDM eColType1 = (eDM) pExprType->iPop();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the negate operator
  if (pTkn->bSame("negate"_Stz)) {
    sOperation = aFM[eMacro_Neg[eColType1]];
    eResult = eMacro_NegResult[eColType1];
  }
  else {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get second operand type
    eDM eColType2 = (eDM) pExprType->iPop();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the arithmetic operator
    if (pTkn->bSame("+"_Stz)) {
      sOperation = aFM[eMacro_Add[eColType1][eColType2]];
      eResult = eMacro_AddResult[eColType1][eColType2];
    } else if (pTkn->bSame("/"_Stz)) {
      sOperation = aFM[eMacro_Div[eColType1][eColType2]];
      eResult = eMacro_DivResult[eColType1][eColType2];
    } else if (pTkn->bSame("%"_Stz)) {
      sOperation = aFM[eMacro_Mod[eColType1][eColType2]];
      eResult = eMacro_ModResult[eColType1][eColType2];
    } else if (pTkn->bSame("*"_Stz)) {
      sOperation = aFM[eMacro_Mul[eColType1][eColType2]];
      eResult = eMacro_MulResult[eColType1][eColType2];
    } else if (pTkn->bSame("-"_Stz)) {
      sOperation = aFM[eMacro_Sub[eColType1][eColType2]];
      eResult = eMacro_SubResult[eColType1][eColType2];
    } else
      RASSERT(0);
  }

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
void OpSketch::vPushCast(TypedStack* pExprStack, IntStack* pExprType, eDM eCastType) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s4K sOperation;
  eDM eResult;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get column type
  eDM eColType1 = (eDM) pExprType->iPop();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the cast operator if different types

  if (eColType1 != eCastType) {
    sOperation = aFM[eMacro_Cast[eColType1][eCastType]];
    eResult = eMacro_CastResult[eColType1][eCastType];

    if (eResult == DM_VCH) {
      sOperation.vConcat("(aCombinedT->m_iErrorCode, szStrArea)"_Stz);
    } else {
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // push new expr string on the ExprStack
    pExprStack->vPushStr(sOperation);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // push result type string on the ExprType stack
    pExprType->vPush((i8) eResult);
  }
  else
    pExprType->vPush((i8) eColType1);

}

/*----------------------------------------------------------------------------*/
/* push column                                                                */
/*----------------------------------------------------------------------------*/
void OpSketch::vPushColumn(TypedStack* pExprStack, IntStack* pTypeStack, StreamDef* pStreamDef, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s256 sOperation;
  s256 sColumn;

  Column *pColumn = pStreamDef->pFindColumn(pTkn->szChars());

  vMakeColumnNm(pStreamDef, sColumn, m_eStencilFrom, pColumn->iColumnNo());

  sOperation.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push token on the exprStack and type on the typeStack
  pTypeStack->vPush(Column::eDatum(pColumn->eColumnType()));
  pExprStack->vPushStz(sOperation);

  if (pExprStack == m_pGbyStack) {
    m_pOrdAggStack->vPushStz(sOperation);
  }

  // if column nullable generate null check expressions
  if (pColumn->bIsNullable()) {
    sOperation.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());
    m_pNullStack->vPushStz(sOperation);
    sOperation.vSprint("_ISNULL(aCombinedT->m_iErrorCode)"_Stz);
    m_pNullStack->vPushStz(sOperation);
    m_pNullStack->vPushStz("_ASSIGN(bNullResult)\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* push count                                                                 */
/*----------------------------------------------------------------------------*/
void OpSketch::vPushCount(TypedStack* pExprStack, IntStack* pTypeStack, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

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
void OpSketch::vPushLiteral(TypedStack* pExprStack, IntStack* pTypeStack, Token* pTkn,
                                ePE ePreExp, eEK eExprKind, eDM eLiteral) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s48K sOperation;
  i8 iValue;
  d8 dValue;

  sOperation = aFM[eMacro_Push[eLiteral]];

  switch(eLiteral) {
    case DM_BCD:
      vGetValue(DM_BCD, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      pExprStack->vPushStz(sOperation);
      break;
    case DM_BOL:
      vGetValue(DM_BOL, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      pExprStack->vPushStz(sOperation);
      break;
    case DM_DTE:
      vGetValue(DM_DTE, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      pExprStack->vPushStz(sOperation);
      break;
    case DM_FLT:
      vGetValue(DM_FLT, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%.9e)"_Stz, dValue);
      pExprStack->vPushStz(sOperation);
      break;
    case DM_INT:
      sOperation.vSprintCat("(%s)"_Stz, pTkn->szChars());
      pExprStack->vPushStz(sOperation);
      break;
    case DM_TME:
      vGetValue(DM_TME, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      pExprStack->vPushStz(sOperation);
      break;
    case DM_TSP:
      vGetValue(DM_TSP, pTkn->azChars(), iValue, dValue);
      sOperation.vSprintCat("(%lld)"_Stz, iValue);
      pExprStack->vPushStz(sOperation);
      break;
    case DM_UNK:
      sOperation.vSprintCat("(%lld)"_Stz, C_DATUM_NULL);
      pExprStack->vPushStz(sOperation);
      break;
    case DM_VCH:
      sOperation.vSprintCat("(\"%s\")"_Stz, pTkn->szChars());
      pExprStack->vPushStz(sOperation);
      if (ePreExp == PE_EXP && eExprKind != EK_FILTER)
        pExprStack->vPushStz("_ALLOC_VCH(szStrArea)"_Stz);
      break;
    default:
      NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push type on the ExprType
  pTypeStack->vPush(eLiteral);
}

/*----------------------------------------------------------------------------*/
/* push logical expression                                                    */
/*----------------------------------------------------------------------------*/
void OpSketch::vPushLogical(TypedStack* pExprStack, IntStack* pExprType,
                                Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s4K sOperation;
  eDM eResult;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the logical operator
  eResult = DM_BOL;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get columnn types
  eDM eColType1 = (eDM) pExprType->iPop();

  if (pTkn->bSame("NOT"_Stz)) {
    sOperation = aFM[FM_NOT_BOLBOL];

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // expect only booleans or integers
    RASSERT(eColType1 == DM_BOL || eColType1 == DM_INT);
  }
  else if (pTkn->bSame("isNULL"_Stz)) {
    sOperation = "_ISNULL"_Stz;
  }
  else {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get columnn types
    eDM eColType2 = (eDM) pExprType->iPop();

    if (pTkn->bSame("AND"_Stz)) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // expect only booleans or integers
      RASSERT(eColType2 == DM_BOL || eColType2 == DM_INT);
      sOperation = aFM[FM_AND_BOLBOL];
    }
    else
    if (pTkn->bSame("OR"_Stz)) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // expect only booleans or integers
      RASSERT(eColType2 == DM_BOL || eColType2 == DM_INT);
      sOperation = aFM[FM_OR_BOLBOL];
    }
    else
    if (pTkn->bSame("XOR"_Stz)) {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // expect only booleans or integers
      RASSERT(eColType2 == DM_BOL || eColType2 == DM_INT);
      sOperation = aFM[FM_XOR_BOLBOL];

    }
    else
      RASSERT(0);
  }

  sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);

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
void OpSketch::vPushOrdCol(TypedStack* pExprStack, IntStack* pTypeStack, ePE ePreExp, eEK eExprKind, Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s256 sColumn;
  s256 sColumnPL;
  Stz* sOrdinalColumn;
  Column* pColumn;
  s256 sPush;

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
//  if ( m_eStencilFrom == SR_PL) {
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // is column in the key ?
//    Column* pIndexColumn = m_pIndexDef->pFindColumn(pColumn->iColumnNo());
//
//    if (pIndexColumn) {
//      RASSERT(m_pIndexDef->eKeyKind() == KE_INT);
//      // get the index col no maching the streamDef column no
//      i8 iIndexCol = m_pIndexDef->iGetColumn(pColumn->iColumnNo());
//      sPush.vSprint("_PSH_COL(((struct Ent_int*)aCombinedT->m_aEntIntPL)->m_iIntElm0 + (%lld*8))"_Stz, iIndexCol);
//      pExprStack->vPushStz(sPush);
//    }
//    else {
//      sPush.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());
//      pExprStack->vPushStz(sPush);
  //  }
//  }
//  else {
  sPush.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());
  pExprStack->vPushStz(sPush);

  if (pExprStack == m_pGbyStack) {
    m_pOrdAggStack->vPushStz(sPush);
  }
//  }


  if (pColumn->eColumnType() == CT_VARCHAR && ePreExp == PE_EXP &&
      eExprKind != EK_FILTER && eExprKind != EK_SORT)
    pExprStack->vPushStz("_ALLOC_VCH(szStrArea)"_Stz);

  // push the type
  pTypeStack->vPush((i8)Column::eDatum(pColumn->eColumnType()));

  // if column nullable generate null check expressions
  if (pColumn->bIsNullable()) {
    sPush.vSprint("_PSH_COL(%s)"_Stz, sColumn.azChar());
    m_pNullStack->vPushStz(sPush);
    sPush.vSprint("_ISNULL(aCombinedT->m_iErrorCode)"_Stz);
    m_pNullStack->vPushStz(sPush);
    m_pNullStack->vPushStz("_ASSIGN(bNullResult)\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* push relational expression                                                 */
/*----------------------------------------------------------------------------*/
void OpSketch::vPushRelational(TypedStack* pExprStack, IntStack* pExprType,
                                   Token* pTkn) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s4K sOperation;

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
void OpSketch::vPushAssignCol(TypedStack* pExprStack, TypedStack* pPostStack, eEK eExprKind, ePE ePreExp,
                                  i8 i_Col, i8 i_ColPL, i8& i_DupCtr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  s256 sColNm;
  s256 sPush;
  eDM eDatum;
  Column* pColumn;

  switch(eExprKind) {

    case EK_AGGREG:
      // make the column name
      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);
      pColumn = m_pStreamDefOut->pGetColumn(i_Col);

      if (ePreExp == PE_MIN || ePreExp == PE_MAX) {
        if (pColumn->eColumnType() == CT_VARCHAR)
          sPush.vSprint("_COPYSTR_IFTRUE(aCombinedT->m_racStrAreaAgg, p%s->%s)\n"_Stz,
                        xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());
        else
          sPush.vSprint("_ASSIGN_IFTRUE(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());
      }
      else
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());

      pExprStack->vPushStz(sPush);

      if (pPostStack->nValues() > 1) {
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());
        pPostStack->vPushStz(sPush);
      }
      else
        pPostStack->vReset();
      break;

    case EK_AGGORD:

      if (ePreExp == PE_GKY) {
        break;
      }


      // make the column name
      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);
      pColumn = m_pStreamDefOut->pGetColumn(i_Col);
      if (ePreExp == PE_MIN || ePreExp == PE_MAX) {
        if (pColumn->eColumnType() == CT_VARCHAR)
          sPush.vSprint("_COPYSTR_IFTRUE(aCombinedT->m_racStrAreaAgg, p%s->%s)\n"_Stz,
                        xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());
        else
          sPush.vSprint("_ASSIGN_IFTRUE(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());
      }
      else
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());


      if (ePreExp == PE_NONE) {
        m_pInitStack->vPushStz(sPush);
      }
      else {
        pExprStack->vPushStz(sPush);
      }

      if (pPostStack->nValues() > 1) {
        sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());
        pPostStack->vPushStz(sPush);
      }
      else
        pPostStack->vReset();
      break;

    case EK_AGGGBY:

      if (ePreExp == PE_GKY) {
        break;
      }

      // make the column name
      vMakeColumnNm(m_pStreamDefOut, sColNm, m_eStencilTo, i_Col);
      pColumn = m_pStreamDefOut->pGetColumn(i_Col);
      if (ePreExp == PE_MIN || ePreExp == PE_MAX) {
        if (pColumn->eColumnType() == CT_VARCHAR)
          sPush.vSprint("_COPYSTR_IFTRUE_PL(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_ColPL);
        else
          sPush.vSprint("_ASSIGN_IFTRUE(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_ColPL);
      }
      else {
        if (pColumn->eColumnType() == CT_VARCHAR)
          sPush.vSprint("_COPYSTR_PL(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_ColPL);
        else
          sPush.vSprint("_ASSIGN(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_ColPL);
      }

      pExprStack->vPushStz(sPush);

      if (pPostStack->nValues() > 1) {
        if (pColumn->eColumnType() == CT_VARCHAR)
          sPush.vSprint("_COPYSTR_PL(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_ColPL);
        else
          sPush.vSprint("_ASSIGN(aCombinedT->m_aiPayload[%lld])\n"_Stz, i_ColPL);
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
      sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());
      pExprStack->vPushStz(sPush);

      sPush.vSprint("_PSH_COL(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());
      m_pOrdAggStack->vPushStz(sPush);
      pColumn = m_pStreamDefOut->pGetColumn(i_Col);
      eDatum = Column::eDatum(pColumn->eColumnType());
      sPush = aFM[eMacro_Eq[eDatum][eDatum]];
      sPush.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      m_pOrdAggStack->vPushStz(sPush);

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

//      if (m_pIndexDef->nColumns() > 1) {
//        vTrace("Warning: Multicolumn indexes not supported\n"_Stz);
//        RASSERT(m_pIndexDef->nColumns() == 1);
//      }

      if (m_pIndexDef->eKeyKind()) {
        sPush.vSprint("_ASSIGN(*(i8*)aCombinedT->m_aEntKeyT)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      else {
        pColumn = m_pIndexDef->pGetColumn(i_Col);
        if (pColumn->eColumnType() == CT_VARCHAR ||
            pColumn->eColumnType() == CT_CHAR) {
          sPush.vSprint("_ASSIGN_TO_ENTSTZ(aCombinedT->m_aEntWrkT)\n"_Stz);
        }
        else {
          sPush.vSprint("_ASSIGN_TO_ENTINT(aCombinedT->m_aEntWrkT)\n"_Stz);
        }
        pExprStack->vPushStz(sPush);
      }
      break;

    case EK_PROJECT:

      // make the column name
      vMakeColumnNmStruct(m_pStreamDefOut, sColNm, i_DupCtr, i_Col);
      sPush.vSprint("_ASSIGN(p%s->%s)\n"_Stz, xRowRef_v2__[m_eStencilTo].m_szInclude, sColNm.azChar());

      pExprStack->vPushStz(sPush);
      break;

    case EK_HASH:

      // assign to iMixIn
      sPush.vSprint("_ASSIGN(iMixIn)\n"_Stz);
      pExprStack->vPushStz(sPush);
      break;

    case EK_SORT:
      //RASSERT(ePreExp == PE_SRT);

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

    case EK_TOP:
      if (m_pIndexDef->nColumns() > 1) {
        vTrace("Warning: Multicolumn indexes not supported\n"_Stz);
        RASSERT(m_pIndexDef->nColumns() == 1);
      }

      if (m_pIndexDef->eKeyKind()) {
        sPush.vSprint("_ASSIGN(*(i8*)aCombinedT->m_aEntKeyPL)\n"_Stz);
        pExprStack->vPushStz(sPush);
      }
      else {
        sPush.vSprint("_ASSIGN_TO_ENTSTZ(aCombinedT->m_aEntWrkPL)\n"_Stz);
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
void OpSketch::vPushFunc(TypedStack* pExprStack, IntStack* pTypeStack, ePE ePreExp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

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
      eExpr3 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_Bet[eExpr1][eExpr2]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // CASE EXPRESSIONS
    case PE_CSE:
      //eExpr1 = (eDM)pTypeStack->iPop();
      //eExpr2 = (eDM)pTypeStack->iPop();
      //sOperation = aFM[eMacro_Inx[eExpr1][eExpr2]];
      //sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      //pExprStack->vPushStz(sOperation);
      //pTypeStack->vPush(DM_BOL);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // IN OPERATOR
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
      eExpr3 = (eDM)pTypeStack->iPop();
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
      eExpr3 = (eDM)pTypeStack->iPop();
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
      eExpr3 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_MATCH_TRAILING(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_BOL);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // SUBSTRING2
    case PE_SS2:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_STR_SUBSTRING2((char*)szStrArea, aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_VCH);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // SUBSTRING3
    case PE_SS3:
      eExpr1 = (eDM)pTypeStack->iPop();
      eExpr2 = (eDM)pTypeStack->iPop();
      eExpr3 = (eDM)pTypeStack->iPop();
      sOperation.vConcat("_STR_SUBSTRING3(szStrArea, aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_VCH);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // EXTRACT DAY
    case PE_XDY:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractDay[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // EXTRACT HOUR
    case PE_XHR:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractHour[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // EXTRACT MINUTE
    case PE_XMN:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractMinute[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // EXTRACT MONTH
    case PE_XMT:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractMonth[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // EXTRACT SECOND
    case PE_XSC:
      eExpr1 = (eDM)pTypeStack->iPop();
      sOperation = aFM[eMacro_ExtractSecond[eExpr1]];
      sOperation.vConcat("(aCombinedT->m_iErrorCode)"_Stz);
      pExprStack->vPushStz(sOperation);
      pTypeStack->vPush(DM_INT);
      break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // EXTRACT YEAR
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
/* get working stack                                                          */
/*----------------------------------------------------------------------------*/
TypedStack* OpSketch::pWorkingStack(eEK eExprKind, ePE ePreExp, TypedStack* pPostStack)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPSKETCH, _FOOTER_);

  switch(eExprKind) {
    case EK_AGGGBY:
    case EK_AGGREG:
      if (ePreExp == PE_GKY ||
          ePreExp == PE_EXP)
        return m_pExprStack;

      //if (ePreExp == PE_GKY)
      //  return m_pExprStack;

      if (ePreExp == PE_ALL ||
          ePreExp == PE_CNT ||
          ePreExp == PE_MIN ||
          ePreExp == PE_MAX ||
          ePreExp == PE_SUM) {
        return pPostStack;
      }
      else
        return m_pInitStack;

    case EK_AGGORD:
      if (ePreExp == PE_ALL ||
          ePreExp == PE_CNT ||
          ePreExp == PE_MIN ||
          ePreExp == PE_MAX ||
          ePreExp == PE_SUM) {
        return pPostStack;
      }
      else
        return m_pExprStack;

    case EK_GROUPBY:
    case EK_ORDGROUPBY:
      return m_pGbyStack;

    case EK_FILTER:
    case EK_HASH:
    case EK_INDEXJOIN:
    case EK_INDEXSCAN:
    case EK_PROJECT:
    case EK_SORT:
    case EK_TOP:
      return m_pExprStack;

    case EK_NONE:
      if (ePreExp == PE_ALL ||
          ePreExp == PE_CNT ||
          ePreExp == PE_MIN ||
          ePreExp == PE_MAX ||
          ePreExp == PE_SUM) {
        return pPostStack;
      }
      else
        return m_pExprStack;

    default:
      NEVER_REACH;
  }

  return nullptr;
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
