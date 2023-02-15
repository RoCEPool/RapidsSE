/*============================================================================*/
/* Card_BoardNo.hpp -- Board Number                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_BoardNo : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoardNo(void);
   ~Card_BoardNo(void) noexcept(false);
  };

  Card_BoardNo* _pCard_BoardNo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
