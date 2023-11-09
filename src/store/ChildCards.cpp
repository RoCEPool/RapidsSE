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

  for (Card* pCard = pPopChild(); pCard; pCard = pPopChild())
    delete pCard;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append child card                                                          */
/*----------------------------------------------------------------------------*/
void ChildCards::vAppendChild(Card* pChild)
{
  RASSERT(pChild);

  vAppend(pChild);

  pChild->vAddedCorrectly();

  pChild->vOnAdd_child();
}

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
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
i8 ChildCards::nChildCards(eCK eChild)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = 0;

  eCK eSubCard;
  i8 nSubCards = nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pSubCard = pGetChild(i_Child, eSubCard);

    if (eChild == pSubCard->eCard())
      nChildren++;
  }

  return nChildren;
}

/*----------------------------------------------------------------------------*/
/* remove and delete all children (non-recursive)                             */
/*----------------------------------------------------------------------------*/
void ChildCards::vDeleteAllChildren(void)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  while (Card* pChild = pPopChild()) {
    delete pChild;
  }
}

/*----------------------------------------------------------------------------*/
/* remove and delete given immediate child card (non-recursive)               */
/*----------------------------------------------------------------------------*/
void ChildCards::vDeleteChild(Card* pChild)
{
  RASSERT(pChild);

  RASSERT(bRemoveChild(pChild));

  delete pChild;
}

/*----------------------------------------------------------------------------*/
/* remove and delete given immediate child card (non-recursive)               */
/*----------------------------------------------------------------------------*/
void ChildCards::vDeleteSub(Card* pSub)
{
  RASSERT(pSub);

  vRemoveSub(pSub);

  delete pSub;
}


/*----------------------------------------------------------------------------*/
/* ensure card is first                                                       */
/*----------------------------------------------------------------------------*/
void ChildCards::vEnsureFirst(eCK eChild)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Child;
  Card* pChild = pFindChild(eChild,i_Child);
  if ((! pChild) || 0 == i_Child)
    return;

  vRotateFirst(pChild);
}

/*----------------------------------------------------------------------------*/
/* ensure card is first                                                       */
/*----------------------------------------------------------------------------*/
void ChildCards::vEnsureLast(eCK eChild)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pChild_last = pPeekChild_tail();
  if (! pChild_last)
    return;
  if (eChild == pChild_last->eCard())
    return;

  Card* pChild = pFindChild(eChild);
  if (! pChild)
    return;

  vRotateLast(pChild);
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
Card* ChildCards::pFindChild(eCK eCard, Stz* szFacet, i8& i_Child)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card* pFoundCard = nullptr;

  eCK eChildCard;
  i8 nChildren = nChildCards();
  for (i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child, eChildCard);

    if (eCard == pChildCard->eCard() && pChildCard->pSymbol()->bHasFacet(szFacet)) {
      pFoundCard = pChildCard;
      break;
    }
  }

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pFindChildWithSub(eCK eChild, eCK eSub, Stz* szFacet, i8& i_Child)
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
    if (! pChildCard->pFindChild(eSub, szFacet, i_Sub))
      continue;

    pFoundChild = pChildCard;
    break;
  }

  return pFoundChild;
}

/*----------------------------------------------------------------------------*/
/* find sub-card                                                              */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pFindSubCard(eCK eSub)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  Card* pFoundCard = nullptr;
  while (pSubCard_I->bHasNext()) {
    Card* pSubCard = pSubCard_I->pNext();

    if (eSub == pSubCard->eCard()) {
      pFoundCard = pSubCard;
      break;
    }

    pFoundCard = pSubCard->pFindSubCard(eSub);
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
eCK ChildCards::eGetChild(i8 i_Child, Card*& rpChild)
{
  rpChild = pGet(i_Child);
  if (! rpChild)
    return CK_NONE;

  eCK eChild = rpChild->eCard();
  return eChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pGetChild(i8 i_Child)
{
  Card* pChild = pGet(i_Child);
  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pGetChild(i8 i_Child, eCK& reChild)
{
  Card* pChild = pGet(i_Child);

  if (pChild)
    reChild = pChild->eCard();
  else
    reChild = CK_NONE;

  return pChild;
}

/*----------------------------------------------------------------------------*/
/* insert child card after given card                                         */
/*----------------------------------------------------------------------------*/
void ChildCards::vInsertChildAfter(Card* pChild, Card* pAfter)
{
  RASSERT(pChild);
  RASSERT(pAfter);

  vInsertAfter(pChild,pAfter);

  pChild->vAddedCorrectly();
  pChild->vOnAdd_child();
}

/*----------------------------------------------------------------------------*/
/* insert child card before given card                                        */
/*----------------------------------------------------------------------------*/
void ChildCards::vInsertChildBefore(Card* pChild, Card* pBefore)
{
  RASSERT(pChild);
  RASSERT(pBefore);

  vInsertBefore(pChild,pBefore);

  pChild->vAddedCorrectly();
  pChild->vOnAdd_child();
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pPeekChild(i8 i_Child)
{
  Card* pChild = pPeek(i_Child);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCK ChildCards::ePeekChild(i8 i_Child)
{
  Card* pChild = pPeekChild(i_Child);

  if (pChild)
    return pChild->eCard();

  return CK_NONE;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pPeekChild_tail(void)
{
  i8 i_Last = nChildCards() - 1;
  if (i_Last < 0)
    return nullptr;

  Card* pChild = pPeek(i_Last);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pPopChild(void)
{
  i8 nChildren = nChildCards();
  if (! nChildren)
    return nullptr;

  Card* pChild = pPop();
  RASSERT(pChild);

  pChild->vRemovedCorrectly();
  pChild->vOnRemove_child();

  return pChild;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* ChildCards::pPopChild_tail(void)
{
  i8 nChildren = nChildCards();
  if (! nChildren)
    return nullptr;

  Card* pChild = pGetChild(nChildren-1);
  RASSERT(pChild);
  vRemoveChild(pChild);

  return pChild;
}

/*----------------------------------------------------------------------------*/
/* pop top child if it matches given symbol                                   */
/*----------------------------------------------------------------------------*/
bool ChildCards::bPopDeleteChildIf(eCK eCard, Symbol xSymbol)
{
  Card* pChild = pPeekChild();
  if (! pChild)
    return false;

  if (pChild->bIsCard(eCard) && pChild->bSymbolMatches(&xSymbol)) {
    pChild = pPopChild();
    RASSERT(pChild);
    delete pChild;
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void ChildCards::vPushChild(Card* pChild)
{
  RASSERT(pChild);

  vPush(pChild);

  pChild->vAddedCorrectly();
  pChild->vOnAdd_child();
}

/*----------------------------------------------------------------------------*/
/* remove given immediate child card (non-recursive)                          */
/*----------------------------------------------------------------------------*/
bool ChildCards::bRemoveChild(Card* pChild)
{
  RASSERT(pChild);

  if (C_UNKNOWN == iFindChild(pChild))
    return false;

  vRemove(pChild);

  pChild->vRemovedCorrectly();
  pChild->vOnRemove_child();

  return true;
}

/*----------------------------------------------------------------------------*/
/* remove given immediate child card (non-recursive)                          */
/*----------------------------------------------------------------------------*/
void ChildCards::vRemoveChild(Card* pChild)
{
  //RASSERT(C_UNKNOWN != iFindChild(pChild));

  RASSERT(pChild);

  vRemove(pChild);

  pChild->vRemovedCorrectly();
  pChild->vOnRemove_child();
}

/*----------------------------------------------------------------------------*/
/* remove given subCard (recursive)                                           */
/*----------------------------------------------------------------------------*/
void ChildCards::vRemoveSub(Card* pSub)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  RASSERT(pSub);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  while (pSubCard_I->bHasNext()) {
    Card* pSub_test = pSubCard_I->pNext();

    if (pSub_test == pSub) {
      RASSERT(bRemoveChild(pSub_test));
      pSub_test->vRemovedCorrectly();
      pSub_test->vOnRemove_child();
      break;
    }
    pSub_test->vRemoveSub(pSub);
  }

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* remove given child card from this card and replace it with its children    */
/*----------------------------------------------------------------------------*/
void ChildCards::vReplaceChildWithItsSubs(Card* pChild)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  Card* pAfter = pChild;

  while (Card* pSub = pChild->pPopChild()) {
    vInsertChildAfter(pSub, pAfter);
    pAfter = pSub;
  }

  vRemoveChild(pChild);
}

/*----------------------------------------------------------------------------*/
/* replace immediate child with given card                                    */
/*----------------------------------------------------------------------------*/
void ChildCards::vReplaceChild(Card* pChild, Card* pWith)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  vReplace(pChild, pWith);

  pWith->vAddedCorrectly();
  pWith->vOnAdd_child();

  pChild->vRemovedCorrectly();
  pChild->vOnRemove_child();
}

/*----------------------------------------------------------------------------*/
/* remove and push child card                                                 */
/*----------------------------------------------------------------------------*/
void ChildCards::vRotateFirst(Card* pChild)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vRemoveChild(pChild);
  vPushChild(pChild);
}

/*----------------------------------------------------------------------------*/
/* remove and append child card                                               */
/*----------------------------------------------------------------------------*/
void ChildCards::vRotateLast(Card* pChild)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vRemoveChild(pChild);
  vAppendChild(pChild);
}

/*----------------------------------------------------------------------------*/
/* transfer all child card (incl. sub-cards) from given card set to this card */
/*----------------------------------------------------------------------------*/
void ChildCards::vTransferChildren(Card* pFrom)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  while (Card* pPoppedChild = pFrom->pPopChild())
    vAppendChild(pPoppedChild);
}

/*----------------------------------------------------------------------------*/
/* transfer all child card (incl. sub-cards) from given card set to this card */
/*----------------------------------------------------------------------------*/
void ChildCards::vTransferChildren(Card* pFrom, eSF eFacet_stopAfter)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  while (Card *pPoppedChild = pFrom->pPopChild()) {
    vAppendChild(pPoppedChild);

    if (pPoppedChild->pSymbol()->bFacetIs(eFacet_stopAfter))
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
