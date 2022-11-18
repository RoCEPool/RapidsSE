/*============================================================================*/
/* Card_Av_AggregPNO.hpp -- Aggregate View Aggregate Partition                */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_AggregPNO : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_AggregPNO(void);
    ~Card_Av_AggregPNO(void) noexcept(false);
  };

  Card_Av_AggregPNO* _pCard_Av_AggregPNO(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
