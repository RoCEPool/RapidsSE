/*============================================================================*/
/* Card_MaybeIndexJoin.hpp -- Maybe Index Join                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_MaybeIndexJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_MaybeIndexJoin(void);
   ~Card_MaybeIndexJoin(void) noexcept(false);
  };

  Card_MaybeIndexJoin* _pCard_MaybeIndexJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
