/*============================================================================*/
/* Card_RelnOp.hpp -- Relational Operator (<|<=|==|!=|>=|>)                   */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_RelnOp : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RelnOp(void);
   ~Card_RelnOp(void) noexcept(false);
  };

  Card_RelnOp* _pCard_RelnOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
