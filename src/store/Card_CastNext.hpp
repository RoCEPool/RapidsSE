/*============================================================================*/
/* Card_CastNext.hpp -- How buffers were cast to a next join                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CastNext : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CastNext(void);
   ~Card_CastNext(void) noexcept(false);
  };

  Card_CastNext* _pCard_CastNext(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
