/*============================================================================*/
/* Card_Send.hpp -- Send                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_UNIT.hpp"

namespace STORE
{
  class Card_Send : public Card_UNIT
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Send(eXK eSendKind);
   ~Card_Send(void) noexcept(false);
  };

  Card_Send* _pCard_Send(void* aAddr);
  bool _bIsCard_Send(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
