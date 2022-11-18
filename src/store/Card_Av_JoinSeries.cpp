/*============================================================================*/
/* Card_Av_JoinSeries.cpp -- Aggregate View Join Series                       */
/*============================================================================*/

#include "Card_Av_JoinSeries.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_JoinSeries, CK_AV_JOINSERIES)
__aNewFunction(Card_Av_JoinSeries, CN_CARD)
__vDeleteFunction(Card_Av_JoinSeries, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_JoinSeries::Card_Av_JoinSeries(void)
    : Card(CK_AV_JOINSERIES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_JoinSeries::~Card_Av_JoinSeries(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
