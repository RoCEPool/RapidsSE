/*============================================================================*/
/* Card_BOUND.hpp -- Abstract Bound                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CmpPred;

  class Card_BOUND : public Card
  {  
  public:
    Card_BOUND(eCK eCard);
   ~Card_BOUND(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
