/*============================================================================*/
/* Card_UBound.hpp -- Upper Bound                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_BOUND.hpp"

namespace STORE
{
  class Card_UBound : public Card_BOUND
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_UBound(void);
   ~Card_UBound(void) noexcept(false);
  };

  Card_UBound* _pCard_UBound(void* aAddr);
  bool _bIsCard_UBound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
