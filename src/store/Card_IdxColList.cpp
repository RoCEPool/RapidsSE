/*============================================================================*/
/* Card_IdxColList.cpp -- Index Column List                                   */
/*============================================================================*/

#include "Card_IdxColList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IdxColList,CK_IDXCOLLIST)
__aNewFunction(Card_IdxColList,CN_CARD)
__vDeleteFunction(Card_IdxColList,CN_CARD)

__bIsCard(STORE,Card_IdxColList,CK_IDXCOLLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IdxColList::Card_IdxColList(void)
: Card_SUB(CK_IDXCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IdxColList::~Card_IdxColList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
