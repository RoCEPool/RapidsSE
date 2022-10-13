/*============================================================================*/
/* Card_Hash.hpp -- Hash                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Hash : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Hash(void);
   ~Card_Hash(void) noexcept(false);
  };

  Card_Hash* _pCard_Hash(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
