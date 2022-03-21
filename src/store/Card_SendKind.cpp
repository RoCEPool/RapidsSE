/*============================================================================*/
/* Card_SendKind.cpp -- Kind                                                      */
/*============================================================================*/

#include "Card_SendKind.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_SendKind,CK_SENDKIND)
__aNewFunction(Card_SendKind,CN_CARD)
__vDeleteFunction(Card_SendKind,CN_CARD)

__bIsCard(STORE,Card_SendKind,CK_SENDKIND)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_SendKind::Card_SendKind(eXK eSendKind)
: Card_SUB(CK_SENDKIND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vSendKind(eSendKind);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_SendKind::~Card_SendKind(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
