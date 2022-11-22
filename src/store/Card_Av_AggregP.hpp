/*============================================================================*/
/* Card_Av_AggregP.hpp -- Aggregate View                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_AggregP : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_AggregP(void);
    ~Card_Av_AggregP(void) noexcept(false);
  };

  Card_Av_AggregP* _pCard_Av_AggregP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
