/*============================================================================*/
/* Card_As.hpp -- As                                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_As : public Card_SUB
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_As(Stz* szAs);
   ~Card_As(void) noexcept(false);
  };

  Card_As* _pCard_As(void* aAddr);
  bool _bIsCard_As(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
