/*============================================================================*/
/* XF_Fold.cpp -- XF_Fold                                                           */
/*============================================================================*/

#include "XF_Fold.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Fold, CN_XF_FOLD)
__aNewFunction(XF_Fold, CN_XF_FOLD)
__vDeleteFunction(XF_Fold, CN_XF_FOLD)
__aFooterFunction(XF_Fold)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Fold::XF_Fold(i8 i_depth)
: Object(CN_XF_FOLD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  m_i_depth      = i_depth;
  m_p_deck       = nullptr;

  m_p_correlation = nullptr;

  m_pSubQuery    = nullptr;
  m_p_fold       = nullptr;
  m_pOuterQuery  = nullptr;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Fold::XF_Fold(i8 i_depth, Card_Deck* p_deck)
: Object(CN_XF_FOLD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  m_i_depth      = i_depth;
  m_p_deck       = p_deck;

  m_p_correlation = nullptr;

  m_pSubQuery    = nullptr;
  m_p_fold       = nullptr;
  m_pOuterQuery  = nullptr;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Fold::~XF_Fold(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Correlation* XF_Fold::p_correlation(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  return m_p_correlation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Fold::v_correlation(Card_Correlation* p_correlation)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  m_p_correlation = p_correlation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Fold::v_deck(Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  m_p_deck = p_deck;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 XF_Fold::i_depth(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  return m_i_depth;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Fold* XF_Fold::p_fold(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  return m_p_fold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Fold::v_fold(Card_Fold* p_fold)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  m_p_fold = p_fold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
SubQuery* XF_Fold::pOuterQuery(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  return m_pOuterQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Fold::vOuterQuery(SubQuery* pOuterQuery)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  m_pOuterQuery = pOuterQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
SubQuery* XF_Fold::pSubQuery(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  return m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Fold::vSubQuery(SubQuery* pSubQuery)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  m_pSubQuery = pSubQuery;
}




/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Derive correlated fold projection                                          */
/*----------------------------------------------------------------------------*/
void XF_Fold::vDerive_Correleted_fold_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  pCI->vBeginEach();

  while (pCI->bEachMain_NONE()) {

    if (CK_c_FOLD == pCI->G.e_MainCard)
      break;

    if (Card_Emit* p_emit = pCI->G.p_MainCard->pSub_emit()) {
      //pCI->vPrintBoard(__func__,__FILE__,__LINE__);
      pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = p_emit;
    }
  }

  RASSERT(CK_c_FOLD == pCI->G.e_MainCard);

  pCI->G.p_stream = new Card_Stream;
  pCI->G.p_fold->vAppendChild(pCI->G.p_stream);
  pCI->G.p_stream->vAppendChild(pCI->Flux.m_pArriving->pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* XF_Fold a sub-query (before outer-query)                                   */
/*----------------------------------------------------------------------------*/
void XF_Fold::vFold_basic(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  vOuterQuery(pCI->pBoard()->pSubQuery());

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

  vSubQuery(pCI->pBoard()->pSubQuery());

  SubQuery* pSubQuery_save = m_pSubQuery;

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
  pCI->G.p_fold = Card_Fold::pCraft_fold(szFoldNm, m_pSubQuery->pBoard()->i_Board());
  pCI->G.pDeck->vPushChild(pCI->G.p_fold);

  v_fold(pCI->G.p_fold);

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

  RASSERT(pSubQuery_save == m_pSubQuery);

  // 7) derive the fold (SQ result) expr
  pCI->Sqy.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->bEachMain(CK_c_FOLD)) {
    pCI->Sqy.m_i_FoldCard = pCI->G.i_MainCard;
    Board::vDerive_foldExpr_H_A_NC_ro(pCI,m_pSubQuery);
    pCI->Sqy.m_pExpr_fold = pCI->G.p_expr;
  }
 // pCI->vPrintBoard(__func__,__FILE__,__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


 // pCI->vPrintBoard(__func__,__FILE__,__LINE__);
}

/*----------------------------------------------------------------------------*/
/* XF_Fold a from                                                                */
/*----------------------------------------------------------------------------*/
CarryForward* XF_Fold::pFold_from(Stz* szFoldNm, CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

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
    pCI->Sqy.m_i_FoldCard = pCI->G.i_MainCard;
    Board::vDerive_foldProjection_F(pCI);
  }

  return pCarryForward;
}


/*----------------------------------------------------------------------------*/
/* XF_Fold a non-correlated sub-query (before outer-query)                       */
/*----------------------------------------------------------------------------*/
CarryForward* XF_Fold::pFold_nc_subQuery(CardIter* pCI, Stz* szFoldNm,
                                         eCK eCardWithSQ, eQO eWhatOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  CarryForward* pCarryForward = new CarryForward;

  vOuterQuery(pCI->pBoard()->pSubQuery());

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


  vSubQuery(pCI->pBoard()->pSubQuery());
  SubQuery* pSubQuery_save = m_pSubQuery;

  // 3) get copy of sq
  pCI->Sqy.m_pCard_sqyCopy = pCI->G.pDeck->pDupSubCardsRecursive(0);

  // 4) pop back to mq
  pCI->vPopBoard();

  // 5) add <fold> (before MQ)
  pCI->G.p_fold = Card_Fold::pCraft_fold(szFoldNm, m_pSubQuery->pBoard()->i_Board());
  pCI->G.pDeck->vPushChild(pCI->G.p_fold);

  v_fold(pCI->G.p_fold);

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

  RASSERT(pSubQuery_save == m_pSubQuery);

  // 7) derive the fold (SQ result) expr
  pCI->Sqy.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->bEachMain(CK_c_FOLD)) {
    pCI->Sqy.m_i_FoldCard = pCI->G.i_MainCard;
    Board::vDerive_foldExpr_H_A_NC_ro(pCI,m_pSubQuery);
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
/* count number of tables in sub-query                                        */
/*----------------------------------------------------------------------------*/
i8 XF_Fold::nTables_In_SubQuery(CardIter* pCI, Stz* szFold, eCK eCardWithSQ)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  vOuterQuery(pCI->pBoard()->pSubQuery());

  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolPred>/<roPred>
  pCI->vClearRestart();
  if (pCI->bEachMainWithSub(eCardWithSQ, CK_BOOLPRED)) {
    if (! Board::bFindAndMarkSubQryCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz,szFold);
      NEVER_REACH
    }
  }
  // pCI->vPrintBoard(__func__,__FILE__,__LINE__);

  // 2) push to sq
  if (! pCI->bPushBoard(pCI->Sqy.m_pTerm_sqyCall->pSymbol())) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Sqy.m_pTerm_sqyCall->szFacet(SF_SQY_NO));
    NEVER_REACH
  }

  i8 nTables = Card_Table::nTables(pCI);

  // 4) pop back to mq
  pCI->vPopBoard();

  return nTables;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
