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
  pCI->vBeginEachReverse();
  if (pCI->bEachMainWithSub_reverse_foldBreak_b(eCardWithSQ, CK_BOOLPRED)) {
    if (! Board::bFindAndMarkSubQryCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz, szFoldNm);
      NEVER_REACH
    }
  }
  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // 2) push to sq
  if (! pCI->bPushBoard(pCI->Sqy.m_p_term_sqyCall->pSymbol())) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Sqy.m_p_term_sqyCall->szFacet(SF_SQY_NO));
    NEVER_REACH
  }

  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  SubQuery* pSubQuery_save = pCI->pBoard()->pSubQuery();

  i8 i_dup_from = 0;
  pCI->vBeginEach();
  if (pCI->bEachMain(CK_o_CORRELATION)) {
    pCI->Sqy.m_pCorrelation = pCI->G.pDeck->pSub_correlation();
    i_dup_from = 1;
  }




  // 3) get copy of sq
  pCI->Sqy.m_pCard_sqyCopy = pCI->G.pDeck->pDupSubCardsRecursive(i_dup_from);

  // 4) pop back to mq
  pCI->vPopBoard();

  // 5) add <fold> (before MQ)
  pCI->G.p_fold = Card_Fold::pCraft_fold(szFoldNm, pSubQuery_save->pBoard()->i_Board());
  pCI->G.pDeck->vPushChild(pCI->G.p_fold);

  // 6) copy up sq (add before fold)
  pCI->G.pDeck->vPushChild(pCI->Sqy.m_pCard_sqyCopy);
  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->Sqy.m_pCard_sqyCopy);
  DELETE_CARD(pCI->Sqy.m_pCard_sqyCopy);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRestart();
  while (pCI->bEachMain_NONE()) {

    if (pCI->G.p_MainCard->pSub_emit())
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving =
      pCI->G.p_emit = pCI->G.p_MainCard->pSub_emit();

    if (pCI->bIsNextMain(CK_c_FOLD))
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 7) derive the fold (SQ result) expr
  pCI->Sqy.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->bEachMain(CK_c_FOLD)) {
    pCI->Sqy.m_i_fold = pCI->G.i_MainCard;
    pCI->G.p_fold->vDerive_emit_fold(pCI, pSubQuery_save);
    pCI->Sqy.m_pExpr_fold = pCI->G.p_expr;
  }
  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);
}

/*----------------------------------------------------------------------------*/
/* XF_Fold a from                                                                */
/*----------------------------------------------------------------------------*/
CarryForward* XF_FLATTEN::pFold_from(Stz* szFoldNm, CardIter* pCI)
{
  CarryForward* pCarryForward = new CarryForward;

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolPred>

  RASSERT(pCI->bEachMain(CK_o_CALL));

  Board* pBoard = pCI->m_pBench->pFindBoard(pCI->G.p_call->pSymbol());

  pCI->G.p_fold = Card_Fold::pCraft_fold(szFoldNm, pBoard->i_Board());
  pCI->G.pDeck->vPushChild(pCI->G.p_fold);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! pCI->bPushBoard(pCI->G.p_call->pSymbol())) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->G.p_call->szFacet(SF_SQY_NO));
    NEVER_REACH
  }

  pCI->Sqy.m_pCard_sqyCopy = pCI->G.pDeck->pDupSubCardsRecursive(0);

  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  pCI->vPopBoard();

  pCI->G.pDeck->vPushChild(pCI->Sqy.m_pCard_sqyCopy);
  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->Sqy.m_pCard_sqyCopy);
  DELETE_CARD(pCI->Sqy.m_pCard_sqyCopy);

  pCI->G.pDeck->vRemoveChild(pCI->G.p_call);
  DELETE_CARD(pCI->G.p_call);

  // 7) derive the fold (SQ result) expr
  pCI->vClearRestart();
  if (pCI->bEachMain(CK_c_FOLD)) {
    pCI->Sqy.m_i_fold = pCI->G.i_MainCard;
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
  if (pCI->bEachMainWithSub(eCardWithSQ, CK_BOOLPRED)) {
    if (! Board::bFindAndMarkSubQryCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz, szFoldNm);
      NEVER_REACH
    }
  }
  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // 2) push to sq
  if (! pCI->bPushBoard(pCI->Sqy.m_p_term_sqyCall->pSymbol())) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Sqy.m_p_term_sqyCall->szFacet(SF_SQY_NO));
    NEVER_REACH
  }


  SubQuery* pSubQuery_save = pCI->pBoard()->pSubQuery();

  // 3) get copy of sq
  pCI->Sqy.m_pCard_sqyCopy = pCI->G.pDeck->pDupSubCardsRecursive(0);

  // 4) pop back to mq
  pCI->vPopBoard();

  // 5) add <fold> (before MQ)
  pCI->G.p_fold = Card_Fold::pCraft_fold(szFoldNm, pSubQuery_save->pBoard()->i_Board());
  pCI->G.pDeck->vPushChild(pCI->G.p_fold);

  // 6) copy up sq (add before fold)
  pCI->G.pDeck->vPushChild(pCI->Sqy.m_pCard_sqyCopy);
  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->Sqy.m_pCard_sqyCopy);
  DELETE_CARD(pCI->Sqy.m_pCard_sqyCopy);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRestart();
  while (pCI->bEachMain_NONE()) {

    if (pCI->G.p_MainCard->pSub_emit())
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving =
      pCI->G.p_emit = pCI->G.p_MainCard->pSub_emit();

    if (pCI->bIsNextMain(CK_c_FOLD))
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  // 7) derive the fold (SQ result) expr
  pCI->Sqy.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->bEachMain(CK_c_FOLD)) {
    pCI->Sqy.m_i_fold = pCI->G.i_MainCard;
    pCI->G.p_fold->vDerive_emit_fold(pCI, pSubQuery_save);
    pCI->Sqy.m_pExpr_fold = pCI->G.p_expr;
  }
  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);


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

  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

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


  pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  pCI->vBeginEach();
  RASSERT(pCI->bEachMain(CK_c_FOLD));
  RASSERT(pCI->G.p_MainCard->bIsCard(CK_c_FOLD));
  //RASSERT(pCI->m_pExpr_fold);

  if(pCI->bIsNextMain(CK_SUBQUERY))
    RASSERT(pCI->pAdvanceChild());

  // Table*
  while (pCI->bIsNextMain(CK_o_FROM) && pCI->bEachMain(CK_o_FROM)) {
    if (pCarryForward->Table.m_bSeen) {
      pCI->G.p_table = pCI->G.p_from->pSub_table();
      pCI->G.p_OP->vRecreate_emit(pCI);
      pCI->vPrintBoard(__func__,__FILE__,__LINE__);
    }
  }



  // [Where]
  if (pCI->bIsNextMain(CK_o_WHERE) && pCI->bEachMain(CK_o_WHERE)) {
    if (pCarryForward->Where_preAgg.m_bSeen) {
      //pCI->Flux.m_pArriving->vDump(__func__,"Smooth where"_Stz);
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving;
      pCI->G.p_where->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__func__,__FILE__,__LINE__);
    }
  }


  // GbyAgg_partition
  if (pCI->bIsNextMain(CK_o_GBYAGG) && pCI->bEachMain(CK_o_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__func__,__FILE__,__LINE__);
    }
  }

  // GbyAgg_node
  if (pCI->bIsNextMain(CK_o_GBYAGG) && pCI->bEachMain(CK_o_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__func__,__FILE__,__LINE__);
    }
  }

  // GbyAgg_origin
  if (pCI->bIsNextMain(CK_o_GBYAGG) && pCI->bEachMain(CK_o_GBYAGG)) {
    if (pCarryForward->GbyAgg.m_bSeen) {
      pCI->G.p_gbyAgg->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__func__,__FILE__,__LINE__);

    }
    pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = pCI->G.p_gbyAgg->pSub_emit();
  }

  // [project (avg)]
  if (pCI->bIsNextMain(CK_o_PROJECT) && pCI->bEachMain(CK_o_PROJECT)) {
    if (pCarryForward->Project_postAgg.m_bSeen) {
      pCI->G.p_project->vSmooth_postAgg(pCI, pCarryForward);
      pCI->vPrintBoard(__func__,__FILE__,__LINE__);
    }
  }

  // Having
  if (pCI->bIsNextMain(CK_o_HAVING) && pCI->bEachMain(CK_o_HAVING)) {
    if (pCarryForward->Having.m_bSeen) {
      pCI->G.p_having->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__func__,__FILE__,__LINE__);
    }
  }

  // Sort
  if (pCI->bIsNextMain(CK_o_SORT) && pCI->bEachMain(CK_o_SORT)) {
    if (pCarryForward->Sort.m_bSeen) {
      pCI->G.p_sort->vSmooth(pCI, pCarryForward);
      pCI->vPrintBoard(__func__,__FILE__,__LINE__);
    }
  }

  //  --  --  --  --  --  --  --  --  --  --  --  --  --  --

  if (pCarryForward->Project_postAgg.m_bSeen &&  pCI->G.p_having) {
    pCI->G.p_project->vInject_foldExpr(pCI);
    pCI->G.p_having->vAcclimate_foldExpr(pCI);
    pCI->G.p_emit = pCI->G.p_project->pSub_emit();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
    pCI->vPrintBoard(__func__,__FILE__,__LINE__);
  }
  else
  if (pCarryForward->Unproject.m_bUnproject_foldExpr) {
    pCI->G.p_emit = pCI->G.p_gbyAgg->pSub_emit();
    Card_Project::vUnproject_foldExpr(pCI, pCarryForward);
    pCI->vPrintBoard(__func__,__FILE__,__LINE__);
    vDebugPoint();
  }

  pCI->vPrintBoard(__func__,__FILE__,__LINE__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_FLATTEN::vInto_KEYnotEXISTS(CardIter* pCI)
{
  pCI->vClearRestart();
  RASSERT(pCI->bEachMain_foldBreak(CK_o_FROM));


  //RASSERT(pCI->bEachMain(CK_o_WHERE));
  RASSERT(pCI->bEachMain_foldBreak(CK_o_PROJECT));

//  pCI->G.p_constraint = pCI->G.p_where->pSub_constraint();
//  pCI->G.p_where->vRemoveSub(pCI->G.p_constraint);
//
//  //pCI->G.p_filter = Card_Filter::pNew(p_constraint);
//  pCI->G.p_table->vAppendChild(pCI->G.p_constraint);

  Card_From* p_from_exists = pCI->G.p_from;
  pCI->G.pDeck->vRemove(p_from_exists);

  pCI->G.p_term = pCI->G.p_project->pSub_term();
  pCI->G.p_project->vRemoveSub(pCI->G.p_term);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  pCI->G.pDeck->vDeleteChild(pCI->G.p_where);
  pCI->G.pDeck->vDeleteChild(pCI->G.p_project);

  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vBeginEachReverse();
  RASSERT(pCI->bEachMain_reverse_foldBreak_b(CK_o_WHERE));

  RASSERT(pCI->bEachMain_reverse_foldBreak_b(CK_o_FROM));
  Card_Emit* p_emitted_table = pCI->G.p_from->pSub_emit();

  pCI->vBeginEachReverse();
  RASSERT(pCI->bEachMain_reverse_foldBreak_b(CK_o_WHERE));


  pCI->G.pDeck->vInsertChildBefore(p_from_exists,pCI->G.p_where);

  pCI->G.p_boolPred = pCI->G.p_where->pSub_boolPred();
  pCI->G.p_rlnConj = pCI->G.p_boolPred->pFind_rlnConj_With_sqyCall();

  if (pCI->G.p_rlnConj->pSymbol()->bHasFacet("in"_Stz) ||
      pCI->G.p_rlnConj->pSymbol()->bHasFacet("exists"_Stz))
    p_from_exists->vAppendChild(new Card_KeyExists);
  else
  if (pCI->G.p_rlnConj->pSymbol()->bHasFacet("notIn"_Stz) ||
      pCI->G.p_rlnConj->pSymbol()->bHasFacet("notExists"_Stz))
    p_from_exists->vAppendChild(new Card_KeyNotExists);

  pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = p_emitted_table;
  pCI->G.p_rlnConj->vMorph_sqlOp_To_EQ(pCI, pCI->G.p_term);

  DELETE_CARD(pCI->G.p_term);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (! pCI->Sqy.m_pCorrelation) {
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


  Card_Term* p_term_sqyArg = pCI->Sqy.m_p_term_sqyPrm->pClone();
  p_term_sqyArg->vApplyCast_p(p_term_found);
  //p_term_sqyArg->vDump();


  Symbol xSymbol_prmCol(SF_SQY_PRM, 1);
  p_from_exists->bUpdateSymbolRecursive(CK_TERM, &xSymbol_prmCol, p_term_sqyArg->pSymbol());

  delete p_term_sqyArg;

  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  pCI->vClearRestart();
  if (pCI->bEachMainWithSub(CK_o_WHERE, CK_BOOLPRED)) {
    (void)pCI->G.p_boolPred->nCullMarked_rlnConj(MARK_AT);
  }

  pCI->vClearRestart();
  pCI->vDeleteWhenEmptySub(CK_o_WHERE, CK_BOOLPRED);

  vDebugPoint();

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
