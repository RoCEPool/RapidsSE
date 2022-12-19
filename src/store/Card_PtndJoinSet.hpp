/*============================================================================*/
/* Card_PtndJoinSet.hpp -- Join Partition List                                */
/*============================================================================*/
#pragma once

#include "Card_CrossPtnJoinList.hpp"

namespace STORE
{
  class Card_PtndJoinSet : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PtndJoinSet(void);
   ~Card_PtndJoinSet(void) noexcept(false);

    static Card_PtndJoinSet* pExtract(Card_CrossPtnJoinList* p_crossPtnJoinList);
  };

  Card_PtndJoinSet* _pCard_PtndJoinSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
