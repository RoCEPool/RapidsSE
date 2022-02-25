/*============================================================================*/
/* Card_SortDirn.cpp -- Sort Direction                                        */
/*============================================================================*/

#include "Card_SortDirn.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_SortDirn,CK_GBYELIST)
__aNewFunction(Card_SortDirn,CN_CARD)
__vDeleteFunction(Card_SortDirn,CN_CARD)

__bIsCard(STORE,Card_SortDirn,CK_GBYELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_SortDirn::Card_SortDirn(void)
: Card(CK_GBYELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_SortDirn::~Card_SortDirn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
