/*============================================================================*/
/* Card_Map.hpp -- Map                                                        */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Map : public Card_symbol<Card_Map>
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
