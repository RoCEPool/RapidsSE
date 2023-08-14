/*============================================================================*/
/* Card_OrPred.hpp -- Or Predicate                                            */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_OrPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrPred(void);
   ~Card_OrPred(void) noexcept(false);

    Card_CmpPred* pOrPredFindBound(Index* pIndex, ePB eBound);
    bool bOrPredHasBounds(Index* pIndex);

    void vEnsureTypesFor_stmOdn(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OrPred* _pCard_OrPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
