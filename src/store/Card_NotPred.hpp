/*============================================================================*/
/* Card_NotPred.hpp -- Not Predicate                                          */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_NotPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_NotPred(void);
   ~Card_NotPred(void) noexcept(false);

    void vEnsureTypesFor_stmOdn(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_NotPred* _pCard_NotPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
