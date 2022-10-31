/*============================================================================*/
/* Card_ExePred.hpp -- Execute Predicate                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_ExePred : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ExePred(void);
   ~Card_ExePred(void) noexcept(false);
  };

  Card_ExePred* _pCard_ExePred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
