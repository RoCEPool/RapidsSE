/*============================================================================*/
/* Card_JoinLocus.hpp -- Join Head (head of join sequence)                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinLocus : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinLocus(void);
   ~Card_JoinLocus(void) noexcept(false);

    i8 nEstLocusCosts(eXK eLocusCast,
                      i8 nBuff_emitted,
                      i8 nBuff_joining);

  };

  Card_JoinLocus* _pCard_JoinLocus(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
