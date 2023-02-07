/*============================================================================*/
/* Card_GbyAgg.cpp -- Aggreg-GroupBy                                          */
/*============================================================================*/

#include "Card_GbyAgg.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_GbyAgg, CK_GBYAGG)
__aNewFunction(Card_GbyAgg, CN_CARD)
__vDeleteFunction(Card_GbyAgg, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_GbyAgg::Card_GbyAgg(void)
: Card_Project(CK_GBYAGG)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_GbyAgg::~Card_GbyAgg(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* bump key & projection of this gbyAgg rightwards by given amount            */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_Key* p_key = pSub_key())
    p_key->vBump_Rightwards(nBumpBy);

  if (Card_Projection* p_projection = pSub_projection())
    p_projection->vBump_Rightwards(nBumpBy);
}


/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* convert terms which are stmCol to tblCol                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_projection = pSub_projection()))
    pCI->G.p_projection->vConvert_stmColOdn_terms_To_tblColOdn(pCI);

  if ((pCI->G.p_key = pSub_key()))
    pCI->G.p_key->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* inject sub-query result into this gbyAgg                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vInject_sqResult(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_key = pCI->G.p_gbyAgg->pSub_key();
  pCI->G.p_key->vBump_stmOdn_In_rpnCalc(pCI);

  pCI->G.p_projection = pCI->G.p_gbyAgg->pSub_projection();
  pCI->G.p_projection->vBump_keyOdn_In_rpnCalc(pCI);
  pCI->G.p_projection->vBump_stmOdn_In_rpnCalc(pCI);

  Card_FoldElist::vPush_foldExpr_Into_gbyAgg_projection(pCI,p_expr_fold);
  Card_FoldElist::vPush_foldExpr_Into_gbyAgg_key(pCI,p_expr_fold);

  pCI->G.p_key->vRenumberAllExpr();
  pCI->G.p_projection->vRenumberAllExpr();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
