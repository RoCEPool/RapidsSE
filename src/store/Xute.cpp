/*============================================================================*/
/* Xute.cpp --                                                                */
/*============================================================================*/

#include "Xute.hpp"
#include "Xgearing.hpp"
#include "Table.hpp"
#include "PtnDef.hpp"

#include "RelOp.hpp"
#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xute::Xute(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xute::~Xute(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vBump_keyOdn_In_rpnCalc_Of_Elist(CardIter* pCardIter,
                                            Card_ELIST* p_ELIST)
{
  _static_

  CardIter xCI(p_ELIST);
  while ((pCardIter->G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    pCardIter->G.p_rpnCalc = pCardIter->G.p_expr->pSub_rpnCalc();

    if (! pCardIter->G.p_rpnCalc)
      continue;

    CardIter xCI2(pCardIter->G.p_rpnCalc);
    while ((pCardIter->G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = pCardIter->G.p_term->szValue();

      if (Card::bIsKeyOrdinal(szTerm)) {
        pCardIter->G.p_term->vBump_KeyOrdinal();
        pCardIter->G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vBump_stmOdn_In_rpnCalc_Of_Elist(CardIter* pCardIter,
                                            Card_ELIST* p_ELIST)
{
  _static_

  CardIter xCI(p_ELIST);
  while ((pCardIter->G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    pCardIter->G.p_rpnCalc =pCardIter->G.p_expr->pSub_rpnCalc();

    if (! pCardIter->G.p_rpnCalc)
      continue;

    CardIter xCI2(pCardIter->G.p_rpnCalc);
    while ((pCardIter->G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = pCardIter->G.p_term->szValue();

      if (Card::bIsStreamColOrdinal(szTerm)) {
        pCardIter->G.p_term->vBump_StreamColOrdinal();
        pCardIter->G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vChoose_index_For_crossJoin(CardIter* pCardIter)
{
  _static_

  if (! pCardIter->G.p_index) {
    s256 sErrorMsg;
    sErrorMsg.vSprint("No index for table %s"_Stz,pCardIter->G.p_table->pSub_tableNm()->szValue());
    Error xError(EC_MISSING_JOIN_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

  pCardIter->G.p_index->Y_vSet(P_ChosenIndex);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vChoose_index_For_indexJoin(CardIter* pCardIter)
{
  _static_

  CardIter xCI(pCardIter->G.p_indexList);

  while (Card_Index* p_index = _pCard_Index(xCI.pEachChild(CK_INDEX))) {
    if (p_index->pSub_eBound()) {
      p_index->Y_vSet(P_ChosenIndex);
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vChoose_index_For_indexScan(CardIter* pCardIter)
{
  _static_

  CardIter xCI(pCardIter->G.p_indexList);

  while (Card_Index* p_index = _pCard_Index(xCI.pEachChild(CK_INDEX))) {
    if (p_index->pSub_bounds()) {
      p_index->Y_vSet(P_ChosenIndex);
      break;
    }
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vChoose_index_For_keyExists(CardIter* pCardIter)
{
  _static_

  pCardIter->G.p_index->Y_vSet(P_ChosenIndex);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vChoose_index_For_keyNotExists(CardIter* pCardIter)
{
  _static_

  pCardIter->G.p_index->Y_vSet(P_ChosenIndex);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vDerive_stream_order_For_indexJoin(CardIter* pCardIter)
{
  _static_

  RASSERT(pCardIter->G.p_stmColList);
  Card_StmColList* p_stmColList_l = pCardIter->G.p_stmColList;
  RASSERT(pCardIter->G.p_order);
  Card_Order* p_stmOrdList_l = pCardIter->G.p_order;

  RASSERT(pCardIter->G.p_indexList);
  RASSERT(pCardIter->G.p_stream);

  pCardIter->G.p_index = pCardIter->G.p_indexList->pFindChosenIndex();
  if (! pCardIter->G.p_index)
    return;

  pCardIter->G.p_bounds = pCardIter->G.p_index->pSub_bounds();
  if (! pCardIter->G.p_bounds)
    return;

  pCardIter->G.p_eBound = pCardIter->G.p_bounds->pSub_eBound();
  if (! pCardIter->G.p_eBound)
    return;

  // get tblColOdn of eBound
  Stz* szTblColOdn_r = pCardIter->G.p_eBound->szGet_tblColOdn();
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
  pCardIter->G.p_stream->vAppend(p_stmOrdList_ij);

  pCardIter->G.p_ordElmOdn =
      _pCard_OrdElmOdn(p_stmOrdList_ij->pAppendNewCardTail(CK_ORDELMODN,
                                                           p_stmOrdOdn_l->szValue()));
  pCardIter->G.p_ordElmOdn->vAppendNewCardTail(CK_DIRECTION,"ASC"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vDerive_stream_order_For_sort(CardIter* pCardIter)
{
  _static_

  pCardIter->G.p_stream = pCardIter->G.p_sort->pSub_stream();
  pCardIter->G.p_order = _pCard_Order(pCardIter->G.p_stream->pAppendNewCardTail(CK_ORDER));

  CardIter xCI(pCardIter->G.p_sbyElist);
  while ((pCardIter->G.p_expr = _pCard_Expr(xCI.pEachChild(CK_EXPR)))) {
    pCardIter->G.p_rpnCalc = pCardIter->G.p_expr->pSub_rpnCalc();

    if (! (pCardIter->G.p_term = pCardIter->G.p_rpnCalc->pGetSingle_stmColOdn()))
      break;

    Stz* szStmColOdn = pCardIter->G.p_term->szValue();
    Stz* szStmOrdOdn = szStmColOdn;
    pCardIter->G.p_ordElmOdn = _pCard_OrdElmOdn(pCardIter->G.p_order->pAppendNewCardTail(CK_ORDELMODN, szStmOrdOdn));

    if ((pCardIter->G.p_direction = pCardIter->G.p_expr->pSub_direction()))
      pCardIter->G.p_ordElmOdn->vAppend(pCardIter->G.p_direction->pDupCardAndSubCards());
    else
      pCardIter->G.p_ordElmOdn->vAppendNewCardTail(CK_DIRECTION,"ASC"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vIndividuate_filter(CardIter* pCardIter)
{
  _static_

  pCardIter->G.p_MainCard->vRemoveChild(pCardIter->G.p_filter);
  pCardIter->G.pDeck->vInsertAfter(pCardIter->G.p_filter, pCardIter->G.p_MainCard);

  if (! pCardIter->G.p_filter->pSub_level()) {
    pCardIter->G.p_filter->vPush(pCardIter->G.p_MainCard->pSub_level()->pDupCardAndSubCards());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xute::bOrder_Joins(CardIter* pCardIter)
{
  _static_

  CardIter xCI(pCardIter->G.pDeck);
  if (xCI.pEachChild(CK_TABLE) && xCI.pEachChild(CK_TABLE))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* find sub card with given value and replace                                 */
/*----------------------------------------------------------------------------*/
bool Xute::bReplaceValueOfSub(CardIter* pCardIter, Card* pCard, eCK eSub,
                              Stz* szValue, Stz* szWith)
{
  _static_

  bool bReplaced = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCardIter, szValue, szWith, &bReplaced](Card *pCard)
  {
    pCardIter->G.p_term = _pCard_Term(pCard);

    if (! pCardIter->G.p_term->bIsValue(szValue))
      return C_CONTINUE_RECURSION;

    pCardIter->G.p_term->vValue(szWith);
    bReplaced = true;

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCard->bApplyRecursive(eSub, bLambdaFn);

  return bReplaced;
}

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Xute::vPush_Correlated_cmpPred_Onto_mq_boolCalc(Card_BoolCalc *p_boolCalc,
                                                     STORE::Card_PRED *p_PRED)
{
  _static_

  if (p_boolCalc->nChildCards()) {
    p_boolCalc->vPushPred(p_PRED);
  }
  else {
    p_boolCalc->vAppend(p_PRED);
  }
}



