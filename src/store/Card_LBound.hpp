/*============================================================================*/
/* Card_LBound.hpp -- Lower Bound                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_BOUND.hpp"

namespace STORE
{
  class Card_LBound : public Card_BOUND
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LBound(void);
   ~Card_LBound(void) noexcept(false);
  };

  Card_LBound* _pCard_LBound(void* aAddr);
  bool _bIsCard_LBound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
