/*============================================================================*/
/* Card_GbyElist.cpp -- List of Group By Expressions                          */
/*============================================================================*/

#include "Card_GbyElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_GbyElist,CK_GBYELIST)
__aNewFunction(Card_GbyElist,CN_CARD)
__vDeleteFunction(Card_GbyElist,CN_CARD)

__bIsCard(STORE,Card_GbyElist,CK_GBYELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_GbyElist::Card_GbyElist(void)
: Card(CK_GBYELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_GbyElist::~Card_GbyElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
