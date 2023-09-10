/*============================================================================*/
/* Card_AndPred.hpp -- And Predicate                                          */
/*============================================================================*/
#pragma once

#include "Card_CONJUNCTION.hpp"

namespace STORE
{
  class Card_AndPred : public Card_CONJUNCTION
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AndPred(void);
   ~Card_AndPred(void) noexcept(false);

    Card_CmpPred* pAndPredFindBound(Index* pIndex, ePB eBound);
    bool bAndPredHasBounds(Index* pIndex);
    void vEnsureTypesFor_stmOdn(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_AndPred* _pCard_AndPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
