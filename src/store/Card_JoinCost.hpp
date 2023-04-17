/*============================================================================*/
/* Card_JoinCost.hpp -- Estimate cost for a JoinNext of a Query               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinCost : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinCost(void);
   ~Card_JoinCost(void) noexcept(false);
  };

  Card_JoinCost* _pCard_JoinCost(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
