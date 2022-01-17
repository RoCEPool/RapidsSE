/*============================================================================*/
/* Card_Index.cpp -- Index Card                                               */
/*============================================================================*/

#include "Card_Index.hpp"
#include "ShardDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Index,CK_INDEX)
__aNewFunction(Card_Index,CN_CARD_INDEX)
__vDeleteFunction(Card_Index,CN_CARD_INDEX)

__bIsClass(STORE,Card_Index,CN_CARD_INDEX)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Index::Card_Index(eCK eKind, Index* pIndex)
: Card(CK_INDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
   vIndex(pIndex);
  
  _checkInitialised_(CN_CARD_INDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Index::~Card_Index(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
