/*============================================================================*/
/* CardSet.cpp -- Card Set                                                    */
/*============================================================================*/

#include "CardSet.hpp"

#include "Card.hpp"
#include "Card_Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CardSet::CardSet(void)
{
  //_precondition_(_ADDRESS_,_FOOTER_);
          
  //_postcondition_(_ADDRESS_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CardSet::~CardSet(void) noexcept(false)
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
i8 CardSet::nChildCards(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = this->nItems();

  return nChildren;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
i8 CardSet::iFindChild(Card* pChild)
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
Card* CardSet::pFindChild(eCK eCard)
{
  //_precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Child;

  Card* pFoundCard = pFindChild(eCard, i_Child);

  return pFoundCard;
}

/*----------------------------------------------------------------------------*/
/* find immediate child-card                                                  */
/*----------------------------------------------------------------------------*/
Card* CardSet::pFindChild(eCK eCard, i8& i_Child)
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
/* find sub-card                                                              */
/*----------------------------------------------------------------------------*/
Card* CardSet::pFindSubCard(eCK eCard)
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
Card_Table* CardSet::pFindTableCard(char cTableSyn, Card*& rpMainCard)
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
      p_table = _pSub_Table(rpMainCard);

    if (! p_table)
      continue;

    if (cTableSyn == p_table->cPpy(PPY_TableSyn))
      return p_table;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardSet::pGetChild(i8 i_Child)
{
  Card* pCard = pGet(i_Child);
  return pCard;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card* CardSet::pGetChild(i8 i_Child, eCK& reCard)
{
  Card* pCard = pGet(i_Child);

  if (pCard)
    reCard = pCard->eCard();
  else
    reCard = CK_NONE;

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* remove given immediate child card (non-recursive)                          */
/*----------------------------------------------------------------------------*/
void CardSet::vRemoveChild(Card* pChildCard)
{

  vRemove(pChildCard);
}

/*----------------------------------------------------------------------------*/
/* remove given subCard (recursive)                                           */
/*----------------------------------------------------------------------------*/
void CardSet::vRemoveSubCard(Card* pSubCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  ListExoIter2_t<Card>* pSubCard_I = pNewIter();

  while (pSubCard_I->bHasNext()) {
    Card* pTestCard = pSubCard_I->pNext();

    if (pTestCard == pSubCard) {
      vRemove(pTestCard);
      break;
    }
    pTestCard->vRemoveSubCard(pSubCard);
  }

  delete pSubCard_I;
}

/*----------------------------------------------------------------------------*/
/* remove given child card from this card and replace it with its children    */
/*----------------------------------------------------------------------------*/
void CardSet::vReplaceChildWithItsSubs(Card* pChildCard)
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
void CardSet::vReplaceChild(Card* pChildCard, Card* pWithCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  vReplace(pChildCard, pWithCard);
}

/*----------------------------------------------------------------------------*/
/* transfer all child card (incl. sub-cards) from given card set to this card */
/*----------------------------------------------------------------------------*/
void CardSet::vTransferChildren(Card* pFromCard)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

  while (Card* pPoppedChildCard = pFromCard->pPop())
    vAppend(pPoppedChildCard);
}

/*----------------------------------------------------------------------------*/
/* transfer cards to this card from given card onwards                        */
/*----------------------------------------------------------------------------*/
void CardSet::vTransferFrom(Card* pCards, Card* pFrom,
                         Card* pAt, bool bAfter)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

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
void CardSet::vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                           Card* pAt, bool bAfter)
{
  //_precondition_(_ADDRESS_,_FOOTER_);

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
void CardSet::vTransferTill(Card* pCards, Card* pTill,
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
