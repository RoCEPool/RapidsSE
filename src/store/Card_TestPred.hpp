/*============================================================================*/
/* Card_TestPred.hpp -- Test Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_TestPred : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TestPred(void);
   ~Card_TestPred(void) noexcept(false);
  };

  Card_TestPred* _pCard_TestPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
