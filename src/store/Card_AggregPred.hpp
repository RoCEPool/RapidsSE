/*============================================================================*/
/* Card_AggregPred.hpp -- Aggregate Predicate                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_AggregPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AggregPred(void);
   ~Card_AggregPred(void) noexcept(false);
  };

  Card_AggregPred* _pCard_AggregPred(void* aAddr);
  bool _bIsCard_AggregPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
