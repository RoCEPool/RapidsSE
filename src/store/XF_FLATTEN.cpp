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
  if (! pCI->bPushBoard(pCI->Sqy.m_pTerm_sqyCall->pSymbol())) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Sqy.m_pTerm_sqyCall->szFacet(SF_SQY_NO));
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
  if (! pCI->bPushBoard(pCI->Sqy.m_pTerm_sqyCall->pSymbol())) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Sqy.m_pTerm_sqyCall->szFacet(SF_SQY_NO));
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
      pCI->G.p_from->vSmooth(pCI, pCarryForward);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
