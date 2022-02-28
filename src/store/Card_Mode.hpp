/*============================================================================*/
/* Card_Mode.hpp -- Mode                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Mode : public Card_SUB
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Mode(Stz* szMode);
   ~Card_Mode(void) noexcept(false);
  };

  Card_Mode* _pCard_Mode(void* aAddr);
  bool _bIsCard_Mode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
