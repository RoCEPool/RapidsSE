/*============================================================================*/
/* Card_Part.hpp -- Part                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Part : public Card
  {  
  public:
  
    Card_Part(eCK eKind, eCN eClass);
   ~Card_Part(void) noexcept(false);
  };

  Card_Part* _pCard_Part(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
