/*============================================================================*/
/* Card_FoldExpr.cpp -- Fold Typed Expression                                 */
/*============================================================================*/

#include "Card_FoldExpr.hpp"

#include "Card_Type.hpp"
#include "Card_As.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_FoldExpr,CK_FOLDEXPR)
__aNewFunction(Card_FoldExpr,CN_CARD)
__vDeleteFunction(Card_FoldExpr,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_FoldExpr::Card_FoldExpr(void)
: Card(CK_FOLDEXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_FoldExpr::~Card_FoldExpr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
