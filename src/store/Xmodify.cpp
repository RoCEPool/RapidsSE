/*============================================================================*/
/* Xmodify.cpp --                                                       */
/*============================================================================*/

#include "Xmodify.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xmodify)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xmodify::Xmodify(eCN eClass)
: Xcreate(eClass)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_iOrdLo      = 1;
  m_iOrdHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xmodify::~Xmodify(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! p_boolCalc->nChildCards())
    return;

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  while (pCard_pred &&
         M_bBoolCalc_CullPred(p_boolCalc, pCard_pred, cWithMark) &&
         p_boolCalc->nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Xmodify::M_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }
  else
  if (CK_TSTPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      M_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDPRED == eCard_pred || CK_ORPRED == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(p_pred->pGetChild(i_Child));
    if (M_bBoolCalc_CullPred(p_boolCalc, pCard_pred_child, cWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pParentCard = p_boolCalc->pFindParent(p_pred);

  pParentCard->vRemoveSubCard(p_pred);

  if (CK_ANDPRED == pParentCard->eCard() || CK_ORPRED == pParentCard->eCard()) {

    if (0 == pParentCard->nChildCards()) {
      p_boolCalc->vRemoveSubCard(pParentCard);
    }
    else {
      Card* pSiblingCard = pParentCard->pGetChild(0);
      Card* pGrandCard = p_boolCalc->pFindParent((pParentCard));
      pParentCard->vRemoveSubCard(pSiblingCard);
      pGrandCard->vReplaceChild(pParentCard, pSiblingCard);
    }

    delete pParentCard;
  }
}

/*----------------------------------------------------------------------------*/
/* resolve the parameters terms in given boolCalc                             */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_Resolve_Parameters(Card_BoolCalc* p_boolCalc,
                                             Card_ParamList* p_paramList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfXYZ = [this,p_paramList](Card_PRED* p_pred, bool bUpwards)
  {
    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! T_bTerm_IsParamOrdinal(p_term))
      return C_CONTINUE_RECURSION;

    Card_Param* p_param = _pCard_Param(p_paramList->pFindPpyRecursive(P_ParamOrd));
    if (! p_param)
      return false;

    Card_StmColOrd* p_stmColOrd = p_param->pSub_stmColOrd();
    M_vCard_Set_Value(p_term,p_stmColOrd->szValue(),MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = M_bPredRecursive(p_pred_ROOT,bfXYZ);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBounds_Extract_FromFilter(Card_Table* p_table, Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  //M.p_table->vDump();

  Card_BoolCalc* p_boolCalc = p_filter->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfXYZ = [this,p_table](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_TERMINATE_RECURSION;

    if (! p_pred->bIsCard(CK_CMPPRED))
      return C_CONTINUE_RECURSION;

    Card_CmpPred* p_cmpPred = _pCard_CmpPred(p_pred);

    Card_Term* pTerm_0 = _pCard_Term(p_cmpPred->pGetChild(0));
    Card_Term* pTerm_1 = _pCard_Term(p_cmpPred->pGetChild(1));

    if (! T_bTerm_IsStreamColOrdinal(pTerm_0) ||
        ! T_bTerm_IsStreamColOrdinal(pTerm_1))
      return C_CONTINUE_RECURSION;

    // - - - -

    Card_Bounds* p_bounds = p_table->pSub_bounds();
    if (! p_bounds)
      p_bounds = new Card_Bounds;
    Card_IndexJoin* p_indexJoin = p_table->pSub_indexJoin();
    Card_Stream* p_stream = p_table->pSub_stream();
    p_table->vInsertAfter(p_bounds, p_stream);

    Card_Term* p_term = new Card_Term("BOUND"_Stz);
    p_bounds->vAppend(p_term);

    // - - - -

    // one of {pCard_term_0,pCard_term_1} is $In

    i8 iStreamOrdLo = p_stream->Y_iVal(P_OrdLo);
    i8 iStreamOrdHi = p_stream->Y_iVal(P_OrdHi);

    i8 i_iOrdLo_0 = E_iTerm_StreamColOdn(pTerm_0);
    i8 i_iOrdLo_1 = E_iTerm_StreamColOdn(pTerm_1);

    if (i_iOrdLo_0 >= iStreamOrdLo && i_iOrdLo_0 <= iStreamOrdHi) {
      p_term = new Card_Term("$I1"_Stz);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term(pTerm_1->sz());
      p_bounds->vAppend(p_term);
    }
    else
    if (i_iOrdLo_1 >= iStreamOrdLo && i_iOrdLo_1 <= iStreamOrdHi) {
      p_term = new Card_Term("$I1"_Stz);
      p_bounds->vAppend(p_term);
      p_term = new Card_Term(pTerm_0->sz());
      p_bounds->vAppend(p_term);
    }
    else {
      NEVER_REACH
    }

    p_term = new Card_Term(p_cmpPred->sz());
    p_bounds->vAppend(p_term);

    p_cmpPred->vMark1(MARK_UPWARDS);


    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = M_bPredRecursive(p_pred_ROOT,bfXYZ);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Mark_Changed(Card* pCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  pCard->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_Value(Card* pCard, Stz* szValue)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  pCard->vValue(szValue);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_Value(Card* pCard, Stz* szValue, char cMark1)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  pCard->vValue(szValue);
  pCard->vMark1(cMark1);
}

/*----------------------------------------------------------------------------*/
/* mark the bounds of this <predCmp>                                          */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Stz* szCmp = p_cmpPred->sz();

  char cBound = '?';

  if (szCmp->bSame("<"_Stz))  cBound = 'U';
  else if (szCmp->bSame("<="_Stz)) cBound = 'U';
  else if (szCmp->bSame("="_Stz))  cBound = 'E';
  else if (szCmp->bSame("=="_Stz)) cBound = 'E';
  else if (szCmp->bSame("!="_Stz)) cBound = 'N';
  else if (szCmp->bSame(">="_Stz)) cBound = 'L';
  else if (szCmp->bSame(">"_Stz))  cBound = 'L';

  p_cmpPred->vMark1(cBound);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vFilter_CullPred(Card_Filter* p_filter, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = p_filter->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = p_filter->pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_BOOLCALC)
      continue;

    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pChildCard);

    M_vBoolCalc_CullPred(p_boolCalc, cWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vIdxColList_ToOrdinals(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 iOrdLo = pCard_Stream->Y_iVal(P_OrdLo);

  Card_StmColList* pCard_StreamColList = pCard_Stream->pSub_stmColList();

  i8 nSubCards = p_idxColList->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_IdxCol* pCard_idxCol = _pCard_IdxCol(p_idxColList->pGetChild(i_Sub));

    i8 i_StreamColNo = C_UNKNOWN;
    Stz* szColNm = pCard_idxCol->sz();
    Card_StmCol* pCard_streamCol =
        T_pStmColList_Find_streamCol(pCard_StreamColList, szColNm, i_StreamColNo);

    s32 sOrdinal;
    sOrdinal.vSprint("$%lld"_Stz,iOrdLo+i_StreamColNo);
    pCard_idxCol->vValue(sOrdinal);

    pCard_idxCol->vMark1(MARK_CHANGED);
  }
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of pred and apply lambda function                      */
/*----------------------------------------------------------------------------*/
bool Xmodify::M_bPredRecursive(Card_PRED* p_pred,
                             std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  bool bContinue;

  eCK eCard = p_pred->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // only call term once

  if (CK_TERM == eCard) {
    bContinue = bfApply(p_pred,false);
    return bContinue;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // apply pred function for downwards
  bContinue = bfApply(p_pred,false);
  if (! bContinue)
    return false;

  // descend into children
  i8 nSubCards = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_pred->pGetChild(i_Child);
    bContinue = M_bPredRecursive(static_cast<Card_PRED *>(pSubCard), bfApply);
    if (! bContinue)
      return false;
  }

  // apply pred function for upwards
  bContinue = bfApply(p_pred,true);
  return bContinue;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vScan_Add_cmpPred_To_bounds(Card_Scan* p_scan, Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! ('L' ==p_cmpPred->cMark1() || 'U' == p_cmpPred->cMark1()) )
    return;

  Card_Filter* p_filter = p_scan->pSub_filter();

  Card_Bounds* p_bounds = p_scan->pSub_bounds();
  if (! p_bounds) {
    p_bounds = new Card_Bounds;
    p_scan->vInsertBefore(p_bounds,p_filter);
  }

  if ('L' == p_cmpPred->cMark1()) {
    Card_LBound* p_lBound = p_bounds->pSub_lBound();
    if (! p_lBound) {
      p_lBound = new Card_LBound;
      p_bounds->vAppend(p_lBound);
    }
    Card_CmpPred* p_cmpPred_dup = _pCard_CmpPred(p_cmpPred->pDupCardAndSubCards());
    StrLib::vInsertChar(p_cmpPred_dup->pGetChild(0)->sz()->azChar()+1,'I');
    p_lBound->vAppend(p_cmpPred_dup);
  }
  else
  if ('U' == p_cmpPred->cMark1()) {
    Card_UBound* p_uBound = p_bounds->pSub_uBound();
    if (! p_uBound) {
      p_uBound = new Card_UBound;
      p_bounds->vAppend(p_uBound);
    }
    Card_CmpPred* p_cmpPred_dup = _pCard_CmpPred(p_cmpPred->pDupCardAndSubCards());
    StrLib::vInsertChar(p_cmpPred_dup->pGetChild(0)->sz()->azChar()+1,'I');
    p_uBound->vAppend(p_cmpPred_dup);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vScan_Add_indexList(Card_Scan* p_scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_scan->pSub_tableNm();
  Table* pTable = A_pAccess_Table(p_tableNm->sz());

  Card_IndexList* p_indexList = C_pProduce_indexList_ofTable(pTable);
  if (! p_indexList)
    return;

  Card_Comment* p_comment = new Card_Comment(SEP_SCRATCH);
  p_scan->vAppend(p_comment);

  p_scan->vAppend(p_indexList);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_OrdinalRange(Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // <scan>/<stream>:iOrdLo,iOrdHi
  i8 nStreamCols = p_stream->pSub_stmColList()->nChildCards();
  p_stream->Y_vSet(P_OrdLo, m_iOrdLo);
  m_iOrdHi = m_iOrdLo + nStreamCols - 1;
  p_stream->Y_vSet(P_OrdHi, m_iOrdHi);

  p_stream->vMark1(MARK_CHANGED);

  CardIter xCI(p_stream->pSub_stmColList());
  i8 iOrdinal = m_iOrdLo;
  while (Card_StmCol* p_stmCol = _pCard_StmCol(xCI.pEachChild(CK_STMCOL))) {
    Card_StmColOrd* p_stmColOrd = new Card_StmColOrd;
    p_stmColOrd->vValueSprint("$%lld"_Stz, iOrdinal);
    p_stmCol->vAppend(p_stmColOrd);
    iOrdinal++;
  }

  m_iOrdLo = m_iOrdHi + 1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Set_OrdinalRange(Card_Stream* p_stream, i8 iLo, i8 iHi)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_stream->Y_vSet(P_OrdLo,iLo);
  p_stream->Y_vSet(P_OrdHi,iHi);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTable_Add_IndexJoin(Card_Table* p_table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();

  Table* pTable = A_pAccess_Table(p_tableNm->sz());

  Card_IndexList* p_indexList = C_pProduce_indexList_ofTable(pTable);

  while (Card_Index* p_index = _pCard_Index(p_indexList->pPopChild())) {
    Card* p_indexJoin = new Card_IndexJoin;
    p_indexJoin->vAppend(p_index);
    Card_Comment* p_comment = new Card_Comment(SEP_SCRATCH);
    p_table->vAppend(p_comment);
    p_table->vAppend(p_indexJoin);
  }

  DELETE_CARD(p_indexList);
}

/*----------------------------------------------------------------------------*/
/* set synonym of table                                                       */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTable_Set_Syn(Card_Table* p_table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();

  Table* pTable = A_pAccess_Table(p_tableNm->sz());

  p_table->vMark1(MARK_CHANGED);

  char cTableSyn_actual = m_cTableSyn;
  if (pTable->bIsReplicated())
    cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

  p_table->vValueSprint("%c"_Stz,cTableSyn_actual);

  m_cTableSyn++;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_MarkIfMatchesAn_idxCol(Card_Term* p_term, Card_IdxColList* p_idxColList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Stz* szTerm = p_term->sz();
  if (!szTerm->bIsStreamColOrd())
    return;

  i8 i_IdxCol = C_UNKNOWN;

  Card_IdxCol* p_idxCol =
      _pCard_IdxCol(p_idxColList->pFindChild(CK_IDXCOL, szTerm, i_IdxCol));
  if (! p_idxCol)
    return;

  p_term->vMark1('I');
  p_term->vMark2(i_IdxCol);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_Set_StmColOdn(Card_Term* p_term, i8 iStmColOrd)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_term->vValueSprint("$%lld"_Stz,iStmColOrd);
}

/*----------------------------------------------------------------------------*/
/*-- Predicate Pull-Up -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Xmodify::vPullUp_MarkPredLeftwards(Card_BoolCalc* p_bool,
                                        Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* p_pred = static_cast<Card_PRED*>(p_bool->pGetChild(0));

  vPullUp_MarkPredDrillDown(p_pred, p_deck);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Xmodify::vPullUp_MarkPredDrillDown(Card_PRED* p_pred,
                                        Card_Deck* p_deck)
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
        cPullup_FindTableForPred(p_pred, p_deck);
    if (C_UNKNOWN != cWhichTable) {
      p_pred->vMark1(MARK_ASSIGNED);
      p_pred->vMark2(cWhichTable);
    }
    return;
  }

  i8 nSubCards = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_pred->pGetChild(i_Child);

    vPullUp_MarkPredDrillDown(static_cast<Card_PRED *>(pSubCard), p_deck);
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
char Xmodify::cPullup_FindTableForPred(Card_PRED* p_pred,
                                       Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nAbsJoin = 0;
  i8 nTable    = 0;
  char cWhichTable = C_UNKNOWN;

  //vTrace("iAssignPredLeft candidate %p\n"_Stz,p_pred);
  //p_pred->vDump();

  for (i8 i_Main = 0; /* none */ ; i_Main++) {
    Card *pMainCard = p_deck->pGetChild(i_Main);

    if (CK_WHERE == pMainCard->eCard())
      return C_UNKNOWN;

    eCK eMainCard = pMainCard->eCard();

    if (CK_TABLE == eMainCard) {
      nTable++;
      cWhichTable = cPullUp_TableContainsPred(_pCard_Table(pMainCard), p_pred);
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
char Xmodify::cPullUp_TableContainsPred(Card_Table* p_table,
                                        Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_Stream* p_stream = p_table->pSub_stream();

  // are all $n of <pred> in this <stream>?
  if (bPullUp_StreamContainsPred(p_stream, p_pred)) {
    char cTableSyn = p_table->cValue();
    return cTableSyn;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
bool Xmodify::bPullUp_StreamContainsPred(Card_Stream* p_stream,
                                         Card_PRED* pCard_PRED)
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

    if (!bPullUp_StreamContainsPred(p_stream, static_cast<Card_PRED *>(pSubCard)))
      return false;
  }

  return true;
}


/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Xmodify::vPullUp_MovePredLeftwards(Card_BoolCalc* p_calcBool,
                                        Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while (p_calcBool->nChildCards()) {
    Card_PRED* p_pred = static_cast<Card_PRED*>(p_calcBool->pGetChild(0));
    bool bSubRemoved = bPullUp_MovePredDrillDown(p_pred, p_calcBool, p_deck);
    if (! bSubRemoved)
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
bool Xmodify::bPullUp_MovePredDrillDown(Card_PRED* pCard_pred,
                                        Card_BoolCalc* p_calcBool,
                                        Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (MARK_ASSIGNED == pCard_pred->cMark1()) {
    Card* pMainCard;

    char cTableSyn = pCard_pred->cMark2();
    Card_Table* pCard_table = p_deck->pFindTableCard(cTableSyn, pMainCard);

    Card_Filter* pCard_filter_existing = pMainCard->pSub_filter();

    if (pCard_filter_existing) {
      pCard_filter_existing->vMark1(pCard_table->cMark1());
      pCard_filter_existing->vMark2(pCard_table->cMark2());
      vPullUp_MovePredToFilter(pCard_filter_existing, pCard_pred);
    }
    else {
      Card_Filter* pCard_filter_new = new Card_Filter;
      pCard_filter_new->vMark1(pCard_table->cMark1());
      pCard_filter_new->vMark2(pCard_table->cMark2());
      pMainCard->vAppend(pCard_filter_new);
      vPullUp_MovePredToFilter(pCard_filter_new, pCard_pred);
    }

    M_vBoolCalc_RemovePred(p_calcBool, pCard_pred);

    return true;
  }

  i8 nSubCards = pCard_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_pred->pGetChild(i_Child);

    bool bRemoved = bPullUp_MovePredDrillDown(static_cast<Card_PRED *>(pSubCard), p_calcBool, p_deck);
    if (bRemoved)
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Xmodify::vPullUp_MovePredToFilter(Card_Filter* p_filter,
                                       Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (p_filter->nChildCards())
    vPullUp_MovePredToExistingFilter(p_filter, p_pred);
  else
    vPullUp_MovePredToNewFilter(p_filter, p_pred);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Xmodify::vPullUp_MovePredToExistingFilter(Card_Filter* p_filter_existing,
                                               Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  RASSERT(p_filter_existing->nChildCards());

  Card_BoolCalc* pCard_calcBool = p_filter_existing->pSub_boolCalc();

  Card* pCard_top = pCard_calcBool->pPop();

  Card_AndPred* pCard_predAnd = new Card_AndPred;
  pCard_predAnd->vAppend(pCard_top);
  pCard_predAnd->vAppend(pCard_pred);

  pCard_calcBool->vAppend(pCard_predAnd);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Xmodify::vPullUp_MovePredToNewFilter(Card_Filter* p_filter_new,
                                          Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  RASSERT(! p_filter_new->nChildCards());

  Card_BoolCalc* p_calcBool = new Card_BoolCalc;
  p_filter_new->vAppend(p_calcBool);

  p_calcBool->vAppend(p_pred);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::vStreamAnalysisReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_iOrdLo      = 1;
  m_iOrdHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
