/*============================================================================*/
/* Card_OrConj.hpp -- Or Predicate                                            */
/*============================================================================*/
#pragma once

#include "Card_CONJUNCTION.hpp"

namespace STORE
{
  class Card_OrConj : public Card_CONJUNCTION
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrConj(void);
   ~Card_OrConj(void) noexcept(false);

    Card_CmpConj* pOrPredFindBound(Index* pIndex, ePB eBound);
    bool bOrPredHasBounds(Index* pIndex);

    void vEnsureTypesFor_stmOdn(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OrConj* _pCard_OrConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
