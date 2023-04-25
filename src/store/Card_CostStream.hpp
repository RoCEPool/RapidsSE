/*============================================================================*/
/* Card_CostStream.hpp -- Estimate Cost for Query                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CostStream : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostStream(void);
   ~Card_CostStream(void) noexcept(false);

    bool bIsHigherCost(Card_CostStream* p_costQuery_arg);
    bool bIsLowerCost(Card_CostStream* p_costQuery_arg);
  };

  Card_CostStream* _pCard_CostStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
