/*============================================================================*/
/* Card_TypedExpr.hpp -- Typed Expression                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_TypedExpr : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TypedExpr(void);
   ~Card_TypedExpr(void) noexcept(false);
    
    Card_ComparePred* pFindBound(Index* pIndex, eBK eBound);
    bool bHasBounds(Index* pIndex);
    void vReduceFilter(void);
    bool bRemoveBound(Card_ComparePred* pCard_ComparePred);
  };

  Card_TypedExpr* _pCard_TypedExpr(void* aAddr);
  bool _bIsCard_TypedExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
