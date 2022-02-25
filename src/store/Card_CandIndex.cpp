/*============================================================================*/
/* Card_CandIndex.cpp -- Candidate Index                                 */
/*============================================================================*/

#include "Card_CandIndex.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CandIndex,CK_CANDINDEX)
__aNewFunction(Card_CandIndex,CN_CARD)
__vDeleteFunction(Card_CandIndex,CN_CARD)

__bIsCard(STORE,Card_CandIndex,CK_CANDINDEX)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CandIndex::Card_CandIndex(void)
: Card(CK_CANDINDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CandIndex::~Card_CandIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
