/*============================================================================*/
/* Xreshape.cpp --                                                       */
/*============================================================================*/

#include "Xreshape.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

#include "RelOp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xreshape)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xreshape::Xreshape(eCN eClass, Bench* pBench)
: Xgearing(eClass,pBench)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xreshape::~Xreshape(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Push_Correlated_cmpPred_Onto_mq_boolCalc(Card_BoolCalc *p_boolCalc, STORE::Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (p_boolCalc->nChildCards()) {
    M_vBoolCalc_PushPred(p_boolCalc,p_PRED);
  }
  else {
    p_boolCalc->vAppend(p_PRED);
  }
}


/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_AddPredToTableFilter(Card_Table* p_table, Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if ((G.p_filter = p_table->pSub_filter())) {
    G.p_boolCalc = G.p_filter->pSub_boolCalc();
    M_vBoolCalc_PushPred(G.p_boolCalc,p_PRED);
  }
  else {
    G_vAppendNewChild(p_table,CK_FILTER);
    G_vAppendNewChild(G.p_filter,CK_BOOLCALC);
    G_vAppendChild(CK_BOOLCALC,p_PRED);
  }
}

/*----------------------------------------------------------------------------*/
/* visit each stream and adjust it's lo-hi range                              */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_AdjustStmLoHi(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
  vReshape(__func__);

  i8 iOdn_shift = 0;

  i8 iLoOdn_running = 0;
  i8 iHiOdn_running = 0;

  // TODO: G_bEachSub(CK_STREAM)

  G_vRestart();
  while (G_bEachMain()) {

//    if (! (G.p_MainCard->bIsCard(CK_TABLE) || G.p_MainCard->bIsCard(CK_PROJECT)))
//      continue;

    G.p_stream = G.p_MainCard->pSub_stream();
    if (! G.p_stream)
      continue;

    if (G.p_MainCard->bIsCard(CK_AGGREG) ||
        G.p_MainCard->bIsCard(CK_GBYAGG) ||
        G.p_MainCard->bIsCard(CK_PROJECT)) {
      iOdn_shift = 0;
      iLoOdn_running = 0;
      iHiOdn_running = 0;
    }

    G.p_stmColList = G.p_stream->pSub_stmColList();
    i8 nStmCols = G.p_stmColList->nChildCards();

    i8 iLoOdn_stream = E_iStream_OdnLo(G.p_stream);
    if (C_UNKNOWN == iLoOdn_stream) {
      i8 iLoOdn_adjusted = iHiOdn_running + 1;
      i8 iHiOdn_adjusted = iLoOdn_adjusted + nStmCols - 1;
      M_vStream_Adjust_OrdinalRange(G.p_stream, iLoOdn_adjusted, iHiOdn_adjusted);

      iOdn_shift = iHiOdn_adjusted;
      iLoOdn_running = iLoOdn_adjusted;
      iHiOdn_running = iHiOdn_adjusted;

    }
    else {
      i8 iLoOdn_adjusted = iOdn_shift + 1;
      i8 iHiOdn_adjusted = iLoOdn_adjusted + nStmCols - 1;

      M_vStream_Adjust_OrdinalRange(G.p_stream, iLoOdn_adjusted, iHiOdn_adjusted);
      iOdn_shift = iHiOdn_adjusted;

      iLoOdn_running = iLoOdn_adjusted;
      iHiOdn_running = iHiOdn_adjusted;
    }

  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Bump_keyOdn_In_rpnCalc_Of_Elist(Card_ELIST* p_ELIST)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);


  CardIter xCI(p_ELIST);
  while ((G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    G.p_rpnCalc = G.p_expr->pSub_rpnCalc();

    if (! G.p_rpnCalc)
      continue;

    CardIter xCI2(G.p_rpnCalc);
    while ((G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = G.p_term->szValue();

      if (T_bValue_IsKeyOrdinal(szTerm)) {
        M_vTerm_Bump_KeyOrdinal(G.p_term);
        G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(Card_ELIST* p_ELIST)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  CardIter xCI(p_ELIST);
  while ((G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    G.p_rpnCalc = G.p_expr->pSub_rpnCalc();

    if (! G.p_rpnCalc)
      continue;

    CardIter xCI2(G.p_rpnCalc);
    while ((G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = G.p_term->szValue();

      if (T_bValue_IsStreamColOrdinal(szTerm)) {
        M_vTerm_Bump_StreamColOrdinal(G.p_term);
        G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Choose_index_For_crossJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_index->Y_vSet(P_ChosenIndex);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Choose_index_For_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_index->Y_vSet(P_ChosenIndex);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Choose_index_For_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_index->Y_vSet(P_ChosenIndex);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Choose_index_For_keyExists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_index->Y_vSet(P_ChosenIndex);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Choose_index_For_keyNotExists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_index->Y_vSet(P_ChosenIndex);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Derive_foldElist_From_project_stream(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // <fold> W_A_C_ro


  G_vAppendNewChild(G.p_fold, CK_FOLDELIST);

  G.p_stmColList = G.p_stream->pSub_stmColList();

  if (G.p_stmColList->nChildCards() > 1) {

    G_vAppendNewChild(G.p_foldElist, CK_EXPR);
    G_vAppendNewChild(G.p_expr, CK_NOTE, "// sq gbykey"_Stz);
    G.p_stmCol = _pCard_StmCol(G.p_stmColList->pGetChild(0));
    G.p_stmColNm = G.p_stmCol->pSub_stmColNm();
    G.p_type = G.p_stmCol->pSub_type();
    G.p_expr->vAppend(G.p_type->pDupCardAndSubCards());
    G_vAppendNewChild(G.p_expr, CK_AS, G.p_stmColNm->szValue());
    G_vAppendNewChild(G.p_expr, CK_RPNCALC);
    G_vAppendNewChild(G.p_rpnCalc, CK_TERM, "$1"_Stz);

    G_vAppendNewChild(G.p_foldElist, CK_EXPR);
    G_vAppendNewChild(G.p_expr, CK_NOTE, "// sq result"_Stz);
    G.p_stmCol = _pCard_StmCol(G.p_stmColList->pGetChild(1));
    G.p_stmColNm = G.p_stmCol->pSub_stmColNm();
    G.p_type = G.p_stmCol->pSub_type();
    G.p_as = G.p_stmCol->pSub_as();
    G.p_expr->vAppend(G.p_type->pDupCardAndSubCards());
    G_vAppendNewChild(G.p_expr, CK_AS, G.p_stmColNm->szValue());
    G_vAppendNewChild(G.p_expr, CK_RPNCALC);
    G_vAppendNewChild(G.p_rpnCalc,CK_TERM,"$2"_Stz);

    G.p_fold->Y_vSet(P_BumpRight,(i8)2);
  }
  else {

    G_vAppendNewChild(G.p_foldElist, CK_EXPR);
    G_vAppendNewChild(G.p_expr, CK_NOTE, "// sq result"_Stz);
    G.p_stmCol = _pCard_StmCol(G.p_stmColList->pGetChild(0));
    G.p_stmColNm = G.p_stmCol->pSub_stmColNm();
    G.p_type = G.p_stmCol->pSub_type();
    G.p_as = G.p_stmCol->pSub_as();
    G.p_expr->vAppend(G.p_type->pDupCardAndSubCards());
    G_vAppendNewChild(G.p_expr, CK_AS, G.p_stmColNm->szValue());
    G_vAppendNewChild(G.p_expr, CK_RPNCALC);
    G_vAppendNewChild(G.p_rpnCalc,CK_TERM,"$1"_Stz);

    G.p_fold->Y_vSet(P_BumpRight,(i8)1);
  }


/*
  if (G.p_type) {
    G.p_expr->vAppend(G.p_type->pDupCardAndSubCards());
  }
  else {
    G_vAppendNewChild(G.p_expr, CK_TYPE, "((type missing))"_Stz);
  }
  G_vAppendNewChild(G.p_expr, CK_AS, "__SQ1RESULT"_Stz);
  G_vAppendNewChild(G.p_expr, CK_RPNCALC);
  G_vAppendNewChild(G.p_rpnCalc,CK_TERM,"$1"_Stz);


  G.p_foldElist = G.p_fold->pSub_foldElist();

  G_vPushNewChild(G.p_foldElist, CK_EXPR);

  */
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Derive_stmOrdList_For_gbyAgg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  //G.p_gbyAgg->vDump();

  G.p_stream = G.p_gbyAgg->pSub_stream();
  if (! G.p_stream) {
    G_vAppendNewChild(G.p_gbyAgg,CK_STREAM);
  }

  if ((G.p_stmOrdList = G.p_stream->pSub_stmOrdList())) {
    G.p_stream->vRemoveChild(G.p_stmOrdList);
    DELETE_CARD(G.p_stmOrdList);
  }

  G_vAppendNewChild(G.p_stream,CK_STMORDLIST);

  G.p_aggElist =  G.p_gbyAgg->pSub_aggElist();
  CardIter xCI(G.p_aggElist);
  while ((G.p_expr = _pCard_Expr(xCI.pEachChild()))) {
    i8 i_Key = E_iExpr_rpnCalc_WhichGbyKey(G.p_expr);
    if (C_UNKNOWN == i_Key)
      continue;

    G_vAppendNewChild(G.p_stmOrdList,CK_STMORDODN);

    G.p_stmOrdOdn->vValueSprint("$%lld"_Stz,xCI.i_Child()+1);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Derive_stmOrdList_For_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  RASSERT(G.p_stmColList);
  Card_StmColList* p_stmColList_l = G.p_stmColList;
  RASSERT(G.p_stmOrdList);
  Card_StmOrdList* p_stmOrdList_l = G.p_stmOrdList;

  RASSERT(G.p_indexList);
  RASSERT(G.p_stream);

  G.p_index = T_pIndex_Find_chosenIndex(G.p_indexList);
  if (! G.p_index)
    return;

  G.p_bounds = G.p_index->pSub_bounds();
  if (! G.p_bounds)
    return;

  G.p_eBound = G.p_bounds->pSub_eBound();
  if (! G.p_eBound)
    return;

  Stz* szTblColOdn_r = E_szEBound_Get_tblColOdn(G.p_eBound);
  if (! szTblColOdn_r)
    return;

  vTrace("szTblColOdn_r %s\n"_Stz, szTblColOdn_r);


  // find the stmCol_r == szTblColOdn_r in prior stream stmColList
  i8 i_StmCol;
  Card_StmCol* p_stmCol_l =
      T_pStmCol_FindBy_tblColOdn(p_stmColList_l, szTblColOdn_r, i_StmCol);
  if (! p_stmCol_l)
    return;

  Card_StmColOdn* p_stmColOdn_l = p_stmCol_l->pSub_stmColOdn();
  Stz* szStmColOdn_l = p_stmColOdn_l->szValue();


  i8 i_StmOrdOdn;
  Card_StmOrdOdn* p_stmOrdOdn_l =
    T_pStmOrdOdn_FindBy_stmColOdn(p_stmOrdList_l,szStmColOdn_l,i_StmOrdOdn);

  if (! p_stmOrdOdn_l)
    return;

  Card_StmOrdList* p_stmOrdList_ij = new Card_StmOrdList;
  G.p_stream->vAppend(p_stmOrdList_ij);

  p_stmOrdList_ij->vAppendNewCardTail(CK_STMORDODN, p_stmOrdOdn_l->szValue());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Derive_stmOrdList_For_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_stream = G.p_indexScan->pSub_stream();
  if (! G.p_stream)
    return;
  G.p_stmColList = G.p_stream->pSub_stmColList();
  if (! G.p_stmColList)
    return;

  G.p_stmOrdList = nullptr;

  i8 i_StmCol;
  G.p_index = G.p_indexScan->pSub_index();
  G.p_idxColList = G.p_index->pSub_idxColList();
  CardIter xCI(G.p_idxColList);
  while ((G.p_idxCol = _pCard_IdxCol(xCI.pEachChild()))) {
    G.p_stmCol = T_pStmCol_FindBy_tblColOdn(G.p_stmColList,
                                            G.p_idxCol->szValue(),
                                            i_StmCol);
    if (! G.p_stmCol)
      return;

    if (! G.p_stmOrdList) {
      G.p_stmOrdList = new Card_StmOrdList;
      G.p_stream->vAppend(G.p_stmOrdList);
    }

    G_vAppendNewChild(G.p_stmOrdList,CK_STMORDODN);
    G.p_stmOrdOdn->vValueSprint("$%lld"_Stz,i_StmCol+1);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Derive_stmOrdList_For_project(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_stmOrdList->vDump();
  G.p_project->vDump();

  G.p_projElist = G.p_project->pSub_projElist();
  G.p_stream = G.p_project->pSub_stream();
  G.p_stmColList = G.p_stream->pSub_stmColList();

  Card_StmOrdList* p_stmOrdList_project = nullptr;

  i8 nLeading = 0;
  CardIter xCI(G.p_stmOrdList);
  while ((G.p_stmOrdOdn = _pCard_StmOrdOdn(xCI.pEachChild()))) {
    i8 i_Expr = E_iELIST_rpnCalc_Has_stmOrdOdn(G.p_projElist,G.p_stmOrdOdn);
    if (C_UNKNOWN != i_Expr) {
      nLeading++;

      if (! p_stmOrdList_project) {
        p_stmOrdList_project = new Card_StmOrdList;
        G.p_stream->vAppend(p_stmOrdList_project);
      }

      G_vAppendNewChild(p_stmOrdList_project,CK_STMORDODN);
      G.p_stmOrdOdn->vValueSprint("$%lld"_Stz,i_Expr+1);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Derive_stmOrdList_For_sort(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: need a stream?

  G.p_stream = _pCard_Stream(G.p_sort->pAppendNewCardTail(CK_STREAM));
  G.p_stmOrdList = _pCard_StmOrdList(G.p_sort->pAppendNewCardTail(CK_STMORDLIST));

  CardIter xCI(G.p_sbyElist);
  while ((G.p_expr = _pCard_Expr(xCI.pEachChild(CK_EXPR)))) {
    G.p_rpnCalc = G.p_expr->pSub_rpnCalc();

    if (! (G.p_term = M_pRpnCalc_GetSingle_stmColOdn(G.p_rpnCalc)))
      break;

    Stz* szStmColOdn = G.p_term->szValue();
    Stz* szStmOrdOdn = szStmColOdn;
    G.p_stmOrdList->vAppendNewCardTail(CK_STMORDODN,szStmOrdOdn);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Derive_Stream_Order(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create stream order for index scan

  G_vRestart();
  while (G_bEachMain(CK_INDEXSCAN)) {
    R_vUTE_Derive_stmOrdList_For_indexScan();
  }

  vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  G_vRestart();
  while (G_bEachMain(CK_INDEXSCAN)) {
    G.p_stmColList = G.p_indexScan->pSub_stmColList();
    G.p_stmOrdList = G.p_indexScan->pSub_stmOrdList();

    for (;;) {
      if (! G_bEachMainWith2Sub(CK_INDEXJOIN,CK_INDEXLIST,CK_STREAM))
        break;
      R_vUTE_Derive_stmOrdList_For_indexJoin();
    }
  }

  vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G_vRestart();
  while (G_bEachMain()) {

    if (! G.p_MainCard->bIsCard(CK_GBYAGG))
      continue;

    R_vUTE_Derive_stmOrdList_For_gbyAgg();

    if (G_bIsNextMain(CK_GBYAGG)) {
      G_bEachMain();
      R_vUTE_Derive_stmOrdList_For_gbyAgg();

      if (G_bIsNextMain(CK_GBYAGG)) {
        G_bEachMain();
        R_vUTE_Derive_stmOrdList_For_gbyAgg();
      }
    }

  }

  G.pDeck->vRenumberRecursive();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G_vRestart();
  while (G_bEachMain()) {

    if (!G.p_MainCard->bIsCard(CK_GBYAGG))
      continue;

    G.p_stmOrdList = G.p_gbyAgg->pSub_stmOrdList();
    while (G_bIsNextMain(CK_GBYAGG)) {
      G_bEachMain();
      G.p_stmOrdList = G.p_gbyAgg->pSub_stmOrdList();
    }

    if (G_bIsNextMain(CK_PROJECT)) {
      G_bEachMain();
      R_vUTE_Derive_stmOrdList_For_project();
    }

    break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G_vRestart();
  while (G_bEachMainWithSub(CK_SORT,CK_SBYELIST)) {
    R_vUTE_Derive_stmOrdList_For_sort();
  }

  G.pDeck->vRenumberRecursive();
  vPrintBoard();

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Enrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold promote sq <aggreg> seq to <gbyAgg> seq

  for (i8 i_Series = 0; i_Series < 3; i_Series++) {

    G.p_MainCard->vCard(CK_GBYAGG);
    G.p_gbyAgg = _pCard_GbyAgg(G.p_MainCard);

    G.p_level = G.p_gbyAgg->pSub_level();
    G.e_Level = (eAL) G.p_level->Y_iGet(P_Level);

    G.p_projElist = G.p_gbyAgg->pSub_projElist();

    G.p_projElist->vCard(CK_AGGELIST);
    G.p_aggElist = _pCard_AggElist(G.p_projElist);

    G.p_gbyElist = new Card_GbyElist;
    G.p_gbyElist->Y_vSet(P_Injected);
    G.p_gbyAgg->vInsertBefore(G.p_gbyElist,G.p_aggElist);


    if (AL_PARTITION == G.e_Level) {
      G.p_expr = D_pDerive_Gby_expr_From_stmCol(G.p_stmCol);
      G.p_gbyElist->vAppend(G.p_expr);
    }
    else {
      G.p_type = G.p_stmCol->pSub_type();
      G_vNewMember(CK_EXPR);
      G_vAppendChild(CK_EXPR,G.p_type->pDupCardAndSubCards());
      G_vAppendNewChild(G.p_expr, CK_RPNCALC);
      G_vAppendNewChild(G.p_rpnCalc, CK_TERM, "$1"_Stz);
      G.p_gbyElist->vAppend(G.p_expr);

    }
    // TODO: hack...
    G.p_expr->vCardNo(0);

    if (AL_PARTITION == G.e_Level) {
      G.p_expr = D_pDerive_Proj_expr_From_Gby_expr(G.p_expr,G.p_stmCol->szValue());
    }
    else {
      G.p_gbyElist->Y_vSet(P_ProjBump, (i8)1);
      G.p_type = _pCard_Type(G.p_expr->pSub_type()->pDupCardAndSubCards());

      G_vNewMember(CK_EXPR);
      G_vAppendChild(CK_EXPR,G.p_type->pDupCardAndSubCards());
      G_vAppendNewChild(G.p_expr, CK_AS,G.p_stmCol->szValue());
      G_vAppendNewChild(G.p_expr, CK_RPNCALC);
      G_vAppendNewChild(G.p_rpnCalc, CK_TERM, "$K1"_Stz);
    }

    G.p_expr->Y_vSet(P_Injected);
    G.p_aggElist->vPush(G.p_expr);


    if (AL_PARTITION != G.e_Level) {
      R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(G.p_aggElist);
      G.p_gbyElist->Y_vCut(P_ProjBump);
      G.p_gbyElist->Y_vSet(P_ProjBumped);
    }

    if (G.p_stream = G.p_gbyAgg->pSub_stream()) {
      G.p_gbyAgg->vRemoveChild(G.p_stream);
      DELETE_CARD(G.p_stream);
    }
    G.p_stream = C_pProduce_stream_Of_aggreg(G.p_aggElist);
    G.p_gbyAgg->vAppend(G.p_stream);

    if (G_bEachMain()) {
      if (! G.p_MainCard->bIsCard(CK_AGGREG)) {
        bool b = G_bRewindMain();
        RASSERT(b);
        break;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Enhance_sq_project_To_Include_Correlation_Column(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold sq <project>

  RASSERT (G.p_MainCard->bIsCard(CK_PROJECT));

  G.p_projElist = G.p_project->pSub_projElist();

  G.p_projElist->Y_vSet(P_ProjBump,(i8)1);
  G.p_projElist->Y_vSet(P_Restream);

  G.p_type = G.p_gbyElist->pGetChild(0)->pSub_type();

  // push <project>/__SQ1IGBKY1
  G_vPushNewChild(G.p_projElist, CK_EXPR);
  G.p_expr->vAppend(G.p_type->pDupCardAndSubCards());
  G_vAppendNewChild(G.p_expr, CK_AS,G.p_stmCol->szValue());
  G_vAppendNewChild(G.p_expr, CK_RPNCALC);
  G_vAppendNewChild(G.p_rpnCalc, CK_TERM, "$1"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // replace <project>/<stream> now that __SQ1IGBKY1 has been added

  G.p_stream = G.p_project->pSub_stream();

  G.p_project->vRemoveChild(G.p_stream);
  DELETE_CARD(G.p_stream);

  G.p_stream = C_pProduce_stream_Of_project(G.p_projElist);
  G.p_project->vAppend(G.p_stream);

  G.p_projElist->Y_vCut(P_Restream);
  G.p_projElist->Y_vSet(P_Restreamed);

  i8 nStmCol = G.p_stream->pSub_stmColList()->nChildCards();
  G.p_stream->Y_vSet(P_OdnLo,(i8)1);
  G.p_stream->Y_vSet(P_OdnHi,(i8)nStmCol);

  // bump all $n in <project>/<projElist>/<expr>/<rpnCalc>
  //   now that __SQ1IGBKY1 has been added

  i8 nBumpBy = G.p_projElist->Y_iGet(P_ProjBump);

  R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(G.p_projElist);// TODO: ,nBumpBy);
  G.p_projElist->Y_vSet(P_ProjBumped,(i8)nBumpBy);
  G.p_projElist->Y_vCut(P_ProjBump);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Convert_tblColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  i8 i_Main = G.pDeck->iFindChild(G.p_MainCard);

  Card_BoolCalc* p_boolCalc = G.p_constraint->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,i_Main](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! T_bValue_IsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    G_bResolveSubCard(CK_STMCOL, CK_STMCOLODN, p_term->szValue(), i_Main + 1);

    Card_StmColNm* p_stmColNm = G.p_stmCol->pSub_stmColNm();

    M_vCard_Set_Value(p_term,p_stmColNm->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = M_bPredRecursive(p_pred_ROOT,bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Convert_tblColOdn_To_stmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    vTrace("CONVERT %s\n"_Stz,p_term->szValue());

    if (! T_bValue_IsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    G.p_stmColOdn = R_pUTE_Resolve_tblColOdn_Into_stmColOdn(p_term,G.i_MainCard+1);

    M_vCard_Set_Value(p_term, G.p_stmColOdn->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_MainCard->bApplyRecursive(CK_TERM, bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Correct_having_BaseOn_foldExpr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_boolCalc = G.p_having->pSub_boolCalc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card *pCard) {

    if (MARK_AT != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    G.p_roPred = _pCard_RoPred(pCard);

    m_pTerm_0 = _pCard_Term(G.p_roPred->pGet(0));
    M_vTerm_Bump_StreamColOrdinal(m_pTerm_0);

    m_pTerm_1 = _pCard_Term(G.p_roPred->pGet(1));
    G.p_roPred->vRemoveChild(m_pTerm_1);
    DELETE_CARD(m_pTerm_1);
    G_vAppendNewChild(G.p_roPred, CK_TERM, "$1"_Stz);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_boolCalc->bApplyRecursive(CK_ROPRED, bLambdaFn);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Bump_where_BaseOn_foldExpr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_boolCalc = G.p_where->pSub_boolCalc();

  i8 nBumpRight = G.p_fold->Y_iGet(P_BumpRight);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,nBumpRight](Card *pCard) {

    if (MARK_AT != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    G.p_roPred = _pCard_RoPred(pCard);

//    m_pTerm_0 = _pCard_Term(G.p_roPred->pGet(0));
//    M_vTerm_Bump_StreamColOrdinal(m_pTerm_0,nBumpRight);

    m_pTerm_1 = _pCard_Term(G.p_roPred->pGet(1));
    G.p_roPred->vRemoveChild(m_pTerm_1);
    DELETE_CARD(m_pTerm_1);

    m_pTerm_1 = _pCard_Term(G.p_roPred->pGet(1));
    G.p_roPred->vRemoveChild(m_pTerm_1);
    DELETE_CARD(m_pTerm_1);

    G_vAppendNewChild(G.p_roPred, CK_TERM, "$2"_Stz);

    G_vMorphCard(G.p_roPred, CK_CMPPRED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_boolCalc->bApplyRecursive(CK_ROPRED, bLambdaFn);

  G.p_fold->Y_vSet(P_BumpedRight,nBumpRight);
  G.p_fold->Y_vCut(P_BumpRight);
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Derive_foldExpr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! G_bResolveSubCard(CK_PROJELIST,G.i_FoldCard))
    return;

  G.p_type = m_pCard_resolved->pSub_type();

  G_vAppendNewChild(G.p_fold, CK_FOLDELIST);
  G_vAppendNewChild(G.p_foldElist, CK_EXPR);

  if (G.p_type) {
    G.p_expr->vAppend(G.p_type->pDupCardAndSubCards());
  }
  else {
    G_vAppendNewChild(G.p_expr, CK_TYPE, "((type missing))"_Stz);
  }
  G_vAppendNewChild(G.p_expr, CK_AS, "__SQ1RESULT"_Stz);
  G_vAppendNewChild(G.p_expr, CK_RPNCALC);
  G_vAppendNewChild(G.p_rpnCalc,CK_TERM,"$1"_Stz);
}

/*----------------------------------------------------------------------------*/
/* find and mark call to the subquery referenced in G.p_boolCalc              */
/*----------------------------------------------------------------------------*/
bool Xreshape::R_bUTE_FindAndMarkSubQryCall(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  bool bFoundCall = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_roPred = [this,&bFoundCall](Card* pCard)
  {
    G.p_roPred = _pCard_RoPred(pCard);

    if (G.p_roPred->bIsValue("exists"_Stz) || G.p_roPred->bIsValue("notExists"_Stz)) {
      m_pTerm_0 = _pCard_Term(G.p_roPred->pGet(0));
      m_sSqyArg = m_pTerm_0->szValue();
      m_pTerm_1 = _pCard_Term(G.p_roPred->pGet(1));
      m_sSqyNm = m_pTerm_1->szValue();

      bFoundCall = true;

      G.p_roPred->vMark1(MARK_AT);

      return C_CONTINUE_RECURSION;
    }

    if (G.p_roPred->bIsValue("in"_Stz) || G.p_roPred->bIsValue("notIn"_Stz)) {
      m_pTerm_0 = _pCard_Term(G.p_roPred->pGet(0));
      m_sSqyNm = m_pTerm_0->szValue();
      m_pTerm_1 = _pCard_Term(G.p_roPred->pGet(1));
      m_sSqyRsl = m_pTerm_1->szValue();
      m_pTerm_2 = _pCard_Term(G.p_roPred->pGet(2));
      m_sSqyArg = m_pTerm_2->szValue();

      bFoundCall = true;

      G.p_roPred->vMark1(MARK_AT);

      return C_CONTINUE_RECURSION;
    }

    // TODO: needs to be a list
    if (   G.p_roPred->bIsValue("<"_Stz)
        || G.p_roPred->bIsValue("<="_Stz)
        || G.p_roPred->bIsValue("=="_Stz)
        || G.p_roPred->bIsValue("!="_Stz)
        || G.p_roPred->bIsValue(">="_Stz)
        || G.p_roPred->bIsValue(">"_Stz)
        ) {

      if (3 == G.p_roPred->nChildCards()) {
        m_pTerm_0 = _pCard_Term(G.p_roPred->pGet(0));
        // TODO: this doesnt seem right
        m_sSqyRO = m_pTerm_0->szValue();

        m_pTerm_1 = _pCard_Term(G.p_roPred->pGet(1));
        m_sSqyArg = m_pTerm_1->szValue();

        m_pTerm_2 = _pCard_Term(G.p_roPred->pGet(2));
        m_sSqyNm = m_pTerm_2->szValue();
      }
      else {
        m_pTerm_0 = _pCard_Term(G.p_roPred->pGet(0));
        m_sSqyArg = m_pTerm_0->szValue();

        m_pTerm_1 = _pCard_Term(G.p_roPred->pGet(1));
        m_sSqyNm = m_pTerm_1->szValue();
      }

      bFoundCall = true;

      G.p_roPred->vMark1(MARK_AT);

      return C_CONTINUE_RECURSION;
    }

    NEVER_REACH

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  auto bLambdaFn_cmpPred = [this,&bFoundCall](Card* pCard)
//  {
//    G.p_cmpPred = _pCard_CmpPred(pCard);
//    m_sSqyRO  =  G.p_cmpPred->szValue();
//
//    m_pTerm_0 = _pCard_Term(G.p_cmpPred->pGet(0));
//    m_sSqyArg = m_pTerm_0->szValue();
//
//    m_pTerm_1 = _pCard_Term(G.p_cmpPred->pGet(1));
//    m_sSqyNm = m_pTerm_1->szValue();
//
//    bFoundCall = true;
//
//    G.p_cmpPred->vMark1(MARK_AT);
//
//    return C_CONTINUE_RECURSION;
//  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_boolCalc->vDump();

  G.p_boolCalc->bApplyRecursive(CK_ROPRED, bLambdaFn_roPred);
  if (bFoundCall)
    return true;

//  G.p_boolCalc->bApplyRecursive(CK_CMPPRED, bLambdaFn_cmpPred);
//  if (bFoundCall)
//    return true;


  return bFoundCall;
}

/*----------------------------------------------------------------------------*/
/* fold in the fold expression                                                */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_FoldIn_foldExpr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  while (G_bEachMain()) {

    if (CK_GBYAGG == G.e_MainCard) {

      G.p_gbyElist = G.p_gbyAgg->pSub_gbyElist();
      R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(G.p_gbyElist);

      G.p_aggElist = G.p_gbyAgg->pSub_aggElist();
      R_vUTE_Bump_keyOdn_In_rpnCalc_Of_Elist(G.p_aggElist);
      R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(G.p_aggElist);

      R_vUTE_Push_foldExpr_Into_aggElist();
      R_vUTE_Push_foldExpr_Into_gbyElist();
    }

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Identify_Correlation_Column_In_cmpPred_Of_sq_boolCalc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pre-fold identify the <table>/<filter>/<boolCalc>/<cmpPred> which is correlated by parameter

  auto bMarkCorrPred = [this](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_CMPPRED))
      return C_CONTINUE_RECURSION;

    bool bIsCorrPred = false;
//    CardIter xCI(p_pred);
//    while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
//
//      // TODO: hack... %1 $n may not be in this order
//      if (T_bValue_IsParamOrdinal(G.p_term->szValue())) {
//        bIsCorrPred = true;
//        m_pTerm_corrPred = _pCard_Term(xCI.pPrevChild());
//        break;
//      }
//    }

    if ((m_pTerm_corrPred = E_pCmpPred_Extract_Arg_term(static_cast<Card_CmpPred*>(p_pred)))) {
      bIsCorrPred = true;
      m_pTerm_corrPred->vDump();
    }

    if (bIsCorrPred) {
      p_pred->vMark1(MARK_AT);
      p_pred->Y_vSet(P_CorrPred);
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_boolCalc = G.p_filter->pSub_boolCalc();
  G.p_PRED = static_cast<Card_PRED*>(G.p_boolCalc->pGetChild(0));
  bool bIgnore = M_bPredRecursive(G.p_PRED,bMarkCorrPred);


  m_pTerm_corrPred->vDump();
//  G.p_stream->vDump();
//  G.p_filter->vDump();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_MainCard->vRemoveChild(G.p_filter);
  G.pDeck->vInsertAfter(G.p_filter, G.p_MainCard);

  if (! G.p_filter->pSub_level()) {
    G.p_filter->vPush(G.p_MainCard->pSub_level()->pDupCardAndSubCards());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Find_stmCol_Definition_Of_Correlation_Column(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // find the <table>/<stream>/<stmColList>/<stmCol>
  //  corresponding to the correlated predicate

  m_pCard_found = G.p_stream->pFindValueRecursive(m_pTerm_corrPred->szValue(), m_pCard_parent);
  G.p_stmCol = _pCard_StmCol(m_pCard_parent);

  G.p_stmCol->vMark1(MARK_AT);
  G.p_stmCol->Y_vSet(P_CorrStmCol);
  G.p_stmCol->vDump();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Move_corrPred_To_mq_table_As_bounds(Card_BoolCalc *p_boolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vMove = [this](Card_PRED* p_pred, Card_BoolCalc *p_boolCalc,
                      auto&& vTwiddle /*oddity permits recursive lambda*/)
  {
    if (! p_pred->bIsPRED())
      return; /* this effectively declares twiddle as void */

    if (p_pred->bIsCard(CK_CMPPRED) && p_pred->Y_bHas(P_CorrPred)) {

      Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_pred);

      M_vBoolCalc_RemovePred(p_boolCalc,p_pred);

      // todo: find table 'd'->'a' and add p_pred as bounds

      Card_Table* p_table = R_pUTE_Resolve_table_By_tblSyn('a',G.i_MainCard);

      Card_Bounds* pBounds = C_pBounds_CreateFrom_cmpPred(p_cmpPred,'a');

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
/* move <where> to <table keyExists>/<constraint>                             */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_MoveWhereToTableAsConstraint(Card_Table* p_table, Card_Where* p_where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_boolCalc = p_where->pSub_boolCalc();
  G.p_PRED = static_cast<Card_PRED*>(G.p_boolCalc->pGetChild(0));
  G.p_PRED->vMark1(MARK_AT);

  G.p_subSet = G.p_boolCalc->pDupSubCards();
  RASSERT(1 == G.p_subSet->nChildCards());
  G.p_PRED = static_cast<Card_PRED*>(G.p_subSet->pPop());
  DELETE_CARD(G.p_subSet);

  if ((G.p_constraint = p_table->pSub_constraint())) {
    G.p_boolCalc = G.p_constraint->pSub_boolCalc();
    M_vBoolCalc_PushPred(G.p_boolCalc,G.p_PRED);
  }
  else {
    G_vAppendNewChild(p_table,CK_CONSTRAINT);
    G_vAppendNewChild(G.p_constraint,CK_BOOLCALC);
    G_vAppendChild(CK_BOOLCALC,G.p_PRED);
  }


}

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_MoveWhereToTableAsFilter(Card_Table* p_table, Card_Where* p_where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_boolCalc = p_where->pSub_boolCalc();
  G.p_PRED = static_cast<Card_PRED*>(G.p_boolCalc->pGetChild(0));
  G.p_PRED->vMark1(MARK_AT);

  G.p_PRED = reinterpret_cast<Card_PRED*>(G.p_PRED->pDupSubCards());

  if ((G.p_filter = p_table->pSub_filter())) {
    G.p_boolCalc = G.p_filter->pSub_boolCalc();
    M_vBoolCalc_PushPred(G.p_boolCalc,G.p_PRED);
  }
  else {
    G_vAppendNewChild(p_table,CK_FILTER);
    G_vAppendNewChild(G.p_filter,CK_BOOLCALC);
    G_vAppendChild(CK_FILTER,G.p_PRED);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Push_foldExpr_Into_aggElist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_foldElist->vDump();

  G_vNewMember(CK_EXPR);

  G.p_expr->vAppend(G.p_foldElist->pSub_type()->pDupCardAndSubCards());

  G.p_expr->vAppend(G.p_foldElist->pSub_as()->pDupCardAndSubCards());

  G_vAppendNewChild(G.p_expr,CK_RPNCALC);
  G_vAppendNewChild(G.p_rpnCalc,CK_TERM,"$K1"_Stz);

  G.p_aggElist->vPush(G.p_expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Push_foldExpr_Into_gbyElist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G_vNewMember(CK_EXPR);

  G.p_expr->vAppend(G.p_foldElist->pSub_type()->pDupCardAndSubCards());

  G.p_expr->vAppend(G.p_foldElist->pSub_rpnCalc()->pDupCardAndSubCards());

  G.p_gbyElist->vPush(G.p_expr);
}

/*----------------------------------------------------------------------------*/
/* relabel expressions                                                        */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_RelabelExpressions(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 i_Expr;
  s32 sLabel;

  G_vRestart();
  while (G_bEachMainWith2Sub(CK_GBYAGG, CK_GBYELIST, CK_PROJELIST)) {

    i_Expr = 0;
    CardIter xCI_g(G.p_gbyElist);
    while ((G.p_expr = _pCard_Expr(xCI_g.pEachChild()))) {
      sLabel.vSprint("$K%lld"_Stz,++i_Expr);
      G.p_expr->vValue(sLabel.szStz());
    }

    i_Expr = 0;
    CardIter xCI_p(G.p_projElist);
    while ((G.p_expr = _pCard_Expr(xCI_p.pEachChild()))) {
      sLabel.vSprint("$%lld"_Stz,++i_Expr);
      G.p_expr->vValue(sLabel.szStz());
    }

  }

  i8 i_StmCol;

  G_vRestart();
  while (G_bEachMain()) {
    if (! (G.p_stream = G.p_MainCard->pSub_stream()))
      continue;

    i_StmCol = G.p_stream->Y_iGet(P_OdnLo);

    G.p_stmColList = G.p_stream->pSub_stmColList();

    CardIter xCI_s(G.p_stmColList);
    while ((G.p_stmCol = _pCard_StmCol(xCI_s.pEachChild()))) {
      sLabel.vSprint("$%lld"_Stz,i_StmCol++);
      G.p_stmCol->vValue(sLabel.szStz());
    }

  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Remove_filter_IfEmpty(Card* pParent, Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: xt

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();

  if (p_boolCalc->nChildCards())
    return;

  pParent->vRemoveChild(p_filter);
  delete p_filter;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Remove_PRED_From_boolCalc(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: remove? It is unused

  M_vBoolCalc_RemovePred(p_boolCalc,p_pred);
}

/*----------------------------------------------------------------------------*/
/* visit each stream and adjust it's lo-hi range                              */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_RenumberStmOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
  vReshape(__func__);

  // TODO: G_bEachSub(CK_STREAM)

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card *pCard) {

    G.p_stmCol    = _pCard_StmCol(pCard);
    G.p_stmColOdn = G.p_stmCol->pSub_stmColOdn();

    if (! G.p_stmColOdn)
      return C_CONTINUE_RECURSION;

    M_vCard_Set_StreamColOrdinal(G.p_stmColOdn,G.i_stmColOdn++);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G_vRestart();
  while (G_bEachMain()) {

    if (! (G.p_MainCard->bIsCard(CK_TABLE) || G.p_MainCard->bIsCard(CK_PROJECT)))
      continue;

    G.p_stream = G.p_MainCard->pSub_stream();

    G.i_stmColOdn = G.p_stream->Y_iGet(P_OdnLo);

    G.p_stmColList = G.p_stream->pSub_stmColList();

    G.p_stmColList->bApplyRecursive(CK_STMCOL,bLambdaFn);
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/* find sub card with given value and replace                                 */
/*----------------------------------------------------------------------------*/
bool Xreshape::R_bUTE_ReplaceValueOfSub(Card* pCard, eCK eSub, Stz* szValue, Stz* szWith)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  bool bReplaced = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this, szValue, szWith, &bReplaced](Card *pCard)
  {
    G.p_term = _pCard_Term(pCard);

    if (! G.p_term->bIsValue(szValue))
      return C_CONTINUE_RECURSION;

    G.p_term->vValue(szWith);
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
void Xreshape::R_vUTE_Resolve_Param_Ordinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Stz* szParamNm = nullptr;
  Board* pSubQryBoard = m_pBench->pGetBoard(1);

  G_vPushBoard(pSubQryBoard);

  if (G_bEachMainWithSub(CK_PARAMLIST,CK_PARAM)) {
    szParamNm = G.p_param->szValue();
  }

  G_vPopBoard();

  if (! szParamNm) {
    p_term->vValue("((no-parm))"_Stz);
    return;
  }

  //p_term->vValue(szParamNm);

  if (! G_bResolveSubCard(CK_STMCOL,CK_STMCOLNM,szParamNm,G.i_MainCard)) {
    p_term->vValue("((unresolved))"_Stz);
    return;
  }

  RASSERT(m_pCard_resolved->bIsCard(CK_STMCOL));

  G.p_tblColOdn = m_pCard_resolved->pSub_tblColOdn();
  if (! G.p_tblColOdn) {
    p_term->vValue("((no-tblColOdn))"_Stz);
    return;
  }

  p_term->vValue(G.p_tblColOdn->szValue());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Xreshape::
R_pUTE_Resolve_table_By_tblSyn(char cTblSyn, i8 i_Before)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Table* p_table = nullptr;

  G_vPushIter();

  if (G_bResolveCard(CK_TABLE,cTblSyn,i_Before)) {
    RASSERT(m_pCard_resolved->bIsCard(CK_TABLE));

    p_table = _pCard_Table(m_pCard_resolved);
  }

  G_vPopIter();

  return p_table;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_StmColOdn* Xreshape::
R_pUTE_Resolve_tblColOdn_Into_stmColOdn(Card_Term* p_term_tblColOdn, i8 i_Before)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_StmColOdn* p_term_stmColOdn = nullptr;

  G_vPushIter();

  Stz* szTblColOdn = p_term_tblColOdn->szValue();
  if (G_bResolveSubCard(CK_STMCOL,CK_TBLCOLODN,szTblColOdn,i_Before)) {
    RASSERT(m_pCard_resolved->bIsCard(CK_STMCOL));

    p_term_stmColOdn = m_pCard_resolved->pSub_stmColOdn();
  }

  G_vPopIter();

  return p_term_stmColOdn;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Resolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(Card_BoolCalc *p_boolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [this,p_boolCalc](Card_PRED* p_pred,
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

        if (T_bValue_IsParamOrdinal(p_child->szValue())) {
          //R_vUTE_Resolve_Param_Ordinal(_pCard_Term(p_child));
          p_child->vValue("$1"_Stz);  // TODO: hack!!
        }
        else {
          //p_child->vValue("$1"_Stz);
          char cTblSyn = E_cValue_TableColSyn(p_child->szValue());
          cTblSyn = 'a'; // TODO: hack!!
          M_vCard_Set_TableColSyn(p_child,cTblSyn);
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

        if (T_bValue_IsSubQueryName(p_child->szValue())) {
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
void Xreshape::R_vUTE_Unproject_foldExpr_After_having(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);


  if (! G_bResolveCardWithSub(CK_GBYAGG, CK_AGGELIST, G.i_MainCard))
    return;

  G.p_gbyAgg->vDump();

  G.p_aggElist->vDump();

  G_vNewMember(CK_PROJECT);
  G.pDeck->vAppend(G.p_project);

  G_vNewMemberOf(CK_LEVEL,""_Stz,CK_PROJECT);
  G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  G_vNewMemberOf(CK_PROJELIST,""_Stz,CK_PROJECT);

  i8 i_Col = 0;
  i8 nChildren_aggElist = G.p_aggElist->nChildCards();
  for (i8 i_expr_aggElist = 0;
       i_expr_aggElist < nChildren_aggElist;
       i_expr_aggElist++) {

    i_Col++;

    G.p_expr = _pCard_Expr(G.p_aggElist->pGetChild(i_expr_aggElist));
    G.p_as = G.p_expr->pSub_as();

    if (T_bValue_IsSqyColOrdinal(G.p_as->szValue()))
      continue;

    Card_Expr* p_expr_simple = D_pProject_Simple_expr_Of(G.p_expr,i_Col);

    G_vAppendChild(CK_PROJELIST,p_expr_simple);
  }


  G.p_stream = C_pProduce_stream_Of_project(G.p_projElist);

  G.p_project->vAppend(G.p_stream);

  G_vNewMember(CK_COMMENT,"unproject of Sq1"_Stz);
  G.p_project->vAppend(G.p_comment);


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Unproject_foldExpr_After_where(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: remove? It is unused


  G_vNewMember(CK_PROJECT);
  G.pDeck->vInsertAfter(G.p_project,G.p_where);

  G_vNewMemberOf(CK_LEVEL,""_Stz,CK_PROJECT);
  G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  G_vNewMemberOf(CK_PROJELIST,""_Stz,CK_PROJECT);

  // TODO : unproject $1

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Yank_Correlated_cmpPred_From_sq_boolCalc(Card_BoolCalc* p_boolCalc,
                                                               Card_PRED*& rp_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: xt

  bool b = M_bBoolCalc_Yank_PRED(p_boolCalc, MARK_AT, rp_PRED);
  RASSERT(b);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_26_SubQuery_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10d_Main_Query_Analysis_Of_where_rpnCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  SubQuery* pSubQuery = m_pBench->pGetBoard((i8)1)->pSubQuery();

  pSubQuery->vCaller(0);

  // there is a subQuery, how is it called?

  pSubQuery->vClause(QC_WHERE_CLAUSE);

  CardIter xCI(G.p_rpnCalc);
  while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szValue_term = G.p_term->szValue();

    if (!G_bIsBoard(szValue_term))
      continue;

    if (! (G.p_term = _pCard_Term(xCI.pEachChild())))
      break;

    szValue_term = G.p_term->szValue();

    if (szValue_term->bSame("exists"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
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
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
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
    if (T_bValue_IsResultOrdinal(szValue_term)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (T_bValue_IsStreamColOrdinal(szValue_term)) {
          if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
            szValue_term = G.p_term->szValue();
            if (szValue_term->bSame("in"_Stz)) {
              if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
                szValue_term = G.p_term->szValue();
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

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10e_Main_Query_Analysis_Of_having_rpnCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  SubQuery* pSubQuery = m_pBench->pGetBoard((i8)1)->pSubQuery();

  pSubQuery->vCaller(0);

  // there is a subQuery, how is it called?

  pSubQuery->vClause(QC_HAVING_CLAUSE);

  CardIter xCI(G.p_rpnCalc);
  while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szValue_term = G.p_term->szValue();

    if (!G_bIsBoard(szValue_term))
      continue;

    if (! (G.p_term = _pCard_Term(xCI.pEachChild())))
      break;

    szValue_term = G.p_term->szValue();

    if (szValue_term->bSame("exists"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
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
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
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

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_33_To_TblColOdn */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S33_T10_Convert_stmColOdn_To_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);
    if (! T_bValue_IsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Stz* szStmColOdn = p_term->szValue();
    if (! G_bResolveSubCard(CK_STREAM,CK_STMCOLODN,szStmColOdn,G.i_MainCard))
      return C_CONTINUE_RECURSION;

    RASSERT(m_pCard_parent->bIsCard(CK_STMCOL));

    G.p_tblColOdn = m_pCard_parent->pSub_tblColOdn();
    M_vCard_Set_Value(p_term, G.p_tblColOdn->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_MainCard->bApplyRecursive(CK_TERM, bLambdaFn);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_34_Predicate_Pullup */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_36_Bound_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S36_T30_Delete_Empty_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  if (! T_bFilter_IsEmpty(G.p_filter))
    return;

  G.p_MainCard->vRemoveChild(G.p_filter);
  DELETE_CARD(G.p_filter);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_50_Join_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T30_Compute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_coPtnJoinList = new Card_CoPtnJoinList;
  G.pDeck->vAppendMain(G.p_coPtnJoinList);

  G.p_coPtnPredList =
      _pCard_CoPtnPredList(G.pDeck->pFindChild(CK_COPTNPREDLIST));

  i8 nChildren = G.p_coPtnPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    G.p_coPtnPred =
        _pCard_CoPtnPred(G.p_coPtnPredList->pGetChild(i_Child));

    G.p_subSet = G.p_coPtnPred->pDupSubCards();

    if (! (G.p_coPtnJoin = T_pCoPtnJoinList_Find_coPtn_for(G.p_coPtnJoinList, G.p_coPtnPred)) ) {
      G.p_coPtnJoin = new Card_CoPtnJoin;
      G.p_coPtnJoinList->vAppend(G.p_coPtnJoin);
    }

    G.p_coPtnJoin->vTransferChildren(G.p_subSet);
    DELETE_CARD(G.p_subSet);
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T40_Compute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_joinPredList =
      _pCard_JoinPredList(G.pDeck->pFindChild(CK_JOINPREDLIST));

  G.p_crossPtnJoinList = new Card_CrossPtnJoinList;
  G.pDeck->vAppendMain(G.p_crossPtnJoinList);

  i8 nChildren = G.p_joinPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    G.p_cmpPred =
        _pCard_CmpPred(G.p_joinPredList->pGetChild(i_Child));


    if (T_bCoPtnJoinList_HasEqPredCmp(G.p_coPtnJoinList, G.p_cmpPred))
      continue;

    G.p_crossPtnJoin = new Card_CrossPtnJoin;
    G.p_crossPtnJoin->vAppend(G.p_cmpPred->pDupCardAndSubCards());
    G.p_crossPtnJoinList->vAppend(G.p_crossPtnJoin);
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T50_Compute_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_ptndJoinSet = new Card_PtndJoinSet;
  G.pDeck->vAppendMain(G.p_ptndJoinSet);

  G.p_coPtnJoinList =
      _pCard_CoPtnJoinList(G.pDeck->pFindChild(CK_COPTNJOINLIST));
  G.p_subSet = G.p_coPtnJoinList->pDupSubCards();
  G.p_ptndJoinSet->vTransferChildren(G.p_subSet);
  DELETE_CARD(G.p_subSet);

  G.p_crossPtnJoinList =
      _pCard_CrossPtnJoinList(G.pDeck->pFindChild(CK_CROSSPTNJOINLIST));
  G.p_subSet = G.p_crossPtnJoinList->pDupSubCards();
  G.p_ptndJoinSet->vTransferChildren(G.p_subSet);
  DELETE_CARD(G.p_subSet);

  i8 nChildren = G.p_ptndJoinSet->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_pCard_child = G.p_ptndJoinSet->pGetChild(i_Child);
    m_pCard_child->vCard(CK_PTNDJOIN);
    m_pCard_child->Y_vSet(P_PtndJoin, i_Child);
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T60_Copy_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pBench->pCardCtl()->vNextHorizon(CK_PERMUTATION, true);

  G.p_ptndJoinSet =
      _pCard_PtndJoinSet(G.pDeck->pFindChild(CK_PTNDJOINSET));

  G.pDeck->vAppendMain(new Card_Note("PERMUTATIONS"_Stz));

  i8 i_PtdnJoin = 0;
  G.p_subSet = G.p_ptndJoinSet->pDupSubCards();
  while ((m_pCard_popped = G.p_subSet->pPop())) {
    G.pDeck->vAppendMain(m_pCard_popped);
  }
  DELETE_CARD(G.p_subSet);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T70_Generate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  m_pPermGen = new PermGen(G.p_ptndJoinSet->nChildCards());

  G.p_subSet = new Card_SubSet;

  while (m_pPermGen->bNextPermutation()) {
    G.pDeck->vAppendMain((G.p_permutation = new Card_Permutation));
    G.p_permutation->Y_vSet(P_Permutation, m_pPermGen->nPermutations() - 1);

    RASSERT(0 == G.p_subSet->nChildCards());
    for (i8 i_Item = 0; i_Item < m_pPermGen->nItems(); i_Item++) {
      i8 i_Child = m_pPermGen->iItem(i_Item);
      m_pCard_child = G.p_ptndJoinSet->pGetChild(i_Child);
      G.pDeck->vAppendMain(m_pCard_child->pDupCardAndSubCards());

      G.p_ptndJoin = _pCard_PtndJoin(m_pCard_child);
      if (!T_bPtndJoin_MayFollowAfter(G.p_ptndJoin, G.p_subSet)) {
        G.pDeck->vAppendMain(new Card_Note("**DOESN'T FOLLOW**"_Stz));
        G.p_permutation->Y_vSet(P_Invalid);
        break;
      }
      G.p_subSet->vAppend(m_pCard_child);
    }
    G.p_subSet->vRemoveAll();
  }

  DELETE_CARD(G.p_subSet);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T80_Remove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  i8 i_Main = 0;
  i8 i_Permutation = 0;
  while (i_Main < G.pDeck->nChildCards()) {
    G.p_MainCard = G.pDeck->pGetChild(i_Main);

    if (CK_PERMUTATION != G.p_MainCard->eCard()) {
      i_Main++;
      continue;
    }

    if (! G.p_MainCard->Y_bHas(P_Invalid)) {
      G.p_MainCard->Y_vSet(P_Permutation, i_Permutation++);
      i_Main++;
      continue;
    }


    G.pDeck->vRemoveChild(G.p_MainCard);
    while (i_Main < G.pDeck->nChildCards()) {
      G.p_MainCard = G.pDeck->pGetChild(i_Main);
      if (CK_PTNDJOIN != G.p_MainCard->eCard() && CK_NOTE != G.p_MainCard->eCard())
        break;
      G.pDeck->vRemoveChild(G.p_MainCard);
      delete G.p_MainCard;
    }
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T92_Estimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.p_tableList =
      _pCard_TableList(G.pDeck->pFindChild(CK_TABLELIST));


  BoardIter xBI(G.pBoard);

//  while (G.p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN))) {
  while ((G.p_MainCard = xBI.pEachMain())) {
    eCK eCard = G.p_MainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      G.p_tableList->vDeletePropertyRecursive(P_TableUsed);
      continue;
    }
    if (CK_PTNDJOIN != eCard)
      continue;


    G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    D_vDerive_PtndJoin(G.p_ptndJoin, G.p_tableList);
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T94_Estimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  BoardIter xBI(G.pBoard);

  i8 nNetwork = 0;
  i8 nSentOut = 0;
  G.p_permutation = nullptr;
  G.p_ptndJoin    = nullptr;

  while ((G.p_MainCard = xBI.pEachMain())) {


    eCK eCard = G.p_MainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      if (G.p_permutation)
        G.p_permutation->Y_vSet(P_Network, nNetwork);
      G.p_permutation = _pCard_Permutation(G.p_MainCard);

      G.p_ptndJoin = nullptr;
      nNetwork = 0;
      nSentOut = 0;
    }
    else
    if (CK_PTNDJOIN == eCard) {
      if (G.p_ptndJoin) {
        nSentOut += G.p_ptndJoin->Y_iGet(P_JoinedBuff);
        nNetwork += nSentOut;
        G.p_ptndJoin->Y_vSet(P_SentOut, nSentOut);
      }
      G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    }
  }

  if (G.p_permutation)
    G.p_permutation->Y_vSet(P_Network, nNetwork);


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T96_Estimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  i8 iLowestNetwork = 999999999;

  BoardIter xBI(G.pBoard);

  while ((G.p_permutation =
              _pCard_Permutation(xBI.pEachMain(CK_PERMUTATION)))) {
    i8 iNetwork = G.p_permutation->Y_iGet(P_Network);
    if (iNetwork >= iLowestNetwork)
      continue;
    G.p_permutation_low = G.p_permutation;
    iLowestNetwork = iNetwork;
  }

  G.pDeck->vAppendMain((G.p_joinOrderSeq = new Card_JoinOrderSeq));
  G.p_joinOrderSeq->Y_vSet(P_Network, iLowestNetwork);

  xBI.vSet(G.p_permutation_low);
  while ((G.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != G.p_MainCard->eCard())
      break;

    G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    D_vDerive_JoinOrderSeq(G.p_joinOrderSeq, G.p_ptndJoin);
  }


  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S50_T98_Extract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  G.pDeck->vAppendMain((G.p_joinOrderSeq = new Card_JoinOrderSeq));

  BoardIter xBI(G.pBoard);

  xBI.vSet(G.p_permutation_low);
  while ((G.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != G.p_MainCard->eCard())
      break;


    G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    D_vDerive_JoinOrderSeq(G.p_joinOrderSeq, G.p_ptndJoin);
  }

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T10_Morph_table_To_EEop_v2(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SCAN
  if (0 == G.i_MainCard)  {
    G.p_scan = new Card_Scan;
    G.p_scan->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table,G.p_scan);
    G.p_scan->vInsert(G.p_table);

    if (G.p_indexJoin) {
      NEVER_REACH
      G.p_scan->vRemoveChild(G.p_indexJoin);
      DELETE_CARD(G.p_indexJoin);
    }
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // KEYEXISTS
  else if (G.p_keyExists) {
    R_vUTE_Choose_index_For_keyExists();

    G.p_table->vRemoveSubCard(G.p_keyExists);

    G.p_keyExists->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_keyExists);
    G.p_keyExists->vInsert(G.p_table);

    G.p_keyExists->vRemoveSubCard(G.p_index);
    G.p_keyExists->vAppend(G.p_index);
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // KEYNOTEXISTS
  else if (G.p_keyNotExists) {
    R_vUTE_Choose_index_For_keyNotExists();

    G.p_table->vRemoveSubCard(G.p_keyNotExists);

    G.p_keyNotExists->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_keyNotExists);
    G.p_keyNotExists->vInsert(G.p_table);

    G.p_keyNotExists->vRemoveSubCard(G.p_index);
    G.p_keyNotExists->vAppend(G.p_index);
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CROSSJOIN
  else if (! G.p_bounds) {
    R_vUTE_Choose_index_For_crossJoin();

    G.p_crossJoin = new Card_CrossJoin;
    G.p_crossJoin->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table,G.p_crossJoin);
    G.p_crossJoin->vInsert(G.p_table);

    G_vNewMember(CK_BOUNDS);
    G.p_crossJoin->vAppend(G.p_bounds);

    G_vNewMember(CK_XBOUND);
    G.p_bounds->vAppend(G.p_xBound);

    G_vNewMember(CK_TERM,"bound"_Stz);
    G.p_xBound->vAppend(G.p_term);

    G_vNewMember(CK_TERM,"$I1"_Stz);
    G.p_xBound->vAppend(G.p_term);
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN
  else {
    R_vUTE_Choose_index_For_indexJoin();

    G_vNewMember(CK_INDEXJOIN);

    G.p_indexJoin->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_indexJoin);
    G.p_indexJoin->vInsert(G.p_table);

    //G.p_indexJoin->vRemoveSubCard(G.p_index);
    //G.p_indexJoin->vAppend(G.p_index);
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T30_Correct_stmColOdn_Of_keyExists_stream(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  i8 iOrdLo_stream = E_iStream_OdnLo(G.p_stream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this, iOrdLo_stream](Card* pCard)
  {
    Card_StmColOdn* p_stmColOrd = _pCard_StmColOdn(pCard);
    if (! T_bValue_IsStreamColOrdinal(p_stmColOrd->szValue()))
      return C_CONTINUE_RECURSION;

    i8 iStmColOdn = E_iValue_StreamColOdn(p_stmColOrd->szValue());
    i8 iStmColOdn_corrected = iOrdLo_stream + iStmColOdn - 1;
    M_vCard_Set_StreamColOrdinal(p_stmColOrd, iStmColOdn_corrected);

    p_stmColOrd->vMark1(MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_stream->bApplyRecursive(CK_STMCOLODN, bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  i8 i_Main = G.pDeck->iFindChild(G.p_MainCard);

  Card_BoolCalc* p_boolCalc = G.p_constraint->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,i_Main](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! T_bValue_IsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    G_bResolveSubCard(CK_STMCOL, CK_STMCOLODN, p_term->szValue(), i_Main + 1);

    Card_StmColNm* p_stmColNm = G.p_stmCol->pSub_stmColNm();

    M_vCard_Set_Value(p_term,p_stmColNm->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = M_bPredRecursive(p_pred_ROOT,bLambdaFn);

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_80_Index_Scan */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_85_Cross_Join */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_90_Stream_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_A0_Filter_Refinement */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SA0_T10_Individuate_indexJoin_filter_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // todo: xt

  G.p_indexJoin->vRemoveChild(G.p_filter);
  G.pDeck->vInsertAfter(G.p_filter, G.p_indexJoin);

  if (! G.p_filter->pSub_level()) {
    G.p_filter->vPush(G.p_indexJoin->pSub_level()->pDupCardAndSubCards());
  }

  vEndReshape();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SA0_T10c_Individuate_crossJoin_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vReshape(__func__);

  // todo: xt

  G.p_crossJoin->vRemoveChild(G.p_filter);
  G.pDeck->vInsertAfter(G.p_filter, G.p_crossJoin);

  if (! G.p_filter->pSub_level()) {
    G.p_filter->vPush(G.p_crossJoin->pSub_level()->pDupCardAndSubCards());
  }

  vEndReshape();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T20_Reinstate_stagesV3(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vReshape(__func__);

  i8 nStages = 0;

  Card_Stage* p_stage = new Card_Stage;
  p_stage->vValueSprint("S%02lld"_Stz,++nStages);
  Card_Topology* p_topology = new Card_Topology(YG_ALL_PARTITIONS);
  p_stage->vAppend(p_topology);
  G.pDeck->vPush(p_stage);

  eAL e_level_running = M_eStageTopology_To_Level(YG_ALL_PARTITIONS);

  bool bSawAggOp = false;
  bool bSawIndexJoinOp = false;
  bool bSawProjectOp = false;
  bool bSawSortOp = false;

  auto vAddCombined = [this] (Card* p_main, i8& ri_main) {
    Card_Combined* p_combined = new Card_Combined;
    if (C_UNKNOWN == ri_main) {
      G.pDeck->vAppend(p_combined);
      return;
    }
    G.pDeck->vInsertBefore(p_combined, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  auto vAddNext = [this] (Card* p_main, i8& ri_main, Stz* szNextValue) {
    Card_Next* p_next = new Card_Next;
    p_next->vValue(szNextValue);
    if (C_UNKNOWN == ri_main) {
      G.pDeck->vAppend(p_next);
      return;
    }
    G.pDeck->vInsertBefore(p_next, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  auto vAddStage = [this] (Card* p_main, i8& ri_main, eAL e_level_running, i8& nStages) {
    Card_Stage *p_stage = new Card_Stage;
    p_stage->vValueSprint("S%02lld"_Stz,++nStages);
    eYG eStageTopo = M_eLevel_To_StageTopology(e_level_running);
    Card_Topology *p_topology = new Card_Topology(eStageTopo);
    p_stage->vAppend(p_topology);
    G.pDeck->vInsertBefore(p_stage, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  G_vRestart();
  if (G_bEachMain(CK_FOLD))
    G_vRemoveDeleteMain(CK_FOLD);

  Card* p_prev = nullptr;
  eCK   e_prev = CK_NONE;
  Card* p_main = p_stage;
  eCK   e_main = p_main->eCard();

  for (i8 i_Main = 1; i_Main < G.pDeck->nChildCards(); i_Main++) {

    p_prev = p_main;
    e_prev = e_main;

    p_main = G.pDeck->pGetChild(i_Main);
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
      if (CK_GBYAGG == e_main || CK_AGGREG == e_main)
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
        && (bSawAggOp || bSawIndexJoinOp || bSawProjectOp) ) {
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

    if (CK_GBYAGG == e_main && (bSawAggOp || bSawIndexJoinOp || bSawProjectOp)) {
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

    if (CK_GBYAGG == e_main)
      bSawAggOp = true;
    if (CK_PROJECT == e_main || CK_AGGREG == e_main)
      bSawProjectOp = true;
  }

  i8 i_End = C_UNKNOWN;
  if (CK_SCAN != e_prev)
    vAddCombined(p_main, i_End);
  vAddNext(p_main, i_End,""_Stz);

  vEndReshape();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
