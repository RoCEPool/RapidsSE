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
Board::Board(Stz* szBoardNm, eBK eBoardKind, i8 i_Board, eBT eBoardType, Bench* pBench)
: Object(CN_BOARD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_i_Board    = i_Board;
  m_sBoardNm   = szBoardNm;
  m_eBoardKind = eBoardKind;
  m_eBoardType = eBoardType;
  m_eBoardRole = BR_NONE;
  m_eBoardForm = BF_NONE;
  m_eBoardCall = BC_NONE;

  m_eSubQuery  = QS_NONE;

  m_pBench     = pBench;

  m_pOptimiser = m_pBench->pOptimiser();

  m_pCard_Deck = new Card_Deck;

  m_iXform = C_UNKNOWN;

  m_bMayHaveStmColOdn = true;

  m_pSubQuery = new SubQuery;

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

  if (BT_VIEW == m_eBoardType) {
    m_pCard_Deck->vRemoveAll();
  }

  delete m_pCard_Deck;

  delete m_pSubQuery;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vConvert_stmColOdn_Of_constraint_To_stmColNm(CardIter* pCI)
{
  _static_

  i8 i_Main = pCI->G.pDeck->iFindChild(pCI->G.p_MainCard);

  Card_BoolCalc* p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,i_Main](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! Card::bIsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    pCI->G_bResolveSubCard(CK_STMCOL, CK_STMCOLODN, p_term->szValue(), i_Main + 1);

    Card_StmColNm* p_stmColNm = pCI->G.p_stmCol->pSub_stmColNm();

    p_term->vSetValueMark1(p_stmColNm->szValue(), MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vConvert_tblColOdn_To_stmOdn(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    //vTrace("CONVERT %s\n"_Stz,p_term->szValue());

    if (! Card::bIsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Stz* szStmColOdn = Card_StmColOdn::szResolve_tblColOdn_Into_stmColOdn(pCI, p_term,pCI->G.i_MainCard+1);

    p_term->vSetValueMark1(szStmColOdn, MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_MainCard->bApplyRecursive(CK_TERM, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vCorrect_having_BaseOn_foldExpr(CardIter* pCI)
{
  _static_

  pCI->G.p_boolCalc = pCI->G.p_having->pSub_boolCalc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card *pCard) {

    if (MARK_AT != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    pCI->G.p_roPred = _pCard_RoPred(pCard);

    pCI->m_pTerm_0 = _pCard_Term(pCI->G.p_roPred->pGet(0));
    pCI->m_pTerm_0->vBump_StreamColOrdinal();

    pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
    pCI->G.p_roPred->vRemoveChild(pCI->m_pTerm_1);
    DELETE_CARD(pCI->m_pTerm_1);
    pCI->G_vAppendNewChild(pCI->G.p_roPred, CK_TERM, "$1"_Stz);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc->bApplyRecursive(CK_ROPRED, bLambdaFn);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_foldElist_From_project_stream(CardIter* pCI)
{
  _static_


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // <fold> W_A_C_ro


  pCI->G_vAppendNewChild(pCI->G.p_fold, CK_FOLDELIST);

  pCI->G.p_stmColList = pCI->G.p_stream->pSub_stmColList();

  if (pCI->G.p_stmColList->nChildCards() > 1) {

    pCI->G_vAppendNewChild(pCI->G.p_foldElist, CK_EXPR);
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_NOTE, "// sq gbykey"_Stz);
    pCI->G.p_stmCol = _pCard_StmCol(pCI->G.p_stmColList->pGetChild(0));
    pCI->G.p_stmColNm = pCI->G.p_stmCol->pSub_stmColNm();
    pCI->G.p_type = pCI->G.p_stmCol->pSub_type();
    pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, pCI->G.p_stmColNm->szValue());
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
    pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$1"_Stz);

    pCI->G_vAppendNewChild(pCI->G.p_foldElist, CK_EXPR);
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_NOTE, "// sq result"_Stz);
    pCI->G.p_stmCol = _pCard_StmCol(pCI->G.p_stmColList->pGetChild(1));
    pCI->G.p_stmColNm = pCI->G.p_stmCol->pSub_stmColNm();
    pCI->G.p_type = pCI->G.p_stmCol->pSub_type();
    pCI->G.p_as = pCI->G.p_stmCol->pSub_as();
    pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, pCI->G.p_stmColNm->szValue());
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
    pCI->G_vAppendNewChild(pCI->G.p_rpnCalc,CK_TERM,"$2"_Stz);

    pCI->G.p_fold->Y_vSet(P_BumpRight,(i8)2);
  }
  else {

    pCI->G_vAppendNewChild(pCI->G.p_foldElist, CK_EXPR);
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_NOTE, "// sq result"_Stz);
    pCI->G.p_stmCol = _pCard_StmCol(pCI->G.p_stmColList->pGetChild(0));
    pCI->G.p_stmColNm = pCI->G.p_stmCol->pSub_stmColNm();
    pCI->G.p_type = pCI->G.p_stmCol->pSub_type();
    pCI->G.p_as = pCI->G.p_stmCol->pSub_as();
    pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, pCI->G.p_stmColNm->szValue());
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
    pCI->G_vAppendNewChild(pCI->G.p_rpnCalc,CK_TERM,"$1"_Stz);

    pCI->G.p_fold->Y_vSet(P_BumpRight,(i8)1);
  }


/*
  if (pCI->G.p_type) {
    pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
  }
  else {
    pCI->G_vAppendNewChild(pCI->G.p_expr, CK_TYPE, "((type missing))"_Stz);
  }
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS, "__SQ1RESULT"_Stz);
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc,CK_TERM,"$1"_Stz);


  pCI->G.p_foldElist = pCI->G.p_fold->pSub_foldElist();

  pCI->G_vPushNewChild(pCI->G.p_foldElist, CK_EXPR);

  */
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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_indexJoin(CardIter* pCI)
{
  _static_

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_indexScan(CardIter* pCI)
{
  _static_

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_project(CardIter* pCI)
{
  _static_

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_stream_order_For_sort(CardIter* pCI)
{
  _static_

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_order(CardIter* pCI)
{
  _static_

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

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vDerive_foldExpr(CardIter* pCI)
{
  _static_

  if (! pCI->G_bResolveSubCard(CK_PROJECTION,pCI->G.i_FoldCard))
    return;

  pCI->G.p_type = pCI->m_pCard_resolved->pSub_type();

  pCI->G_vAppendNewChild(pCI->G.p_fold, CK_FOLDELIST);
  pCI->G_vAppendNewChild(pCI->G.p_foldElist, CK_EXPR);

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
void Board::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold promote sq <aggreg> seq to <gbyAgg> seq

  for (i8 i_Series = 0; i_Series < 3; i_Series++) {

    pCI->G.p_MainCard->vCard(CK_GBYAGG);
    pCI->G.p_gbyAgg = _pCard_GbyAgg(pCI->G.p_MainCard);

    pCI->G.p_level = pCI->G.p_gbyAgg->pSub_level();
    pCI->G.e_Level = (eAL) pCI->G.p_level->Y_iGet(P_Level);

    pCI->G.p_projection = pCI->G.p_gbyAgg->pSub_projection();

    pCI->G.p_projection->vCard(CK_PROJECTION);
    pCI->G.p_projection = _pCard_Projection(pCI->G.p_projection);

    pCI->G.p_key = new Card_Key;
    pCI->G.p_key->Y_vSet(P_Injected);
    pCI->G.p_gbyAgg->vInsertBefore(pCI->G.p_key,pCI->G.p_projection);


    if (AL_PARTITION == pCI->G.e_Level) {
      pCI->G.p_expr = Card_Expr::pGby_expr_From_stmCol(pCI->G.p_stmCol);
      pCI->G.p_key->vAppend(pCI->G.p_expr);
    }
    else {
      pCI->G.p_type = pCI->G.p_stmCol->pSub_type();
      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendChild(CK_EXPR,pCI->G.p_type->pDupCardAndSubCards());
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$1"_Stz);
      pCI->G.p_key->vAppend(pCI->G.p_expr);

    }
    // TODO: hack...
    pCI->G.p_expr->vCardNo(0);

    if (AL_PARTITION == pCI->G.e_Level) {
      pCI->G.p_expr = Card_Expr::pProj_expr_From_Gby_expr(pCI->G.p_expr,pCI->G.p_stmCol->szValue());
    }
    else {
      pCI->G.p_key->Y_vSet(P_ProjBump, (i8)1);
      pCI->G.p_type = _pCard_Type(pCI->G.p_expr->pSub_type()->pDupCardAndSubCards());

      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendChild(CK_EXPR,pCI->G.p_type->pDupCardAndSubCards());
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS,pCI->G.p_stmCol->szValue());
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$K1"_Stz);
    }

    pCI->G.p_expr->Y_vSet(P_Injected);
    pCI->G.p_projection->vPush(pCI->G.p_expr);


    if (AL_PARTITION != pCI->G.e_Level) {
      pCI->G.p_projection->vBump_stmOdn_In_rpnCalc(pCI);
      pCI->G.p_key->Y_vCut(P_ProjBump);
      pCI->G.p_key->Y_vSet(P_ProjBumped);
    }

    if (pCI->G.p_stream = pCI->G.p_gbyAgg->pSub_stream()) {
      pCI->G.p_gbyAgg->vRemoveChild(pCI->G.p_stream);
      DELETE_CARD(pCI->G.p_stream);
    }
    pCI->G.p_stream = Card_Stream::pDerive_Of_projection(pCI->G.p_projection);
    pCI->G.p_gbyAgg->vAppend(pCI->G.p_stream);

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
void Board::vEnhance_sq_project_To_Include_Correlation_Column(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold sq <project>

  RASSERT (pCI->G.p_MainCard->bIsCard(CK_PROJECT));

  pCI->G.p_projection = pCI->G.p_project->pSub_projection();

  pCI->G.p_projection->Y_vSet(P_ProjBump,(i8)1);
  pCI->G.p_projection->Y_vSet(P_Restream);

  pCI->G.p_type = pCI->G.p_key->pGetChild(0)->pSub_type();

  // push <project>/__SQ1IGBKY1
  pCI->G_vPushNewChild(pCI->G.p_projection, CK_EXPR);
  pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS,pCI->G.p_stmCol->szValue());
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$1"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // replace <project>/<stream> now that __SQ1IGBKY1 has been added

  pCI->G.p_stream = pCI->G.p_project->pSub_stream();

  pCI->G.p_project->vRemoveChild(pCI->G.p_stream);
  DELETE_CARD(pCI->G.p_stream);

  pCI->G.p_stream = Card_Stream::pDerive_Of_projection(pCI->G.p_projection);
  pCI->G.p_project->vAppend(pCI->G.p_stream);

  pCI->G.p_projection->Y_vCut(P_Restream);
  pCI->G.p_projection->Y_vSet(P_Restreamed);

  i8 nStmCol = pCI->G.p_stream->pSub_stmColList()->nChildCards();
  pCI->G.p_stream->Y_vSet(P_OdnLo,(i8)1);
  pCI->G.p_stream->Y_vSet(P_OdnHi,(i8)nStmCol);

  // bump all $n in <project>/<projElist>/<expr>/<rpnCalc>
  //   now that __SQ1IGBKY1 has been added

  i8 nBumpBy = pCI->G.p_projection->Y_iGet(P_ProjBump);

  pCI->G.p_projection->vBump_stmOdn_In_rpnCalc(pCI);// TODO: ,nBumpBy);
  pCI->G.p_projection->Y_vSet(P_ProjBumped,(i8)nBumpBy);
  pCI->G.p_projection->Y_vCut(P_ProjBump);
}

/*----------------------------------------------------------------------------*/
/* find and mark call to the subquery referenced in pCI->G.p_boolCalc              */
/*----------------------------------------------------------------------------*/
bool Board::bFindAndMarkSubQryCall(CardIter* pCI)
{
  _static_

  bool bFoundCall = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_roPred = [pCI,&bFoundCall](Card* pCard)
  {
    pCI->G.p_roPred = _pCard_RoPred(pCard);

    if (pCI->G.p_roPred->bIsValue("exists"_Stz) || pCI->G.p_roPred->bIsValue("notExists"_Stz)) {
      pCI->m_pTerm_0 = _pCard_Term(pCI->G.p_roPred->pGet(0));
      pCI->m_sSqyArg = pCI->m_pTerm_0->szValue();
      pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
      pCI->m_sSqyNm = pCI->m_pTerm_1->szValue();

      bFoundCall = true;

      pCI->G.p_roPred->vMark1(MARK_AT);

      return C_CONTINUE_RECURSION;
    }

    if (pCI->G.p_roPred->bIsValue("in"_Stz) || pCI->G.p_roPred->bIsValue("notIn"_Stz)) {
      pCI->m_pTerm_0 = _pCard_Term(pCI->G.p_roPred->pGet(0));
      pCI->m_sSqyNm = pCI->m_pTerm_0->szValue();
      pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
      pCI->m_sSqyRsl = pCI->m_pTerm_1->szValue();
      pCI->m_pTerm_2 = _pCard_Term(pCI->G.p_roPred->pGet(2));
      pCI->m_sSqyArg = pCI->m_pTerm_2->szValue();

      bFoundCall = true;

      pCI->G.p_roPred->vMark1(MARK_AT);

      return C_CONTINUE_RECURSION;
    }

    // TODO: needs to be a list
    if (   pCI->G.p_roPred->bIsValue("<"_Stz)
           || pCI->G.p_roPred->bIsValue("<="_Stz)
           || pCI->G.p_roPred->bIsValue("=="_Stz)
           || pCI->G.p_roPred->bIsValue("!="_Stz)
           || pCI->G.p_roPred->bIsValue(">="_Stz)
           || pCI->G.p_roPred->bIsValue(">"_Stz)
        ) {

      if (3 == pCI->G.p_roPred->nChildCards()) {
        pCI->m_pTerm_0 = _pCard_Term(pCI->G.p_roPred->pGet(0));
        // TODO: this doesnt seem right
        pCI->m_sSqyRO = pCI->m_pTerm_0->szValue();

        pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
        pCI->m_sSqyArg = pCI->m_pTerm_1->szValue();

        pCI->m_pTerm_2 = _pCard_Term(pCI->G.p_roPred->pGet(2));
        pCI->m_sSqyNm = pCI->m_pTerm_2->szValue();
      }
      else {
        pCI->m_pTerm_0 = _pCard_Term(pCI->G.p_roPred->pGet(0));
        pCI->m_sSqyArg = pCI->m_pTerm_0->szValue();

        pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
        pCI->m_sSqyNm = pCI->m_pTerm_1->szValue();
      }

      bFoundCall = true;

      pCI->G.p_roPred->vMark1(MARK_AT);

      return C_CONTINUE_RECURSION;
    }

    NEVER_REACH

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  pCI->G.p_boolCalc->bApplyRecursive(CK_ROPRED, bLambdaFn_roPred);
  if (bFoundCall)
    return true;

  return bFoundCall;
}

/*----------------------------------------------------------------------------*/
/* fold in the fold expression                                                */
/*----------------------------------------------------------------------------*/
void Board::vFoldIn_foldExpr(CardIter* pCI)
{
  _static_

  while (pCI->G_bEachMain()) {

    if (CK_GBYAGG == pCI->G.e_MainCard) {

      pCI->G.p_key = pCI->G.p_gbyAgg->pSub_key();
      pCI->G.p_key->vBump_stmOdn_In_rpnCalc(pCI);

      pCI->G.p_projection = pCI->G.p_gbyAgg->pSub_projection();
      pCI->G.p_projection->vBump_keyOdn_In_rpnCalc(pCI);
      pCI->G.p_projection->vBump_stmOdn_In_rpnCalc(pCI);

      Card_FoldElist::vPush_foldExpr_Into_aggElist(pCI);
      Card_FoldElist::vPush_foldExpr_Into_gbyElist(pCI);
    }

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vIdentify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold identify the <table>/<filter>/<boolCalc>/<cmpPred> which is correlated by parameter

  auto bMarkCorrPred = [pCI](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_CMPPRED))
      return C_CONTINUE_RECURSION;

    bool bIsCorrPred = false;
//    CardIter xCI(p_pred);
//    while ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
//
//      // TODO: hack... %1 $n may not be in this order
//      if (Card::bIsParamOrdinal(pCI->G.p_term->szValue())) {
//        bIsCorrPred = true;
//        m_pTerm_corrPred = _pCard_Term(xCI.pPrevChild());
//        break;
//      }
//    }

    if ((pCI->m_pTerm_corrPred = static_cast<Card_CmpPred*>(p_pred)->pExtractArgTerm())) {
      bIsCorrPred = true;
    }

    if (bIsCorrPred) {
      p_pred->vMark1(MARK_AT);
      p_pred->Y_vSet(P_CorrPred);
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc = pCI->G.p_filter->pSub_boolCalc();
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));
  bool bIgnore = pCI->G.p_PRED->bPredRecursive(bMarkCorrPred);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vMove_corrPred_To_mq_table_As_bounds(CardIter* pCI, Card_BoolCalc *p_boolCalc)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vMove = [pCI](Card_PRED* p_pred, Card_BoolCalc *p_boolCalc,
                     auto&& vTwiddle /*oddity permits recursive lambda*/)
  {
    if (! p_pred->bIsPRED())
      return; /* this effectively declares twiddle as void */

    if (p_pred->bIsCard(CK_CMPPRED) && p_pred->Y_bHas(P_CorrPred)) {

      Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_pred);

      p_boolCalc->vRemovePred(p_pred);

      // todo: find table 'd'->'a' and add p_pred as bounds

      Card_Table* p_table = Card_Table::pResolve_table_By_tblSyn(pCI, 'a',pCI->G.i_MainCard);

      Card_Bounds* pBounds = Card_Bounds::pBounds_CreateFrom_cmpPred(p_cmpPred,'a');

      //p_table->vAppend(pBounds);
      Card_IndexList* p_indexList = p_table->pSub_indexList();
      Card_Index* p_index = p_table->pSub_index();
      p_index->vAppend(pBounds);

      DELETE_CARD(p_pred);
      return;

    }

    CardIter xCI(p_pred);
    while (Card* p_child = xCI.pEachChild()) {
      if (p_child->bIsPRED()) {
        vTwiddle(static_cast<Card_PRED *>(p_child), p_boolCalc, vTwiddle);
        continue;
      }
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_PRED* p_PRED = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  vMove(p_PRED, p_boolCalc, vMove);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vResolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(CardIter* pCI, Card_BoolCalc *p_boolCalc)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [p_boolCalc](Card_PRED* p_pred,
                               auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! p_pred->bIsPRED())
      return; /* this effectively declares resolve as void */

    if (p_pred->bIsCard(CK_CMPPRED)) {

      if (! p_pred->Y_bHas(P_CorrPred))
        return;

      CardIter xCI(p_pred);
      while (Card* p_child = xCI.pEachChild()) {
        if (p_child->bIsPRED()) {
          vResolve(static_cast<Card_PRED*>(p_child), vResolve);
          continue;
        }
        // TODO: test for: == $d.2 %1 => == $1 [TRANSLATE PARAM %1]

        if (Card::bIsParamOrdinal(p_child->szValue())) {
          //R_vUTE_Resolve_Param_Ordinal(_pCard_Term(p_child));
          p_child->vValue("$1"_Stz);  // TODO: hack!!
        }
        else {
          //p_child->vValue("$1"_Stz);
          char cTblSyn = Card::cTableColSyn(p_child->szValue());
          cTblSyn = 'a'; // TODO: hack!!
          p_child->vSetTableColSyn(cTblSyn);
        }
      }
      return;
    }
    else
    if (p_pred->bIsCard(CK_ROPRED)) {
      p_pred->vCard(CK_CMPPRED);
      CardIter xCI(p_pred);
      while (Card* p_child = xCI.pEachChild()) {
        if (p_child->bIsPRED()) {
          vResolve(static_cast<Card_PRED*>(p_child), vResolve);
          continue;
        }
        // TODO: test for: < $a.5 $b.1 __subquery2 => < $a.5 $2

        if (Card::bIsSubQueryName(p_child->szValue())) {
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
    if (p_pred->bIsCard(CK_BOLPRED)) {
      CardIter xCI(p_pred);
      while (Card* p_child = xCI.pEachChild()) {
        if (p_child->bIsPRED()) {
          vResolve(static_cast<Card_PRED *>(p_child), vResolve);
          continue;
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
  Card_Stream*     p_stream_prior     = p_main_prior->pSub_stream();
  Card_StmColList* p_stmColList_prior = p_stream_prior->pSub_stmColList();

  Card_Table*      p_table_curr     = p_main_curr->pSub_table();
  Card_IndexList*  p_indexList_curr = p_main_curr->pSub_indexList();
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vUnproject_foldExpr_After_having(CardIter* pCI)
{
  _static_


  if (! pCI->G_bResolveCardWithSub(CK_GBYAGG, CK_PROJECTION, pCI->G.i_MainCard))
    return;

  pCI->G_vNewMember(CK_PROJECT);
  pCI->G.pDeck->vAppend(pCI->G.p_project);

  pCI->G_vNewMemberOf(CK_LEVEL,""_Stz,CK_PROJECT);
  pCI->G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  pCI->G_vNewMemberOf(CK_PROJECTION,""_Stz,CK_PROJECT);

  i8 i_Col = 0;
  i8 nChildren_aggElist = pCI->G.p_projection->nChildCards();
  for (i8 i_expr_aggElist = 0;
       i_expr_aggElist < nChildren_aggElist;
       i_expr_aggElist++) {

    i_Col++;

    pCI->G.p_expr = _pCard_Expr(pCI->G.p_projection->pGetChild(i_expr_aggElist));
    pCI->G.p_as = pCI->G.p_expr->pSub_as();

    if (Card::bIsSqyColOrdinal(pCI->G.p_as->szValue()))
      continue;

    Card_Expr* p_expr_simple = Card_Expr::pProj_Simple_expr_Of(pCI->G.p_expr,i_Col);

    pCI->G_vAppendChild(CK_PROJECTION,p_expr_simple);
  }


  pCI->G.p_stream = Card_Stream::pDerive_Of_projection(pCI->G.p_projection);

  pCI->G.p_project->vAppend(pCI->G.p_stream);

  pCI->G_vNewMember(CK_COMMENT,"unproject of Sq1"_Stz);
  pCI->G.p_project->vAppend(pCI->G.p_comment);


}

/*----------------------------------------------------------------------------*/
/* relabel expressions                                                        */
/*----------------------------------------------------------------------------*/
void Board::vRelabelExpressions(CardIter* pCI)
{
  _static_

  i8 i_Expr;
  s32 sLabel;

  pCI->G_vRestart();
  while (pCI->G_bEachMainWith2Sub(CK_GBYAGG, CK_KEY, CK_PROJECTION)) {

    i_Expr = 0;
    CardIter xCI_g(pCI->G.p_key);
    while ((pCI->G.p_expr = _pCard_Expr(xCI_g.pEachChild()))) {
      sLabel.vSprint("$K%lld"_Stz,++i_Expr);
      pCI->G.p_expr->vValue(sLabel.szStz());
    }

    i_Expr = 0;
    CardIter xCI_p(pCI->G.p_projection);
    while ((pCI->G.p_expr = _pCard_Expr(xCI_p.pEachChild()))) {
      sLabel.vSprint("$%lld"_Stz,++i_Expr);
      pCI->G.p_expr->vValue(sLabel.szStz());
    }

  }

  i8 i_StmCol;

  pCI->G_vRestart();
  while (pCI->G_bEachMain()) {
    if (! (pCI->G.p_stream = pCI->G.p_MainCard->pSub_stream()))
      continue;

    i_StmCol = pCI->G.p_stream->Y_iGet(P_OdnLo);

    pCI->G.p_stmColList = pCI->G.p_stream->pSub_stmColList();

    CardIter xCI_s(pCI->G.p_stmColList);
    while ((pCI->G.p_stmCol = _pCard_StmCol(xCI_s.pEachChild()))) {
      sLabel.vSprint("$%lld"_Stz,i_StmCol++);
      pCI->G.p_stmCol->vValue(sLabel.szStz());
    }

  }

}

/*----------------------------------------------------------------------------*/
/* visit each stream and adjust it's lo-hi range                              */
/*----------------------------------------------------------------------------*/
void Board::vRenumberStmOdn(CardIter* pCI)
{
  _static_

  // TODO: G_bEachSub(CK_STREAM)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card *pCard) {

    pCI->G.p_stmCol    = _pCard_StmCol(pCard);
    pCI->G.p_stmColOdn = pCI->G.p_stmCol->pSub_stmColOdn();

    if (! pCI->G.p_stmColOdn)
      return C_CONTINUE_RECURSION;

    pCI->G.p_stmColOdn->vSetStreamColOrdinal(pCI->G.i_stmColOdn++);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G_vRestart();
  while (pCI->G_bEachMain()) {

    if (! (pCI->G.p_MainCard->bIsCard(CK_TABLE) || pCI->G.p_MainCard->bIsCard(CK_PROJECT)))
      continue;

    pCI->G.p_stream = pCI->G.p_MainCard->pSub_stream();

    pCI->G.i_stmColOdn = pCI->G.p_stream->Y_iGet(P_OdnLo);

    pCI->G.p_stmColList = pCI->G.p_stream->pSub_stmColList();

    pCI->G.p_stmColList->bApplyRecursive(CK_STMCOL,bLambdaFn);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_26_SubQuery_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vS26_T10d_Main_Query_Analysis_Of_where_rpnCalc(CardIter* pCI)
{
  _static_

  SubQuery* pSubQuery = pCI->m_pBench->pGetBoard((i8)1)->pSubQuery();

  if (pSubQuery->bViaFrom())
    return;

  pSubQuery->vCaller(0);

  // there is a subQuery, how is it called?

  pSubQuery->vClause(QC_WHERE_CLAUSE);

  CardIter xCI(pCI->G.p_rpnCalc);
  while ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szValue_term = pCI->G.p_term->szValue();

    if (!pCI->G_bIsBoard(szValue_term))
      continue;

    if (! (pCI->G.p_term = _pCard_Term(xCI.pEachChild())))
      break;

    szValue_term = pCI->G.p_term->szValue();

    if (szValue_term->bSame("exists"_Stz)) {
      if ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = pCI->G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_EXISTS_OP);
        else
          pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
      break;
    }
    else
    if (szValue_term->bSame("in"_Stz)) {
      if ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = pCI->G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_IN_OP);
        else
          pSubQuery->vCalcOp(QO_IN_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_IN_OP);
      }
      break;
    }
    else
    if (Card::bIsResultOrdinal(szValue_term)) {
      if ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = pCI->G.p_term->szValue();
        if (Card::bIsStreamColOrdinal(szValue_term)) {
          if ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
            szValue_term = pCI->G.p_term->szValue();
            if (szValue_term->bSame("in"_Stz)) {
              if ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
                szValue_term = pCI->G.p_term->szValue();
                if (szValue_term->bSame("not"_Stz))
                  pSubQuery->vCalcOp(QO_NOT_IN_OP);
                else
                  pSubQuery->vCalcOp(QO_IN_OP);
              }
              else {
                pSubQuery->vCalcOp(QO_IN_OP);
              }
            }
          }
        }
      }
      break;
    }
    else
    {
      eRO eRelnOp = RelOp::eDecode(szValue_term);
      if (RO_NONE != eRelnOp) {
        pSubQuery->vCalcOp(QO_COMPARE_OP);
        pSubQuery->vRelnOp(eRelnOp);
      }
      break;
    }

  }

  SubQuery* pMainQuery = pCI->m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vS26_T10e_Main_Query_Analysis_Of_having_rpnCalc(CardIter* pCI)
{
  _static_

  SubQuery* pSubQuery = pCI->m_pBench->pGetBoard((i8)1)->pSubQuery();

  pSubQuery->vCaller(0);

  // there is a subQuery, how is it called?

  pSubQuery->vClause(QC_HAVING_CLAUSE);

  CardIter xCI(pCI->G.p_rpnCalc);
  while ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szValue_term = pCI->G.p_term->szValue();

    if (!pCI->G_bIsBoard(szValue_term))
      continue;

    if (! (pCI->G.p_term = _pCard_Term(xCI.pEachChild())))
      break;

    szValue_term = pCI->G.p_term->szValue();

    if (szValue_term->bSame("exists"_Stz)) {
      if ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = pCI->G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_EXISTS_OP);
        else
          pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
    }
    else
    if (szValue_term->bSame("in"_Stz)) {
      if ((pCI->G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = pCI->G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_IN_OP);
        else
          pSubQuery->vCalcOp(QO_IN_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_IN_OP);
      }
    }
    else
    {
      eRO eRelnOp = RelOp::eDecode(szValue_term);
      if (RO_NONE != eRelnOp) {
        pSubQuery->vCalcOp(QO_COMPARE_OP);
        pSubQuery->vRelnOp(eRelnOp);
      }
    }

    break;
  }

  SubQuery* pMainQuery = pCI->m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_33_To_TblColOdn */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vConvert_stmColOdn_To_tblColOdn(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);
    if (! Card::bIsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Stz* szStmColOdn = p_term->szValue();
    if (! pCI->G_bResolveSubCard(CK_STREAM,CK_STMCOLODN,szStmColOdn,pCI->G.i_MainCard))
      return C_CONTINUE_RECURSION;

    RASSERT(pCI->m_pCard_parent->bIsCard(CK_STMCOL));

    pCI->G.p_tblColOdn = pCI->m_pCard_parent->pSub_tblColOdn();
    p_term->vSetValueMark1(pCI->G.p_tblColOdn->szValue(), MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_MainCard->bApplyRecursive(CK_TERM, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_36_Bound_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vS36_T30_Delete_Empty_filter(CardIter* pCI)
{
  _static_

  if (! pCI->G.p_filter->bIsEmpty())
    return;

  pCI->G.p_MainCard->vRemoveChild(pCI->G.p_filter);
  DELETE_CARD(pCI->G.p_filter);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vS75_T10_Morph_table_To_EEop_v2(CardIter* pCI)
{
  _static_

  Card* pMain = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SCAN
  if (0 == pCI->G.i_MainCard)  {
    pCI->G.p_scan = new Card_Scan;
    pCI->G.p_scan->vTransferChildren(pCI->G.p_table);

    pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table,pCI->G.p_scan);
    pCI->G.p_scan->vInsert(pCI->G.p_table);

    if (pCI->G.p_indexJoin) {
      NEVER_REACH
      pCI->G.p_scan->vRemoveChild(pCI->G.p_indexJoin);
      DELETE_CARD(pCI->G.p_indexJoin);
    }

    pMain = pCI->G.p_scan;
  }
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // KEYEXISTS
  else if (pCI->G.p_keyExists) {
    pCI->G.p_index->vSet_Choose_index();

    pCI->G.p_table->vRemoveSubCard(pCI->G.p_keyExists);

    pCI->G.p_keyExists->vTransferChildren(pCI->G.p_table);

    pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table, pCI->G.p_keyExists);
    pCI->G.p_keyExists->vInsert(pCI->G.p_table);

    pCI->G.p_keyExists->vRemoveSubCard(pCI->G.p_index);
    pCI->G.p_keyExists->vAppend(pCI->G.p_index);

    pMain = pCI->G.p_keyExists;
  }
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // KEYNOTEXISTS
  else if (pCI->G.p_keyNotExists) {
    pCI->G.p_index->vSet_Choose_index();

    pCI->G.p_table->vRemoveSubCard(pCI->G.p_keyNotExists);

    pCI->G.p_keyNotExists->vTransferChildren(pCI->G.p_table);

    pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table, pCI->G.p_keyNotExists);
    pCI->G.p_keyNotExists->vInsert(pCI->G.p_table);

    pCI->G.p_keyNotExists->vRemoveSubCard(pCI->G.p_index);
    pCI->G.p_keyNotExists->vAppend(pCI->G.p_index);

    pMain = pCI->G.p_keyNotExists;
  }
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // CROSSJOIN
//  else if (! pCI->G.p_bounds) {
//    Board::vChoose_index_For_crossJoin(static_cast<CardIter*>(this));
//
//    pCI->G.p_crossJoin = new Card_CrossJoin;
//    pCI->G.p_crossJoin->vTransferChildren(pCI->G.p_table);
//
//    pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table,pCI->G.p_crossJoin);
//    pCI->G.p_crossJoin->vInsert(pCI->G.p_table);
//
//    G_vNewMember(CK_BOUNDS);
//    pCI->G.p_crossJoin->vAppend(pCI->G.p_bounds);
//
//    G_vNewMember(CK_XBOUND);
//    pCI->G.p_bounds->vAppend(pCI->G.p_xBound);
//
//    G_vNewMember(CK_TERM,"bound"_Stz);
//    pCI->G.p_xBound->vAppend(pCI->G.p_term);
//
//    G_vNewMember(CK_TERM,"$I1"_Stz);
//    pCI->G.p_xBound->vAppend(pCI->G.p_term);
//
//    pMain = pCI->G.p_crossJoin;
//  }
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // INDEXJOIN
  else {

    if (! pCI->G.p_bounds) {
      s256 sErrorMsg;
      sErrorMsg.vSprint("No index for table(%c) %s"_Stz,
                        pCI->G.p_table->cValue(),
                        pCI->G.p_table->pSub_tableNm()->szValue());
      Error xError(EC_MISSING_JOIN_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
      throw xError;
    }

    pCI->G.p_indexList->vChoose_index_For_indexJoin();

    pCI->G_vNewMember(CK_INDEXJOIN);

    pCI->G.p_indexJoin->vTransferChildren(pCI->G.p_table);

    pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table, pCI->G.p_indexJoin);
    pCI->G.p_indexJoin->vInsert(pCI->G.p_table);

    //pCI->G.p_indexJoin->vRemoveSubCard(pCI->G.p_index);
    //pCI->G.p_indexJoin->vAppend(pCI->G.p_index);

    pMain = pCI->G.p_indexJoin;
  }

  if ((pCI->G.p_tableNm = pMain->pSub_tableNm())) {
    pMain->vRemoveSubCard(pCI->G.p_tableNm);
    pCI->G.p_table->vAppend(pCI->G.p_tableNm);
  }
  if ((pCI->G.p_tablePtn = pMain->pSub_tablePtn())) {
    pMain->vRemoveSubCard(pCI->G.p_tablePtn);
    pCI->G.p_table->vAppend(pCI->G.p_tablePtn);
  }
  if ((pCI->G.p_size = pMain->pSub_size())) {
    pMain->vRemoveSubCard(pCI->G.p_size);
    pCI->G.p_table->vAppend(pCI->G.p_size);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vS75_T30_Correct_stmColOdn_Of_keyExists_stream(CardIter* pCI)
{
  _static_

  i8 iOrdLo_stream = pCI->G.p_stream->iGetOdnLow();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [iOrdLo_stream](Card* pCard)
  {
    Card_StmColOdn* p_stmColOrd = _pCard_StmColOdn(pCard);
    if (! Card::bIsStreamColOrdinal(p_stmColOrd->szValue()))
      return C_CONTINUE_RECURSION;

    i8 iStmColOdn = Card::iGetColOdn(p_stmColOrd->szValue());
    i8 iStmColOdn_corrected = iOrdLo_stream + iStmColOdn - 1;
    p_stmColOrd->vSetStreamColOrdinal(iStmColOdn_corrected);

    p_stmColOrd->vMark1(MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_stream->bApplyRecursive(CK_STMCOLODN, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(CardIter* pCI)
{
  _static_

  i8 i_Main = pCI->G.pDeck->iFindChild(pCI->G.p_MainCard);

  Card_BoolCalc* p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,i_Main](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! Card::bIsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    pCI->G_bResolveSubCard(CK_STMCOL, CK_STMCOLODN, p_term->szValue(), i_Main + 1);

    Card_StmColNm* p_stmColNm = pCI->G.p_stmCol->pSub_stmColNm();

    p_term->vSetValueMark1(p_stmColNm->szValue(), MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_A0_Make_Weavable */
/*============================================================================*/

void Board::vSA0_T10_Individuate_indexJoin_filter_b(CardIter* pCI)
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
void Board::vSA0_T10c_Individuate_crossJoin_filter(CardIter* pCI)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Board::vSZ0_T20_Reinstate_stagesV3(CardIter* pCI)
{
  _static_

  i8 nStages = 0;

  Card_Stage* p_stage = new Card_Stage;
  p_stage->vValueSprint("S%02lld"_Stz,++nStages);
  Card_Topology* p_topology = new Card_Topology(YG_ALL_PARTITIONS);
  p_stage->vAppend(p_topology);
  pCI->G.pDeck->vPush(p_stage);

  eAL e_level_running = Card::eStageTopologyToAggrLevel(YG_ALL_PARTITIONS);

  bool bSawAggOp = false;
  bool bSawIndexJoinOp = false;
  bool bSawProjectOp = false;
  bool bSawSortOp = false;

  auto vAddCombined = [pCI] (Card* p_main, i8& ri_main) {
    Card_Combined* p_combined = new Card_Combined;
    if (C_UNKNOWN == ri_main) {
      pCI->G.pDeck->vAppend(p_combined);
      return;
    }
    pCI->G.pDeck->vInsertBefore(p_combined, p_main);
    ri_main++;
    RASSERT(pCI->G.pDeck->pGetChild(ri_main) == p_main);
  };

  auto vAddNext = [pCI] (Card* p_main, i8& ri_main, Stz* szNextValue) {
    Card_Next* p_next = new Card_Next;
    p_next->vValue(szNextValue);
    if (C_UNKNOWN == ri_main) {
      pCI->G.pDeck->vAppend(p_next);
      return;
    }
    pCI->G.pDeck->vInsertBefore(p_next, p_main);
    ri_main++;
    RASSERT(pCI->G.pDeck->pGetChild(ri_main) == p_main);
  };

  auto vAddStage = [pCI] (Card* p_main, i8& ri_main, eAL e_level_running, i8& nStages) {
    Card_Stage *p_stage = new Card_Stage;
    p_stage->vValueSprint("S%02lld"_Stz,++nStages);
    eYG eStageTopo = Card::eAggrLevelToStageTopology(e_level_running);
    Card_Topology *p_topology = new Card_Topology(eStageTopo);
    p_stage->vAppend(p_topology);
    pCI->G.pDeck->vInsertBefore(p_stage, p_main);
    ri_main++;
    RASSERT(pCI->G.pDeck->pGetChild(ri_main) == p_main);
  };

  pCI->G_vRestart();
  if (pCI->G_bEachMain(CK_FOLD))
    pCI->G_vRemoveDeleteMain(CK_FOLD);

  Card* p_prev = nullptr;
  eCK   e_prev = CK_NONE;
  Card* p_main = p_stage;
  eCK   e_main = p_main->eCard();

  for (i8 i_Main = 1; i_Main < pCI->G.pDeck->nChildCards(); i_Main++) {

    p_prev = p_main;
    e_prev = e_main;

    p_main = pCI->G.pDeck->pGetChild(i_Main);
    e_main = p_main->eCard();
    eAL e_level_main = (eAL) p_main->pSub_level()->Y_iGet(P_Level);

    // change of level
    if (e_level_main != e_level_running) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      if (e_level_running == AL_ORIGIN &&
          e_level_main == AL_PARTITION)
        vAddNext(p_main, i_Main,"broadcast"_Stz);
      else
        vAddNext(p_main, i_Main,""_Stz);
      e_level_running = e_level_main;
      vAddStage(p_main, i_Main, e_level_running,nStages);
      if (CK_GBYAGG == e_main || CK_AGGREG == e_main || CK_ORDAGG == e_main)
        bSawAggOp = true;
      else
        bSawAggOp = false;
      if (CK_PROJECT == e_main)
        bSawProjectOp = true;
      else
        bSawProjectOp = false;
      continue;
    }

    if (CK_CROSSJOIN == e_main || CK_INDEXJOIN == e_main
        || CK_KEYEXISTS == e_main
        || CK_KEYNOTEXISTS == e_main) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = true;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if ((CK_PROJECT == e_main || CK_AGGREG == e_main)
        && (bSawAggOp || bSawIndexJoinOp || bSawProjectOp || bSawSortOp) ) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = true;
      bSawSortOp = false;
      continue;
    }

    if (CK_AGGREG == e_main && (bSawAggOp || bSawIndexJoinOp || bSawProjectOp)) {
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if ((CK_GBYAGG == e_main || CK_ORDAGG == e_main) &&
        (bSawAggOp || bSawIndexJoinOp || bSawProjectOp)) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if (CK_SORT == e_main) {
      if (bSawAggOp || bSawIndexJoinOp || bSawProjectOp || bSawSortOp) {
        vAddCombined(p_main, i_Main);
      }

      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = true;
      continue;
    }

    // if filter then continue?
    if (CK_FILTER == e_main) {
      continue;
    }

    if (bSawAggOp || bSawIndexJoinOp) {
      vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if (CK_GBYAGG == e_main|| CK_ORDAGG == e_main)
      bSawAggOp = true;
    if (CK_PROJECT == e_main || CK_AGGREG == e_main)
      bSawProjectOp = true;
  }

  e_prev = e_main;

  i8 i_End = C_UNKNOWN;
  if (CK_SCAN != e_prev)
    vAddCombined(p_main, i_End);
  vAddNext(p_main, i_End,""_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get board call                                                             */
/*----------------------------------------------------------------------------*/
eBC Board::eBoardCall(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eBoardCall;
}

/*----------------------------------------------------------------------------*/
/* set board call                                                             */
/*----------------------------------------------------------------------------*/
void Board::vBoardCall(eBC eBoardCall)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_eBoardCall = eBoardCall;
}

/*----------------------------------------------------------------------------*/
/* get board form                                                             */
/*----------------------------------------------------------------------------*/
eBF Board::eBoardForm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eBoardForm;
}

/*----------------------------------------------------------------------------*/
/* set board form                                                             */
/*----------------------------------------------------------------------------*/
void Board::vBoardForm(eBF eBoardForm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_eBoardForm = eBoardForm;
}

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
/* get board role                                                             */
/*----------------------------------------------------------------------------*/
eBR Board::eBoardRole(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eBoardRole;
}

/*----------------------------------------------------------------------------*/
/* set board role                                                             */
/*----------------------------------------------------------------------------*/
void Board::vBoardRole(eBR eBoardRole)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_eBoardRole = eBoardRole;
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
/* get may have stmColOdn                                                     */
/*----------------------------------------------------------------------------*/
bool Board::bMayHaveStmColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_bMayHaveStmColOdn;
}

/*----------------------------------------------------------------------------*/
/* set may have stmColOdn                                                     */
/*----------------------------------------------------------------------------*/
void Board::vMayHaveStmColOdn(bool bMayHaveStmColOdn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_bMayHaveStmColOdn = bMayHaveStmColOdn;
}

/*----------------------------------------------------------------------------*/
/* pin board onto given ribbon                                                */
/*----------------------------------------------------------------------------*/
void Board::vPinBanners(Ribbon* pRibbon)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  pRibbon->vDrawH('-',true);

  if (m_bMayHaveStmColOdn)
    pRibbon->vDrawH('|');
  else
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

  CardCtl* pCardCtl = m_pOptimiser->pCardCtl();

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

  CardCtl* pCardCtl = m_pOptimiser->pCardCtl();

  if (! m_pCard_Deck->nItems())
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // line 1
  pRibbon->vSprint("Transform: %02lld Board %s Query %s :: %s %s %s"_Stz,
                   m_iXform,
                   m_sBoardNm.szStz(),
                   m_pOptimiser->szQueryNm(),
                   aBR[m_eBoardRole],
                   aBF[m_eBoardForm],
                   aBC[m_eBoardCall]
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

                     m_pSubQuery->i_SubQuery(),
                     aQC[m_pSubQuery->eClause()],
                     aQO[m_pSubQuery->eCalcOp()],
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

  Ribbon* pRibbon = new Ribbon(m_pOptimiser->pCardCtl(),this);

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
/* get sub-query kind                                                         */
/*----------------------------------------------------------------------------*/
eQS Board::eSubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  return m_eSubQuery;
}

/*----------------------------------------------------------------------------*/
/* set sub-query kind                                                         */
/*----------------------------------------------------------------------------*/
void Board::vSubQuery(eQS eSubQuery)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BOARD,_FOOTER_);

  m_eSubQuery = eSubQuery;
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
