/*============================================================================*/
/* Card_CrossPtnList.hpp -- Cross-Partition List                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CrossPtnList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CrossPtnList(void);
   ~Card_CrossPtnList(void) noexcept(false);
  };

  Card_CrossPtnList* _pCard_CrossPtnList(void* aAddr);
  bool _bIsCard_CrossPtnList(void* aAddr);
  Card_CrossPtnList* _pGetSub_CrossPtnList(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
