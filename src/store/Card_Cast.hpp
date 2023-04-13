/*============================================================================*/
/* Card_Cast.hpp -- Stage Cast (aka Next)                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Cast : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Cast(void);
   ~Card_Cast(void) noexcept(false);
  };

  Card_Cast* _pCard_Cast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
