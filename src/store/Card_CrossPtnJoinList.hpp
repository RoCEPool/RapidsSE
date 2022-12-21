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

    static Card_CrossPtnJoinList* pExtract(Card_JoinPredList* p_joinPredList,
                                           Card_CoPtnJoinList* p_coPtnJoinList);
  };

  Card_CrossPtnJoinList* _pCard_CrossPtnJoinList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
