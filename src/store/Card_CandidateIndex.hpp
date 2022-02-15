/*============================================================================*/
/* Card_CandidateIndex.hpp -- Candidate Index                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_CandidateIndex : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CandidateIndex(Index* pIndex);
   ~Card_CandidateIndex(void) noexcept(false);
  };

  Card_CandidateIndex* _pCard_CandidateIndex(void* aAddr);
  bool _bIsCard_CandidateIndex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
