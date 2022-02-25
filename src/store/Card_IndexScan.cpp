/*============================================================================*/
/* Card_IndexScan.cpp -- Index Scan                                           */
/*============================================================================*/

#include "Card_IndexScan.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexScan,CK_INDEXSCAN)
__aNewFunction(Card_IndexScan,CN_CARD)
__vDeleteFunction(Card_IndexScan,CN_CARD)

__bIsCard(STORE,Card_IndexScan,CK_INDEXSCAN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexScan::Card_IndexScan(void)
: Card_UNIT(CK_INDEXSCAN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexScan::~Card_IndexScan(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
