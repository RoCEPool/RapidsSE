/*============================================================================*/
/* Card_AggLevel.hpp -- Aggregate Level                                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_AggLevel : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AggLevel(eAL eAggLevel);
   ~Card_AggLevel(void) noexcept(false);
  };

  Card_AggLevel* _pCard_AggLevel(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
