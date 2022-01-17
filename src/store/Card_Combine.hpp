/*============================================================================*/
/* Card_Combine.hpp -- Combine                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"
#include "eXK.h"

namespace STORE
{
  class Card_Combine : public Card_Unit
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Combine(eCK eKind);
   ~Card_Combine(void) noexcept(false);
  };

  Card_Combine* _pCard_Combine(void* aAddr);
  bool _bIsCard_Combine(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
