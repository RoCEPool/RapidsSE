/*============================================================================*/
/* Card_XBound.hpp -- Cross Bound                                             */
/*============================================================================*/
#pragma once

#include "Card_Bound.hpp"

namespace STORE
{
  class Card_XBound : public Card_Bound
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_XBound(void);
    ~Card_XBound(void) noexcept(false);
  };

  Card_XBound* _pCard_XBound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
