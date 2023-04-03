/*============================================================================*/
/* Card_Load.hpp -- Load (of join)                                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Load : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Load(void);
   ~Card_Load(void) noexcept(false);
  };

  Card_Load* _pCard_Load(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
