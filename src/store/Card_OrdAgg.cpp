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
void Card_OrdAgg::vEarly_project_root(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->EarlyProject.m_pProjection_needed);

  pCI->EarlyProject.m_pOP_root = (Card_OP*)this;

  pCI->G.p_key = pSub_key();

  // get the reduced form of ordAgg's projection
  pCI->EarlyProject.m_pProjection_needed =
      pSub_projection()->pDeduceProjectionNeeded(pCI);

  pCI->vSave();
  Card_OP::vEarly_project_test(pCI);
  pCI->vSave();

  // todo: arrived back ...
  pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();
  pCI->G.p_key->vFlipTerms(pCI->EarlyProject.m_pProjection_needed);

  pCI->vPrintBoard();

  pCI->G.p_projection = pSub_projection();
  pCI->G.p_projection->vFlipTerms(pCI->EarlyProject.m_pProjection_needed);

  pCI->ProjectionAnalysis.m_pProjection_running = pCI->G.p_projection;

  pCI->vPrintBoard();

  vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
