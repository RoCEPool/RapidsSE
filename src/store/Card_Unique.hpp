/*============================================================================*/
/* Card_Unique.hpp -- Unique                                                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Unique : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Unique(void);
   ~Card_Unique(void) noexcept(false);
  };

  Card_Unique* _pCard_Unique(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
