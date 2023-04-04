/*============================================================================*/
/* Card_CostLocus.hpp -- Estimate cost for a JoinNext of a Query               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CostLocus : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostLocus(void);
   ~Card_CostLocus(void) noexcept(false);
  };

  Card_CostLocus* _pCard_CostLocus(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
