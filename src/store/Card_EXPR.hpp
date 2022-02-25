/*============================================================================*/
/* Card_EXPR.hpp -- Abstract Expression                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_EXPR : public Card_PRED
  {  
  public:
    Card_EXPR(eCK eCard);
   ~Card_EXPR(void) noexcept(false);
  };

  Card_EXPR* _pCard_EXPR(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
