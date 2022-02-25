/*============================================================================*/
/* Card_BOUND.hpp -- Abstract Bound                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_CmpPred;

  class Card_BOUND : public Card_SUB
  {  
  public:
    Card_BOUND(eCK eCard);
   ~Card_BOUND(void) noexcept(false);
  };

  //Card_BOUND* _pCard_BOUND(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
