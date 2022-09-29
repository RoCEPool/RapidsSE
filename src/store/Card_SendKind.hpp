/*============================================================================*/
/* Card_SendKind.hpp -- Kind                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_SendKind : public Card
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
