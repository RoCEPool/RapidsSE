/*============================================================================*/
/* Card_OTHER.hpp -- Abstract Other-Card                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_OTHER : public Card
  {  
  public:
    Card_OTHER(eCK eCard);
   ~Card_OTHER(void) noexcept(false);
  };

  Card_OTHER* _pCard_OTHER(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
