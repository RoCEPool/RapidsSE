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
void Xute::vConvert_tblColOdn_Of_constraint_To_stmColNm(CardIter* pCI)
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
void Xute::vConvert_tblColOdn_To_stmOdn(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    //vTrace("CONVERT %s\n"_Stz,p_term->szValue());

    if (! Card::bIsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    pCI->G.p_stmColOdn = pResolve_tblColOdn_Into_stmColOdn(pCI, p_term,pCI->G.i_MainCard+1);

    p_term->vSetValueMark1(pCI->G.p_stmColOdn->szValue(), MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_MainCard->bApplyRecursive(CK_TERM, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vCorrect_having_BaseOn_foldExpr(CardIter* pCI)
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
void Xute::vEnrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(CardIter* pCI)
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

    pCI->G.p_projElist = pCI->G.p_gbyAgg->pSub_projElist();

    pCI->G.p_projElist->vCard(CK_AGGELIST);
    pCI->G.p_aggElist = _pCard_AggElist(pCI->G.p_projElist);

    pCI->G.p_gbyElist = new Card_GbyElist;
    pCI->G.p_gbyElist->Y_vSet(P_Injected);
    pCI->G.p_gbyAgg->vInsertBefore(pCI->G.p_gbyElist,pCI->G.p_aggElist);


    if (AL_PARTITION == pCI->G.e_Level) {
      pCI->G.p_expr = Card_Expr::pGby_expr_From_stmCol(pCI->G.p_stmCol);
      pCI->G.p_gbyElist->vAppend(pCI->G.p_expr);
    }
    else {
      pCI->G.p_type = pCI->G.p_stmCol->pSub_type();
      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendChild(CK_EXPR,pCI->G.p_type->pDupCardAndSubCards());
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$1"_Stz);
      pCI->G.p_gbyElist->vAppend(pCI->G.p_expr);

    }
    // TODO: hack...
    pCI->G.p_expr->vCardNo(0);

    if (AL_PARTITION == pCI->G.e_Level) {
      pCI->G.p_expr = Card_Expr::pProj_expr_From_Gby_expr(pCI->G.p_expr,pCI->G.p_stmCol->szValue());
    }
    else {
      pCI->G.p_gbyElist->Y_vSet(P_ProjBump, (i8)1);
      pCI->G.p_type = _pCard_Type(pCI->G.p_expr->pSub_type()->pDupCardAndSubCards());

      pCI->G_vNewMember(CK_EXPR);
      pCI->G_vAppendChild(CK_EXPR,pCI->G.p_type->pDupCardAndSubCards());
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS,pCI->G.p_stmCol->szValue());
      pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
      pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$K1"_Stz);
    }

    pCI->G.p_expr->Y_vSet(P_Injected);
    pCI->G.p_aggElist->vPush(pCI->G.p_expr);


    if (AL_PARTITION != pCI->G.e_Level) {
      Xute::vBump_stmOdn_In_rpnCalc_Of_Elist(pCI, pCI->G.p_aggElist);
      pCI->G.p_gbyElist->Y_vCut(P_ProjBump);
      pCI->G.p_gbyElist->Y_vSet(P_ProjBumped);
    }

    if (pCI->G.p_stream = pCI->G.p_gbyAgg->pSub_stream()) {
      pCI->G.p_gbyAgg->vRemoveChild(pCI->G.p_stream);
      DELETE_CARD(pCI->G.p_stream);
    }
    pCI->G.p_stream = Card_Stream::pDerive_Of_aggreg(pCI->G.p_aggElist);
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
void Xute::vEnhance_sq_project_To_Include_Correlation_Column(CardIter* pCI)
{
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold sq <project>

  RASSERT (pCI->G.p_MainCard->bIsCard(CK_PROJECT));

  pCI->G.p_projElist = pCI->G.p_project->pSub_projElist();

  pCI->G.p_projElist->Y_vSet(P_ProjBump,(i8)1);
  pCI->G.p_projElist->Y_vSet(P_Restream);

  pCI->G.p_type = pCI->G.p_gbyElist->pGetChild(0)->pSub_type();

  // push <project>/__SQ1IGBKY1
  pCI->G_vPushNewChild(pCI->G.p_projElist, CK_EXPR);
  pCI->G.p_expr->vAppend(pCI->G.p_type->pDupCardAndSubCards());
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_AS,pCI->G.p_stmCol->szValue());
  pCI->G_vAppendNewChild(pCI->G.p_expr, CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc, CK_TERM, "$1"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // replace <project>/<stream> now that __SQ1IGBKY1 has been added

  pCI->G.p_stream = pCI->G.p_project->pSub_stream();

  pCI->G.p_project->vRemoveChild(pCI->G.p_stream);
  DELETE_CARD(pCI->G.p_stream);

  pCI->G.p_stream = Card_Stream::pDerive_Of_project(pCI->G.p_projElist);
  pCI->G.p_project->vAppend(pCI->G.p_stream);

  pCI->G.p_projElist->Y_vCut(P_Restream);
  pCI->G.p_projElist->Y_vSet(P_Restreamed);

  i8 nStmCol = pCI->G.p_stream->pSub_stmColList()->nChildCards();
  pCI->G.p_stream->Y_vSet(P_OdnLo,(i8)1);
  pCI->G.p_stream->Y_vSet(P_OdnHi,(i8)nStmCol);

  // bump all $n in <project>/<projElist>/<expr>/<rpnCalc>
  //   now that __SQ1IGBKY1 has been added

  i8 nBumpBy = pCI->G.p_projElist->Y_iGet(P_ProjBump);

  Xute::vBump_stmOdn_In_rpnCalc_Of_Elist(pCI, pCI->G.p_projElist);// TODO: ,nBumpBy);
  pCI->G.p_projElist->Y_vSet(P_ProjBumped,(i8)nBumpBy);
  pCI->G.p_projElist->Y_vCut(P_ProjBump);
}

/*----------------------------------------------------------------------------*/
/* find and mark call to the subquery referenced in pCI->G.p_boolCalc              */
/*----------------------------------------------------------------------------*/
bool Xute::bFindAndMarkSubQryCall(CardIter* pCI)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vFind_stmCol_Definition_Of_Correlation_Column(CardIter* pCI)
{
  _static_

  // find the <table>/<stream>/<stmColList>/<stmCol>
  //  corresponding to the correlated predicate

  pCI->m_pCard_found = pCI->G.p_stream->pFindValueRecursive(pCI->m_pTerm_corrPred->szValue(), pCI->m_pCard_parent);
  pCI->G.p_stmCol = _pCard_StmCol(pCI->m_pCard_parent);

  pCI->G.p_stmCol->vMark1(MARK_AT);
  pCI->G.p_stmCol->Y_vSet(P_CorrStmCol);
}

/*----------------------------------------------------------------------------*/
/* fold in the fold expression                                                */
/*----------------------------------------------------------------------------*/
void Xute::vFoldIn_foldExpr(CardIter* pCI)
{
  _static_

  while (pCI->G_bEachMain()) {

    if (CK_GBYAGG == pCI->G.e_MainCard) {

      pCI->G.p_gbyElist = pCI->G.p_gbyAgg->pSub_gbyElist();
      Xute::vBump_stmOdn_In_rpnCalc_Of_Elist(pCI, pCI->G.p_gbyElist);

      pCI->G.p_aggElist = pCI->G.p_gbyAgg->pSub_aggElist();
      Xute::vBump_keyOdn_In_rpnCalc_Of_Elist(pCI, pCI->G.p_aggElist);
      Xute::vBump_stmOdn_In_rpnCalc_Of_Elist(pCI, pCI->G.p_aggElist);

      vPush_foldExpr_Into_aggElist(pCI);
      vPush_foldExpr_Into_gbyElist(pCI);
    }

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vIdentify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(CardIter* pCI)
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
void Xute::vMove_corrPred_To_mq_table_As_bounds(CardIter* pCI, Card_BoolCalc *p_boolCalc)
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

      Card_Table* p_table = pResolve_table_By_tblSyn(pCI, 'a',pCI->G.i_MainCard);

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
/* move <where> to <table keyExists>/<constraint>                             */
/*----------------------------------------------------------------------------*/
void Xute::vMoveWhereToTableAsConstraint(CardIter* pCI, Card_Table* p_table, Card_Where* p_where)
{
  _static_

  pCI->G.p_boolCalc = p_where->pSub_boolCalc();
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));
  pCI->G.p_PRED->vMark1(MARK_AT);

  pCI->G.p_subSet = pCI->G.p_boolCalc->pDupSubCards();
  RASSERT(1 == pCI->G.p_subSet->nChildCards());
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_subSet->pPop());
  DELETE_CARD(pCI->G.p_subSet);

  if ((pCI->G.p_constraint = p_table->pSub_constraint())) {
    pCI->G.p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();
    pCI->G.p_boolCalc->vPushPred(pCI->G.p_PRED);
  }
  else {
    pCI->G_vAppendNewChild(p_table,CK_CONSTRAINT);
    pCI->G_vAppendNewChild(pCI->G.p_constraint,CK_BOOLCALC);
    pCI->G_vAppendChild(CK_BOOLCALC,pCI->G.p_PRED);
  }


}

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Xute::vMoveWhereToTableAsFilter(CardIter* pCI, Card_Table* p_table, Card_Where* p_where)
{
  _static_

  pCI->G.p_boolCalc = p_where->pSub_boolCalc();
  pCI->G.p_PRED = static_cast<Card_PRED*>(pCI->G.p_boolCalc->pGetChild(0));
  pCI->G.p_PRED->vMark1(MARK_AT);

  pCI->G.p_PRED = reinterpret_cast<Card_PRED*>(pCI->G.p_PRED->pDupSubCards());

  if ((pCI->G.p_filter = p_table->pSub_filter())) {
    pCI->G.p_boolCalc = pCI->G.p_filter->pSub_boolCalc();
    pCI->G.p_boolCalc->vPushPred(pCI->G.p_PRED);
  }
  else {
    pCI->G_vAppendNewChild(p_table,CK_FILTER);
    pCI->G_vAppendNewChild(pCI->G.p_filter,CK_BOOLCALC);
    pCI->G_vAppendChild(CK_FILTER,pCI->G.p_PRED);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vResolve_Param_Ordinal(CardIter* pCI, Card_Term* p_term)
{
  _static_

  Stz* szParamNm = nullptr;
  Board* pSubQryBoard = pCI->m_pBench->pGetBoard(1);

  pCI->G_vPushBoard(pSubQryBoard);

  if (pCI->G_bEachMainWithSub(CK_PARAMLIST,CK_PARAM)) {
    szParamNm = pCI->G.p_param->szValue();
  }

  pCI->G_vPopIter();

  if (! szParamNm) {
    p_term->vValue("((no-parm))"_Stz);
    return;
  }

  //p_term->vValue(szParamNm);

  if (! pCI->G_bResolveSubCard(CK_STMCOL,CK_STMCOLNM,szParamNm, pCI->G.i_MainCard)) {
    p_term->vValue("((unresolved))"_Stz);
    return;
  }

  RASSERT(pCI->m_pCard_resolved->bIsCard(CK_STMCOL));

  pCI->G.p_tblColOdn = pCI->m_pCard_resolved->pSub_tblColOdn();
  if (! pCI->G.p_tblColOdn) {
    p_term->vValue("((no-tblColOdn))"_Stz);
    return;
  }

  p_term->vValue(pCI->G.p_tblColOdn->szValue());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Xute::
pResolve_table_By_tblSyn(CardIter* pCI, char cTblSyn, i8 i_Before)
{
  _static_

  Card_Table* p_table = nullptr;

  pCI->G_vPushIter();

  if (pCI->G_bResolveCard(CK_TABLE,cTblSyn,i_Before)) {
    RASSERT(pCI->m_pCard_resolved->bIsCard(CK_TABLE));

    p_table = _pCard_Table(pCI->m_pCard_resolved);
  }

  pCI->G_vPopIter();

  return p_table;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColOdn* Xute::
pResolve_tblColOdn_Into_stmColOdn(CardIter* pCI, Card_Term* p_term_tblColOdn, i8 i_Before)
{
  _static_

  Card_StmColOdn* p_term_stmColOdn = nullptr;

  pCI->G_vPushIter();

  Stz* szTblColOdn = p_term_tblColOdn->szValue();
  if (pCI->G_bResolveSubCard(CK_STMCOL,CK_TBLCOLODN,szTblColOdn,i_Before)) {
    RASSERT(pCI->m_pCard_resolved->bIsCard(CK_STMCOL));

    p_term_stmColOdn = pCI->m_pCard_resolved->pSub_stmColOdn();
  }

  pCI->G_vPopIter();

  return p_term_stmColOdn;
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vPush_foldExpr_Into_aggElist(CardIter* pCI)
{
  _static_

  pCI->G_vNewMember(CK_EXPR);

  pCI->G.p_expr->vAppend(pCI->G.p_foldElist->pSub_type()->pDupCardAndSubCards());

  pCI->G.p_expr->vAppend(pCI->G.p_foldElist->pSub_as()->pDupCardAndSubCards());

  pCI->G_vAppendNewChild(pCI->G.p_expr,CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc,CK_TERM,"$K1"_Stz);

  pCI->G.p_aggElist->vPush(pCI->G.p_expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xute::vPush_foldExpr_Into_gbyElist(CardIter* pCI)
{
  _static_

  pCI->G_vNewMember(CK_EXPR);

  pCI->G.p_expr->vAppend(pCI->G.p_foldElist->pSub_type()->pDupCardAndSubCards());

  pCI->G.p_expr->vAppend(pCI->G.p_foldElist->pSub_rpnCalc()->pDupCardAndSubCards());

  pCI->G.p_gbyElist->vPush(pCI->G.p_expr);
}

/*----------------------------------------------------------------------------*/
/* relabel expressions                                                        */
/*----------------------------------------------------------------------------*/
void Xute::vRelabelExpressions(CardIter* pCI)
{
  _static_

  i8 i_Expr;
  s32 sLabel;

  pCI->G_vRestart();
  while (pCI->G_bEachMainWith2Sub(CK_GBYAGG, CK_GBYELIST, CK_PROJELIST)) {

    i_Expr = 0;
    CardIter xCI_g(pCI->G.p_gbyElist);
    while ((pCI->G.p_expr = _pCard_Expr(xCI_g.pEachChild()))) {
      sLabel.vSprint("$K%lld"_Stz,++i_Expr);
      pCI->G.p_expr->vValue(sLabel.szStz());
    }

    i_Expr = 0;
    CardIter xCI_p(pCI->G.p_projElist);
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
void Xute::vRenumberStmOdn(CardIter* pCI)
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

