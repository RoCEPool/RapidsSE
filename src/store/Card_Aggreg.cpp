/*============================================================================*/
/* Card_Aggreg.cpp -- Aggreg                                                  */
/*============================================================================*/

#include "Card_Aggreg.hpp"

#include "Card_Emit.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Aggreg, CK_o_AGGREG)
__aNewFunction(Card_Aggreg,CN_CARD)
__vDeleteFunction(Card_Aggreg,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Aggreg::Card_Aggreg(void)
: Card_AGGREGATE(CK_o_AGGREG, 1)
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
  pCI->E.p_emit = pSub_emit();
  pCI->Early.m_pNeeded_later  = pCI->E.p_emit->pCraftRoot_needed(pCI);

  if (0 == pCI->Early.m_pNeeded_later->nChildCards()) {
    DELETE_CARD(pCI->Early.m_pNeeded_later);
    pCI->Early.m_pOP_leaf = this;
    return;
  }

  // insert empty early projection (has only needed) before this ordAgg
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->vInsertBefore(p_project_early, this);
  pCI->vResynchOp();

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  pCI->E.p_aggreg->vRestate_emit(pCI);

 // pCI->PRINTBOARD_dbg();
}

/*----------------------------------------------------------------------------*/
/* craft needed                                                               */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vCraft_needed_root(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->Early.m_pNeeded_later = pCI->E.p_emit->pCraft_needed_root(pCI);
  DELETE_CARD(pCI->Early.m_pNeeded_later);
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->E.p_aggreg->pSymbol()->Y_vSet(P_Smoothed);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Aggreg::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  vValidate_mustHave(CK_s_LEVEL, CK_t_EMIT, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
