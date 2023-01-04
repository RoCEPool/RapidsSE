/*============================================================================*/
/* Card_Agg.hpp -- Agg                                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Agg : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Agg(void);
   ~Card_Agg(void) noexcept(false);
  };

  Card_Agg* _pCard_Agg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
