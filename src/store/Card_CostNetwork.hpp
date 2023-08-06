/*============================================================================*/
/* Card_CostNetwork.hpp -- Load of Network (in load of join)                  */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_CostNetwork : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostNetwork(void);
   ~Card_CostNetwork(void) noexcept(false);
  };

  Card_CostNetwork* _pCard_CostNetwork(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
