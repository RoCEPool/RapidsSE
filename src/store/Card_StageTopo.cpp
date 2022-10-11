/*============================================================================*/
/* Card_StageTopo.cpp -- Stage Topology                                       */
/*============================================================================*/

#include "Card_StageTopo.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StageTopo,CK_STAGETOPO)
__aNewFunction(Card_StageTopo,CN_CARD)
__vDeleteFunction(Card_StageTopo,CN_CARD)

__bIsCard(STORE,Card_StageTopo,CK_STAGETOPO)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StageTopo::Card_StageTopo(eYG eStageTopo)
: Card(CK_STAGETOPO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vStageTopo(eStageTopo);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StageTopo::~Card_StageTopo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stage topology                                                         */
/*----------------------------------------------------------------------------*/
eYG Card_StageTopo::eStageTopo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eYG eStageTopo = (eYG)iPpy(PPY_StageTopo);

  return eStageTopo;
}

/*----------------------------------------------------------------------------*/
/* set stage topology                                                         */
/*----------------------------------------------------------------------------*/
void Card_StageTopo::vStageTopo(eYG  eStageTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAddProperty_int(PPY_StageTopo,eStageTopo);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
