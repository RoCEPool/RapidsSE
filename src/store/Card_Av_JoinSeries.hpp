/*============================================================================*/
/* Card_Av_JoinSeries.hpp -- Aggregate View Join Series                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_JoinSeries : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_JoinSeries(void);
    ~Card_Av_JoinSeries(void) noexcept(false);
  };

  Card_Av_JoinSeries* _pCard_Av_JoinSeries(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
