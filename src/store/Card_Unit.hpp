/*============================================================================*/
/* Card_Unit.hpp -- Abstract Unit                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Unit : public Card
  {  
  public:
    Card_Unit(eCK eKind);
   ~Card_Unit(void) noexcept(false);
  };

  Card_Unit* _pCard_Unit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
