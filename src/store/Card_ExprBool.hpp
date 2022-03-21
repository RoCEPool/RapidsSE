/*============================================================================*/
/* Card_ExprBool.hpp -- Boolean Predicate                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_EXPRESSION.hpp"

namespace STORE
{
  class Card_ExprBool : public Card_EXPRESSION
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ExprBool(void);
   ~Card_ExprBool(void) noexcept(false);
    
    Card_PredCmp* pFindBound(Index* pIndex, eBK eBound);
    bool bHasBounds(Index* pIndex);
    void vReduceExpr(void);
  };

  Card_ExprBool* _pCard_ExprBool(void* aAddr);
  bool _bIsCard_ExprBool(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
