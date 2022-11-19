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

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
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
  if (CK_EXEPRED == eCard_pred) {
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vBoolCalc_PushPred(Card_BoolCalc* p_boolCalc,
                                   Card_PRED* p_pred_toBeAdded)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* p_pred_oldTop = static_cast<Card_PRED*>(p_boolCalc->pPop());
  RASSERT(! p_boolCalc->nChildCards());

  Card_AndPred* p_andPred_new = new Card_AndPred;
  p_andPred_new->vAppend(p_pred_oldTop);
  p_andPred_new->vAppend(p_pred_toBeAdded);

  p_boolCalc->vPush(p_andPred_new);
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
void Xmodify::M_vBoolCalc_Substitute_Parameters(Card_BoolCalc* p_boolCalc,
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
    if (! T_bValue_IsParamOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Card_Param* p_param = _pCard_Param(p_paramList->pFindPpyRecursive(P_ParamOdn));
    if (! p_param)
      return false;

    //Card_StmColOdn* p_stmColOrd = p_param->pSub_stmColOdn();
    //M_vCard_Set_Value(p_term,p_stmColOrd->szValue(),MARK_CHANGED);
    Card_TblColOdn* p_tblColOrd = p_param->pSub_tblColOdn();
    M_vCard_Set_Value(p_term,p_tblColOrd->szValue(),MARK_CHANGED);

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
void Xmodify::M_vCard_Set_IndexColOrdinal(Card* p_card, i8 iIdxColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$I%lld"_Stz, iIdxColOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_KeyOrdinal(Card* p_card, i8 iKeyOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$K%lld"_Stz, iKeyOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_ParamOrdinal(Card* p_card, i8 iParamOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$P%lld"_Stz, iParamOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_StreamColOrdinal(Card* p_card, i8 iStmColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$%lld"_Stz, iStmColOdn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vCard_Set_TableColOrdinal(Card* p_card, char cTblSyn, i8 iTblColOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_card->vValueSprint("$%c.%lld"_Stz, cTblSyn, iTblColOdn);
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
/* affix each idxCol with it's $c.n of this table stream                      */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vIdxColList_Affix_tblColOdn(Card_IdxColList* p_idxColList, Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 i_StreamColNo = C_UNKNOWN;

  Card_StmColList* p_streamColList = p_stream->pSub_stmColList();

  CardIter xCI(p_idxColList);
  while (Card_IdxCol* p_idxCol = _pCard_IdxCol(xCI.pEachChild(CK_IDXCOL))) {
    Stz* szColNm = p_idxCol->sz();

    Card_StmCol* p_streamCol =
        T_pStmColList_Find_streamCol(p_streamColList, szColNm, i_StreamColNo);

    Card_TblColOdn* p_tblColOdn = p_streamCol->pSub_tblColOdn();
    Stz* szTblColOdn = p_tblColOdn->sz();

    M_vCard_Set_Value(p_idxCol,szTblColOdn,MARK_CHANGED);
  }
}

/*----------------------------------------------------------------------------*/
/* convert eLevel to P/N/O                                                    */
/*----------------------------------------------------------------------------*/
char Xmodify::M_eLevel_To_char(eAL eLevel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  switch (eLevel) {
    case AL_PARTITION : return 'P';
    case AL_NODE      : return 'N';
    case AL_ORIGIN    : return 'O';
    default           : return '?';
  }
}


/*----------------------------------------------------------------------------*/
/* convert eAL to eYG                                                         */
/*----------------------------------------------------------------------------*/
eYG Xmodify::M_eLevel_To_StageTopology(eAL eLevel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  switch (eLevel) {
    case AL_PARTITION : return YG_ALL_PARTITIONS;
    case AL_NODE      : return YG_ALL_NODES;
    case AL_ORIGIN    : return YG_ORIGIN_NODE;
    default           : return YG_ALL_PARTITIONS;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vRpnCalc_Convert_sqyOp_not_To_notSqyOp(Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  CardIter xCI(p_rpnCalc);
  while (Card_Term* p_term = _pCard_Term(xCI.pEachChild())) {

    if (! xCI.bMore())
      break;

    if (p_term->bSame("exists"_Stz)) {

      Card_Term* p_term_next = _pCard_Term(xCI.pEachChild());
      if (p_term_next->bSame("not"_Stz)) {
        xCI.vRewind();
        p_rpnCalc->vRemoveChild(p_term_next);
        p_term->vValue("notExists"_Stz);
        continue;
      }
      continue;
    }

    if (p_term->bSame("in"_Stz)) {

      Card_Term* p_term_next = _pCard_Term(xCI.pEachChild());
      if (p_term_next->bSame("not"_Stz)) {
        xCI.vRewind();
        p_rpnCalc->vRemoveChild(p_term_next);
        p_term->vValue("notIn"_Stz);
        continue;
      }
      continue;
    }

  }

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
void Xmodify::M_vScan_Produce_indexList(Card_Scan* p_scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_scan->pSub_tableNm();
  Table* pTable = A_pAccess_Table(p_tableNm->sz());

  Card_IndexList* p_indexList = C_pProduce_indexList_ofTable(pTable);
  if (! p_indexList)
    return;

  Card_Note* p_note = new Card_Note(SEP_SCRATCH);
  p_scan->vAppend(p_note);

  p_scan->vAppend(p_indexList);
}

/*----------------------------------------------------------------------------*/
/* convert eYG to eAL                                                         */
/*----------------------------------------------------------------------------*/
eAL Xmodify::M_eStageTopology_To_Level(eYG eStageTopo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  eAL eLevel;

  switch (eStageTopo) {
    case YG_ALL_PARTITIONS : eLevel = AL_PARTITION; break;
    case YG_ALL_NODES      : eLevel = AL_NODE;      break;
    case YG_ORIGIN_NODE    : eLevel = AL_ORIGIN;    break;
    default                : eLevel = AL_NONE;      break;
  }

  return eLevel;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Derive_OrdinalRange_aggreg(Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 nStreamCols = p_stream->pSub_stmColList()->nChildCards();
  p_stream->Y_vSet(P_OdnLo, (i8)1);
  p_stream->Y_vSet(P_OdnHi, nStreamCols);

  p_stream->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Derive_OrdinalRange_project(Card_Stream* p_stream)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 nStreamCols = p_stream->pSub_stmColList()->nChildCards();
  p_stream->Y_vSet(P_OdnLo, (i8)1);
  p_stream->Y_vSet(P_OdnHi, nStreamCols);

  p_stream->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Derive_OrdinalRange_table(Card_Stream* p_stream, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // <scan>/<stream>:iOrdLo,iOdnHi
  i8 nStreamCols = p_stream->pSub_stmColList()->nChildCards();
  p_stream->Y_vSet(P_OdnLo, m_iOdnLo);
  m_iOdnHi = m_iOdnLo + nStreamCols - 1;
  p_stream->Y_vSet(P_OdnHi, m_iOdnHi);

  p_stream->vMark1(MARK_CHANGED);

  CardIter xCI(p_stream->pSub_stmColList());
  i8 iTblColOdn = 1;
  i8 iStmColOdn = m_iOdnLo;
  while (Card_StmCol* p_stmCol = _pCard_StmCol(xCI.pEachChild(CK_STMCOL))) {
    Card_StmColOdn* p_stmColOrd = new Card_StmColOdn;
    M_vCard_Set_StreamColOrdinal(p_stmColOrd, iStmColOdn);
    p_stmCol->vAppend(p_stmColOrd);

    Card_TblColOdn* p_tblColOrd = new Card_TblColOdn;
    M_vCard_Set_TableColOrdinal(p_tblColOrd, cTblSyn, iTblColOdn);
    p_stmCol->vAppend(p_tblColOrd);

    iStmColOdn++;
    iTblColOdn++;
  }

  m_iOdnLo = m_iOdnHi + 1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vStream_Adjust_OrdinalRange(Card_Stream* p_stream, i8 iLo, i8 iHi)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  p_stream->Y_vSet(P_OdnLo, iLo);
  p_stream->Y_vSet(P_OdnHi, iHi);

  p_stream->vMark1(MARK_CHANGED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTable_Produce_MaybeIndexJoin(Card_Table* p_table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_TableNm* p_tableNm = p_table->pSub_tableNm();

  Table* pTable = A_pAccess_Table(p_tableNm->sz());

  Card_IndexList* p_indexList = C_pProduce_indexList_ofTable(pTable);

  while (Card_Index* p_index = _pCard_Index(p_indexList->pPopChild())) {
    Card_MaybeIndexJoin* p_maybeIndexJoin = new Card_MaybeIndexJoin;
    p_maybeIndexJoin->vAppend(p_index);
    Card_Note* p_note = new Card_Note(SEP_SCRATCH);
    p_table->vAppend(p_note);
    p_table->vAppend(p_maybeIndexJoin);
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
void Xmodify::M_vTerm_Bump_KeyOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 iKeyOdn = E_iValue_KeyOdn(p_term->szValue());

  M_vCard_Set_KeyOrdinal(p_term,iKeyOdn+1);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::M_vTerm_Bump_StreamColOrdinal(Card_Term* p_term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 iStmColOdn = E_iValue_StreamColOdn(p_term->szValue());

  M_vCard_Set_StreamColOrdinal(p_term,iStmColOdn+1);
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
      p_pred->vMark1(MARK_AT);
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
      p_pred->vMark1(MARK_AT);
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

  //vTraceing("iAssignPredLeft candidate %p\n"_Stz,p_pred);
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
  if (bPullUp_StreamContainsPred(p_table, p_stream, p_pred)) {
    char cTableSyn = p_table->cValue();
    return cTableSyn;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this table+stream?                                  */
/*----------------------------------------------------------------------------*/
bool Xmodify::bPullUp_StreamContainsPred(Card_Table* p_table,
                                          Card_Stream* p_stream,
                                          Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  eCK ePredCard = pCard_PRED->eCard();

  char cTableSyn = p_table->cValue();

  if (CK_TERM == ePredCard) {

    Stz* szTerm = pCard_PRED->sz();

    if (T_bValue_IsParamOrdinal(szTerm))
      return false;

    if (! T_bValue_IsTableColOrdinal(szTerm))
      return false;

    char cTblColSyn = E_cValue_TableColSyn(szTerm);
    if (cTableSyn < cTblColSyn)
      return false;

    return true;
  }

  i8 nSubCards = pCard_PRED->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_PRED->pGetChild(i_Child);

    if (!bPullUp_StreamContainsPred(p_table, p_stream, static_cast<Card_PRED *>(pSubCard)))
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

  if (MARK_AT == pCard_pred->cMark1()) {

    Card *pMainCard;
    char cTableSyn = pCard_pred->cMark2();
    Card_Table *pCard_table = p_deck->pFindTableCard(cTableSyn, pMainCard);

    if (pCard_table) {
      Card_Filter *pCard_filter_existing = pMainCard->pSub_filter();

      if (pCard_filter_existing) {
        pCard_filter_existing->vMark1(pCard_table->cMark1());
        pCard_filter_existing->vMark2(pCard_table->cMark2());
        vPullUp_MovePredToFilter(pCard_filter_existing, pCard_pred);
      } else {
        Card_Filter *pCard_filter_new = new Card_Filter;
        pCard_filter_new->vMark1(pCard_table->cMark1());
        pCard_filter_new->vMark2(pCard_table->cMark2());
        pMainCard->vAppend(pCard_filter_new);
        vPullUp_MovePredToFilter(pCard_filter_new, pCard_pred);
      }

      M_vBoolCalc_RemovePred(p_calcBool, pCard_pred);

      return true;
    }
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

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
