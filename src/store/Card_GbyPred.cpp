/*============================================================================*/
/* Card_GbyPred.cpp -- GroupBy Predicate                                      */
/*============================================================================*/

#include "Card_GbyPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_GbyPred,CK_GBYPRED)
__aNewFunction(Card_GbyPred,CN_CARD)
__vDeleteFunction(Card_GbyPred,CN_CARD)

__bIsCard(STORE,Card_GbyPred,CK_GBYPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_GbyPred::Card_GbyPred(void)
: Card_PRED(CK_GBYPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_GbyPred::~Card_GbyPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
