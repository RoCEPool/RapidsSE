/*============================================================================*/
/* OuterQuery.cpp -- Outer Query                                              */
/*============================================================================*/

#include "OuterQuery.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"
#include "RelOp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, OuterQuery, CN_OUTERQUERY)
__aNewFunction(OuterQuery, CN_OUTERQUERY)
__vDeleteFunction(OuterQuery, CN_OUTERQUERY)
__aFooterFunction(OuterQuery)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OuterQuery::OuterQuery(void)
: Object(CN_OUTERQUERY)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OUTERQUERY, _FOOTER_);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_OUTERQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OuterQuery::~OuterQuery(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_OUTERQUERY, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Analyse all sub-queries                                                    */
/*----------------------------------------------------------------------------*/
void OuterQuery::vQueryAnalysis(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OUTERQUERY, _FOOTER_);

  //pCI->vTransform(__func__, "Analyse all sub-queries"_Stz);

  pCI->vBoardIteration();
  while (pCI->bEachBoard()) {

    SubQuery* pOuterQuery = pCI->G.pBoard->pSubQuery();
    pOuterQuery->vDisposition(QD_LINEAR);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    pCI->vRestart();
    while (pCI->G_bEachMain()) {
      if (pCI->G.p_MainCard->bIsCard(CK_AGGREG) ||
          pCI->G.p_MainCard->bIsCard(CK_GBYAGG)) {
        pOuterQuery->vDisposition(QD_AGGREGATION);
        break;
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for from calls to sub-queries

    pCI->vRestart();
    while (pCI->G_bEachMain(CK_CALL)) {

      vAnalysis_Of_from_call(pCI);
      pOuterQuery->vSubQueries(pOuterQuery->nSubQueries() + 1);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for where calls to sub-queries

    pCI->vRestart();
    while (pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
      vAnalysis_Of_where_boolCalc(pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for having calls to sub-queries

    pCI->vRestart();
    while (pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
      vAnalysis_Of_having_boolCalc(pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for parameters

    pCI->G_vRestart();
    i8 nParams = 0;
    while (pCI->G_bEachMain(CK_CORRELATION))
      nParams++;
    pOuterQuery->vCorrCols(nParams);

  }

  pCI->vBoardIteration();
  while (pCI->bEachBoard()) {
    SubQuery* pOuterQuery = pCI->G.pBoard->pSubQuery();
    //pOuterQuery->vDump();
  }

  //pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OuterQuery::vAnalysis_Of_from_call(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OUTERQUERY, _FOOTER_);

  Stz* szBoardNm = pCI->G.p_call->szSymbol();
  Board* pBoard = pCI->m_pBench->pGetBoard(szBoardNm);
  SubQuery *pSubQuery = pBoard->pSubQuery();
  pSubQuery->vCaller(pCI->G.i_Board);
  pSubQuery->vClause(QC_FROM_CLAUSE);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OuterQuery::vAnalysis_Of_having_boolCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OUTERQUERY, _FOOTER_);

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_PRED *p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Stz* szValue_term = p_pred->szSymbol();
    eRO eRelnOp = RelOp::eDecode(szValue_term);

    if (RO_NONE == eRelnOp)
      return C_CONTINUE_RECURSION;

    Stz* szSubBoardNm = p_pred->szGetBoardNmInTerm(pCI);
    if ( ! szSubBoardNm || !pCI->G_bIsBoardNm(szSubBoardNm))
      return C_CONTINUE_RECURSION;

    p_pred->pSymbol()->Y_vSet(P_SubQryCall);

    Board* pSubBoard = pCI->m_pBench->pGetBoard(szSubBoardNm);
    SubQuery* pSubQuery = pSubBoard->pSubQuery();

    pSubQuery->vCaller(pCI->G.i_Board);
    pSubQuery->vClause(QC_HAVING_CLAUSE);
    pSubQuery->vCallOp(QO_RELN_OP);
    pSubQuery->vRelnOp(eRelnOp);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OuterQuery::vAnalysis_Of_where_boolCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OUTERQUERY, _FOOTER_);

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_PRED *p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Stz *szSubBoardNm = p_pred->szGetBoardNmInTerm(pCI);
    if (! szSubBoardNm || !pCI->G_bIsBoardNm(szSubBoardNm))
      return C_CONTINUE_RECURSION;

    // todo: pCI->pGetBoard(szSubBoardNm); pCI->pGetSubQuery(szSubBoardNm);
    Board *pSubBoard = pCI->m_pBench->pGetBoard(szSubBoardNm);
    SubQuery *pSubQuery = pSubBoard->pSubQuery();

    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --

    RASSERT(p_pred->bIsCard(CK_CMPPRED));

    p_pred->pSymbol()->Y_vSet(P_SubQryCall);

    Stz* szValue_pred = p_pred->szSymbol();

    if (p_pred->bIsRelnOp()) {

      eRO eRelnOp = RelOp::eDecode(szValue_pred);
      RASSERT(RO_NONE != eRelnOp);

      pSubQuery->vCaller(pCI->G.i_Board);
      pSubQuery->vClause(QC_WHERE_CLAUSE);
      pSubQuery->vCallOp(QO_RELN_OP);
      pSubQuery->vRelnOp(eRelnOp);
      return C_CONTINUE_RECURSION;
    }

    RASSERT(p_pred->bIsSubQueryOp());

    eQO eSubQryOp = p_pred->pSymbol()->eEvalSubQryOp();
    RASSERT(QO_NONE != eSubQryOp);

    pSubQuery->vCaller(pCI->G.i_Board);
    pSubQuery->vClause(QC_WHERE_CLAUSE);
    pSubQuery->vCallOp(eSubQryOp);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
