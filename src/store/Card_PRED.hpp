/*============================================================================*/
/* Card_PRED.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_PRED : public Card
  {  
  public:
    Card_PRED(eCK eCard);
    Card_PRED(eCK eCard, Stz* szValue);
   ~Card_PRED(void) noexcept(false);

   bool bPredRecursive(std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
