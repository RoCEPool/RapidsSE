/*============================================================================*/
/* Card_CalcOp.hpp -- Calc Op (e.g. in, exists, ==)                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CalcOp : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CalcOp(void);
   ~Card_CalcOp(void) noexcept(false);
  };

  Card_CalcOp* _pCard_CalcOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
