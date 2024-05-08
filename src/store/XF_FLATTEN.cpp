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
  m_eFlatten = QF_NONE;
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
/* set carry                                                                  */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vSet_carry(CardIter* pCI, eCK eCardWithSQ, eQO eWhatOp)
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vBeginEach();

  RASSERT(pCI->bEachOp(CK_o_FOLD));
  RASSERT(pCI->E.p_Op->bIsCard(CK_o_FOLD));

  // SubQuery
  if (pCI->pPeekNextIf(CK_SUBQUERY))
    RASSERT(pCI->bEachOp());

  // From*
  while (pCI->pPeekNextIf(CK_o_FROM) && pCI->bEachOp(CK_o_FROM)) {
    pCI->Carry.Table.m_bSeen = true;
    pCI->Carry.Table.m_nTables++;
  }

  // [Where]
  if (pCI->pPeekNextIf(CK_o_WHERE) && pCI->bEachOp(CK_o_WHERE)) {
    pCI->Carry.Where_preAgg.m_bSeen = true;
  }

  // Aggreg
  while (pCI->pPeekNextIf(CK_o_AGGREG) && pCI->bEachOp(CK_o_AGGREG)) {
    pCI->Carry.GbyAgg.m_bSeen = true;
  }
  // GbyAgg
  while (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
    pCI->Carry.GbyAgg.m_bSeen = true;
  }

  // [project]
  if (pCI->pPeekNextIf(CK_o_PROJECT) && pCI->bEachOp(CK_o_PROJECT)) {
    pCI->Carry.Project_postAgg.m_bSeen = true;
  }

  // Having
  if (pCI->pPeekNextIf(CK_o_HAVING) && pCI->bEachOp(CK_o_HAVING)) {
    pCI->Carry.Having.m_bSeen = true;
  }

  // Sort
  if (pCI->pPeekNextIf(CK_o_SORT) && pCI->bEachOp(CK_o_SORT)) {
    pCI->Carry.Sort.m_bSeen = true;
  }

  if (CK_o_HAVING == eCardWithSQ) {
    pCI->Carry.GbyAgg.m_bInject_foldExpr = true;
    pCI->Carry.Having.m_bCorrect_After_Inject = true;
    pCI->Carry.Unproject.m_bUnproject_foldExpr = true;
  }
  else {
    if (QO_IN_OP == eWhatOp || QO_NOT_IN_OP == eWhatOp)
      pCI->Carry.Where_preAgg.m_bMorph_sqlOp_To_EQ = true;
    else
    if (QO_RELN_OP == eWhatOp)
      pCI->Carry.Where_preAgg.bReplace_sqCall_With_result = true;
  }

  if (pCI->Carry.Sort.m_bSeen)
    pCI->Carry.Sort.m_bCorrect_After_Inject = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vSmooth(CardIter* pCI)
{
  //pCI->PRINTBOARD_dbg();

  pCI->vBeginEach();
  RASSERT(pCI->bEachOp(CK_o_FOLD));
  RASSERT(pCI->E.p_Op->bIsCard(CK_o_FOLD));
  //RASSERT(pCI->m_pExpr_fold);

  if (pCI->pPeekNextIf(CK_SUBQUERY))
    RASSERT(pCI->bEachOp());

  // Table*
  while (pCI->pPeekNextIf(CK_o_FROM) && pCI->bEachOp(CK_o_FROM)) {
    if (pCI->Carry.Table.m_bSeen) {
      pCI->E.p_table = pCI->E.p_from->pSub_table();
      pCI->E.p_Op->vRestate_emit(pCI);
      pCI->vSnapIf_arriving();
      //pCI->PRINTBOARD_dbg();
    }
  }



  // [Where]
  if (pCI->pPeekNextIf(CK_o_WHERE) && pCI->bEachOp(CK_o_WHERE)) {
    if (pCI->Carry.Where_preAgg.m_bSeen) {
      //pCI->Arriving.m_pEmitted_prior->vDump(__func__,"Smooth where"_Stz);
      pCI->Dic.m_p_expr = pCI->Arriving.m_pEmitted_prior;
      pCI->E.p_where->vSmooth(pCI);
      //pCI->PRINTBOARD_dbg();
    }
  }


  // GbyAgg_partition
  if (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
    if (pCI->Carry.GbyAgg.m_bSeen) {
      pCI->E.p_gbyAgg->vSmooth(pCI);
      //pCI->PRINTBOARD_dbg();
    }
  }

  // GbyAgg_node
  if (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
    if (pCI->Carry.GbyAgg.m_bSeen) {
      pCI->E.p_gbyAgg->vSmooth(pCI);
      //pCI->PRINTBOARD_dbg();
    }
  }

  // GbyAgg_origin
  if (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
    if (pCI->Carry.GbyAgg.m_bSeen) {
      pCI->E.p_gbyAgg->vSmooth(pCI);
      //pCI->PRINTBOARD_dbg();

    }
    pCI->vSnapIf_arriving();
  }

  // [project (avg)]
  if (pCI->pPeekNextIf(CK_o_PROJECT) && pCI->bEachOp(CK_o_PROJECT)) {
    if (pCI->Carry.Project_postAgg.m_bSeen) {
      pCI->E.p_project->vSmooth_postAgg(pCI);
      //pCI->PRINTBOARD_dbg();
    }
  }

  // Having
  if (pCI->pPeekNextIf(CK_o_HAVING) && pCI->bEachOp(CK_o_HAVING)) {
    if (pCI->Carry.Having.m_bSeen) {
      pCI->E.p_having->vSmooth(pCI);
      //pCI->PRINTBOARD_dbg();
    }
  }

  // Sort
  if (pCI->pPeekNextIf(CK_o_SORT) && pCI->bEachOp(CK_o_SORT)) {
    if (pCI->Carry.Sort.m_bSeen) {
      pCI->E.p_sort->vSmooth(pCI);
      //pCI->PRINTBOARD_dbg();
    }
  }

  //  --  --  --  --  --  --  --  --  --  --  --  --  --  --

  if (pCI->Carry.Unproject.m_bUnproject_foldExpr) {
    pCI->E.p_emit = pCI->E.p_gbyAgg->pSub_emit();
    Card_Project::vUnproject_foldExpr(pCI);
    //pCI->PRINTBOARD_dbg();
    vDebugPoint();
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
  pCI->vPushEach();
  pCI->vUseBoard(pBoard);

  i8 nTables = Card_Table::nTables(pCI);

  // 4) pop back to mq
  pCI->vPopEach();

  return nTables;
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
