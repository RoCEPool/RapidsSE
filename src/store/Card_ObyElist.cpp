/*============================================================================*/
/* Card_ObyElist.cpp -- List of Order By Expressions                          */
/*============================================================================*/

#include "Card_ObyElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_ObyElist,CK_OBYELIST)
__aNewFunction(Card_ObyElist,CN_CARD)
__vDeleteFunction(Card_ObyElist,CN_CARD)

__bIsCard(STORE,Card_ObyElist,CK_OBYELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ObyElist::Card_ObyElist(void)
: Card(CK_OBYELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ObyElist::~Card_ObyElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
