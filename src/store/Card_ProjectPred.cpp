/*============================================================================*/
/* Card_ProjectPred.cpp -- Project Predicate                                  */
/*============================================================================*/

#include "Card_ProjectPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_ProjectPred,CK_PROJECTPRED)
__aNewFunction(Card_ProjectPred,CN_CARD_PROJECTPRED)
__vDeleteFunction(Card_ProjectPred,CN_CARD_PROJECTPRED)

__bIsClass(STORE,Card_ProjectPred,CN_CARD_PROJECTPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ProjectPred::Card_ProjectPred(eCK eKind)
: Card_Pred(CK_PROJECTPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ProjectPred::~Card_ProjectPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
