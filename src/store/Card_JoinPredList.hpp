/*============================================================================*/
/* Card_JoinPredList.hpp -- Join Predicate List                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinPredList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPredList(void);
   ~Card_JoinPredList(void) noexcept(false);
  };

  Card_JoinPredList* _pCard_JoinPredList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
