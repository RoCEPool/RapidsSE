/*============================================================================*/
/* Card_SqtPred.hpp -- Test Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_SqtPred : public Card_PRED
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SqtPred(void);
   ~Card_SqtPred(void) noexcept(false);
  };

  Card_SqtPred* _pCard_SqtPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
