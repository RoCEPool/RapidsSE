/*============================================================================*/
/* Xmodify.cpp --                                                       */
/*============================================================================*/

#include "Xmodify.hpp"


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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xmodify::~Xmodify(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

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
void Xmodify::M_vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream)
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
        X_pStmColList_Find_streamCol(pCard_StreamColList, szColNm, i_StreamColNo);

    s32 sOrdinal;
    sOrdinal.vSprint("$%lld"_Stz,iOrdLo+i_StreamColNo);
    pCard_idxCol->vValue(sOrdinal);

    pCard_idxCol->vMark1(MARK_CHANGED);
  }
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
