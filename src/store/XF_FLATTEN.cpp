/*============================================================================*/
/* XF_FLATTEN.cpp -- Flatten                                              */
/*============================================================================*/

#include "XF_FLATTEN.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_FLATTEN::XF_FLATTEN(eCN eClass, CardIter* pCI)
: XF_TRANSFORM(eClass, pCI)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_FLATTEN::~XF_FLATTEN(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* XF_Fold a from                                                             */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vFold_from(Stz* szFoldNm, CardIter* pCI)
{

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolPred>

  RASSERT(pCI->bEachOp(CK_o_CALL));

  Board* pBoard = pCI->m_pBench->pFindBoard(pCI->E.p_call->pSymbol());
  if (! pBoard) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->E.p_call->szFacet(SF_SQY_NO));
    NEVER_REACH
  }

  pCI->E.p_fold = Card_Fold::pCraft_fold(szFoldNm, pBoard->i_Board());
  pCI->E.p_deck->vPushChild(pCI->E.p_fold);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq

  pCI->vPushEach();
  pCI->vUseBoard(pBoard);

  pCI->Flat.m_pCopy_inner = pCI->E.p_deck->pDupSubCardsRecursive(0);

  // pCI->PRINTBOARD_dbg();

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  pCI->vPopEach();
  pCI->E.p_deck->vPushChild(pCI->Flat.m_pCopy_inner);
  pCI->E.p_deck->vRemoveChildLeavingSubs(pCI->Flat.m_pCopy_inner);
  DELETE_CARD(pCI->Flat.m_pCopy_inner);

  pCI->E.p_deck->vRemoveChild(pCI->E.p_call);
  DELETE_CARD(pCI->E.p_call);

  // 7) derive the fold (SQ result) expr
  pCI->vClearRegisters();
  pCI->vBeginEach();
  if (pCI->bEachOp(CK_o_FOLD)) {
    pCI->Flat.m_i_fold = pCI->E.i_Op;
    Board::vDerive_foldProjection_F(pCI);
  }
}


/*----------------------------------------------------------------------------*/
/* XF_Fold a sub-query (before outer-query)                                   */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vFold_subQuery(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ)
{
  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolPred>/<roPred>
  pCI->vBeginEach_tail();
  if (pCI->bEachOp_reverse_foldBreak(eCardWithSQ, CK_PREDICATE)) {
    if (!Board::bMark_sqyCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz, szFoldNm);
      NEVER_REACH
    }
  }
  // pCI->PRINTBOARD_dbg();

  // 2) push to inner query
  Board* pBoard_inner = pCI->m_pBench->pFindBoard(pCI->Flat.m_p_term_sqyCall->pSymbol());
  if (! pBoard_inner) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Flat.m_p_term_sqyCall->szFacet(SF_SQY_NO));
    NEVER_REACH
  }

  pCI->vPushEach();
  Board* pBoard_outer = pCI->pBoard();
  pCI->vUseBoard(pBoard_inner);

  // pCI->PRINTBOARD_dbg();

  i8 i_dup_from = 0;
  pCI->vBeginEach();
  if (pCI->bEachOp(CK_o_CORRELATION)) {
    pCI->Flat.m_pCorrelation = pCI->E.p_deck->pSub_correlation();
    i_dup_from = 1;
  }
  else {
    vDebugPoint();
  }


  // 3) get copy of this inner query
  pCI->Flat.m_pCopy_inner = pCI->E.p_deck->pDupSubCardsRecursive(i_dup_from);

  // 4) pop back to outer query
  pCI->vUseBoard(pBoard_outer);
  pCI->vPopEach();

  // 5) add <fold> (before MQ)
  pCI->E.p_fold = Card_Fold::pCraft_fold(szFoldNm, pBoard_inner->i_Board());
  pCI->E.p_deck->vPushChild(pCI->E.p_fold);

  // 6) copy up inner (add before fold)
  pCI->E.p_deck->vPushChild(pCI->Flat.m_pCopy_inner);
  pCI->E.p_deck->vRemoveChildLeavingSubs(pCI->Flat.m_pCopy_inner);
  DELETE_CARD(pCI->Flat.m_pCopy_inner);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRegisters();
  pCI->vBeginEach();
  while (pCI->bEachOp()) {

    pCI->vSnapIf_arriving();
    if (pCI->pPeekNextIf(CK_o_FOLD))
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 7) derive the fold (SQ result) expr
  pCI->Flat.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->bEachOp(CK_o_FOLD)) {
    pCI->Flat.m_i_fold = pCI->E.i_Op;
    pCI->E.p_fold->vDerive_emit_fold(pCI, pBoard_inner->pSubQuery());
    pCI->Flat.m_pExpr_fold = pCI->E.p_expr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.pBoard->vRenumberCards();

  // pCI->PRINTBOARD_dbg();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vInto_KEYnotEXISTS(CardIter* pCI)
{
  pCI->vClearRegisters();
  pCI->vBeginEach();
  RASSERT(pCI->bEachOp_break(CK_o_FROM));


  //RASSERT(pCI->bEachOp(CK_o_WHERE));
  RASSERT(pCI->bEachOp_break(CK_o_PROJECT));

//  pCI->G.p_constraint = pCI->G.p_where->pSub_constraint();
//  pCI->G.p_where->vRemoveSub(pCI->G.p_constraint);
//
//  //pCI->G.p_filter = Card_Filter::pNew(p_constraint);
//  pCI->G.p_table->vAppendChild(pCI->G.p_constraint);

  Card_From* p_from_exists = pCI->E.p_from;
  pCI->E.p_deck->vRemoveChild(p_from_exists);

  pCI->E.p_term = pCI->E.p_project->pSub_term();
  pCI->E.p_project->vRemoveSub(pCI->E.p_term);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->G.p_deck->vDeleteChild(pCI->G.p_where);
  pCI->E.p_deck->vDeleteChild(pCI->E.p_project);

  // pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vBeginEach_tail();
  RASSERT(pCI->bEachOp_reverse_foldBreak(CK_o_WHERE));

  RASSERT(pCI->bEachOp_reverse_foldBreak(CK_o_FROM));
  Card_Emit* p_emitted_table = pCI->E.p_from->pSub_emit();

  pCI->vBeginEach_tail();
  RASSERT(pCI->bEachOp_reverse_foldBreak(CK_o_WHERE));


  pCI->E.p_deck->vInsertChildBefore(p_from_exists, pCI->E.p_where);

  pCI->E.p_predicate = pCI->E.p_where->pSub_predicate();
  pCI->E.p_rlnConj  = _pCard_RlnConj(pCI->E.p_predicate->pFindCard_WithSubFacet(CK_RLNCONJ, SF_SQY_NO));

  if (pCI->E.p_rlnConj->pSymbol()->bHasFacet("in"_Stz) ||
      pCI->E.p_rlnConj->pSymbol()->bHasFacet("exists"_Stz))
    p_from_exists->vAppendChild(new Card_KeyExists);
  else
  if (pCI->E.p_rlnConj->pSymbol()->bHasFacet("notIn"_Stz) ||
      pCI->E.p_rlnConj->pSymbol()->bHasFacet("notExists"_Stz))
    p_from_exists->vAppendChild(new Card_KeyNotExists);

  pCI->vSnapIf_arriving(p_emitted_table,nullptr);
  pCI->E.p_rlnConj->vMorph_sqlOp_To_EQ(pCI, pCI->E.p_term);

  DELETE_CARD(pCI->E.p_term);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! pCI->Flat.m_pCorrelation) {
    return;
  }

  // todo: the %1 needs to be decoded... its a parameter
  Card* p_parent;
  Symbol xPrmOrd(SF_SQY_PRM, 1);
  Card_Term* p_term_sqyarg = _pCard_Term(p_from_exists->pFindSymbolRecursive(CK_TERM,&xPrmOrd,p_parent));

  Card_Expr* p_expr_found =
      p_emitted_table->pGet_expr(p_term_sqyarg->pSymbol()->iTo_ordinal(SF_SQY_PRM) - 1);

  //p_expr_found->vDump();
  Card_Term* p_term_found = p_expr_found->pGet_term(0);


  Card_Term* p_term_sqyArg = pCI->Flat.m_p_term_crlKey->pClone();
  p_term_sqyArg->vApplyCast_p(p_term_found);
  //p_term_sqyArg->vDump();


  Symbol xSymbol_prmCol(SF_SQY_PRM, 1);
  p_from_exists->bUpdateSymbolRecursive(CK_TERM, &xSymbol_prmCol, p_term_sqyArg->pSymbol());

  delete p_term_sqyArg;

  // pCI->PRINTBOARD_dbg();

  pCI->vClearRegisters();
  pCI->vBeginEach();
  if (pCI->bEachOp(CK_o_WHERE, CK_PREDICATE)) {
    (void)pCI->E.p_predicate->nCullMarked_rlnConj(MARK_AT);
  }

  pCI->vClearRegisters();
  pCI->vBeginEach();
  pCI->vDeleteWhenEmptySub(CK_o_WHERE, CK_PREDICATE);

  vDebugPoint();

}

/*----------------------------------------------------------------------------*/
/* set smooth                                                                 */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vSet_smooth(CardIter* pCI, eCK eCardWithSQ, eQO eWhatOp)
{
  pCI->vBeginEach();

  RASSERT(pCI->bEachOp(CK_o_FOLD));
  RASSERT(pCI->E.p_Op->bIsCard(CK_o_FOLD));

  // <subQuery>
  if (pCI->pPeekNextIf(CK_SUBQUERY))
    RASSERT(pCI->bEachOp());

  // <from>*
  while (pCI->pPeekNextIf(CK_o_FROM) && pCI->bEachOp(CK_o_FROM))
    {;}

  // [<where>]
  if (pCI->pPeekNextIf(CK_o_WHERE) && pCI->bEachOp(CK_o_WHERE))
    {;}

  // <aggreg>*
  while (pCI->pPeekNextIf(CK_o_AGGREG) && pCI->bEachOp(CK_o_AGGREG))
    {;}

  // <gbyAgg>*
  while (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG))
  {;}

  // [<project>]
  if (pCI->pPeekNextIf(CK_o_PROJECT) && pCI->bEachOp(CK_o_PROJECT)) {
    pCI->Smooth.Project_postAgg.m_bSeen = true;
  }

  // <having>
  if (pCI->pPeekNextIf(CK_o_HAVING) && pCI->bEachOp(CK_o_HAVING))
  {;}

  // <sort>
  if (pCI->pPeekNextIf(CK_o_SORT) && pCI->bEachOp(CK_o_SORT)) {
    pCI->Smooth.Sort.m_bCorrect_After_Inject = true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_o_HAVING == eCardWithSQ) {
    pCI->Smooth.GbyAgg.m_bInject_foldExpr = true;
    pCI->Smooth.Having.m_bCorrect_After_Inject = true;
    pCI->Smooth.Unproject.m_bUnproject_foldExpr = true;
  }
  else {
    if (QO_IN_OP == eWhatOp || QO_NOT_IN_OP == eWhatOp)
      pCI->Smooth.Where_preAgg.m_bMorph_sqlOp_To_EQ = true;
    else
    if (QO_RELN_OP == eWhatOp)
      pCI->Smooth.Where_preAgg.bReplace_sqCall_With_result = true;
  }
}

/*----------------------------------------------------------------------------*/
/* smooth after fold                                                          */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vSmooth(CardIter* pCI)
{
  //pCI->PRINTBOARD_dbg();

  pCI->vBeginEach();
  RASSERT(pCI->bEachOp(CK_o_FOLD));
  RASSERT(pCI->E.p_Op->bIsCard(CK_o_FOLD));

  if (pCI->pPeekNextIf(CK_SUBQUERY))
    RASSERT(pCI->bEachOp());

  // <from>*
  while (pCI->pPeekNextIf(CK_o_FROM) && pCI->bEachOp(CK_o_FROM)) {
      pCI->E.p_table = pCI->E.p_from->pSub_table();
      pCI->E.p_Op->vRestate_emit(pCI);
    pCI->vSnapIf_arriving();
  }

  // [<where>]
  if (pCI->pPeekNextIf(CK_o_WHERE) && pCI->bEachOp(CK_o_WHERE)) {
      pCI->Dic.m_p_expr = pCI->Arriving.m_pEmitted_prior;
      pCI->E.p_where->vSmooth(pCI);
      pCI->E.p_Op->vRestate_emit(pCI);
    pCI->vSnapIf_arriving();
  }

  // [<gbyAgg partition|node|origin>]
  while (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
      pCI->E.p_gbyAgg->vSmooth(pCI);
      pCI->E.p_Op->vRestate_emit(pCI);
    pCI->vSnapIf_arriving();
  }

  // [<project>]
  if (pCI->pPeekNextIf(CK_o_PROJECT) && pCI->bEachOp(CK_o_PROJECT)) {
      pCI->E.p_project->vSmooth_postAgg(pCI);
      pCI->E.p_Op->vRestate_emit(pCI);
    pCI->vSnapIf_arriving();
  }

  // [<having>]
  if (pCI->pPeekNextIf(CK_o_HAVING) && pCI->bEachOp(CK_o_HAVING)) {
      pCI->E.p_having->vSmooth(pCI);
      pCI->E.p_Op->vRestate_emit(pCI);
    pCI->vSnapIf_arriving();
  }

  // [<sort>]
  if (pCI->pPeekNextIf(CK_o_SORT) && pCI->bEachOp(CK_o_SORT)) {
    if (pCI->Arriving.m_pEmitted_prior) {
      pCI->E.p_sort->vSmooth(pCI);
      pCI->E.p_Op->vRestate_emit(pCI);
    }
    pCI->vSnapIf_arriving();
  }

  //  --  --  --  --  --  --  --  --  --  --  --  --  --  --

  if (pCI->Smooth.Unproject.m_bUnproject_foldExpr) {
    pCI->E.p_emit = pCI->E.p_gbyAgg->pSub_emit();
    Card_Project::vUnproject_foldExpr(pCI);
    //pCI->E.p_Op->vRestate_emit(pCI);
    //pCI->vSnapIf_arriving();
  }

  //pCI->PRINTBOARD_dbg();
}

/*----------------------------------------------------------------------------*/
/* count number of tables in sub-query                                        */
/*----------------------------------------------------------------------------*/
i8 XF_FLATTEN::nTables_In_SubQuery(CardIter* pCI, Stz* szFold, eCK eCardWithSQ)
{
  _static_

  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolPred>/<roPred>
  pCI->vClearRegisters();
  pCI->vBeginEach();
  if (pCI->bEachOp(eCardWithSQ, CK_PREDICATE)) {
    if (!Board::bMark_sqyCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz,szFold);
      NEVER_REACH
    }
  }
  // pCI->PRINTBOARD_dbg();

  // 2) push to sq
  Board* pBoard = pCI->m_pBench->pFindBoard(pCI->Flat.m_p_term_sqyCall->pSymbol());
  if (! pBoard) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Flat.m_p_term_sqyCall->szFacet(SF_SQY_NO));
    NEVER_REACH
  }

  pCI->vPushEach(pBoard);
  i8 nTables = Card_Table::nTables(pCI);
  pCI->vPopEach();

  return nTables;
}

/*----------------------------------------------------------------------------*/
/* remove left-most subquery and rotate next sub-query to be first card       */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vRotate_Next(void)
{
  //m_pCI->vTransform(__FILE__, __func__, XF_FLATTEN_DELETE_LEFT_SUBQUERY);

  Kid xDeck(m_pCI->E.p_deck);
  if (xDeck.bEach(CK_SUBQUERY)) {
    m_pCI->E.p_deck->vRotateFirst(xDeck.pChild());
  }

  //m_pCI->vEndTransform(__LINE__);
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert call of sub-query to correlated join to left sub-stream            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vPromote_aggreg_To_gbyAgg(CardIter* pCI)
{

  // We have a flattened query.  It has a left-size IQ, and a right-side OQ

  //        IQ... ...OQ           //
  // F F A A A P + F F W A A A    //
  //                              //

  // Start with processing left-side IQ (inner sub-query)

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // innerQuery innerQuery innerQuery innerQuery innerQuery innerQuery innerQuery
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // snap correlation key in IQ:<from>/<filter>/<boolPred>//<rlnConj>
  //

  //   v                          //
  // F F A A A P + F F W A A A    // ]FROM
  //   f         +                //  .]filter
  //   b         +                //  ..]boolPred
  //   r         +                //  ...]rlnConj == ((pCI->Flat.m_p_conj_crlKey))
  //             +                //            $d.2 ((pCI->Flat.m_p_term_crlKey))
  //             +                //              %1 // correlation parameter

  bool bSnapped = bSnap_IQ_crlKey(pCI);
  RASSERT(bSnapped);
  //pCI->PRINTBOARD_dbg();
//  pCI->Flat.m_p_expr_crlKey->vDump(__func__);
//  pCI->Flat.m_p_conj_crlKey->vDump(__func__);
//  pCI->Flat.m_p_term_crlKey->vDump(__func__);
  RASSERT(pCI->Flat.m_p_expr_crlKey->pSymbol()->bMatches(pCI->Flat.m_p_term_crlKey->pSymbol()));


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // promote IQ:<aggreg> to be IQ:<gbyAgg>/<key=crlKey>
  //   thereby producing ordered subQuery stream

  vPromote_IQ_aggreg_To_gbyAgg_crlKey(pCI);
  //pCI->PRINTBOARD_dbg();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add correlation key to IQ:[<project>/<emit>/<crlKey>]

  vEnhance_IQ_emit_project_Add_crlKey(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // snap final IQ:<op>/<emit>

  if (CK_o_PROJECT == pCI->E.e_Op || CK_o_GBYAGG == pCI->E.e_Op)
    pCI->E.p_emit = pCI->E.p_Op->pSub_emit();
  else
    NEVER_REACH

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  RASSERT(pCI->bEachOp(CK_o_FOLD));

  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  // outerQuery outerQuery outerQuery outerQuery outerQuery outerQuery outerQuery
  // = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

  // restate the emits of the leading OQ:<from>* sequence
  pCI->vSnap_arriving(nullptr,nullptr);
  while (pCI->pPeekNextIf(CK_o_FROM) && pCI->bEachOp(CK_o_FROM)) {
    pCI->E.p_Op->vRestate_emit(pCI);
    pCI->vSnapIf_arriving();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Convert the OQ to join to the IQ gby correlation column ordered streamed from SQ

  // v                             //  ]filter
  // F G G G P + F F F W A A A     //  .]boolPred
  // s         +                   //  ..]cmpPred ==
  // f         +                   //           $d.2  ((pCI->m_pTerm_corrArg))
  //           +                   //             %1


  pCI->vClearRegisters();
  pCI->vBeginEach();
  while (pCI->bEachOp(CK_o_FROM, CK_o_FILTER, CK_PREDICATE)) {
    if (pCI->E.p_predicate->bHasSub(pCI->Flat.m_p_conj_crlKey)) {
      pCI->E.p_predicate->vRemoveSub(pCI->Flat.m_p_conj_crlKey);
      break;
    }
  }
  RASSERT(pCI->Flat.m_p_conj_crlKey->pSymbol()->Y_bHas(P_CrlKeyConj));
  // tidy up empty SQ:<table>/<filter>
  pCI->E.p_from->vCull_filter_IfEmpty(pCI->E.p_filter);

  //                   v           //  ]filter
  // F A A A P + F F F W A A A     //  .]boolPred
  // s         +       f           //  ..]cmpPred ==
  // f         +       bc          //           $d.2  ((pCI->m_pTerm_corrArg))
  //           +                   //             %1

  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  // fold fold fold fold fold fold fold fold fold fold fold fold fold fold fold
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

  RASSERT(pCI->bEachOp(CK_o_FOLD));

  // todo: this is messy and unclear

  while (pCI->bEachOp_reverse()) {
    if (pCI->E.p_Op->pSub_emit())
      break;
  }

  pCI->vSnapIf_arriving();
  RASSERT(pCI->bEachOp(CK_o_FOLD));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ???

  RASSERT(pCI->bEachOp(CK_o_WHERE, CK_PREDICATE));
  pCI->E.p_predicate->vBump_term_ordCol(SF_EXP_COL, 2);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append correlation key conjunction onto the OQ:<where>/<filter>/<boolPred>

  pCI->Flat.m_p_conj_crlKey->pSymbol()->Y_vCut(P_CrlKeyConj);
  pCI->Flat.m_p_conj_crlKey->pSymbol()->Y_vSet(P_CrlKeyTest);

  pCI->E.p_predicate->vAppend_and_rlnConj(_pCard_RlnConj(pCI->Flat.m_p_conj_crlKey));

  //                   v           //  ]filter
  // F A A A P + F F F W A A A     //  .]boolPred
  // s         +       f           //  ..]roPred <
  // f         +       bc          //           $a.5
  //           +                   //           $b.1
  //           +                   //           $Q1

  //pCI->PRINTBOARD_dbg();
  //pCI->G.p_predicate->vDump(__func__);
  vConvert_OQ_sqyPrm_Into_expCol(pCI, pCI->E.p_predicate);

  //pCI->G.p_predicate->vDump(__func__);
  vConvert_OQ_sqyNo_Into_expCol(pCI, pCI->E.p_predicate);

  //pCI->PRINTBOARD_dbg();
  //pCI->G.p_predicate->vDump(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be done using standard predicate moving

  pCI->vClearRegisters();
  pCI->vBeginEach();

  //                   v           //  ]filter
  // F A A A P + F F F W A A A     //  .]boolPred
  // s         +       f           //  ..]cmpPred ==
  // f         +       bc          //           $d.2  ((pCI->m_pTerm_corrArg))
  //           + b <-  cp          //             %1

  RASSERT(pCI->bEachOp(CK_o_FOLD));
  RASSERT(pCI->bEachOp(CK_o_WHERE, CK_PREDICATE));
  vMove_OQ_rlnConj_rslTest_To_from_filter(pCI, pCI->E.p_predicate);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->vClearRegisters();
  pCI->vBeginEach();
//  if (pCI->bEachOp(CK_o_FOLD)) {
//    if (pCI->bEachOp(CK_o_WHERE)) {
//      // TODO:
//      // R_vUTE_Unproject_foldExpr_After_where();
//      R_vUTE_Bump_where_BaseOn_foldExpr();
//    }
//  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool XF_FLATTEN::bSnap_IQ_crlKey(CardIter* pCI)
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold identify the <table>/<filter>/<boolPred>/<cmpPred> which is correlated by parameter
  bool rbSnapped = false;

  auto fbSnap_sqyArg = [pCI,&rbSnapped](Card_CONJ* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;
    if (! p_conj->bIsCorrConj())
      return C_CONTINUE_RECURSION;

    pCI->Flat.m_p_conj_crlKey = p_conj;
    pCI->Flat.m_p_conj_crlKey->vMark1(MARK_AT);
    pCI->Flat.m_p_conj_crlKey->pSymbol()->Y_vSet(P_CrlKeyConj);

    pCI->Flat.m_p_term_crlKey = static_cast<Card_RlnConj *>(p_conj)->pGet_term_crlKey();
    pCI->Flat.m_p_term_crlKey->pSymbol()->Y_vSet(P_CrlKeyTerm);

    rbSnapped = true;

    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRegisters();
  pCI->vBeginEach();

  while (pCI->bEachOp(CK_o_FROM, CK_p_EMIT, CK_o_FILTER)) {

    pCI->E.p_predicate = pCI->E.p_filter->pSub_predicate();
    Card_CONJ *p_conj_root = pCI->E.p_predicate->pRoot_conj();

    bool bIgnore = p_conj_root->bLambdaWalk_conj(fbSnap_sqyArg);
    UNUSED(bIgnore);
    if (rbSnapped)
      break;
  }

  if (! rbSnapped)
    return false;

  pCI->Flat.m_p_expr_crlKey = pCI->pBackSearch_expr_For_col(pCI->Flat.m_p_term_crlKey->pSymbol(),
                                                            pCI->E.i_Op + 1);

  pCI->Flat.m_p_expr_crlKey->vMark1(MARK_AT);
  pCI->Flat.m_p_expr_crlKey->pSymbol()->Y_vSet(P_CrlKeyDefn);

  return true;
}
/*----------------------------------------------------------------------------*/
/* promote aggreg of inner query to be a gbyAgg keyed by crlKey               */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vPromote_IQ_aggreg_To_gbyAgg_crlKey(CardIter* pCI)
{
  // promote IQ aggreg... to gbyAgg...

  pCI->vClearRegisters();

  pCI->vBeginEach();
  RASSERT(pCI->bEachOp_break(CK_o_AGGREG));
  for (i8 i_agg = 0; i_agg < 3; i_agg++) {

    pCI->E.p_Op->vCard(CK_o_GBYAGG);

    pCI->E.p_gbyAgg = _pCard_GbyAgg(pCI->E.p_Op);
    pCI->E.p_level  = pCI->E.p_gbyAgg->pSub_level();
    pCI->E.p_emit   = pCI->E.p_gbyAgg->pSub_emit();

    // -- -- -- -- -- -- -- -- -- -- --
    // key

    Card_Key::vAssemble_injCrlKey(pCI);
    pCI->E.p_gbyAgg->vInsertChildBefore(pCI->E.p_key, pCI->E.p_emit);
    // -- -- -- -- -- -- -- -- -- -- --
    // emit

    Card_Expr::vAssemble_injCrlRsl(pCI);
    pCI->E.p_emit->vPushChild(pCI->E.p_expr);
    pCI->E.p_emit->vRenumberAll_expr();

    // -- -- -- -- -- -- -- -- -- -- --

    if (pCI->bEachOp()) {
      if (! pCI->E.p_Op->bIsCard(CK_o_AGGREG)) {
        bool b = pCI->bRewindOp();
        RASSERT(b);
        break;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* enhance possible last IQ project adding crlKey to emit                     */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vEnhance_IQ_emit_project_Add_crlKey(CardIter* pCI)
{

  if (!pCI->pPeekNextIf(CK_o_PROJECT))
    return;
  RASSERT(pCI->bEachOp(CK_o_PROJECT));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold sq <project>

  RASSERT (pCI->E.p_Op->bIsCard(CK_o_PROJECT));
  RASSERT(CT_NONE != pCI->E.p_key->eCTVal());

  pCI->E.p_emit = pCI->E.p_project->pSub_emit();
  pCI->E.p_emit->vBump_expCol(pCI);// TODO: ,nBumpBy);


  // push <project>/_SQCRLKEY_
  pCI->vPushChild(pCI->E.p_emit, CK_EXPR);
  i8 i_Board = pCI->Flat.m_pSqy_inner->pBoard()->i_Board();
  pCI->E.p_expr->pSymbol()->vApplyFacet_varg(SF_NMD_COL, C_CRLKEY, i_Board);
  pCI->E.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);

  pCI->vAppendNewChild(pCI->E.p_expr, CK_TERM, Symbol(SF_EXP_COL, "$1"_Stz));
  Symbol x_Symbol_expCol1(SF_EXP_COL, 1);

  pCI->E.p_term->vApplyCast_x(pCI->E.p_key->xCast());
  pCI->E.p_expr->vApplyCast_x(pCI->E.p_key->xCast());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // supplemental

  pCI->E.p_expr = pCI->E.p_emit->pGet_expr(1);
  pCI->E.p_expr->pSymbol()->vApplyFacet_varg(SF_NMD_COL, C_SQYRSL, i_Board);



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->E.p_emit->vRenumberAll_expr();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vConvert_OQ_sqyPrm_Into_expCol(CardIter* pCI,
                                                            Card_Predicate *p_predicate)
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [pCI](Card_CONJ* p_conj_arg,
                        auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! Card_CONJ::bIsConj(p_conj_arg))
      return; /* this effectively declares resolve as void */

    if (p_conj_arg->bIsCard(CK_RLNCONJ)) {

      if (!p_conj_arg->pSymbol()->Y_bHas(P_CrlKeyTest))
        return;

      RASSERT(p_conj_arg->pSymbol()->bHasFacet("=="_Stz));

      Kid xConj(p_conj_arg);
      while (xConj.bEach()) {
        // TODO: test for: == $d.2 %1 => == $1 [TRANSLATE PARAM %1]

        if (xConj.pChild()->pSymbol()->bHasFacet(SF_SQY_PRM)) {

          Symbol xSym_expCol1(SF_EXP_COL, pCI->Flat.m_pSqy_inner->pBoard()->i_Board());
          xConj.pChild()->pSymbol()->vApplyFacets(&xSym_expCol1);

          Card_Expr* p_expr_found =
              pCI->Arriving.m_pEmitted_prior->pGet_expr((i8) 0);

          xConj.pChild()->vApplyCast_x(p_expr_found->xCast());

        }
        else
        if (xConj.pChild()->bIsCard(CK_TERM) && xConj.pChild()->pSymbol()->bHasFacet(SF_TBL_COL)) {
          Card_OuterColNm* p_outerColNm = pCI->Flat.m_pCorrelation->pSub_outerColNm();

          // todo: hack
          Card* p_card = p_outerColNm->pDupCardRecursive();
          p_card->vCard(CK_TERM);
          Card_Term* p_term = _pCard_Term(p_card);
          Symbol* pSym_term = p_term->pSymbol();

          Card_Expr* p_expr_otr = pCI->pBackSearch_expr_For_colNm(pSym_term, pCI->i_Child());
          Stz* szProjectedCol = p_expr_otr->pGet_term(0)->szFacet(SF_TBL_COL);
          xConj.pChild()->pSymbol()->vApplyFacet(SF_TBL_COL, szProjectedCol);  // TODO: hack!!
          DELETE_CARD(p_term);
        }
        else {
          NEVER_REACH
        }
      }
      return;
    }
    else
    if (p_conj_arg->bIsCard(CK_ANDCONJ) || p_conj_arg->bIsCard(CK_ORCONJ)) {
      Kid xConj(p_conj_arg);
      while (xConj.bEach()) {
        RASSERT(Card_CONJ::bIsConj(xConj.pChild()));
        vResolve((Card_CONJ*)xConj.pChild(), vResolve);
      }
    }
    else {
      NEVER_REACH;
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJ* p_conj_root = static_cast<Card_CONJ*>(p_predicate->pRoot_conj());

  vResolve(p_conj_root, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vConvert_OQ_sqyNo_Into_expCol(CardIter* pCI,
                                                           Card_Predicate *p_predicate)
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto vResolve = [pCI](Card_CONJ* p_conj_arg,
                        auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! Card_CONJ::bIsConj(p_conj_arg))
      return; /* this effectively declares resolve as void */

    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // process AND or OR

    if (p_conj_arg->bIsCard(CK_ANDCONJ) || p_conj_arg->bIsCard(CK_ORCONJ)) {
      for (Kid xConj(p_conj_arg); xConj.bEach(); /**/) {
        RASSERT(Card_CONJ::bIsConj(xConj.pChild()));
        vResolve(static_cast<Card_CONJ*>(xConj.pChild()), vResolve);
      }
      return;
    }

    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    // process <RLNCONJ>

    RASSERT(p_conj_arg->bIsCard(CK_RLNCONJ));

    if (! p_conj_arg->bHasSubFacet(SF_SQY_NO))
      return;

    RASSERT(! p_conj_arg->pSymbol()->Y_bHas(P_CrlKeyConj));
    RASSERT(p_conj_arg->bIsRelnOp());

    for (Kid xConj(p_conj_arg); xConj.bEach(); /**/) {
      Card* p_conj_child = xConj.pChild();
      if (Card_CONJ::bIsConj(p_conj_child)) {
        vResolve(static_cast<Card_CONJ*>(p_conj_child), vResolve);
        continue;
      }

      if (! p_conj_child->pSymbol()->bHasFacet(SF_SQY_NO))
        continue;

      RASSERT(pCI->m_pBench->bKnown_sqy(p_conj_child->pSymbol()));

      Card* p_prev_conj = xConj.pPrev();
      p_conj_arg->vRemoveChild(p_prev_conj);
      DELETE_CARD(p_prev_conj);

      p_conj_child->pSymbol()->vApplyFacet(SF_EXP_COL, "$2"_Stz);
      Card_Expr* p_expr_found = pCI->Arriving.m_pEmitted_prior->pGet_expr(1);
      p_conj_child->vApplyCast_x(p_expr_found->xCast());
      p_conj_arg->pSymbol()->Y_vSet(P_SqyRslTest);
    }

    return;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJ* p_conj_root = static_cast<Card_CONJ*>(p_predicate->pRoot_conj());

  vResolve(p_conj_root, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vMove_OQ_rlnConj_rslTest_To_from_filter(CardIter* pCI, Card_Predicate *p_predicate)
{
  p_predicate->vRemove_rlnConj(pCI->Flat.m_p_conj_rslTest);

  //p_rlnConj_sqyRslTest->vDump();

  Card_Term* p_term_tblCol = pCI->Flat.m_p_conj_rslTest->pGetFirstTerm_tblCol();
  Symbol* pSym_term_tblCol = p_term_tblCol->pSymbol();
  Card_OP* p_OP = pCI->pFind_from_By_tblSyn(pSym_term_tblCol, pCI->E.i_Op);

  pCI->E.p_filter = p_OP->pEnsure_filter();

  pCI->E.p_filter->pSub_predicate()->vAppend_and_rlnConj(pCI->Flat.m_p_conj_rslTest);
}

/*----------------------------------------------------------------------------*/
/* Derive correlated fold projection                                          */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vRestate_emit_fold(CardIter* pCI)
{
  pCI->vSnapIf_arriving_till(CK_o_FOLD);
  RASSERT(CK_o_FOLD == pCI->E.e_Op);

  pCI->E.p_fold->vAppendChild(pCI->Arriving.m_pEmitted_prior->pDupCardRecursive());
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
