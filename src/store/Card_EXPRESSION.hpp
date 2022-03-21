/*============================================================================*/
/* Card_EXPRESSION.hpp -- Abstract Expression                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_EXPRESSION : public Card_PRED
  {  
  public:
    Card_EXPRESSION(eCK eCard);
   ~Card_EXPRESSION(void) noexcept(false);
  };

  Card_EXPRESSION* _pCard_EXPRESSION(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
