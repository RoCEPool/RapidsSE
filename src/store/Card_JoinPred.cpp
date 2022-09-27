/*============================================================================*/
/* Card_JoinPred.cpp -- Join Predicate                                        */
/*============================================================================*/

#include "Card_JoinPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPred,CK_JOINPRED)
__aNewFunction(Card_JoinPred,CN_CARD)
__vDeleteFunction(Card_JoinPred,CN_CARD)

__bIsCard(STORE,Card_JoinPred,CK_JOINPRED)
__pGetSubFunction_Card(STORE,Card_JoinPred,CK_JOINPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPred::Card_JoinPred(void)
: Card_MAIN(CK_JOINPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPred::~Card_JoinPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
