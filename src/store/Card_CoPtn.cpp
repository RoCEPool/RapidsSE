/*============================================================================*/
/* Card_CoPtn.cpp -- Co-Partition                                             */
/*============================================================================*/

#include "Card_CoPtn.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CoPtn,CK_COPTN)
__aNewFunction(Card_CoPtn,CN_CARD)
__vDeleteFunction(Card_CoPtn,CN_CARD)

__bIsCard(STORE,Card_CoPtn,CK_COPTN)
__pGetSubFunction_Card(STORE,Card_CoPtn,CK_COPTN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CoPtn::Card_CoPtn(void)
: Card(CK_COPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CoPtn::~Card_CoPtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
