/*============================================================================*/
/* Board.cpp -- Board                                                         */
/*============================================================================*/

#include "Board.hpp"

#include "PtnDef.hpp"
#include "Cue.hpp"

#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"

#include "Card_StmCol.hpp"
#include "RelOp.hpp"
#include "Error.hpp"
#include "CardIter.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Board,CN_BOARD)
__aNewFunction(Board,CN_BOARD)
__vDeleteFunction(Board,CN_BOARD)
__aFooterFunction(Board)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Board::Board(Stz* szBoardNm, eBK eBoardKind, i8 i_Board, Bench* pBench)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_i_Board    = i_Board;
  m_sBoardNm   = szBoardNm;
  m_eBoardKind = eBoardKind;

  m_pBench     = pBench;

  m_pCard_Deck = new Card_Deck;

  m_iXform = C_UNKNOWN;

  m_pSubQuery = new SubQuery(this);

  _checkInitialised_(CN_BOARD);

  pBench->vAddBoard(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Board::~Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  //m_pCard_Deck->vRemoveAll();

  delete m_pCard_Deck;

  delete m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add partitioning                                                           */
/*----------------------------------------------------------------------------*/
void Board::vAdd_Partitioning(CardIter* pCI)
{
  _static_


  Card_Next* p_next_prior = nullptr;
  Card_Combined* p_combined_prior = nullptr;

  Card* p_main_prior = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  while (pCI->G_bEachMain()) {
    if (CK_INDEXJOIN == pCI->G.e_MainCard ||
        CK_INDEXSCAN == pCI->G.e_MainCard ||
        CK_KEYEXISTS == pCI->G.e_MainCard ||
        CK_KEYNOTEXISTS == pCI->G.e_MainCard ||
        CK_SCAN == pCI->G.e_MainCard) {
      if (p_main_prior) {
        i8 nSkip = Board::iStage_Partitioning(pCI, p_main_prior, p_combined_prior,
                                              p_next_prior, pCI->G.p_MainCard);
        while (nSkip--)
          pCI->G_vSkipNextMain();
        pCI->vPrintBoard(__LINE__);
        vDebugPoint();
      }
      p_main_prior = pCI->G.p_MainCard;
    }
    else
    if (CK_COMBINED == pCI->G.e_MainCard) {
      p_combined_prior = pCI->G.p_combined;
    }
    else
    if (CK_NEXT == pCI->G.e_MainCard) {
      p_next_prior = pCI->G.p_next;
    }

  }

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_fold_projection_From_projection(CardIter* pCI)
{
  _static_

  pCI->G.p_fold->vAppend(pCI->G.p_projection->pDupCardAndSubCards());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_group_order_For_gbyAgg(CardIter* pCI)
{
  _static_

  // todo: to be coded...

  //pCI->G.p_group = _pCard_Group(pCI->G.p_gbyAgg->pAppendNewCardTail(CK_GROUP));

  if ((pCI->G.p_group = pCI->G.p_gbyAgg->pSub_group()))
    pCI->G_vRemoveDeleteMain(CK_GROUP);

  pCI->G_vAppendNewChild(pCI->G.p_gbyAgg, CK_GROUP);
  pCI->G_vAppendNewChild(pCI->G.p_group, CK_ORDER);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect the leading $c.n in <gbyAgg>/<gbyElist>/<expr>/<rpnCalc>/<term=$c.n>
  //  into <gbyAgg>/<group>/<order>/<ordElmOdn=$c.n>

  pCI->G.p_key = pCI->G.p_gbyAgg->pSub_key();

  CardIter xCI(pCI->G.p_key);
  while ((pCI->G.p_expr = _pCard_Expr(xCI.pEachChild(CK_EXPR)))) {
    pCI->G.p_rpnCalc = pCI->G.p_expr->pSub_rpnCalc();

    if (! (pCI->G.p_term = pCI->G.p_rpnCalc->pGetSingle_stmColOdn()))
      break;

    Stz* szStmColOdn = pCI->G.p_term->szValue();
    Stz* szOrdElmOdn = szStmColOdn;
    pCI->G.p_ordElmOdn = _pCard_OrdElmOdn(pCI->G.p_order->pAppendNewCardTail(CK_ORDELMODN, szOrdElmOdn));
    pCI->G.p_ordElmOdn->vAppendNewCardTail(CK_DIRECTION,"ASC"_Stz);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_gbyAgg(CardIter* pCI)
{
  _static_
  NEVER_REACH

/*
  pCI->G.p_stream = pCI->G.p_gbyAgg->pSub_stream();
  if (! pCI->G.p_stream) {
    pCI->G_vAppendNewChild(pCI->G.p_gbyAgg,CK_STREAM);
  }

  if ((pCI->G.p_order = pCI->G.p_stream->pSub_order())) {
    pCI->G.p_stream->vRemoveChild(pCI->G.p_order);
    DELETE_CARD(pCI->G.p_order);
  }

  pCI->G_vAppendNewChild(pCI->G.p_stream, CK_ORDER);

  pCI->G.p_projection =  pCI->G.p_gbyAgg->pSub_projection();
  CardIter xCI(pCI->G.p_projection);
  while ((pCI->G.p_expr = _pCard_Expr(xCI.pEachChild()))) {
    i8 i_Key = pCI->G.p_expr->iRpnCalc_WhichGbyKey();
    if (C_UNKNOWN == i_Key)
      continue;

    pCI->G_vAppendNewChild(pCI->G.p_order, CK_ORDELMODN);

    pCI->G.p_ordElmOdn->vValueSprint("$%lld"_Stz, xCI.i_Child() + 1);
    pCI->G.p_ordElmOdn->vAppendNewCardTail(CK_DIRECTION,"ASC"_Stz);
  }
*/
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_indexJoin(CardIter* pCI)
{
  _static_
  NEVER_REACH
/*
  RASSERT(pCI->G.p_stmColList);
  Card_StmColList* p_stmColList_l = pCI->G.p_stmColList;
  RASSERT(pCI->G.p_order);
  Card_Order* p_stmOrdList_l = pCI->G.p_order;

  RASSERT(pCI->G.p_indexList);
  RASSERT(pCI->G.p_stream);

  pCI->G.p_index = pCI->G.p_indexList->pFindChosenIndex();
  if (! pCI->G.p_index)
    return;

  pCI->G.p_bounds = pCI->G.p_index->pSub_bounds();
  if (! pCI->G.p_bounds)
    return;

  pCI->G.p_eBound = pCI->G.p_bounds->pSub_eBound();
  if (! pCI->G.p_eBound)
    return;

  // get tblColOdn of eBound
  Stz* szTblColOdn_r = pCI->G.p_eBound->szGet_tblColOdn();
  if (! szTblColOdn_r)
    return;
  //vTrace("szTblColOdn_r %s\n"_Stz, szTblColOdn_r);

  // find the stmCol_l == szTblColOdn_r in prior stream stmColList_l
  i8 i_StmCol_l;
  Card_StmCol* p_stmCol_l =
      p_stmColList_l->pStmCol_FindBy_tblColOdn(szTblColOdn_r, i_StmCol_l);
  if (! p_stmCol_l)
    return;

  // get the stmColOdn_l of stmCol_l
  Card_StmColOdn* p_stmColOdn_l = p_stmCol_l->pSub_stmColOdn();
  Stz* szStmColOdn_l = p_stmColOdn_l->szValue();


  // find the stmOrdOdn_l == szStmColOdn_l in prior stream stmColList_l
  i8 i_StmOrdOdn;
  Card_OrdElmOdn* p_stmOrdOdn_l =
      p_stmOrdList_l->pStmOrdOdn_FindBy_stmColOdn(szStmColOdn_l,i_StmOrdOdn);
  if (! p_stmOrdOdn_l)
    return;

  // navigated: ../<eBound>/<tblColOdn_r> -> ../<stmCol_l>/<stmColOdn_l>
  //                                      -> ../<stmOrdOrdn_l>

  Card_Order* p_stmOrdList_ij = new Card_Order;
  pCI->G.p_stream->vAppend(p_stmOrdList_ij);

  pCI->G.p_ordElmOdn =
      _pCard_OrdElmOdn(p_stmOrdList_ij->pAppendNewCardTail(CK_ORDELMODN,
                                                           p_stmOrdOdn_l->szValue()));
  pCI->G.p_ordElmOdn->vAppendNewCardTail(CK_DIRECTION,"ASC"_Stz);
*/
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_indexScan(CardIter* pCI)
{
  _static_
  NEVER_REACH

/*
  pCI->G.p_stream = pCI->G.p_indexScan->pSub_stream();
  if (! pCI->G.p_stream)
    return;
  pCI->G.p_stmColList = pCI->G.p_stream->pSub_stmColList();
  if (! pCI->G.p_stmColList)
    return;

  pCI->G.p_order = nullptr;

  i8 i_StmCol;
  pCI->G.p_index = pCI->G.p_indexScan->pSub_index();
  pCI->G.p_idxColList = pCI->G.p_index->pSub_idxColList();
  CardIter xCI(pCI->G.p_idxColList);
  while ((pCI->G.p_idxCol = _pCard_IdxCol(xCI.pEachChild()))) {
    pCI->G.p_stmCol = pCI->G.p_stmColList->pStmCol_FindBy_tblColOdn(pCI->G.p_idxCol->szValue(), i_StmCol);
    if (! pCI->G.p_stmCol)
      return;

    if (! pCI->G.p_order) {
      pCI->G.p_order = new Card_Order;
      pCI->G.p_stream->vAppend(pCI->G.p_order);
    }

    pCI->G_vAppendNewChild(pCI->G.p_order, CK_ORDELMODN);
    pCI->G.p_ordElmOdn->vValueSprint("$%lld"_Stz, i_StmCol + 1);
    pCI->G.p_ordElmOdn->vAppendNewCardTail(CK_DIRECTION,"ASC"_Stz);
  }

*/
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_project(CardIter* pCI)
{
  _static_
  NEVER_REACH

/*
  pCI->G.p_projection = pCI->G.p_project->pSub_projection();
  pCI->G.p_stream = pCI->G.p_project->pSub_stream();
  pCI->G.p_stmColList = pCI->G.p_stream->pSub_stmColList();

  Card_Order* p_stmOrdList_project = nullptr;

  i8 nLeading = 0;
  CardIter xCI(pCI->G.p_order);
  while ((pCI->G.p_ordElmOdn = _pCard_OrdElmOdn(xCI.pEachChild()))) {
    i8 i_Expr = pCI->G.p_projection->iRpnCalc_Has_stmOrdOdn(pCI->G.p_ordElmOdn);
    if (C_UNKNOWN == i_Expr)
      continue;

    nLeading++;

    if (! p_stmOrdList_project) {
      p_stmOrdList_project = new Card_Order;
      pCI->G.p_stream->vAppend(p_stmOrdList_project);
    }

    pCI->G.p_direction = pCI->G.p_ordElmOdn->pSub_direction();

    pCI->G_vAppendNewChild(p_stmOrdList_project, CK_ORDELMODN);
    pCI->G.p_ordElmOdn->vValueSprint("$%lld"_Stz, i_Expr + 1);
    if (pCI->G.p_direction)
      pCI->G.p_ordElmOdn->vAppend(pCI->G.p_direction->pDupCardAndSubCards());
  }

*/
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_sort(CardIter* pCI)
{
  _static_
  NEVER_REACH

/*
  pCI->G.p_stream = pCI->G.p_sort->pSub_stream();
  pCI->G.p_order = _pCard_Order(pCI->G.p_stream->pAppendNewCardTail(CK_ORDER));

  CardIter xCI(pCI->G.p_key);
  while ((pCI->G.p_expr = _pCard_Expr(xCI.pEachChild(CK_EXPR)))) {
    pCI->G.p_rpnCalc = pCI->G.p_expr->pSub_rpnCalc();

    if (! (pCI->G.p_term = pCI->G.p_rpnCalc->pGetSingle_stmColOdn()))
      break;

    Stz* szStmColOdn = pCI->G.p_term->szValue();
    Stz* szStmOrdOdn = szStmColOdn;
    pCI->G.p_ordElmOdn = _pCard_OrdElmOdn(pCI->G.p_order->pAppendNewCardTail(CK_ORDELMODN, szStmOrdOdn));

    if ((pCI->G.p_direction = pCI->G.p_expr->pSub_direction()))
      pCI->G.p_ordElmOdn->vAppend(pCI->G.p_direction->pDupCardAndSubCards());
    else
      pCI->G.p_ordElmOdn->vAppendNewCardTail(CK_DIRECTION,"ASC"_Stz);
  }
*/
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_order(CardIter* pCI)
{
  _static_
  NEVER_REACH

/*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create stream order for index scan

  pCI->G_vRestart();
  while (pCI->G_bEachMain(CK_INDEXSCAN)) {
    vDerive_stream_order_For_indexScan(pCI);
  }

  pCI->vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pCI->G_vRestart();
  if (pCI->G_bEachMain(CK_INDEXSCAN)) {
    pCI->G.p_stmColList = pCI->G.p_indexScan->pSub_stmColList();
    pCI->G.p_order = pCI->G.p_indexScan->pSub_order();

    for (;;) {
      if (! pCI->G_bEachMainWith2Sub(CK_INDEXJOIN,CK_INDEXLIST,CK_STREAM))
        break;
      Board::vDerive_stream_order_For_indexJoin(pCI);
      pCI->G.p_stmColList = pCI->G.p_indexJoin->pSub_stmColList();
      pCI->G.p_order = pCI->G.p_indexJoin->pSub_order();

    }
  }

  pCI->vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G_vRestart();
  while (pCI->G_bEachMain()) {

    if (! pCI->G.p_MainCard->bIsCard(CK_GBYAGG))
      continue;

    vDerive_group_order_For_gbyAgg(pCI);
    vDerive_stream_order_For_gbyAgg(pCI);

    if (pCI->G_bIsNextMain(CK_GBYAGG)) {
      pCI->G_bEachMain();
      vDerive_group_order_For_gbyAgg(pCI);
      vDerive_stream_order_For_gbyAgg(pCI);

      if (pCI->G_bIsNextMain(CK_GBYAGG)) {
        pCI->G_bEachMain();
        vDerive_group_order_For_gbyAgg(pCI);
        vDerive_stream_order_For_gbyAgg(pCI);
      }
    }

  }

  pCI->G.pDeck->vRenumberRecursive();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G_vRestart();
  while (pCI->G_bEachMain()) {

    if (!pCI->G.p_MainCard->bIsCard(CK_GBYAGG))
      continue;

    pCI->G.p_order = pCI->G.p_gbyAgg->pSub_order();
    while (pCI->G_bIsNextMain(CK_GBYAGG)) {
      pCI->G_bEachMain();
      pCI->G.p_order = pCI->G.p_gbyAgg->pSub_order();
    }

    if (pCI->G_bIsNextMain(CK_PROJECT)) {
      pCI->G_bEachMain();
      vDerive_stream_order_For_project(pCI);
    }

    break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G_vRestart();
  while (pCI->G_bEachMainWithSub(CK_SORT,CK_KEY)) {
    Board::vDerive_stream_order_For_sort(pCI);
  }

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard();

*/
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_foldExpr_H_A_NC_ro(CardIter* pCI)
{
  _static_

  if (! pCI->G_bResolveSubCard(CK_PROJECTION,pCI->G.i_FoldCard))
    return;

  pCI->G.p_type = pCI->m_pCard_resolved->pSub_type();

//  pCI->G_vAppendNewChild(pCI->G.p_fold, CK_FOLDELIST);
//  pCI->G_vAppendNewChild(pCI->G.p_foldElist, CK_EXPR);

  pCI->G_vAppendNewChild(pCI->G.p_fold, CK_EXPR);

  if (pCI->G.p_type) {
    pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
  }
  else {
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_TYPE, "((type missing))"_Stz);
  }
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, "__SQ1RESULT"_Stz);
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc,CK_TERM,"$1"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_foldProjection_F(CardIter* pCI)
{
  _static_

  if (! pCI->G_bResolveSubCard(CK_PROJECTION,pCI->G.i_FoldCard))
    return;

  pCI->G.p_fold->vAppend(pCI->G.p_projection->pDupCardAndSubCards());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(CardIter* pCI)
{
  _static_

  // promote sq <aggreg> seq to <gbyAgg> seq

  for (i8 i_agg = 0; i_agg < 3; i_agg++) {

    pCI->G.p_MainCard->vCard(CK_GBYAGG);
    pCI->G.p_gbyAgg = _pCard_GbyAgg(pCI->G.p_MainCard);

    pCI->G.p_level = pCI->G.p_gbyAgg->pSub_level();
    pCI->G.e_Level = (eAL) pCI->G.p_level->Y_iGet(P_Level);

    pCI->G.p_projection = pCI->G.p_gbyAgg->pSub_projection();

    pCI->G.p_key = new Card_Key;
    pCI->G.p_key->Y_vSet(P_Injected);
    pCI->G.p_gbyAgg->vInsertBefore(pCI->G.p_key,pCI->G.p_projection);


    if (AL_PARTITION == pCI->G.e_Level) {
      pCI->G.p_expr = Card_Expr::pGby_expr_From_expr_corr(pCI->m_pExpr_corr);
      pCI->G.p_key->vAppend(pCI->G.p_expr);
    }
    else {
      pCI->G.p_type = pCI->m_pExpr_corr->pSub_type();
      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendChild(CK_EXPR,pCI->G.p_type->pDupCardAndSubCards());
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$1"_Stz);
      pCI->G.p_key->vAppend(pCI->G.p_expr);

    }
    // TODO: hack...
    pCI->G.p_expr->vCardNo(0);

    if (AL_PARTITION == pCI->G.e_Level) {
      //pCI->G.p_expr = Card_Expr::pProj_expr_From_Gby_expr(pCI->G.p_expr,pCI->m_pExpr_corr->szValue());
      pCI->G.p_expr = Card_Expr::pProj_expr_From_Gby_expr(pCI->G.p_expr,"__SQCRLKEY"_Stz);
    }
    else {
      pCI->G.p_key->Y_vSet(P_ProjBump, (i8)1);
      pCI->G.p_type = _pCard_Type(pCI->G.p_expr->pSub_type()->pDupCardAndSubCards());

      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendChild(CK_EXPR,pCI->G.p_type);
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS,"__SQCRLKEY"_Stz);
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$K1"_Stz);
    }

    pCI->G.p_expr->Y_vSet(P_Injected);
    pCI->G.p_projection->vPush(pCI->G.p_expr);


    if (AL_PARTITION != pCI->G.e_Level) {
      pCI->G.p_projection->vBump_stmOdn_In_rpnCalc(pCI);
      //pCI->G.p_key->Y_vCut(P_ProjBump);
      pCI->G.p_key->Y_vSet(P_ProjBumped);
    }

//    if (pCI->G.p_stream = pCI->G.p_gbyAgg->pSub_stream()) {
//      pCI->G.p_gbyAgg->vRemoveChild(pCI->G.p_stream);
//      DELETE_CARD(pCI->G.p_stream);
//    }
//    pCI->G.p_stream = Card_Stream::pDerive_Of_projection(pCI->G.p_projection);
//    pCI->G.p_gbyAgg->vAppend(pCI->G.p_stream);

    pCI->G.p_key->vRenumberAllExpr();
    pCI->G.p_projection->vRenumberAllExpr();

    if (pCI->G_bEachMain()) {
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
void Board::vEnhance_sq_project_To_Include_Correlation_Column(CardIter* pCI, Stz* szAs)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold sq <project>

  RASSERT (pCI->G.p_MainCard->bIsCard(CK_PROJECT));

  pCI->G.p_type = pCI->G.p_key->pGetChild(0)->pSub_type();

  pCI->G.p_projection = pCI->G.p_project->pSub_projection();

  pCI->G.p_projection->vBump_stmOdn_In_rpnCalc(pCI);// TODO: ,nBumpBy);



  // push <project>/__SQCRLKEY
  pCI->G_vPushNewChild(pCI->G.p_projection, CK_EXPR);
  pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS,szAs);
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$1"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // replace <project>/<stream> now that __SQ1IGBKY1 has been added

//  pCI->G.p_stream = pCI->G.p_project->pSub_stream();
//
//  pCI->G.p_project->vRemoveChild(pCI->G.p_stream);
//  DELETE_CARD(pCI->G.p_stream);
//
//  pCI->G.p_stream = Card_Stream::pDerive_Of_projection(pCI->G.p_projection);
//  pCI->G.p_project->vAppend(pCI->G.p_stream);

//  pCI->G.p_projection->Y_vCut(P_Restream);
//  pCI->G.p_projection->Y_vSet(P_Restreamed);

//  i8 nStmCol = pCI->G.p_stream->pSub_stmColList()->nChildCards();
//  pCI->G.p_stream->Y_vSet(P_OdnLo,(i8)1);
//  pCI->G.p_stream->Y_vSet(P_OdnHi,(i8)nStmCol);

  // bump all $n in <project>/<projElist>/<expr>/<rpnCalc>
  //   now that __SQ1IGBKY1 has been added

  //i8 nBumpBy = pCI->G.p_projection->Y_iGet(P_ProjBump);

//  pCI->G.p_projection->Y_vSet(P_ProjBumped,(i8)nBumpBy);
//  pCI->G.p_projection->Y_vCut(P_ProjBump);

  pCI->G.p_projection->vRenumberAllExpr();
}

/*----------------------------------------------------------------------------*/
/* find and mark possible call to subquery referenced in pCI->G.p_boolCalc    */
/*----------------------------------------------------------------------------*/
bool Board::bFindAndMarkSubQryCall(CardIter* pCI)
{
  _static_

  bool bFoundCall = false;

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,&bFoundCall](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --

    if (! (pCI->m_pTerm_sqCall = p_pred->pGetTerm_boardNm(pCI)))
      return C_CONTINUE_RECURSION;

    pCI->m_sSqyNm  = pCI->m_pTerm_sqCall->szValue();
    p_pred->vMark1(MARK_AT);
    bFoundCall = true;

    if (p_pred->bIsValue("exists"_Stz) || p_pred->bIsValue("notExists"_Stz)) {

      RASSERT(p_pred->bIsSubQueryOp());

      pCI->m_pTerm_0 = p_pred->pGetTerm(0);
      pCI->m_sSqyArg = pCI->m_pTerm_0->szValue();
      pCI->m_pTerm_1 = p_pred->pGetTerm(1);
      pCI->m_sSqyNm  = pCI->m_pTerm_1->szValue();
      return C_TERMINATE_RECURSION;
    }

    if (p_pred->bIsValue("in"_Stz) || p_pred->bIsValue("notIn"_Stz)) {
      RASSERT(p_pred->bIsSubQueryOp());
      pCI->m_pTerm_2 = _pCard_Term(p_pred->pGet(2));
      pCI->m_sSqyArg = pCI->m_pTerm_2->szValue();
      return C_TERMINATE_RECURSION;
    }

    RASSERT(p_pred->bIsRelnOp());

    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);

  if (bFoundCall)
    return true;
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Board::bIdentify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold identify the <table>/<filter>/<boolCalc>/<cmpPred> which is correlated by parameter
  bool bIsCorrPred = false;

  auto bMarkCorrPred = [pCI,&bIsCorrPred](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCorrPred())
      return C_CONTINUE_RECURSION;

    p_pred->vDump();
    bIsCorrPred = true;

    Card_Term* pTerm_corrCol = static_cast<Card_CmpPred *>(p_pred)->pGetCorrCol();
    pCI->m_pTerm_corrCol = pTerm_corrCol;

    p_pred->vMark1(MARK_AT);
    p_pred->Y_vSet(P_SqCorrPred);
    return C_TERMINATE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc = pCI->G.p_filter->pSub_boolCalc();
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));
  bool bIgnore = pCI->G.p_PRED->bPredRecursive(bMarkCorrPred);

  return bIsCorrPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vFind_Correlation_Expr_Of_Correlation_Column(CardIter* pCI)
{
  _static_

  // find the <table>/<stream>/<stmColList>/<stmCol>
  //  corresponding to the correlated predicate

  pCI->m_pCard_found = pCI->G.p_projection->pFindValueRecursive(pCI->m_pTerm_corrCol->szValue(), pCI->m_pCard_parent);
  pCI->m_pExpr_corr = _pCard_Expr(pCI->m_pCard_parent);

  pCI->m_pExpr_corr->vMark1(MARK_AT);
  pCI->m_pExpr_corr->Y_vSet(P_CorrExpr);
}


/*----------------------------------------------------------------------------*/
/* convert <table> into either <scan>, <indexJoin> or <keyExists>             */
/*----------------------------------------------------------------------------*/
void Board::vMorph_Table_To_EEop(CardIter* pCI)
{
  _static_

  pCI->vPrintBoard(__LINE__);

//  i8 i_Fold = C_UNKNOWN;
//  bool bFold = pCI->G_bLocateCard(CK_FOLD,C_UNKNOWN,i_Fold);

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_FLAT))
    pCI->G.pDeck->vRemoveDeleteChild(pCI->G.p_flat);

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_TABLE)) {

    pCI->G.p_bounds         = pCI->G.p_table->pSub_bounds();
    pCI->G.p_index          = pCI->G.p_table->pSub_index();
    pCI->G.p_indexJoin      = pCI->G.p_table->pSub_indexJoin();
    pCI->G.p_indexList      = pCI->G.p_table->pSub_indexList();
    pCI->G.p_keyExists      = pCI->G.p_table->pSub_keyExists();
    pCI->G.p_keyNotExists   = pCI->G.p_table->pSub_keyNotExists();

    vTrace("MainCard %p : [%lld] %s %s\n"_Stz,
           pCI->G.p_table,pCI->G.p_table->iCardNo(),aCK[pCI->G.p_table->eCard()],pCI->G.p_table->szValue());

    RASSERT(! pCI->G.p_indexJoin);


    // SCAN
    if (0 == pCI->G.i_MainCard)  {
      Card_Scan::vTableMorphsToScan(pCI);
    }
    // KEYEXISTS
    else if (pCI->G.p_keyExists) {
      Card_KeyExists::vTableMorphsToKeyExists(pCI);
    }
    // KEYNOTEXISTS
    else if (pCI->G.p_keyNotExists) {
      Card_KeyNotExists::vTableMorphsToKeyNotExists(pCI);
    }
    // CROSSJOIN
    else if (! pCI->G.p_bounds) { // && bFold && pCI->G.i_MainCard == i_Fold+1) {
      Card_CrossJoin::vTableMorphsToCrossJoin(pCI);
    }
    // INDEXJOIN
    else {
      Card_IndexJoin::vTableMorphsToIndexJoin(pCI);
    }


    if ((pCI->G.p_tableNm = pCI->G.p_MainCard->pSub_tableNm())) {
      pCI->G.p_MainCard->vRemoveSub(pCI->G.p_tableNm);
      pCI->G.p_table->vAppend(pCI->G.p_tableNm);
    }
    if ((pCI->G.p_tablePtn = pCI->G.p_MainCard->pSub_tablePtn())) {
      pCI->G.p_MainCard->vRemoveSub(pCI->G.p_tablePtn);
      pCI->G.p_table->vAppend(pCI->G.p_tablePtn);
    }
    if ((pCI->G.p_size = pCI->G.p_MainCard->pSub_size())) {
      pCI->G.p_MainCard->vRemoveSub(pCI->G.p_size);
      pCI->G.p_table->vAppend(pCI->G.p_size);
    }

    pCI->G.pDeck->vRenumberRecursive();

    pCI->vPrintBoard(__LINE__);

    vDebugPoint();
  }

  pCI->vPrintBoard(__LINE__);

  pCI->G_vRestart();
  while (pCI->G_bEachMain(CK_SCAN)) {
    i8 i_Child;
    pCI->G.p_note = _pCard_Note(pCI->G.p_scan->pFindChild(CK_NOTE,i_Child));

    if (i_Child == pCI->G.p_scan->nChildCards() - 1) {
      pCI->G.p_scan->vRemoveChild(pCI->G.p_note);
      DELETE_CARD(pCI->G.p_note);
    }
  }
  pCI->vPrintBoard(__LINE__);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vMove_corrPred_To_table_filter(CardIter* pCI, Card_BoolCalc *p_boolCalc)
{
  _static_

  Card_PRED* p_pred_corr = p_boolCalc->pFindRemove_CorrPred();
  if (! p_pred_corr)
    return;

  p_pred_corr->vDump();

  Card_Term* p_term_tblColOdn = p_pred_corr->pGetFirstTerm_tblColOdn();
  char cTblSyn = p_term_tblColOdn->cGetTableSyn();
  Card_Table* p_table = Card_Table::pResolve_table_By_tblSyn(pCI,cTblSyn,pCI->G.i_MainCard);

  p_table->vAdd_Pred_Under_filter(p_pred_corr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred_a(CardIter* pCI, Card_BoolCalc *p_boolCalc)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [pCI, p_boolCalc](Card_PRED* p_pred,
                                    auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! p_pred->bIsPRED())
      return; /* this effectively declares resolve as void */

    if (p_pred->bIsCard(CK_CMPPRED)) {

      RASSERT(p_pred->bIsValue("=="_Stz));

      if (! p_pred->Y_bHas(P_OqCorrTest))
        return;

      CardIter xCI(p_pred);
      while (Card* p_child = xCI.pEachChild()) {
        if (p_child->bIsPRED()) {
          vResolve(static_cast<Card_PRED*>(p_child), vResolve);
          continue;
        }
        // TODO: test for: == $d.2 %1 => == $1 [TRANSLATE PARAM %1]

        if (Card::bIsCorrParamOrdinal(p_child->szValue())) {
          //R_vUTE_Resolve_Param_Ordinal(_pCard_Term(p_child));
          p_child->vValue("$1"_Stz);  // TODO: hack!!
        }
        else {
          Card_OuterColNm* p_outerColNm = pCI->m_pCorrelation->pSub_outerColNm();
          Stz* szOuterColNm = p_outerColNm->szValue();
          if (pCI->G_bResolveSubCard(CK_EXPR, CK_COLNM, szOuterColNm, pCI->G.i_MainCard)) {

            if (pCI->m_pCard_resolved->bIsCard(CK_EXPR)) {
              pCI->G.p_tblColOdn = pCI->G.p_expr->pSub_tblColOdn();
              vDebugPoint();

              p_child->vValue(pCI->G.p_tblColOdn->szValue());

            }
          }




//          char cTblSyn = Card::cTableColSyn(p_child->szValue());
//          cTblSyn = 'a'; // TODO: hack!!
//          p_child->vSetTableColSyn(cTblSyn);


        }
      }
      return;
    }
    else
    if (p_pred->bIsCard(CK_BOLPRED)) {
      return;
    }
    else
    if (p_pred->bIsCard(CK_ANDPRED) || p_pred->bIsCard(CK_ORPRED)) {
      CardIter xCI(p_pred);
      while (Card_PRED* p_PRED_child = static_cast<Card_PRED*>(xCI.pEachChild())) {
        RASSERT(p_PRED_child->bIsPRED());
        vResolve(p_PRED_child, vResolve);
      }
    }
    else {
      NEVER_REACH;
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_PRED* p_PRED = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  vResolve(p_PRED, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred_b(CardIter* pCI, Card_BoolCalc *p_boolCalc)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [pCI, p_boolCalc](Card_PRED* p_pred,
                                    auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! p_pred->bIsPRED())
      return; /* this effectively declares resolve as void */

    if (p_pred->bIsCard(CK_CMPPRED)) {
      return;
    }
    else
    if (p_pred->bIsCard(CK_BOLPRED)) {

      if (! p_pred->bContains_callToSubQuery(pCI))
        return;

      RASSERT(p_pred->bIsBoolFn() ||
              p_pred->bIsRelnOp() ||
              p_pred->bIsSubQueryOp());

      RASSERT(! p_pred->Y_bHas(P_SqCorrPred));

      p_pred->vCard(CK_CMPPRED);
      CardIter xCI(p_pred);
      while (Card* p_child = xCI.pEachChild()) {
        if (p_child->bIsPRED()) {
          vResolve(static_cast<Card_PRED*>(p_child), vResolve);
          continue;
        }
        // TODO: test for: < $a.5 $b.1 __subquery2 => < $a.5 $2

        if (pCI->m_pBench->bIsSubQueryNm(p_child->szValue())) {
          Card* p_arg = xCI.pPrevChild();

          Card_Term* p_term_new = new Card_Term("$2"_Stz);
          p_pred->vInsertAfter(p_term_new,p_child);

          p_pred->vRemoveChild(p_arg);
          DELETE_CARD(p_arg);

          p_pred->vRemoveChild(p_child);
          DELETE_CARD(p_child);
        }
      }
      return;
    }
    else
    if (p_pred->bIsCard(CK_ANDPRED) || p_pred->bIsCard(CK_ORPRED)) {
      CardIter xCI(p_pred);
      while (Card_PRED* p_PRED_child = static_cast<Card_PRED*>(xCI.pEachChild())) {
        RASSERT(p_PRED_child->bIsPRED());
        vResolve(p_PRED_child, vResolve);
      }
    }
    else {
      NEVER_REACH;
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_PRED* p_PRED = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  vResolve(p_PRED, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Board::iStage_Partitioning(CardIter* pCI, Card* p_main_prior,
                             Card_Combined* p_combined_prior,
                             Card_Next* p_next_prior,
                             Card* p_main_curr)
{
  _static_

  Card_Table*      p_table_prior      = p_main_prior->pSub_table();

  Card_Table*      p_table_curr     = p_main_curr->pSub_table();
  Card_IndexList*  p_indexList_curr = p_main_curr->pSub_indexList();

  p_indexList_curr->vDump();

  Card_Index*      p_index_curr     = p_indexList_curr->pFindChosenIndex();
  Card_Bounds*     p_bounds_curr    = p_main_curr->pSub_bounds();

  Card_Hash* p_hash;
  Card_Term* p_term_hash;
  Stz* szTblColOdn;
  Card_StmCol* p_stmCol_hash;
  i8 i_StmCol;

  eXK eCastKind = p_bounds_curr->eCastKind_v2(p_index_curr,
                                              p_table_prior, p_table_curr,
                                              p_term_hash);

  switch (eCastKind) {
    case XK_BROADCAST:
      p_next_prior->vValue("broadcast"_Stz);
      return 0;

    case XK_HASHCAST:
      p_next_prior->vValue("hashcast"_Stz);

      szTblColOdn = p_term_hash->szValue();
      //p_stmCol_hash = p_stmColList_prior->pStmCol_FindBy_tblColOdn(szTblColOdn, i_StmCol);
      //p_hash = Card_Hash::pDerive(p_stmCol_hash);

      RASSERT(pCI->G_bResolveSubCard(CK_STMCOL,CK_TBLCOLODN,szTblColOdn, pCI->G.i_MainCard+1));

      //p_hash = Card_Hash::pDerive(pCI->G.p_stmCol);
      p_hash = Card_Hash::pDerive(pCI->G.p_expr);


      if (p_combined_prior) {
        pCI->G_vInsertBefore(p_hash, p_combined_prior);
        return 1;
      }
      else
      if (p_next_prior) {
        pCI->G_vInsertBefore(p_hash, p_next_prior);
        Card_Combined* p_combined = new Card_Combined;
        pCI->G_vInsertBefore(p_combined, p_next_prior);
        return 2;
      }
      else
        NEVER_REACH

    case XK_UNICAST:
      return 0;

    default:
      NEVER_REACH
  }

  NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_26_SubQuery_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vOuter_Query_Analysis_Of_from_call(CardIter* pCI)
{
  _static_

  Stz* szBoardNm = pCI->G.p_call->szValue();
  Board* pBoard = pCI->m_pBench->pGetBoard(szBoardNm);
  SubQuery *pSubQuery = pBoard->pSubQuery();
  pSubQuery->vCaller(pCI->G.i_Board);
  pSubQuery->vClause(QC_FROM_CLAUSE);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vOuter_Query_Analysis_Of_having_boolCalc(CardIter* pCI)
{
  _static_

  //i8 i_Main = pCI->G.pDeck->iFindChild(pCI->G.p_MainCard);


  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_PRED *p_pred, bool bUpwards) {

    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (p_pred->bIsCard(CK_TERM)) {
      return C_CONTINUE_RECURSION;
    }

    Stz* szValue_term = p_pred->szValue();
    eRO eRelnOp = RelOp::eDecode(szValue_term);

    if (RO_NONE == eRelnOp)
      return C_CONTINUE_RECURSION;

    Stz* szSubBoardNm = p_pred->szGetBoardNmInTerm(pCI);
    if ( ! szSubBoardNm || ! pCI->G_bIsBoard(szSubBoardNm))
      return C_CONTINUE_RECURSION;

    Board* pSubBoard = pCI->m_pBench->pGetBoard(szSubBoardNm);
    SubQuery* pSubQuery = pSubBoard->pSubQuery();


    pSubQuery->vCaller(pCI->G.i_Board);
    pSubQuery->vClause(QC_HAVING_CLAUSE);
    pSubQuery->vCallOp(QO_RELN_OP);
    pSubQuery->vRelnOp(eRelnOp);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vOuter_Query_Analysis_Of_where_boolCalc(CardIter* pCI)
{
  _static_

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_PRED *p_pred, bool bUpwards) {

    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (p_pred->bIsCard(CK_TERM)) {
      return C_CONTINUE_RECURSION;
    }

    Stz *szSubBoardNm = p_pred->szGetBoardNmInTerm(pCI);
    if (! szSubBoardNm || ! pCI->G_bIsBoard(szSubBoardNm))
      return C_CONTINUE_RECURSION;

    // todo: pCI->pGetBoard(szSubBoardNm); pCI->pGetSubQuery(szSubBoardNm);
    Board *pSubBoard = pCI->m_pBench->pGetBoard(szSubBoardNm);
    SubQuery *pSubQuery = pSubBoard->pSubQuery();

    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --

    if (p_pred->bIsCard(CK_BOLPRED)) {
      Stz* szValue_pred = p_pred->szValue();

      if (p_pred->bIsRelnOp()) {

        eRO eRelnOp = RelOp::eDecode(szValue_pred);
        RASSERT(RO_NONE != eRelnOp);

        pSubQuery->vCaller(pCI->G.i_Board);
        pSubQuery->vClause(QC_WHERE_CLAUSE);
        pSubQuery->vCallOp(QO_RELN_OP);
        pSubQuery->vRelnOp(eRelnOp);
        return C_CONTINUE_RECURSION;
      }

      RASSERT(Card::bIsBoolFn(szValue_pred) || Card::bIsSubQueryOp(szValue_pred));

      eQO eSubQryOp = Card::eEvalSubQryOp(szValue_pred);
      RASSERT(QO_NONE != eSubQryOp);

      pSubQuery->vCaller(pCI->G.i_Board);
      pSubQuery->vClause(QC_WHERE_CLAUSE);
      pSubQuery->vCallOp(eSubQryOp);
      return C_CONTINUE_RECURSION;
    }

    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --


    //  --  --  --  --  --  --  --  --  --  --  --  -- -- --

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_A0_Make_Weavable */
/*============================================================================*/

void Board::vIndividuate_indexJoin_filter_b(CardIter* pCI)
{
  _static_

  // todo: xt

  pCI->G.p_indexJoin->vRemoveChild(pCI->G.p_filter);
  pCI->G.pDeck->vInsertAfter(pCI->G.p_filter,
                             pCI->G.p_indexJoin);

  if (! pCI->G.p_filter->pSub_level()) {
    pCI->G.p_filter->vPush(pCI->G.p_indexJoin->pSub_level()->pDupCardAndSubCards());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vIndividuate_crossJoin_filter(CardIter* pCI)
{
  _static_

  // todo: xt

  pCI->G.p_crossJoin->vRemoveChild(pCI->G.p_filter);
  pCI->G.pDeck->vInsertAfter(pCI->G.p_filter,
                             pCI->G.p_crossJoin);

  if (! pCI->G.p_filter->pSub_level()) {
    pCI->G.p_filter->vPush(pCI->G.p_crossJoin->pSub_level()->pDupCardAndSubCards());
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get board kind                                                             */
/*----------------------------------------------------------------------------*/
eBK Board::eBoardKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eBoardKind;
}

/*----------------------------------------------------------------------------*/
/* get board name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Board::szBoardNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sBoardNm;
}

/*----------------------------------------------------------------------------*/
/* set board name                                                             */
/*----------------------------------------------------------------------------*/
void Board::vBoardNm(Stz* szBoardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sBoardNm = szBoardNm;
}

/*----------------------------------------------------------------------------*/
/* get card registers                                                         */
/*----------------------------------------------------------------------------*/
CardCtl* Board::pCardCtl(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pBench->pCardCtl();
}

/*----------------------------------------------------------------------------*/
/* clear all marks                                                           */
/*----------------------------------------------------------------------------*/
void Board::vClearMarks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_pCard_Deck->vClearMarksRecursive();
}

/*----------------------------------------------------------------------------*/
/* get card subSet                                                            */
/*----------------------------------------------------------------------------*/
Card_Deck* Board::pDeck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pCard_Deck;
}


/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinBanners(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  pRibbon->vDrawH('-',true);

  pRibbon->vDrawH('=');

  //pRibbon->vPutTextRight(szXhand(),1);
  //pRibbon->vPutTextRight(szXform(),2);

  s32 sLeftMargin;

  char cPrefix = m_pBench->cPrefix();

  if (' ' == cPrefix)
    sLeftMargin.vSprint("%lld "_Stz, m_i_Board);
  else
  if ('m' == cPrefix || 'w' == cPrefix)
    sLeftMargin.vSprint("%c "_Stz, cPrefix);
  else
  if (m_pBench->bIsFlat() && 0 == m_i_Board)
    sLeftMargin.vSprint("fq "_Stz);
  else
    sLeftMargin.vSprint("%c%lld "_Stz, cPrefix,m_i_Board);

  pRibbon->vInsertCharsV(0,0,sLeftMargin);
  //pRibbon->vDrawV(0,0,"x"_Stz);
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinCards(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  CardCtl* pCardCtl = m_pBench->pOptimiser()->pCardCtl();

  if (! m_pCard_Deck->nItems())
    return;

  pRibbon->vLaneFirst();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Ribbon* pSubRibbon = new Ribbon(pCardCtl,this);

  CardIter xBI(m_pCard_Deck);

  i8 nCount = 0;

  while (Card* pCard = xBI.pEachChild()) {
    eCK eCard = pCard->eCard();

    // skip non-printable horizontal lines
    if (pCardCtl->bHeatNext(eCard) &&
        pCard->Y_bHas(pCardCtl->szHorizExclude(eCard))) {

      while (Card* pNextMain = xBI.pNextChild()) {
        eCK eNextCard = pNextMain->eCard();
        if (pCardCtl->bHeatNext(eNextCard))
          break;
        xBI.vForward();
      }

      continue;
    }

    if (! nCount++)
      pSubRibbon->vLaneFirst();
    else
    if (pSubRibbon->i_Char() && pCardCtl->bHeatNext(eCard))
      pSubRibbon->vHeatNext();
    else
      pSubRibbon->vLaneNext();

    /*
    if (pCard->bIsCard(CK_INDEXJOIN) ||
        pCard->bIsCard(CK_SCAN)      ||
        pCard->bIsCard(CK_TABLE)) {
      eLevel = AL_PARTITION;
    }
    else
    if (   pCard->bIsCard(CK_AGGREG)
        || pCard->bIsCard(CK_PROJECT)) {
      Card_Level* p_level = pCard->pSub_level();
      if (p_level)
        eLevel = (eAL)p_level->Y_iGet(P_Level);
    }
    else
    if (pCard->bIsCard(CK_STAGE)) {
      Card_Topology* p_topology = pCard->pSub_topology();
      eYG eStageTopo = (eYG)p_topology->Y_iGet(P_StageTopo);
      switch (eStageTopo) {
        case YG_ALL_PARTITIONS : eLevel = AL_PARTITION; break;
        case YG_ALL_NODES      : eLevel = AL_NODE;      break;
        case YG_ORIGIN_NODE    : eLevel = AL_ORIGIN;    break;
        default                : eLevel = AL_NONE;      break;
      }
    }
*/

    bool bPrune = false;
    eAL eLevel = AL_NONE;
    Card_Level* p_level = pCard->pSub_level();
    if (p_level)
      eLevel = (eAL) p_level->Y_iGet(P_Level);
    pCard->vPrintCardRecursive(pSubRibbon, 0, m_pCard_Deck, bPrune, eLevel, false);
  }

  pRibbon->vPinSubRibbon(pRibbon->i_Heat(), 0, pSubRibbon);
  
  delete pSubRibbon;
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinTitle(Ribbon* pRibbon,
                      i8 iTransformNo, Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  if (! m_pCard_Deck->nItems())
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // line 1
  pRibbon->vSprint("Transform: %02lld Board %lld %s Query %s"_Stz,
                   m_iXform,
                   m_i_Board,
                   m_sBoardNm.szStz(),
                   m_pBench->pOptimiser()->szQueryNm()
                   );

  // line 2
  pRibbon->vSprint("%s"_Stz,szXdesc());

  // line 3
  if (0 == m_i_Board)
    pRibbon->vSprint("%s %s"_Stz,
                     "MainQuery"_Stz,
                     aQD[m_pSubQuery->eDisposition()]);
  else
    pRibbon->vSprint("%s(%lld params) %s caller: board[%lld] %s %s %s"_Stz,
                     "SubQuery"_Stz,
                     m_pSubQuery->nCorrCols(),
                     aQD[m_pSubQuery->eDisposition()],

                     m_pSubQuery->pBoard()->i_Board(),
                     aQC[m_pSubQuery->eClause()],
                     aQO[m_pSubQuery->eCallOp()],
                     aRO[m_pSubQuery->eRelnOp()]
                     );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pRibbon->vTitleLines();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vPrintBoard(Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  Ribbon* pRibbon = new Ribbon(m_pBench->pOptimiser()->pCardCtl(),this);

  vPinTitle(pRibbon, m_iXform, szDescription);

  vPinCards(pRibbon);

  vPinBanners(pRibbon);

  pRibbon->vPrintRibbon();

  delete pRibbon;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Board::i_Board(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  return m_i_Board;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vBoardNo(i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  m_i_Board = i_Board;
}

/*----------------------------------------------------------------------------*/
/* get sub-query                                                              */
/*----------------------------------------------------------------------------*/
SubQuery* Board::pSubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/* get description of transformation                                          */
/*----------------------------------------------------------------------------*/
Stz* Board::szXdesc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sXdesc;
}

/*----------------------------------------------------------------------------*/
/* set description of transformation                                          */
/*----------------------------------------------------------------------------*/
void Board::vXdesc(Stz* szDescription)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sXdesc = szDescription;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
i8 Board::iXform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_iXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
void Board::vXform(i8 iXform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_iXform = iXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
Stz* Board::szXform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
void Board::vXform(Stz* szTransformation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sXform = szTransformation;
}

/*----------------------------------------------------------------------------*/
/* get transformation phase                                                   */
/*----------------------------------------------------------------------------*/
Stz* Board::szXhand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_sXhand;
}

/*----------------------------------------------------------------------------*/
/* set transformation phase                                                   */
/*----------------------------------------------------------------------------*/
void Board::vXhand(const char* asXhand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_sXhand = asXhand;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
