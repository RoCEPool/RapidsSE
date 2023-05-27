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
    Card_COND(eCK eCard);
   ~Card_COND(void) noexcept(false);

   void vAddTransients(CardIter* pCI);
  };

  Card_COND* _pCard_COND(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
