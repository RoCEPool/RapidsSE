/*============================================================================*/
/* Card_Av_Limit.hpp -- Aggregate View Limit                                   */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_Limit : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_Limit(void);
    ~Card_Av_Limit(void) noexcept(false);
  };

  Card_Av_Limit* _pCard_Av_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
