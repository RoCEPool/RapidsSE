/*============================================================================*/
/* Card_LoadJoined.hpp -- Load of the actual Join                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CostJoining : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostJoining(void);
   ~Card_CostJoining(void) noexcept(false);
  };

  Card_CostJoining* _pCard_CostJoining(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
