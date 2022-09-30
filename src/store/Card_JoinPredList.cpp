/*============================================================================*/
/* Card_JoinPredList.cpp -- Table List                                           */
/*============================================================================*/

#include "Card_JoinPredList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPredList,CK_TABLELIST)
__aNewFunction(Card_JoinPredList,CN_CARD)
__vDeleteFunction(Card_JoinPredList,CN_CARD)

__bIsCard(STORE,Card_JoinPredList,CK_TABLELIST)
__pGetSubFunction_Card(STORE,Card_JoinPredList,CK_TABLELIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPredList::Card_JoinPredList(void)
: Card(CK_TABLELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPredList::~Card_JoinPredList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
