/*============================================================================*/
/* Card_Mapping.hpp -- Mapping                                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Mapping : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Mapping(void);
   ~Card_Mapping(void) noexcept(false);
  };

  Card_Mapping* _pCard_Mapping(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
