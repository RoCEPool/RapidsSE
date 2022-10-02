/*============================================================================*/
/* Card_CrossPtnJoinList.hpp -- Cross-Partition List                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CrossPtnJoinList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CrossPtnJoinList(void);
   ~Card_CrossPtnJoinList(void) noexcept(false);
  };

  Card_CrossPtnJoinList* _pCard_CrossPtnJoinList(void* aAddr);
  bool _bIsCard_CrossPtnJoinList(void* aAddr);
  Card_CrossPtnJoinList* _pGetSub_CrossPtnJoinList(Card * pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
