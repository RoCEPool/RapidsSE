/*============================================================================*/
/* Card_Filter.hpp -- Filter Card                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"

namespace STORE
{
  class Card_Filter : public Card_Unit
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Filter(eCK eKind);
   ~Card_Filter(void) noexcept(false);
  };

  Card_Filter* _pCard_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
