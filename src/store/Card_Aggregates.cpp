/*============================================================================*/
/* Card_Aggregates.cpp -- Aggregates                                          */
/*============================================================================*/

#include "Card_Aggregates.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Aggregates,CK_AGGREGATES)
__aNewFunction(Card_Aggregates,CN_CARD)
__vDeleteFunction(Card_Aggregates,CN_CARD)

__bIsCard(STORE,Card_Aggregates,CK_AGGREG)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Aggregates::Card_Aggregates(void)
: Card_Project(CK_AGGREG)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Aggregates::~Card_Aggregates(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
