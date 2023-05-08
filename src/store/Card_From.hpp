/*============================================================================*/
/* Card_From.hpp -- From                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_From : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_From(void);
   ~Card_From(void) noexcept(false);
  };

  Card_From* _pCard_From(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
