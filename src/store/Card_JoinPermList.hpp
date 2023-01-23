/*============================================================================*/
/* Card_JoinPermList.hpp -- Join Permutation List                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_JoinPermList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPermList(void);
   ~Card_JoinPermList(void) noexcept(false);
   };

  Card_JoinPermList* _pCard_JoinPermList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
