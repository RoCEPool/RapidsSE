/*============================================================================*/
/* Card_Expr.hpp -- Typed Expression                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Expr : public Card_SUB
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Expr(void);
    Card_Expr(Column* pColumn);
   ~Card_Expr(void) noexcept(false);
    
    void vAsEnsure(void);
    Card_PredCmp* pFindBound(Index* pIndex, eBK eBound);
    bool bHasBounds(Index* pIndex);
    void vReduceBoolExpr(void);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
  bool _bIsCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
