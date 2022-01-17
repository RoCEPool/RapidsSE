/*============================================================================*/
/* Card_OriginCast.cpp -- Origin Cast                                         */
/*============================================================================*/

#include "Card_OriginCast.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_OriginCast,CK_ORIGINCAST)
__aNewFunction(Card_OriginCast,CN_CARD_ORIGINCAST)
__vDeleteFunction(Card_OriginCast,CN_CARD_ORIGINCAST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OriginCast::Card_OriginCast(eCK eKind)
: Card(CK_ORIGINCAST)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OriginCast::~Card_OriginCast(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
