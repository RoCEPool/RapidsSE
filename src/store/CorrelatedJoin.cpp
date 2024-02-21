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
  // identify correlation key predicate in SQ:<table>/<filter>/<boolPred>//<cmpPred>
  //

  //   v                          //  ]filter
  // T T A A A P F T T W A A A    //  .]boolPred
  //   f                          //  ..]cmpPred ==
  //   p                          //           $d.2  ((pCI->m_pTerm_corrCol))
  //                              //             %1

  pCI->vClearRestart();
  while (pCI->G_bEachMainWith2Sub(CK_o_FROM, CK_t_EMIT, CK_o_FILTER))
   if (bIdentify_Correlation_Key_Predicate(pCI))
     break;
  RASSERT(pCI->Sqy.m_pTerm_corrCol); // eg $d.2 (L_PARTKEY)

  // find SQ:<op>/<projection>/<expr> which describes the correlation column
  vIdentify_Expr_Of_Correlation_Key_Column(pCI);
  RASSERT(pCI->Sqy.m_pExpr_corr);    // eg $15 (L_PARTKEY)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // enrich SQ:<aggreg> to be SQ:<gbyAgg>/<key=CorrCol>,
  //   thereby producing ordered subQuery stream

  RASSERT(pCI->G_bEachMain(CK_o_AGGREG));
  vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Key(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // include correlation column SQ:[<project>/<projection>]

  if (pCI->G_bIsNextMain(CK_o_PROJECT)) {
    RASSERT(pCI->G_bEachMain(CK_o_PROJECT));
    vEnhance_sq_project_To_Include_Correlation_Key(pCI, C_SQCRLKEY);
  }
  else {

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get final SQ:<op>/<projection>

  if (CK_o_PROJECT == pCI->G.e_MainCard || CK_o_GBYAGG == pCI->G.e_MainCard)
    pCI->G.p_emit = pCI->G.p_MainCard->pSub_emit();
  else
    NEVER_REACH

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  pCI->vBeginEachReverse();
  RASSERT(pCI->G_bEachMain_reverse_b(CK_c_FOLD));

  // Now processing OQ (outer-query)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // re-project the leading OQ:<table>* sequence
  pCI->vForwards(true);
  while (pCI->G_bIsNextMain(CK_o_FROM) && pCI->G_bEachMain(CK_o_FROM)) {
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
  // T A A A P F T T T W A A A     //  .]boolPred
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((pCI->m_pTerm_corrCol))
  //                               //             %1

  // todo: this should be Card_RlnConj
  Card_CONJ* p_conj_corr = nullptr;
  while (pCI->G_bEachMainWith2Sub(CK_o_FROM, CK_o_FILTER, CK_BOOLPRED))
    if ((p_conj_corr = pCI->G.p_boolPred->pYank_rlnConj(MARK_AT)))
      break;
  RASSERT(p_conj_corr->pSymbol()->Y_bHas(P_CrlKeyPred));

  // tidy up empty SQ:<table>/<filter>
  pCI->G.p_from->vCullIfEmpty_filter(pCI->G.p_filter);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolPred
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((pCI->m_pTerm_corrCol))
  //                               //             %1

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  RASSERT(pCI->G_bEachMain(CK_c_FOLD));

  pCI->vForwards(false);
  while (!pCI->G.p_MainCard->pSub_emit())
    pCI->G_bEachMain_reverse_NONE();
  pCI->vForwards(true);

  pCI->Flux.m_pArriving = pCI->G.p_MainCard->pSub_emit();
  RASSERT(pCI->G_bEachMain(CK_c_FOLD));

  RASSERT(pCI->G_bEachMainWithSub(CK_o_WHERE, CK_BOOLPRED));
  pCI->G.p_boolPred->vBump_expCol(2);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push correlated cmpPred into the OQ:<where>/<constraint>/<boolPred>

  p_conj_corr->pSymbol()->Y_vCut(P_CrlKeyPred);
  p_conj_corr->pSymbol()->Y_vSet(P_CrlKeyTest);

  pCI->G.p_boolPred->vExtend_andConj(_pCard_RlnConj(p_conj_corr));

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolPred
  // s                 f           //  ..]roPred <
  // f                 bc          //           $a.5
  //                               //           $b.1
  //                               // __subquery0002

  vConvert_SqyPrm_Into_ExpCol(pCI, pCI->G.p_boolPred);

  vConvert_SqyNo_Into_ExpCol(pCI, pCI->G.p_boolPred);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be done using standard predicate moving

  pCI->vClearRestart();

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolPred
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((pCI->m_pTerm_corrCol))
  //             b <-  cp          //             %1

  RASSERT(pCI->G_bEachMain(CK_c_FOLD));
  RASSERT(pCI->G_bEachMainWithSub(CK_o_WHERE, CK_BOOLPRED));
  //Board::vMove_corrPred_To_mq_table_As_bounds(pCI, pCI->G.p_boolPred);
  vMove_corrPred_To_table_filter(pCI, pCI->G.p_boolPred);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->vClearRestart();
//  if (pCI->G_bEachMain(CK_c_FOLD)) {
//    if (pCI->G_bEachMain(CK_o_WHERE)) {
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
  // pre-fold identify the <table>/<filter>/<boolPred>/<cmpPred> which is correlated by parameter
  bool bIsCorrPred = false;

  auto bMarkCorrPred = [pCI,&bIsCorrPred](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (!p_conj->bIsCorrConj())
      return C_CONTINUE_RECURSION;

    //p_conj->vDump();
    bIsCorrPred = true;

    Card_Term* pTerm_corrCol = static_cast<Card_RlnConj *>(p_conj)->pGetCorrCol();
    pCI->Sqy.m_pTerm_corrCol = pTerm_corrCol;

    p_conj->vMark1(MARK_AT);
    p_conj->pSymbol()->Y_vSet(P_CrlKeyPred);
    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolPred = pCI->G.p_filter->pSub_boolPred();
  Card_CONJ* p_conj = static_cast<Card_CONJ*>(pCI->G.p_boolPred->pGetChild(0));

  //p_conj->vDump(__func__);

  bool bIgnore = p_conj->bConjRecursive(bMarkCorrPred);
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

  pCI->Sqy.m_pExpr_corr = pCI->pExpr_For_col(pCI->Sqy.m_pTerm_corrCol,
                                             pCI->G.i_MainCard+1);

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

    pCI->G.p_MainCard->vCard(CK_o_GBYAGG);
    pCI->G.p_gbyAgg = _pCard_GbyAgg(pCI->G.p_MainCard);

    pCI->G.p_level = pCI->G.p_gbyAgg->pSub_level();
    eAL e_Level = (eAL) pCI->G.p_level->pSymbol()->Y_iGet(P_Level);

    pCI->G.p_stream = pCI->G.p_gbyAgg->pSub_stream();
    pCI->G.p_emit = pCI->G.p_stream->pSub_emit();

    pCI->G.p_key = new Card_Key;
    pCI->G.p_key->pSymbol()->Y_vSet(P_InjCrlKey);
    pCI->G.p_gbyAgg->vInsertChildBefore(pCI->G.p_key,pCI->G.p_stream);


    // key

    if (AL_PARTITION == e_Level) {
      pCI->G.p_expr = Card_Expr::pGby_expr_From_expr_corr(pCI->Sqy.m_pExpr_corr);
      pCI->G.p_key->vAppendChild(pCI->G.p_expr);
      if (1 == pCI->G.p_key->nChildCards())
        pCI->G.p_key->vCast(pCI->G.p_expr->xCast());
    }
    else {
      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G.p_expr->vCast(pCI->Sqy.m_pExpr_corr->xCast());
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, Symbol(SF_EXP_COL, "$1"_Stz));
      pCI->G.p_term->vCast(pCI->Sqy.m_pExpr_corr->xCast());
      pCI->G.p_key->vAppendChild(pCI->G.p_expr);
      if (1 == pCI->G.p_key->nChildCards())
        pCI->G.p_key->vCast(pCI->G.p_expr->xCast());

      pCI->G.p_rpnCalc->vCast(pCI->Sqy.m_pExpr_corr->xCast());
      pCI->G.p_term->vCast(pCI->Sqy.m_pExpr_corr->xCast());

    }
    // TODO: hack...
    pCI->G.p_expr->v_Child(0);

    // projection

    if (AL_PARTITION == e_Level) {
      pCI->G.p_expr = Card_Expr::pProj_expr_From_Gby_expr(pCI->G.p_expr,C_SQCRLKEY);
    }
    else {
      pCI->G.p_key->pSymbol()->Y_vSet(P_ProjBump, (i8)1);


      Symbol xSymbol_KeyCol(SF_KEY_COL,1);

      pCI->G_vNewMember(CK_EXPR);
      pCI->G.p_expr->pSymbol()->vApplyFacet(SF_NMD_COL, C_SQCRLKEY);
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, &xSymbol_KeyCol);

      pCI->G.p_rpnCalc->vCast(pCI->G.p_key->xCast());
      pCI->G.p_term->vCast(pCI->G.p_key->xCast());
      pCI->G.p_expr->vCast(pCI->G.p_key->xCast());
    }

    pCI->G.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);
    pCI->G.p_emit->vPushChild(pCI->G.p_expr);


    if (AL_PARTITION != e_Level) {
      pCI->G.p_emit->vBump_expCol_In_rpnCalc(pCI);
      //pCI->G.p_key->Y_vCut(P_ProjBump);
      pCI->G.p_key->pSymbol()->Y_vSet(P_ProjBumped);
    }

    pCI->G.p_key->vRenumberAll_expr();
    pCI->G.p_emit->vRenumberAll_expr();

    if (pCI->G_bEachMain_NONE()) {
      if (! pCI->G.p_MainCard->bIsCard(CK_o_AGGREG)) {
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

  RASSERT (pCI->G.p_MainCard->bIsCard(CK_o_PROJECT));
  RASSERT(CT_NONE != pCI->G.p_key->eCTVal());

  pCI->G.p_emit = pCI->G.p_project->pSub_emit();
  pCI->G.p_emit->vBump_expCol_In_rpnCalc(pCI);// TODO: ,nBumpBy);


  // push <project>/_SQCRLKEY_
  pCI->G_vPushNewChild(pCI->G.p_emit, CK_EXPR);
  pCI->G.p_expr->pSymbol()->vApplyFacet(SF_NMD_COL, szAs);
  //pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, Symbol(SF_NMD_COL, szAs));
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, Symbol(SF_EXP_COL, "$1"_Stz));
  Symbol x_Symbol_expCol1(SF_EXP_COL, 1);


  pCI->G.p_rpnCalc->vCast(pCI->G.p_key->xCast());
  pCI->G.p_term->vCast(pCI->G.p_key->xCast());
  pCI->G.p_expr->vCast(pCI->G.p_key->xCast());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // replace <project>/<stream> now that __SQ1IGBKY1 has been added

//  pCI->G.p_stream = pCI->G.p_project->pSub_stream();
//
//  pCI->G.p_project->vRemoveChild(pCI->G.p_stream);
//  DELETE_CARD(pCI->G.p_stream);
//
//  pCI->G.p_stream = Card_Stream::pDerive_Of_projection(pCI->G.p_emit);
//  pCI->G.p_project->vAppendChild(pCI->G.p_stream);

//  pCI->G.p_emit->Y_vCut(P_Restream);
//  pCI->G.p_emit->Y_vSet(P_Restreamed);

//  i8 nStmCol = pCI->G.p_stream->pSub_stmColList()->nChildCards();
//  pCI->G.p_stream->Y_vSet(P_OdnLo,(i8)1);
//  pCI->G.p_stream->Y_vSet(P_OdnHi,(i8)nStmCol);

  // bump all $n in <project>/<projElist>/<expr>/<rpnCalc>
  //   now that __SQ1IGBKY1 has been added

  //i8 nBumpBy = pCI->G.p_emit->Y_iGet(P_ProjBump);

//  pCI->G.p_emit->Y_vSet(P_ProjBumped,(i8)nBumpBy);
//  pCI->G.p_emit->Y_vCut(P_ProjBump);

  pCI->G.p_emit->vRenumberAll_expr();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vConvert_SqyPrm_Into_ExpCol(CardIter* pCI, Card_BoolPred *p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [pCI](Card_CONJ* p_conj,
                        auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (!Card_CONJ::bIsConj(p_conj))
      return; /* this effectively declares resolve as void */

    if (p_conj->bIsCard(CK_RLNCONJ)) {

      if (!p_conj->pSymbol()->Y_bHas(P_CrlKeyTest))
        return;

      RASSERT(p_conj->pSymbol()->bHasFacet("=="_Stz));

      Kid xKid(p_conj);
      while (xKid.bEach()) {
        // TODO: test for: == $d.2 %1 => == $1 [TRANSLATE PARAM %1]

        if (xKid.pChild()->pSymbol()->bHasFacet(SF_SQY_PRM)) {

          Symbol x_Symbol_expCol1(SF_EXP_COL, 1);
          xKid.pChild()->pSymbol()->vApplyCastFacets(&x_Symbol_expCol1);  // TODO: hack!!

          Card_Expr* p_expr_found =
              pCI->Flux.m_pArriving->pGet_expr((i8) 0);

          xKid.pChild()->vCast(p_expr_found->xCast());

        }
        else
        if (xKid.pChild()->bIsCard(CK_TERM) && xKid.pChild()->pSymbol()->bHasFacet(SF_TBL_COL)) {
          Card_OuterColNm* p_outerColNm = pCI->Sqy.m_pCorrelation->pSub_outerColNm();

          // todo: hack
          Card* p_card = p_outerColNm->pDupCardRecursive();
          p_card->vCard(CK_TERM);
          Card_Term* p_term = _pCard_Term(p_card);

          Card_Expr* p_expr_otr = pCI->pExpr_For_colNm(p_term);
          Stz* szProjectedCol = p_expr_otr->pGet_term(0)->szFacet(SF_TBL_COL);
          xKid.pChild()->pSymbol()->vApplyFacet(SF_TBL_COL, szProjectedCol);  // TODO: hack!!

          DELETE_CARD(p_term);
        }
        else {
          NEVER_REACH
        }
      }
      return;
    }
    else
    if (p_conj->bIsCard(CK_ANDCONJ) || p_conj->bIsCard(CK_ORCONJ)) {
      Kid xKid(p_conj);
      while (xKid.bEach()) {
        RASSERT(Card_CONJ::bIsConj(xKid.pChild()));
        vResolve((Card_CONJ*)xKid.pChild(), vResolve);
      }
    }
    else {
      NEVER_REACH;
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJ* p_conj = static_cast<Card_CONJ*>(p_boolPred->pGetChild(0));

  vResolve(p_conj, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vConvert_SqyNo_Into_ExpCol(CardIter* pCI, Card_BoolPred *p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto vResolve = [pCI](Card_CONJ* p_conj,
                        auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (!Card_CONJ::bIsConj(p_conj))
      return; /* this effectively declares resolve as void */

    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // process AND or OR

    if (p_conj->bIsCard(CK_ANDCONJ) || p_conj->bIsCard(CK_ORCONJ)) {
      for (Kid xConj(p_conj); xConj.bEach(); /**/) {
        RASSERT(Card_CONJ::bIsConj(xConj.pChild()));
        vResolve(static_cast<Card_CONJ*>(xConj.pChild()), vResolve);
      }
      return;
    }

    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // process <RLNCONJ>

    RASSERT(p_conj->bIsCard(CK_RLNCONJ));

    if (!p_conj->bContains_sqyNo())
      return;

    RASSERT(! p_conj->pSymbol()->Y_bHas(P_CrlKeyPred));
    RASSERT(p_conj->bIsRelnOp());

    for (Kid xConj(p_conj); xConj.bEach(); /**/) {

      Card* p_child_conj = xConj.pChild();
      if (Card_CONJ::bIsConj(p_child_conj)) {
        vResolve(static_cast<Card_CONJ*>(p_child_conj), vResolve);
        continue;
      }
      // TODO: test for: < $a.5 $b.1 __subquery2 => < $a.5 $2

      if (! p_child_conj->pSymbol()->bHasFacet(SF_SQY_NO))
        continue;

//      Stz* szSqyNo = p_child_conj->szFacet(SF_SQY_NO);
//      RASSERT(Symbol::bIs_Sqy_No(szSqyNo));
      pCI->m_pBench->vDump(__func__);
      RASSERT(pCI->m_pBench->bKnown_sqy(p_child_conj->pSymbol()));


      Card* p_prev_conj = xConj.pPrev();
      p_conj->vRemoveChild(p_prev_conj);
      DELETE_CARD(p_prev_conj);

      p_child_conj->pSymbol()->vApplyFacet(SF_EXP_COL, "$2"_Stz);
      Card_Expr* p_expr_found = pCI->Flux.m_pArriving->pGet_expr(1);
      p_child_conj->vCast(p_expr_found->xCast());

      p_conj->pSymbol()->Y_vSet(P_CrlRslTest);
    }

    return;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJ* p_conj = static_cast<Card_CONJ*>(p_boolPred->pGetChild(0));

  vResolve(p_conj, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CorrelatedJoin::vMove_corrPred_To_table_filter(CardIter* pCI, Card_BoolPred *p_boolPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CORRELATEDJOIN, _FOOTER_);

  // todo: this should be Card_RlnConj
  Card_RlnConj* p_rlnConj_corr = p_boolPred->pFind_rlnConj(P_CrlRslTest);
  if (! p_rlnConj_corr)
    return;

  p_boolPred->vRemove_rlnConj(p_rlnConj_corr);

  //p_rlnConj_corr->vDump();

  Card_Term* p_term_tblCol = p_rlnConj_corr->pGetFirstTerm_tblCol();
  char cTblSyn = p_term_tblCol->pSymbol()->cTblSyn();

  Card_OP* p_OP = Card_Table::pFind_By_tblSyn(pCI, cTblSyn, pCI->G.i_MainCard);

  pCI->G.p_filter = p_OP->pEnsure_filter();

  pCI->G.p_filter->vExtend_rlnConj(p_rlnConj_corr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
