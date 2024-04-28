/*============================================================================*/
/* Card_Having.cpp -- Having                                                  */
/*============================================================================*/

#include "Card_Having.hpp"

#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Having, CK_o_HAVING)
__aNewFunction(Card_Having,CN_CARD)
__vDeleteFunction(Card_Having,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Having::Card_Having(void)
: Card_CONDITION(CK_o_HAVING, 1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Having::~Card_Having(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Having::vSmooth(CardIter* pCI, bool bSawProject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->E.p_having->pSymbol()->Y_vSet(P_Smoothed);

  bool bBump = ! bSawProject;

  vCorrect_After_Inject(pCI, bBump);
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Having::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->E.p_having->pSymbol()->Y_vSet(P_Smoothed);

  bool bBump = ! pCI->Carry.Project_postAgg.m_bSeen;

  if (pCI->Carry.Having.m_bCorrect_After_Inject)
    vCorrect_After_Inject(pCI, bBump);

  pCI->vSnapIf_arriving();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vCorrect_After_Inject(CardIter* pCI, bool bBump)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->E.p_predicate = pSub_predicate();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_cmp = [](Card_Term *p_term)
  {

    if (p_term->pSymbol()->bHasFacet(SF_EXP_COL))
      p_term->vBump_ordCol(SF_EXP_COL);

    return true;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bBump)
    pCI->E.p_predicate->bLambdaWalk_term(bLambdaFn_cmp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_ro = [pCI](Card *pCard)
  {
    if (! pCard->bMark1(MARK_AT))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term_1 = _pCard_Term(pCard->pGet(1));
    pCard->vRemoveChild(p_term_1);
    Symbol x_Symbol_expCol1(SF_EXP_COL, 1);
    pCI->vAppendNewChild(pCard, CK_TERM, Symbol(SF_EXP_COL, "$1"_Stz));

    Card_Expr* p_expr_found =
        pCI->Dic.m_p_expr->pGet_expr((i8) 0);

    RASSERT(CT_NONE != p_expr_found->eCTVal());

    pCI->E.p_term->vApplyCast_x(p_expr_found->xCast());

    DELETE_CARD(p_term_1);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->E.p_predicate->bLambdaWalk(CK_RLNCONJ, bLambdaFn_ro);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_child_mustBe(CK_c_LEVEL, CK_NONE);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
