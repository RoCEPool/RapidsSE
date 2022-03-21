/*============================================================================*/
/* Card_Combine.hpp -- Combine                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_Combine : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Combine(void);
   ~Card_Combine(void) noexcept(false);
  };

  Card_Combine* _pCard_Combine(void* aAddr);
  bool _bIsCard_Combine(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
