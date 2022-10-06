/*============================================================================*/
/* Card_CardPtr.cpp -- Card                                                    */
/*============================================================================*/

#include "Card_CardPtr.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CardPtr, CK_CARDPTR)
__aNewFunction(Card_CardPtr, CN_CARD)
__vDeleteFunction(Card_CardPtr, CN_CARD)

__bIsCard(STORE,Card_CardPtr,CK_CARDPTR)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CardPtr::Card_CardPtr(Card* pCard)
: Card(CK_CARDPTR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vCard(pCard);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CardPtr::~Card_CardPtr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print {card}                                                               */
/*----------------------------------------------------------------------------*/
void Card_CardPtr::vCardPtrPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CardPtr* pCard_CardPtr = _pCard_CardPtr(this);
  Card* pCard = _pCard(pCard_CardPtr->pCard());

  pRibbon->vSput(FR_NAME,pCard->szCardNm());

  if (pRibbon->iArea())
    return;

  pCard->vPrintCardProperties(pRibbon, iDepth);

  SubCardIter* pSubCardIter = pCard->pNewSubCardIter();
  while (pSubCardIter->bHasNext()) {
    Card *pSubCard = pSubCardIter->pNext();
    pSubCard->vPrintCardProperties(pRibbon, iDepth + 1);
  }
  delete pSubCardIter;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
