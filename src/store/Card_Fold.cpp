/*============================================================================*/
/* Card_Fold.cpp -- old (delineates the two halves during flattening)         */
/*============================================================================*/

#include "Card_Fold.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Fold, CK_c_FOLD)
__aNewFunction(Card_Fold,CN_CARD)
__vDeleteFunction(Card_Fold,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Fold::Card_Fold(void)
: Card_CONSTRUCTION(CK_c_FOLD, 1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Fold::~Card_Fold(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vMorph_to_flat(CardIter* pCI)
{
  _static_

  pCI->vBeginEach();
  while (pCI->G_bEachMain(CK_c_FOLD))
    pCI->G.p_fold->vCard(CK_c_FLAT);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vPush_foldExpr_Into_gbyAgg_key(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  pCI->G.p_expr = new Card_Expr;

  pCI->G.p_expr->pSymbol()->Y_vSet(P_Injected);

  pCI->G.p_expr->vAppendChild(p_expr_fold->pSub_rpnCalc()->pDupCardRecursive());
  pCI->G.p_expr->vCast(p_expr_fold->pSub_rpnCalc());

  pCI->G.p_key->vPushChild(pCI->G.p_expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vPush_foldExpr_Into_gbyAgg_projection(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  //pCI->G_vNewMember(CK_EXPR);

  RASSERT(CT_NONE != p_expr_fold->eCTVal());

  Card_Expr* p_expr_new = new Card_Expr;

  p_expr_new->pSymbol()->vAppendFacet(p_expr_fold->pSymbol()->aFacet(SF_NMD_COL));
  p_expr_new->pSymbol()->Y_vSet(P_Injected);

  Card_RpnCalc* p_rpnCalc_new = new Card_RpnCalc;
  p_expr_new->vAppendChild(p_rpnCalc_new);

  Symbol xSymbol_KeyCol(SF_KEY_COL,1);

  Card_Term* p_term_new = p_expr_fold->pSub_term()->pClone(&xSymbol_KeyCol);
  p_rpnCalc_new->vCast(p_expr_fold->xCast());

  p_rpnCalc_new->vAppendChild(p_term_new);
  p_expr_new->vCast(p_rpnCalc_new);

  pCI->G.p_emit->vPushChild(p_expr_new);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_CONSTRUCTION::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
