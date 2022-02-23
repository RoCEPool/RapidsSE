/*============================================================================*/
/* Card_Gby.hpp -- a group by element                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Gby : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Gby(void);
   ~Card_Gby(void) noexcept(false);
  };

  Card_Gby* _pCard_Gby(void* aAddr);
  bool _bIsCard_Gby(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
