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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
