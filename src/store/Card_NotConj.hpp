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

    void vApplyCast(CardIter* pCI, Card_Emit* p_Elist_ref);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_NotConj* _pCard_NotConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
