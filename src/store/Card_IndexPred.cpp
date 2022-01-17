/*============================================================================*/
/* Card_IndexPred.cpp -- Index Predicate                                      */
/*============================================================================*/

#include "Card_IndexPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexPred,CK_INDEXPRED)
__aNewFunction(Card_IndexPred,CN_CARD)
__vDeleteFunction(Card_IndexPred,CN_CARD)

__bIsCard(STORE,Card_IndexPred,CK_INDEXPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexPred::Card_IndexPred(eCK eKind)
: Card_Pred(CK_INDEXPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexPred::~Card_IndexPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
