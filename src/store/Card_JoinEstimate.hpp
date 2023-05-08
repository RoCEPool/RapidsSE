/*============================================================================*/
/* Card_JoinEstimate.hpp -- Estimate cost for a JoinNext of a Query               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinEstimate : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinEstimate(void);
   ~Card_JoinEstimate(void) noexcept(false);
  };

  Card_JoinEstimate* _pCard_JoinEstimate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
