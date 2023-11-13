/*============================================================================*/
/* Card_Having.cpp -- Having                                                  */
/*============================================================================*/

#include "Card_Having.hpp"

#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Having,CK_HAVING)
__aNewFunction(Card_Having,CN_CARD)
__vDeleteFunction(Card_Having,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Having::Card_Having(void)
: Card_CONDITION(CK_HAVING, 1)
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
/* acclimate for injected fold expr                                           */
/*----------------------------------------------------------------------------*/
void Card_Having::vAcclimate_foldExpr(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_constraint = pSub_constraint();

  pCI->G.p_constraint->vAcclimate_foldExpr(pCI,pCI->Sqy.m_pExpr_fold);
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Having::vSmooth(CardIter* pCI, bool bSawProject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_having->pSymbol()->Y_vSet(P_Smoothed);

  bool bBump = ! bSawProject;

  vCorrect_After_Inject(pCI, bBump);
 // pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Having::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_having->pSymbol()->Y_vSet(P_Smoothed);

  bool bBump = ! pCarryForward->Project_postAgg.m_bSeen;

  if (pCarryForward->Having.m_bCorrect_After_Inject)
    vCorrect_After_Inject(pCI, bBump);

  pCI->G.p_having->vUpdate_emitted(pCI);

  pCI->Flux.m_pArriving = pSub_emit();


 // pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vCorrect_After_Inject(CardIter* pCI, bool bBump)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_boolPred = pSub_boolPred();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_cmp = [](Card_Term *p_term)
  {

    if (p_term->bIs_expCol())
      p_term->vBump_expCol();

    return true;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bBump)
    pCI->G.p_boolPred->bApplyRecursive_term(bLambdaFn_cmp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_ro = [pCI](Card *pCard)
  {
    if (! pCard->bMark1(MARK_AT))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term_1 = _pCard_Term(pCard->pGet(1));
    pCard->vRemoveChild(p_term_1);
    Symbol x_Symbol_expCol1(SF_EXP_COL, 1);
    pCI->G_vAppendNewChild(pCard, CK_TERM, Symbol(SF_EXP_COL, "$1"_Stz));

    Card_Expr* p_expr_found =
        pCI->Flux.m_pArriving->pGet_expr((i8) 0);

    RASSERT(CT_NONE != p_expr_found->xTypeId().eCTVal());

    pCI->G.p_term->pSymbol()->vTypeId(p_expr_found->xTypeId());

    DELETE_CARD(p_term_1);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolPred->bApplyRecursive(CK_RLNCONJ, bLambdaFn_ro);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vUpdate_emitted(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_stream = pCI->G.p_having->pSub_stream())) {
    if ((pCI->G.p_emit = pCI->G.p_stream->pSub_emit())) {

      pCI->G.p_stream->vDeleteChild(pCI->G.p_emit);
      pCI->G.p_stream->vAppendChild(pCI->Flux.m_pArriving->pPropagate());
    }
  }

  pCI->Flux.m_pArriving = pSub_emit();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_LEVEL,CK_NONE);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
