/*============================================================================*/
/* Card_AndPred.hpp -- And Predicate                                          */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_AndPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AndPred(void);
   ~Card_AndPred(void) noexcept(false);
    
    Card_CmpPred* pAndPred_FindBound(Index* pIndex, ePB eBound);
    bool bAndPred_HasBounds(Index* pIndex);
  };

  Card_AndPred* _pCard_AndPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
