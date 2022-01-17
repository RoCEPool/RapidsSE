/*============================================================================*/
/* Card_IndexPred.cpp -- Index Predicate                                      */
/*============================================================================*/

#include "Card_IndexPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexPred,CK_INDEXPRED)
__aNewFunction(Card_IndexPred,CN_CARD_INDEXPRED)
__vDeleteFunction(Card_IndexPred,CN_CARD_INDEXPRED)

__bIsClass(STORE,Card_IndexPred,CN_CARD_INDEXPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexPred::Card_IndexPred(eCK eKind)
: Card_Pred(CK_INDEXPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
  _checkInitialised_(CN_CARD_INDEXPRED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexPred::~Card_IndexPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
