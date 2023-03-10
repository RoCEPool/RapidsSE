/*============================================================================*/
/* CorrelatedJoin.cpp -- CorrelatedJoin                                                 */
/*============================================================================*/

#include "CorrelatedJoin.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, CorrelatedJoin, CN_CORRELATEDJOIN)
__aNewFunction(CorrelatedJoin, CN_CORRELATEDJOIN)
__vDeleteFunction(CorrelatedJoin, CN_CORRELATEDJOIN)
__aFooterFunction(CorrelatedJoin)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CorrelatedJoin::CorrelatedJoin(void)
: Object(CN_CORRELATEDJOIN)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CorrelatedJoin::~CorrelatedJoin(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vCorrelate(CardIter* pCI, eSM Correlate)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* convert call of sub-query to correlated join to left sub-stream            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vVia_gbyAgg_Promotion(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // Start with processing SQ (sub-query)

  //                              //
  // T T A A A P F T T W A A A    //
  //                              //

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // identify correlation column in some SQ:<table>/<filter>/<boolCalc>//<cmpPred>

  //   v                          //  ]filter
  // T T A A A P F T T W A A A    //  .]boolCalc
  //   f                          //  ..]cmpPred ==
  //   p                          //           $d.2  ((pCI->m_pTerm_corrCol))
  //                              //             %1

  pCI->G_vRestart();
  while (pCI->G_bEachMainWith2Sub(CK_TABLE,CK_PROJECTION,CK_FILTER))
   if (Board::bIdentify_Correlation_Key_Predicate(pCI))
     break;
  RASSERT(pCI->m_pTerm_corrCol); // eg $d.2 (L_PARTKEY)
  pCI->vPrintBoard(__LINE__);

  // find SQ:<op>/<projection>/<expr> which describes the correlation column
  Board::vIdentify_Expr_Of_Correlation_Key_Column(pCI);
  RASSERT(pCI->m_pExpr_corr);    // eg $15 (L_PARTKEY)
  pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // enrich SQ:<aggreg> to be SQ:<gbyAgg>/<key=CorrCol>,
  //   thereby producing ordered subQuery stream

  RASSERT(pCI->G_bEachMain(CK_AGGREG));
  Board::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Key(pCI);
  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // include correlation column SQ:[<project>/<projection>]

  if (pCI->G_bIsNextMain(CK_PROJECT)) {
    RASSERT(pCI->G_bEachMain(CK_PROJECT));
    Board::vEnhance_sq_project_To_Include_Correlation_Key(pCI, "__SQCRLKEY"_Stz);
  }
  else {

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get final SQ:<op>/<projection>

  if (CK_PROJECT == pCI->G.e_MainCard || CK_GBYAGG == pCI->G.e_MainCard)
    pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
  else
    NEVER_REACH

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  pCI->vRestart();
  RASSERT(pCI->G_bEachMain_reverse(CK_FOLD));

  // Now processing OQ (outer-query)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // re-project the leading OQ:<table>* sequence

  i8 nTables = 0;
  while (pCI->G_bIsNextMain(CK_TABLE) && pCI->G_bEachMain(CK_TABLE)) {
    nTables++;
    pCI->G.p_table->vSmooth(pCI);
    pCI->vPrintBoard(__LINE__);
  }
  pCI->vPrintBoard(__LINE__);

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // subQuery subQuery subQuery subQuery subQuery subQuery subQuery subQuery
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  // Convert the OQ to join to the gby correlation column ordered streamed from SQ

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // yank the correlation predicate from SQ

  pCI->G_vRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((pCI->m_pTerm_corrCol))
  //                               //             %1

  while (pCI->G_bEachMainWith2Sub(CK_TABLE,CK_FILTER,CK_BOOLCALC))
    if (pCI->G.p_boolCalc->bYank_PRED(MARK_AT, pCI->G.p_PRED))
      break;
  RASSERT(pCI->G.p_PRED->Y_bHas(P_CrlKeyPred));
  pCI->vPrintBoard(__LINE__);

  // tidy up empty SQ:<table>/<filter>
  pCI->G.p_table->vRemove_filter_IfEmpty(pCI->G.p_filter);
  pCI->vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((pCI->m_pTerm_corrCol))
  //                               //             %1

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  RASSERT(pCI->G_bEachMain(CK_FOLD));


  RASSERT(pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  pCI->G.p_boolCalc->vBump_Rightwards(2);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push correlated cmpPred into the OQ:<where>/<constraint>/<boolCalc>

  pCI->G.p_boolCalc->vPush_CrlKeyPred_As_CrlKeyTest(pCI->G.p_PRED);
  pCI->vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]roPred <
  // f                 bc          //           $a.5
  //                               //           $b.1
  //                               // __subquery0002

  Board::vConvert_CrlPrmOdn_Into_StmColOdn(pCI, pCI->G.p_boolCalc);
  Board::vConvert_SubQryCall_Into_StmColOdn(pCI, pCI->G.p_boolCalc);
  pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be done using standard predicate moving

  pCI->G_vRestart();

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((pCI->m_pTerm_corrCol))
  //             b <-  cp          //             %1

  RASSERT(pCI->G_bEachMain(CK_FOLD));
  RASSERT(pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  //Board::vMove_corrPred_To_mq_table_As_bounds(pCI, pCI->G.p_boolCalc);
  Board::vMove_corrPred_To_table_filter(pCI, pCI->G.p_boolCalc);
  pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->G_vRestart();
//  if (pCI->G_bEachMain(CK_FOLD)) {
//    if (pCI->G_bEachMain(CK_WHERE)) {
//      // TODO:
//      // R_vUTE_Unproject_foldExpr_After_where();
//      R_vUTE_Bump_where_BaseOn_foldExpr();
//    }
//  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
