/*============================================================================*/
/* Card_IndexJoin.cpp -- Index Join Card                                     */
/*============================================================================*/

#include "Card_IndexJoin.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexJoin,CK_INDEXJOIN)
__aNewFunction(Card_IndexJoin,CN_CARD_INDEXJOIN)
__vDeleteFunction(Card_IndexJoin,CN_CARD_INDEXJOIN)

__bIsClass(STORE,Card_IndexJoin,CN_CARD_INDEXJOIN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::Card_IndexJoin(eCK eKind, Index* pIndex)
: Card_Unit(CK_INDEXJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
        
  vIndex(pIndex);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::~Card_IndexJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
