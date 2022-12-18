/*============================================================================*/
/* Card_OrdElmOdn.hpp -- Order Element Ordinal                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_OrdElmOdn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_OrdElmOdn(void);
    ~Card_OrdElmOdn(void) noexcept(false);
  };

  Card_OrdElmOdn* _pCard_OrdElmOdn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
