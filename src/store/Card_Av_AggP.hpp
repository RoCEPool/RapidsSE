/*============================================================================*/
/* Card_Av_AggP.hpp -- Aggregate View Aggregate Partition                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_AggP : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_AggP(void);
    ~Card_Av_AggP(void) noexcept(false);
  };

  Card_Av_AggP* _pCard_Av_AggP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
