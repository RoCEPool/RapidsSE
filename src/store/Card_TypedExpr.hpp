/*============================================================================*/
/* Card_TypedExpr.hpp -- Typed Expression                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_TypedExpr : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TypedExpr(void);
   ~Card_TypedExpr(void) noexcept(false);
  };

  Card_TypedExpr* _pCard_TypedExpr(void* aAddr);
  bool _bIsCard_TypedExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
