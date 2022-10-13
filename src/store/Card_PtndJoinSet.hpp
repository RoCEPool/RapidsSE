/*============================================================================*/
/* Card_PtndJoinSet.hpp -- Join Partition List                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_PtndJoinSet : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PtndJoinSet(void);
   ~Card_PtndJoinSet(void) noexcept(false);
  };

  Card_PtndJoinSet* _pCard_PtndJoinSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
