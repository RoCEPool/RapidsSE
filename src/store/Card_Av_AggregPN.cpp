/*============================================================================*/
/* Card_Av_AggregPN.cpp -- Aggregate View Aggregate Partition                 */
/*============================================================================*/

#include "Card_Av_AggregPN.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_AggregPN, CK_AV_AGGREGPN)
__aNewFunction(Card_Av_AggregPN, CN_CARD)
__vDeleteFunction(Card_Av_AggregPN, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_AggregPN::Card_Av_AggregPN(void)
    : Card(CK_AV_AGGREGPN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_AggregPN::~Card_Av_AggregPN(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
