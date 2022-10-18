/*============================================================================*/
/* Card_LinksTo.hpp -- Links To                                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LinksTo : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LinksTo(Stz* szRows);
   ~Card_LinksTo(void) noexcept(false);
  };

  Card_LinksTo* _pCard_LinksTo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
