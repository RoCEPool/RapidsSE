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
void Card_FoldElist::vPush_foldExpr_Into_aggElist(CardIter* pCI)
{
  _static_

  pCI->G_vNewMember(CK_EXPR);

  pCI->G.p_expr->vAppend(pCI->G.p_foldElist->pSub_type()->pDupCardAndSubCards());

  pCI->G.p_expr->vAppend(pCI->G.p_foldElist->pSub_as()->pDupCardAndSubCards());

  pCI->G_vAppendNewChild(pCI->G.p_expr,CK_RPNCALC);
  pCI->G_vAppendNewChild(pCI->G.p_rpnCalc,CK_TERM,"$K1"_Stz);

  pCI->G.p_aggElist->vPush(pCI->G.p_expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_FoldElist::vPush_foldExpr_Into_gbyElist(CardIter* pCI)
{
  _static_

  pCI->G_vNewMember(CK_EXPR);

  pCI->G.p_expr->vAppend(pCI->G.p_foldElist->pSub_type()->pDupCardAndSubCards());

  pCI->G.p_expr->vAppend(pCI->G.p_foldElist->pSub_rpnCalc()->pDupCardAndSubCards());

  pCI->G.p_key->vPush(pCI->G.p_expr);
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
