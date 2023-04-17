/*============================================================================*/
/* Card_StreamCast.hpp -- How buffers were cast to a join locus                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StreamCast : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamCast(void);
   ~Card_StreamCast(void) noexcept(false);
  };

  Card_StreamCast* _pCard_StreamCast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
