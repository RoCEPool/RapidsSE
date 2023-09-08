/*============================================================================*/
/* Card_COND.cpp -- Abstract Condition                                        */
/*============================================================================*/

#include "Card_COND.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_COND::Card_COND(eCK eCard, i8 iFixedDepth)
: Card_OP(eCard,iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_COND::~Card_COND(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add transients to needed                                                   */
/*----------------------------------------------------------------------------*/
void Card_COND::vAddTransients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambda = [pCI] (Card_Term* p_term) {

    if (!p_term->bIsValOdn())
      return C_CONTINUE_RECURSION;

    if (pCI->Early.m_pNeeded_later->pFind_expr_Of_symbol(p_term->pSymbol()))
      return C_CONTINUE_RECURSION;

    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(p_term)) {
      //p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      pCI->Early.m_pNeeded_later->vAppend(p_expr_clone);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pSub_boolCalc()->bApplyRecursive_term(bLambda);
}

/*----------------------------------------------------------------------------*/
/* convert all colOdn to stmOdn                                               */
/*----------------------------------------------------------------------------*/
void Card_COND::vConvert_colOdn_To_stmOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_boolCalc = pSub_boolCalc()))
    pCI->G.p_boolCalc->vConvert_colOdn_To_stmOdn(pCI);
  else if ((pCI->G.p_rpnCalc = pSub_rpnCalc()))
    pCI->G.p_rpnCalc->vConvert_colOdn_To_stmOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* convert all stmOdn to colOdn                                               */
/*----------------------------------------------------------------------------*/
void Card_COND::vConvert_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_boolCalc = pSub_boolCalc()))
    pCI->G.p_boolCalc->vConvert_stmOdn_To_colOdn(pCI);
  else if ((pCI->G.p_rpnCalc = pSub_rpnCalc()))
    pCI->G.p_rpnCalc->vConvert_stmOdn_To_colOdn(pCI);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
