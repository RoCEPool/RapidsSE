/*============================================================================*/
/* XF_Prolog_Analyse_subQuery.cpp -- Analyse SubQuery                         */
/*============================================================================*/

#include "XF_Prolog_Analyse_subQuery.hpp"

#include "RelOp.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Analyse_subQuery, CN_XF_PROLOG_ANALYSE_SUBQUERY)
__aNewFunction(XF_Prolog_Analyse_subQuery, CN_XF_PROLOG_ANALYSE_SUBQUERY)
__vDeleteFunction(XF_Prolog_Analyse_subQuery, CN_XF_PROLOG_ANALYSE_SUBQUERY)
__aFooterFunction(XF_Prolog_Analyse_subQuery)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_subQuery::XF_Prolog_Analyse_subQuery(CardIter* m_pCI)
: XF_PROLOG(CN_XF_PROLOG_ANALYSE_SUBQUERY, m_pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_ANALYSE_SUBQUERY);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Analyse_subQuery::~XF_Prolog_Analyse_subQuery(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_subQuery::vProlog(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_subQuery* pXF_subQuery_Analysis = new XF_Prolog_Analyse_subQuery(pCI);

  pXF_subQuery_Analysis->vXF_subQuery_Analysis();

  delete pXF_subQuery_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* subQuery analysis                                                          */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_subQuery::vXF_subQuery_Analysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_SUBQUERY_ANALYSIS, "subQuery analysis"_Stz);
  
  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {

    SubQuery* pOuterQuery = m_pCI->G.pBoard->pSubQuery();
    pOuterQuery->vDisposition(QD_LINEAR);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    m_pCI->vBeginEach();
    while (m_pCI->G_bEachMain_NONE()) {
      if (m_pCI->G.p_MainCard->bIsCard(CK_AGGREG) ||
          m_pCI->G.p_MainCard->bIsCard(CK_GBYAGG)) {
        pOuterQuery->vDisposition(QD_AGGREGATION);
        break;
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for from calls to sub-queries

    m_pCI->vBeginEach();
    while (m_pCI->G_bEachMain(CK_CALL)) {

      vAnalysis_Of_from_call(m_pCI);
      pOuterQuery->vSubQueries(pOuterQuery->nSubQueries() + 1);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for where calls to sub-queries

    m_pCI->vBeginEach();
    while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
      vAnalysis_Of_where_boolCalc(m_pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for having calls to sub-queries

    m_pCI->vBeginEach();
    while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
      vAnalysis_Of_having_boolCalc(m_pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for parameters

    m_pCI->vClearRestart();
    i8 nParams = 0;
    while (m_pCI->G_bEachMain(CK_CORRELATION))
      nParams++;
    pOuterQuery->vCorrCols(nParams);

  }

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {
    SubQuery* pSubQuery = m_pCI->G.pBoard->pSubQuery();

    if ((m_pCI->G.p_subQuery =
          _pCard_SubQuery(m_pCI->G.pBoard->pDeck()->pFindChild(CK_SUBQUERY)))) {
      m_pCI->G.p_subQuery->vPopulate_subQuery(m_pCI,pSubQuery);
    }

    UNUSED(pSubQuery);
    //pSubQuery->vDump();
  }


  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_subQuery::vAnalysis_Of_from_call(CardIter* m_pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);

  Stz* szBoardNm = m_pCI->G.p_call->szSymbol(SF_NONE);
  Board* pBoard = m_pCI->m_pBench->pGetBoard(szBoardNm);
  SubQuery *pSubQuery = pBoard->pSubQuery();
  pSubQuery->vCaller(m_pCI->G.i_Board);
  pSubQuery->vClause(QC_FROM_CLAUSE);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_subQuery::vAnalysis_Of_having_boolCalc(CardIter* m_pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);

  Card_CONJUNCTION* p_pred_ROOT = static_cast<Card_CONJUNCTION*>(m_pCI->G.p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [m_pCI](Card_CONJUNCTION *p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Stz* szValue_term = p_pred->szSymbol(SF_NONE);
    eRO eRelnOp = RelOp::eDecode(szValue_term);

    if (RO_NONE == eRelnOp)
      return C_CONTINUE_RECURSION;

    Stz* szSubBoardNm = p_pred->szGetBoardNmInTerm(m_pCI);
    if ( ! szSubBoardNm || !m_pCI->G_bIsBoardNm(szSubBoardNm))
      return C_CONTINUE_RECURSION;

    p_pred->pSymbol()->Y_vSet(P_SubQryCall);

    Board* pSubBoard = m_pCI->m_pBench->pGetBoard(szSubBoardNm);
    SubQuery* pSubQuery = pSubBoard->pSubQuery();

    pSubQuery->vCaller(m_pCI->G.i_Board);
    pSubQuery->vClause(QC_HAVING_CLAUSE);
    pSubQuery->vCallOp(QO_RELN_OP);
    pSubQuery->vRelnOp(eRelnOp);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_subQuery::vAnalysis_Of_where_boolCalc(CardIter* m_pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);

  Card_CONJUNCTION* p_pred_ROOT = static_cast<Card_CONJUNCTION*>(m_pCI->G.p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [m_pCI](Card_CONJUNCTION *p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Stz *szSubBoardNm = p_pred->szGetBoardNmInTerm(m_pCI);
    if (! szSubBoardNm || !m_pCI->G_bIsBoardNm(szSubBoardNm))
      return C_CONTINUE_RECURSION;

    // todo: m_pCI->pGetBoard(szSubBoardNm); m_pCI->pGetSubQuery(szSubBoardNm);
    Board *pSubBoard = m_pCI->m_pBench->pGetBoard(szSubBoardNm);
    SubQuery *pSubQuery = pSubBoard->pSubQuery();

    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --

    RASSERT(p_pred->bIsCard(CK_CMPCONJ));

    p_pred->pSymbol()->Y_vSet(P_SubQryCall);

    Stz* szValue_pred = p_pred->szSymbol(SF_NONE);

    if (p_pred->bIsRelnOp()) {

      eRO eRelnOp = RelOp::eDecode(szValue_pred);
      RASSERT(RO_NONE != eRelnOp);

      pSubQuery->vCaller(m_pCI->G.i_Board);
      pSubQuery->vClause(QC_WHERE_CLAUSE);
      pSubQuery->vCallOp(QO_RELN_OP);
      pSubQuery->vRelnOp(eRelnOp);
      return C_CONTINUE_RECURSION;
    }

    RASSERT(p_pred->bIsSubQueryOp());

    eQO eSubQryOp = p_pred->pSymbol()->eEvalSubQryOp();
    RASSERT(QO_NONE != eSubQryOp);

    pSubQuery->vCaller(m_pCI->G.i_Board);
    pSubQuery->vClause(QC_WHERE_CLAUSE);
    pSubQuery->vCallOp(eSubQryOp);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
  UNUSED(bIgnore);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
