/*============================================================================*/
/* Card_Key.cpp -- Key                                                        */
/*============================================================================*/

#include "Card_Key.hpp"

#include "Card_As.hpp"
#include "Card_Direction.hpp"
#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_StmColOdn.hpp"
#include "Card_Term.hpp"
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

  if (Card_As* p_as = p_expr->pSub_as())
    p_expr_new->vAppend(p_as->pDupCardAndSubCards());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  if (p_expr->pSub_rpnCalc())
    p_rpnCalc->vAppend(p_expr->pSub_rpnCalc()->pGetTerm(0)->pDupCardAndSubCards());
  p_expr_new->vAppend(p_rpnCalc);

  if (p_expr->pSub_direction()) {
    Card_Direction *p_direction = new Card_Direction(p_expr->pSub_direction()->szValue());
    p_expr_new->vAppend(p_direction);
  }
  else
  if (bAsc) {
    Card_Direction *p_direction = new Card_Direction("ASC"_Stz);
    p_expr_new->vAppend(p_direction);
  }
  vAppend(p_expr_new);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Key::bIsEqual(Card_Key* p_key_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Type* p_type_this = pSub_type();
  Card_Type* p_type_arg  = p_key_arg->pSub_type();

  if (p_type_this) {
    if (p_type_arg) {
      if (! p_type_this->bIsEqual(p_type_arg))
        return false;
    }
    else {
      return false;
    }
  }
  else
  if (p_type_arg) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_RpnCalc* p_rpnCalc_this = pSub_rpnCalc();
  Card_RpnCalc* p_rpnCalc_arg  = p_key_arg->pSub_rpnCalc();

  if (p_rpnCalc_this) {
    if (p_rpnCalc_arg) {
      if (! p_rpnCalc_this->bIsEqual(p_rpnCalc_arg))
        return false;
    }
    else {
      return false;
    }
  }
  else
  if (p_rpnCalc_arg) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Direction* p_direction_this = pSub_direction();
  Card_Direction* p_direction_arg  = p_key_arg->pSub_direction();

  if (p_direction_this) {
    if (p_direction_arg) {
      if (! p_direction_this->bIsEqual(p_direction_arg))
        return false;
    }
    else {
      return false;
    }
  }
  else
  if (p_direction_arg) {
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return true;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
