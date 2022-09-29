/*============================================================================*/
/* Card_Aggregates.hpp -- Aggregates                                          */
/*============================================================================*/
#pragma once

#include "Card_Project.hpp"

namespace STORE
{
  class Card_Aggregates : public Card_Project
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Aggregates(void);
   ~Card_Aggregates(void) noexcept(false);
  };

  Card_Aggregates* _pCard_Aggregates(void* aAddr);
  bool _bIsCard_Aggregates(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
