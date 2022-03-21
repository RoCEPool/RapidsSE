/*============================================================================*/
/* Card_Unique.hpp -- Unique                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Unique : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Unique(void);
   ~Card_Unique(void) noexcept(false);
  };

  Card_Unique* _pCard_Unique(void* aAddr);
  bool _bIsCard_Unique(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
