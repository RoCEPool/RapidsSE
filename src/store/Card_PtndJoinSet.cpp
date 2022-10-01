/*============================================================================*/
/* Card_JoinPtnList.cpp -- Cross-Partition List                              */
/*============================================================================*/

#include "Card_PtndJoinSet.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_PtndJoinSet,CK_PTNDJOINSET)
__aNewFunction(Card_PtndJoinSet,CN_CARD)
__vDeleteFunction(Card_PtndJoinSet,CN_CARD)

__bIsCard(STORE,Card_PtndJoinSet,CK_PTNDJOINSET)
__pGetSubFunction_Card(STORE,PtndJoinSet,CK_PTNDJOINSET)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PtndJoinSet::Card_PtndJoinSet(void)
: Card(CK_PTNDJOINSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PtndJoinSet::~Card_PtndJoinSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
