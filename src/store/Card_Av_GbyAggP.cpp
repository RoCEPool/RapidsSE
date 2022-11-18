/*============================================================================*/
/* Card_Av_GbyAggP.cpp -- Aggregate View Group By Aggregate Partition         */
/*============================================================================*/

#include "Card_Av_GbyAggP.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_GbyAggP, CK_AV_GBYAGGP)
__aNewFunction(Card_Av_GbyAggP, CN_CARD)
__vDeleteFunction(Card_Av_GbyAggP, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_GbyAggP::Card_Av_GbyAggP(void)
    : Card(CK_AV_GBYAGGP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_GbyAggP::~Card_Av_GbyAggP(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
