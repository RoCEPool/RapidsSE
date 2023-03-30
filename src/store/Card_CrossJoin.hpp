/*============================================================================*/
/* Card_CrossJoin.hpp -- Cross Join                                           */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_CrossJoin : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CrossJoin(void);
   ~Card_CrossJoin(void) noexcept(false);

    static void vIndividuateFilter(CardIter* pCI);

    static void vTableMorphsToCrossJoin(CardIter* pCI);

    void vEarlyProject_recurse_update(CardIter* pCI);

  };

  Card_CrossJoin* _pCard_CrossJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
