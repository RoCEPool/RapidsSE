/*============================================================================*/
/* Card_CrossPtnPred.hpp -- Filter                                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CrossPtnPred : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CrossPtnPred(void);
   ~Card_CrossPtnPred(void) noexcept(false);
  };

  Card_CrossPtnPred* _pCard_CrossPtnPred(void* aAddr);
  bool _bIsCard_CrossPtnPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
