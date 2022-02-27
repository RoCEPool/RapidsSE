/*============================================================================*/
/* Card_CalcExpr.cpp -- Calculation Predicate                                 */
/*============================================================================*/

#include "Card_CalcExpr.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CalcExpr,CK_CALCEXPR)
__aNewFunction(Card_CalcExpr,CN_CARD)
__vDeleteFunction(Card_CalcExpr,CN_CARD)

__bIsCard(STORE,Card_CalcExpr,CK_CALCEXPR)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CalcExpr::Card_CalcExpr(void)
: Card_EXPRESSION(CK_CALCEXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CalcExpr::~Card_CalcExpr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
