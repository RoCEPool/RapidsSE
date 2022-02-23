/*============================================================================*/
/* Card_FiltElist.cpp -- List of Filter Expressions                           */
/*============================================================================*/

#include "Card_FiltElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_FiltElist,CK_FILTELIST)
__aNewFunction(Card_FiltElist,CN_CARD)
__vDeleteFunction(Card_FiltElist,CN_CARD)

__bIsCard(STORE,Card_FiltElist,CK_FILTELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_FiltElist::Card_FiltElist(void)
: Card(CK_FILTELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_FiltElist::~Card_FiltElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
