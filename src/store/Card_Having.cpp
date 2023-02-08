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

  Board::vCorrect_having_After_Inject(pCI,bBump);
  pCI->G.pDeck->vRenumberRecursive();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
