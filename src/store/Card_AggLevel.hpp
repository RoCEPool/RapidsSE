/*============================================================================*/
/* Card_AggLevel.hpp -- Aggregate Level                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_AggLevel : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AggLevel(eAL eAggLevel);
   ~Card_AggLevel(void) noexcept(false);
  };

  Card_AggLevel* _pCard_AggLevel(void* aAddr);
  bool _bIsCard_AggLevel(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
