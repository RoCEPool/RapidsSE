/*============================================================================*/
/* Card_JoinNext.hpp -- Join Head (head of join sequence)                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_JoinNext : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinNext(void);
   ~Card_JoinNext(void) noexcept(false);
   };

  Card_JoinNext* _pCard_JoinNext(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
