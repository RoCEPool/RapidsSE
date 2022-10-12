/*============================================================================*/
/* Card_CoPtnPredList.cpp -- Co-Partition Predicate List                      */
/*============================================================================*/

#include "Card_CoPtnPredList.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CoPtnPredList,CK_COPTNPREDLIST)
__aNewFunction(Card_CoPtnPredList,CN_CARD)
__vDeleteFunction(Card_CoPtnPredList,CN_CARD)

__bIsCard(STORE,Card_CoPtnPredList,CK_COPTNPREDLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CoPtnPredList::Card_CoPtnPredList(void)
: Card(CK_COPTNPREDLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CoPtnPredList::~Card_CoPtnPredList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
