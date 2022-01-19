/*============================================================================*/
/* Card_Hash.hpp -- Hash                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_UNIT.hpp"

namespace STORE
{
  class Card_Hash : public Card_UNIT
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Hash(void);
   ~Card_Hash(void) noexcept(false);
  };

  Card_Hash* _pCard_Hash(void* aAddr);
  bool _bIsCard_Hash(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
