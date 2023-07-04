/*============================================================================*/
/* Card_Level.hpp -- Aggregate Level                                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Level : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Level(eAL eAggLevel);
   ~Card_Level(void) noexcept(false);
  };

  Card_Level* _pCard_Level(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
