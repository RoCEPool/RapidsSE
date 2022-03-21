/*============================================================================*/
/* Card_IndexJoin.cpp -- Index Join                                           */
/*============================================================================*/

#include "Card_IndexJoin.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexJoin,CK_INDEXJOIN)
__aNewFunction(Card_IndexJoin,CN_CARD)
__vDeleteFunction(Card_IndexJoin,CN_CARD)

__bIsCard(STORE,Card_IndexJoin,CK_INDEXJOIN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::Card_IndexJoin(void)
: Card_MAIN(CK_INDEXJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::~Card_IndexJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
