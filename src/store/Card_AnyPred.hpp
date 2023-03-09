/*============================================================================*/
/* Card_AnyPred.hpp -- Any Predicate                                          */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_AnyPred : public Card_PRED
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AnyPred(void);
   ~Card_AnyPred(void) noexcept(false);
  };

  Card_AnyPred* _pCard_AnyPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
