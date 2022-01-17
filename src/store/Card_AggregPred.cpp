/*============================================================================*/
/* Card_AggregPred.cpp -- Aggregate Predicate                                 */
/*============================================================================*/

#include "Card_AggregPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_AggregPred,CK_AGGREGPRED)
__aNewFunction(Card_AggregPred,CN_CARD)
__vDeleteFunction(Card_AggregPred,CN_CARD)

__bIsCard(STORE,Card_AggregPred,CK_AGGREGPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AggregPred::Card_AggregPred(eCK eKind)
: Card_Pred(CK_AGGREGPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AggregPred::~Card_AggregPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
