/*============================================================================*/
/* Card_Next.hpp -- Next                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Next : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Next(void);
   ~Card_Next(void) noexcept(false);
  };

  Card_Next* _pCard_Next(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
