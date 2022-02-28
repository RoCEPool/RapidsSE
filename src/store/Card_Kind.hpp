/*============================================================================*/
/* Card_Kind.hpp -- Kind                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Kind : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Kind(i8 iKind);
   ~Card_Kind(void) noexcept(false);
  };

  Card_Kind* _pCard_Kind(void* aAddr);
  bool _bIsCard_Kind(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
