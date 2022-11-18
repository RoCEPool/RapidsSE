/*============================================================================*/
/* Card_Av_AggregPNO.cpp -- Aggregate View Aggregate Partition                */
/*============================================================================*/

#include "Card_Av_AggregPNO.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_AggregPNO, CK_AV_AGGREGPNO)
__aNewFunction(Card_Av_AggregPNO, CN_CARD)
__vDeleteFunction(Card_Av_AggregPNO, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_AggregPNO::Card_Av_AggregPNO(void)
    : Card(CK_AV_AGGREGPNO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_AggregPNO::~Card_Av_AggregPNO(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
