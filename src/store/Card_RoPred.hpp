/*============================================================================*/
/* Card_RoPred.hpp -- Ro Test Predicate                                       */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_RoPred : public Card_PRED
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RoPred(void);
   ~Card_RoPred(void) noexcept(false);

    void vMorph_To_bolPred(CardIter* pCI);

  };

  Card_RoPred* _pCard_RoPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
