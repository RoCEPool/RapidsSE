/*============================================================================*/
/* Card_Retrieve.cpp -- Scan                                                      */
/*============================================================================*/

#include "Card_Retrieve.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Retrieve,CK_RETRIEVE)
__aNewFunction(Card_Retrieve,CN_CARD)
__vDeleteFunction(Card_Retrieve,CN_CARD)

__bIsCard(STORE,Card_Retrieve,CK_RETRIEVE)
__pSubFunction_Card(STORE,Retrieve,CK_RETRIEVE)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Retrieve::Card_Retrieve(void)
: Card(CK_RETRIEVE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Retrieve::~Card_Retrieve(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is predicate within scope of this <scan> ?                                 */
/*----------------------------------------------------------------------------*/
bool Card_Retrieve::bIsPredInScope(Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
