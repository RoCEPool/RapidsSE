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
void Xute::vAdd_where(CardIter* pCI, Card* p_after)
{
  _static_

  pCI->G_vNewMember(CK_WHERE);

  pCI->G.pDeck->vInsertAfter(pCI->G.p_where,p_after);

  pCI->G_vAppendNewChild(pCI->G.p_where,CK_RPNCALC);
}

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Xute::vAddPredToTableFilter(CardIter* pCI, Card_Table* p_table, Card_PRED* p_PRED)
{
  _static_

  if ((pCI->G.p_filter = p_table->pSub_filter())) {
    pCI->G.p_boolCalc = pCI->G.p_filter->pSub_boolCalc();
    pCI->G.p_boolCalc->vPushPred(p_PRED);
  }
  else {
    pCI->G_vAppendNewChild(p_table,CK_FILTER);
    pCI->G_vAppendNewChild(pCI->G.p_filter,CK_BOOLCALC);
    pCI->G_vAppendChild(CK_BOOLCALC,p_PRED);
  }
}

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
void Xute::vBump_where_BaseOn_foldExpr(CardIter* pCI)
{
  _static_

  pCI->G.p_boolCalc = pCI->G.p_where->pSub_boolCalc();

  i8 nBumpRight = pCI->G.p_fold->Y_iGet(P_BumpRight);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,nBumpRight](Card *pCard) {

    if (MARK_AT != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    pCI->G.p_roPred = _pCard_RoPred(pCard);

//    m_pTerm_0 = _pCard_Term(pCI->G.p_roPred->pGet(0));
//    m_pTerm_0->vBump_StreamColOrdinal(nBumpRight);

    pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
    pCI->G.p_roPred->vRemoveChild(pCI->m_pTerm_1);
    DELETE_CARD(pCI->m_pTerm_1);

    pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
    pCI->G.p_roPred->vRemoveChild(pCI->m_pTerm_1);
    DELETE_CARD(pCI->m_pTerm_1);

    pCI->G_vAppendNewChild(pCI->G.p_roPred, CK_TERM, "$2"_Stz);

    pCI->G_vMorphCard(pCI->G.p_roPred, CK_CMPPRED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc->bApplyRecursive(CK_ROPRED, bLambdaFn);

  pCI->G.p_fold->Y_vSet(P_BumpedRight,nBumpRight);
  pCI->G.p_fold->Y_vCut(P_BumpRight);
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
void Xute::vDerive_foldElist_From_project_stream(CardIter* pCI)
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
void Xute::vDerive_group_order_For_gbyAgg(CardIter* pCI)
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

  pCI->G.p_gbyElist = pCI->G.p_gbyAgg->pSub_gbyElist();

  CardIter xCI(pCI->G.p_gbyElist);
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
void Xute::vDerive_stream_order_For_gbyAgg(CardIter* pCI)
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

  pCI->G.p_aggElist =  pCI->G.p_gbyAgg->pSub_aggElist();
  CardIter xCI(pCI->G.p_aggElist);
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
void Xute::vDerive_stream_order_For_indexScan(CardIter* pCI)
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
void Xute::vDerive_stream_order_For_project(CardIter* pCI)
{
  _static_

  pCI->G.p_projElist = pCI->G.p_project->pSub_projElist();
  pCI->G.p_stream = pCI->G.p_project->pSub_stream();
  pCI->G.p_stmColList = pCI->G.p_stream->pSub_stmColList();

  Card_Order* p_stmOrdList_project = nullptr;

  i8 nLeading = 0;
  CardIter xCI(pCI->G.p_order);
  while ((pCI->G.p_ordElmOdn = _pCard_OrdElmOdn(xCI.pEachChild()))) {
    i8 i_Expr = pCI->G.p_projElist->iRpnCalc_Has_stmOrdOdn(pCI->G.p_ordElmOdn);
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
void Xute::vDerive_order(CardIter* pCI)
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
      Xute::vDerive_stream_order_For_indexJoin(pCI);
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
  while (pCI->G_bEachMainWithSub(CK_SORT,CK_SBYELIST)) {
    Xute::vDerive_stream_order_For_sort(pCI);
  }

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vDerive_foldExpr(CardIter* pCI)
{
  _static_

  if (! pCI->G_bResolveSubCard(CK_PROJELIST,pCI->G.i_FoldCard))
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
void Xute::vMove_on_To_where(CardIter* pCI)
{
  _static_

  Card_RpnCalc* p_rpnCalc_on = pCI->G.p_on->pSub_rpnCalc();
  if (! p_rpnCalc_on->nChildCards())
    return;
  Card_Term* p_term_0 = _pCard_Term(p_rpnCalc_on->pGetChild(0));
  if (p_term_0->bSame("CROSS"_Stz))
    return;

  if (! pCI->G.p_where) {
    Xute::vAdd_where(pCI, pCI->G.p_absJoin);
  }

  pCI->G.p_absJoin->vRemoveChild(pCI->G.p_on);

  pCI->G.p_rpnCalc = pCI->G.p_where->pSub_rpnCalc();

  pCI->G.p_rpnCalc->vAppendRpnCalc(p_rpnCalc_on);

  DELETE_CARD(pCI->G.p_on);
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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_36_Bound_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vS36_T30_Delete_Empty_filter(CardIter* pCardIter)
{
  _static_

  if (! pCardIter->G.p_filter->bIsEmpty())
    return;

  pCardIter->G.p_MainCard->vRemoveChild(pCardIter->G.p_filter);
  DELETE_CARD(pCardIter->G.p_filter);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vS75_T30_Correct_stmColOdn_Of_keyExists_stream(CardIter* pCardIter)
{
  _static_

  i8 iOrdLo_stream = pCardIter->G.p_stream->iGetOdnLow();

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

  pCardIter->G.p_stream->bApplyRecursive(CK_STMCOLODN, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_A0_Make_Weavable */
/*============================================================================*/

void Xute::vSA0_T10_Individuate_indexJoin_filter_b(CardIter* pCardIter)
{
  _static_

  // todo: xt

  pCardIter->G.p_indexJoin->vRemoveChild(pCardIter->G.p_filter);
  pCardIter->G.pDeck->vInsertAfter(pCardIter->G.p_filter,
                                   pCardIter->G.p_indexJoin);

  if (! pCardIter->G.p_filter->pSub_level()) {
    pCardIter->G.p_filter->vPush(pCardIter->G.p_indexJoin->pSub_level()->pDupCardAndSubCards());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vSA0_T10c_Individuate_crossJoin_filter(CardIter* pCardIter)
{
  _static_

  // todo: xt

  pCardIter->G.p_crossJoin->vRemoveChild(pCardIter->G.p_filter);
  pCardIter->G.pDeck->vInsertAfter(pCardIter->G.p_filter,
                                   pCardIter->G.p_crossJoin);

  if (! pCardIter->G.p_filter->pSub_level()) {
    pCardIter->G.p_filter->vPush(pCardIter->G.p_crossJoin->pSub_level()->pDupCardAndSubCards());
  }
}

