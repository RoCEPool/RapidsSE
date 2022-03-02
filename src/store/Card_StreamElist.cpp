/*============================================================================*/
/* Card_StreamElist.cpp -- List of Stream Expressions                         */
/*============================================================================*/

#include "Card_StreamElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamElist,CK_STREAMELIST)
__aNewFunction(Card_StreamElist,CN_CARD)
__vDeleteFunction(Card_StreamElist,CN_CARD)

__bIsCard(STORE,Card_StreamElist,CK_STREAMELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamElist::Card_StreamElist(void)
: Card_ELIST(CK_STREAMELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamElist::Card_StreamElist(StreamDef* pStreamDef)
: Card_ELIST(CK_STREAMELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamElist::~Card_StreamElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
