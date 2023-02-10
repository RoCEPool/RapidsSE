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

  pCI->G.p_constraint->vAcclimate_foldExpr(pCI,pCI->m_pExpr_fold);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Having::vCorrect_After_Inject(CardIter* pCI, bool bBump)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_boolCalc = pSub_boolCalc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,bBump](Card *pCard) {

    if (MARK_AT != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    pCI->G.p_roPred = _pCard_RoPred(pCard);

    if (bBump) {
      pCI->m_pTerm_0 = _pCard_Term(pCI->G.p_roPred->pGet(0));
      pCI->m_pTerm_0->vBump_StreamColOrdinal();
    }

    pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
    pCI->G.p_roPred->vRemoveChild(pCI->m_pTerm_1);
    DELETE_CARD(pCI->m_pTerm_1);
    pCI->G_vAppendNewChild(pCI->G.p_roPred, CK_TERM, "$1"_Stz);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc->bApplyRecursive(CK_ROPRED, bLambdaFn);

}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
