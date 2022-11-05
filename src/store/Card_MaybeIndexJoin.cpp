/*============================================================================*/
/* Card_MaybeIndexJoin.cpp -- Index Join                                           */
/*============================================================================*/

#include "Card_MaybeIndexJoin.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_MaybeIndexJoin,CK_MAYBEINDEXJOIN)
__aNewFunction(Card_MaybeIndexJoin,CN_CARD)
__vDeleteFunction(Card_MaybeIndexJoin,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_MaybeIndexJoin::Card_MaybeIndexJoin(void)
: Card(CK_MAYBEINDEXJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_MaybeIndexJoin::~Card_MaybeIndexJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
