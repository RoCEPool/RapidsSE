/*============================================================================*/
/* Card_OrdAgg.cpp -- Ordered Aggregate                                       */
/*============================================================================*/

#include "Card_OrdAgg.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_OrdAgg, CK_o_ORDAGG)
__aNewFunction(Card_OrdAgg,CN_CARD)
__vDeleteFunction(Card_OrdAgg,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrdAgg::Card_OrdAgg(void)
: Card_AGGREGATE(CK_o_ORDAGG, 1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OrdAgg::~Card_OrdAgg(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vDeleteAll();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_OrdAgg::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(pCI);
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_OrdAgg::vEarlyProject_root(CardIter* pCI)
{
  vDump(__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->Early.m_pNeeded_later);

  if (pCI->pPrevChild()->bIsCard(CK_o_PROJECT))
    return;

  pCI->G.p_key = pSub_key();
  pCI->G.p_emit = pSub_emit();

  // get the root needed form of ordAgg's projection
  pCI->Early.m_pOP_root = this;
  pCI->Early.m_pOP_leaf = nullptr;
  pCI->Early.m_pNeeded_later  = pCI->G.p_emit->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this ordAgg
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->G_vInsertBefore(p_project_early, this);
  pCI->G_vMainResynch();

 // pCI->vRenumberCards();
 // pCI->vPrintBoard();


  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  vCorrect_expCol(pCI->Flux.m_pArriving);

 // pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();

 // pCI->vPrintBoard();

  pCI->G.p_emit = pSub_emit();

  pCI->Flux.m_pArriving = pCI->G.p_emit;

 // pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OrdAgg::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_s_LEVEL, CK_s_STREAM, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
