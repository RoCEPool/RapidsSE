/*============================================================================*/
/* Card_Av_ScanOnly.cpp -- Aggregate View Scan Only                           */
/*============================================================================*/

#include "Card_Av_ScanOnly.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Av_ScanOnly, CK_AV_SCANONLY)
__aNewFunction(Card_Av_ScanOnly, CN_CARD)
__vDeleteFunction(Card_Av_ScanOnly, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Av_ScanOnly::Card_Av_ScanOnly(void)
    : Card(CK_AV_SCANONLY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Av_ScanOnly::~Card_Av_ScanOnly(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
