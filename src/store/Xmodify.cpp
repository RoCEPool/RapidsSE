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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::T_vPRED_CullByKids(Card_PRED* p_PRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildCards = p_PRED->nItems();
  for (i8 i_ChildCard = 0; i_ChildCard < nChildCards; i_ChildCard++) {
    Card* pCard_Child = p_PRED->pGet(i_ChildCard);
    if (!T_bPRED_IsPred(pCard_Child))
      continue;

    Card_PRED* pCard_ChildPRED = (Card_PRED*)pCard_Child;

    T_vPRED_CullByKids(pCard_ChildPRED);

    if (0 == pCard_ChildPRED->nItems()) {

      p_PRED->vRemove(pCard_ChildPRED);
      delete pCard_ChildPRED;

      i_ChildCard--;
      nChildCards--;

      continue;
    }

    if (1 == pCard_ChildPRED->nItems()) {

      Card_PRED* pCard_KidPRED = (Card_PRED*)pCard_Child->pPop();

      p_PRED->vReplace(pCard_ChildPRED,pCard_KidPRED);
      delete pCard_ChildPRED;

      continue;
    }


  }
}

/*----------------------------------------------------------------------------*/
/* mark the bounds of this <predCmp>                                          */
/*----------------------------------------------------------------------------*/
void Xmodify::T_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred)
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
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Xmodify::X_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      X_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      X_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      X_vBoolCalc_RemovePred(p_boolCalc, p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDPRED == eCard_pred || CK_ORPRED == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(p_pred->pGetChild(i_Child));
    if (X_bBoolCalc_CullPred(p_boolCalc, pCard_pred_child, cWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xmodify::X_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (! p_boolCalc->nChildCards())
    return;

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  while (pCard_pred &&
         X_bBoolCalc_CullPred(p_boolCalc, pCard_pred, cWithMark) &&
         p_boolCalc->nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Xmodify::X_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xmodify::X_vFilter_CullPred(Card_Filter* p_filter, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = p_filter->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = p_filter->pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_BOOLCALC)
      continue;

    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pChildCard);

    X_vBoolCalc_CullPred(p_boolCalc, cWithMark);
  }
}

/*----------------------------------------------------------------------------*/
/* add table and index                                                        */
/*----------------------------------------------------------------------------*/
void Xmodify::X_vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream)
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
/* convert <calc> to <calcBool>                                               */
/*----------------------------------------------------------------------------*/
void Xmodify::X_vSubCard_ReplaceSub(Card* pSub, Card* pInParent, Card* pWith)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card* pParent = pInParent->pFindParent(pSub);

  pParent->vReplaceChild(pSub,pWith);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
