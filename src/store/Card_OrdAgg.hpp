/*============================================================================*/
/* Card_OrdAgg.hpp -- Ordered Aggregate                                       */
/*============================================================================*/
#pragma once

#include "Card_Project.hpp"

namespace STORE
{
  class Card_OrdAgg : public Card_Project
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrdAgg(void);
   ~Card_OrdAgg(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

    void vEarlyProject_root(CardIter* pCI);
    void vUpdate_projection(CardIter* pCI);

  };

  Card_OrdAgg* _pCard_OrdAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
