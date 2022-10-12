/*============================================================================*/
/* Card_CrossPtnJoin.hpp -- Cross Partition                                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CrossPtnJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CrossPtnJoin(void);
   ~Card_CrossPtnJoin(void) noexcept(false);
  };

  Card_CrossPtnJoin* _pCard_CrossPtnJoin(void* aAddr);
  bool _bIsCard_CrossPtnJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
