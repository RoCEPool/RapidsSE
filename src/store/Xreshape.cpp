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

  E_vRemoveDeleteMain(CK_STAGE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T15_Hide_Ignorable_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_stage->Y_vSet(P_Exclude);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T20_Delete_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  E_vRemoveDeleteMain(CK_ABSJOIN);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T30_Delete_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  E_vRemoveDeleteMain(CK_COMBINED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H10_T40_Delete_send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  E_vRemoveDeleteMain(CK_SEND);
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
/*                                                    Access Metadata Hand 20 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H20_T10_Add_tableSyn_ForAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pTable = A_pAccess_Table(M.p_tableNm->sz());

  M.p_table->vMark1(MARK_CHANGED);
  char cTableSyn_actual = m_cTableSyn;
  if (m_pTable->bIsReplicated())
    cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  M.p_table->vValueSprint("%c"_Stz,cTableSyn_actual);

  m_cTableSyn++;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H20_T20_Add_tablePtn_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pTable = A_pAccess_Table(M.p_tableNm->sz());

  M.p_tablePtn = A_pProduce_tablePtn_ofTable(m_pTable,m_cTableSyn);

  M.p_table->vAppend(M.p_tablePtn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H20_T30_Add_stream_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pTable = A_pAccess_Table(M.p_tableNm->sz());
  M.p_stream = A_pProduce_stream_ofTable(m_pTable);

  M.p_table->vAppend(M.p_stream);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Stream Analysis Hand 30 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H30_T10_Calc_ordinalRange_ForAll_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_stream = M.p_table->pSub_stream();

  // <scan>/<stream>:iOrdLo,iOrdHi
  m_nStreamCols = M.p_stream->pSub_stmColList()->nChildCards();
  M.p_stream->Y_vSet(P_OrdLo, m_iOrdLo);
  m_iOrdHi = m_iOrdLo + m_nStreamCols - 1;
  M.p_stream->Y_vSet(P_OrdHi, m_iOrdHi);

  M.p_stream->vMark1(MARK_CHANGED);

  CardIter xCI(M.p_stream->pSub_stmColList());
  i8 iOrdinal = m_iOrdLo;
  while ((M.p_stmCol = _pCard_StmCol(xCI.pEachChild(CK_STMCOL)))) {
    M.p_stmColOrd = new Card_StmColOrd;
    M.p_stmColOrd->vValueSprint("$%lld"_Stz, iOrdinal);
    M.p_stmCol->vAppend(M.p_stmColOrd);
    iOrdinal++;
  }

  m_iOrdLo = m_iOrdHi + 1;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Predicate Pulluo Hand 40 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T10_Convert_where_calc_To_where_calcBool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  M.p_boolCalc = X_pConvert_rpnCalc_To_boolCalc(M.p_rpnCalc);
  X_vSubCard_ReplaceSub(M.p_rpnCalc, M.p_where, M.p_boolCalc);
  D_vDeleteMember(M.p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T20_Mark_where_pred_Wrt_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T30_Move_Marked_where_pred_To_table_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H40_T40_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T10_Rework_Exists_SubQueries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T20_Copy_Up_SubQueries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T30_Resolve_Parameter_Linkage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H50_T40_Resolve_Parameter_Terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T10_Add_indexJoin_UnderAllJoinable_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T20_Convert_idxCol_name_Of_table_To_idxCol_ord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T30_Add_joinPred_UnderAll_indexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T40_Cull_filter_pred_MovedUpTo_joinPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H60_T50_Delete_Empty_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T00_Extract_tableList_From_queryBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T10_Extract_joinPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T20_Compute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T30_Compute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T40_Compute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T50_Compute_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T60_Copy_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T70_Generate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T80_Remove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T90_Setup_costEst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T92_Estimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T94_Estimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T96_Estimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T98_Extract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H70_T99_Convert_table_To_scan_Or_indexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T10_Insert_indexList_ToAll_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T20_Convert_idxCol_name_Of_scan_To_idxCol_ord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T30_Mark_scan_pred_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T40_Mark_scan_filter_term_WhichMatchIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T50_Add_scanPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T60_Cull_Marked_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H80_T70_Promote_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_H90_T10_Remove_sort_IfSameAs_gby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HA0_T10_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HZ0_T10_Reinstate_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_HZ0_T20_Reinstate_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}




/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Xreshape::vMarkPredLeftwards(Card_BoolCalc* p_bool)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* p_pred = static_cast<Card_PRED*>(p_bool->pGetChild(0));

  vMarkPredDrillDown(p_pred);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Xreshape::vMarkPredDrillDown(Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char cWhichTable = ' ';

  eCK eCard = p_pred->eCard();


  if (CK_ORPRED == eCard) {
    p_pred->vMark1('|');
    return;
  }

  if (CK_CMPPRED == eCard) {
    p_pred->vMark1('?');
    cWhichTable =
        cMovePredLeftwards(p_pred);
    if (C_UNKNOWN != cWhichTable) {
      p_pred->vMark1(MARK_ASSIGNED);
      p_pred->vMark2(cWhichTable);
    }
    return;
  }

  i8 nSubCards = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_pred->pGetChild(i_Child);

    vMarkPredDrillDown(static_cast<Card_PRED *>(pSubCard));
  }


  if (CK_ANDPRED == eCard) {
    p_pred->vMark1('&');
    Card *pSubCard_0 = p_pred->pGetChild(0);
    i8 iStream_0 = pSubCard_0->cMark2() - '0';

    Card *pSubCard_1 = p_pred->pGetChild(1);
    i8 iStream_1 = pSubCard_1->cMark2() - '0';

    if (iStream_0 == iStream_1) {
      p_pred->vMark1(MARK_ASSIGNED);
      p_pred->vMark2(iStream_0);
    }

    return;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
char Xreshape::cMovePredLeftwards(Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nAbsJoin = 0;
  i8 nTable    = 0;
  char cWhichTable = C_UNKNOWN;

  //vTrace("iAssignPredLeft candidate %p\n"_Stz,p_pred);
  //p_pred->vDump();

  for (i8 i_Main = 0; /* none */ ; i_Main++) {
    Card *pMainCard = M.pDeck->pGetChild(i_Main);

    if (CK_WHERE == pMainCard->eCard())
      return C_UNKNOWN;

    eCK eMainCard = pMainCard->eCard();

    if (CK_TABLE == eMainCard) {
      nTable++;
      cWhichTable = cMovePredToTable(_pCard_Table(pMainCard), p_pred);
      if (C_UNKNOWN != cWhichTable)
        return cWhichTable;
      continue;
    }

    // skip over any other card??
    vTrace("bMovePredLeft skipping mainCard %s\n"_Stz,aCK[eMainCard]);
  }

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Xreshape::cMovePredToTable(Card_Table* p_table, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_Stream* p_stream = p_table->pSub_stream();

  // are all $n of <pred> in this <stream>?
  if (bStreamContainsPred(p_stream,p_pred)) {
    char cTableSyn = p_table->cValue();
    return cTableSyn;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
bool Xreshape::bStreamContainsPred(Card_Stream* p_stream, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  eCK ePredCard = pCard_PRED->eCard();

  if (CK_TERM == ePredCard) {
    Stz* szTerm = pCard_PRED->sz();
    if (!szTerm->bIsStreamColOrd())
      return true;

    i8 iOrdLo = p_stream->Y_iVal(P_OrdLo);
    i8 iOrdHi = p_stream->Y_iVal(P_OrdHi);

    i8 iOrd = szTerm->iOrdinal();
    if (iOrd > iOrdHi)
      return false;
  }

  i8 nSubCards = pCard_PRED->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_PRED->pGetChild(i_Child);

    if (! bStreamContainsPred(p_stream,static_cast<Card_PRED*>(pSubCard)))
      return false;
  }

  return true;
}


/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Xreshape::vCopyPredLeftwards(Card_BoolCalc* p_calcBool)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while (p_calcBool->nChildCards()) {
    Card_PRED* p_pred = static_cast<Card_PRED*>(p_calcBool->pGetChild(0));
    bool bSubRemoved = bCopyPredDrillDown(p_pred,p_calcBool);
    if (! bSubRemoved)
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
bool Xreshape::bCopyPredDrillDown(Card_PRED* pCard_pred, Card_BoolCalc* p_calcBool)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (MARK_ASSIGNED == pCard_pred->cMark1()) {
    Card* pMainCard;
//    i8 iStream = pCard_pred->iMark2();
//    Card_Stream* pCard_stream = m_pBoard->pFindStreamCard(iStream,pMainCard);
    char cTableSyn = pCard_pred->cMark2();
    Card_Table* pCard_table = M.pDeck->pFindTableCard(cTableSyn, pMainCard);

    Card_Filter* pCard_filter_existing = pMainCard->pSub_filter();

    if (pCard_filter_existing) {
      pCard_filter_existing->vMark1(pCard_table->cMark1());
      pCard_filter_existing->vMark2(pCard_table->cMark2());
      vCopyPredToExistingFilter(pCard_filter_existing,pCard_pred);
    }
    else {
      Card_Filter* pCard_filter_new = new Card_Filter;
      pCard_filter_new->vMark1(pCard_table->cMark1());
      pCard_filter_new->vMark2(pCard_table->cMark2());
      pMainCard->vAppend(pCard_filter_new);
      vCopyPredToNewFilter(pCard_filter_new,pCard_pred);
    }

    //pCard_stream->vMark1('#');


    X_vBoolCalc_RemovePred(p_calcBool, pCard_pred);

    return true;
  }

  i8 nSubCards = pCard_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_pred->pGetChild(i_Child);

    bool bRemoved = bCopyPredDrillDown(static_cast<Card_PRED *>(pSubCard), p_calcBool);
    if (bRemoved)
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Xreshape::vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_BoolCalc* pCard_calcBool = pCard_filter_existing->pSub_boolCalc();

  Card* pCard_top = pCard_calcBool->pPop();

  Card_AndPred* pCard_predAnd = new Card_AndPred;
  pCard_predAnd->vAppend(pCard_top);
  pCard_predAnd->vAppend(pCard_pred);

  pCard_calcBool->vAppend(pCard_predAnd);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Xreshape::vCopyPredToNewFilter(Card_Filter* p_filter_new, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_BoolCalc* p_calcBool = new Card_BoolCalc;
  p_filter_new->vAppend(p_calcBool);

  p_calcBool->vAppend(p_pred);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vMarkPredAsMoved(Card* pPredCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nSubCards = pPredCard->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pPredCard->pGetChild(i_Child);

    //eCK eSubCard = pSubCard->eCard();

    // mark the

    vMarkPredAsMoved(pSubCard);
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
