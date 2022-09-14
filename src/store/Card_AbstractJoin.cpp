/*============================================================================*/
/* Card_AbstractJoin.cpp -- Avstract Join                                     */
/*============================================================================*/

#include "Card_AbstractJoin.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_AbstractJoin,CK_ABSTRACTJOIN)
__aNewFunction(Card_AbstractJoin,CN_CARD)
__vDeleteFunction(Card_AbstractJoin,CN_CARD)

__bIsCard(STORE,Card_AbstractJoin,CK_ABSTRACTJOIN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AbstractJoin::Card_AbstractJoin(void)
: Card_MAIN(CK_ABSTRACTJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AbstractJoin::~Card_AbstractJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
