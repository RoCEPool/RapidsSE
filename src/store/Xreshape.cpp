/*============================================================================*/
/* Xreshape.cpp --                                                       */
/*============================================================================*/

#include "Xreshape.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"


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
/*----------------------------------------------------------------------------*/
/*                                                    Reduce Pipeline Hand 10 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T10_Delete_Redundant_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vRemoveDeleteMain(CK_STAGE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T20_Hide_Ignorable_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! T_bStage_IsIgnorable(G.p_stage))
    return;

  G.p_stage->Y_vSet(P_HideHeat);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T30_Delete_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vRemoveDeleteMain(CK_ABSJOIN);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T40_Delete_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vRemoveDeleteMain(CK_COMBINED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T50_Delete_send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vRemoveDeleteMain(CK_SEND);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T60_Cull_scan_Leaving_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.pDeck->vReplaceChildWithItsSubs(G.p_scan);
  DELETE_CARD(G.p_scan);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     Table Analysis Hand 20 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H20_T10_AAffix_tableSyn_To_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vTable_Set_Syn(G.p_table);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H20_T20_Produce_tablePtn_Under_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_tablePtn = C_pProduce_tablePtn_ofTable(G.p_table);

  G.p_table->vAppend(G.p_tablePtn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H20_T30_Produce_stream_Under_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pTable = A_pAccess_Table(G.p_tableNm->sz());
  G.p_stream = C_pProduce_stream_ofTable(m_pTable);

  G.p_table->vAppend(G.p_stream);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Stream Analysis Hand 30 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H30_T10_Derive_ordinalRange_For_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vStream_Derive_OrdinalRange(G.p_stream, G.p_table->cValue());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Predicate Pullup Hand 40 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H30_T20_Convert_where_rpnCalc_To_boolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_boolCalc = C_pConvert_rpnCalc_To_boolCalc(G.p_rpnCalc);

  G.p_where->vReplaceSubCard(G.p_rpnCalc,G.p_boolCalc);

  G_vDeleteMember(G.p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H30_T40_Convert_stmColOdn_To_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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

  G.p_MainCard->vApplyRecursive(CK_TERM, bLambdaFn);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T10_AtMark_where_pred_Wrt_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vPullUp_MarkPredLeftwards(G.p_boolCalc, G.pDeck);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T20_Move_AtMarked_where_pred_To_table_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vPullUp_MovePredLeftwards(G.p_boolCalc, G.pDeck);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T30_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (0 == G.p_boolCalc->nChildCards()) {
    G_vRemoveDeleteMain(CK_WHERE);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                       Flatten Call Hand 50 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T10_Extract_subSet_Of_call(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_subSet = E_pSubset_ChildrenBetween(G.pDeck, 1);

  G.pDeck->vPush(G.p_subSet);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T20_Yank_Subset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCard_yanked = G.pDeck->pFindChild(CK_SUBSET);

  G.pDeck->vRemoveChild(m_pCard_yanked);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T20_Put_Yanked(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.pDeck->vInsertAfter(m_pCard_yanked,G.p_call);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_yanked);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T30_RemoveDelete_call(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G_vRemoveDeleteMain(CK_CALL);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T40(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T50(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T60(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    Flatten Exists Hand 55 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H55_T10_Collapse_Exists_SubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vNewMember(CK_KEYEXISTS);

  G.pDeck->vInsertAfter(G.p_keyExists,G.p_stage);

  RASSERT(G_bEachMain());

  while (G_bSubsequentToMain(C_UPDATE_MEMBER)) {
    G.pDeck->vRemove(G.p_SubsequentToMain);
    G_vAppendChild(CK_KEYEXISTS, G.p_SubsequentToMain);
  }
//  G.pDeck->vRenumberRecursive();
//  G.pDeck->vDump();

  Card_Table* p_table = _pCard_Table(G.p_keyExists->pFindChild(CK_TABLE));
  G.p_keyExists->vRemoveChild(p_table);

  G.pDeck->vReplaceChild(G.p_keyExists,p_table);

  p_table->vAppend(G.p_keyExists);

  p_table->vPromoteSubs(G.p_keyExists);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H55_T20_Copy_Up_SubQuery(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCard_temp = G.p_table->pDupCardAndSubCards();
  G_vPopIter();

  G.pDeck->vInsertBefore(m_pCard_temp,G.p_where);
  G_vSkipNextMain();

  //vPrintBoard(false,__LINE__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H55_T30_Adjust_Stream_OrdinalRange(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // CK_TABLE, CK_KEYEXISTS

  Card_Stream* p_stream_keyExists = G.p_table->pSub_stream();

  if (! G_bResolveCardWithSub(CK_TABLE,CK_STREAM,m_pCard_temp))
    return;

  i8 iOrdLo = E_iStream_OrdLo(p_stream_keyExists);
  i8 iOrdHi = E_iStream_OrdHi(p_stream_keyExists);

  i8 iOrdLo_adjusted = E_iStream_OrdHi(G.p_stream) + iOrdLo;
  i8 iOrdHi_adjusted = E_iStream_OrdHi(G.p_stream) + iOrdHi;

  M_vStream_Adjust_OrdinalRange(p_stream_keyExists, iOrdLo_adjusted, iOrdHi_adjusted);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H55_T40_Correct_BoolCalc_Terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // CK_TABLE, CK_KEYEXISTS, CK_BOOLCALC

  Card_Stream* p_stream_keyExists = G.p_table->pSub_stream();
  i8 iStmOrdLo_stream = E_iStream_OrdLo(p_stream_keyExists);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfCorrect_StmColOdn = [this, iStmOrdLo_stream](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);
    if (! T_bValue_IsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    i8 iStmColOdn_term = E_iTerm_StreamColOdn(p_term);
    i8 iStmColOdn_corrected = iStmOrdLo_stream + iStmColOdn_term - 1;
    M_vCard_Set_StreamColOrdinal(p_term, iStmColOdn_corrected);

    // TODO: M_vTerm_MarkAs_Changed();
    p_term->vMark1(MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_boolCalc->vApplyRecursive(CK_TERM, bfCorrect_StmColOdn);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H55_T50_Produce_Parameter_Linkage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // CK_TABLE, CK_KEYEXISTS

  vTracer("Resolving %s"_Stz,G.p_MainCard->szBearing());

  // find under main: <table>//<param>
  if (!G_bPlumbForSubMember(CK_TABLE, CK_PARAM))
    return;

  // find left of main: <stmCol> == <param>
  if (!G_bResolveCard(CK_STMCOL, G.p_param->sz(), G.p_table))
    return;

  // find sub <stmCol>//<tblColOdn>=$c.n
  G.p_tblColOdn = G.p_stmCol->pSub_tblColOdn();

  // add <table>//<param>/<tblColOdn>=$c.n
  G.p_param->vAppend(G.p_tblColOdn->pDupCardAndSubCards());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H55_T60_Substitute_Parameter_Terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vBoolCalc_Substitute_Parameters(G.p_boolCalc, G.p_paramList);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H55_T70_Cull_Marked_CallSub(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vBoolCalc_CullPred(G.p_boolCalc,MARK_CALLSUB);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H55_T80_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (0 == G.p_boolCalc->nChildCards()) {
    G_vRemoveDeleteMain(CK_WHERE);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                         Index Join Hand 60 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T10_Produce_indexJoin_Under_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vTable_Produce_IndexJoin(G.p_table);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T20_Affix_tblColOdn_To_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_stream = G.p_table->pSub_stream();;
  G.p_stmColList = G.p_table->pSub_stmColList();
  G.p_idxColList = G.p_indexJoin->pSub_idxColList();

  M_vIdxColList_Affix_tblColOdn(G.p_idxColList, G.p_stream);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T30_UpCopy_filter_pred_Into_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  C_vBounds_DeriveFrom_filter(G.p_table, G.p_filter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T40_Cull_UpCopied_filter_pred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_boolCalc = G.p_filter->pSub_boolCalc();

  M_vBoolCalc_CullPred(G.p_boolCalc, MARK_UP);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T50_Delete_Empty_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! T_bFilter_IsEmpty(G.p_filter))
    return;

  G.p_MainCard->vRemoveChild(G.p_filter);
  DELETE_CARD(G.p_filter);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     Join Ordering Hand 70 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T00_Extract_tableList_From_queryBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_tableList->vAppend(G.p_table->pDupCardAndSubCards());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T10_Extract_joinPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  D_vDerive_JoinPredList(G.p_joinPredList, G.p_boolCalc);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T20_Compute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);


  G.p_coPtnPred = new Card_CoPtnPred;
  G.p_coPtnPred->vAppend(G.p_cmpPred->pDupCardAndSubCards());

  G.p_coPtnPredList->vAppend(G.p_coPtnPred);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T30_Compute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T40_Compute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T50_Compute_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T60_Copy_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pBench->pCardCtl()->vNextHorizon(CK_PERMUTATION, true);

  G.p_ptndJoinSet =
      _pCard_PtndJoinSet(G.pDeck->pFindChild(CK_PTNDJOINSET));

  G.pDeck->vAppendMain(new Card_Comment("PERMUTATIONS"_Stz));

  i8 i_PtdnJoin = 0;
  G.p_subSet = G.p_ptndJoinSet->pDupSubCards();
  while ((m_pCard_popped = G.p_subSet->pPop())) {
    G.pDeck->vAppendMain(m_pCard_popped);
  }
  DELETE_CARD(G.p_subSet);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T70_Generate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
        G.pDeck->vAppendMain(new Card_Comment("**DOESN'T FOLLOW**"_Stz));
        G.p_permutation->Y_vSet(P_Invalid);
        break;
      }
      G.p_subSet->vAppend(m_pCard_child);
    }
    G.p_subSet->vRemoveAll();
  }

  DELETE_CARD(G.p_subSet);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T80_Remove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Main = 0;
  i8 i_Permutation = 0;
  while (i_Main < G.pDeck->nChildCards()) {
    G.p_MainCard = G.pDeck->pGetChild(i_Main);

    if (CK_PERMUTATION != G.p_MainCard->eCard()) {
      i_Main++;
      continue;
    }

    if (! G.p_MainCard->bHasProperty(P_Invalid)) {
      G.p_MainCard->Y_vSet(P_Permutation, i_Permutation++);
      i_Main++;
      continue;
    }


    G.pDeck->vRemoveChild(G.p_MainCard);
    while (i_Main < G.pDeck->nChildCards()) {
      G.p_MainCard = G.pDeck->pGetChild(i_Main);
      if (CK_PTNDJOIN != G.p_MainCard->eCard() && CK_COMMENT != G.p_MainCard->eCard())
        break;
      G.pDeck->vRemoveChild(G.p_MainCard);
      delete G.p_MainCard;
    }
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T90_Setup_costEst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((G.p_MainCard = G.pDeck->pGetChild(0))) {

    if (CK_PERMUTATION == G.p_MainCard->eCard())
      break;

    G.pDeck->vRemoveChild(G.p_MainCard);
    delete G.p_MainCard;
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T92_Estimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T94_Estimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
        nSentOut += G.p_ptndJoin->Y_iVal(P_JoinedBuff);
        nNetwork += nSentOut;
        G.p_ptndJoin->Y_vSet(P_SentOut, nSentOut);
      }
      G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    }
  }

  if (G.p_permutation)
    G.p_permutation->Y_vSet(P_Network, nNetwork);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T96_Estimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 iLowestNetwork = 999999999;

  BoardIter xBI(G.pBoard);

  while ((G.p_permutation =
              _pCard_Permutation(xBI.pEachMain(CK_PERMUTATION)))) {
    i8 iNetwork = G.p_permutation->Y_iVal(P_Network);
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

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T98_Extract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.pDeck->vAppendMain((G.p_joinOrderSeq = new Card_JoinOrderSeq));

  BoardIter xBI(G.pBoard);

  xBI.vSet(G.p_permutation_low);
  while ((G.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != G.p_MainCard->eCard())
      break;


    G.p_ptndJoin = _pCard_PtndJoin(G.p_MainCard);
    D_vDerive_JoinOrderSeq(G.p_joinOrderSeq, G.p_ptndJoin);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H75_T10_Morph_table_To_EEop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! G.p_bounds) {

    G.p_scan = new Card_Scan;
    G.p_scan->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table,G.p_scan);
    G.p_scan->vInsert(G.p_table);

    if (G.p_indexJoin) {
      G.p_scan->vRemoveChild(G.p_indexJoin);
      DELETE_CARD(G.p_indexJoin);
    }
    return;
  }

  if (G.p_keyExists) {
    G.p_table->vRemoveSubCard(G.p_keyExists);

    G.p_table->vRemoveSubCard(G.p_indexJoin);
    G.p_indexJoin->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_indexJoin);
    G.p_indexJoin->vInsert(G.p_table);

    G.p_indexJoin->vRemoveSubCard(G.p_index);
    G.p_indexJoin->vAppend(G.p_index);

    G.p_indexJoin->vCard(CK_KEYEXISTS);
    DELETE_CARD(G.p_keyExists);
    return;
  }

  if (G.p_indexJoin) {
    G.p_table->vRemoveSubCard(G.p_indexJoin);
    G.p_indexJoin->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_indexJoin);
    G.p_indexJoin->vInsert(G.p_table);

    G.p_indexJoin->vRemoveSubCard(G.p_index);
    G.p_indexJoin->vAppend(G.p_index);
    return;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H75_T10_Morph_table_To_EEop_b(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 i_Child;
  G.p_comment = _pCard_Comment(G.p_scan->pFindChild(CK_COMMENT,i_Child));

  if (i_Child == G.p_scan->nChildCards() - 1) {
    G.p_scan->vRemoveChild(G.p_comment);
    DELETE_CARD(G.p_comment);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H75_T10_Morph_table_To_EEop_c(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 i_Child;
  G.p_comment = _pCard_Comment(G.p_indexJoin->pFindChild(CK_COMMENT,i_Child));
  G.p_comment->vValue(SEP_DERIVED);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H75_T20_Morph_keyExists_filter_To_constraint(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  G.p_filter->vCard(CK_CONSTRAINT);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H75_T30_Correct_stmColOdn_Of_keyExists_stream(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 iOrdLo_stream = E_iStream_OrdLo(G.p_stream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this, iOrdLo_stream](Card* pCard)
  {
    Card_StmColOdn* p_stmColOrd = _pCard_StmColOdn(pCard);
    if (! T_bValue_IsStreamColOrdinal(p_stmColOrd->szValue()))
      return C_CONTINUE_RECURSION;

    i8 iStmColOdn = E_iValue_StreamColOdn(p_stmColOrd->szValue());
    i8 iStmColOdn_corrected = iOrdLo_stream + iStmColOdn - 1;
    M_vCard_Set_StreamColOrdinal(p_stmColOrd, iStmColOdn_corrected);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_stream->vApplyRecursive(CK_STMCOLODN, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

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
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                        Index Scan Hand 80 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T10_Produce_indexList_Under_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vScan_Produce_indexList(G.p_scan);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T20_Affix_idxCol_name_Of_scan_To_ordinal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vIdxColList_Affix_tblColOdn(G.p_idxColList, G.p_stream);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T30_Mark_ULBoundsOf_scan_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfMarkPredBounds = [this](Card* pCard) {
    Card_CmpPred* p_cmpPred = _pCard_CmpPred(pCard);
    M_vCmpPred_MarkBound(p_cmpPred);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_filter->vApplyRecursive(CK_CMPPRED, bfMarkPredBounds);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T40_Mark_IndexMatchOf_scan_filter_terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! (G.p_index = _pCard_Index(G.p_indexList->pGetChild(0))))
    return;
  if (! (G.p_idxColList = (G.p_index->pSub_idxColList())))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfPredBounds = [this](Card* pCard) {
    Card_Term* p_term = _pCard_Term(pCard);
    M_vTerm_MarkIfMatchesAn_idxCol(p_term, G.p_idxColList);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_filter->vApplyRecursive(CK_TERM, bfPredBounds);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfAdd = [this](Card* pCard) {
    G.p_cmpPred = _pCard_CmpPred(pCard);
    M_vScan_Add_cmpPred_To_bounds(G.p_scan, G.p_cmpPred);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_filter->vApplyRecursive(CK_CMPPRED, bfAdd);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T60_Cull_Marked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTracer("Culling <filter> of L & U bounds %s\n"_Stz,G.p_filter->szBearing());
  M_vFilter_CullPred(G.p_filter, 'L');
  M_vFilter_CullPred(G.p_filter, 'U');

  //G.p_filter->vDump();
  if (!T_bFilter_IsEmpty(G.p_filter))
    return;

  vTracer("Deleting empty <filter> %s\n"_Stz,G.p_filter->szBearing());
  G.p_MainCard->vRemoveSubCard(G.p_filter);
  DELETE_CARD(G.p_filter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T70_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_scan->vCard(CK_INDEXSCAN);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T70_Morph_scan_To_indexScan_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Child;
  G.p_comment = _pCard_Comment(G.p_indexScan->pFindChild(CK_COMMENT,i_Child));
  G.p_comment->vValue(SEP_DERIVED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T70_Morph_scan_To_indexScan_c(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_indexScan->vReplaceChildWithItsSubs(G.p_indexList);
  DELETE_CARD((G.p_indexList));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Stream Ordering Hand 90 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H90_T10_Remove_sort_IfSameAs_gby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_gbyElist = G.p_aggreg->pSub_gbyElist();
  G.p_sbyElist = G.p_sort->pSub_sbyElist();

  // TODO: needs to be a T_ test
  if (! G.p_gbyElist->bValuesSameRecursive(G.p_sbyElist))
    return;

  G_vRemoveDeleteMain(CK_SORT);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 Filter Refinement Hand A0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HA0_T10_Individuate_scan_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_scan->vRemoveChild(G.p_filter);
  G.pDeck->vInsertAfter(G.p_filter, G.p_scan);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HA0_T10_Individuate_indexJoin_filter_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_indexJoin->vRemoveChild(G.p_filter);
  G.pDeck->vInsertAfter(G.p_filter, G.p_indexJoin);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                            Restore Stream Ordinals Hand B0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HZ0_T10_Restore_stmColOdn_from_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);
    if (! T_bValue_IsTableColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Stz* szTblColOdn = p_term->szValue();
    if (! G_bResolveSubCard(CK_TABLE,CK_TBLCOLODN,szTblColOdn,G.i_MainCard))
      return C_CONTINUE_RECURSION;

    RASSERT(m_pCard_parent->bIsCard(CK_STMCOL));

    G.p_stmColOdn = m_pCard_parent->pSub_stmColOdn();
    M_vCard_Set_Value(p_term, G.p_stmColOdn->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_MainCard->vApplyRecursive(CK_TERM, bLambdaFn);
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Restate Pipeline Hand Z0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HZ0_T20_Reinstate_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! T_bStage_Reinstate(G.e_MainCard, G.e_SubsequentToMain))
    return;

  m_pCard_dup = G.p_stage->pDupCardAndSubCards();
  G.pDeck->vInsertAfter(m_pCard_dup, G.p_MainCard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HZ0_T30_Reinstate_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G.p_combined = new Card_Combined;
  G.pDeck->vInsertAfter(G.p_combined, G.p_MainCard);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vRR_Convert_All_TblColOrd_to_StmColOrd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    if (C_YES == T_iCard_ConvertableTo_StmColOdn(pCard)) {
      if (G_bLocateCard(CK_STREAM,CK_TBLCOLODN,pCard->szValue(),C_UNKNOWN)) {
        G.p_stmColOdn = G.p_SubLocatedParent->pSub_stmColOdn();
        M_vCard_Set_Value(pCard,G.p_stmColOdn->szValue());
      }
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.pDeck->vApplyRecursive(CK_NONE,bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vRR_Convert_All_StmColOrd_to_TblColOrd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    if (C_YES == T_iCard_ConvertableTo_TblColOdn(pCard)) {
      if (G_bLocateCard(CK_STREAM,CK_STMCOLODN,pCard->szValue(),C_UNKNOWN)) {
        G.p_tblColOdn = G.p_SubLocatedParent->pSub_tblColOdn();
        M_vCard_Set_Value(pCard,G.p_tblColOdn->szValue());
      }
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.pDeck->vApplyRecursive(CK_NONE,bLambdaFn);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
