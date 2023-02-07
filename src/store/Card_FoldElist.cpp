/*============================================================================*/
/* Card_FoldElist.cpp -- Fold Typed Expression                                 */
/*============================================================================*/

#include "Card_FoldElist.hpp"

#include "Card_Type.hpp"
#include "Card_As.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_FoldElist, CK_FOLDELIST)
__aNewFunction(Card_FoldElist, CN_CARD)
__vDeleteFunction(Card_FoldElist, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_FoldElist::Card_FoldElist(void)
: Card_ELIST(CK_FOLDELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_FoldElist::~Card_FoldElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_FoldElist::vPush_foldExpr_Into_gbyAgg_key(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  pCI->G.p_expr = new Card_Expr;

  pCI->G.p_expr->Y_vSet(P_Injected);

  pCI->G.p_expr->vAppend(p_expr_fold->pSub_type()->pDupCardAndSubCards());

  pCI->G.p_expr->vAppend(p_expr_fold->pSub_rpnCalc()->pDupCardAndSubCards());

  pCI->G.p_key->vPush(pCI->G.p_expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_FoldElist::vPush_foldExpr_Into_gbyAgg_projection(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  //pCI->G_vNewMember(CK_EXPR);

  Card_Expr* p_expr_new = new Card_Expr;

  p_expr_new->Y_vSet(P_Injected);

  p_expr_new->vAppend(p_expr_fold->pSub_type()->pDupCardAndSubCards());

  p_expr_new->vAppend(p_expr_fold->pSub_as()->pDupCardAndSubCards());

  Card_RpnCalc* p_rpnCalc_new = new Card_RpnCalc;
  p_expr_new->vAppend(p_rpnCalc_new);

  Card_Term* p_term_new = new Card_Term("$K1"_Stz);

  p_rpnCalc_new->vAppend(p_term_new);

  pCI->G.p_projection->vPush(p_expr_new);
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
