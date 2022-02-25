/*============================================================================*/
/* Card_Type.hpp -- Type                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Type : public Card_SUB
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Type(Stz* szType);
   ~Card_Type(void) noexcept(false);
  };

  Card_Type* _pCard_Type(void* aAddr);
  bool _bIsCard_Type(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
