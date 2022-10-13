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
    Card_Expr(Column* pColumn);
   ~Card_Expr(void) noexcept(false);
    
    void vAsEnsure(void);
    void vCullPred(char cWithMark);
    Card_CmpPred* pFindBound(Index* pIndex, ePB eBound);
    bool bHasBounds(Index* pIndex);
    void vReduceBoolExpr(void);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
