/*============================================================================*/
/* MarkerChain.cpp -- Chain of Free Markers                                   */
/*============================================================================*/

#include "BASE.h"

#include "MarkerChain.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,MarkerChain,CN_MARKERCHAIN)
__aNewFunction(MarkerChain,CN_MARKERCHAIN)
__vDeleteFunction(MarkerChain,CN_MARKERCHAIN)
__aFooterFunction(MarkerChain)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MarkerChain::MarkerChain(void)
: Chain_t(CN_MARKERCHAIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKERCHAIN,_FOOTER_);
  
  _checkInitialised_(CN_MARKERCHAIN);
  
  for (i8 i_Marker = 0; i_Marker < 128; i_Marker++) {
    Marker* pMarker = new (MG_CHAIN)Marker(OM_NONE);
    pMarker->vMarker(MG_CHAIN);
    vPush(pMarker);
  }
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_MARKERCHAIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MarkerChain::~MarkerChain(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKERCHAIN,_FOOTER_);
  
  for (Marker* pMarker = pPop(); pMarker; pMarker = pPop())
    delete pMarker;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
