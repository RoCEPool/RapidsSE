/*============================================================================*/
/* Card_StreamKey.hpp -- Stream Key                                           */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_StreamKey : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamKey(void);
   ~Card_StreamKey(void) noexcept(false);

  };

  Card_StreamKey* _pCard_StreamKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
