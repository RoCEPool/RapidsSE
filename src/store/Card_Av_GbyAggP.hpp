/*============================================================================*/
/* Card_Av_GbyAggP.hpp -- Aggregate View Group By Aggregate Partition         */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_GbyAggP : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_GbyAggP(void);
    ~Card_Av_GbyAggP(void) noexcept(false);
  };

  Card_Av_GbyAggP* _pCard_Av_GbyAggP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
