/*============================================================================*/
/* Card_StmCol.cpp -- Stream Column                                           */
/*============================================================================*/

#include "Card_StmCol.hpp"
#include "Card_As.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_StmCol, CK_STMCOL)
__aNewFunction(Card_StmCol, CN_CARD)
__vDeleteFunction(Card_StmCol, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StmCol::Card_StmCol(void)
: Card(CK_STMCOL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StmCol::~Card_StmCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_StmCol::vFind_Correlation_Expr_Of_Correlation_Column(CardIter* pCI)
{
  _static_

  // find the <table>/<stream>/<stmColList>/<stmCol>
  //  corresponding to the correlated predicate

  pCI->m_pCard_found = pCI->G.p_projection->pFindValueRecursive(pCI->m_pTerm_corrCol->szValue(), pCI->m_pCard_parent);
  pCI->m_pExpr_corr = _pCard_Expr(pCI->m_pCard_parent);

  pCI->m_pExpr_corr->vMark1(MARK_AT);
  pCI->m_pExpr_corr->Y_vSet(P_CrlKeyExpr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
