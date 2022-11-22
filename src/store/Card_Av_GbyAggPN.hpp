/*============================================================================*/
/* Card_Av_GbyAggPN.hpp -- Aggregate View Group By Aggregate Partition Number */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_GbyAggPN : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_GbyAggPN(void);
    ~Card_Av_GbyAggPN(void) noexcept(false);
  };

  Card_Av_GbyAggPN* _pCard_Av_GbyAggPN(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
