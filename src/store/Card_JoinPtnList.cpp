/*============================================================================*/
/* Card_JoinPtnList.cpp -- Cross-Partition List                              */
/*============================================================================*/

#include "Card_JoinPtnList.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPtnList,CK_JOINPTNLIST)
__aNewFunction(Card_JoinPtnList,CN_CARD)
__vDeleteFunction(Card_JoinPtnList,CN_CARD)

__bIsCard(STORE,Card_JoinPtnList,CK_JOINPTNLIST)
__pGetSubFunction_Card(STORE,JoinPtnList,CK_JOINPTNLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPtnList::Card_JoinPtnList(void)
: Card(CK_JOINPTNLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPtnList::~Card_JoinPtnList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
