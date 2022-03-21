/*============================================================================*/
/* Card_SendKind.hpp -- Kind                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_SendKind : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SendKind(eXK eSendKind);
   ~Card_SendKind(void) noexcept(false);
  };

  Card_SendKind* _pCard_SendKind(void* aAddr);
  bool _bIsCard_SendKind(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
