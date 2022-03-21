/*============================================================================*/
/* Card_ExprCalc.hpp -- Calculation Predicate                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_EXPRESSION.hpp"

namespace STORE
{
  class Card_ExprCalc : public Card_EXPRESSION
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ExprCalc(void);
    Card_ExprCalc(Column* pColumn);
   ~Card_ExprCalc(void) noexcept(false);
  };

  Card_ExprCalc* _pCard_ExprCalc(void* aAddr);
  bool _bIsCard_ExprCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
