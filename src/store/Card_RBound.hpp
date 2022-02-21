/*============================================================================*/
/* Card_RBound.hpp -- Range-Bound                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_BOUND.hpp"

namespace STORE
{
  class Card_RBound : public Card_BOUND
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RBound(void);
   ~Card_RBound(void) noexcept(false);
  };

  Card_RBound* _pCard_RBound(void* aAddr);
  bool _bIsCard_RBound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
