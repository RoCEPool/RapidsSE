/*============================================================================*/
/* Card_ExprCalc.cpp -- Calculation Predicate                                 */
/*============================================================================*/

#include "Card_ExprCalc.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_ExprCalc,CK_EXPRCALC)
__aNewFunction(Card_ExprCalc,CN_CARD)
__vDeleteFunction(Card_ExprCalc,CN_CARD)

__bIsCard(STORE,Card_ExprCalc,CK_EXPRCALC)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ExprCalc::Card_ExprCalc(void)
: Card_EXPRESSION(CK_EXPRCALC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ExprCalc::~Card_ExprCalc(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
