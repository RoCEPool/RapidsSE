/*============================================================================*/
/* Fold.cpp -- Fold                                                           */
/*============================================================================*/

#include "Fold.hpp"

#include "CardIter.hpp"
#include "CarryForward.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Fold, CN_FOLD)
__aNewFunction(Fold, CN_FOLD)
__vDeleteFunction(Fold, CN_FOLD)
__aFooterFunction(Fold)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fold::Fold(i8 i_depth)
: Object(CN_FOLD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FOLD, _FOOTER_);

  m_i_depth      = i_depth;
  m_p_deck       = nullptr;

  m_p_correlation = nullptr;

  m_pSubQuery    = nullptr;
  m_p_fold       = nullptr;
  m_pOuterQuery  = nullptr;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_FOLD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fold::Fold(i8 i_depth, Card_Deck* p_deck)
: Object(CN_FOLD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FOLD, _FOOTER_);

  m_i_depth      = i_depth;
  m_p_deck       = p_deck;

  m_p_correlation = nullptr;

  m_pSubQuery    = nullptr;
  m_p_fold       = nullptr;
  m_pOuterQuery  = nullptr;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_FOLD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fold::~Fold(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FOLD, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Correlation* Fold::p_correlation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  return m_p_correlation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Fold::v_correlation(Card_Correlation* p_correlation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  m_p_correlation = p_correlation;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Fold::v_deck(Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  m_p_deck = p_deck;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Fold::i_depth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  return m_i_depth;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Fold* Fold::p_fold(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  return m_p_fold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Fold::v_fold(Card_Fold* p_fold)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  m_p_fold = p_fold;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
SubQuery* Fold::pOuterQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  return m_pOuterQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Fold::vOuterQuery(SubQuery* pOuterQuery)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  m_pOuterQuery = pOuterQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
SubQuery* Fold::pSubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  return m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Fold::vSubQuery(SubQuery* pSubQuery)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  m_pSubQuery = pSubQuery;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
CarryForward* Fold::pFlatten_74_H_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FOLD,_FOOTER_);

  CarryForward* pCarryForward = new CarryForward;

  return pCarryForward;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Fold a (not) exists                                                        */
/*----------------------------------------------------------------------------*/
CarryForward* Fold::pFold_exists(Stz* szFold, CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  CarryForward* pCarryForward = new CarryForward;

  // 1) find W_L_NC_NI subquery call in mq:<where>/<boolCalc>

  // W L NC NI
  //               //
  //               //
  // T T W A A A   // find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //     f         //
  //     bc        //
  //    @ro notIn  //
  //        __sq2  // sub-query
  //           #1  // result
  //         $a.2  // argument


  if (pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (! Board::bFindAndMarkSubQryCall(pCI)) {
      vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace(">>>>>>>>> Found W_L_NC_NI call to %s(%s)->%s\n"_Stz, pCI->m_sSqyNm.szStz(), pCI->m_sSqyArg.szStz(), pCI->m_sSqyRsl.szStz());
  pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! pCI->G_bPushBoard(pCI->m_sSqyNm)) {
    pCI->vTracer("DID NOT FIND Board %s\n"_Stz, pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }

  i8 i_SqBoardNo = pCI->G.pBoard->i_Board();

  // x) pull <where> into <table>/<filter>



  pCI->vPrintBoard(__LINE__);

  pCI->G_vRestart();
  if (pCI->G_bEachMain(CK_TABLE)) {
    if (pCI->G_bEachMain(CK_WHERE)) {
      Card_Where::vMoveWhereToTableAsConstraint(pCI);
    }
  }
  pCI->vPrintBoard(__LINE__);

  // 3) get copy of sq:<table>/* into cc:<table>
  pCI->G_vRestart();
  if (pCI->G_bEachMain(CK_TABLE)) {
    pCI->G_vCopyCard(pCI->G.p_table);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  pCI->G_vPopIter();

  // 5) fold
  //vR_S40_T50a_Fold_W_L_C_E_Before_Mq_As_keyExists();
  // put cc:<table>/* before mq:<where>

  pCI->G_vNewMember(CK_FOLD, szFold);
  pCI->G_vInsertBefore(pCI->G.p_fold, pCI->G.p_where);

  pCI->G_vAppendNewChild(pCI->G.p_fold, CK_BOARDNO);
  pCI->G.p_boardNo->vValue(i_SqBoardNo);


  // W L NC NI
  //                   //
  // ------  --mq----  //
  // T T T F W A A A   // find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //     --sq          //
  //     kne           //

  pCI->G.p_table = _pCard_Table(pCI->m_pCard_copied);

  pCI->G_vInsertBefore(pCI->m_pCard_copied, pCI->G.p_fold);

  eCK eCard_Exists = CK_NONE;
  if (pCI->G.pSubQuery->bByIn())
    eCard_Exists = CK_KEYEXISTS;
  else
  if (pCI->G.pSubQuery->bByIn())
    eCard_Exists = CK_KEYNOTEXISTS;
  else
    NEVER_REACH;

  pCI->G_vAppendNewChild(pCI->m_pCard_copied, eCard_Exists);

  Card_Projection::vProjection_Analysis_v2(pCI);
  pCI->G.pDeck->vRenumberRecursive();
//  Card_Stream::vStream_AdjustLoHi(pCI);
//  Board::vRenumberStmOdn(pCI);
  pCI->vPrintBoard(__LINE__);

  Stz* szSqyArg = pCI->m_sSqyArg;

  i8 i_Table = pCI->G.pDeck->iFindChild(pCI->G.p_table);
  bool b = pCI->G_bResolveSubCard(CK_EXPR, szSqyArg, CK_TBLCOLODN, i_Table);
  RASSERT(b);

  Stz* szTblColOdn = pCI->m_pCard_resolved->pSub_tblColOdn()->szValue();
  pCI->G.p_bounds = Card_Bounds::pBounds_Create_Eq_ForGivenCol(szTblColOdn);
  //pCI->m_pCard_copied->vAppend(pCI->G.p_bounds);

  pCI->G.p_indexList = pCI->G.p_table->pSub_indexList();
  pCI->G.p_index = pCI->G.p_table->pSub_index();
  pCI->G.p_index->vAppend(pCI->G.p_bounds);


  pCI->vPrintBoard(__LINE__);

  // W L NC NI
  //                   //
  // T T T F W A A A   //
  //     b             //
  //     kne           //

  pCI->G_vRestart();
  RASSERT(pCI->G_bEachMainWith2Sub(CK_TABLE, eCard_Exists, CK_CONSTRAINT));
  pCI->G.p_table->vRemoveChild(pCI->G.p_constraint);
  pCI->G.p_indexList = pCI->G.p_table->pSub_indexList();
  pCI->G.p_index = pCI->G.p_table->pSub_index();
  pCI->G.p_index->vAppend(pCI->G.p_constraint);
  pCI->G_vCopyCard(nullptr);

  return pCarryForward;
}


/*----------------------------------------------------------------------------*/
/* Fold a from                                                                */
/*----------------------------------------------------------------------------*/
CarryForward* Fold::pFold_from(Stz* szFold, CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FOLD, _FOOTER_);

  CarryForward* pCarryForward = new CarryForward;

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolCalc>

  RASSERT(pCI->G_bEachMain(CK_CALL));
  vTrace(">>>>>>>>> Found From CALl: %s\n"_Stz, pCI->G.p_call->szValue());
  pCI->vPrintBoard(__LINE__);


  pCI->G_vNewMember(CK_FOLD, szFold);
  pCI->G.pDeck->vPush(pCI->G.p_fold);

  pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! pCI->G_bPushBoard(pCI->G.p_call->szValue())) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->G.p_call->szValue());
    NEVER_REACH
  }

  pCI->m_pCard_copied = pCI->G.pDeck->pDupSubCards(0);

  pCI->vPrintBoard(__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  pCI->G_vPopIter();

  pCI->G.pDeck->vPush(pCI->m_pCard_copied);
  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->m_pCard_copied);
  DELETE_CARD(pCI->m_pCard_copied);

  pCI->G.pDeck->vRemoveChild(pCI->G.p_call);
  DELETE_CARD(pCI->G.p_call);

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard(__LINE__);

  // 7) derive the fold (SQ result) expr
  pCI->G_vRestart();
  if (pCI->G_bEachMain(CK_FOLD)) {
    pCI->G.i_FoldCard = pCI->G.i_MainCard;
    Board::vDerive_foldProjection_F(pCI);
  }

  return pCarryForward;
}


/*----------------------------------------------------------------------------*/
/* Fold a non-correlated sub-query (before outer-query)                       */
/*----------------------------------------------------------------------------*/
CarryForward* Fold::pFold_nc_subQuery(Stz* szFold, eCK eCardWithSQ, CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FOLD, _FOOTER_);

  CarryForward* pCarryForward = new CarryForward;

  vOuterQuery(pCI->pBoard()->pSubQuery());

  // 1) find H_A_NC_ro subquery call in mq:<having>/<boolCalc>/<roPred>
  if (pCI->G_bEachMainWithSub(eCardWithSQ, CK_BOOLCALC)) {
    if (! Board::bFindAndMarkSubQryCall(pCI)) {
      vTrace("DID NOT FIND %s SubQuery call\n"_Stz,szFold);
      NEVER_REACH
    }
  }
  vTrace(">>>>>>>>> Found %s call:  %s %s %s\n"_Stz, szFold,
         pCI->m_sSqyArg.szStz(), pCI->m_sSqyRO.szStz(), pCI->m_sSqyNm.szStz());
  pCI->vPrintBoard(__LINE__);

  // 2) push to sq
  if (! pCI->G_bPushBoard(pCI->m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz, pCI->m_sSqyNm.szStz());
    NEVER_REACH
  }


  vSubQuery(pCI->pBoard()->pSubQuery());

  // 3) get copy of sq
  pCI->m_pCard_yanked = pCI->G.pDeck->pDupSubCards(0);

  // 4) pop back to mq
  pCI->G_vPopIter();

  // 5) add <fold> (before MQ)
  pCI->G_vNewMember(CK_FOLD, szFold);
  pCI->G.pDeck->vPush(pCI->G.p_fold);


  v_fold(pCI->G.p_fold);


  // 6) copy up sq (add before fold)
  pCI->G.pDeck->vPush(pCI->m_pCard_yanked);
  pCI->G.pDeck->vReplaceChildWithItsSubs(pCI->m_pCard_yanked);
  DELETE_CARD(pCI->m_pCard_yanked);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G_vRestart();
  while (pCI->G_bEachMain()) {

    if (pCI->G.p_MainCard->pSub_projection())
      pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();

    if (pCI->G_bIsNextMain(CK_FOLD))
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 7) derive the fold (SQ result) expr
  pCI->m_pExpr_fold = nullptr;
  pCI->G_vRestart();
  if (pCI->G_bEachMain(CK_FOLD)) {
    pCI->G.i_FoldCard = pCI->G.i_MainCard;
    Board::vDerive_foldExpr_H_A_NC_ro(pCI);
    pCI->m_pExpr_fold = pCI->G.p_expr;
  }
  pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCarryForward->vSetSeen(pCI);

  if (CK_HAVING == eCardWithSQ) {
    pCarryForward->Table.m_bReproject = true;
    pCarryForward->Where_preAgg.m_nBumpBy = 1;
    pCarryForward->GbyAgg.m_bInject_foldExpr = true;
    pCarryForward->Having.m_bCorrect_After_Inject = true;
    pCarryForward->Unproject.m_bUnproject_foldExpr = true;
  }
  else {
    pCarryForward->Table.m_bReproject = true;
    pCarryForward->Where_preAgg.m_bMorph_bolPred_To_cmpPred = true;
    pCarryForward->Where_preAgg.m_nBumpBy = 1;
  }

  pCI->vPrintBoard(__LINE__);

  return pCarryForward;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
