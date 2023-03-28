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
: Card(CK_HAVING)
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

  pCI->G.p_having->Y_vSet(P_Smoothed);

  bool bBump = ! bSawProject;

  vCorrect_After_Inject(pCI, bBump);
  pCI->G.pDeck->vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Having::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_having->Y_vSet(P_Smoothed);

  bool bBump = ! pCarryForward->Project_postAgg.m_bSeen;

  if (pCarryForward->Having.m_bCorrect_After_Inject)
    vCorrect_After_Inject(pCI, bBump);

  pCI->Analysis.m_pProjection = pSub_projection();

  pCI->G.pDeck->vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vCorrect_After_Inject(CardIter* pCI, bool bBump)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_boolCalc = pSub_boolCalc();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_cmp = [](Card_Term *p_term)
  {

    if (p_term->bIsStmColOdn())
      p_term->vBump_StreamColOrdinal();

    return true;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bBump)
    pCI->G.p_boolCalc->bApplyRecursive_term(bLambdaFn_cmp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn_ro = [pCI,bBump](Card *pCard)
  {
    if (MARK_AT != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    Card_Term* p_term_1 = _pCard_Term(pCard->pGet(1));
    pCard->vRemoveChild(p_term_1);
    pCI->G_vAppendNewChild(pCard, CK_TERM, "$1"_Stz);

    Card_Expr* p_expr_found =
        pCI->Analysis.m_pProjection->pGetExpr(0);
    pCI->G.p_term->Y_vSet(P_Type,szCT_char[Card::eTo_eCT(p_expr_found->pSub_type()->szValue())]);

    DELETE_CARD(p_term_1);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc->bApplyRecursive(CK_CMPPRED, bLambdaFn_ro);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
