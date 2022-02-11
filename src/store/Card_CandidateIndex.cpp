/*============================================================================*/
/* Card_CandidateIndex.cpp -- Candidate Index                                 */
/*============================================================================*/

#include "Card_CandidateIndex.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CandidateIndex,CK_CANDIDATEINDEX)
__aNewFunction(Card_CandidateIndex,CN_CARD)
__vDeleteFunction(Card_CandidateIndex,CN_CARD)

__bIsCard(STORE,Card_CandidateIndex,CK_CANDIDATEINDEX)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CandidateIndex::Card_CandidateIndex(Index* pIndex)
: Card(CK_CANDIDATEINDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

   vIndex(pIndex);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CandidateIndex::~Card_CandidateIndex(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
