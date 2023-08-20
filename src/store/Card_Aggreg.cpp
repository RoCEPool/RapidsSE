/*============================================================================*/
/* Card_Aggreg.cpp -- Aggreg                                                  */
/*============================================================================*/

#include "Card_Aggreg.hpp"

#include "Card_Projection.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Aggreg,CK_AGGREG)
__aNewFunction(Card_Aggreg,CN_CARD)
__vDeleteFunction(Card_Aggreg,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Aggreg::Card_Aggreg(void)
: Card_AGG(CK_AGGREG,1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Aggreg::~Card_Aggreg(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_Projection* p_projection = pSub_projection())
    p_projection->vBump_Rightwards(nBumpBy);
}

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(pCI);
}

/*----------------------------------------------------------------------------*/
/* convert terms which are stmCol to tblCol                                   */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_projection = pSub_projection()))
    pCI->G.p_projection->vConvertAll_stmOdn_To_colOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vDerive_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(pCI);

  // creates a new projection key

}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vEarlyProject_root(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->Early.m_pNeeded_later);

  // get the root needed form of ordAgg's projection
  pCI->Early.m_pOP_root = (Card_OP*)this;
  pCI->G.p_projection = pSub_projection();
  pCI->Early.m_pNeeded_later  = pCI->G.p_projection->pCraftRoot_needed(pCI);

  if (0 == pCI->Early.m_pNeeded_later->nChildCards()) {
    DELETE_CARD(pCI->Early.m_pNeeded_later);
    pCI->Early.m_pOP_leaf = this;
    return;
  }

  // insert empty early projection (has only needed) before this ordAgg
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->G_vInsertBefore(p_project_early, this);
  pCI->G_vMainResynch();

  pCI->vRenumberCards();
  pCI->vPrintBoard();


  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  vCorrectStmOdn(pCI->Prior.m_pProjection);

  pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();

  pCI->vPrintBoard();

  pCI->G.p_projection = pSub_projection();

  pCI->Prior.m_pProjection = pCI->G.p_projection;

  pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_aggreg->pSymbol()->Y_vSet(P_Smoothed);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection())) {

    if (pCI->Prior.m_pProjection) {
      pCI->G.p_projection->vEnsureTypes_For_stmColOdn(pCI);
      pCI->G.p_projection->vRenumberAll_expr();
    }

  }

  vValidationPassed();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_LEVEL,CK_PROJECTION,CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
