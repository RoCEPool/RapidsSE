/*============================================================================*/
/* Card_AggPred.cpp -- Aggregate Predicate                                    */
/*============================================================================*/

#include "Card_AggPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_AggPred,CK_AGGPRED)
__aNewFunction(Card_AggPred,CN_CARD)
__vDeleteFunction(Card_AggPred,CN_CARD)

__bIsCard(STORE,Card_AggPred,CK_AGGPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AggPred::Card_AggPred(void)
: Card_PRED(CK_AGGPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AggPred::~Card_AggPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
