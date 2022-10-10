/*============================================================================*/
/* Card_CoPtnPred.cpp -- Permutation                                        */
/*============================================================================*/

#include "Card_CoPtnPred.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CoPtnPred,CK_COPTNPRED)
__aNewFunction(Card_CoPtnPred,CN_CARD)
__vDeleteFunction(Card_CoPtnPred,CN_CARD)

__bIsCard(STORE,Card_CoPtnPred,CK_COPTNPRED)
__pSubFunction_Card(STORE,CoPtnPred,CK_COPTNPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CoPtnPred::Card_CoPtnPred(void)
: Card(CK_COPTNPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CoPtnPred::~Card_CoPtnPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
