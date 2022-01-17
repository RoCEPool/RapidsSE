/*============================================================================*/
/* Card_Hash.hpp -- Hash                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"
#include "eXK.h"

namespace STORE
{
  class Card_Hash : public Card_Unit
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Hash(eCK eKind);
   ~Card_Hash(void) noexcept(false);
  };

  Card_Hash* _pCard_Hash(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
