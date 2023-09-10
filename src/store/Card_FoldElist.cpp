/*============================================================================*/
/* Card_FoldElist.cpp -- Fold Typed Expression                                 */
/*============================================================================*/

#include "Card_FoldElist.hpp"

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
: Card_ELIST(CK_FOLDELIST,1)
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

  pCI->G.p_expr->pSymbol()->Y_vSet(P_Injected);

  pCI->G.p_expr->vAppendChild(p_expr_fold->pSub_rpnCalc()->pDupCardRecursive());
  pCI->G.p_expr->vColType(p_expr_fold->pSub_rpnCalc());

  pCI->G.p_key->vPushChild(pCI->G.p_expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_FoldElist::vPush_foldExpr_Into_gbyAgg_projection(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  //pCI->G_vNewMember(CK_EXPR);

  RASSERT(p_expr_fold->szP_Type());

  Card_Expr* p_expr_new = new Card_Expr;

  p_expr_new->pSymbol()->Y_vSet(P_Injected);

  p_expr_new->vAppendChild(p_expr_fold->pSub_as()->pDupCardRecursive());

  Card_RpnCalc* p_rpnCalc_new = new Card_RpnCalc;
  p_expr_new->vAppendChild(p_rpnCalc_new);

  Symbol xSymbol_keyOdn(SF_KEY_COL,1);

  Card_Term* p_term_new = p_expr_fold->pSub_term()->pClone(&xSymbol_keyOdn);
  p_rpnCalc_new->pSymbol()->Y_vColType(p_expr_fold->szP_Type());

  p_rpnCalc_new->vAppendChild(p_term_new);
  p_expr_new->vColType(p_rpnCalc_new);

  pCI->G.p_projection->vPushChild(p_expr_new);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_FoldElist::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
