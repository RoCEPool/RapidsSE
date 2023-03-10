/*============================================================================*/
/* OuterQuery.cpp -- OuterQuery                                                 */
/*============================================================================*/

#include "OuterQuery.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

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

      Board::vOuter_Query_Analysis_Of_from_call(pCI);
      pOuterQuery->vSubQueries(pOuterQuery->nSubQueries() + 1);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for where calls to sub-queries

    pCI->vRestart();
    while (pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
      Board::vOuter_Query_Analysis_Of_where_boolCalc(pCI);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // search for having calls to sub-queries

    pCI->vRestart();
    while (pCI->G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
      Board::vOuter_Query_Analysis_Of_having_boolCalc(pCI);
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
    pOuterQuery->vDump();
  }

  //pCI->vEndTransform();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
