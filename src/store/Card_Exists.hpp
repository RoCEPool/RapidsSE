/*============================================================================*/
/* Card_Exists.hpp -- Exists Card                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Exists : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Exists(void);
   ~Card_Exists(void) noexcept(false);
  };

  Card_Exists* _pCard_Exists(void* aAddr);
  bool _bIsCard_Exists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
