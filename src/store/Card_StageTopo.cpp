/*============================================================================*/
/* Card_StageTopo.cpp -- Stage Topology                                       */
/*============================================================================*/

#include "Card_StageTopo.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StageTopo,CK_STAGETOPO)
__aNewFunction(Card_StageTopo,CN_CARD)
__vDeleteFunction(Card_StageTopo,CN_CARD)

__bIsCard(STORE,Card_StageTopo,CK_STAGETOPO)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StageTopo::Card_StageTopo(eYG eStageTopo)
: Card_SUB(CK_STAGETOPO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vStageTopo(eStageTopo);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StageTopo::~Card_StageTopo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
