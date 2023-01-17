/*============================================================================*/
/* Card_CrossJoin.hpp -- Cross Join                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CrossJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CrossJoin(void);
   ~Card_CrossJoin(void) noexcept(false);

   static void vIndividuateFilter(CardIter* pCI);

  };

  Card_CrossJoin* _pCard_CrossJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
