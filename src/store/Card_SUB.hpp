/*============================================================================*/
/* Card_SUB.hpp -- Abstract Sub-Card                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_SUB : public Card
  {  
  public:
    Card_SUB(eCK eCard);
   ~Card_SUB(void) noexcept(false);
  };

  Card_SUB* _pCard_SUB(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
