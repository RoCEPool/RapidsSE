/*============================================================================*/
/* Card_ObyElist.cpp -- List of Order By Expressions                          */
/*============================================================================*/

#include "Card_SbyElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_SbyElist,CK_SBYELIST)
__aNewFunction(Card_SbyElist,CN_CARD)
__vDeleteFunction(Card_SbyElist,CN_CARD)

__bIsCard(STORE,Card_SbyElist,CK_SBYELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_SbyElist::Card_SbyElist(void)
: Card_ELIST(CK_SBYELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_SbyElist::~Card_SbyElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
