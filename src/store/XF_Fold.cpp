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

  while (pCI->G_bEachMain_NONE()) {

    if (CK_FOLD == pCI->G.e_MainCard)
      break;

    if (Card_Emit* p_emit = pCI->G.p_MainCard->pSub_emit()) {
      //pCI->vPrintBoard(__LINE__);
      pCI->Flux.m_pArriving = p_emit;
    }
  }

  RASSERT(CK_FOLD == pCI->G.e_MainCard);

  pCI->G.p_stream = new Card_Stream;
  pCI->G.p_fold->vAppendChild(pCI->G.p_stream);
  pCI->G.p_stream->vAppendChild(pCI->Flux.m_pArriving->pDupCardRecursive());

 // pCI->vRenumberCards();

}

/*----------------------------------------------------------------------------*/
/* XF_Fold a non-correlated sub-query (before outer-query)                       */
/*----------------------------------------------------------------------------*/
void XF_Fold::vFold_basic(CardIter* pCI, Stz* szFold, eCK eCardWithSQ)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  vOuterQuery(pCI->pBoard()->pSubQuery());

  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolPred>/<roPred>
  pCI->vBeginEachReverse();
  if (pCI->G_bEachMainWithSub_reverse_foldBreak_b(eCardWithSQ, CK_BOOLPRED)) {
    if (! Board::bFindAndMarkSubQryCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz,szFold);
      NEVER_REACH
    }
  }
 // pCI->vPrintBoard(__LINE__);

  // 2) push to sq
  if (! pCI->G_bPushBoard(pCI->Sqy.m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Sqy.m_sSqyNm.szStz());
    NEVER_REACH
  }

 // pCI->vPrintBoard(__LINE__);

  vSubQuery(pCI->pBoard()->pSubQuery());


  i8 i_dup_from = 0;
  pCI->vBeginEach();
  if (pCI->G_bEachMain(CK_CORRELATION)) {
    pCI->Sqy.m_pCorrelation = pCI->G.pDeck->pSub_correlation();
    i_dup_from = 1;
  }




  // 3) get copy of sq
  pCI->Sqy.m_pCard_sqCopy = pCI->G.pDeck->pDupSubCardsRecursive(i_dup_from);

  // 4) pop back to mq
  pCI->G_vPopIter();

  // 5) add <fold> (before MQ)
  pCI->G_vNewMember(CK_FOLD, Symbol(SF_FLD_NM,szFold));
  pCI->G.pDeck->vPushChild(pCI->G.p_fold);
  pCI->G.p_level = new Card_Level(AL_NONE);
  pCI->G.p_fold->vAppendChild(pCI->G.p_level);

  v_fold(pCI->G.p_fold);


  // 6) copy up sq (add before fold)
  pCI->G.pDeck->vPushChild(pCI->Sqy.m_pCard_sqCopy);
  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->Sqy.m_pCard_sqCopy);
  DELETE_CARD(pCI->Sqy.m_pCard_sqCopy);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRestart();
  while (pCI->G_bEachMain_NONE()) {

    if (pCI->G.p_MainCard->pSub_emit())
      pCI->Flux.m_pArriving =
          pCI->G.p_emit = pCI->G.p_MainCard->pSub_emit();

    if (pCI->G_bIsNextMain(CK_FOLD))
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 7) derive the fold (SQ result) expr
  pCI->Sqy.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->G_bEachMain(CK_FOLD)) {
    pCI->Sqy.m_i_FoldCard = pCI->G.i_MainCard;
    Board::vDerive_foldExpr_H_A_NC_ro(pCI);
    pCI->Sqy.m_pExpr_fold = pCI->G.p_expr;
  }
 // pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


 // pCI->vPrintBoard(__LINE__);
}

/*----------------------------------------------------------------------------*/
/* XF_Fold a from                                                                */
/*----------------------------------------------------------------------------*/
CarryForward* XF_Fold::pFold_from(Stz* szFold, CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);

  CarryForward* pCarryForward = new CarryForward;

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolPred>

  RASSERT(pCI->G_bEachMain(CK_CALL));
  //vTrace(">>>>>>>>> Found From CALl: %s\n"_Stz, pCI->G.p_call->szFacet(SF_NONE));
 // pCI->vPrintBoard(__LINE__);

  pCI->G_vNewMember(CK_FOLD, Symbol(SF_FLD_NM,szFold));
  pCI->G.pDeck->vPushChild(pCI->G.p_fold);
  pCI->G.p_level = new Card_Level(AL_NONE);
  pCI->G.p_fold->vAppendChild(pCI->G.p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! pCI->G_bPushBoard(pCI->G.p_call->szFacet(SF_NONE))) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->G.p_call->szFacet(SF_NONE));
    NEVER_REACH
  }

  pCI->Sqy.m_pCard_sqCopy = pCI->G.pDeck->pDupSubCardsRecursive(0);

 // pCI->vPrintBoard(__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  pCI->G_vPopIter();

  pCI->G.pDeck->vPushChild(pCI->Sqy.m_pCard_sqCopy);
  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->Sqy.m_pCard_sqCopy);
  DELETE_CARD(pCI->Sqy.m_pCard_sqCopy);

  pCI->G.pDeck->vRemoveChild(pCI->G.p_call);
  DELETE_CARD(pCI->G.p_call);

  // 7) derive the fold (SQ result) expr
  pCI->vClearRestart();
  if (pCI->G_bEachMain(CK_FOLD)) {
    pCI->Sqy.m_i_FoldCard = pCI->G.i_MainCard;
    Board::vDerive_foldProjection_F(pCI);
  }

  return pCarryForward;
}


/*----------------------------------------------------------------------------*/
/* XF_Fold a non-correlated sub-query (before outer-query)                       */
/*----------------------------------------------------------------------------*/
CarryForward* XF_Fold::pFold_nc_subQuery(CardIter* pCI, Stz* szFold,
                                         eCK eCardWithSQ, eQO eWhatOp, eSM eSmoosh)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FOLD, _FOOTER_);
  UNUSED(eSmoosh);

  CarryForward* pCarryForward = new CarryForward;

  vOuterQuery(pCI->pBoard()->pSubQuery());

  pCI->vBeginEach();

  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolPred>/<roPred>
  if (pCI->G_bEachMainWithSub(eCardWithSQ, CK_BOOLPRED)) {
    if (! Board::bFindAndMarkSubQryCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz,szFold);
      NEVER_REACH
    }
  }
 // pCI->vPrintBoard(__LINE__);

  // 2) push to sq
  if (! pCI->G_bPushBoard(pCI->Sqy.m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Sqy.m_sSqyNm.szStz());
    NEVER_REACH
  }


  vSubQuery(pCI->pBoard()->pSubQuery());

  // 3) get copy of sq
  pCI->Sqy.m_pCard_sqCopy = pCI->G.pDeck->pDupSubCardsRecursive(0);

  // 4) pop back to mq
  pCI->G_vPopIter();

  // 5) add <fold> (before MQ)
  pCI->G_vNewMember(CK_FOLD, Symbol(SF_FLD_NM,szFold));
  pCI->G.pDeck->vPushChild(pCI->G.p_fold);
  pCI->G.p_level = new Card_Level(AL_NONE);
  pCI->G.p_fold->vAppendChild(pCI->G.p_level);


  v_fold(pCI->G.p_fold);


  // 6) copy up sq (add before fold)
  pCI->G.pDeck->vPushChild(pCI->Sqy.m_pCard_sqCopy);
  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->Sqy.m_pCard_sqCopy);
  DELETE_CARD(pCI->Sqy.m_pCard_sqCopy);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->vClearRestart();
  while (pCI->G_bEachMain_NONE()) {

    if (pCI->G.p_MainCard->pSub_emit())
      pCI->Flux.m_pArriving =
          pCI->G.p_emit = pCI->G.p_MainCard->pSub_emit();

    if (pCI->G_bIsNextMain(CK_FOLD))
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 7) derive the fold (SQ result) expr
  pCI->Sqy.m_pExpr_fold = nullptr;
  pCI->vBeginEach();
  if (pCI->G_bEachMain(CK_FOLD)) {
    pCI->Sqy.m_i_FoldCard = pCI->G.i_MainCard;
    Board::vDerive_foldExpr_H_A_NC_ro(pCI);
    pCI->Sqy.m_pExpr_fold = pCI->G.p_expr;
  }
 // pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCarryForward->vSetSeen(pCI);

  if (CK_HAVING == eCardWithSQ) {
    pCarryForward->Table.m_bReproject = true;
    //pCarryForward->Where_preAgg.m_nBumpBy = 1;
    pCarryForward->GbyAgg.m_bInject_foldExpr = true;
    pCarryForward->Having.m_bCorrect_After_Inject = true;
    pCarryForward->Unproject.m_bUnproject_foldExpr = true;
  }
  else {
    pCarryForward->Table.m_bReproject = true;
    //pCarryForward->Where_preAgg.m_nBumpBy = 1;
    if (QO_IN_OP == eWhatOp || QO_NOT_IN_OP == eWhatOp)
      pCarryForward->Where_preAgg.m_bMorph_sqlOp_To_EQ = true;
    else
    if (QO_RELN_OP == eWhatOp)
      pCarryForward->Where_preAgg.bReplace_sqCall_With_result = true;

//    if (pCarryForward->GbyAgg.m_bSeen)
//      pCarryForward->GbyAgg.m_nBumpBy = 1;

  }

 // pCI->vPrintBoard(__LINE__);

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
  if (pCI->G_bEachMainWithSub(eCardWithSQ, CK_BOOLPRED)) {
    if (! Board::bFindAndMarkSubQryCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz,szFold);
      NEVER_REACH
    }
  }
  // pCI->vPrintBoard(__LINE__);

  // 2) push to sq
  if (! pCI->G_bPushBoard(pCI->Sqy.m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->Sqy.m_sSqyNm.szStz());
    NEVER_REACH
  }

  i8 nTables = Card_Table::nTables(pCI);

  // 4) pop back to mq
  pCI->G_vPopIter();

  return nTables;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
