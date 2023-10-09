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
  UNUSED(pCI);
  UNUSED(Correlate);

}

/*----------------------------------------------------------------------------*/
/* convert call of sub-query to correlated join to left sub-stream            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vVia_gbyAgg_Promotion(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // We have a flattened query.  It has a left-size SQ, and a right-side OQ

  //        SQ... ...OQ           //
  // T T A A A P F T T W A A A    //
  //                              //

  // Start with processing left-side SQ (sub-query)


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // identify correlation key predicate in SQ:<table>/<filter>/<boolCalc>//<cmpPred>
  //

  //   v                          //  ]filter
  // T T A A A P F T T W A A A    //  .]boolCalc
  //   f                          //  ..]cmpPred ==
  //   p                          //           $d.2  ((pCI->m_pTerm_corrCol))
  //                              //             %1

  pCI->vClearRestart();
  while (pCI->G_bEachMainWith2Sub(CK_FROM, CK_EMITTED, CK_FILTER))
   if (bIdentify_Correlation_Key_Predicate(pCI))
     break;
  RASSERT(pCI->Sqy.m_pTerm_corrCol); // eg $d.2 (L_PARTKEY)

  // find SQ:<op>/<projection>/<expr> which describes the correlation column
  vIdentify_Expr_Of_Correlation_Key_Column(pCI);
  RASSERT(pCI->Sqy.m_pExpr_corr);    // eg $15 (L_PARTKEY)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // enrich SQ:<aggreg> to be SQ:<gbyAgg>/<key=CorrCol>,
  //   thereby producing ordered subQuery stream

  RASSERT(pCI->G_bEachMain(CK_AGGREG));
  vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Key(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // include correlation column SQ:[<project>/<projection>]

  if (pCI->G_bIsNextMain(CK_PROJECT)) {
    RASSERT(pCI->G_bEachMain(CK_PROJECT));
    vEnhance_sq_project_To_Include_Correlation_Key(pCI, C_SQCRLKEY);
  }
  else {

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get final SQ:<op>/<projection>

  if (CK_PROJECT == pCI->G.e_MainCard || CK_GBYAGG == pCI->G.e_MainCard)
    pCI->G.p_emitted = pCI->G.p_MainCard->pSub_emitted();
  else
    NEVER_REACH

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  pCI->vBeginEachReverse();
  RASSERT(pCI->G_bEachMain_reverse_b(CK_FOLD));

  // Now processing OQ (outer-query)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // re-project the leading OQ:<table>* sequence
  pCI->vForwards(true);
  while (pCI->G_bIsNextMain(CK_FROM) && pCI->G_bEachMain(CK_FROM)) {
    pCI->G.p_from->vSmooth(pCI);
  }

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // subQuery subQuery subQuery subQuery subQuery subQuery subQuery subQuery
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  // Convert the OQ to join to the gby correlation column ordered streamed from SQ

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // yank the correlation predicate from SQ

  pCI->vClearRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((pCI->m_pTerm_corrCol))
  //                               //             %1

  Card_CONJUNCTION* p_PRED_corr = nullptr;
  while (pCI->G_bEachMainWith2Sub(CK_FROM,CK_FILTER,CK_BOOLCALC))
    if (pCI->G.p_boolCalc->bYank_PRED(MARK_AT, p_PRED_corr))
      break;
  RASSERT(p_PRED_corr->pSymbol()->Y_bHas(P_CrlKeyPred));

  // tidy up empty SQ:<table>/<filter>
  pCI->G.p_from->vDeleteFilterIfEmpty(pCI->G.p_filter);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((pCI->m_pTerm_corrCol))
  //                               //             %1

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  RASSERT(pCI->G_bEachMain(CK_FOLD));

  pCI->vForwards(false);
  while (!pCI->G.p_MainCard->pSub_emitted())
    pCI->G_bEachMain_reverse_NONE();
  pCI->vForwards(true);

  pCI->Prior.m_pEmitted = pCI->G.p_MainCard->pSub_emitted();
  RASSERT(pCI->G_bEachMain(CK_FOLD));

  RASSERT(pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  pCI->G.p_boolCalc->vBump_stmOdn(2);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push correlated cmpPred into the OQ:<where>/<constraint>/<boolCalc>

  pCI->G.p_boolCalc->vPush_CrlKeyPred_As_CrlKeyTest(p_PRED_corr);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]roPred <
  // f                 bc          //           $a.5
  //                               //           $b.1
  //                               // __subquery0002

  vConvert_CrlPrmOdn_Into_StmColOdn(pCI, pCI->G.p_boolCalc);

  vConvert_SubQryCall_Into_StmColOdn(pCI, pCI->G.p_boolCalc);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be done using standard predicate moving

  pCI->vClearRestart();

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((pCI->m_pTerm_corrCol))
  //             b <-  cp          //             %1

  RASSERT(pCI->G_bEachMain(CK_FOLD));
  RASSERT(pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  //Board::vMove_corrPred_To_mq_table_As_bounds(pCI, pCI->G.p_boolCalc);
  vMove_corrPred_To_table_filter(pCI, pCI->G.p_boolCalc);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->vClearRestart();
//  if (pCI->G_bEachMain(CK_FOLD)) {
//    if (pCI->G_bEachMain(CK_WHERE)) {
//      // TODO:
//      // R_vUTE_Unproject_foldExpr_After_where();
//      R_vUTE_Bump_where_BaseOn_foldExpr();
//    }
//  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CorrelatedJoin::bIdentify_Correlation_Key_Predicate(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold identify the <table>/<filter>/<boolCalc>/<cmpPred> which is correlated by parameter
  bool bIsCorrPred = false;

  auto bMarkCorrPred = [pCI,&bIsCorrPred](Card_CONJUNCTION* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCorrPred())
      return C_CONTINUE_RECURSION;

    //p_pred->vDump();
    bIsCorrPred = true;

    Card_Term* pTerm_corrCol = static_cast<Card_CmpConj *>(p_pred)->pGetCorrCol();
    pCI->Sqy.m_pTerm_corrCol = pTerm_corrCol;

    p_pred->vMark1(MARK_AT);
    p_pred->pSymbol()->Y_vSet(P_CrlKeyPred);
    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc = pCI->G.p_filter->pSub_boolCalc();
  Card_CONJUNCTION* p_PRED = static_cast<Card_CONJUNCTION*>(pCI->G.p_boolCalc->pGetChild(0));
  bool bIgnore = p_PRED->bPredRecursive(bMarkCorrPred);
  UNUSED(bIgnore);

  return bIsCorrPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vIdentify_Expr_Of_Correlation_Key_Column(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // find the <table>/<stream>/<stmColList>/<stmCol>
  //  corresponding to the correlated predicate

  pCI->Sqy.m_pExpr_corr = pCI->pExpr_For_col(pCI->Sqy.m_pTerm_corrCol,pCI->G.i_MainCard+1);

  pCI->Sqy.m_pExpr_corr->vMark1(MARK_AT);
  pCI->Sqy.m_pExpr_corr->pSymbol()->Y_vSet(P_CrlKeyExpr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // promote sq <aggreg> seq to <gbyAgg> seq

  for (i8 i_agg = 0; i_agg < 3; i_agg++) {

    pCI->G.p_MainCard->vCard(CK_GBYAGG);
    pCI->G.p_gbyAgg = _pCard_GbyAgg(pCI->G.p_MainCard);

    pCI->G.p_level = pCI->G.p_gbyAgg->pSub_level();
    eAL e_Level = (eAL) pCI->G.p_level->pSymbol()->Y_iGet(P_Level);

    pCI->G.p_stream = pCI->G.p_gbyAgg->pSub_stream();
    pCI->G.p_emitted = pCI->G.p_stream->pSub_emitted();

    pCI->G.p_key = new Card_Key;
    pCI->G.p_key->pSymbol()->Y_vSet(P_InjCrlKey);
    pCI->G.p_gbyAgg->vInsertChildBefore(pCI->G.p_key,pCI->G.p_stream);


    // key

    if (AL_PARTITION == e_Level) {
      pCI->G.p_expr = Card_Expr::pGby_expr_From_expr_corr(pCI->Sqy.m_pExpr_corr);
      pCI->G.p_key->vAppendChild(pCI->G.p_expr);
    }
    else {
      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, Symbol(SF_STMCOL, "$1"_Stz));
      pCI->G.p_term->pSymbol()->Y_vColType(pCI->Sqy.m_pExpr_corr->szP_Type());
      pCI->G.p_key->vAppendChild(pCI->G.p_expr);

      pCI->G.p_rpnCalc->pSymbol()->Y_vColType(pCI->Sqy.m_pExpr_corr->szP_Type());
      pCI->G.p_term->pSymbol()->Y_vColType(pCI->Sqy.m_pExpr_corr->szP_Type());
      pCI->G.p_expr->pSymbol()->Y_vColType(pCI->Sqy.m_pExpr_corr->szP_Type());

    }
    // TODO: hack...
    pCI->G.p_expr->v_Child(0);

    // projection

    if (AL_PARTITION == e_Level) {
      pCI->G.p_expr = Card_Expr::pProj_expr_From_Gby_expr(pCI->G.p_expr,C_SQCRLKEY);
    }
    else {
      pCI->G.p_key->pSymbol()->Y_vSet(P_ProjBump, (i8)1);


      Symbol xSymbol_keyOdn(SF_KEY_COL,1);

      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, Symbol(SF_AS_NM, C_SQCRLKEY));
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, &xSymbol_keyOdn);

      pCI->G.p_rpnCalc->pSymbol()->Y_vColType(pCI->G.p_key->szP_Type());
      pCI->G.p_term->pSymbol()->Y_vColType(pCI->G.p_key->szP_Type());
      pCI->G.p_expr->pSymbol()->Y_vColType(pCI->G.p_key->szP_Type());
    }

    pCI->G.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);
    pCI->G.p_emitted->vPushChild(pCI->G.p_expr);


    if (AL_PARTITION != e_Level) {
      pCI->G.p_emitted->vBump_stmOdn_In_rpnCalc(pCI);
      //pCI->G.p_key->Y_vCut(P_ProjBump);
      pCI->G.p_key->pSymbol()->Y_vSet(P_ProjBumped);
    }

    pCI->G.p_key->vRenumberAll_expr();
    pCI->G.p_emitted->vRenumberAll_expr();

    if (pCI->G_bEachMain_NONE()) {
      if (! pCI->G.p_MainCard->bIsCard(CK_AGGREG)) {
        bool b = pCI->G_bRewindMain();
        RASSERT(b);
        break;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vEnhance_sq_project_To_Include_Correlation_Key(CardIter* pCI, Stz* szAs)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold sq <project>

  RASSERT (pCI->G.p_MainCard->bIsCard(CK_PROJECT));
  RASSERT(pCI->G.p_key->szP_Type());

  pCI->G.p_emitted = pCI->G.p_project->pSub_emitted();
  pCI->G.p_emitted->vBump_stmOdn_In_rpnCalc(pCI);// TODO: ,nBumpBy);


  // push <project>/_SQCRLKEY_
  pCI->G_vPushNewChild(pCI->G.p_emitted, CK_EXPR);
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, Symbol(SF_AS_NM, szAs));
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, Symbol(SF_STMCOL, "$1"_Stz));
  Symbol x_Symbol_stmCol1(SF_STMCOL, 1);


  pCI->G.p_rpnCalc->pSymbol()->Y_vColType(pCI->G.p_key->szP_Type());
  pCI->G.p_term->pSymbol()->Y_vColType(pCI->G.p_key->szP_Type());
  pCI->G.p_expr->pSymbol()->Y_vColType(pCI->G.p_key->szP_Type());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // replace <project>/<stream> now that __SQ1IGBKY1 has been added

//  pCI->G.p_stream = pCI->G.p_project->pSub_stream();
//
//  pCI->G.p_project->vRemoveChild(pCI->G.p_stream);
//  DELETE_CARD(pCI->G.p_stream);
//
//  pCI->G.p_stream = Card_Stream::pDerive_Of_projection(pCI->G.p_emitted);
//  pCI->G.p_project->vAppendChild(pCI->G.p_stream);

//  pCI->G.p_emitted->Y_vCut(P_Restream);
//  pCI->G.p_emitted->Y_vSet(P_Restreamed);

//  i8 nStmCol = pCI->G.p_stream->pSub_stmColList()->nChildCards();
//  pCI->G.p_stream->Y_vSet(P_OdnLo,(i8)1);
//  pCI->G.p_stream->Y_vSet(P_OdnHi,(i8)nStmCol);

  // bump all $n in <project>/<projElist>/<expr>/<rpnCalc>
  //   now that __SQ1IGBKY1 has been added

  //i8 nBumpBy = pCI->G.p_emitted->Y_iGet(P_ProjBump);

//  pCI->G.p_emitted->Y_vSet(P_ProjBumped,(i8)nBumpBy);
//  pCI->G.p_emitted->Y_vCut(P_ProjBump);

  pCI->G.p_emitted->vRenumberAll_expr();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vConvert_CrlPrmOdn_Into_StmColOdn(CardIter* pCI, Card_BoolCalc *p_boolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [pCI](Card_CONJUNCTION* p_pred,
                        auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! Card_CONJUNCTION::bIsPRED(p_pred))
      return; /* this effectively declares resolve as void */

    if (p_pred->bIsCard(CK_CMPCONJ)) {

      if (!p_pred->pSymbol()->Y_bHas(P_CrlKeyTest))
        return;

      RASSERT(p_pred->pSymbol()->bSame("=="_Stz));

      Kid xKid(p_pred);
      while (xKid.bEach()) {
        // TODO: test for: == $d.2 %1 => == $1 [TRANSLATE PARAM %1]

        if (xKid.pChild()->pSymbol()->bIs_Sqy_Prm()) {

          Symbol x_Symbol_stmCol1(SF_STMCOL, 1);
          xKid.pChild()->pSymbol()->vFacets(&x_Symbol_stmCol1);  // TODO: hack!!

          Card_Expr* p_expr_found =
              pCI->Prior.m_pEmitted->pGetExpr(0);

          xKid.pChild()->pSymbol()->Y_vColType(p_expr_found->szP_Type());

        }
        else
        if (xKid.pChild()->bIsCard(CK_TERM) && xKid.pChild()->pSymbol()->bIs_Col_Tc()) {
          Card_OuterColNm* p_outerColNm = pCI->Sqy.m_pCorrelation->pSub_outerColNm();

          // todo: hack
          Card* p_card = p_outerColNm->pDupCardRecursive();
          p_card->vCard(CK_TERM);
          Card_Term* p_term = _pCard_Term(p_card);

          Card_Expr* p_expr_otr = pCI->pExpr_For_colNm(p_term);
          //p_expr_otr->vDump();
          Stz* szProjectedCol = p_expr_otr->pGetTerm(0)->szSymbol(SF_NONE);
          RASSERT(Symbol::bIs_Col_Tc(szProjectedCol));
          xKid.pChild()->pSymbol()->vFacet(SF_TBLCOL, szProjectedCol);  // TODO: hack!!

          DELETE_CARD(p_term);
        }
        else {
          NEVER_REACH
        }
      }
      return;
    }
    else
    if (p_pred->bIsCard(CK_ANDCONJ) || p_pred->bIsCard(CK_ORCONJ)) {
      Kid xKid(p_pred);
      while (xKid.bEach()) {
        RASSERT(Card_CONJUNCTION::bIsPRED(xKid.pChild()));
        vResolve((Card_CONJUNCTION*)xKid.pChild(), vResolve);
      }
    }
    else {
      NEVER_REACH;
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJUNCTION* p_PRED = static_cast<Card_CONJUNCTION*>(p_boolCalc->pGetChild(0));

  vResolve(p_PRED, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vConvert_SubQryCall_Into_StmColOdn(CardIter* pCI, Card_BoolCalc *p_boolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [pCI](Card_CONJUNCTION* p_pred,
                        auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! Card_CONJUNCTION::bIsPRED(p_pred))
      return; /* this effectively declares resolve as void */

//    if (p_pred->bIsCard(CK_CMPCONJ)) {
//      return;
//    }
//    else
    if (p_pred->bIsCard(CK_CMPCONJ)) {

      if (! p_pred->bContains_callToSubQuery(pCI))
        return;

      RASSERT(p_pred->bIsBoolFn() ||
              p_pred->bIsRelnOp() ||
              p_pred->bIsSubQueryOp());

      RASSERT(!p_pred->pSymbol()->Y_bHas(P_CrlKeyPred));

      p_pred->vCard(CK_CMPCONJ);
      Kid xKid(p_pred);
      while (xKid.bEach()) {
        if (Card_CONJUNCTION::bIsPRED(xKid.pChild())) {
          vResolve(static_cast<Card_CONJUNCTION*>(xKid.pChild()), vResolve);
          continue;
        }
        // TODO: test for: < $a.5 $b.1 __subquery2 => < $a.5 $2

        if (pCI->m_pBench->bIsSubQueryNm(xKid.pChild()->szSymbol(SF_NONE))) {
          Card* p_arg = xKid.pPrev();

          Card_Term* p_term_new = (new Card_Term) ->SYM(SF_STMCOL, "$2"_Stz);
          p_pred->vInsertChildAfter(p_term_new,xKid.pChild());


          Card_Expr* p_expr_found =
              pCI->Prior.m_pEmitted->pGetExpr(1);

           p_term_new->pSymbol()->Y_vColType(p_expr_found->szP_Type());


          p_pred->vRemoveChild(p_arg);
          DELETE_CARD(p_arg);

          Card* pChild = xKid.pChild();
          p_pred->vRemoveChild(pChild);
          DELETE_CARD(pChild);

          p_pred->pSymbol()->Y_vSet(P_CrlRslTest);
        }
      }
      return;
    }
    else
    if (p_pred->bIsCard(CK_ANDCONJ) || p_pred->bIsCard(CK_ORCONJ)) {
      Kid xKid(p_pred);
      while (xKid.bEach()) {
        RASSERT(Card_CONJUNCTION::bIsPRED(xKid.pChild()));
        vResolve(static_cast<Card_CONJUNCTION*>(xKid.pChild()), vResolve);
      }
    }
    else {
      NEVER_REACH;
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJUNCTION* p_PRED = static_cast<Card_CONJUNCTION*>(p_boolCalc->pGetChild(0));

  vResolve(p_PRED, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vMove_corrPred_To_table_filter(CardIter* pCI, Card_BoolCalc *p_boolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  Card_CONJUNCTION* p_pred_corr = p_boolCalc->pFindRemove_CorrPred();
  if (! p_pred_corr)
    return;

  //p_pred_corr->vDump();

  Card_Term* p_term_tblColOdn = p_pred_corr->pGetFirstTerm_tblColOdn();
  char cTblSyn = p_term_tblColOdn->cGetTblSyn();

  Card_OP* p_OP = Card_Table::pFind_By_tblSyn(pCI, cTblSyn, pCI->G.i_MainCard);

  p_OP->vAdd_Pred_Under_filter(p_pred_corr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
