/*============================================================================*/
/* Card_Av_Filter.hpp -- Filter                                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_Filter : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_Filter(void);
    ~Card_Av_Filter(void) noexcept(false);
  };

  Card_Av_Filter* _pCard_Av_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
