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
void Card_OrdAgg::vEarly_project(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! bIsPartitionLevel())
    return;

  RASSERT(! pCI->EarlyProject.m_pProjection_reduced);

  pCI->EarlyProject.m_pOP_root = (Card_OP*)this;

  // get the carry form of ordAgg's projection
  pCI->EarlyProject.m_pProjection_reduced =
      pSub_projection()->pToCarry_v2(pCI,pSub_key());

  Card_OP::vEarly_project(pCI);

  // todo: arrived back ...
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
