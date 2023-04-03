/*============================================================================*/
/* Card_LoadUnicast.hpp -- Load of Unicast (in load of join)                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LoadUnicast : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LoadUnicast(void);
   ~Card_LoadUnicast(void) noexcept(false);

    void vEstUnicastVolume(eXK eNextCast,
                           i8 nBuff_arriving,
                           i8 nBuff_joiningTo);
  };

  Card_LoadUnicast* _pCard_LoadUnicast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
