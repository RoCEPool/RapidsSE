/*============================================================================*/
/* Card_NotConj.hpp -- Not Predicate                                          */
/*============================================================================*/
#pragma once

#include "Card_CONJ.hpp"

namespace STORE
{
  class Card_NotConj : public Card_CONJ
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_NotConj(void);
   ~Card_NotConj(void) noexcept(false);

    void vEnsure_Cast(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_NotConj* _pCard_NotConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
