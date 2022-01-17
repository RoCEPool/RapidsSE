/*============================================================================*/
/* Card_As.hpp -- As                                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Part.hpp"

namespace STORE
{
  class Card_As : public Card_Part
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_As(eCK eKind, Stz* szAsNm);
   ~Card_As(void) noexcept(false);
  };

  Card_As* _pCard_As(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
