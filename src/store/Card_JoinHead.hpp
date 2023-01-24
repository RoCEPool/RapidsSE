/*============================================================================*/
/* Card_JoinHead.hpp -- Join Head (head of join sequence)                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_JoinHead : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinHead(void);
   ~Card_JoinHead(void) noexcept(false);
   };

  Card_JoinHead* _pCard_JoinHead(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
