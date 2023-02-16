/*============================================================================*/
/* Card_SubQuery.hpp -- Sub-Query                                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_SubQuery : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SubQuery(void);
   ~Card_SubQuery(void) noexcept(false);
  };

  Card_SubQuery* _pCard_SubQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
