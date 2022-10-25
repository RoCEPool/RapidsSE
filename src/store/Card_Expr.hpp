/*============================================================================*/
/* Card_Expr.hpp -- Typed Expression                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Expr : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Expr(void);
   ~Card_Expr(void) noexcept(false);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
