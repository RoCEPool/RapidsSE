/*============================================================================*/
/* Card_AggElist.cpp -- List of Aggreg Expressions                            */
/*============================================================================*/

#include "Card_AggElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_AggElist,CK_AGGELIST)
__aNewFunction(Card_AggElist,CN_CARD)
__vDeleteFunction(Card_AggElist,CN_CARD)

__bIsCard(STORE,Card_AggElist,CK_AGGELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AggElist::Card_AggElist(void)
: Card(CK_AGGELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AggElist::~Card_AggElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
