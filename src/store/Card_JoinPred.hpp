/*============================================================================*/
/* Card_JoinPred.hpp -- Join Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinPred : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPred(void);
   ~Card_JoinPred(void) noexcept(false);
  };

  Card_JoinPred* _pCard_JoinPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
