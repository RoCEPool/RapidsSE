/*============================================================================*/
/* Card_LoadJoin.hpp -- Load of Join                                          */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LoadJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LoadJoin(void);
   ~Card_LoadJoin(void) noexcept(false);
  };

  Card_LoadJoin* _pCard_LoadJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
