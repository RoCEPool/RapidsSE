/*============================================================================*/
/* Card_Key.cpp -- Key                                                        */
/*============================================================================*/

#include "Card_Key.hpp"

#include "Card_Direction.hpp"
#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_Term.hpp"
#include "Card_StmColOdn.hpp"
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
/* add a key expr to this key based on given projection expr                  */
/*----------------------------------------------------------------------------*/
void Card_Key::vAdd_key_expr_BasedOn_expr(Card_Expr* p_expr, bool bAsc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Key = nChildCards() + 1;

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vRenumber(i_Key, true);

  p_expr_new->vAppend(p_expr->pSub_type()->pDupCardAndSubCards());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  if (p_expr->pSub_tblColOdn())
    p_rpnCalc->vAppend(p_expr->pSub_tblColOdn()->pDupCardAndSubCards());
  else
  if (p_expr->pSub_stmColOdn())
    p_rpnCalc->vAppend(p_expr->pSub_stmColOdn()->pDupCardAndSubCards());
  else
  if (p_expr->pSub_rpnCalc())
    p_rpnCalc->vAppend(p_expr->pSub_rpnCalc()->pGetTerm(0)->pDupCardAndSubCards());
  p_expr_new->vAppend(p_rpnCalc);

  if (bAsc) {
    Card_Direction *p_direction = new Card_Direction("ASC"_Stz);
    p_expr_new->vAppend(p_direction);
  }
  vAppend(p_expr_new);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
