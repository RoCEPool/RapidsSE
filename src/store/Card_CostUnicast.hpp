/*============================================================================*/
/* Card_CostUnicast.hpp -- Load of Unicast (in load of join)                  */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_CostUnicast : public Card_symbol<Card_CostUnicast>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostUnicast(void);
   ~Card_CostUnicast(void) noexcept(false);
  };

  Card_CostUnicast* _pCard_CostUnicast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
