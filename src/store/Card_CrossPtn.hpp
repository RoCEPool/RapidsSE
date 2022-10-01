/*============================================================================*/
/* Card_CrossPtn.hpp -- Cross Partition                                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CrossPtn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CrossPtn(void);
   ~Card_CrossPtn(void) noexcept(false);
  };

  Card_CrossPtn* _pCard_CrossPtn(void* aAddr);
  bool _bIsCard_CrossPtn(void* aAddr);
  Card_CrossPtn* _pGetSub_CrossPtn(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
