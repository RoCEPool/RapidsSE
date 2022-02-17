/*============================================================================*/
/* Card_CalcPred.cpp -- Calculation Predicate                                 */
/*============================================================================*/

#include "Card_CalcPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CalcPred,CK_CALCPRED)
__aNewFunction(Card_CalcPred,CN_CARD)
__vDeleteFunction(Card_CalcPred,CN_CARD)

__bIsCard(STORE,Card_CalcPred,CK_CALCPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CalcPred::Card_CalcPred(void)
: Card_PRED(CK_CALCPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CalcPred::~Card_CalcPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
