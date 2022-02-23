/*============================================================================*/
/* Card_CalcExpr.hpp -- Calculation Predicate                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_CalcExpr : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CalcExpr(void);
   ~Card_CalcExpr(void) noexcept(false);
  };

  Card_CalcExpr* _pCard_CalcExpr(void* aAddr);
  bool _bIsCard_CalcExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
