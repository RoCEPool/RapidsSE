/*============================================================================*/
/* Card_ProjElist.cpp -- List of Project Expressions                          */
/*============================================================================*/

#include "Card_ProjElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_ProjElist,CK_PROJELIST)
__aNewFunction(Card_ProjElist,CN_CARD)
__vDeleteFunction(Card_ProjElist,CN_CARD)

__bIsCard(STORE,Card_ProjElist,CK_PROJELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ProjElist::Card_ProjElist(void)
: Card_ELIST(CK_PROJELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ProjElist::~Card_ProjElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
