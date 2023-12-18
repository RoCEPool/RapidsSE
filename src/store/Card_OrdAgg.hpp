/*============================================================================*/
/* Card_OrdAgg.hpp -- Ordered Aggregate                                       */
/*============================================================================*/
#pragma once

#include "Card_AGGREGATE.hpp"

namespace STORE
{
  class Card_OrdAgg : public Card_AGGREGATE
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrdAgg(void);
   ~Card_OrdAgg(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

    void vEarlyProject_root(CardIter* pCI);
    void vUpdate_emit(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OrdAgg* _pCard_OrdAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
