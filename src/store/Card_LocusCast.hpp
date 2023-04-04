/*============================================================================*/
/* Card_LocusCast.hpp -- How buffers were cast to a join locus                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LocusCast : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LocusCast(void);
   ~Card_LocusCast(void) noexcept(false);
  };

  Card_LocusCast* _pCard_LocusCast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
