/*============================================================================*/
/* Card_CONDITION.cpp -- Abstract Condition                                        */
/*============================================================================*/

#include "Card_CONDITION.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CONDITION::Card_CONDITION(eCK eCard, i8 iFixedDepth)
: Card_OP(eCard,iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CONDITION::~Card_CONDITION(void) noexcept(false)
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
void Card_CONDITION::vAddTransients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambda = [pCI] (Card_Term* p_term) {

    if (!p_term->bIsColSym())
      return C_CONTINUE_RECURSION;

    if (pCI->Early.m_pNeeded_later->pFind_expr_Via_term0(p_term->pSymbol()))
      return C_CONTINUE_RECURSION;

    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(p_term)) {
      //p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      pCI->Early.m_pNeeded_later->vAppendChild(p_expr_clone);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pSub_boolPred()->bApplyRecursive_term(bLambda);
}

/*----------------------------------------------------------------------------*/
/* convert all stmCol to tblCol                                               */
/*----------------------------------------------------------------------------*/
void Card_CONDITION::vX_stmCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_boolPred = pSub_boolPred()))
    pCI->G.p_boolPred->vX_stmCol_To_tblCol(pCI);
  else if ((pCI->G.p_rpnCalc = pSub_rpnCalc()))
    pCI->G.p_rpnCalc->vX_stmCol_To_tblCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* convert all tblCol to stmCol                                               */
/*----------------------------------------------------------------------------*/
void Card_CONDITION::vX_tblCol_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_boolPred = pSub_boolPred()))
    pCI->G.p_boolPred->vX_tblCol_To_stmCol(pCI);
  else if ((pCI->G.p_rpnCalc = pSub_rpnCalc()))
    pCI->G.p_rpnCalc->vX_tblCol_To_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_CONDITION::vShortForm(String* p_string)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolPred* p_boolCalc = pSub_boolPred())
    p_boolCalc->vShortForm(p_string);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
