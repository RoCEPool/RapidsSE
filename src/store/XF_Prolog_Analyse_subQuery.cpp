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
void XF_Prolog_Analyse_subQuery::vAnalyse(CardIter* pCI)
{
  _static_

  XF_Prolog_Analyse_subQuery* pXF_subQuery_Analysis = new XF_Prolog_Analyse_subQuery(pCI);

  pXF_subQuery_Analysis->vAnalyse_subQuery();

  delete pXF_subQuery_Analysis;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* subQuery analysis                                                          */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_subQuery::vAnalyse_subQuery(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_ANALYSE_SUBQUERY, "subQuery analysis"_Stz);
  
  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {

    SubQuery* pOuterQuery = m_pCI->G.pBoard->pSubQuery();
    pOuterQuery->vDisposition(QD_LINEAR);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    m_pCI->vBeginEach();
    while (m_pCI->G_bEachMain_NONE()) {
      if (m_pCI->G.p_MainCard->bIsCard(CK_o_AGGREG) ||
          m_pCI->G.p_MainCard->bIsCard(CK_o_GBYAGG)) {
        pOuterQuery->vDisposition(QD_AGGREGATION);
        break;
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for from calls to sub-queries

    m_pCI->vBeginEach();
    while (m_pCI->G_bEachMain(CK_o_CALL)) {

      vAnalysis_Of_from_call(m_pCI);
      pOuterQuery->vSubQueries(pOuterQuery->nSubQueries() + 1);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for where calls to sub-queries

    m_pCI->vBeginEach();
    while (m_pCI->G_bEachMainWithSub(CK_o_WHERE, CK_BOOLPRED)) {
      vAnalysis_Of_where_boolPred(m_pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for having calls to sub-queries

    m_pCI->vBeginEach();
    while (m_pCI->G_bEachMainWithSub(CK_o_HAVING, CK_BOOLPRED)) {
      vAnalysis_Of_having_boolPred(m_pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for parameters

    m_pCI->vClearRestart();
    i8 nParams = 0;
    while (m_pCI->G_bEachMain(CK_o_CORRELATION))
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


  m_pCI->vEndTransform(__LINE__);
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

  Stz* szSqyNo = m_pCI->G.p_call->szFacet(SF_NONE);
//  Board* pBoard = m_pCI->m_pBench->pFindBoard_By_sqyNm(szSqyNo);
//  if (!pBoard)
  Board* pBoard = m_pCI->m_pBench->pFindBoard_By_sqyNo_v2(szSqyNo);
  if (!pBoard) {
    m_pCI->m_pBench->vDump(__func__ );
    NEVER_REACH;
  }
  SubQuery *pSubQuery = pBoard->pSubQuery();
  pSubQuery->vCaller(m_pCI->G.i_Board);
  pSubQuery->vClause(QC_FROM_CLAUSE);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_subQuery::vAnalysis_Of_having_boolPred(CardIter* m_pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);

  Card_CONJ* p_conj_ROOT = static_cast<Card_CONJ*>(m_pCI->G.p_boolPred->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [m_pCI](Card_CONJ *p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_RLNCONJ))
      return C_CONTINUE_RECURSION;

    Stz* szValue_term = p_conj->szFacet(SF_NONE);
    eRO eRelnOp = RelOp::eDecode(szValue_term);
    if (RO_NONE == eRelnOp)
      return C_CONTINUE_RECURSION;

    Stz* szSqyNo = p_conj->szGetTerm_sqyNo(m_pCI);
    if (! szSqyNo || !m_pCI->G_bIsSqyNo(szSqyNo))
      return C_CONTINUE_RECURSION;

    p_conj->pSymbol()->Y_vSet(P_SqyCall);

    Board* pSubBoard = m_pCI->m_pBench->pFindBoard_By_sqyNo_v2(szSqyNo);
    SubQuery* pSubQuery = pSubBoard->pSubQuery();

    pSubQuery->vCaller(m_pCI->G.i_Board);
    pSubQuery->vClause(QC_HAVING_CLAUSE);
    pSubQuery->vCallOp(QO_RELN_OP);
    pSubQuery->vRelnOp(eRelnOp);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Analyse_subQuery::vAnalysis_Of_where_boolPred(CardIter* m_pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_ANALYSE_SUBQUERY, _FOOTER_);

  Card_CONJ* p_conj_ROOT = static_cast<Card_CONJ*>(m_pCI->G.p_boolPred->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [m_pCI](Card_CONJ *p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = p_conj->pGetTerm_sqyNo(m_pCI);
    if (! p_term)
      return C_CONTINUE_RECURSION;

    SubQuery *pSubQuery = m_pCI->m_pBench->pFindSubQuery(p_term->pSymbol());
    if (! pSubQuery)
      return C_CONTINUE_RECURSION;

    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --

    RASSERT(p_conj->bIsCard(CK_RLNCONJ));

    p_conj->pSymbol()->Y_vSet(P_SqyCall);

    Stz* szValue_conj = p_conj->szFacet(SF_NONE);

    if (p_conj->bIsRelnOp()) {

      eRO eRelnOp = RelOp::eDecode(szValue_conj);
      RASSERT(RO_NONE != eRelnOp);

      pSubQuery->vCaller(m_pCI->G.i_Board);
      pSubQuery->vClause(QC_WHERE_CLAUSE);
      pSubQuery->vCallOp(QO_RELN_OP);
      pSubQuery->vRelnOp(eRelnOp);
      return C_CONTINUE_RECURSION;
    }

    RASSERT(p_conj->bIsSqyOp());

    eQO eSubQryOp = p_conj->pSymbol()->eEvalSqyOp();
    RASSERT(QO_NONE != eSubQryOp);

    pSubQuery->vCaller(m_pCI->G.i_Board);
    pSubQuery->vClause(QC_WHERE_CLAUSE);
    pSubQuery->vCallOp(eSubQryOp);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
