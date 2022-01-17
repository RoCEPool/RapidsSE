/*============================================================================*/
/* Card_OrPred.hpp -- Or Predicate                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_OrPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrPred(void);
   ~Card_OrPred(void) noexcept(false);
  };

  Card_OrPred* _pCard_OrPred(void* aAddr);
  bool _bIsCard_OrPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
