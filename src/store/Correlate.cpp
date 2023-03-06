/*============================================================================*/
/* Correlate.cpp -- Correlate                                                 */
/*============================================================================*/

#include "Correlate.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Correlate, CN_CORRELATE)
__aNewFunction(Correlate, CN_CORRELATE)
__vDeleteFunction(Correlate, CN_CORRELATE)
__aFooterFunction(Correlate)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Correlate::Correlate(void)
: Object(CN_CORRELATE)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATE, _FOOTER_);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_CORRELATE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Correlate::~Correlate(void) noexcept(false)
{
_precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATE, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Correlate::vCorrelate(CardIter* pCI, eSM Correlate)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATE, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Correlate::vInto_GBYAGG(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATE, _FOOTER_);
  pCI->G_vRestart();
  while (pCI->G_bEachMainWith2Sub(CK_TABLE,CK_PROJECTION,CK_FILTER))
   if (Board::bIdentify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(pCI))
     break;

  RASSERT(pCI->m_pTerm_corrCol); // eg $d.2 (L_PARTKEY)
  Board::vFind_Correlation_Expr_Of_Correlation_Column(pCI);
  RASSERT(pCI->m_pExpr_corr);    // eg L_PARTKEY

  pCI->vPrintBoard(__LINE__);

  // T A A A P F T T T W A A A     //
  //   v v v                       //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <gbyElist> and <aggElist>

  RASSERT(pCI->G_bEachMain(CK_AGGREG));
  Board::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(pCI);

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard(__LINE__);

  // T A A A P F T T T W A A A     //
  //         v                     //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <projElist>

  if (pCI->G_bIsNextMain(CK_PROJECT)) {
    RASSERT(pCI->G_bEachMain(CK_PROJECT));
    Board::vEnhance_sq_project_To_Include_Correlation_Column(pCI,"__SQCRLKEY"_Stz);
  }
  else {

  }







  // T G G G P F T T T W A A A      //
  //             s s s              // adjust stream ordinsla

//  Card_Stream::vStream_AdjustLoHi(pCI);
//  Board::vRenumberStmOdn(pCI);
//  pCI->vPrintBoard(__LINE__);


  //pCI->G_vRestart();
  //RASSERT(pCI->G_bEachMainWithSub(CK_PROJECT,CK_PROJECTION));


  if (CK_PROJECT == pCI->G.e_MainCard || CK_GBYAGG == pCI->G.e_MainCard)
    pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
  else
    NEVER_REACH

  pCI->vRestart();
  RASSERT(pCI->G_bEachMain_reverse(CK_FOLD));

  // Table*
  i8 nTables = 0;
  while (pCI->G_bIsNextMain(CK_TABLE) && pCI->G_bEachMain(CK_TABLE)) {
    nTables++;
    pCI->G.p_table->vSmooth(pCI);
    pCI->vPrintBoard(__LINE__);
  }
  pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert the mq to join to the gby correlation column streamed from sq

  pCI->G_vRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((pCI->m_pTerm_corrCol))
  //                               //             %1

  while (pCI->G_bEachMainWith2Sub(CK_TABLE,CK_FILTER,CK_BOOLCALC))
    if (pCI->G.p_boolCalc->bYank_PRED(MARK_AT, pCI->G.p_PRED))
      break;
  RASSERT(pCI->G.p_PRED->Y_bHas(P_CorrPred));
  pCI->vPrintBoard(__LINE__);

  pCI->G.p_table->vRemove_filter_IfEmpty(pCI->G.p_filter);
  pCI->vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((pCI->m_pTerm_corrCol))
  //                               //             %1

  RASSERT(pCI->G_bEachMain(CK_FOLD));
  RASSERT(pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  pCI->G.p_boolCalc->vBump_Rightwards(2);

  pCI->G.p_boolCalc->vPush_Correlated_cmpPred_Onto_mq_boolCalc(pCI->G.p_PRED);
  pCI->vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]roPred <
  // f                 bc          //           $a.5
  //                               //           $b.1
  //                               // __subquery0002

  Board::vResolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(pCI, pCI->G.p_boolCalc);
  pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be in two steps

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
