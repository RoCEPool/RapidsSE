/*============================================================================*/
/* Card_Order.hpp -- Order                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Order : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Order(void);
    ~Card_Order(void) noexcept(false);
  };

  Card_Order* _pCard_Order(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
