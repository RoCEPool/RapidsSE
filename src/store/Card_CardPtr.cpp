/*============================================================================*/
/* Card_CardPtr.cpp -- Card                                                    */
/*============================================================================*/

#include "Card_CardPtr.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CardPtr, CK_CARDPTR)
__aNewFunction(Card_CardPtr, CN_CARD)
__vDeleteFunction(Card_CardPtr, CN_CARD)


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
/* return pCard                                                               */
/*----------------------------------------------------------------------------*/
Card* Card_CardPtr::pCard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pCard = _pCard(Y_pVal(P_Card));

  return pCard;
}

/*----------------------------------------------------------------------------*/
/* set pCard                                                                  */
/*----------------------------------------------------------------------------*/
void Card_CardPtr::vCard(Card* pCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Y_vSet(P_Card, pCard);
}


/*----------------------------------------------------------------------------*/
/* print {card}                                                               */
/*----------------------------------------------------------------------------*/
void Card_CardPtr::vCardPtrPrint(Ribbon* pRibbon, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CardPtr* pCard_CardPtr = _pCard_CardPtr(this);
  Card* pCard = _pCard(pCard_CardPtr->pCard());

  pRibbon->vSput(FR_NAME,pCard->szCardNm());

  if (pRibbon->iArea())
    return;

  pCard->vPrintProperties(pRibbon, iDepth, pParent);

  CardIter xCI(this);
  while (Card *pSubCard = xCI.pEachChild())
    pSubCard->vPrintProperties(pRibbon, iDepth + 1,pParent);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
