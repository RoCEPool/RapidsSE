/*============================================================================*/
/* Card_AggLevel.cpp -- Aggregate Level                                       */
/*============================================================================*/

#include "Card_AggLevel.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_AggLevel,CK_AGGLEVEL)
__aNewFunction(Card_AggLevel,CN_CARD)
__vDeleteFunction(Card_AggLevel,CN_CARD)

__bIsCard(STORE,Card_AggLevel,CK_AGGLEVEL)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AggLevel::Card_AggLevel(eAL eAggLevel)
: Card(CK_AGGLEVEL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Y_vSet(P_AggLevel, (i8)eAggLevel);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AggLevel::~Card_AggLevel(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
