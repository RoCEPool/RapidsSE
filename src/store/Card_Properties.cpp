/*============================================================================*/
/* Card_Properties.cpp -- Set of Properties                                   */
/*============================================================================*/

#include "Card_Properties.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Properties,CK_PROPERTIES)
__aNewFunction(Card_Properties,CN_CARD)
__vDeleteFunction(Card_Properties,CN_CARD)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Properties::Card_Properties(eCK eKind)
: Card(CK_PROPERTIES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Properties::~Card_Properties(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vDeleteAll();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
