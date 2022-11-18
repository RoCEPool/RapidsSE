/*============================================================================*/
/* Card_Av_AggPNO.hpp -- Aggregate View Aggregate Partition Number            */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_AggPNO : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_AggPNO(void);
    ~Card_Av_AggPNO(void) noexcept(false);
  };

  Card_Av_AggPNO* _pCard_Av_AggPNO(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
