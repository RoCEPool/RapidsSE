/*============================================================================*/
/* Card_Av_GbyPNO.cpp -- Aggregate View Group By Partition                    */
/*============================================================================*/

#include "Card_Av_GbyPNO.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_GbyPNO, CK_AV_GBYPNO)
__aNewFunction(Card_Av_GbyPNO, CN_CARD)
__vDeleteFunction(Card_Av_GbyPNO, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_GbyPNO::Card_Av_GbyPNO(void)
    : Card(CK_AV_GBYPNO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_GbyPNO::~Card_Av_GbyPNO(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
//
// Created by Arjun on 18/11/2022.
//

#include "Card_Av_GbyPNO.hpp"
