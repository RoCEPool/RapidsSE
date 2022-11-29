/*============================================================================*/
/* Card_NotPred.hpp -- Not Predicate                                          */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_NotPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_NotPred(void);
   ~Card_NotPred(void) noexcept(false);
  };

  Card_NotPred* _pCard_NotPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
