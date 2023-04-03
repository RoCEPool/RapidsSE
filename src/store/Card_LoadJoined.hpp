/*============================================================================*/
/* Card_LoadJoined.hpp -- Load of the actual Join                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LoadJoined : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LoadJoined(void);
   ~Card_LoadJoined(void) noexcept(false);

    i8 nEstJoinVolume(i8 nNodes,
                      eXK eNextCast,
                      i8 nBuff_arriving,
                      i8 iSize_r);

  };

  Card_LoadJoined* _pCard_LoadJoined(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
