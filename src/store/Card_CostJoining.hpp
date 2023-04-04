/*============================================================================*/
/* Card_LoadJoined.hpp -- Load of the actual Join                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CostJoining : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostJoining(void);
   ~Card_CostJoining(void) noexcept(false);

    i8 nEstJoiningBufferVolume(i8 nBuff_emitted_prior,
                               eXK eLocusCast,
                               i8 nBuff_joining_with);
  };

  Card_CostJoining* _pCard_CostJoining(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
