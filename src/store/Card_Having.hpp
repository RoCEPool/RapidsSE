/*============================================================================*/
/* Card_Having.hpp -- Having Card                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"

namespace STORE
{
  class Card_Having : public Card_Unit
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Having(eCK eKind);
   ~Card_Having(void) noexcept(false);
  };

  Card_Having* _pCard_Having(void* aAddr);
  bool _bIsCard_Having(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
