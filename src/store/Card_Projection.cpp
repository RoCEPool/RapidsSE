/*============================================================================*/
/* Card_Projection.cpp -- Project Card                                           */
/*============================================================================*/

#include "Card_Projection.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Projection,CK_PROJECTION)
__aNewFunction(Card_Projection,CN_CARD)
__vDeleteFunction(Card_Projection,CN_CARD)

__bIsCard(STORE,Card_Projection,CK_PROJECTION)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Projection::Card_Projection(eCK eKind)
: Card(CK_PROJECTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
        
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Projection::~Card_Projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
