/*============================================================================*/
/* Card_OrdAgg.cpp -- Ordered Aggregate                                       */
/*============================================================================*/

#include "Card_OrdAgg.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_OrdAgg,CK_ORDAGG)
__aNewFunction(Card_OrdAgg,CN_CARD)
__vDeleteFunction(Card_OrdAgg,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrdAgg::Card_OrdAgg(void)
: Card_Project(CK_ORDAGG)
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
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_OrdAgg::vEarlyProject_root(CardIter* pCI)
{
  vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->Early.m_pNeeded_later);

  if (pCI->pPrevChild()->bIsCard(CK_PROJECT))
    return;

  pSymbol()->Y_vSet(P_BreakPt);

  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  // get the root needed form of ordAgg's projection
  pCI->Early.m_pOP_root = this;
  pCI->Early.m_pOP_leaf = nullptr;
  pCI->Early.m_pNeeded_later  = pCI->G.p_projection->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this ordAgg
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->G_vInsertBefore(p_project_early, this);
  pCI->G_vMainResynch();

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard();


  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();

  pCI->vPrintBoard();

  pCI->G.p_projection = pSub_projection();

  pCI->Analysis.m_pProjection_prior = pCI->G.p_projection;

  pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OrdAgg::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key())) {
    if (pCI->Analysis.m_pProjection_prior) {
      pCI->G.p_key->vTypes_For_stmColOdn(pCI);
      pCI->G.p_key->vRenumberAll_expr();
    }
  }

  if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection())) {

    if (pCI->Analysis.m_pProjection_prior) {
      pCI->G.p_projection->vEnsureTypes_For_stmColOdn(pCI);
      pCI->G.p_projection->vRenumberAll_expr();
    }

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OrdAgg::vValidate(i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
