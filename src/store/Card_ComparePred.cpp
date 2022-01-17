/*============================================================================*/
/* Card_ComparePred.cpp -- Compare Predicate                                  */
/*============================================================================*/

#include "Card_ComparePred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_ComparePred,CK_COMPAREPRED)
__aNewFunction(Card_ComparePred,CN_CARD)
__vDeleteFunction(Card_ComparePred,CN_CARD)

__bIsCard(STORE,Card_ComparePred,CK_COMPAREPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ComparePred::Card_ComparePred(eCK eKind)
: Card_Pred(CK_COMPAREPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ComparePred::~Card_ComparePred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
