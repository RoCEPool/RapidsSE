/*============================================================================*/
/* Card_AGGREGATE.hpp -- Abstract Aggregate                                   */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"
#include "Card_CONJ.hpp"

namespace STORE
{
  class Card_AGGREGATE : public Card_OP
  {
  public:
    Card_AGGREGATE(eCK eCard, i8 iFixedDepth);
   ~Card_AGGREGATE(void) noexcept(false);

  };

  Card_AGGREGATE* _pCard_AGGREGATE(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
