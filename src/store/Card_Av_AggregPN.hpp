/*============================================================================*/
/* Card_Av_AggregPN.hpp -- Aggregate View Aggregate Partition                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_AggregPN : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_AggregPN(void);
    ~Card_Av_AggregPN(void) noexcept(false);
  };

  Card_Av_AggregPN* _pCard_Av_AggregPN(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
