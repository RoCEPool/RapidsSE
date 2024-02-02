/*============================================================================*/
/* Card_OrConj.hpp -- Or Predicate                                            */
/*============================================================================*/
#pragma once

#include "Card_CONJ.hpp"

namespace STORE
{
  class Card_OrConj : public Card_CONJ
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrConj(void);
   ~Card_OrConj(void) noexcept(false);

    void vEnsure_Cast(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OrConj* _pCard_OrConj(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
