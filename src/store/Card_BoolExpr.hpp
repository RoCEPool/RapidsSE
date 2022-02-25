/*============================================================================*/
/* Card_BoolExpr.hpp -- Boolean Predicate                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_EXPR.hpp"

namespace STORE
{
  class Card_BoolExpr : public Card_EXPR
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoolExpr(void);
   ~Card_BoolExpr(void) noexcept(false);
    
    Card_CmpPred* pFindBound(Index* pIndex, eBK eBound);
    bool bHasBounds(Index* pIndex);
    void vReduceExpr(void);
  };

  Card_BoolExpr* _pCard_BoolExpr(void* aAddr);
  bool _bIsCard_BoolExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
