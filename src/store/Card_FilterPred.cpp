/*============================================================================*/
/* Card_FilterPred.cpp -- Filter Predicate                                    */
/*============================================================================*/

#include "Card_FilterPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_FilterPred,CK_FILTERPRED)
__aNewFunction(Card_FilterPred,CN_CARD)
__vDeleteFunction(Card_FilterPred,CN_CARD)

__bIsCard(STORE,Card_FilterPred,CK_FILTERPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_FilterPred::Card_FilterPred(void)
: Card_PRED(CK_FILTERPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_FilterPred::~Card_FilterPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
