/*============================================================================*/
/* Card_Av_GbyAggPN.cpp -- Aggregate View Group By Aggregate Partition Number */
/*============================================================================*/

#include "Card_Av_GbyAggPN.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_GbyAggPN, CK_AV_GBYAGGPN)
__aNewFunction(Card_Av_GbyAggPN, CN_CARD)
__vDeleteFunction(Card_Av_GbyAggPN, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_GbyAggPN::Card_Av_GbyAggPN(void)
    : Card(CK_AV_GBYAGGPN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_GbyAggPN::~Card_Av_GbyAggPN(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
