/*============================================================================*/
/* Card_Send.hpp -- Send                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_Send : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Send(void);
   ~Card_Send(void) noexcept(false);
  };

  Card_Send* _pCard_Send(void* aAddr);
  bool _bIsCard_Send(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
