/*============================================================================*/
/* Card_Limit.hpp -- Limit                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_UNIT.hpp"

namespace STORE
{
  class Card_Limit : public Card_UNIT
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Limit(Stz* szRows);
   ~Card_Limit(void) noexcept(false);
  };

  Card_Limit* _pCard_Limit(void* aAddr);
  bool _bIsCard_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
