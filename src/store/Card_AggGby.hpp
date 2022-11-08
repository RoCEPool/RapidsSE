/*============================================================================*/
/* Card_AggGby.hpp -- Aggreg-GroupBy                                          */
/*============================================================================*/
#pragma once

#include "Card_Project.hpp"

namespace STORE
{
  class Card_AggGby : public Card_Project
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AggGby(void);
   ~Card_AggGby(void) noexcept(false);
  };

  Card_AggGby* _pCard_AggGby(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
