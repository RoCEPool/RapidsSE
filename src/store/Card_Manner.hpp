/*============================================================================*/
/* Card_Manner.hpp -- Fold Kind ( a-b or b-a )                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Manner : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Manner(void);
   ~Card_Manner(void) noexcept(false);
  };

  Card_Manner* _pCard_Manner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
