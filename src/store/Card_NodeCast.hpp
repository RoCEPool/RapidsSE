/*============================================================================*/
/* Card_NodeCast.hpp -- Node Cast                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_NodeCast : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_NodeCast(eCK eKind);
   ~Card_NodeCast(void) noexcept(false);
  };

  Card_NodeCast* _pCard_NodeCast(void* aAddr);
  bool _bIsCard_NodeCast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
