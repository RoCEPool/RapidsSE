/*============================================================================*/
/* Card_COND.hpp -- Abstract Condition                                        */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_COND : public Card_OP
  {
  public:
    Card_COND(void);
   ~Card_COND(void) noexcept(false);

  };

  Card_COND* _pCard_COND(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
