/*============================================================================*/
/* Card_CostUnicast.hpp -- Load of Unicast (in load of join)                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CostUnicast : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostUnicast(void);
   ~Card_CostUnicast(void) noexcept(false);

    void vEstUnicastBufferVolume(i8 nLocus, i8 nBuff_emitted_prior, eXK eLocusCast);
  };

  Card_CostUnicast* _pCard_CostUnicast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
