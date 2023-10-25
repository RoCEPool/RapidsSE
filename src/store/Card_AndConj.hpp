/*============================================================================*/
/* Card_AndConj.hpp -- And Predicate                                          */
/*============================================================================*/
#pragma once

#include "Card_CONJUNCTION.hpp"

namespace STORE
{
  class Card_AndConj : public Card_CONJUNCTION
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AndConj(void);
   ~Card_AndConj(void) noexcept(false);

    Card_RlnConj* pAndConjFindBound(Index* pIndex, ePB eBound);
    bool bAndPredHasBounds(Index* pIndex);
    void vEnsureTypesFor_stmCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_AndConj* _pCard_AndConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
