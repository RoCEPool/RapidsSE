/*============================================================================*/
/* Card_AndConj.hpp -- And Predicate                                          */
/*============================================================================*/
#pragma once

#include "Card_CONJ.hpp"

namespace STORE
{
  class Card_AndConj : public Card_CONJ
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AndConj(void);
   ~Card_AndConj(void) noexcept(false);

    void vApplyCast(CardIter* pCI, Card_Emit* p_Elist_ref);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_AndConj* _pCard_AndConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
