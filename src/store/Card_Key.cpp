/*============================================================================*/
/* Card_Key.cpp -- Key                                                        */
/*============================================================================*/

#include "Card_Key.hpp"

#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_TblColOdn.hpp"
#include "Card_Type.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Key,CK_KEY)
__aNewFunction(Card_Key,CN_CARD)
__vDeleteFunction(Card_Key,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Key::Card_Key(void)
: Card_ELIST(CK_KEY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Key::~Card_Key(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Key::vAdd_expr_BasedOn_expr(Card_Expr* p_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Expr = nChildCards();

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vRenumber(i_Expr+1,true);

  p_expr_new->vAppend(p_expr->pSub_type()->pDupCardAndSubCards());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  p_rpnCalc->vAppend(p_expr->pSub_tblColOdn()->pDupCardAndSubCards());

  p_expr_new->vAppend(p_rpnCalc);

  vAppend(p_expr_new);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
