/*============================================================================*/
/* Card_OrdAgg.hpp -- Ordered Aggregate                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Project.hpp"

namespace STORE
{
  class Card_OrdAgg : public Card_Project
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrdAgg(eAL eAggLvl);
   ~Card_OrdAgg(void) noexcept(false);
  };

  Card_OrdAgg* _pCard_OrdAgg(void* aAddr);
  bool _bIsCard_OrdAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
