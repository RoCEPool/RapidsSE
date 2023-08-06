/*============================================================================*/
/* Card_Map.hpp -- Map                                                        */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_Map : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Map(void);
   ~Card_Map(void) noexcept(false);
  };

  Card_Map* _pCard_Map(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
