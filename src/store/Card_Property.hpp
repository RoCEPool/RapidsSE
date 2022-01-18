/*============================================================================*/
/* Card_Property.hpp -- Property                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Property : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Property(Stz* szAtom);
   ~Card_Property(void) noexcept(false);
  };

  Card_Property* _pCard_Property(void* aAddr);
  void _Card_Property(void* aAddr);
  bool _bIsCard_Property(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
