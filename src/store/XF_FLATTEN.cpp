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
void XF_FLATTEN::vFold_basic(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ)
{
  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolPred>/<roPred>
  pCI->vBeginEach_reverse();
  if (pCI->bEachOp_reverse_foldBreak(eCardWithSQ, CK_BOOLPRED)) {
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

  pCI->vSaveMembers();
  Board* pBoard_outer = pCI->pBoard();
  pCI->vUseBoard(pBoard_inner);

  // pCI->PRINTBOARD_dbg();

  i8 i_dup_from = 0;
  pCI->vBeginEach();
  if (pCI->bEachOp(CK_o_CORRELATION)) {
    pCI->Flat.m_pCorrelation = pCI->E.p_deck->pSub_correlation();
    i_dup_from = 1;
  }


  // 3) get copy of this inner query
  pCI->Flat.m_pCopy_inner = pCI->E.p_deck->pDupSubCardsRecursive(i_dup_from);

  // 4) pop back to outer query
  pCI->vUseBoard(pBoard_outer);
  pCI->vRestoreMembers();

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

    if (pCI->E.p_Op->pSub_emit())
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving =
      pCI->E.p_emit = pCI->E.p_Op->pSub_emit();

    if (pCI->pPeekNextIf(CK_c_FOLD))
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 7) derive the fold (SQ result) expr
  pCI->Flat.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->bEachOp(CK_c_FOLD)) {
    pCI->Flat.m_i_fold = pCI->E.i_Op;
    pCI->E.p_fold->vDerive_emit_fold(pCI, pBoard_inner->pSubQuery());
    pCI->Flat.m_pExpr_fold = pCI->E.p_expr;
  }
  // pCI->PRINTBOARD_dbg();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->E.pBoard->vRenumberCards();

  // pCI->PRINTBOARD_dbg();
}

/*----------------------------------------------------------------------------*/
/* XF_Fold a from                                                             */
/*----------------------------------------------------------------------------*/
CarryForward* XF_FLATTEN::pFold_from(Stz* szFoldNm, CardIter* pCI)
{
  CarryForward* pCarryForward = new CarryForward;

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

  pCI->vSaveMembers();
  pCI->vUseBoard(pBoard);

  pCI->Flat.m_pCopy_inner = pCI->E.p_deck->pDupSubCardsRecursive(0);

  // pCI->PRINTBOARD_dbg();

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  //pCI->vPopBoard();
  pCI->vRestoreMembers();
  pCI->E.p_deck->vPushChild(pCI->Flat.m_pCopy_inner);
  pCI->E.p_deck->vRemoveChildLeavingSubs(pCI->Flat.m_pCopy_inner);
  DELETE_CARD(pCI->Flat.m_pCopy_inner);

  pCI->E.p_deck->vRemoveChild(pCI->E.p_call);
  DELETE_CARD(pCI->E.p_call);

  // 7) derive the fold (SQ result) expr
  pCI->vClearRegisters();
  pCI->vBeginEach();
  if (pCI->bEachOp(CK_c_FOLD)) {
    pCI->Flat.m_i_fold = pCI->E.i_Op;
    Board::vDerive_foldProjection_F(pCI);
  }

  return pCarryForward;
}


/*----------------------------------------------------------------------------*/
/* XF_Fold a non-correlated sub-query (before outer-query)                       */
/*----------------------------------------------------------------------------*/
CarryForward* XF_FLATTEN::pFold_nc_subQuery(CardIter* pCI, Stz* szFoldNm,
                                            eCK eCardWithSQ, eQO eWhatOp)
{
  CarryForward* pCarryForward = new CarryForward;

  pCI->vBeginEach();

  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolPred>/<roPred>
  if (pCI->bEachOp(eCardWithSQ, CK_BOOLPRED)) {
    if (!Board::bMark_sqyCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz, szFoldNm);
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
  pCI->vSaveMembers();
  pCI->vUseBoard(pBoard);


  SubQuery* pSubQuery_save = pCI->pBoard()->pSubQuery();

  // 3) get copy of sq
  pCI->Flat.m_pCopy_inner = pCI->E.p_deck->pDupSubCardsRecursive(0);

  // 4) pop back to mq
  //pCI->vPopBoard();
  pCI->vRestoreMembers();

  // 5) add <fold> (before MQ)
  pCI->E.p_fold = Card_Fold::pCraft_fold(szFoldNm, pSubQuery_save->pBoard()->i_Board());
  pCI->E.p_deck->vPushChild(pCI->E.p_fold);

  // 6) copy up sq (add before fold)
  pCI->E.p_deck->vPushChild(pCI->Flat.m_pCopy_inner);
  pCI->E.p_deck->vRemoveChildLeavingSubs(pCI->Flat.m_pCopy_inner);
  DELETE_CARD(pCI->Flat.m_pCopy_inner);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRegisters();
  pCI->vBeginEach();
  while (pCI->bEachOp()) {

    if (pCI->E.p_Op->pSub_emit())
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving =
      pCI->E.p_emit = pCI->E.p_Op->pSub_emit();

    if (pCI->pPeekNextIf(CK_c_FOLD))
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  // 7) derive the fold (SQ result) expr
  pCI->Flat.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->bEachOp(CK_c_FOLD)) {
    pCI->Flat.m_i_fold = pCI->E.i_Op;
    pCI->E.p_fold->vDerive_emit_fold(pCI, pSubQuery_save);
    pCI->Flat.m_pExpr_fold = pCI->E.p_expr;
  }
  // pCI->PRINTBOARD_dbg();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCarryForward->vSetSeen(pCI);

  if (CK_o_HAVING == eCardWithSQ) {
    pCarryForward->Table.m_bReproject = true;
    pCarryForward->GbyAgg.m_bInject_foldExpr = true;
    pCarryForward->Having.m_bCorrect_After_Inject = true;
    pCarryForward->Unproject.m_bUnproject_foldExpr = true;
  }
  else {
    pCarryForward->Table.m_bReproject = true;
    if (QO_IN_OP == eWhatOp || QO_NOT_IN_OP == eWhatOp)
      pCarryForward->Where_preAgg.m_bMorph_sqlOp_To_EQ = true;
    else
    if (QO_RELN_OP == eWhatOp)
      pCarryForward->Where_preAgg.bReplace_sqCall_With_result = true;
  }

  if (pCarryForward->Sort.m_bSeen)
    pCarryForward->Sort.m_bCorrect_After_Inject = true;

  // pCI->PRINTBOARD_dbg();

  return pCarryForward;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vSmooth(CardIter* pCI, CarryForward* pCarryForward, bool bEarlyExit)
{
  // todo: REMOVE
  if (bEarlyExit)
    return;


  //pCI->PRINTBOARD_dbg();

  pCI->vBeginEach();
  RASSERT(pCI->bEachOp(CK_c_FOLD));
  RASSERT(pCI->E.p_Op->bIsCard(CK_c_FOLD));
  //RASSERT(pCI->m_pExpr_fold);

  if(pCI->pPeekNextIf(CK_SUBQUERY))
    RASSERT(pCI->pAdvanceChild());

  // Table*
  while (pCI->pPeekNextIf(CK_o_FROM) && pCI->bEachOp(CK_o_FROM)) {
    if (pCarryForward->Table.m_bSeen) {
      pCI->E.p_table = pCI->E.p_from->pSub_table();
      ((Card_OP*)pCI->E.p_Op)->vRestate_emit(pCI);
      //pCI->PRINTBOARD_dbg();
    }
  }



  // [Where]
  if (pCI->pPeekNextIf(CK_o_WHERE) && pCI->bEachOp(CK_o_WHERE)) {
    if (pCarryForward->Where_preAgg.m_bSeen) {
      //pCI->Flux.m_pArriving->vDump(__func__,"Smooth where"_Stz);
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving;
      pCI->E.p_where->vSmooth(pCI, pCarryForward);
      //pCI->PRINTBOARD_dbg();
    }
  }


  // GbyAgg_partition
  if (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->E.p_gbyAgg->vSmooth(pCI, pCarryForward);
      //pCI->PRINTBOARD_dbg();
    }
  }

  // GbyAgg_node
  if (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->E.p_gbyAgg->vSmooth(pCI, pCarryForward);
      //pCI->PRINTBOARD_dbg();
    }
  }

  // GbyAgg_origin
  if (pCI->pPeekNextIf(CK_o_GBYAGG) && pCI->bEachOp(CK_o_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->E.p_gbyAgg->vSmooth(pCI, pCarryForward);
      //pCI->PRINTBOARD_dbg();

    }
    pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = pCI->E.p_gbyAgg->pSub_emit();
  }

  // [project (avg)]
  if (pCI->pPeekNextIf(CK_o_PROJECT) && pCI->bEachOp(CK_o_PROJECT)) {
    if (pCarryForward->Project_postAgg.m_bSeen) {
      pCI->E.p_project->vSmooth_postAgg(pCI, pCarryForward);
      //pCI->PRINTBOARD_dbg();
    }
  }

  // Having
  if (pCI->pPeekNextIf(CK_o_HAVING) && pCI->bEachOp(CK_o_HAVING)) {
    if (pCarryForward->Having.m_bSeen) {
      pCI->E.p_having->vSmooth(pCI, pCarryForward);
      //pCI->PRINTBOARD_dbg();
    }
  }

  // Sort
  if (pCI->pPeekNextIf(CK_o_SORT) && pCI->bEachOp(CK_o_SORT)) {
    if (pCarryForward->Sort.m_bSeen) {
      pCI->E.p_sort->vSmooth(pCI, pCarryForward);
      //pCI->PRINTBOARD_dbg();
    }
  }

  //  --  --  --  --  --  --  --  --  --  --  --  --  --  --

  if (pCarryForward->Project_postAgg.m_bSeen &&  pCI->E.p_having) {
    pCI->E.p_project->vInject_foldExpr(pCI);
    pCI->E.p_having->vAcclimate_foldExpr(pCI);
    pCI->E.p_emit = pCI->E.p_project->pSub_emit();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
    //pCI->PRINTBOARD_dbg();
  }
  else
  if (pCarryForward->Unproject.m_bUnproject_foldExpr) {
    pCI->E.p_emit = pCI->E.p_gbyAgg->pSub_emit();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
    //pCI->PRINTBOARD_dbg();
    vDebugPoint();
  }

  //pCI->PRINTBOARD_dbg();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vInto_KEYnotEXISTS(CardIter* pCI)
{
  pCI->vClearRegisters();
  pCI->vBeginEach();
  RASSERT(pCI->bEachOp_foldBreak(CK_o_FROM));


  //RASSERT(pCI->bEachOp(CK_o_WHERE));
  RASSERT(pCI->bEachOp_foldBreak(CK_o_PROJECT));

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

  pCI->vBeginEach_reverse();
  RASSERT(pCI->bEachOp_reverse_foldBreak(CK_o_WHERE));

  RASSERT(pCI->bEachOp_reverse_foldBreak(CK_o_FROM));
  Card_Emit* p_emitted_table = pCI->E.p_from->pSub_emit();

  pCI->vBeginEach_reverse();
  RASSERT(pCI->bEachOp_reverse_foldBreak(CK_o_WHERE));


  pCI->E.p_deck->vInsertChildBefore(p_from_exists, pCI->E.p_where);

  pCI->E.p_boolPred = pCI->E.p_where->pSub_boolPred();
  pCI->E.p_rlnConj  = _pCard_RlnConj(pCI->E.p_boolPred->pFindCard_WithSubFacet(CK_RLNCONJ, SF_SQY_NO));

  if (pCI->E.p_rlnConj->pSymbol()->bHasFacet("in"_Stz) ||
      pCI->E.p_rlnConj->pSymbol()->bHasFacet("exists"_Stz))
    p_from_exists->vAppendChild(new Card_KeyExists);
  else
  if (pCI->E.p_rlnConj->pSymbol()->bHasFacet("notIn"_Stz) ||
      pCI->E.p_rlnConj->pSymbol()->bHasFacet("notExists"_Stz))
    p_from_exists->vAppendChild(new Card_KeyNotExists);

  pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = p_emitted_table;
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
  if (pCI->bEachOp(CK_o_WHERE, CK_BOOLPRED)) {
    (void)pCI->E.p_boolPred->nCullMarked_rlnConj(MARK_AT);
  }

  pCI->vClearRegisters();
  pCI->vBeginEach();
  pCI->vDeleteWhenEmptySub(CK_o_WHERE, CK_BOOLPRED);

  vDebugPoint();

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
