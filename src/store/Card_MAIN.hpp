/*============================================================================*/
/* Card_MAIN.hpp -- Abstract Main-Card                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_MAIN : public Card
  {  
  public:
    Card_MAIN(eCK eCard);
   ~Card_MAIN(void) noexcept(false);
  };

  Card_MAIN* _pCard_MAIN(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
