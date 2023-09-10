/*============================================================================*/
/* Card_AGG.hpp -- Abstract Aggregate                                         */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"
#include "Card_CONJUNCTION.hpp"

namespace STORE
{
  class Card_AGG : public Card_OP
  {
  public:
    Card_AGG(eCK eCard, i8 iFixedDepth);
   ~Card_AGG(void) noexcept(false);

  };

  Card_AGG* _pCard_AGG(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
