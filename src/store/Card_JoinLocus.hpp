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

    i8 nEstLocusCosts(eXK eCastLocus,
                      i8 nBuff_arriving,
                      i8 nBuff_joiningTo);

  };

  Card_JoinLocus* _pCard_JoinLocus(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
