/*============================================================================*/
/* Card_Av_ScanJoinSeries.hpp -- Aggregate View Scan Join Series              */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_ScanJoinSeries : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_ScanJoinSeries(void);
    ~Card_Av_ScanJoinSeries(void) noexcept(false);
  };

  Card_Av_ScanJoinSeries* _pCard_Av_ScanJoinSeries(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
