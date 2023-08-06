/*============================================================================*/
/* Card_JoinOrder.hpp -- Join Order Sequence                               */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
   class Card_JoinOrder : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinOrder(void);
    Card_JoinOrder(Stz* szValue);
   ~Card_JoinOrder(void) noexcept(false);
   };

  Card_JoinOrder* _pCard_JoinOrder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
