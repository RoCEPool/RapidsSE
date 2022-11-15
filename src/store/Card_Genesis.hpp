/*============================================================================*/
/* Card_Genesis.hpp -- Genesis (e.g. subquery origin/beginning)               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Genesis : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Genesis(void);
   ~Card_Genesis(void) noexcept(false);
  };

  Card_Genesis* _pCard_Genesis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
