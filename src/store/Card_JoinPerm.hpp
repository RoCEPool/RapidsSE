/*============================================================================*/
/* Card_JoinPerm.hpp -- Join Permutation                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_JoinPerm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPerm(void);
   ~Card_JoinPerm(void) noexcept(false);
   };

  Card_JoinPerm* _pCard_JoinPerm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
