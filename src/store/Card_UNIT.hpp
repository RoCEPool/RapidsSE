/*============================================================================*/
/* Card_UNIT.hpp -- Abstract Unit                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_UNIT : public Card
  {  
  public:
    Card_UNIT(eCK eCard);
   ~Card_UNIT(void) noexcept(false);
  };

  Card_UNIT* _pCard_Unit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
