/*============================================================================*/
/* Card_AggPred.hpp -- Aggregate Predicate                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_AggPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AggPred(void);
   ~Card_AggPred(void) noexcept(false);
  };

  Card_AggPred* _pCard_AggPred(void* aAddr);
  bool _bIsCard_AggPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
