/*============================================================================*/
/* Card_Av_Sort.hpp -- Aggregate View Sort                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_Sort : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_Sort(void);
    ~Card_Av_Sort(void) noexcept(false);
  };

  Card_Av_Sort* _pCard_Av_Sort(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
