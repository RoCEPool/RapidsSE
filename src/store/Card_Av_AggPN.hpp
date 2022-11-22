/*============================================================================*/
/* Card_Av_AggPN.hpp -- Aggregate View Aggregate Partition Number             */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_AggPN : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_AggPN(void);
    ~Card_Av_AggPN(void) noexcept(false);
  };

  Card_Av_AggPN* _pCard_Av_AggPN(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
