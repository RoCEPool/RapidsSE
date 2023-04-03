/*============================================================================*/
/* Card_CostNext.hpp -- Estimate cost for a JoinNext of a Query               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CostNext : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostNext(void);
   ~Card_CostNext(void) noexcept(false);
  };

  Card_CostNext* _pCard_CostNext(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
