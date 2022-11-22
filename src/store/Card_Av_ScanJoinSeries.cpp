/*============================================================================*/
/* Card_Av_ScanJoinSeries.cpp -- Aggregate View Scan Join Series              */
/*============================================================================*/

#include "Card_Av_ScanJoinSeries.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_ScanJoinSeries, CK_AV_SCANJOINSERIES)
__aNewFunction(Card_Av_ScanJoinSeries, CN_CARD)
__vDeleteFunction(Card_Av_ScanJoinSeries, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_ScanJoinSeries::Card_Av_ScanJoinSeries(void)
    : Card(CK_AV_SCANJOINSERIES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_ScanJoinSeries::~Card_Av_ScanJoinSeries(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
//
// Created by Arjun on 18/11/2022.
//

#include "Card_Av_ScanJoinSeries.hpp"
