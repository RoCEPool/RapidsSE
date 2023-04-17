/*============================================================================*/
/* Card_JoinCast.hpp -- How buffers were cast to a join locus                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinCast : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinCast(void);
   ~Card_JoinCast(void) noexcept(false);
  };

  Card_JoinCast* _pCard_JoinCast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
