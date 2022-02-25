/*============================================================================*/
/* Card_EBound.hpp -- Equi-Bound                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_BOUND.hpp"

namespace STORE
{
  class Card_EBound : public Card_BOUND
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_EBound(void);
   ~Card_EBound(void) noexcept(false);
  };

  Card_EBound* _pCard_EBound(void* aAddr);
  bool _bIsCard_EBound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
