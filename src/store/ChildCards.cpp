/*============================================================================*/
/* ChildCards.cpp -- Card Set                                                    */
/*============================================================================*/

#include "ChildCards.hpp"

#include "Card.hpp"
#include "Card_Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ChildCards::ChildCards(void)
{
  //_precondition_(_ADDRESS_,_FOOTER_);
          
  //_postcondition_(_ADDRESS_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ChildCards::~ChildCards(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  for (Card* pCard = pPop(); pCard; pCard = pPop())
    delete pCard;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get number of child cards                                                  */
/*----------------------------------------------------------------------------*/
i8 ChildCards::nChildCards(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = this->nItems();

  return nChildren;
}

/*----------------------------------------------------------------------------*/
/* concat children                                                            */
/*----------------------------------------------------------------------------*/
void ChildCards::vConcatChildren(Stz* szTerms, eCK eChild)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);
    if (CK_NONE == eChild || eChild == pChildCard->eCard())
      szTerms->vCatSprint(" %s"_Stz, pChildCard->szSymbol(SF_NONE));
  }
}

/*----------------------------------------------------------------------------*/
/* remove and delete all children (non-recursive)                             */
/*----------------------------------------------------------------------------*/
void ChildCards::vDeleteAllChildren(void)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  while(Card* pChild = pPopChild()) {
    //vDeleteChild(pChild);
    delete pChild;
  }
}

/*----------------------------------------------------------------------------*/
/* remove and delete given immediate child card (non-recursive)               */
/*----------------------------------------------------------------------------*/
void ChildCards::vDeleteChild(Card* pChild)
{

  vRemove(pChild);

  delete pChild;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
i8 ChildCards::iFindChild(eCK eCard)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards = nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card* pTestCard = pGetChild(i_Sub);

    if (pTestCard->bIsCard(eCard))
      return i_Sub;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
i8 ChildCards::iFindChild(Card* pChild)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards = nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card* pTestCard = pGetChild(i_Sub);

    if (pChild == pTestCard)
      return i_Sub;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pFindChild(eCK eCard)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Child;

  Card* pFoundCard = pFindChild(eCard, i_Child);

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pFindChild(eCK eCard, i8& i_Child)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pFoundCard = nullptr;

  eCK eSubCard;
  i8 nSubCards = nChildCards();
  for (i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChild(i_Child, eSubCard);

    if (eCard == pSubCard->eCard()) {
      pFoundCard = pSubCard;
      break;
    }
  }

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pFindChild(eCK eCard, Stz* szValue, i8& i_Child)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pFoundCard = nullptr;

  eCK eChildCard;
  i8 nChildren = nChildCards();
  for (i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child, eChildCard);

    if (eCard == pChildCard->eCard() && pChildCard->pSymbol()->bSame(szValue)) {
      pFoundCard = pChildCard;
      break;
    }
  }

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pFindChildWithSub(eCK eChild, eCK eSub, Stz* szValue, i8& i_Child)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pFoundChild = nullptr;

  eCK eChildCard;
  i8 nChildrenCards = nChildCards();
  for (i_Child = 0; i_Child < nChildrenCards; i_Child++) {
    Card* pChildCard = pGetChild(i_Child, eChildCard);

    if (! pChildCard->bIsCard(eChild))
      continue;

    i8 i_Sub;
    if (! pChildCard->pFindChild(eSub,szValue,i_Sub))
      continue;

    pFoundChild = pChildCard;
    break;
  }

  return pFoundChild;
}

/*----------------------------------------------------------------------------*/
/* find sub-card                                                              */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pFindSubCard(eCK eCard)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  Card* pFoundCard = nullptr;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    if (eCard == pSubCard->eCard()) {
      pFoundCard = pSubCard;
      break;
    }

    pFoundCard = pSubCard->pFindSubCard(eCard);
    if (pFoundCard)
      break;
  }

  delete pSubCard_I;

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find table card by synonym                                                 */
/*----------------------------------------------------------------------------*/
Card_Table* ChildCards::pFindTableCard(char cTableSyn, Card*& rpMainCard)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_BOARD, _FOOTER_);

  Card_Table* p_table = nullptr;

  i8 nMains = nChildCards();
  for (i8 i_Main = 0; i_Main < nMains; i_Main++) {
    rpMainCard = pGetChild(i_Main);
    eCK eMainCard = rpMainCard->eCard();

    if (CK_TABLE == eMainCard)
      p_table = _pCard_Table(rpMainCard);
    else
      p_table = rpMainCard->pSub_table();

    if (! p_table)
      continue;

    if (cTableSyn == p_table->pSymbol()->cSF_char())
      return p_table;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pGetChild(i8 i_Child)
{
  Card* pCard = pGet(i_Child);
  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pGetChild(i8 i_Child, eCK& reCard)
{
  Card* pCard = pGet(i_Child);

  if (pCard)
    reCard = pCard->eCard();
  else
    reCard = CK_NONE;

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* get expr relating to term                                                  */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pGetChild_bySym(STORE::Symbol *pSymbol)
{
  i8 i_Child  = pSymbol->iSF_ordinal() - 1;

  Card* pCard = pGetChild(i_Child);

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* morph all children of eFrom to be eTo                                      */
/*----------------------------------------------------------------------------*/
void ChildCards::vMorphChildrenTo(eCK eFrom, eCK eTo)
{
  i8 nSubCards = nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card *p_child = pGetChild(i_Sub);
    if (p_child->bIsCard(eFrom))
      p_child->vCard(eTo);
  }

}

/*----------------------------------------------------------------------------*/
/* convert child values to an stz list                                        */
/*----------------------------------------------------------------------------*/
void ChildCards::vOntoStzList(StzList* pStzList)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  i8 nSubCards = nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card* p_child = pGetChild(i_Sub);
    if (p_child->szSymbol(SF_NONE))
      pStzList->vAppend(p_child->szSymbol(SF_NONE));
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pPopChild(void)
{
  Card* pChild = pPop();

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pPlopChild(void)
{

  i8 nChildren = nChildCards();
  if (! nChildren)
    return nullptr;

  Card* pChild = pGetChild(nChildren-1);
  vRemoveChild(pChild);

  return pChild;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void ChildCards::vPromoteSubs(Card* pChildCard)
{
  Card* p_after = pChildCard;

  while (Card* pSub = pChildCard->pPop()) {
    vInsertAfter(pSub,p_after);
    p_after = pSub;
  }
}

/*----------------------------------------------------------------------------*/
/* remove all children (non-recursive)                                        */
/*----------------------------------------------------------------------------*/
void ChildCards::vRemoveAllChildren(void)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  while(Card* pChild = pPopChild()) {
    vRemoveChild(pChild);
  }
}

/*----------------------------------------------------------------------------*/
/* remove given immediate child card (non-recursive)                          */
/*----------------------------------------------------------------------------*/
void ChildCards::vRemoveChild(Card* pChildCard)
{

  vRemove(pChildCard);
}

/*----------------------------------------------------------------------------*/
/* remove given subCard (recursive)                                           */
/*----------------------------------------------------------------------------*/
void ChildCards::vRemoveSub(Card* pSubCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  while (pSubCard_I->bHasNext()) {
    Card* pTestCard = pSubCard_I->pNext();

    if (pTestCard == pSubCard) {
      vRemove(pTestCard);
      break;
    }
    pTestCard->vRemoveSub(pSubCard);
  }

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* remove given child card from this card and replace it with its children    */
/*----------------------------------------------------------------------------*/
void ChildCards::vReplaceChildWithItsSubs(Card* pChildCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  Card* pAfter = pChildCard;

  while (Card* pSub = pChildCard->pPop()) {
    vInsertAfter(pSub, pAfter);
    pAfter = pSub;
  }

  vRemoveChild(pChildCard);
}

/*----------------------------------------------------------------------------*/
/* replace immediate child with given card                                    */
/*----------------------------------------------------------------------------*/
void ChildCards::vReplaceChild(Card* pChildCard, Card* pWithCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  vReplace(pChildCard, pWithCard);
}

/*----------------------------------------------------------------------------*/
/* replace immediate child with given card                                    */
/*----------------------------------------------------------------------------*/
void ChildCards::vRotateChild(Card* pChildCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  vRemoveChild(pChildCard);

  vPush(pChildCard);
}

/*----------------------------------------------------------------------------*/
/* convert child values to an stz list                                        */
/*----------------------------------------------------------------------------*/
StzList* ChildCards::pToStzList(void)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  StzList* pStzList = new StzList;

  vOntoStzList(pStzList);

  return pStzList;
}

/*----------------------------------------------------------------------------*/
/* transfer all child card (incl. sub-cards) from given card set to this card */
/*----------------------------------------------------------------------------*/
void ChildCards::vTransferChildren(Card* pFromCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  while (Card* pPoppedChildCard = pFromCard->pPop())
    vAppend(pPoppedChildCard);
}

/*----------------------------------------------------------------------------*/
/* transfer cards to this card from given card onwards                        */
/*----------------------------------------------------------------------------*/
void ChildCards::vTransferFrom(Card* pCards, Card* pFrom,
                               Card* pAt, bool bAfter)
{
  //_precondition_(_ADDRESS_,_FOOTER_);
  UNUSED(pCards);

  ListExoIter2_t<Card>* pCard_I = pNewIter();

  bool b = pCard_I->bPositionBefore(pFrom);
  if (! b) {
    delete pCard_I;
    return;
  }

  while (pCard_I->bHasNext()) {
    Card* pCard = pCard_I->pNext();

    pCard_I->vRemove();

    if (! pAt)
      vAppend(pCard);
    else
    if (bAfter)
      vInsertAfter(pAt,pCard);
    else
      vInsertBefore(pAt,pCard);
  }

  delete pCard_I;
}

/*----------------------------------------------------------------------------*/
/* transfer subset of cards from given card up till (and including) given card*/
/*----------------------------------------------------------------------------*/
void ChildCards::vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                                 Card* pAt, bool bAfter)
{
  //_precondition_(_ADDRESS_,_FOOTER_);
  UNUSED(pCards);

  ListExoIter2_t<Card>* pCard_I = pNewIter();

  bool b = pCard_I->bPositionBefore(pFrom);
  if (! b) {
    delete pCard_I;
    return;
  }

  while (pCard_I->bHasNext()) {
    Card* pCard = pCard_I->pNext();

    pCard_I->vRemove();

    if (! pAt)
      vAppend(pCard);
    else
    if (bAfter)
      vInsertAfter(pAt,pCard);
    else
      vInsertBefore(pAt,pCard);

    if (pTill == pCard)
      break;
  }

  delete pCard_I;
}

/*----------------------------------------------------------------------------*/
/* transfer cards to this card up till (and including) given card             */
/*----------------------------------------------------------------------------*/
void ChildCards::vTransferTill(Card* pCards, Card* pTill,
                               Card* pAt, bool bAfter)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  while (Card* pCard = pCards->pPop()) {

    if (! pAt)
      vAppend(pCard);
    else
    if (bAfter)
      vInsertAfter(pAt,pCard);
    else
      vInsertBefore(pAt,pCard);

    if (pTill == pCard)
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* remove any child with matching symbol                                      */
/*----------------------------------------------------------------------------*/
i8 ChildCards::nTrimChildren(eCK eCard, Stz* szSymbol)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  i8 nTrimmed = 0;

  i8 i_Child;

  while (Card* pChild = pFindChild(eCard,szSymbol,i_Child))  {
    vDeleteChild(pChild);
    nTrimmed++;
  }

  return nTrimmed;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
