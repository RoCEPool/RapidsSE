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
void Xreshape::vR_H10_T15_Hide_Ignorable_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! T_bStage_IsIgnorable(M.p_stage))
    return;

  M.p_stage->Y_vSet(P_HideHeat);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T20_Delete_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vRemoveDeleteMain(CK_ABSJOIN);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T30_Delete_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vRemoveDeleteMain(CK_COMBINED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T40_Delete_send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vRemoveDeleteMain(CK_SEND);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T50_Cull_scan_Leaving_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.pDeck->vReplaceChildWithItsSubs(M.p_scan);
  DELETE_CARD(M.p_scan);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     Table Analysis Hand 20 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H20_T10_Add_tableSyn_ForAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vTable_syn(M.p_table);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H20_T20_Add_tablePtn_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_tablePtn = C_pProduce_tablePtn_ofTable(M.p_table);

  M.p_table->vAppend(M.p_tablePtn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H30_T10_Add_stream_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pTable = A_pAccess_Table(M.p_tableNm->sz());
  M.p_stream = C_pProduce_stream_ofTable(m_pTable);

  M.p_table->vAppend(M.p_stream);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Stream Analysis Hand 30 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H30_T20_Calc_ordinalRange_ForAll_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vStream_OrdinalRange(M.p_stream);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Predicate Pullup Hand 40 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T10_Convert_where_rpnCalc_To_boolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_boolCalc = C_pConvert_rpnCalc_To_boolCalc(M.p_rpnCalc);

  M.p_where->vReplaceSubCard(M.p_rpnCalc,M.p_boolCalc);

  G_vDeleteMember(M.p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T20_Mark_where_pred_Wrt_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vPullUp_MarkPredLeftwards(M.p_boolCalc, M.pDeck);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T30_Move_Marked_where_pred_To_table_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vPullUp_MovePredLeftwards(M.p_boolCalc, M.pDeck);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T40_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (0 == M.p_boolCalc->nChildCards()) {
    G_vRemoveDeleteMain(CK_WHERE);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    Flatten Exists Hand 50 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T10_Rework_Exists_SubQueries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  G_vNewMember(CK_KEYEXISTS);

  M.pDeck->vInsertAfter(M.p_keyExists,M.p_stage);

  RASSERT(G_bEachMain());

  while (G_bSubsequentToMain(C_UPDATE_MEMBER)) {
    M.pDeck->vRemove(M.p_SubsequentToMain);
    G_vAppendChild(CK_KEYEXISTS, M.p_SubsequentToMain);
  }
//  M.pDeck->vRenumberRecursive();
//  M.pDeck->vDump();

  Card_Table* p_table = _pCard_Table(M.p_keyExists->pFindChild(CK_TABLE));
  M.p_keyExists->vRemoveChild(p_table);

  M.pDeck->vReplaceChild(M.p_keyExists,p_table);

  p_table->vAppend(M.p_keyExists);

  p_table->vPromoteSubs(M.p_keyExists);

  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T20_Copy_Up_SubQueries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCard_temp = M.p_table->pDupCardAndSubCards();
  G_vPopIter();
  M.pDeck->vInsertBefore(m_pCard_temp,M.p_where);
  M.p_tstPred->vMark1(MARK_LEFTWARDS);
  G_vSkipNextMain();
  //vPrintBoard(false,__LINE__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T30_Resolve_Parameter_Linkage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_stmColOrd = M.p_stmCol->pSub_stmColOrd();
  M.p_stmColOrd->vValue(M.p_stmColOrd->szValue());
  M.p_param->vAppend(M.p_stmColOrd);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T40_Resolve_Parameter_Terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_stmColOrd = M.p_param->pSub_stmColOrd();
  M.p_term->vValue(M.p_stmColOrd->szValue());

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      Initial Join Hand 60 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T10_Add_indexJoin_UnderFiltered_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vTable_IndexJoin(M.p_table);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T20_Convert_idxCol_name_Of_table_To_ordinal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_stream = M.p_table->pSub_stream();;
  M.p_stmColList = M.p_table->pSub_stmColList();
  M.p_idxColList = M.p_indexJoin->pSub_idxColList();

  M_vIdxColList_ToOrdinals(M.p_idxColList, M.p_stream);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T30_Add_bounds_UnderFiltered_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vTable_Bounds(M.p_table, M.p_filter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T40_Cull_filter_pred_MovedUpTo_joinPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_boolCalc = M.p_filter->pSub_boolCalc();

  M_vBoolCalc_CullPred(M.p_boolCalc, MARK_UPWARDS);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T50_Delete_Empty_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! T_bFilter_IsEmpty(M.p_filter))
    return;

  M.p_MainCard->vRemoveChild(M.p_filter);
  DELETE_CARD(M.p_filter);
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

  M.p_tableList->vAppend(M.p_table->pDupCardAndSubCards());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T10_Extract_joinPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  D_vDerive_JoinPredList(M.p_joinPredList, M.p_boolCalc);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T20_Compute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);


  M.p_coPtnPred = new Card_CoPtnPred;
  M.p_coPtnPred->vAppend(M.p_cmpPred->pDupCardAndSubCards());

  M.p_coPtnPredList->vAppend(M.p_coPtnPred);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T30_Compute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_coPtnJoinList = new Card_CoPtnJoinList;
  M.pDeck->vAppendMain(M.p_coPtnJoinList);

  M.p_coPtnPredList =
      _pCard_CoPtnPredList(M.pDeck->pFindChild(CK_COPTNPREDLIST));

  i8 nChildren = M.p_coPtnPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    M.p_coPtnPred =
        _pCard_CoPtnPred(M.p_coPtnPredList->pGetChild(i_Child));

    M.p_subSet = M.p_coPtnPred->pDupSubCards();

    if (! (M.p_coPtnJoin = T_pCoPtnJoinList_Find_coPtn_for(M.p_coPtnJoinList, M.p_coPtnPred)) ) {
      M.p_coPtnJoin = new Card_CoPtnJoin;
      M.p_coPtnJoinList->vAppend(M.p_coPtnJoin);
    }

    M.p_coPtnJoin->vTransferChildren(M.p_subSet);
    DELETE_CARD(M.p_subSet);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T40_Compute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_joinPredList =
      _pCard_JoinPredList(M.pDeck->pFindChild(CK_JOINPREDLIST));

  M.p_crossPtnJoinList = new Card_CrossPtnJoinList;
  M.pDeck->vAppendMain(M.p_crossPtnJoinList);

  i8 nChildren = M.p_joinPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    M.p_cmpPred =
        _pCard_CmpPred(M.p_joinPredList->pGetChild(i_Child));


    if (T_bCoPtnJoinList_HasEqPredCmp(M.p_coPtnJoinList, M.p_cmpPred))
      continue;

    M.p_crossPtnJoin = new Card_CrossPtnJoin;
    M.p_crossPtnJoin->vAppend(M.p_cmpPred->pDupCardAndSubCards());
    M.p_crossPtnJoinList->vAppend(M.p_crossPtnJoin);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T50_Compute_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_ptndJoinSet = new Card_PtndJoinSet;
  M.pDeck->vAppendMain(M.p_ptndJoinSet);

  M.p_coPtnJoinList =
      _pCard_CoPtnJoinList(M.pDeck->pFindChild(CK_COPTNJOINLIST));
  M.p_subSet = M.p_coPtnJoinList->pDupSubCards();
  M.p_ptndJoinSet->vTransferChildren(M.p_subSet);
  DELETE_CARD(M.p_subSet);

  M.p_crossPtnJoinList =
      _pCard_CrossPtnJoinList(M.pDeck->pFindChild(CK_CROSSPTNJOINLIST));
  M.p_subSet = M.p_crossPtnJoinList->pDupSubCards();
  M.p_ptndJoinSet->vTransferChildren(M.p_subSet);
  DELETE_CARD(M.p_subSet);

  i8 nChildren = M.p_ptndJoinSet->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_pCard_child = M.p_ptndJoinSet->pGetChild(i_Child);
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

  M.p_ptndJoinSet =
      _pCard_PtndJoinSet(M.pDeck->pFindChild(CK_PTNDJOINSET));

  M.pDeck->vAppendMain(new Card_Comment("PERMUTATIONS"_Stz));

  i8 i_PtdnJoin = 0;
  M.p_subSet = M.p_ptndJoinSet->pDupSubCards();
  while ((m_pCard_popped = M.p_subSet->pPop())) {
    M.pDeck->vAppendMain(m_pCard_popped);
  }
  DELETE_CARD(M.p_subSet);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T70_Generate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pPermGen = new PermGen(M.p_ptndJoinSet->nChildCards());

  M.p_subSet = new Card_SubSet;

  while (m_pPermGen->bNextPermutation()) {
    M.pDeck->vAppendMain((M.p_permutation = new Card_Permutation));
    M.p_permutation->Y_vSet(P_Permutation, m_pPermGen->nPermutations() - 1);

    RASSERT(0 == M.p_subSet->nChildCards());
    for (i8 i_Item = 0; i_Item < m_pPermGen->nItems(); i_Item++) {
      i8 i_Child = m_pPermGen->iItem(i_Item);
      m_pCard_child = M.p_ptndJoinSet->pGetChild(i_Child);
      M.pDeck->vAppendMain(m_pCard_child->pDupCardAndSubCards());

      M.p_ptndJoin = _pCard_PtndJoin(m_pCard_child);
      if (!T_bPtndJoin_MayFollowAfter(M.p_ptndJoin, M.p_subSet)) {
        M.pDeck->vAppendMain(new Card_Comment("**DOESN'T FOLLOW**"_Stz));
        M.p_permutation->Y_vSet(P_Invalid);
        break;
      }
      M.p_subSet->vAppend(m_pCard_child);
    }
    M.p_subSet->vRemoveAll();
  }

  DELETE_CARD(M.p_subSet);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T80_Remove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Main = 0;
  i8 i_Permutation = 0;
  while (i_Main < M.pDeck->nChildCards()) {
    M.p_MainCard = M.pDeck->pGetChild(i_Main);

    if (CK_PERMUTATION != M.p_MainCard->eCard()) {
      i_Main++;
      continue;
    }

    if (! M.p_MainCard->bHasProperty(P_Invalid)) {
      M.p_MainCard->Y_vSet(P_Permutation, i_Permutation++);
      i_Main++;
      continue;
    }


    M.pDeck->vRemoveChild(M.p_MainCard);
    while (i_Main < M.pDeck->nChildCards()) {
      M.p_MainCard = M.pDeck->pGetChild(i_Main);
      if (CK_PTNDJOIN != M.p_MainCard->eCard() && CK_COMMENT != M.p_MainCard->eCard())
        break;
      M.pDeck->vRemoveChild(M.p_MainCard);
      delete M.p_MainCard;
    }
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T90_Setup_costEst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while ((M.p_MainCard = M.pDeck->pGetChild(0))) {

    if (CK_PERMUTATION == M.p_MainCard->eCard())
      break;

    M.pDeck->vRemoveChild(M.p_MainCard);
    delete M.p_MainCard;
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T92_Estimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_tableList =
      _pCard_TableList(M.pDeck->pFindChild(CK_TABLELIST));


  BoardIter xBI(M.pBoard);

//  while (M.p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN))) {
  while ((M.p_MainCard = xBI.pEachMain())) {
    eCK eCard = M.p_MainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      M.p_tableList->vDeletePropertyRecursive(P_TableUsed);
      continue;
    }
    if (CK_PTNDJOIN != eCard)
      continue;


    M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    D_vDerive_PtndJoin(M.p_ptndJoin, M.p_tableList);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T94_Estimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  BoardIter xBI(M.pBoard);

  i8 nNetwork = 0;
  i8 nSentOut = 0;
  M.p_permutation = nullptr;
  M.p_ptndJoin    = nullptr;

  while ((M.p_MainCard = xBI.pEachMain())) {


    eCK eCard = M.p_MainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      if (M.p_permutation)
        M.p_permutation->Y_vSet(P_Network, nNetwork);
      M.p_permutation = _pCard_Permutation(M.p_MainCard);

      M.p_ptndJoin = nullptr;
      nNetwork = 0;
      nSentOut = 0;
    }
    else
    if (CK_PTNDJOIN == eCard) {
      if (M.p_ptndJoin) {
        nSentOut += M.p_ptndJoin->Y_iVal(P_JoinedBuff);
        nNetwork += nSentOut;
        M.p_ptndJoin->Y_vSet(P_SentOut, nSentOut);
      }
      M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    }
  }

  if (M.p_permutation)
    M.p_permutation->Y_vSet(P_Network, nNetwork);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T96_Estimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 iLowestNetwork = 999999999;

  BoardIter xBI(M.pBoard);

  while ((M.p_permutation =
              _pCard_Permutation(xBI.pEachMain(CK_PERMUTATION)))) {
    i8 iNetwork = M.p_permutation->Y_iVal(P_Network);
    if (iNetwork >= iLowestNetwork)
      continue;
    M.p_permutation_low = M.p_permutation;
    iLowestNetwork = iNetwork;
  }

  M.pDeck->vAppendMain((M.p_joinOrderSeq = new Card_JoinOrderSeq));
  M.p_joinOrderSeq->Y_vSet(P_Network, iLowestNetwork);

  xBI.vSet(M.p_permutation_low);
  while ((M.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != M.p_MainCard->eCard())
      break;

    M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    D_vDerive_JoinOrderSeq(M.p_joinOrderSeq, M.p_ptndJoin);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T98_Extract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.pDeck->vAppendMain((M.p_joinOrderSeq = new Card_JoinOrderSeq));

  BoardIter xBI(M.pBoard);

  xBI.vSet(M.p_permutation_low);
  while ((M.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != M.p_MainCard->eCard())
      break;


    M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    D_vDerive_JoinOrderSeq(M.p_joinOrderSeq, M.p_ptndJoin);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T99_Convert_table_To_scan_Or_indexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! M.p_bounds) {

    M.p_scan = new Card_Scan;
    M.p_scan->vTransferChildren(M.p_table);

    M.pBoard->pDeck()->vReplace(M.p_table,M.p_scan);
    M.p_scan->vInsert(M.p_table);

    if (M.p_indexJoin) {
      M.p_scan->vRemoveChild(M.p_indexJoin);
      DELETE_CARD(M.p_indexJoin);
    }
    return;
  }

  M.p_table->vRemoveSubCard(M.p_indexJoin);
  M.p_indexJoin->vTransferChildren(M.p_table);

  M.pBoard->pDeck()->vReplace(M.p_table,M.p_indexJoin);
  M.p_indexJoin->vInsert(M.p_table);

  M.p_indexJoin->vRemoveSubCard(M.p_index);
  M.p_indexJoin->vAppend(M.p_index);


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T99_Convert_table_To_scan_Or_indexJoin_b(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 i_Child;
  M.p_comment = _pCard_Comment(M.p_scan->pFindChild(CK_COMMENT,i_Child));

  if (i_Child == M.p_scan->nChildCards() - 1) {
    M.p_scan->vRemoveChild(M.p_comment);
    DELETE_CARD(M.p_comment);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T99_Convert_table_To_scan_Or_indexJoin_c(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 i_Child;
  M.p_comment = _pCard_Comment(M.p_indexJoin->pFindChild(CK_COMMENT,i_Child));
  M.p_comment->vValue(SEP_DERIVED);

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                        Index Scan Hand 80 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T10_Add_indexList_Under_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vScan_Add_indexList(M.p_scan);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T20_Convert_idxCol_name_Of_scan_To_ordinal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M_vIdxColList_ToOrdinals(M.p_idxColList, M.p_stream);
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

  M.p_filter->vApplyRecursive(CK_CMPPRED, bfMarkPredBounds);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T40_Mark_IndexMatchOf_scan_filter_terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! (M.p_index = _pCard_Index(M.p_indexList->pGetChild(0))))
    return;
  if (! (M.p_idxColList = (M.p_index->pSub_idxColList())))
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfPredBounds = [this](Card* pCard) {
    Card_Term* p_term = _pCard_Term(pCard);
    M_vTerm_MarkIfMatchesAn_idxCol(p_term, M.p_idxColList);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  M.p_filter->vApplyRecursive(CK_TERM, bfPredBounds);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfAdd = [this](Card* pCard) {
    M.p_cmpPred = _pCard_CmpPred(pCard);
    M_vScan_Add_cmpPred_To_bounds(M.p_scan, M.p_cmpPred);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  M.p_filter->vApplyRecursive(CK_CMPPRED, bfAdd);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T60_Cull_Marked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTracer("Culling <filter> of L & U bounds %s\n"_Stz,M.p_filter->szBearing());
  M_vFilter_CullPred(M.p_filter, 'L');
  M_vFilter_CullPred(M.p_filter, 'U');

  //M.p_filter->vDump();
  if (!T_bFilter_IsEmpty(M.p_filter))
    return;

  vTracer("Deleting empty <filter> %s\n"_Stz,M.p_filter->szBearing());
  M.p_MainCard->vRemoveSubCard(M.p_filter);
  DELETE_CARD(M.p_filter);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T70_Promote_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_scan->vCard(CK_INDEXSCAN);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T70_Promote_scan_To_indexScan_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_Child;
  M.p_comment = _pCard_Comment(M.p_indexScan->pFindChild(CK_COMMENT,i_Child));
  M.p_comment->vValue(SEP_DERIVED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T70_Promote_scan_To_indexScan_c(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_indexScan->vReplaceChildWithItsSubs(M.p_indexList);
  DELETE_CARD((M.p_indexList));
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

  M.p_gbyElist = M.p_aggreg->pSub_gbyElist();
  M.p_sbyElist = M.p_sort->pSub_sbyElist();

  // TODO: needs to be a T_ test
  if (! M.p_gbyElist->bValuesSameRecursive(M.p_sbyElist))
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

  M.p_scan->vRemoveChild(M.p_filter);
  M.pDeck->vInsertAfter(M.p_filter, M.p_scan);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HA0_T10_Individuate_indexJoin_filter_b(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_indexJoin->vRemoveChild(M.p_filter);
  M.pDeck->vInsertAfter(M.p_filter, M.p_indexJoin);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Restate Pipeline Hand Z0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HZ0_T10_Reinstate_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! T_bStage_Reinstate(M.e_MainCard, M.e_SubsequentToMain))
    return;

  m_pCard_dup = M.p_stage->pDupCardAndSubCards();
  M.pDeck->vInsertAfter(m_pCard_dup, M.p_MainCard);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HZ0_T20_Reinstate_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_combined = new Card_Combined;
  M.pDeck->vInsertAfter(M.p_combined, M.p_MainCard);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
