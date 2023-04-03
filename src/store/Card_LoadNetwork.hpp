/*============================================================================*/
/* Card_LoadNetwork.hpp -- Load of Network (in load of join)                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LoadNetwork : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LoadNetwork(void);
   ~Card_LoadNetwork(void) noexcept(false);

    void vEstNetworkVolume(eXK eNextCast,
                           i8 nBuff_arriving,
                           i8 nBuff_joiningTo);
  };

  Card_LoadNetwork* _pCard_LoadNetwork(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
