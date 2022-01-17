/*============================================================================*/
/* Card_Pred.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Pred : public Card
  {  
  public:
    Card_Pred(eCK eKind, eCN eClass);
   ~Card_Pred(void) noexcept(false);
  };

  Card_Pred* _pCard_Pred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
