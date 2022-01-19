/*============================================================================*/
/* Card_PART.hpp -- Abstract Part                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_PART : public Card
  {  
  public:
    Card_PART(eCK eCard);
   ~Card_PART(void) noexcept(false);
  };

  Card_PART* _pCard_Part(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
